/* Copy propagation on hard registers for accumulate style instruction.
   Copyright (C) 2000-2014 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "regs.h"
#include "addresses.h"
#include "basic-block.h"
#include "reload.h"
#include "function.h"
#include "recog.h"
#include "flags.h"
#include "diagnostic-core.h"
#include "obstack.h"
#include "tree-pass.h"
#include "df.h"
#include "output.h"
#include <vector>

/* For each move instruction, we have a two-dimensional vector that record
   what insns need to replace the operands when the move instruction is
   propagated.  */

typedef std::vector<rtx> insn_list;

/* Function called by note_uses to replace used subexpressions.  */

struct replace_src_operands_data
{
  rtx dst_reg;
  rtx src_reg;
  unsigned int old_regno;
  unsigned int new_regno;
  rtx insn;
};

/* Return true if a mode change from ORIG to NEW is allowed for REGNO.
   Adapted from mode_change_ok in regcprop.  */

static bool
nds32_mode_change_ok (enum machine_mode orig_mode, enum machine_mode new_mode,
		      unsigned int regno ATTRIBUTE_UNUSED)
{
  if (GET_MODE_SIZE (orig_mode) < GET_MODE_SIZE (new_mode))
    return false;

#ifdef CANNOT_CHANGE_MODE_CLASS
  return !REG_CANNOT_CHANGE_MODE_P (regno, orig_mode, new_mode);
#endif

  return true;
}

/* Register REGNO was originally set in ORIG_MODE.  It - or a copy of it -
   was copied in COPY_MODE to COPY_REGNO, and then COPY_REGNO was accessed
   in NEW_MODE.
   Return a NEW_MODE rtx for REGNO if that's OK, otherwise return NULL_RTX.
   Adapted from maybe_mode_change in regcprop.  */

static rtx
nds32_mode_change_reg (enum machine_mode orig_mode, enum machine_mode copy_mode,
		       enum machine_mode new_mode, unsigned int regno,
		       unsigned int copy_regno ATTRIBUTE_UNUSED)
{
  if (GET_MODE_SIZE (copy_mode) < GET_MODE_SIZE (orig_mode)
      && GET_MODE_SIZE (copy_mode) < GET_MODE_SIZE (new_mode))
    return NULL_RTX;

  if (orig_mode == new_mode)
    return gen_rtx_raw_REG (new_mode, regno);
  else if (nds32_mode_change_ok (orig_mode, new_mode, regno))
    {
      int copy_nregs = hard_regno_nregs[copy_regno][copy_mode];
      int use_nregs = hard_regno_nregs[copy_regno][new_mode];
      int copy_offset
	= GET_MODE_SIZE (copy_mode) / copy_nregs * (copy_nregs - use_nregs);
      int offset
	= GET_MODE_SIZE (orig_mode) - GET_MODE_SIZE (new_mode) - copy_offset;
      int byteoffset = offset % UNITS_PER_WORD;
      int wordoffset = offset - byteoffset;

      offset = ((WORDS_BIG_ENDIAN ? wordoffset : 0)
		+ (BYTES_BIG_ENDIAN ? byteoffset : 0));
      regno += subreg_regno_offset (regno, orig_mode, offset, new_mode);
      if (HARD_REGNO_MODE_OK (regno, new_mode))
	return gen_rtx_raw_REG (new_mode, regno);
    }
  return NULL_RTX;
}

/* Return true if INSN is a register-based move instruction, false
   otherwise.  */

static bool
nds32_is_reg_mov_p (rtx insn)
{
  rtx pat = PATTERN (insn);

  if (GET_CODE (pat) != SET)
    return false;

  rtx src_reg = SET_SRC (pat);
  rtx dst_reg = SET_DEST (pat);

  if (REG_P (dst_reg) && REG_P (src_reg) && can_copy_p (GET_MODE (dst_reg)))
    return true;
  else
    return false;
}


/* Return accumulated register if INSN is an accumulate style instruction,
   otherwise return NULL_RTX.  */

