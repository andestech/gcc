
/* lmwsmw pass of Andes NDS32 cpu for GNU compiler
   Copyright (C) 2012-2013 Free Software Foundation, Inc.
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
#include "tree.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"	/* Required by recog.h.  */
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"		/* For DFA state_t.  */
#include "insn-codes.h"		/* For CODE_FOR_xxx.  */
#include "reload.h"		/* For push_reload().  */
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "df.h"
#include "tm_p.h"
#include "tm-constrs.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"		/* For add_builtin_function().  */
#include "ggc.h"
#include "tree-pass.h"
#include "target-globals.h"
#include "ira-int.h"
#include "regrename.h"
#include "nds32-load-store-opt.h"
#include "nds32-reg-utils.h"
#include <set>
#include <vector>
#include <algorithm>

#define NDS32_GPR_NUM 32

static int
compare_order (const void *a, const void *b)
{
  const load_store_info_t *fp1 = (const load_store_info_t *) a;
  const load_store_info_t *fp2 = (const load_store_info_t *) b;
  const load_store_info_t f1 = *fp1;
  const load_store_info_t f2 = *fp2;

  return f1.order < f2.order ? -1 : 1;
}

static int
compare_offset (const void *a, const void *b)
{
  const load_store_info_t *fp1 = (const load_store_info_t *) a;
  const load_store_info_t *fp2 = (const load_store_info_t *) b;
  const load_store_info_t f1 = *fp1;
  const load_store_info_t f2 = *fp2;

  return f1.offset < f2.offset ? -1 : 1;
}

static bool
compare_amount(available_reg_info_t a, available_reg_info_t b)
{
    return a.amount > b.amount;
}

static bool
nds32_load_store_reg_plus_offset (rtx insn, load_store_info_t *load_store_info)
{
  rtx pattern, mem, reg, base_reg, addr;
  HOST_WIDE_INT offset;
  bool load_p;
  enum nds32_memory_post_type post_type = NDS32_NONE;

  pattern = PATTERN (insn);
  mem = NULL_RTX;
  reg = NULL_RTX;
  base_reg = NULL_RTX;
  offset = 0;
  load_p = false;

  if (GET_CODE (pattern) != SET)
    return false;

  if (MEM_P (SET_SRC (pattern)))
    {
      mem = SET_SRC (pattern);
      reg = SET_DEST (pattern);
      load_p = true;
    }

  if (MEM_P (SET_DEST (pattern)))
    {
      mem = SET_DEST (pattern);
      reg = SET_SRC (pattern);
      load_p = false;
    }

  if (mem == NULL_RTX || reg == NULL_RTX || !REG_P (reg))
    return false;

  /* The FPU ISA has not load-store-multiple instruction.  */
  if (!NDS32_IS_GPR_REGNUM (REGNO (reg)))
    return false;

  if (MEM_VOLATILE_P (mem))
    return false;

  if (GET_MODE (reg) != SImode)
    return false;

  gcc_assert (REG_P (reg));

  addr = XEXP (mem, 0);

  /* We only care about [reg] and [reg+const].  */
  if (REG_P (addr))
    {
      base_reg = addr;
      offset = 0;
    }
  else if (GET_CODE (addr) == PLUS
	   && CONST_INT_P (XEXP (addr, 1)))
    {
      base_reg = XEXP (addr, 0);
      offset = INTVAL (XEXP (addr, 1));
      if (!REG_P (base_reg))
	return false;
    }
  else if (GET_CODE (addr) == POST_INC)
    {
      base_reg = XEXP (addr, 0);
      offset = 0;
      post_type = NDS32_POST_INC;
    }
  else if (GET_CODE (addr) == POST_DEC)
    {
      base_reg = XEXP (addr, 0);
      offset = 0;
      post_type = NDS32_POST_DEC;
    }
  else
    return false;

  if ((REGNO (base_reg) > NDS32_LAST_GPR_REGNUM)
      && (REGNO (base_reg) < FIRST_PSEUDO_REGISTER))
    return false;

  if (load_store_info)
    {
      load_store_info->load_p   = load_p;
      load_store_info->offset   = offset;
      load_store_info->reg      = reg;
      load_store_info->base_reg = base_reg;
      load_store_info->insn     = insn;
      load_store_info->mem      = mem;
      load_store_info->post_type = post_type;
    }

  return true;
}

static bool
nds32_insn_alias_p (rtx memref, rtx x)
{
  rtx mem;

  if (GET_CODE (x) == PARALLEL)
    {
      int i, j;

      for (i = GET_RTX_LENGTH (GET_CODE (x)) - 1; i >= 0; i--)
	{
	  for (j = XVECLEN (x, i) - 1; j >= 0; j--)
	    if (nds32_insn_alias_p (memref, XVECEXP (x, i, j)))
	      return true;
	}

      return false;
    }

  if (GET_CODE (x) != SET)
    return true;

  if (MEM_P (SET_SRC (x)))
    mem = SET_SRC (x);
  else if (MEM_P (SET_DEST (x)))
    mem = SET_DEST (x);
  else
    return false;

  if (may_alias_p (memref, mem))
    return true;
  else
    return false;
}

static void
nds32_emit_multiple_insn (load_store_infos_t *multiple_insn,
			  rtx base_reg, rtx place, bool update_p)
{
  unsigned int i;
  unsigned int num_use_regs = multiple_insn->length ();
  int par_index = 0;
  int offset = 0;
  bool load_p = (*multiple_insn)[0].load_p;

  rtx reg;
  rtx mem;
  rtx push_rtx;
  rtx update_offset;
  rtx parallel_insn;

  /* In addition to used registers,
     we need one more space for (set base base-x) rtx.  */
  if (update_p)
    num_use_regs++;

  parallel_insn = gen_rtx_PARALLEL (VOIDmode,
				    rtvec_alloc (num_use_regs));

  /* Set update insn.  */
    if (update_p)
      {
	update_offset = GEN_INT (multiple_insn->length () * 4);
	push_rtx = gen_addsi3 (base_reg, base_reg, update_offset);
	XVECEXP (parallel_insn, 0, par_index) = push_rtx;
	par_index++;
      }

  /* Create (set mem regX) from start_reg to end_reg.  */
  for (i = 0; i < multiple_insn->length (); ++i)
    {
      reg = (*multiple_insn)[i].reg;
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
						  base_reg,
						  offset));
      MEM_COPY_ATTRIBUTES (mem, (*multiple_insn)[i].mem);

      if (load_p)
	push_rtx = gen_rtx_SET (VOIDmode, reg, mem);
      else
	push_rtx = gen_rtx_SET (VOIDmode, mem, reg);

      XVECEXP (parallel_insn, 0, par_index) = push_rtx;
      offset = offset + 4;
      par_index++;
    }

  emit_insn_before (parallel_insn, place);

  if (dump_file)
    {
      fprintf (dump_file, "lmw/smw instruction:\n");
      print_rtl_single (dump_file, parallel_insn);
    }
}

static void
nds32_emit_add_insn (load_store_info_t insn, rtx base_reg,
		     rtx place, bool add_p)
{
  rtx add_insn;
  HOST_WIDE_INT offset = insn.offset;
  if (!add_p)
    offset = -offset;

  add_insn = gen_addsi3 (base_reg, insn.base_reg, GEN_INT (offset));
  emit_insn_before (add_insn, place);
}

/* Get the instruction of same ID.  */
static void
nds32_fetch_group_insn (load_store_infos_t *src,
			load_store_infos_t *dst, int id)
{
  unsigned int i = 0;

  while (i < src->length ())
    {
      if (id == (*src)[i].group)
	{
	  dst->safe_push ((*src)[i]);
	  src->ordered_remove (i);
	  i = 0;
	}
      else
	i++;
    }
}

