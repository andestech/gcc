/* load-store-opt pass of Andes NDS32 cpu for GNU compiler
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


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"   /* Required by recog.h.  */
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"     /* For DFA state_t.  */
#include "insn-codes.h"    /* For CODE_FOR_xxx.  */
#include "reload.h"     /* For push_reload ().  */
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
#include "langhooks.h"     /* For add_builtin_function ().  */
#include "ggc.h"
#include "tree-pass.h"
#include "target-globals.h"
#include "ira-int.h"
#include "nds32-load-store-opt.h"
#include "nds32-reg-utils.h"
#include <set>

#define NDS32_GPR_NUM 32

static new_base_reg_info_t gen_new_base (rtx,
					 offset_info_t,
					 unsigned,
					 HOST_WIDE_INT,
					 HOST_WIDE_INT);

static const load_store_optimize_pass *load_store_optimizes[] =
{
  /*    allow_regclass, new_base_regclass,
	offset_lower_bound, offset_upper_bound,
	load_only_p, name */
  new load_store_optimize_pass (
	LOW_REGS, LOW_REGS,
	0, (32-4),
	false, "lswi333"),
  new load_store_optimize_pass (
	LOW_REGS, FRAME_POINTER_REG,
	0, (512-4),
	false, "lswi37"),
  new load_store_optimize_pass (
	MIDDLE_REGS, GENERAL_REGS,
	0, 0,
	false, "lswi450"),
  new load_store_optimize_pass (
	MIDDLE_REGS, R8_REG,
	-128, -4,
	true, "lwi45fe")
};

static const int N_LOAD_STORE_OPT_TYPE = sizeof (load_store_optimizes)
					 / sizeof (load_store_optimize_pass*);

load_store_optimize_pass
::load_store_optimize_pass (enum reg_class allow_regclass,
			    enum reg_class new_base_regclass,
			    HOST_WIDE_INT offset_lower_bound,
			    HOST_WIDE_INT offset_upper_bound,
			    bool load_only_p,
			    const char *name)
  : m_allow_regclass (allow_regclass),
    m_new_base_regclass (new_base_regclass),
    m_offset_lower_bound (offset_lower_bound),
    m_offset_upper_bound (offset_upper_bound),
    m_load_only_p (load_only_p),
    m_name (name)
{
  gcc_assert (offset_lower_bound <= offset_upper_bound);
}

int
load_store_optimize_pass::calc_gain (HARD_REG_SET *available_regset,
				     offset_info_t offset_info,
				     load_store_infos_t *load_store_info) const
{
  int extra_cost = 0;
  int gain = 0;
  unsigned i;
  unsigned chain_size;
  unsigned new_base_regnum;
  HOST_WIDE_INT allow_range = m_offset_upper_bound - m_offset_lower_bound;
  new_base_regnum  = find_available_reg (available_regset, m_new_base_regclass);
  chain_size = load_store_info->length ();

  if (new_base_regnum == INVALID_REGNUM)
    {
      if (dump_file)
	fprintf (dump_file,
		 "%s have no avariable register, so give up try %s\n",
		 reg_class_names[m_new_base_regclass],
		 m_name);
      return 0;
    }
  else if (dump_file)
    fprintf (dump_file,
	     "%s is avariable, get %s, try %s, chain size = %u\n",
	     reg_class_names[m_new_base_regclass],
	     reg_names[new_base_regnum],
	     m_name,
	     chain_size);

  HOST_WIDE_INT range = offset_info.max_offset - offset_info.min_offset;

  if (range > allow_range)
    {
      /* TODO: We can perform load-store opt for only part of load store.  */
      if (dump_file)
	fprintf (dump_file,
		 "range is too large for %s"
		 " (range = " HOST_WIDE_INT_PRINT_DEC ", "
		 "allow_range = " HOST_WIDE_INT_PRINT_DEC ")\n",
		 m_name, range, allow_range);
      return 0;
    }

  if (offset_info.min_offset >= m_offset_lower_bound
      && offset_info.max_offset <= m_offset_upper_bound)
    {
      /* mov55.  */
      extra_cost = 2;
    }
  else
    {
      if (satisfies_constraint_Is15 (GEN_INT (offset_info.min_offset
						   - m_offset_lower_bound)))
	{
	  /* add.  */
	  extra_cost = 4;
	}
      else
	{
	  /* TODO: Try m_offset_upper_bound instead of m_offset_lower_bound
		   again.  */
	  /* add45 + movi.  */
	  if (satisfies_constraint_Is20 (GEN_INT (offset_info.min_offset
						  - m_offset_lower_bound)))
	    extra_cost = 6;
	  else
	    return -1; /* Give up if this constant is too large.  */
	}
    }

  for (i = 0; i < chain_size; ++i)
    {
      if (m_load_only_p && !(*load_store_info)[i].load_p)
	continue;

      if (in_reg_class_p ((*load_store_info)[i].reg, m_allow_regclass))
	gain += 2;
    }

  if (dump_file)
    fprintf (dump_file,
	     "%s: gain = %d extra_cost = %d\n",
	     m_name, gain, extra_cost);

  return gain - extra_cost;
}