static rtx
nds32_is_acc_insn_p (rtx insn)
{
  int i, alt;
  rtx pat;

  if (get_attr_length (insn) != 4)
    return NULL_RTX;

  pat = PATTERN (insn);
  if (GET_CODE (pat) != SET)
    return NULL_RTX;

  /* Try to get the insn data from recog_data.  */
  recog_memoized (insn);
  extract_insn (insn);
  if (!constrain_operands (1))
    fatal_insn_not_found (insn);
  /* Transform the constraint strings into a more usable form,
     recog_op_alt.  */
  preprocess_constraints ();
  alt = which_alternative;

  /* Check all operands whether the output operand is identical to
     another input operand  */
  for (i = 0; i < recog_data.n_operands; ++i)
    {
      int matches = recog_op_alt[i][alt].matches;
      int matched = recog_op_alt[i][alt].matched;
      if ((matches >= 0
	   && (recog_data.operand_type[i] != OP_IN
	       || recog_data.operand_type[matches] != OP_IN))
	  || (matched >= 0
	      && (recog_data.operand_type[i] != OP_IN
		  || recog_data.operand_type[matched] != OP_IN)))
	return recog_data.operand[i];
    }

  return NULL_RTX;
}

/* Finds the reference corresponding to the definition of register whose
   register number is REGNO in INSN. DF is the dataflow object.
   Adapted from df_find_def in df-core.  */

static df_ref
nds32_df_find_regno_def (rtx insn, unsigned int regno)
{
  unsigned int uid;
  df_ref *def_rec;

  uid = INSN_UID (insn);
  for (def_rec = DF_INSN_UID_DEFS (uid); *def_rec; def_rec++)
    {
      df_ref def = *def_rec;
      if (DF_REF_REGNO (def) == regno)
	return def;
    }

  return NULL;
 }

/* Return true if the REG in INSN is only defined by one insn whose uid
   is DEF_UID, otherwise return false.  */

static bool
nds32_is_single_def_p (rtx insn, rtx reg, unsigned int def_uid)
{
  df_ref *use_rec;

  for (use_rec = DF_INSN_USES (insn); *use_rec; use_rec++)
    {
      df_ref use = *use_rec;
      df_link *link;
      unsigned int uid;

      if (DF_REF_REGNO (use) >= REGNO (reg)
	  && DF_REF_REGNO (use) < END_HARD_REGNO (reg))
	{
	  link = DF_REF_CHAIN (use);
	  if (link->next
	      || DF_REF_IS_ARTIFICIAL (link->ref))
	    return false;

	  uid = DF_REF_INSN_UID (link->ref);
	  if (uid != def_uid)
	    return false;
	}
    }

  return true;
}

/* Filtering out the definitions in RD_LOCAL that doesn't define the register
   REG. Keeping the result in RD_FILTER.  */

static void
nds32_filter_reg_def (rtx reg, bitmap rd_local, bitmap rd_filter)
{
  df_ref ref;
  bitmap reg_defs = BITMAP_ALLOC (NULL);
  unsigned int rn;

  for (rn = REGNO (reg); rn < END_HARD_REGNO (reg); ++rn)
    for (ref = DF_REG_DEF_CHAIN (rn); ref; ref = DF_REF_NEXT_REG (ref))
      bitmap_set_bit (reg_defs, DF_REF_ID (ref));

  bitmap_and (rd_filter, rd_local, reg_defs);
  BITMAP_FREE (reg_defs);
}

/* Return true if there is no definition of REG on any path from the insn
   whose uid is FROM_UID (called FROM) to insn TO, otherwise return false.
   This function collects the reaching definitions bitmap at insn TO, and
   check if all uses of REG in insn FROM can reach insn TO.  */