/* Check registers are not used and defined.  */
static rtx
nds32_lmwsmw_insert_place (load_store_infos_t *insn_set)
{
  unsigned int i, position;
  bool combine_p;
  rtx insn;
  auto_vec<load_store_info_t, 64> temp_set;

  for (i = 0; i < insn_set->length (); i++)
    temp_set.safe_push ((*insn_set)[i]);

  /* Check registers are not used and defined
     between first instruction and last instruction,
     and find insert lmw/smw instruction place.
       example:
	 lwi $r0, [$r2 + 4]
	 lwi $r1, [$r2 + 8]

     Check $r0 and $r1 are not used and defined.  */
  temp_set.qsort (compare_order);

  for (position = 0; position < temp_set.length (); ++position)
    {
      combine_p = true;

      /* Check instruction form first instruction to position.  */
      for (i = 0; i < position; i++)
	{
	  for (insn = NEXT_INSN (temp_set[i].insn);
	       insn != temp_set[position].insn;
	       insn = NEXT_INSN (insn))
	    {
	      if (!NONDEBUG_INSN_P (insn))
		continue;
	      if (df_reg_used (insn, temp_set[i].reg)
		  || df_reg_defined (insn, temp_set[i].reg))
		{
		  if (dump_file)
		    {
		      fprintf (dump_file, "Fail:register has modify\n");
		      fprintf (dump_file, "insn uid:%d, reg: r%d,\n",
			       INSN_UID (temp_set[position].insn),
			       REGNO (temp_set[position].reg));
		      fprintf (dump_file, "Modify instruction:\n");
		      print_rtl_single (dump_file, insn);
		    }
		  combine_p = false;
		  break;
		}
	    }
	}

      /* Check instruction form position to last instruction.  */
      for (i = position + 1; i < temp_set.length (); i++)
	{
	  for (insn = temp_set[position].insn;
	       insn != temp_set[i].insn;
	       insn = NEXT_INSN (insn))
	    {
	      if (!NONDEBUG_INSN_P (insn))
		continue;
	      if (df_reg_used (insn, temp_set[i].reg)
		  || df_reg_defined (insn, temp_set[i].reg))
		{
		  if (dump_file)
		    {
		      fprintf (dump_file, "Fail:register has modify\n");
		      fprintf (dump_file, "insn uid:%d, reg: r%d,\n",
			       INSN_UID (temp_set[position].insn),
			       REGNO (temp_set[position].reg));
		      fprintf (dump_file, "Modify instruction:\n");
		      print_rtl_single (dump_file, insn);
		    }
		  combine_p = false;
		  break;
		}
	    }
	}

      if (combine_p)
	return temp_set[position].insn;
    }

  return NULL_RTX;
}

/* Check registers are not used and defined.  */
static bool
nds32_base_reg_safe_p (load_store_infos_t *insn_set)
{
  unsigned int i;
  rtx insn;
  auto_vec<load_store_info_t, 64> temp_set;

  /* We will change 'insn_set' element order,
     to avoid change order using 'temp_set'.  */
  for (i = 0; i < insn_set->length (); i++)
    temp_set.safe_push ((*insn_set)[i]);

  /* We want to combine load and store instructions,
     need to check base register is not used and defined
     between first insn and last insn.
     example:
       lwi $r0, [$r3 + 4]
	    ...		  <- check here
       lwi $r1, [$r3 + 8]
	    ...		  <- check here
       lwi $r2, [$r3 + 12]

     Check $r3 is not used and defined,
     between first insn and last insn.  */

  /* Scan instruction from top to bottom,
     so need to sort by order.  */
  temp_set.qsort (compare_order);

  for (i = 0; i < temp_set.length () - 1; ++i)
    {
      for (insn = NEXT_INSN (temp_set[i].insn);
	   insn != temp_set[i + 1].insn;
	   insn = NEXT_INSN (insn))
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  if (nds32_insn_alias_p (temp_set[0].mem, PATTERN (insn)))
	    {
	      if (dump_file)
		{
		  fprintf (dump_file, "Memory alias:\n");
		  print_rtl_single (dump_file, insn);
		}
	      return false;
	    }

	  if (temp_set[0].load_p)
 	    {
	      if (df_reg_defined (insn, temp_set[0].base_reg))
 		{
		  if (dump_file)
		    {
		      fprintf (dump_file, "Fail: base register has modify\n");
		      fprintf (dump_file, "insn uid:%d, base reg: r%d,\n",
			       INSN_UID (temp_set[i].insn),
			       REGNO (temp_set[i].reg));
		      fprintf (dump_file, "Modify instruction:\n");
		      print_rtl_single (dump_file, insn);
		    }
		  return false;
		}
	    }
	  else
	    {
	      if (df_reg_used (insn, temp_set[0].base_reg))
		{
		  if (dump_file)
		    {
		      fprintf (dump_file, "Fail: base register has modify\n");
		      fprintf (dump_file, "insn uid:%d, base reg: r%d,\n",
			       INSN_UID (temp_set[i].insn),
			       REGNO (temp_set[i].reg));
		      fprintf (dump_file, "Modify instruction:\n");
		      print_rtl_single (dump_file, insn);
		    }
		  return false;
 		}
 	    }
	}
    }
  return true;
}

static bool
nds32_gain_size_p (load_store_infos_t *insn, bool new_base_p)
{
  unsigned int i, new_cost = 4, old_cost = 0;
  rtx reg;
  rtx base_reg = (*insn)[0].base_reg;
  HOST_WIDE_INT offset;

  for (i = 0; i < insn->length (); ++i)
    {
      reg = (*insn)[i].reg;
      offset = (*insn)[i].offset;

      if (in_reg_class_p (reg, LOW_REGS))
	{
	  /* lwi37.sp/swi37.sp/lwi37/swi37 */
	  if ((REGNO (base_reg) == SP_REGNUM
	      || REGNO (base_reg) == FP_REGNUM)
	      && (offset >= 0 && offset < 512 && (offset % 4 == 0)))
	    old_cost += 2;
	  /* lwi333/swi333 */
	  else if (in_reg_class_p (base_reg, LOW_REGS)
		   && (offset >= 0 && offset < 32 && (offset % 4 == 0)))
	    old_cost += 2;
	  else
	    old_cost += 4;
        }
      else
	{
	  /* lwi450/swi450 */
	  if (in_reg_class_p (reg, MIDDLE_REGS)
	      && offset == 0)
	    old_cost += 2;
	  else
	    old_cost += 4;
	}
    }

  offset = (*insn)[0].offset;
  if (offset != 0)
    {
      /* addi333 */
      if (in_reg_class_p (base_reg, LOW_REGS)
	  && satisfies_constraint_Iu05 (GEN_INT (offset)))
	new_cost += 2;
      /* addi45 */
      else if (in_reg_class_p (base_reg, MIDDLE_REGS)
	       && satisfies_constraint_Iu05 (GEN_INT (offset)))
	new_cost += 2;
      else
	new_cost += 4;

      /* subri */
      if (!new_base_p)
	new_cost += 4;
    }

  if (dump_file)
    fprintf (dump_file, "Code size compare: old code size is %d,"
			" new code size is %d\n", old_cost, new_cost);

  return new_cost < old_cost;
}

static bool
nds32_gain_speed_p (load_store_infos_t *insn, bool new_base_p)
{
  unsigned int new_cost = 0, old_cost = insn->length ();

  if (TARGET_PIPELINE_GRAYWOLF)
    {
      new_cost = insn->length () / 2 + insn->length () % 2;

      if ((*insn)[0].offset != 0)
	{
	  /* Need addi instruction. */
	  new_cost += 1;

	  /* Need subri instruction. */
	  if (!new_base_p)
	    new_cost += 1;
	}
    }
  else
    {
      if ((*insn)[0].offset != 0)
	return false;
    }

  return new_cost < old_cost;
}

