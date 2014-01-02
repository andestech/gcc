/* Global CSE pass of Andes NDS32 cpu for GNU compiler
   Copyright (C) 2012-2015 Free Software Foundation, Inc.
   Contributed by Andes Technology Corporation.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* ------------------------------------------------------------------------ */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "diagnostic-core.h"

#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "basic-block.h"
#include "function.h"
#include "expr.h"
#include "except.h"
#include "intl.h"
#include "obstack.h"
#include "hashtab.h"
#include "params.h"
#include "target.h"
#include "tree-pass.h"
#include "dbgcnt.h"
#include "df.h"
#include "tm-constrs.h"

/* ------------------------------------------------------------------------ */

typedef struct reg_avail_info
{
  rtx insn;
  unsigned int uint;
  unsigned int regno;
} reg_avail_info_t;


static void find_common_const (void);
static bool try_rematerialize (rtx, unsigned int,
			       auto_vec<reg_avail_info_t, 32> *);
static void clean_reg_avail_info (rtx ,const_rtx, void *);
static rtx get_const (rtx);
static bool addsi3_format_p (rtx);

/* Search the register records.  */
static bool
try_rematerialize (rtx insn, unsigned int uint_r,
		   auto_vec<reg_avail_info_t, 32> *reg_avail_infos)
{
  unsigned int i, uint_i, cl_i, cl_r, ct_i, ct_r;
  rtx pat, src, dest, new_insn;
  bool done = FALSE;
  df_ref *df_rec;
  df_link *link;

  cl_r = __builtin_clz (uint_r);
  ct_r = __builtin_ctz (uint_r);
  for (i = 0; i < reg_avail_infos->length (); ++i)
    {
      if ((*reg_avail_infos)[i].uint != uint_r)
	{
	  uint_i = (*reg_avail_infos)[i].uint;
	  if (dump_file)
	    fprintf (dump_file, "Try rematerialize %08x with const %08x\n",
		     uint_r, uint_i);
	  cl_i = __builtin_clz (uint_i);
	  ct_i = __builtin_ctz (uint_i);
	  src = SET_DEST (PATTERN ((*reg_avail_infos)[i].insn));
	  dest = SET_DEST (PATTERN (insn));

	  if (cl_r > cl_i
	      && (uint_i >> (cl_r - cl_i)) == uint_r)
	    {
	      /* Right shift logical.  */
	      pat = gen_rtx_LSHIFTRT (SImode, src, GEN_INT (cl_r - cl_i));
	      done = TRUE;
	      if (dump_file)
		fprintf (dump_file,
			 "Rematerialize %08x with const %08x by l>> %d\n",
			 uint_r, uint_i, (cl_r - cl_i));
	    }
	  else if (ct_i >= ct_r
		   && ((int) uint_i >> (ct_i - ct_r)) == (int) uint_r)
	    {
	      /* Right shift arithmetic.  */
	      pat = gen_rtx_ASHIFTRT (SImode, src, GEN_INT (ct_i - ct_r));
	      done = TRUE;
	      if (dump_file)
		fprintf (dump_file,
			 "Rematerialize %08x with const %08x by a>> %d\n",
			 uint_r, uint_i, (cl_r - cl_i));
	    }
	  else if (ct_r > ct_i
		   && (uint_i << (ct_r - ct_i)) == uint_r)
	    {
	      /* Left shift.  */
	      pat = gen_rtx_ASHIFT (SImode, src, GEN_INT (ct_r - ct_i));
	      done = TRUE;
	      if (dump_file)
		fprintf (dump_file,
			 "Rematerialize %08x with const %08x by << %d\n",
			uint_r, uint_i, (ct_r - ct_i));
	    }
	  else if (TARGET_EXT_PERF && __builtin_popcount (uint_r ^ uint_i) == 1)
	    {
	      unsigned int val = uint_r ^ uint_i;
	      if ((uint_r & (uint_r ^ uint_i)) != 0)
		{
		  if (val > (1 << 5))
		    {
		      /* Bit set.  */
		      pat =
			gen_rtx_IOR (SImode, src,
				     GEN_INT (trunc_int_for_mode (val, GET_MODE (dest))));
		      done = TRUE;
		      if (dump_file)
			fprintf (dump_file,
				 "Rematerialize %08x with const %08x by | %08x\n",
				 uint_r, uint_i, uint_r ^ uint_i);
		    }
		  else
		    {
		      /* Transform to plus if immediate can fit addi45.  */
		      pat = gen_rtx_PLUS (SImode, src, GEN_INT (val));
		      done = TRUE;
		      if (dump_file)
			fprintf (dump_file,
				 "Rematerialize %08x with const %08x by | %08x\n",
				 uint_r, uint_i, uint_r ^ uint_i);
		    }
		}
	      else
		{
		  if (val > (1 << 5))
		    {
		      /* Bit clear.  */
		      pat = gen_rtx_AND (SImode, src, GEN_INT (~(uint_r ^ uint_i)));
		      done = TRUE;
		      if (dump_file)
			fprintf (dump_file,
				 "Rematerialize %08x with const %08x by & %08x\n",
				 uint_r, uint_i, ~(uint_r ^ uint_i));
		    }
		  else
		    {
		      /* Transform to plus if immediate can fit subi45.  */
		      pat = gen_rtx_PLUS (SImode, src, GEN_INT ((int) -val));
		      done = TRUE;
		      if (dump_file)
			fprintf (dump_file,
				 "Rematerialize %08x with const %08x by | %08x\n",
				 uint_r, uint_i, uint_r ^ uint_i);
		    }
		}
	    }
	  else if  ((uint_r > uint_i ? uint_r - uint_i
		     : uint_i - uint_r) < 0x4000)
	    {
	      /* Check insn_info existence because the instruction
		 maybe be deleted.*/
	      if (DF_INSN_INFO_GET ((*reg_avail_infos)[i].insn))
		{
		  df_rec = DF_INSN_DEFS ((*reg_avail_infos)[i].insn);
		  link = DF_REF_CHAIN (*df_rec);

		  /* Do not use the dead instruction. */
		  /* Do not use the original matched sethi.  */
		  if (!link)
		    continue;
		  for (link = DF_REF_CHAIN (*df_rec); link; link = link->next)
		    {
		      if (DF_REF_REGNO (link->ref) == 0
			  || !DF_REF_INSN_INFO (link->ref)
			  || DF_REF_INSN (link->ref) == insn)
			break;
		    }
		  if (link)
		    continue;
		}

	      /* Add.  */
	      if (uint_r > uint_i)
		{
		  pat = gen_rtx_PLUS (SImode, src, GEN_INT (uint_r - uint_i));
		  done = TRUE;
		}
	      else
		{
		  pat = gen_rtx_PLUS (SImode, src, GEN_INT ((HOST_WIDE_INT)
							    uint_r - uint_i));
		  done = TRUE;
		}
	    }

	  if (done)
	    {
	      /* Emit the new instruction.  */
	      new_insn = gen_move_insn (dest, pat);
	      emit_insn_before (new_insn, insn);
	      set_dst_reg_note (new_insn, REG_EQUAL, GEN_INT (uint_r), dest);
	      return TRUE;
	    }
	}
    }
  return FALSE;
}