static bool
nds32_no_define_reg_p (rtx to, rtx reg, unsigned int from_uid)
{
  basic_block bb = BLOCK_FOR_INSN (to);
  struct df_rd_bb_info *bb_info = DF_RD_BB_INFO (bb);
  bitmap_head rd_local, rd_filter;
  bitmap from_uses = BITMAP_ALLOC (NULL);
  bool result;
  rtx insn;
  df_ref *use_rec;

  bitmap_initialize (&rd_local, &bitmap_default_obstack);
  bitmap_copy (&rd_local, &bb_info->in);
  df_rd_simulate_artificial_defs_at_top (bb, &rd_local);

  for (insn = BB_HEAD (bb); insn != to; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      df_rd_simulate_one_insn (bb, insn, &rd_local);

  bitmap_initialize (&rd_filter, &bitmap_default_obstack);
  nds32_filter_reg_def (reg, &rd_local, &rd_filter);

  if (dump_file)
    {
      fprintf (dump_file, "scan reach define:");
      print_rtl_single (dump_file, to);

      fprintf (dump_file, "bb rd in:\n");
      dump_bitmap (dump_file, &bb_info->in);

      fprintf (dump_file, "reach def:\n");
      dump_bitmap (dump_file, &rd_local);

      fprintf (dump_file, "filter reach def:\n");
      dump_bitmap (dump_file, &rd_filter);
    }

  for (use_rec = DF_INSN_UID_USES (from_uid); *use_rec; use_rec++)
    {
      df_ref use = *use_rec;
      df_link *link;

      if (DF_REF_REGNO (use) >= REGNO (reg)
	  && DF_REF_REGNO (use) < END_HARD_REGNO (reg))
	for (link = DF_REF_CHAIN (use); link; link = link->next)
	  {
	    if (dump_file)
	      {
		fprintf (dump_file, "use ID %d\n", DF_REF_ID (link->ref));
		if (DF_REF_IS_ARTIFICIAL (link->ref))
		  fprintf (dump_file, "use ref is artificial\n");
		else
		  {
		    fprintf (dump_file, "use from insn:");
		    print_rtl_single (dump_file, DF_REF_INSN (link->ref));
		  }
	      }
	    bitmap_set_bit (from_uses, DF_REF_ID (link->ref));
	  }
    }

  result = bitmap_equal_p (&rd_filter, from_uses);

  bitmap_clear (&rd_local);
  bitmap_clear (&rd_filter);
  BITMAP_FREE (from_uses);
  return result;
}

/* Return true if the value held by REG is no longer needed before INSN
   (i.e. REG is dead before INSN), otherwise return false.  */

static bool
nds32_is_dead_reg_p (rtx insn, rtx reg)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  bitmap live = BITMAP_ALLOC (&reg_obstack);
  bool result = true;
  rtx i;
  unsigned int rn;

  bitmap_copy (live, DF_LR_IN (bb));
  df_simulate_initialize_forwards (bb, live);

  for (i = BB_HEAD (bb); i != insn; i = NEXT_INSN (i))
    df_simulate_one_insn_forwards (bb, i, live);

  if (dump_file)
    {
      fprintf (dump_file, "scan live regs:");
      print_rtl_single (dump_file, insn);

      fprintf (dump_file, "bb lr in:\n");
      dump_bitmap (dump_file, DF_LR_IN (bb));

      fprintf (dump_file, "live:\n");
      dump_bitmap (dump_file, live);
    }

  for (rn = REGNO (reg); rn < END_HARD_REGNO (reg); ++rn)
    result &= (bitmap_bit_p (live, rn)) ? false : true;

  BITMAP_FREE (live);
  return result;
}

/* Return true if START can do propagation. Notice START maybe a move
   instruction or an accumulate style instruction.
   MOV_UID is the uid of beginning move instruction that is only used by
   function nds32_no_define_reg_p.
   DST_REG & SRC_REG is the SET_DEST and SET_SRC of a move instruction that
   maybe real or unreal, respectively.
   INDEX indicates what number sequence is currently considered rank as
   consecutive hard registers. Simultaneously, INDEX is the index of row in
   INSN_LISTS.   */