/* Check instructions can combine into a mulitple-instruction.  */
static bool
nds32_combine_multiple_p (load_store_infos_t *insn_set, bool new_base_p)
{
  unsigned int i;
  auto_vec<load_store_info_t, 64> temp_set;

  /* We will change 'insn_set' element order,
     to avoid change order using 'temp_set'.  */
  for (i = 0; i < insn_set->length (); i++)
    temp_set.safe_push ((*insn_set)[i]);

  /* Check start offset need to sort by offset.  */
  temp_set.qsort (compare_offset);

  /* The lmw/smw pattern, need two or more instructions.  */
  if (temp_set.length () < 2)
    return false;

  /* The lmw/smw pattern, only allow combine 25 instruction.  */
  if (temp_set.length () > 25)
    return false;

  if (TARGET_LMWSMW_OPT_SIZE
      || (TARGET_LMWSMW_OPT_AUTO && optimize_size))
    {
      /* Compare original instructions with multiple instruction,
	 when mupltiple instruction is small than original instructions
	 then combine it.  */
      if (!nds32_gain_size_p (&temp_set, new_base_p))
	return false;
    }
  else if (TARGET_LMWSMW_OPT_SPEED
	   || (TARGET_LMWSMW_OPT_AUTO && !optimize_size))
    {
      /* The start offset is not zero, we need add a instrucion
	 to handle offset, it is not worth on -O3, -O2 level.  */
      if (!nds32_gain_speed_p (&temp_set, new_base_p))
	return false;
    }

  /* Base register is not equal register, when offset is not zero.  */
  if (temp_set[0].offset != 0)
    for (i = 0; i < temp_set.length (); ++i)
      {
	if (REGNO (temp_set[i].reg)
	    == REGNO (temp_set[0].base_reg))
	  return false;
      }

  /* Don't combine, when start offset is greater then Is15,
     because need extra register.  */
  if (!satisfies_constraint_Is15 (GEN_INT (temp_set[0].offset)))
    return false;

  return true;
}

static bool
nds32_use_bim_p (load_store_infos_t *insn_set,
		 load_store_infos_t *ref_set)
{
  rtx insn;
  bool combine_p = true;

  /* Generate .bim form, need offset is continuous.  */
  if (insn_set->last ().offset != ((*ref_set)[0].offset - 4))
    return false;

  /* Reject 'insn_set' instructions bottom
     of the 'ref_set' instructions.  */
  if ((*insn_set)[0].group > (*ref_set)[0].group)
    return false;

  /* Scan instruction from top to bottom,
     so need to sort by order.  */
  insn_set->qsort (compare_order);
  ref_set->qsort (compare_order);

  /* We want to combine .bim form instruction,
     so need to check base register is not used and defined
     between multiple-insn and next mulitple-insn.
     example:
      lmw.bim $r0, [$r2], $r1
		...		       <- check here
      lmw.bi  $r3, [$r2], $r4

    Use .bim form need to check $r2 is not used and defined,
    between lmw.bim and lmw.bi.  */
    for (insn = NEXT_INSN (insn_set->last ().insn);
	 insn != (*ref_set)[0].insn;
	 insn = NEXT_INSN (insn))
      {
	if (!NONDEBUG_INSN_P (insn))
	  continue;

	if (nds32_insn_alias_p ((*insn_set)[0].mem, PATTERN (insn)))
	  {
	    if (dump_file)
	      {
		fprintf (dump_file, "Have memory instruction:\n");
		print_rtl_single (dump_file, insn);
	      }
	    combine_p = false;
	    break;
	  }

	if (df_reg_used (insn, (*insn_set)[0].base_reg)
	    || df_reg_defined (insn, (*insn_set)[0].base_reg))
	  {
	    if (dump_file)
	      {
		fprintf (dump_file, "Use .bi form: Base reg is"
			 " used or defined between multiple-insn"
			 " and next multiple-insn\n");
		fprintf (dump_file, "Base register: r%d,\n",
			 REGNO ((*insn_set)[0].base_reg));
		fprintf (dump_file, "use or def instruction:\n");
		print_rtl_single (dump_file, insn);
	      }
	    combine_p = false;
	    break;
	  }
      }

  /* Restore element order.  */
  insn_set->qsort (compare_offset);
  ref_set->qsort (compare_offset);

  if (combine_p)
    return true;
  else
    return false;
}

static void
nds32_merge_overlapping_regs (HARD_REG_SET *pset, struct du_head *head)
{
  bitmap_iterator bi;
  unsigned i;
  IOR_HARD_REG_SET (*pset, head->hard_conflicts);
  EXECUTE_IF_SET_IN_BITMAP (&head->conflicts, 0, i, bi)
    {
      du_head_p other = regrename_chain_from_id (i);
      unsigned j = other->nregs;
      gcc_assert (other != head);
      while (j-- > 0)
	SET_HARD_REG_BIT (*pset, other->regno + j);
    }
}

/* Check if NEW_REG can be the candidate register to rename for
   REG in THIS_HEAD chain.  THIS_UNAVAILABLE is a set of unavailable hard
   registers.  */
static bool
nds32_check_new_reg_p (int reg ATTRIBUTE_UNUSED, int new_reg,
		       struct du_head *this_head, HARD_REG_SET this_unavailable)
{
  enum machine_mode mode = GET_MODE (*this_head->first->loc);
  int nregs = hard_regno_nregs[new_reg][mode];
  int i;
  struct du_chain *tmp;

  for (i = nregs - 1; i >= 0; --i)
    if (TEST_HARD_REG_BIT (this_unavailable, new_reg + i)
	|| fixed_regs[new_reg + i]
	|| global_regs[new_reg + i]
	/* Can't use regs which aren't saved by the prologue.  */
	|| (! df_regs_ever_live_p (new_reg + i)
	    && ! call_used_regs[new_reg + i])
#ifdef LEAF_REGISTERS
	/* We can't use a non-leaf register if we're in a
	   leaf function.  */
	|| (crtl->is_leaf
	    && !LEAF_REGISTERS[new_reg + i])
#endif
#ifdef HARD_REGNO_RENAME_OK
	|| ! HARD_REGNO_RENAME_OK (reg + i, new_reg + i)
#endif
	)
      return false;

  /* See whether it accepts all modes that occur in
     definition and uses.  */
  for (tmp = this_head->first; tmp; tmp = tmp->next_use)
    if ((! HARD_REGNO_MODE_OK (new_reg, GET_MODE (*tmp->loc))
	 && ! DEBUG_INSN_P (tmp->insn))
	|| (this_head->need_caller_save_reg
	    && ! (HARD_REGNO_CALL_PART_CLOBBERED
		  (reg, GET_MODE (*tmp->loc)))
	    && (HARD_REGNO_CALL_PART_CLOBBERED
		(new_reg, GET_MODE (*tmp->loc)))))
      return false;

  return true;
}

static int
nds32_find_best_rename_reg (du_head_p this_head, int new_reg, int old_reg)
{
  HARD_REG_SET unavailable;
  int best_new_reg = old_reg;

  COMPL_HARD_REG_SET (unavailable, reg_class_contents[GENERAL_REGS]);
  CLEAR_HARD_REG_BIT (unavailable, this_head->regno);

  /* Further narrow the set of registers we can use for renaming.
     If the chain needs a call-saved register, mark the call-used
     registers as unavailable.  */
  if (this_head->need_caller_save_reg)
    IOR_HARD_REG_SET (unavailable, call_used_reg_set);

  /* Mark registers that overlap this chain's lifetime as unavailable.  */
  nds32_merge_overlapping_regs (&unavailable, this_head);

  if (nds32_check_new_reg_p (old_reg, new_reg, this_head, unavailable))
    best_new_reg = new_reg;

  return best_new_reg;
}

