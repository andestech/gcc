/* A Gimple-level pass of Andes NDS32 cpu for GNU compiler that
   converse the sign of constant operand when the FPU do not be
   accessed.

   Copyright (C) 2012-2015 Free Software Foundation, Inc.
   Contributed by Andes Technology Corporation.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "line-map.h"
#include "params.h"
#include "flags.h"
#include "tree.h"
#include "varasm.h"
#include "stor-layout.h"
#include "basic-block.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "is-a.h"
#include "gimple.h"
#include "gimplify.h"
#include "gimple-iterator.h"
#include "gimplify-me.h"
#include "gimple-ssa.h"
#include "cgraph.h"
#include "tree-cfg.h"
#include "tree-phinodes.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "tree-pass.h"
#include "gimple-pretty-print.h"
#include "gimple-fold.h"

/* Return true if GS is the target of sign conversion.  */

static bool
nds32_sign_conversion_target_p (gimple gs)
{
  if (is_gimple_assign (gs))
    if ((gimple_assign_rhs_code (gs) == MINUS_EXPR)
	&& (TREE_CODE (gimple_assign_rhs2 (gs)) == REAL_CST))
      return true;
  return false;
}

/* Do sign conversion for a GIMPLE statement GS.  */

static void
nds32_do_sign_conversion (gimple gs)
{
  /* Rewrite the rhs operand.  */
  enum tree_code op_code = gimple_assign_rhs_code (gs);
  op_code = PLUS_EXPR;
  gimple_assign_set_rhs_code (gs, op_code);
  /* Rewrite the constant value.  */
  tree rhs2 = gimple_assign_rhs2 (gs);
  rhs2 = build_real (TREE_TYPE (rhs2),
		     real_value_negate (&TREE_REAL_CST (rhs2)));
  gimple_assign_set_rhs2 (gs, rhs2);
  /* When the statement is modified, please mark this statement is modified.  */
  update_stmt (gs);
}

/* Do sign conversion for each basic block BB.  */

static int
nds32_sign_conversion_basic_block (basic_block bb)
{
  gimple_stmt_iterator gsi;
  int converse_number = 0;

  if (dump_file)
    fprintf (dump_file,
	     "\n;; Conversing the sign of gimple stmts for basic block %d\n",
	     bb->index);

  for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      gimple stmt = gsi_stmt (gsi);

      if (nds32_sign_conversion_target_p (stmt))
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, "* The sign of stmt %d is conversing.\n",
		       gimple_uid (stmt));
	      print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM|TDF_RAW);
	    }
	  nds32_do_sign_conversion (stmt);
	  converse_number++;
	}
    }

  return converse_number;
}

/* This function is the entry of sign conversion pass.  */

static int
nds32_sign_conversion_opt (void)
{
  basic_block bb;
  int total_converse_number = 0;

  FOR_EACH_BB_FN (bb, cfun)
    {
      total_converse_number += nds32_sign_conversion_basic_block (bb);
    }

  if (dump_file)
    {
      if (total_converse_number > 0)
	fprintf (dump_file, "\n;; Converse %d stmts in function %s\n",
		 total_converse_number,
		 current_function_name ());
      else
	fprintf (dump_file,
		 "\n;; No sign of stmt is conversed in function %s\n",
		 current_function_name ());
    }

  return 1;
}

const pass_data pass_data_nds32_sign_conversion_opt =
{
  GIMPLE_PASS,				/* type */
  "sign_conversion",			/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  true,					/* has_gate */
  true,					/* has_execute */
  TV_MACH_DEP,				/* tv_id */
  ( PROP_cfg | PROP_ssa ),		/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_verify_ssa,			/* todo_flags_finish */
};

class pass_nds32_sign_conversion_opt : public gimple_opt_pass
{
public:
  pass_nds32_sign_conversion_opt (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_nds32_sign_conversion_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return flag_nds32_sign_conversion && !TARGET_FPU_SINGLE; }
  unsigned int execute () { return nds32_sign_conversion_opt (); }
};

gimple_opt_pass *
make_pass_nds32_sign_conversion_opt (gcc::context *ctxt)
{
  return new pass_nds32_sign_conversion_opt (ctxt);
}