/* Clean the reg_avail_info value.  */
static void
clean_reg_avail_info (rtx dest, const_rtx setter ATTRIBUTE_UNUSED,
		      void *data)
{
  unsigned int i;
  auto_vec<reg_avail_info_t, 32> *reg_avail_infos =
    (auto_vec<reg_avail_info_t, 32> *) data;

  if (GET_CODE (dest) == SUBREG)
    dest = SUBREG_REG (dest);

  if (REG_P (dest))
    for (i = 0; i < reg_avail_infos->length (); ++i)
      if ((*reg_avail_infos)[i].regno == REGNO (dest)
	  || (GET_MODE_SIZE (GET_MODE (dest)) == 8
	      && (*reg_avail_infos)[i].regno == REGNO (dest) + 1))
	reg_avail_infos->unordered_remove (i--);
}

/* Return the const if the setting value is a constant integer.  */
static rtx
get_const (rtx insn)
{
  rtx note;

  if (GET_CODE (PATTERN (insn)) != SET
      || !REG_P (SET_DEST (PATTERN (insn)))
      || GET_MODE (SET_DEST (PATTERN (insn))) != SImode)
    return NULL_RTX;

  /* Constant move instruction.  */
  if (CONST_INT_P (XEXP (PATTERN (insn), 1)))
    return XEXP (PATTERN (insn), 1);

  note = find_reg_note (insn, REG_EQUAL, NULL_RTX);
  if (!note)
    note = find_reg_note (insn, REG_EQUIV, NULL_RTX);

  if (note && CONST_INT_P (XEXP (note, 0)))
    return XEXP (note, 0);

  return NULL_RTX;
}