static bool
nds32_try_rename_reg (rtx insn, unsigned op_pos, unsigned best_reg)
{
  insn_rr_info *info;
  du_head_p op_chain;
  unsigned oldreg, newreg;

  info = &insn_rr[INSN_UID (insn)];

  if (info->op_info == NULL)
    return false;

  if (info->op_info[op_pos].n_chains == 0)
    return false;

  op_chain = regrename_chain_from_id (info->op_info[op_pos].heads[0]->id);

  if (op_chain->cannot_rename)
    return false;

  oldreg = op_chain->regno;
  newreg = nds32_find_best_rename_reg (op_chain, best_reg, oldreg);

  if (newreg == oldreg)
    return false;

  return true;
}

/* Grouping consecutive registers.  */
static void
nds32_group_available_reg (HARD_REG_SET *available_regset, enum reg_class clazz,
			   std::vector <available_reg_info_t> *available_group)
{
  hard_reg_set_iterator hrsi;
  unsigned regno, pre_regno = 0;
  unsigned count = 0;
  available_reg_info_t reg_info;
  std::vector<available_reg_info_t>::iterator it;

  if (!available_group->empty ())
    available_group->clear ();

  /* Find available register form $r16 to $r31.  */
  EXECUTE_IF_SET_IN_HARD_REG_SET (reg_class_contents[clazz], 2, regno, hrsi)
    {
      /* Caller-save register or callee-save register but it's ever live.  */
      if (TEST_HARD_REG_BIT (*available_regset, regno)
	  && (call_used_regs[regno] || df_regs_ever_live_p (regno)))
	{
	  if (pre_regno == 0
	      || (pre_regno + 1) == regno)
	    count++;
	}
      else
	{
	  if (count >= 2)
	    {
	      reg_info.amount = count;
	      reg_info.end = pre_regno;
	      reg_info.start = pre_regno - count + 1;
	      available_group->push_back (reg_info);
	    }
	  count = 0;
	}
      pre_regno = regno;
    }

  sort (available_group->begin(), available_group->end(), compare_amount);

  if (dump_file)
    {
      for (it = available_group->begin();
	   it != available_group->end(); ++it)
	fprintf (dump_file,
		 "available amount = %d start = %d "
		 "end = %d \n", it->amount, it->start,
		 it->end);
    }
}

/* Try to rename insn's register in order.  */
static void
nds32_find_reg (load_store_infos_t *insn, load_store_infos_t *rename_insn,
		HARD_REG_SET *available_regset)
{
  int can_rename_number;
  unsigned i, regno, amount;
  unsigned op_pos = (*insn)[0].load_p ? 0 : 1;
  auto_vec<load_store_info_t, 64> temp_set;
  std::vector<available_reg_info_t> available_group;
  std::vector<available_reg_info_t>::iterator it;
  auto_vec<load_store_info_t, 64> down_set, up_set;
  unsigned int down_num = 0, up_num = 0;
  long offset;
  int m;

  /* We will change 'insn' element order,
     to avoid change order using 'temp_set'.  */
  for (i = 0; i < insn->length (); i++)
    temp_set.safe_push ((*insn)[i]);

  if (temp_set[0].post_type == NDS32_NONE)
    temp_set.qsort (compare_offset);

  nds32_group_available_reg (available_regset, GENERAL_REGS, &available_group);

 /* Check rename register form top insn to bottom insn,
    and avoid using fp, sp, lp, gp registers.  */
  regno = REGNO (temp_set[0].reg);
  can_rename_number = regno + temp_set.length () - 1;
  offset = temp_set[0].offset;

  if (can_rename_number < FP_REGNUM)
    for (i = 1; i < temp_set.length (); ++i)
      {
	/* Find this case:
	     lwi $r0, [$r2 + 4]
	     lwi $r3, [$r2 + 8]

	   Rename $r3 to $r1.  */
	down_num++;
	if ((regno + i) != REGNO (temp_set[i].reg))
	  {
	    if (nds32_try_rename_reg (temp_set[i].insn, op_pos, regno + i))
	      {
		/* Store in temparary set.  */
		down_set.safe_push (temp_set[i]);
		down_set.last ().new_reg = regno + i;
	      }
	    else
	      /* Stop when the register sequence is broken.  */
	      break;
	  }
      }

  /* Check rename register form bottom insn to top insn,
     and avoid using fp, sp, lp, gp registers.  */
  regno = REGNO (temp_set.last ().reg);
  can_rename_number = regno - temp_set.length () + 1;

  if (can_rename_number > 0 && regno < FP_REGNUM)
    for (i = temp_set.length () - 1; i > 0; --i)
      {
	/* Find this case:
	     lwi $r1, [$r2 + 4]
	     lwi $r4, [$r2 + 8]

	   Rename $r1 to $r3.  */
	up_num++;
	if ((regno - i) != REGNO (temp_set[i - 1].reg))
	  {
	    if (nds32_try_rename_reg (temp_set[i - 1].insn, op_pos, regno - i))
	      {
		/* Store in rename_insn.  */
		up_set.safe_push (temp_set[i - 1]);
		up_set.last ().new_reg = regno - i;
	      }
	    else
	      /* Stop when the register sequence is broken.  */
	      break;
	  }
      }

  /* Rename for the longest sequence.  */
  /* The overhead of zero offset instruction is lowest, so try it first.  */
  if ((offset == 0 || down_num >= up_num) && !down_set.is_empty ())
    {
      for (m = down_set.length () - 1; m >= 0; --m)
	{
	  regno = REGNO (down_set[m].reg);
	  CLEAR_HARD_REG_BIT (*available_regset, regno);
	  rename_insn->safe_push (down_set[m]);
	}
      nds32_group_available_reg (available_regset, GENERAL_REGS,
				 &available_group);
      return;
    }
  else if (up_num >= down_num && !up_set.is_empty ())
    {
      for (m = up_set.length () - 1; m >= 0; --m)
	{
	  regno = REGNO (up_set[m].reg);
	  CLEAR_HARD_REG_BIT (*available_regset, regno);
	  rename_insn->safe_push (up_set[m]);
	}
      nds32_group_available_reg (available_regset, GENERAL_REGS,
				 &available_group);
      return;
    }
  /* Check whether it is empty, We will use available table.  */
  else if (available_group.empty ())
    return;

  amount = available_group.begin ()->amount;
  /* Using the minimum number, as the rename amount.  */
  if (amount > temp_set.length ())
    amount = temp_set.length ();

  /* Using most available register number to rename.  */
  regno = available_group.begin ()->start;
  for (i = 0; i < amount; ++i)
    {
      if (nds32_try_rename_reg (temp_set[i].insn, op_pos, regno))
	{
	  rename_insn->safe_push (temp_set[i]);
	  rename_insn->last ().new_reg = regno;
	  CLEAR_HARD_REG_BIT (*available_regset, regno);
	  regno++;
	}
      else
	/* Stop when the register sequence is broken.  */
	break;
    }

  /* Check length here because the whole sequence entries
     have to be renamed.  */
  if (rename_insn->length () > 1)
    {
      /* Update available table.  */
      nds32_group_available_reg (available_regset, GENERAL_REGS,
				 &available_group);
      return;
    }

  /* Using all available register to rename each insn.  */
  for (i = 0; i < (temp_set.length () - 1); i += 2)
    {
      for (it = available_group.begin();
	   it != available_group.end(); ++it)
	{
	  bool change_p = false;
	  unsigned int j;
	  regno = it->start;

	  /* Once replaced two instructions. */
	  for (j = regno; j < (it->end + 1); j += 2)
	    {
	      if (nds32_try_rename_reg (temp_set[i].insn, op_pos, regno)
		  && nds32_try_rename_reg (temp_set[i + 1].insn,
					   op_pos, regno + 1))
		{
		  rename_insn->safe_push (temp_set[i]);
		  rename_insn->last ().new_reg = regno;
		  CLEAR_HARD_REG_BIT (*available_regset, regno);

		  rename_insn->safe_push (temp_set[i + 1]);
		  rename_insn->last ().new_reg = regno + 1;
		  CLEAR_HARD_REG_BIT (*available_regset, regno + 1);
		  change_p = true;
		  break;
		}
	    }

	  if (change_p)
	    {
	      nds32_group_available_reg (available_regset, GENERAL_REGS,
					 &available_group);
	      break;
	    }
	}
    }
}