static bool
nds32_can_cprop_acc_1 (rtx start, unsigned int mov_uid,
		       rtx dst_reg, rtx src_reg,
		       unsigned int index,
		       std::vector<insn_list> &insn_lists)
{
  unsigned int lead_regno = REGNO (dst_reg) + index;
  unsigned int new_regno = REGNO (src_reg) + index;
  df_ref def_rec;
  df_link *link;

  def_rec = nds32_df_find_regno_def (start, lead_regno);
  gcc_assert (def_rec);

  for (link = DF_REF_CHAIN (def_rec); link; link = link->next)
    {
      rtx *use_loc;
      unsigned int use_regno;
      enum machine_mode use_mode;
      rtx use_insn, acc_reg, new_src;

      if (DF_REF_IS_ARTIFICIAL (link->ref))
	return false;

      use_loc = DF_REF_LOC (link->ref);
      gcc_assert (use_loc && REG_P (*use_loc));

      use_regno = REGNO (*use_loc);
      /* Do not propagate when any insns use register that regno is
	 smaller than DST_REG.  */
      if (use_regno < REGNO (dst_reg))
	return false;

      /* This status should be handled by previous call.  */
      if (use_regno < lead_regno)
	continue;

      /* Do not propagate because not all of the pieces of the copy came
	 from DST_REG.  */
      if (END_HARD_REGNO (*use_loc) > END_HARD_REGNO (dst_reg))
	return false;

      use_insn = DF_REF_INSN (link->ref);
      /* Do not propagate since call-used registers can't be replaced.  */
      if (CALL_P (use_insn))
	return false;

      /* Do not replace in asms intentionally referencing hard registers.  */
      if (asm_noperands (PATTERN (use_insn)) >= 0
	  && use_regno == ORIGINAL_REGNO (*use_loc))
	return false;

      /* Do not propagate when the register is defined by more than one
	 instruction.  */
      if (!nds32_is_single_def_p (use_insn, *use_loc, INSN_UID (start)))
	return false;

      use_mode = GET_MODE (*use_loc);
      new_src = nds32_mode_change_reg (GET_MODE (src_reg),
				       GET_MODE (dst_reg),
				       use_mode,
				       new_regno,
				       use_regno);
      /* Do not propagate if we can't generate a new register with new mode.  */
      if (!new_src)
	return false;

      /* Can not replace DST_REG with SRC_REG when SRC_REG is redefined between
	 START and use insn of START.  */
      if (!nds32_no_define_reg_p (use_insn, new_src, mov_uid))
	return false;

      acc_reg = nds32_is_acc_insn_p (use_insn);
      /* Handle the accumulate style instruction that accumulate register
	 may be replaced.
         Also handle the AUTO_INC register that is another form of accumulated
	 register.  */
      if ((acc_reg && rtx_equal_p (acc_reg, *use_loc))
	  || FIND_REG_INC_NOTE (use_insn, *use_loc))
	{
	  unsigned int i, use_nregs;

	  /* ACC_REG can't be replaced since the SRC_REG can't be
	     overwritten.  */
	  if (!nds32_is_dead_reg_p (use_insn, new_src))
	    return false;

	  /* Once we confirm that ACC_REG can be replaced, the unreal move
	     instruction is generated. For example:
	     mov   r0, r1	   mov   r0, r1
	     cmovn r0, r2, r3  ->  cmovn r1, r2, r3
				   mov   r0, r1
	     If the unreal move instruction can do propagation, the ACC_REG
	     can be replaced. We check it in a recursive way.  */
	  use_nregs = hard_regno_nregs [use_regno][(int) use_mode];
	  for (i = 0; i < use_nregs; ++i)
	    if (!nds32_can_cprop_acc_1 (use_insn, mov_uid,
					*use_loc, new_src,
					i, insn_lists))
	      return false;
	}
      insn_lists[index].push_back (use_insn);
    }

  return true;
}

/* Return true if MOV can do propagation, otherwise return false.
   INSN_LISTS is used to record what insns need to replace the operands.  */

static bool
nds32_can_cprop_acc (rtx mov, std::vector<insn_list> &insn_lists)
{
  rtx dst_reg = SET_DEST (PATTERN (mov));
  rtx src_reg = SET_SRC (PATTERN (mov));
  unsigned int dst_regno = REGNO (dst_reg);
  enum machine_mode dst_mode = GET_MODE (dst_reg);
  unsigned int dst_nregs = hard_regno_nregs[dst_regno][(int) dst_mode];
  unsigned int index;

  insn_lists.resize (dst_nregs);
  for (index = 0; index < dst_nregs; ++index)
    if (!nds32_can_cprop_acc_1 (mov, INSN_UID (mov),
				dst_reg, src_reg,
				index, insn_lists))
      return false;

  return true;
}