void
load_store_optimize_pass::do_optimize (
  HARD_REG_SET *available_regset,
  offset_info_t offset_info,
  load_store_infos_t *load_store_info) const
{
  new_base_reg_info_t new_base_reg_info;
  rtx load_store_insn;
  unsigned new_base_regnum;

  new_base_regnum  = find_available_reg (available_regset, m_new_base_regclass);
  gcc_assert (new_base_regnum != INVALID_REGNUM);

  new_base_reg_info =
    gen_new_base ((*load_store_info)[0].base_reg,
		  offset_info,
		  new_base_regnum,
		  m_offset_lower_bound, m_offset_upper_bound);
  unsigned i;
  rtx insn;
  insn = emit_insn_before (new_base_reg_info.set_insns[0],
			   (*load_store_info)[0].insn);
  if (new_base_reg_info.n_set_insns > 1)
    {
      gcc_assert (new_base_reg_info.n_set_insns == 2);
      emit_insn_before (new_base_reg_info.set_insns[1], insn);
    }

  for (i = 0; i < load_store_info->length (); ++i)
    {
      if (m_load_only_p && !(*load_store_info)[i].load_p)
	continue;

      if (!in_reg_class_p ((*load_store_info)[i].reg, m_allow_regclass))
	continue;

      HOST_WIDE_INT offset = (*load_store_info)[i].offset;

      if (new_base_reg_info.need_adjust_offset_p)
	offset = offset + new_base_reg_info.adjust_offset;

      load_store_insn =
	gen_reg_plus_imm_load_store ((*load_store_info)[i].reg,
				     new_base_reg_info.reg,
				     offset,
				     (*load_store_info)[i].load_p,
				     (*load_store_info)[i].mem);

      emit_insn_before (load_store_insn, (*load_store_info)[i].insn);

      delete_insn ((*load_store_info)[i].insn);
    }

  /* Recompute it CFG, to update BB_END() instruction.  */
  compute_bb_for_insn ();
}