static void
nds32_rename_reg (rtx insn, unsigned op_pos, unsigned newreg)
{
  insn_rr_info *info;
  du_head_p op_chain;

  info = &insn_rr[INSN_UID (insn)];
  op_chain = regrename_chain_from_id (info->op_info[op_pos].heads[0]->id);

  if (dump_file)
    {
      fprintf (dump_file, "Try to rename operand %d to %d:\n",
	       op_pos, newreg);
      print_rtl_single (dump_file, insn);
    }

  regrename_do_replace (op_chain, newreg);

  if (dump_file)
    {
      print_rtl_single (dump_file, insn);
    }
}

/* Combine mutilple load/store insn into a lmw/smw insn.  */
static void
nds32_combine_bi_insn (load_store_infos_t *load_store_info)
{
  auto_vec<load_store_info_t, 64> candidate_set, bi_set;
  unsigned int i, j, regno;

  bool load_insn_p;
  enum nds32_memory_post_type post_type;

  for (i = 0; i < load_store_info->length (); ++i)
    {
      /* Recording instruction order of priority and initinal place.  */
      (*load_store_info)[i].order = i;
      (*load_store_info)[i].place = false;
      candidate_set.safe_push ((*load_store_info)[i]);
    }

  for (i = 0; i < candidate_set.length (); ++i)
    {
      load_insn_p = candidate_set[i].load_p;
      post_type = candidate_set[i].post_type;
      regno = REGNO (candidate_set[i].reg);

      for (j = i + 1; j < candidate_set.length (); ++j)
	{
	  if ((post_type == candidate_set[j].post_type)
	      && (load_insn_p == candidate_set[j].load_p)
	      && ((regno + 1) == REGNO (candidate_set[j].reg)))
	    {
	      bi_set.safe_push (candidate_set[i]);
	      bi_set.safe_push (candidate_set[j]);

	      if (nds32_combine_multiple_p (&bi_set, false)
		  && nds32_base_reg_safe_p (&bi_set)
		  && nds32_lmwsmw_insert_place (&bi_set) != NULL_RTX)
		{
		  rtx place = nds32_lmwsmw_insert_place (&bi_set);
		  rtx base_reg = bi_set[0].base_reg;

		  nds32_emit_multiple_insn (&bi_set, base_reg, place, true);
		  delete_insn (bi_set[i].insn);
		  delete_insn (bi_set[j].insn);
		  candidate_set.ordered_remove (j);
		  bi_set.block_remove (0, bi_set.length ());
		  break;
		}

	      bi_set.block_remove (0, bi_set.length ());
	    }
	}
    }
}