/* Replace every occurrence of OLD_REGNO in LOC with NEW_REGNO. LOC maybe a
   part of INSN.
   DST_REG & SRC_REG are used by function nds32_mode_change_reg.
   Mark each change with validate_change passing INSN.  */

static void
nds32_replace_partial_operands (rtx *loc, rtx dst_reg, rtx src_reg,
				unsigned int old_regno, unsigned int new_regno,
				rtx insn)
{
  int i, j;
  rtx x = *loc;
  enum rtx_code code;
  const char *fmt;

  if (!x)
    return;

  code = GET_CODE (x);
  fmt = GET_RTX_FORMAT (code);

  if (REG_P (x) && REGNO (x) == old_regno)
    {
      rtx new_reg = nds32_mode_change_reg (GET_MODE (src_reg),
					   GET_MODE (dst_reg),
					   GET_MODE (x),
					   new_regno,
					   old_regno);

      gcc_assert (new_reg);

      ORIGINAL_REGNO (new_reg) = ORIGINAL_REGNO (x);
      REG_ATTRS (new_reg) = REG_ATTRS (x);
      REG_POINTER (new_reg) = REG_POINTER (x);

      /* ??? unshare or not?  */
      validate_change (insn, loc, new_reg, 1);
      return;
    }

  /* Call ourself recursively to perform the replacements.  */
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	nds32_replace_partial_operands (&XEXP (x, i), dst_reg, src_reg,
					old_regno, new_regno, insn);
      else if (fmt[i] == 'E') /* ??? how about V?  */
	for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	  nds32_replace_partial_operands (&XVECEXP (x, i, j), dst_reg, src_reg,
					  old_regno, new_regno, insn);
    }
}

/* Try replacing every occurrence of OLD_REGNO in INSN with NEW_REGNO.  */

static void
nds32_replace_all_operands (rtx dst_reg, rtx src_reg,
			    unsigned int old_regno, unsigned int new_regno,
			    rtx insn)
{
  nds32_replace_partial_operands (&PATTERN (insn), dst_reg, src_reg,
				  old_regno, new_regno, insn);
}

/* Called via note_uses in function nds32_replace_src_operands, for all used
   rtx do replacement.  */

static void
nds32_replace_src_operands_1 (rtx *loc, void *data)
{
  struct replace_src_operands_data *d
    = (struct replace_src_operands_data *) data;

  nds32_replace_partial_operands (loc, d->dst_reg, d->src_reg,
				  d->old_regno, d->new_regno, d->insn);
}

/* Try replacing every occurrence of OLD_REGNO in INSN with NEW_REGNO,
   avoiding SET_DESTs.  */

static void
nds32_replace_src_operands (rtx dst_reg, rtx src_reg,
			    unsigned int old_regno, unsigned int new_regno,
			    rtx insn)
{
  struct replace_src_operands_data d
    = {dst_reg, src_reg, old_regno, new_regno, insn};

  note_uses (&PATTERN (insn), nds32_replace_src_operands_1, &d);
}

/* Try replacing every occurrence of SRC_REG (include its consecutive hard
   registers) in each insn of INSN_LISTS with DST_REG.  */

static bool
nds32_try_replace_operands (rtx dst_reg, rtx src_reg,
			    std::vector<insn_list> &insn_lists)
{
  unsigned int i;
  std::vector<rtx>::iterator ritr;
  unsigned int old_regno, new_regno;

  old_regno = REGNO (dst_reg);
  new_regno = REGNO (src_reg);

  for (i = 0; i < insn_lists.size (); ++i, ++old_regno, ++new_regno)
    for (ritr = insn_lists[i].begin (); ritr != insn_lists[i].end (); ++ritr)
      {
	rtx insn = *ritr;
	rtx acc_reg;

	acc_reg = nds32_is_acc_insn_p (insn);
	if (acc_reg && REGNO (acc_reg) == old_regno)
	  {
	    /* Replace OP_OUT & OP_INOUT  */
	    nds32_replace_all_operands (dst_reg, src_reg,
					old_regno, new_regno, insn);

	  }
	else
	  {
	    /* Replace OP_IN  */
	    nds32_replace_src_operands (dst_reg, src_reg,
					old_regno, new_regno, insn);
	  }
      }

  if (!apply_change_group ())
    return false;
  else
    {
      df_analyze ();
      return true;
    }
}