static new_base_reg_info_t
gen_new_base (rtx original_base_reg,
	      offset_info_t offset_info,
	      unsigned new_base_regno,
	      HOST_WIDE_INT offset_lower,
	      HOST_WIDE_INT offset_upper)
{
  new_base_reg_info_t new_base_reg_info;

  /* Use gen_raw_REG instead of gen_rtx_REG to prevent break the reg
     info for global one.
     For example, gen_rtx_REG will return frame_pointer_rtx immediate
     instead of create new rtx for gen_raw_REG (Pmode, FP_REGNUM). */
  new_base_reg_info.reg = gen_raw_REG (Pmode, new_base_regno);

  /* Setup register info.  */
  ORIGINAL_REGNO (new_base_reg_info.reg) = ORIGINAL_REGNO (original_base_reg);
  REG_ATTRS (new_base_reg_info.reg) = REG_ATTRS (original_base_reg);

  if (offset_info.max_offset <= offset_upper
      && offset_info.min_offset >= offset_lower)
    {
      new_base_reg_info.set_insns[0] = gen_movsi (new_base_reg_info.reg,
						  original_base_reg);
      new_base_reg_info.n_set_insns = 1;
      new_base_reg_info.need_adjust_offset_p = false;
      new_base_reg_info.adjust_offset = 0;
    }
  else
    {
      /* For example:
	 lwi45.fe allow -4 ~ -128 range:
	 offset_lower = #-4
	 offset_upper = #-128

	 lwi $r2, [$r12 + #10]
	 ->
	 addi $r8, $r12, #14      ! $r8 = $r12 + #10 - offset_lower
				  ! = $r12 + #10 - #-4
				  ! = $r12 + #14
	 lwi45.fe $r2, [$r8 - #4] ! [$r8 - #4]
				  ! = [$r12 + #14 - #4]
				  ! = [$r12 + #10]
      */
      new_base_reg_info.adjust_offset =
	-(offset_info.min_offset - offset_lower);

      rtx offset = GEN_INT (-new_base_reg_info.adjust_offset);


      if (satisfies_constraint_Is15 (offset))
	{
	  new_base_reg_info.set_insns[0] =
	    gen_addsi3(new_base_reg_info.reg,
		       original_base_reg,
		       offset);

	  new_base_reg_info.n_set_insns = 1;
	}
      else
	{
	  if (!satisfies_constraint_Is20 (offset))
	    gcc_unreachable ();

	  new_base_reg_info.set_insns[1] =
	    gen_rtx_SET (VOIDmode,
			 new_base_reg_info.reg,
			 GEN_INT (-new_base_reg_info.adjust_offset));

	  new_base_reg_info.set_insns[0] =
	    gen_addsi3 (new_base_reg_info.reg,
			new_base_reg_info.reg,
			original_base_reg);

	  new_base_reg_info.n_set_insns = 2;
	}

      new_base_reg_info.need_adjust_offset_p = true;
    }

  return new_base_reg_info;
}

static bool
nds32_4byte_load_store_reg_plus_offset (
  rtx insn,
  load_store_info_t *load_store_info)
{
  if (!INSN_P (insn))
    return false;

  rtx pattern = PATTERN (insn);
  rtx mem = NULL_RTX;
  rtx reg = NULL_RTX;
  rtx base_reg = NULL_RTX;
  rtx addr;
  HOST_WIDE_INT offset = 0;
  bool load_p = false;

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
  else
    return false;

  /* At least need MIDDLE_REGS.  */
  if (!in_reg_class_p (reg, MIDDLE_REGS))
    return false;

  /* lwi450/swi450 */
  if (offset == 0)
    return false;

  if (in_reg_class_p (reg, LOW_REGS))
    {
      /* lwi37.sp/swi37.sp/lwi37/swi37 */
      if ((REGNO (base_reg) == SP_REGNUM
	   || REGNO (base_reg) == FP_REGNUM)
	  && (offset >= 0 && offset < 512 && (offset % 4 == 0)))
	return false;

      /* lwi333/swi333 */
      if (in_reg_class_p (base_reg, LOW_REGS)
	  && (offset >= 0 && offset < 32 && (offset % 4 == 0)))
	return false;
    }

  if (load_store_info)
    {
      load_store_info->load_p   = load_p;
      load_store_info->offset   = offset;
      load_store_info->reg      = reg;
      load_store_info->base_reg = base_reg;
      load_store_info->insn     = insn;
      load_store_info->mem      = mem;
    }

  if (GET_MODE (reg) != SImode)
    return false;

  return true;
}

static bool
nds32_4byte_load_store_reg_plus_offset_p (rtx insn)
{
  return nds32_4byte_load_store_reg_plus_offset (insn, NULL);
}

static bool
nds32_load_store_opt_profitable_p (basic_block bb)
{
  int candidate = 0;
  int threshold = 2;
  rtx insn;

  if (dump_file)
    fprintf (dump_file, "scan bb %d\n", bb->index);

  FOR_BB_INSNS (bb, insn)
    {
      if (nds32_4byte_load_store_reg_plus_offset_p (insn))
	candidate++;
    }

  if (dump_file)
    fprintf (dump_file, " candidate = %d\n", candidate);

  return candidate >= threshold;
}