/* Combine mutilple load/store insn into a lmw/smw insn.  */
static void
nds32_combine_load_store_insn (load_store_infos_t *load_store_info,
			       HARD_REG_SET *available_regset)
{
  auto_vec<load_store_info_t, 64> candidate_set, main_set, temp_set;
  auto_vec<load_store_info_t, 64> first_set, second_set;
  HOST_WIDE_INT current_offset, last_offset = 0, add_offset = 0;
  unsigned int i, j, regno;
  int group_num = 0, group_id;
  bool load_insn_p;
  bool new_base_p = false;
  bool prev_bim_p = false;
  bool inc_p = true, dec_p = true;
  rtx new_base_reg = NULL_RTX;
  rtx base_reg = (*load_store_info)[0].base_reg;
  rtx place;
  unsigned new_base_regnum;

  /* Get available register to add offset for first instruction.  */
  new_base_regnum = find_available_reg (available_regset, GENERAL_REGS);
  if (new_base_regnum != INVALID_REGNUM)
    {
      CLEAR_HARD_REG_BIT (*available_regset, new_base_regnum);
      new_base_reg = gen_rtx_REG (Pmode, new_base_regnum);
      /* Copy attribute form base register to new base register.  */
      ORIGINAL_REGNO (new_base_reg) =
	ORIGINAL_REGNO ((*load_store_info)[0].base_reg);
      REG_ATTRS (new_base_reg) = REG_ATTRS ((*load_store_info)[0].base_reg);
      new_base_p = true;

      if (dump_file)
	fprintf (dump_file, "Have new base register: %d\n", new_base_regnum);
    }

  /* Recording instruction order of priority and initinal place.  */
  for (i = 0; i < load_store_info->length (); ++i)
    {
      (*load_store_info)[i].order = i;
      (*load_store_info)[i].place = false;
    }

  /* Fetch first instruction information from 'load_store_info',
     we will use first instruction as base, to search next instruction.  */
  candidate_set.safe_push ((*load_store_info)[0]);
  /* Set offset, regno, load_p state from candidate_set.  */
  current_offset = candidate_set[0].offset;
  regno = REGNO (candidate_set[0].reg);
  load_insn_p = candidate_set[0].load_p;
  /* Set first instruction group ID,
     the group ID mark instruction for the same group.  */
  candidate_set[0].group = group_num;

  /* Search instructions can be combined to a lmw/smw instruction.  */
  for (i = 1; i < load_store_info->length (); ++i)
    {
      /* Collecting register number and offset is increase,
	 for example:

	   lwi $r0, [$r22 + 4]  <- base instruction
	   lwi $r1, [$r22 + 8]  <- collect object

	 The collect object (regno + 1), (offset + 4)
	 from base instruction.  */
      if ((current_offset == (*load_store_info)[i].offset - 4)
	  && ((regno + 1) == REGNO ((*load_store_info)[i].reg))
	  && (load_insn_p == (*load_store_info)[i].load_p)
	  && inc_p)
	{
	  /* Give instruction group ID.  */
	  (*load_store_info)[i].group = group_num;
	  /* Save instruction.  */
	  candidate_set.safe_push ((*load_store_info)[i]);
	  /* Update state, next register number and offset.  */
	  regno = REGNO ((*load_store_info)[i].reg);
	  current_offset += 4;
	  /* Close decrease type, search increase type.  */
	  dec_p = false;
	}
      /* Collecting register number and offset is decrease,
	 for example:

	   lwi $r2, [$r22 + 8]  <- base instruction
	   lwi $r1, [$r22 + 4]  <- collect object

	 The collect object (regno - 1), (offset - 4)
	 from base instruction.  */
      else if ((current_offset == (*load_store_info)[i].offset + 4)
	       && ((regno - 1) == REGNO ((*load_store_info)[i].reg))
	       && (load_insn_p == (*load_store_info)[i].load_p)
	       && dec_p)
	{
	  /* Give instruction group ID.  */
	  (*load_store_info)[i].group = group_num;
	  /* Save instruction.  */
	  candidate_set.safe_push ((*load_store_info)[i]);

	  /* Update state, next register number and offset.  */
	  regno = REGNO ((*load_store_info)[i].reg);
	  current_offset -= 4;
	  /* Close increase type, search decrease type.  */
	  inc_p = false;
	}
      else
	{
	  inc_p = true;
	  dec_p = true;
	}

      /* Instructions collect is complete.  */
      if ((inc_p && dec_p)
          || (i + 1) == load_store_info->length ())
	{
	  /* Filter candidate instructions.  */
	  if (nds32_combine_multiple_p (&candidate_set, new_base_p)
	      && nds32_base_reg_safe_p (&candidate_set)
	      && nds32_lmwsmw_insert_place (&candidate_set) != NULL_RTX)
	    {
	      /* Store candidate instructions to 'main_set'.  */
	      for (j = 0; j < candidate_set.length (); j++)
		main_set.safe_push (candidate_set[j]);
	    }

	  /* Scan to the last instruction, it is complete.  */
	  if ((i + 1) == load_store_info->length ())
	    break;

	  /* Clean candidate_set sequence.  */
	  candidate_set.block_remove (0, candidate_set.length ());
	  /* Reinitialize first instruction infomation
	     to search next instruction.  */
	  candidate_set.safe_push ((*load_store_info)[i]);
	  /* Update group number for next sequence.  */
	  group_num ++;
	  /* Set offset, regno, load_p state from candidate_set.  */
	  current_offset = candidate_set.last ().offset;
	  regno = REGNO (candidate_set.last ().reg);
	  load_insn_p = candidate_set.last ().load_p;
	  candidate_set.last ().group = group_num;
	}
      else if (!nds32_base_reg_safe_p (&candidate_set)
	       || nds32_lmwsmw_insert_place (&candidate_set) == NULL_RTX)
	{
	  /* Check collect instruction for each instruction,
	     we store (n - 1) instructions in group, and
	     last instruction make next group First instruction.  */
	  for (j = 0; j < (candidate_set.length () - 1); j++)
	    temp_set.safe_push (candidate_set[j]);

	  /* Store candidate instructions to 'main_set'.  */
	  if (nds32_combine_multiple_p (&temp_set, new_base_p))
	    {
	      for (j = 0; j < (temp_set.length ()); j++)
		main_set.safe_push (temp_set[j]);
	    }

	  /* Clean temp_set sequence.  */
	  temp_set.block_remove (0, temp_set.length ());
	  /* Clean candidate_set sequence.  */
	  candidate_set.block_remove (0, (candidate_set.length () - 1));
	  /* Update group number for next sequence.  */
	  group_num ++;
	  /* Set offset, regno, load_p state from candidate_set.  */
	  current_offset = candidate_set.last ().offset;
	  regno = REGNO (candidate_set.last ().reg);
	  load_insn_p = candidate_set.last ().load_p;
	  candidate_set.last ().group = group_num;
	  /* Reset it for search increase and decrease type.  */
	  inc_p = true;
	  dec_p = true;
	}
    }

  if (dump_file)
    {
      if (!main_set.is_empty ())
	fprintf (dump_file,"Do lmwsmw instructions:\n");
      for (i = 0; i < main_set.length (); ++i)
	{
	  fprintf (dump_file,
		   "regno = %d base_regno = %d "
		   "offset = " HOST_WIDE_INT_PRINT_DEC " "
		   "load_p = %d UID = %u group = %d,"
		   " order = %d, place = %d\n",
		   REGNO (main_set[i].reg),
		   REGNO (main_set[i].base_reg),
		   main_set[i].offset,
		   main_set[i].load_p,
		   INSN_UID (main_set[i].insn),
		   main_set[i].group,
		   main_set[i].order,
		   main_set[i].place);
	}
    }

  /* Fetch first group instruction from main_set.  */
  if (!main_set.is_empty ())
    {
      /* Sort main_set by offset.  */
      main_set.qsort (compare_offset);

      group_id = main_set[0].group;
      nds32_fetch_group_insn (&main_set, &first_set, group_id);
      last_offset = first_set.last ().offset;
    }

  /* Main loop for emit lmw/smw instrucion.  */
  while (!main_set.is_empty ())
    {
      /* Get second group ID.  */
      group_id = main_set[0].group;
      for (i = 0; i < main_set.length (); ++i)
	{
	  /* Prefer get consecutive offset form
	     first group to second group  */
	  if ((last_offset + 4) == main_set[i].offset)
	    {
	      group_id = main_set[i].group;
	      break;
	    }
	}

      /* Fetch second instrucion group.  */
      nds32_fetch_group_insn (&main_set, &second_set, group_id);
      /* Get lmw/smw insert place.  */
      place = nds32_lmwsmw_insert_place (&first_set);

      /* Adjust address offset, because lmw/smw instruction
	 only allow offset is zero.
	   example:
	    lwi $r0, [$r3 + 4]
	    lwi $r1, [$r3 + 8]
	    lwi $r2, [$r3 + 12]

	    combine into

	    addi $r3, $r3, 4
	    lwm.bi(m) $r0, [$r3], $r2

	 Need addi instrucion to handle offset.  */
      if (first_set[0].offset != 0 && !prev_bim_p)
	{
	  if (dump_file)
	    fprintf (dump_file, "Use addi insn handle offset: "
		     "" HOST_WIDE_INT_PRINT_DEC "\n",
		     first_set[0].offset);
	  /* Use available register to process offset,
	     and don't recovey base register value.  */
	  if (new_base_p)
	    {
	      base_reg = new_base_reg;
	      add_offset = 0;
	      CLEAR_HARD_REG_BIT (*available_regset, new_base_regnum);
	    }
	  else
	    add_offset = first_set[0].offset;

	  nds32_emit_add_insn (first_set[0], base_reg, place, true);
	}

      if (nds32_use_bim_p (&first_set, &second_set))
	{
	  if (dump_file)
	    fprintf (dump_file, "Generate BIM form.\n");

	  nds32_emit_multiple_insn (&first_set, base_reg, place, true);

	  /* Update status, for next instruction sequence.
	     The add_offset need add 4, because the instruction
	     is post increase.  */
	  add_offset = first_set.last ().offset + 4;
	  prev_bim_p = true;
	}
      else
	{
	  if (dump_file)
	    fprintf (dump_file, "Generate BI form.\n");

	  nds32_emit_multiple_insn (&first_set, base_reg, place, false);

	  if (add_offset != 0)
	    {
	      if (dump_file)
		fprintf (dump_file, "Use addi insn handle -offset: "
			 ""HOST_WIDE_INT_PRINT_DEC"\n",
			 add_offset);

	      nds32_emit_add_insn (first_set[0], base_reg, place, false);
	      add_offset = 0;
	    }
	  prev_bim_p = false;

	  /* Recovey base register for next instruction sequence.  */
	  if (REGNO (base_reg) != REGNO (first_set[0].base_reg))
	    base_reg = first_set[0].base_reg;
	}

      /* Delete insn, replace by lmw/smw instruction.  */
      for (i = 0; i < first_set.length (); ++i)
	delete_insn (first_set[i].insn);

      /* Clean first_set for store next instruction group.  */
      first_set.block_remove (0, first_set.length ());
      /* Store next instruction group.  */
      for (i = 0; i < second_set.length (); ++i)
	first_set.safe_insert (i, second_set[i]);

      /* Clean second_set.  */
      second_set.block_remove (0, second_set.length ());

      /* Update last_offset for search next group.  */
      last_offset = first_set.last ().offset;
    }

  /* Processing the last instruction group.  */
  if (!first_set.is_empty ())
    {
      /* Get lmw/smw insert place.  */
      place = nds32_lmwsmw_insert_place (&first_set);

      if (first_set[0].offset != 0 && !prev_bim_p)
	{
	  if (dump_file)
	    fprintf (dump_file, "Use addi insn handle offset: "
		     "" HOST_WIDE_INT_PRINT_DEC "\n",
		     first_set[0].offset);

	  if (new_base_p)
	    {
	      base_reg = new_base_reg;
	      add_offset = 0;
	    }
	  else
	    add_offset = first_set[0].offset;

	  nds32_emit_add_insn (first_set[0], base_reg, place, true);
	}

      if (dump_file)
	fprintf (dump_file, "Generate BI form.\n");

      nds32_emit_multiple_insn (&first_set, base_reg, place, false);

      if (add_offset != 0)
	{
	  if (dump_file)
	    fprintf (dump_file, "Use addi insn handle -offset: "
		     ""HOST_WIDE_INT_PRINT_DEC"\n",
		     -add_offset);

	  nds32_emit_add_insn (first_set[0], base_reg, place, false);
	}

      /* Delete insn, replace by lmw/smw instruction.  */
      for (i = 0; i < first_set.length (); ++i)
	delete_insn (first_set[i].insn);
    }
}

