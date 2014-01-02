/* Operand commutative for soft floating point arithmetic pass
   of Andes NDS32 cpu for GNU compiler
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
#include "tree.h"
#include "function.h"
#include "expr.h"
#include "df.h"
#include "tree-pass.h"

#define SF_ARG0_REGNO 0
#define SF_ARG1_REGNO 1

#define DF_ARG0_REGNO 0
#define DF_ARG1_REGNO 2

static int
nds32_soft_fp_arith_comm_opt (void)
{
  basic_block bb;
  rtx insn;
  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS (bb, insn)
	{
	  if (!CALL_P (insn))
	    continue;

	  rtx pat = PATTERN (insn);

	  if (GET_CODE (pat) != PARALLEL)
	    continue;

	  rtx call_rtx = XVECEXP (pat, 0, 0);

	  if (GET_CODE (call_rtx) == SET)
	    call_rtx = SET_SRC (call_rtx);

	  rtx func_mem = XEXP (call_rtx, 0);
	  rtx symbol = XEXP (func_mem, 0);

	  if (GET_CODE (symbol) != SYMBOL_REF)
	    continue;

	  const char *func_name = XSTR (symbol, 0);
	  bool df_p;
	  if (((strcmp("__mulsf3", func_name) == 0)
	       || (strcmp("__addsf3", func_name) == 0)))
	    df_p = false;
	  else if (((strcmp("__muldf3", func_name) == 0)
		   || (strcmp("__adddf3", func_name) == 0)))
	    df_p = true;
	  else
	    continue;

	  rtx prev_insn = insn;
	  rtx arg0_insn = NULL_RTX;
	  rtx arg1_insn = NULL_RTX;
	  unsigned arg0_regno = df_p ? DF_ARG0_REGNO : SF_ARG0_REGNO;
	  unsigned arg1_regno = df_p ? DF_ARG1_REGNO : SF_ARG1_REGNO;
	  enum machine_mode mode = df_p ? DFmode : SFmode;
	  while ((prev_insn = PREV_INSN (prev_insn)) && prev_insn)
	    {
	      if (arg0_insn != NULL_RTX && arg1_insn != NULL_RTX)
		break;

	      if (BLOCK_FOR_INSN (prev_insn) != BLOCK_FOR_INSN (insn))
		break;

	      if (!NONJUMP_INSN_P (prev_insn))
		break;

	      if (!INSN_P (prev_insn))
		continue;

	      rtx set = PATTERN (prev_insn);

	      if (GET_CODE (set) != SET)
		continue;

	      rtx dst_reg = SET_DEST (set);

	      if (!REG_P (dst_reg))
		break;

	      unsigned regno = REGNO (dst_reg);

	      if (regno == arg0_regno)
		{
		  arg0_insn = prev_insn;
		  continue;
		}
	      else if (regno == arg1_regno)
		{
		  arg1_insn = prev_insn;
		  continue;
		}
	      break;
	    }
	  if (arg0_insn == NULL_RTX || arg1_insn == NULL_RTX)
	   continue;

	  rtx arg0_src = SET_SRC (PATTERN (arg0_insn));
	  rtx arg1_src = SET_SRC (PATTERN (arg1_insn));

	  if ((REG_P (arg0_src)
	       && GET_MODE (arg0_src) == mode
	       && REGNO (arg0_src) == arg1_regno)
	      || (REG_P (arg1_src)
		  && GET_MODE (arg1_src) == mode
		  && REGNO (arg1_src) == arg0_regno))
	    {
	      /* Swap operand! */
	      rtx tmp = SET_DEST (PATTERN (arg0_insn));
	      SET_DEST (PATTERN (arg0_insn)) = SET_DEST (PATTERN (arg1_insn));
	      SET_DEST (PATTERN (arg1_insn)) = tmp;
	    }
	}
    }
  return 1;
}

const pass_data pass_data_nds32_soft_fp_arith_comm_opt =
{
  RTL_PASS,				/* type */
  "soft_fp_arith_comm",			/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  true,					/* has_gate */
  true,					/* has_execute */
  TV_MACH_DEP,				/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_verify_rtl_sharing,		/* todo_flags_finish */
};

class pass_nds32_soft_fp_arith_comm_opt : public rtl_opt_pass
{
public:
  pass_nds32_soft_fp_arith_comm_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_soft_fp_arith_comm_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return TARGET_SOFT_FP_ARITH_COMM && !TARGET_FPU_SINGLE; }
  unsigned int execute () { return nds32_soft_fp_arith_comm_opt (); }
};

rtl_opt_pass *
make_pass_nds32_soft_fp_arith_comm_opt (gcc::context *ctxt)
{
  return new pass_nds32_soft_fp_arith_comm_opt (ctxt);
}
