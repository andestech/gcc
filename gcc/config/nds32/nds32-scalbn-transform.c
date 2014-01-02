/* A Gimple-level pass of Andes NDS32 cpu for GNU compiler.
   This pass transforms the multiplications whose multiplier is a
   power of 2.

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


/* Return true if the current function name is scalbn/scalbnf, or its alias
   includes scalbn/scalbnf, otherwise return false.  */

static bool
nds32_is_scalbn_alias_func_p (void)
{
  int i;
  struct ipa_ref *ref;
  struct cgraph_node *cfun_node;

  if (!strcmp (function_name (cfun), "scalbn")
      || !strcmp (function_name (cfun), "scalbnf"))
    return true;

  cfun_node = cgraph_get_node (current_function_decl);

  if (!cfun_node)
    return false;

  for (i = 0; ipa_ref_list_referring_iterate (&cfun_node->ref_list,
					      i, ref); i++)
    if (ref->use == IPA_REF_ALIAS)
      {
	struct cgraph_node *alias = ipa_ref_referring_node (ref);
	if (!strcmp (alias->asm_name (), "scalbn")
	    || !strcmp (alias->asm_name (), "scalbnf"))
	  return true;
      }

  return false;
}

/* Return true if value of tree node RT is power of 2.  */

static bool
nds32_real_ispow2_p (tree rt)
{
  if (TREE_CODE (rt) != REAL_CST)
    return false;

  if (TREE_REAL_CST_PTR (rt)->cl != rvc_normal)
    return false;

  int i;
  for (i = 0; i < SIGSZ-1; ++i)
    if (TREE_REAL_CST_PTR (rt)->sig[i] != 0)
      return false;
  if (TREE_REAL_CST_PTR (rt)->sig[SIGSZ-1] != SIG_MSB)
    return false;

  return true;
}

/* Return the exponent of tree node RT in base 2.  */

static int
nds32_real_pow2exp (tree rt)
{
  return REAL_EXP (TREE_REAL_CST_PTR (rt)) - 1;
}

/* Return true if GS is the target of scalbn transform.  */

static bool
nds32_scalbn_transform_target_p (gimple gs)
{
  if (is_gimple_assign (gs))
    if ((gimple_assign_rhs_code (gs) == MULT_EXPR)
	&& (TREE_CODE (TREE_TYPE (gimple_assign_rhs1 (gs))) == REAL_TYPE)
	&& nds32_real_ispow2_p (gimple_assign_rhs2 (gs)))
      return true;
  return false;
}

/* Do scalbn transform for a GIMPLE statement GS.

   When the multiplier of GIMPLE statement GS is a positive number,
   GS will be transform to one gimple_call statement and one
   gimple_assign statement as follows:
   A = B * 128.0	-> temp = BUILT_IN_SCALBN (B, 7)
			   A = temp

   When the multiplier is a negative number, the multiplier will be
   conversed the sign first since BUILT_IN_SCALBN can't handle
   negative multiplier. The example is shown below:
   A = B * -128.0	-> temp = BUILT_IN_SCALBN (B, 7)
			   A = -temp
*/