/* Combine mutilple load/store insn into a lmw/smw insn.  */
static void
nds32_rename_bi_insn (load_store_infos_t *load_store_info,
		       HARD_REG_SET *available_regset)
{
  auto_vec<load_store_info_t, 64> candidate_set, bi_set, replace_set;
  unsigned int i, j;

  bool load_insn_p;
  enum nds32_memory_post_type post_type;

  for (i = 0; i < load_store_info->length (); ++i)
    {
      /* Recording instruction order of priority and initinal place.  */
      (*load_store_info)[i].order = i;
      (*load_store_info)[i].place = false;
      candidate_set.safe_push ((*load_store_info)[i]);
    }

  for (i = 0; i < candidate_set.length (); ++i)
    {
      load_insn_p = candidate_set[i].load_p;
      post_type = candidate_set[i].post_type;

      for (j = i + 1; j < candidate_set.length (); ++j)
	{
	  if ((post_type == candidate_set[j].post_type)
	      && (load_insn_p == candidate_set[j].load_p))
	    {
	      bi_set.safe_push (candidate_set[i]);
	      bi_set.safe_push (candidate_set[j]);

	      if (nds32_combine_multiple_p (&bi_set, false)
		  && nds32_base_reg_safe_p (&bi_set)
		  && nds32_lmwsmw_insert_place (&bi_set) != NULL_RTX)
		{
		  nds32_find_reg (&bi_set, &replace_set, available_regset);

		  if (!replace_set.is_empty ())
		    {
		      unsigned k;
		      unsigned op_pos = replace_set[0].load_p ? 0 : 1;

		      /* Do rename register.  */
		      for (k = 0; k < replace_set.length (); ++k)
			nds32_rename_reg (replace_set[k].insn, op_pos,
					  replace_set[k].new_reg);

		      replace_set.block_remove (0, replace_set.length ());
		    }

		  candidate_set.ordered_remove (j);
		  bi_set.block_remove (0, bi_set.length ());
		  break;
		}

	      bi_set.block_remove (0, bi_set.length ());
	    }
	}
    }
}

/* Rename register, can be combined mutilple load/store insn.  */
static void
nds32_rename_load_store_reg (load_store_infos_t *load_store_info,
			     HARD_REG_SET *available_regset)
{
  auto_vec<load_store_info_t, 64> rename_set, temp_set, replace_set;
  HOST_WIDE_INT current_offset;
  unsigned int i, j;
  bool load_insn_p;
  bool inc_p = true, dec_p = true;

  /* Recording instruction order of priority and initinal place.  */
  for (i = 0; i < load_store_info->length (); ++i)
    {
      (*load_store_info)[i].order = i;
      (*load_store_info)[i].place = false;
    }

  /* Fetch first instruction information from 'load_store_info',
     we will use first instruction as base, to search next instruction.  */
  rename_set.safe_push ((*load_store_info)[0]);
  /* Set offset, load_p state from rename_set.  */
  current_offset = rename_set[0].offset;
  load_insn_p = rename_set[0].load_p;

  /* Search instructions can be combined to a lmw/smw instruction.  */
  for (i = 1; i < load_store_info->length (); ++i)
    {
      /* Collecting offset is increase, for example:

	   lwi pseudo_reg, [$r22 + 4]  <- base instruction
	   lwi pseudo_reg, [$r22 + 8]  <- collect object

	 The collect object (offset + 4) from base instruction.  */
      if ((current_offset == (*load_store_info)[i].offset - 4)
	  && (load_insn_p == (*load_store_info)[i].load_p)
	  && inc_p)
	{
	  /* Save instruction.  */
	  rename_set.safe_push ((*load_store_info)[i]);
	  /* Update offset.  */
	  current_offset += 4;
	  /* Close decrease type, search increase type.  */
	  dec_p = false;
	}
      /* Collecting offset is decrease, for example:

	   lwi pseudo_reg, [$r22 + 8]  <- base instruction
	   lwi pseudo_reg, [$r22 + 4]  <- collect object

	 The collect object (offset - 4) from base instruction.  */
      else if ((current_offset == (*load_store_info)[i].offset + 4)
	       && (load_insn_p == (*load_store_info)[i].load_p)
	       && dec_p)
	{
	  /* Save instruction.  */
	  rename_set.safe_push ((*load_store_info)[i]);

	  /* Update offset.  */
	  current_offset -= 4;
	  /* Close increase type, search decrease type.  */
	  inc_p = false;
	}
      else
	{
	  inc_p = true;
	  dec_p = true;
	}

      /* Instructions collect is completed.  */
      if ((inc_p && dec_p)
	  || (i + 1) == load_store_info->length ())
	{
	  /* Check whether the rename register. */
	  if (nds32_combine_multiple_p (&rename_set, false)
	      && nds32_base_reg_safe_p (&rename_set)
	      && nds32_lmwsmw_insert_place (&rename_set) != NULL_RTX)
	    {
	      /* Find can rename instruction, and store in 'replace_set'.  */
	      nds32_find_reg (&rename_set, &replace_set, available_regset);

	      if (!replace_set.is_empty ())
		{
		  unsigned op_pos = replace_set[0].load_p ? 0 : 1;

		  /* Do rename register.  */
		  for (j = 0; j < replace_set.length (); ++j)
		    nds32_rename_reg (replace_set[j].insn, op_pos,
				      replace_set[j].new_reg);

		  replace_set.block_remove (0, replace_set.length ());
		}
	    }

	  /* Scan to the last instruction, it is complete.  */
	  if ((i + 1) == load_store_info->length ())
	    break;

	  /* Clean rename_set sequence.  */
	  rename_set.block_remove (0, rename_set.length ());
	  /* Reinitialize first instruction infomation
	     to search next instruction.  */
	  rename_set.safe_push ((*load_store_info)[i]);
	  /* Set offset, load_p state from rename_set.  */
	  current_offset = rename_set.last ().offset;
	  load_insn_p = rename_set.last ().load_p;
	}
      else if (!nds32_base_reg_safe_p (&rename_set)
	       || nds32_lmwsmw_insert_place (&rename_set) == NULL_RTX)
	{
	  /* Check collect instruction for each instruction,
	     we store (n - 1) instructions in group, and
	     last instruction as the first instruction of the next group.  */
	  for (j = 0; j < (rename_set.length () - 1); j++)
	    temp_set.safe_push (rename_set[j]);

	  if (nds32_combine_multiple_p (&temp_set, false))
	    {
	      /* Find can rename instruction, and store in 'replace_set'.  */
	      nds32_find_reg (&temp_set, &replace_set, available_regset);

	      if (!replace_set.is_empty ())
		{
		  unsigned op_pos = replace_set[0].load_p ? 0 : 1;

		  /* Do rename register.  */
		  for (j = 0; j < replace_set.length (); ++j)
		    nds32_rename_reg (replace_set[j].insn, op_pos,
				      replace_set[j].new_reg);

		  replace_set.block_remove (0, replace_set.length ());
		}
	    }

	  /* Clean temp_set sequence.  */
	  temp_set.block_remove (0, temp_set.length ());
	  /* Clean rename_set sequence.  */
	  rename_set.block_remove (0, (rename_set.length () - 1));
	  /* Set offset, regno, load_p state from rename_set.  */
	  current_offset = rename_set.last ().offset;
	  load_insn_p = rename_set.last ().load_p;
	  /* Reset it for search increase and decrease type.  */
	  inc_p = true;
	  dec_p = true;
	}
    }
}