/* Check if each move instruction in WORK_LIST can do propagation, and
   then try to replace operands if necessary. */

static int
nds32_do_cprop_acc (auto_vec<rtx> &work_list)
{
  int n_replace = 0;
  int i;
  rtx mov;
  std::vector<insn_list> insn_lists;

  FOR_EACH_VEC_ELT (work_list, i, mov)
    {
      if (nds32_can_cprop_acc (mov, insn_lists))
	{
	  if (dump_file)
	    fprintf (dump_file, "\n [CPROP_ACC] insn %d will be cprop. \n",
		     INSN_UID (mov));

	  if (nds32_try_replace_operands (SET_DEST (PATTERN (mov)),
					  SET_SRC (PATTERN (mov)),
					  insn_lists))
	    n_replace++;
	}
      insn_lists.clear ();
    }

  return n_replace;
}

/* Return true if MOV meets the conditions of propagation about move
   instruction, otherwise return false.  */

static bool
nds32_is_target_mov_p (rtx mov)
{
  rtx dst = SET_DEST (PATTERN (mov));
  rtx src = SET_SRC (PATTERN (mov));
  unsigned int dst_regno, src_regno;
  unsigned int dst_nregs, src_nregs;
  bool dst_is_general, src_is_general;

  gcc_assert (REG_P (dst) && REG_P (src));

  dst_regno = REGNO (dst);
  src_regno = REGNO (src);
  dst_nregs = hard_regno_nregs[dst_regno][GET_MODE (dst)];
  src_nregs = hard_regno_nregs[src_regno][GET_MODE (src)];

  /* Do not propagate to the stack pointer, as that can leave memory accesses
     with no scheduling dependency on the stack update.
     Adapted from regcprop.  */
  if (dst_regno == STACK_POINTER_REGNUM)
    return false;

  /* Likewise with the frame pointer, if we're using one.
     Adapted from regcprop.  */
  if (frame_pointer_needed && dst_regno == HARD_FRAME_POINTER_REGNUM)
    return false;

  /* Do not propagate to fixed or global registers, patterns can be relying
     to see particular fixed register or users can expect the chosen global
     register in asm.
     Adapted from regcprop.  */
  if (fixed_regs[dst_regno] || global_regs[dst_regno])
    return false;

  /* Make sure the all consecutive registers of SET_DEST are only defined by
     SET_SRC.  */
  if (dst_nregs > src_nregs)
    return false;

  /* Narrowing on big endian will result in the invalid transformation.  */
  if (dst_nregs < src_nregs
      && (GET_MODE_SIZE (GET_MODE (src)) > UNITS_PER_WORD
	  ? WORDS_BIG_ENDIAN : BYTES_BIG_ENDIAN))
    return false;

  dst_is_general = in_hard_reg_set_p (reg_class_contents[GENERAL_REGS],
				      GET_MODE (dst), REGNO (dst));
  src_is_general = in_hard_reg_set_p (reg_class_contents[GENERAL_REGS],
				      GET_MODE (src), REGNO (src));
  /* Make sure the register class of SET_DEST & SET_SRC are the same.  */
  if (dst_is_general ^ src_is_general)
    return false;

  return true;
}

/* Collect the move instructions that are the uses of accumulated register
   in WORK_LIST */