static offset_info_t
nds32_get_offset_info (auto_vec<load_store_info_t, 64> *load_store_info)
{
  unsigned i;
  std::set<HOST_WIDE_INT> offsets;
  offset_info_t offset_info;
  offset_info.max_offset = 0;
  offset_info.min_offset = 0;
  offset_info.num_offset = 0;

  if (load_store_info->length () == 0)
    return offset_info;

  offset_info.max_offset = (*load_store_info)[0].offset;
  offset_info.min_offset = (*load_store_info)[0].offset;
  offsets.insert ((*load_store_info)[0].offset);

  for (i = 1; i < load_store_info->length (); i++)
    {
      HOST_WIDE_INT offset = (*load_store_info)[i].offset;
      offset_info.max_offset = MAX (offset_info.max_offset, offset);
      offset_info.min_offset = MIN (offset_info.min_offset, offset);
      offsets.insert (offset);
    }

  offset_info.num_offset = offsets.size ();

  return offset_info;
}

static void
nds32_do_load_store_opt (basic_block bb)
{
  rtx insn;
  load_store_info_t load_store_info;
  auto_vec<load_store_info_t, 64> load_store_infos[NDS32_GPR_NUM];
  HARD_REG_SET available_regset;
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
    fprintf (dump_file, "try load store opt for bb %d\n", bb->index);

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

      if (nds32_4byte_load_store_reg_plus_offset (insn, &load_store_info))
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
		       "load_p = %d UID = %u\n",
		       REGNO (load_store_infos[i][j].reg),
		       REGNO (load_store_infos[i][j].base_reg),
		       load_store_infos[i][j].offset,
		       load_store_infos[i][j].load_p,
		       INSN_UID (load_store_infos[i][j].insn));
	    }
	}

      nds32_get_available_reg_set (bb,
				   load_store_infos[i][0].insn,
				   load_store_infos[i].last ().insn,
				   &available_regset);

      if (dump_file)
	{
	  print_hard_reg_set (dump_file, "", available_regset);
	}

      offset_info_t offset_info = nds32_get_offset_info (&load_store_infos[i]);
      if (dump_file)
	{
	  fprintf (dump_file,
		   "max offset = " HOST_WIDE_INT_PRINT_DEC "\n"
		   "min offset = " HOST_WIDE_INT_PRINT_DEC "\n"
		   "num offset = %d\n",
		   offset_info.max_offset,
		   offset_info.min_offset,
		   offset_info.num_offset);
	}

      int gain;
      int best_gain = 0;
      const load_store_optimize_pass *best_load_store_optimize_pass = NULL;

      for (j = 0; j < N_LOAD_STORE_OPT_TYPE; ++j)
	{
	  gain = load_store_optimizes[j]->calc_gain (&available_regset,
						     offset_info,
						     &load_store_infos[i]);

	  if (dump_file)
	    fprintf (dump_file, "%s gain = %d\n",
		     load_store_optimizes[j]->name (), gain);

	  if (gain > best_gain)
	    {
	      best_gain = gain;
	      best_load_store_optimize_pass = load_store_optimizes[j];
	    }
	}

      if (best_load_store_optimize_pass)
	{
	  if (dump_file)
	    fprintf (dump_file, "%s is most profit, optimize it!\n",
		     best_load_store_optimize_pass->name ());

	  best_load_store_optimize_pass->do_optimize (&available_regset,
						      offset_info,
						      &load_store_infos[i]);

	  df_insn_rescan_all ();
	}

    }
}

static unsigned int
nds32_load_store_opt (void)
{
  basic_block bb;

  df_set_flags (DF_LR_RUN_DCE);
  df_note_add_problem ();
  df_analyze ();

  FOR_EACH_BB_FN (bb, cfun)
    {
      if (nds32_load_store_opt_profitable_p (bb))
	nds32_do_load_store_opt (bb);
    }

  return 1;
}

const pass_data pass_data_nds32_load_store_opt =
{
  RTL_PASS,				/* type */
  "load_store_opt",			/* name */
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

class pass_nds32_load_store_opt : public rtl_opt_pass
{
public:
  pass_nds32_load_store_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_load_store_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return TARGET_16_BIT && TARGET_LOAD_STORE_OPT; }
  unsigned int execute () { return nds32_load_store_opt (); }
};

rtl_opt_pass *
make_pass_nds32_load_store_opt (gcc::context *ctxt)
{
  return new pass_nds32_load_store_opt (ctxt);
}