/* Return true if the instruction is addi format.  */
static bool
addsi3_format_p (rtx insn)
{
  if (GET_CODE (XEXP (PATTERN (insn), 1)) == PLUS
      && GET_CODE (XEXP (XEXP (PATTERN (insn), 1), 1)) == CONST_INT)
    return TRUE;

  return FALSE;
}

/* Return true if the instruction is sethi format.  */
static bool
sethi_format_p (rtx insn)
{
  if (GET_CODE (PATTERN (insn)) == SET
      && GET_CODE (XEXP (PATTERN (insn), 1)) == CONST_INT
      && satisfies_constraint_Ihig (XEXP (PATTERN (insn), 1)))
    return TRUE;
  return FALSE;
}

/* Return true if the register definition only be used by insn.  */
static bool
use_only_p (rtx insn)
{
  rtx def_insn;
  df_ref *rec;
  df_link *link;
  rec = DF_INSN_USES (insn);
  link = DF_REF_CHAIN (*rec);

  if (!link
      || DF_REF_REGNO (link->ref) == 0
      || !DF_REF_INSN_INFO (link->ref))
    return FALSE;

  def_insn = DF_REF_INSN (link->ref);

  if (!sethi_format_p (def_insn))
    return FALSE;

  rec = DF_INSN_DEFS (def_insn);
  link = DF_REF_CHAIN (*rec);

  if (!link
      || link->next
      || DF_REF_REGNO (link->ref) == 0
      || !DF_REF_INSN_INFO (link->ref))
    return FALSE;

  return TRUE;
}

/* Traverse instructions in each basic block, and save the value of
   setting constant instructions.  */
static void
find_common_const (void)
{
  basic_block bb;
  unsigned int i;

  /* Save register constant value.  */
  auto_vec<reg_avail_info_t, 32> reg_avail_infos;
  reg_avail_info_t reg_avail_info;


  FOR_EACH_BB_FN (bb, cfun)
    {
      rtx insn, dest, cst;

      /* Clear the vector.  */
      while (!reg_avail_infos.is_empty ())
	reg_avail_infos.pop ();

      FOR_BB_INSNS (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  if (CALL_P (insn))
	    {
	      /* Clean hard register.  */
	      for (i = 0; i < reg_avail_infos.length ();)
		{
		  if (HARD_REGISTER_NUM_P (reg_avail_infos[i].regno)
		      && call_used_regs[reg_avail_infos[i].regno])
		    reg_avail_infos.unordered_remove (i);
		  else
		    ++i;
		}
	    }

	  cst = get_const (insn);
	  if (cst == NULL_RTX)
	    {
	      note_stores (PATTERN (insn), clean_reg_avail_info,
			   &reg_avail_infos);
	      continue;
	    }

	  dest = SET_DEST (PATTERN (insn));

	  if (addsi3_format_p (insn)
	      && use_only_p (insn)
	      && try_rematerialize (insn, XUINT (cst, 0), &reg_avail_infos))
	    {
	      delete_insn (insn);
	      df_insn_rescan_all ();
	    }

	  note_stores (PATTERN (insn), clean_reg_avail_info, &reg_avail_infos);
	  reg_avail_info.insn = insn;
	  reg_avail_info.uint = XUINT (cst, 0);
	  reg_avail_info.regno = REGNO (dest);
	  if (dump_file)
	    fprintf (dump_file, "Find const %08x on %u\n",
		     reg_avail_info.uint, reg_avail_info.regno);
	  reg_avail_infos.safe_push (reg_avail_info);
	}
    }
}

static unsigned int
nds32_const_remater_opt (void)
{
  df_chain_add_problem (DF_DU_CHAIN + DF_UD_CHAIN);
  df_note_add_problem ();
  df_insn_rescan_all ();
  df_analyze ();

  find_common_const ();

  df_insn_rescan_all ();
  return 0;
}

const pass_data pass_data_nds32_const_remater_opt =
{
  RTL_PASS,				/* type */
  "const_remater_opt",				/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  true,					/* has_gate */
  true,					/* has_execute */
  TV_MACH_DEP,				/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  (TODO_df_finish | TODO_verify_rtl_sharing),	/* todo_flags_finish */
};

class pass_nds32_const_remater_opt : public rtl_opt_pass
{
public:
  pass_nds32_const_remater_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_const_remater_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return flag_nds32_const_remater_opt; }
  unsigned int execute () { return nds32_const_remater_opt (); }
};

rtl_opt_pass *
make_pass_nds32_const_remater_opt (gcc::context *ctxt)
{
  return new pass_nds32_const_remater_opt (ctxt);
}

/* ------------------------------------------------------------------------ */