static void
nds32_cprop_acc_find_target_mov (auto_vec<rtx> &work_list)
{
  basic_block bb;
  rtx insn;
  rtx acc_reg;

  FOR_EACH_BB_FN (bb, cfun)
    FOR_BB_INSNS (bb, insn)
      if (INSN_P (insn))
	{
	  acc_reg = nds32_is_acc_insn_p (insn);
	  if (acc_reg)
	    {
	      unsigned int acc_regno;
	      enum machine_mode acc_mode;
	      df_ref use;
	      df_link *link;
	      rtx def_insn;

	      if (!single_set (insn) || !REG_P (acc_reg))
		continue;

	      acc_regno = REGNO (acc_reg);
	      /* Don't replace in asms intentionally referencing hard regs.  */
	      if (asm_noperands (PATTERN (insn)) >= 0
		  && acc_regno == ORIGINAL_REGNO (acc_reg))
		continue;

	      if (dump_file)
		fprintf (dump_file,
			 "\n [CPROP_ACC] "
			 "RTL_UID %d is an exchangeable ACC insn. \n",
			 INSN_UID (insn));

	      use = df_find_use (insn, acc_reg);
	      gcc_assert (use);
	      link = DF_REF_CHAIN (use);

	      if ((link == NULL) || link->next
		  || DF_REF_IS_ARTIFICIAL (link->ref))
		continue;

	      acc_mode = GET_MODE (acc_reg);
	      def_insn = DF_REF_INSN (link->ref);
	      if (nds32_is_reg_mov_p (def_insn))
		{
		  rtx *loc = DF_REF_LOC (link->ref);
		  enum machine_mode loc_mode = GET_MODE (*loc);

		  /* If the move instruction can't define whole accumulated
		     register, the replacement is invalid.  */
		  if (loc_mode != acc_mode)
		    if (hard_regno_nregs[acc_regno][acc_mode]
			> hard_regno_nregs[acc_regno][loc_mode])
		      continue;

		  if (nds32_is_target_mov_p (def_insn))
		    work_list.safe_push (def_insn);
		}
	    }
	}
}

/* Main entry point for the forward copy propagation optimization for
   accumulate style instruction.  */

static int
nds32_cprop_acc_opt (void)
{
  df_chain_add_problem (DF_DU_CHAIN + DF_UD_CHAIN);
  df_note_add_problem ();
  df_set_flags (DF_RD_PRUNE_DEAD_DEFS);
  df_insn_rescan_all ();
  df_analyze ();

  auto_vec<rtx> work_list;

  nds32_cprop_acc_find_target_mov (work_list);
  if (work_list.is_empty())
    {
      if (dump_file)
	fprintf (dump_file, "\n [CPROP_ACC] The work_list is empty. \n");
      return 0;
    }

  if (dump_file)
    {
      int i;
      rtx mov;

      fprintf (dump_file, "\n [CPROP_ACC] The content of work_list:");
      FOR_EACH_VEC_ELT (work_list, i, mov)
	fprintf (dump_file, " %d", INSN_UID (mov));
      fprintf (dump_file, "\n");
    }

  compute_bb_for_insn ();

  int n_replace = nds32_do_cprop_acc (work_list);

  if (dump_file)
    {
      fprintf (dump_file, "\n [CPROP_ACC] Result: ");
      if (n_replace == 0)
	fprintf (dump_file, "No move can do cprop. \n");
      else
	fprintf (dump_file, "Do cprop for %d move. \n", n_replace);
    }

  work_list.release ();
  return 1;
}

const pass_data pass_data_nds32_cprop_acc_opt =
{
  RTL_PASS,                                     /* type */
  "cprop_acc",                                  /* name */
  OPTGROUP_NONE,                                /* optinfo_flags */
  true,                                         /* has_gate */
  true,                                         /* has_execute */
  TV_MACH_DEP,                                  /* tv_id */
  0,                                            /* properties_required */
  0,                                            /* properties_provided */
  0,                                            /* properties_destroyed */
  0,                                            /* todo_flags_start */
  ( TODO_df_finish | TODO_verify_rtl_sharing ), /* todo_flags_finish */
};

class pass_nds32_cprop_acc_opt : public rtl_opt_pass
{
public:
  pass_nds32_cprop_acc_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_cprop_acc_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return optimize > 0 && flag_nds32_cprop_acc; }
  unsigned int execute () { return nds32_cprop_acc_opt (); }
};

rtl_opt_pass *
make_pass_nds32_cprop_acc_opt (gcc::context *ctxt)
{
  return new pass_nds32_cprop_acc_opt (ctxt);
}
