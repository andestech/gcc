/* A Gimple-level pass of Andes NDS32 cpu for GNU compiler.
   This pass collects the usage of float-point.

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
#include "tree.h"
#include "basic-block.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "is-a.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "tree-cfg.h"
#include "tree-pass.h"
#include "gimple-pretty-print.h"
#include "gimple-walk.h"

/* Indicate the translation unit whether including floating-point arithmetic
   or not.  */
bool nds32_include_fp_arith = false;

/* Return true if the return type and argument types of current function
   pass the insepction. Furthermore, the global value NDS32_INCLUDE_FP_ARITH
   is modified.  */

static bool
nds32_acd_func_rtn_args_check (tree fn_decl)
{
  tree fn_type = TREE_TYPE (fn_decl);
  function_args_iterator iter;
  tree arg_type = NULL_TREE;
  tree rtn_type = NULL_TREE;
  unsigned argno = 1;

  gcc_assert (fn_type);

  rtn_type = TREE_TYPE (fn_type);
  if (dump_file)
    {
      fprintf (dump_file,
	       " Check the return & arguments for function %s\n"
	       "  Prototype:",
	       fndecl_name (fn_decl));
      print_generic_decl (dump_file, fn_decl, 0);
      fprintf (dump_file, "\n");
    }

  /* Check the return type.  */
  if (FLOAT_TYPE_P (rtn_type)
      || RECORD_OR_UNION_TYPE_P (rtn_type))
    {
      if (dump_file)
	fprintf (dump_file, "  ! Return type is FP or record/union type\n");
      nds32_include_fp_arith = true;

      return false;
    }

  /* Check if the function has a variable argument list.  */
  if (stdarg_p (fn_type))
    {
      if (dump_file)
	fprintf (dump_file, "  ! Has variable argument list (i.e. ,...)\n");
      nds32_include_fp_arith = true;

      return false;
    }

  /* Check the arguments.  */
  FOREACH_FUNCTION_ARGS (fn_type, arg_type, iter)
    {
      if (arg_type == void_type_node)
	break;

      if (FLOAT_TYPE_P (arg_type)
	  || RECORD_OR_UNION_TYPE_P (arg_type))
	{
	  if (dump_file)
	    fprintf (dump_file,
		     "  ! No.%d argument is FP or record/union type\n",
		     argno);
	  nds32_include_fp_arith = true;

	  return false;
	}
      argno++;
    }

  if (dump_file)
    fprintf (dump_file,
	     "  >> Pass the inspection of return & arguments type\n");

  return true;
}

/* Helper for nds32_abi_compatible. Return *TP if it is a floating-point
   -related operand.  */

static tree
nds32_acd_walk_op_fn (tree *tp, int *walk_subtrees, void *data ATTRIBUTE_UNUSED)
{
  tree t = *tp;

  if (t && TREE_TYPE (t)
      && (FLOAT_TYPE_P (TREE_TYPE (t))
	  || TREE_CODE (t) == REAL_CST
	  || TREE_CODE (t) == COMPLEX_CST
	  || TREE_CODE (t) == FLOAT_EXPR
	  || TREE_CODE (t) == REALPART_EXPR))
    {
      *walk_subtrees = 0;
      return t;
    }

  return NULL_TREE;
}

/* Helper for nds32_abi_compatible. Return non-NULL tree and set
   *HANDLED_OPS_P to true if *GSI_P is an ASM stmt.  */

static tree
nds32_acd_walk_stmt_fn (gimple_stmt_iterator *gsi_p, bool *handled_ops_p,
		       	struct walk_stmt_info *wi ATTRIBUTE_UNUSED)
{
  gimple stmt = gsi_stmt (*gsi_p);

  switch (gimple_code (stmt))
    {
    case GIMPLE_DEBUG:
      *handled_ops_p = true;
      break;

    case GIMPLE_ASM:
      *handled_ops_p = true;
      return (tree) -1;
      break;

    case GIMPLE_CALL:
	{
	  tree call_decl = gimple_call_fndecl (stmt);
	  if (!call_decl
	      || !nds32_acd_func_rtn_args_check (call_decl))
	    {
	      *handled_ops_p = true;
	      return call_decl;
	    }
	}
      break;

    default:
      break;
    }

  return NULL_TREE;
}

/* This function is the entry of ABI compatible detection pass.  */

static int
nds32_abi_compatible (void)
{
  basic_block bb;
  struct walk_stmt_info wi;

  memset (&wi, 0, sizeof (wi));

  if (!nds32_acd_func_rtn_args_check (current_function_decl))
    return 0;

  if (dump_file)
    fprintf (dump_file, "Check function body %s\n",
	     function_name (cfun));

  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple ret;
      gimple_seq seq = bb_seq (bb);

      ret = walk_gimple_seq (seq,
			     nds32_acd_walk_stmt_fn,
			     nds32_acd_walk_op_fn,
			     &wi);
      if (ret != NULL)
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, " ! NO PASS: ");
	      print_gimple_stmt (dump_file, ret, 0, TDF_SLIM|TDF_RAW);
	    }
	  nds32_include_fp_arith = true;
	  break;
	}
    }

  if (dump_file)
    if (!nds32_include_fp_arith)
      fprintf (dump_file,
	       " >> Pass the inspection of FP operand for function body\n");

  return 0;
}

static bool
gate_nds32_abi_compatible (void)
{
  return flag_nds32_abi_compatible
    && !nds32_include_fp_arith;
}

const pass_data pass_data_nds32_abi_compatible =
{
  GIMPLE_PASS,				/* type */
  "abi_compatible",			/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  true,					/* has_gate */
  true,					/* has_execute */
  TV_MACH_DEP,				/* tv_id */
  ( PROP_cfg | PROP_ssa ),		/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0,					/* todo_flags_finish */
};

class pass_nds32_abi_compatible : public gimple_opt_pass
{
public:
  pass_nds32_abi_compatible (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_nds32_abi_compatible, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return gate_nds32_abi_compatible (); }
  unsigned int execute () { return nds32_abi_compatible (); }
};

gimple_opt_pass *
make_pass_nds32_abi_compatible (gcc::context *ctxt)
{
  return new pass_nds32_abi_compatible (ctxt);
}