static void
nds32_do_scalbn_transform (gimple gs)
{
  tree mult_cand = gimple_assign_rhs1 (gs);	/* Multiplicand  */
  tree mult_er = gimple_assign_rhs2 (gs);	/* Multiplier  */
  bool is_neg = false;

  /* Choose the function by type of arg.  */
  enum built_in_function fn_name;
  tree type = TREE_TYPE (mult_cand);
  if (TYPE_MAIN_VARIANT (type) == double_type_node)
    fn_name = BUILT_IN_SCALBN;
  else if (TYPE_MAIN_VARIANT (type) == float_type_node)
    fn_name = BUILT_IN_SCALBNF;
  /* Do not transform long double to scalbnl since some c library don't provide
     it if target don't have real long double type
  else if (TYPE_MAIN_VARIANT (type) == long_double_type_node)
    fn_name = BUILT_IN_SCALBNL;
  */
  else
    return;

  /* Converse the sign of negative number.  */
  if (REAL_VALUE_NEGATIVE (TREE_REAL_CST (mult_er)))
    {
      is_neg = true;
      mult_er = build_real (TREE_TYPE (mult_er),
			    real_value_negate (&TREE_REAL_CST (mult_er)));
    }

  /* Set function name for building gimple_call.  */
  tree fndecl = builtin_decl_explicit (fn_name);

  /* Set last arg for building gimple_call.  */
  tree exp = build_int_cst (integer_type_node,
			    nds32_real_pow2exp (mult_er));

  /* Build a new temp ssa.  */
  tree temp_call_ssa = make_ssa_name (TREE_TYPE (gimple_assign_lhs (gs)), NULL);

  /* Build gimple_call stmt to replace GS.  */
  gimple call_stmt = gimple_build_call (fndecl,
					2,
					mult_cand,
					exp);
  gimple_call_set_lhs (call_stmt, temp_call_ssa);

  enum tree_code subcode = NOP_EXPR;
  /* Handle negative value.  */
  if (is_neg)
    subcode = NEGATE_EXPR;

  /* Build gimple_assign for return value or change the sign.  */
  gimple assign_stmt =
    gimple_build_assign_with_ops (subcode,
				  gimple_assign_lhs (gs),
				  gimple_call_lhs (call_stmt),
				  NULL);

  /* Replace gimple_assign GS by new gimple_call.  */
  gimple_stmt_iterator gsi = gsi_for_stmt (gs);
  update_stmt (call_stmt);
  gsi_insert_before (&gsi, call_stmt, GSI_NEW_STMT);

  /* Insert the gimple_assign after the scalbn call.  */
  update_stmt (assign_stmt);
  gsi_next (&gsi);
  gsi_replace (&gsi, assign_stmt, false);
}

/* Do scalbn transform for each basic block BB.  */

static int
nds32_scalbn_transform_basic_block (basic_block bb)
{
  gimple_stmt_iterator gsi;
  int transform_number = 0;

  if (dump_file)
    fprintf (dump_file,
	     "\n;; Transforming the multiplication for basic block %d\n",
	     bb->index);

  for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      gimple stmt = gsi_stmt (gsi);

      if (nds32_scalbn_transform_target_p (stmt))
	{
	  if (dump_file)
	    {
	      fprintf (dump_file,
		       "* The multiplier of stmt %d is transforming.\n",
		       gimple_uid (stmt));
	      print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM|TDF_RAW);
	    }
	  nds32_do_scalbn_transform (stmt);
	  transform_number++;
	}
    }

  return transform_number;
}

/* This function is the entry of scalbn transform pass.  */

static int
nds32_scalbn_transform_opt (void)
{
  basic_block bb;
  int total_transform_number = 0;

  /* Ignore current and builtin function name are the same.  */
  if (nds32_is_scalbn_alias_func_p ())
    {
      if (dump_file)
	fprintf (dump_file,
		 "* Ignore function %s. "
		 "Transform it will cause infinite loop.\n",
		 function_name (cfun));
      return 0;
    }

  FOR_EACH_BB_FN (bb, cfun)
    {
      total_transform_number += nds32_scalbn_transform_basic_block (bb);
    }

  if (dump_file)
    {
      if (total_transform_number > 0)
	fprintf (dump_file,
		 "\n;; Transform %d multiplication stmt in function %s\n",
		 total_transform_number,
		 current_function_name ());
      else
	fprintf (dump_file,
		 "\n;; No multiplication stmt is transformed in function %s\n",
		 current_function_name ());
    }

  return 1;
}

static bool
gate_nds32_scalbn_transform (void)
{
  return flag_nds32_scalbn_transform
    && !TARGET_FPU_SINGLE
    && !flag_no_builtin;
}

const pass_data pass_data_nds32_scalbn_transform_opt =
{
  GIMPLE_PASS,				/* type */
  "scalbn_transform",			/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  true,					/* has_gate */
  true,					/* has_execute */
  TV_MACH_DEP,				/* tv_id */
  ( PROP_cfg | PROP_ssa ),		/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  ( TODO_update_ssa | TODO_verify_ssa | TODO_verify_stmts), /* todo_flags_finish */
};

class pass_nds32_scalbn_transform_opt : public gimple_opt_pass
{
public:
  pass_nds32_scalbn_transform_opt (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_nds32_scalbn_transform_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return gate_nds32_scalbn_transform (); }
  unsigned int execute () { return nds32_scalbn_transform_opt (); }
};

gimple_opt_pass *
make_pass_nds32_scalbn_transform_opt (gcc::context *ctxt)
{
  return new pass_nds32_scalbn_transform_opt (ctxt);
}