static void
nds32_do_lmwsmw_opt (basic_block bb, bool rename_p)
{
  rtx insn;
  HARD_REG_SET available_regset;
  load_store_info_t load_store_info;
  auto_vec<load_store_info_t, 64> load_store_infos[NDS32_GPR_NUM];
  auto_vec<load_store_info_t, 64> plus_infos[NDS32_GPR_NUM];
  auto_vec<load_store_info_t, 64> post_infos[NDS32_GPR_NUM];
  int i;
  unsigned j;
  unsigned regno;
  unsigned polluting;
  df_ref *def_rec;
  /* Dirty mean a register is define again after
     first load/store instruction.
     For example:

     lwi $r2, [$r3 + #0x100]
     mov $r3, $r4            ! $r3 is dirty after this instruction.
     lwi $r1, [$r3 + #0x120] ! so this load can't chain with prev load.
   */
  bool dirty[NDS32_GPR_NUM];

  if (dump_file)
    fprintf (dump_file, "scan bb %d\n", bb->index);

  for (i = 0; i < NDS32_GPR_NUM; ++i)
    dirty[i] = false;

  FOR_BB_INSNS (bb, insn)
    {
      if (!INSN_P (insn))
	continue;

      polluting = INVALID_REGNUM;

      /* Set def reg is dirty if chain is not empty.  */
      for (def_rec = DF_INSN_DEFS (insn);
	   *def_rec; def_rec++)
	{
	  regno = DF_REF_REGNO (*def_rec);

	  if (!NDS32_IS_GPR_REGNUM (regno))
	    continue;

	  if (!load_store_infos[regno].is_empty ())
	    {
	      /* Set pulluting here because the source register
		 may be the same one.  */
	      if (dirty[regno] == false)
		polluting = regno;

	      dirty[regno] = true;
	    }
	}

      /* Set all caller-save register is dirty if chain is not empty.  */
      if (CALL_P (insn))
	{
	  for (i = 0; i < NDS32_GPR_NUM; ++i)
	    {
	      if (call_used_regs[i] && !load_store_infos[i].is_empty ())
		dirty[i] = true;
	    }
	}

      if (nds32_load_store_reg_plus_offset (insn, &load_store_info))
	{
	  regno = REGNO (load_store_info.base_reg);
	  gcc_assert (NDS32_IS_GPR_REGNUM (regno));

	  /* Don't add to chain if this reg is dirty.  */
	  if (dirty[regno] && polluting != regno)
	    break;

	  /* If the register is first time to be used and be polluted
	     right away, we don't push it.  */
	  if (regno == REGNO (load_store_info.reg) && load_store_info.load_p
	      && dirty[regno] == false)
	    continue;

	  load_store_infos[regno].safe_push (load_store_info);
	}
    }

   for (i = 0; i < NDS32_GPR_NUM; ++i)
    {
      for (j = 0; j < load_store_infos[i].length (); ++j)
	{
	  if (load_store_infos[i][j].post_type == NDS32_NONE)
	    plus_infos[i].safe_push (load_store_infos[i][j]);
	  else
	    post_infos[i].safe_push (load_store_infos[i][j]);
	}
    }

  for (i = 0; i < NDS32_GPR_NUM; ++i)
    {
      if (load_store_infos[i].length () <= 1)
	{
	  if (dump_file && load_store_infos[i].length () == 1)
	    fprintf (dump_file,
		     "Skip Chain for $r%d since chain size only 1\n",
		     i);
	  continue;
	}

      if (dump_file)
	{
	  fprintf (dump_file,
		   "Chain for $r%d: (size = %u)\n",
		   i, load_store_infos[i].length ());

	  for (j = 0; j < load_store_infos[i].length (); ++j)
	    {
	      fprintf (dump_file,
		       "regno = %d base_regno = %d "
		       "offset = " HOST_WIDE_INT_PRINT_DEC " "
		       "load_p = %d UID = %u place = %d\n",
		       REGNO (load_store_infos[i][j].reg),
		       REGNO (load_store_infos[i][j].base_reg),
		       load_store_infos[i][j].offset,
		       load_store_infos[i][j].load_p,
		       INSN_UID (load_store_infos[i][j].insn),
		       load_store_infos[i][j].place);
	    }
	}

      nds32_get_available_reg_set (bb,
				   load_store_infos[i][0].insn,
				   load_store_infos[i].last ().insn,
				   &available_regset);
      if (dump_file)
	print_hard_reg_set (dump_file, "", available_regset);

      /* If rename_p is true, then do rename register of load/store
	 instruction. Otherwise combination of a multiple load/sotre
	 a multiple load/store instruction.  */
      if (rename_p)
	{
          if (plus_infos[i].length () > 1)
	    nds32_rename_load_store_reg (&plus_infos[i], &available_regset);
          if (post_infos[i].length () > 1)
	    nds32_rename_bi_insn (&post_infos[i], &available_regset);
	}
      else
	{
          if (plus_infos[i].length () > 1)
	    nds32_combine_load_store_insn (&plus_infos[i], &available_regset);
          if (post_infos[i].length () > 1)
	    nds32_combine_bi_insn (&post_infos[i]);
	}
    }
}

static void
nds32_lmwsmw_opt (bool rename_p)
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    nds32_do_lmwsmw_opt (bb, rename_p);
}

/* Implement rename register for load and store instruction.  */
static unsigned int
rest_of_handle_rename_lmwsmw_opt (void)
{
  init_alias_analysis ();

  df_set_flags (DF_LR_RUN_DCE);
  df_note_add_problem ();
  df_analyze ();
  df_set_flags (DF_DEFER_INSN_RESCAN);

  regrename_init (true);
  regrename_analyze (NULL);

  nds32_lmwsmw_opt (true);

  regrename_finish ();

  /* We are finished with alias.  */
  end_alias_analysis ();
  return 1;
}

/* Implement generate lmw and smw instruction.  */
static unsigned int
rest_of_handle_gen_lmwsmw_opt (void)
{
  init_alias_analysis ();

  df_note_add_problem ();
  df_analyze ();
  nds32_lmwsmw_opt (false);

  /* We are finished with alias.  */
  end_alias_analysis ();
  return 1;
}


const pass_data pass_data_nds32_rename_lmwsmw_opt =
{
  RTL_PASS,				/* type */
  "rename_lmwsmw_opt",			/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  true,					/* has_gate */
  true,					/* has_execute */
  TV_MACH_DEP,				/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  ( TODO_df_finish | TODO_verify_rtl_sharing),		/* todo_flags_finish */
};

class pass_nds32_rename_lmwsmw_opt : public rtl_opt_pass
{
public:
  pass_nds32_rename_lmwsmw_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_rename_lmwsmw_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return flag_nds32_lmwsmw_opt; }
  unsigned int execute () { return rest_of_handle_rename_lmwsmw_opt (); }
};

rtl_opt_pass *
make_pass_nds32_rename_lmwsmw_opt (gcc::context *ctxt)
{
  return new pass_nds32_rename_lmwsmw_opt (ctxt);
}

const pass_data pass_data_nds32_gen_lmwsmw_opt =
{
  RTL_PASS,				/* type */
  "gen_lmwsmw_opt",			/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  true,					/* has_gate */
  true,					/* has_execute */
  TV_MACH_DEP,				/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  ( TODO_df_finish | TODO_verify_rtl_sharing),		/* todo_flags_finish */
};

class pass_nds32_gen_lmwsmw_opt : public rtl_opt_pass
{
public:
  pass_nds32_gen_lmwsmw_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_gen_lmwsmw_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return flag_nds32_lmwsmw_opt; }
  unsigned int execute () { return rest_of_handle_gen_lmwsmw_opt (); }
};

rtl_opt_pass *
make_pass_nds32_gen_lmwsmw_opt (gcc::context *ctxt)
{
  return new pass_nds32_gen_lmwsmw_opt (ctxt);
}
