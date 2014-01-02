/* Switch shortcutting optimization for GNU C
   Copyright (C) 2013 Free Software Foundation, Inc.
   Contributed by Steve Ellcey (sellcey@imgtec.com).

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

/* This file implements an optimization where, when a variable is set
   to a constant value and there is a path that leads from this definition
   to a switch statement that uses that variable as its controlling expression
   we duplicate the blocks on this path and change the switch goto to a
   direct goto to the label of the switch block that control would goto based
   on the value of the variable.  */

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
#include "cfgloop.h"
#include "pointer-set.h"

#include "tree-inline.h"
#include "tree-ssa-alias.h"
#include "tree-into-ssa.h"
#include "tree-pass.h"

#if 0
#include "tree.h"
#include "internal-fn.h"
//#include "tree-flow.h"
//#include "tree-flow-inline.h"
#include "basic-block.h"
#include "gimple.h"
#include "cfgloop.h"
#endif
#include "params.h"

/* Helper function for find_path, visited_bbs is used to make sure we don't
   fall into an infinite loop.  */

static int
find_path_1(basic_block start_bb, basic_block end_bb, struct pointer_set_t *visited_bbs)
{
  edge_iterator ei;
  edge e;

  if (start_bb == end_bb) return 1;

  if (!pointer_set_insert (visited_bbs, start_bb))
    {
      FOR_EACH_EDGE (e, ei, start_bb->succs)
	if (find_path_1 (e->dest, end_bb, visited_bbs))
	  return 1;
    }
    return 0;
}

/* Return 1 if there is a path from start_bb to end_bb and 0 if there
   is not.  There may be multiple paths from start_bb to end_bb.  */

static int
find_path(basic_block start_bb, basic_block end_bb)
{
  edge_iterator ei;
  edge e;
  struct pointer_set_t *visited_bbs;
  int p = 0;

  if (start_bb == end_bb) return 1;

  visited_bbs = pointer_set_create ();
  if (!pointer_set_insert (visited_bbs, start_bb))
    {
      FOR_EACH_EDGE (e, ei, start_bb->succs)
	if (find_path_1 (e->dest, end_bb, visited_bbs))
	  {
	    p = 1;
	    break;
	  }
    }
  pointer_set_destroy (visited_bbs);
  return p;
}


/* We save the paths we want to copy in bbs_list_array.  n_bbs_list is the
   number of paths saved, bbs_list_array[i] is the list of basic blocks in
   one path.  Each path starts with the block where a variable is assigned
   a constant value (bbs_list_array[i][0]) and ends with the switch statement
   block (bbs_list_array[i][bbs_list_size[i]-2]) and then the block that the
   switch statement is going to go to given the constant value of the
   variable (bbs_list_array[i][bbs_list_size[i]-1]).  */

static basic_block **bbs_list_array;
static int *val_array;
static int *bbs_list_size;
static int max_path_count;
static int max_insn_count;
static int n_bbs_list;

/* bbs_list[0] is the block with the switch statement,
   bbs_list[n-1] is the block where the switch statement variable is assigned
     a constant value,
   The entries in between make a (reverse) path between the two.

   We don't want to change bb_list, we want to leave that alone and
   and copy the path to bbs_list_array so that we wind up with a list (array)
   of paths that we want to update.  We also want to add the block that the
   switch is going to go to on to the list so that we know which exit from
   the switch statement is important.  */

static void
save_new_path (basic_block *bbs_list, int n, tree val)
{
  int i;
  int insn_count;
  basic_block bb;
  edge switch_taken_edge;
  gimple_stmt_iterator gsi;

  if (n <= 1) return;

  if (n_bbs_list >= max_path_count)
    return;

  /* Put the blocks in 'correct' order and add in where we want to go after
     the switch statement, We want to leave bbs_list untouched for future
     calls.  */

  bbs_list_array[n_bbs_list] = XNEWVEC (basic_block, n+1);
  for (i = 0; i < n; i++)
    bbs_list_array[n_bbs_list][i] = bbs_list[n-i-1];

  switch_taken_edge = find_taken_edge (bbs_list[0], val);
  bbs_list_array[n_bbs_list][n] = switch_taken_edge->dest;

  bbs_list_size[n_bbs_list] = n + 1;
  val_array[n_bbs_list] = (int) TREE_INT_CST_LOW (val);

  /* Count how many instructions are in the blocks we are going to
     duplicate and if there are too many do not save this path
     (return without incrementing n_bbs_list).  */

  insn_count = 0;
  for (i = 1; i < n; i++)
    {
      bb = bbs_list_array[n_bbs_list][i];
      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
	insn_count += estimate_num_insns (gsi_stmt (gsi), &eni_size_weights);
    }

  if (insn_count > max_insn_count)
    return;

  n_bbs_list = n_bbs_list + 1;
}

/* switch_stmt is a switch statement whose switch index expression
   is the variable expr.  We trace the value of the variable back
   through any phi nodes looking for places where it gets a constant
   value and save the path in bbs_list.  Then we call save_new_path
   to create a list of such paths.  */

static void
process_switch (tree expr, gimple switch_stmt,
	        struct pointer_set_t *visited_phis,
	        basic_block *bbs_list, int n)
{
  gimple def_stmt;
  tree var;
  unsigned int i;
  edge e;
  edge_iterator ei;
  basic_block bbx;
  basic_block var_bb;
  int e_count;

  gcc_assert (gimple_code (switch_stmt) == GIMPLE_SWITCH);
  var = SSA_NAME_VAR (expr);
  def_stmt = SSA_NAME_DEF_STMT (expr);
  var_bb = gimple_bb (def_stmt);

  if (var == NULL || var_bb == NULL) return;

  /* We have a variable definition (var) that is defined in var_bb,
     We want to put the path from var_bb to the current bb into the
     bbs_list.  If there is more then one path, skip this and don't
     try to do the optimization.  */

  bbx = bbs_list[n-1];
  while (bbx != var_bb)
   {
     e_count = 0;
     FOR_EACH_EDGE (e, ei, bbx->preds)
       {
         if (find_path (var_bb, e->src))
	   {
	     bbs_list[n] = e->src;
	     n = n + 1;
	     e_count = e_count + 1;
	   }
       }
     if (e_count != 1) return;
     bbx = bbs_list[n-1];
   }

  if ((gimple_code (def_stmt) == GIMPLE_PHI)
       && !pointer_set_insert (visited_phis, def_stmt))
    {
      for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
	{
	  tree arg = gimple_phi_arg_def (def_stmt, i);
	  if (arg && (TREE_CODE (arg) == INTEGER_CST))
	    {
	      /* const char *name = IDENTIFIER_POINTER (DECL_NAME (var)); */
	      bbs_list[n] = gimple_phi_arg_edge (def_stmt, i)->src;
	      save_new_path(bbs_list, n + 1, arg);
	    }
	  else if (arg && (TREE_CODE (arg) == SSA_NAME))
	    {
		  bbs_list[n] = gimple_phi_arg_edge (def_stmt, i)->src;
		  process_switch (arg, switch_stmt, visited_phis, bbs_list, n+1);
	    }
	}
    }
}

/* Find paths that lead from blocks where a variable is assigned a constant
   value to a switch statement where that variable is used as the switch
   index.  Save the paths in bbs_list_array so that they can be processed
   by copy_switch_paths.  */

static unsigned int
find_switch_shortcuts (void)
{
  basic_block bb;
  struct pointer_set_t *visited_phis;
  basic_block *bbs_list;
  int n = 1;

  bbs_list = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun));
  visited_phis = pointer_set_create ();
  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple stmt = last_stmt (bb);
      if (stmt && gimple_code (stmt) == GIMPLE_SWITCH)
	{
	  tree op = gimple_switch_index (stmt);
	  tree var = SSA_NAME_VAR (op);
	  if (var)
	    {
	      bbs_list[0] = bb;
	      process_switch (op, stmt, visited_phis, bbs_list, n);
	    }
	}
    }
  pointer_set_destroy (visited_phis);
  XDELETEVEC (bbs_list);
  return 0;
}

/* Call gimple_duplicate_sese_region to douplicate the blocks in bb_list.
   We free and recalculate all ssa and dominance information afterwords
   because the regsion being copied is not really SESE and so we cannot
   trust gimple_duplicate_sese_region to correctly update the dataflow
   information.  */

static void
duplicate_blocks (basic_block *bb_list, int bb_count)
{
  edge orig_edge, exit_edge;

  orig_edge = find_edge (bb_list[0], bb_list[1]);
  exit_edge = find_edge (bb_list[bb_count-2], bb_list[bb_count-1]);
  gimple_duplicate_sese_region (orig_edge, exit_edge, &bb_list[1], bb_count-2, NULL, true);
  free_dominance_info (CDI_DOMINATORS);
  update_ssa (TODO_update_ssa);
  calculate_dominance_info (CDI_DOMINATORS);
}

/* Go through the paths saved in bbs_list_array and make copies of them.  */

static void
copy_switch_paths (void)
{
  int i;

  /* Process each path in bbs_list_size.  */
  for (i = 0; i < n_bbs_list; i++)
    {
    /* For each path in bbs_list_size loop through and copy each block in
       the path (except the first on where the constant is assigned and
       the final one where the switch statement goes to.  */

    if (!single_pred_p (bbs_list_array[i][1]))
      duplicate_blocks (bbs_list_array[i], bbs_list_size[i]);
    }
}

static unsigned int
do_switch_shortcut (void)
{
  int i;

  n_bbs_list = 0;
  max_insn_count = PARAM_VALUE (PARAM_MAX_SWITCH_INSNS);
  max_path_count = PARAM_VALUE (PARAM_MAX_SWITCH_PATHS);
  val_array = XNEWVEC (int, max_path_count);
  bbs_list_size = XNEWVEC (int, max_path_count);
  bbs_list_array = XNEWVEC (basic_block *, max_path_count);
  find_switch_shortcuts ();
  copy_switch_paths ();
  XDELETEVEC (val_array);
  XDELETEVEC (bbs_list_size);
  for (i = 0; i < n_bbs_list; i++)
    XDELETEVEC (bbs_list_array[i]);
  XDELETEVEC (bbs_list_array);
  return 0;
}

/* The pass gate. */

static bool
gate_switch_shortcut (void)
{
   return flag_tree_switch_shortcut;
}

namespace {
#if 0
struct gimple_opt_pass pass_switch_shortcut =
{
 {
  GIMPLE_PASS,
  "switch_shortcut",			/* name */
  OPTGROUP_NONE,                        /* optinfo_flags */
  gate_switch_shortcut,			/* gate */
  do_switch_shortcut,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_SWITCH_SHORTCUT,			/* tv_id */
  PROP_cfg | PROP_ssa,			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_cleanup_cfg | TODO_verify_all,	/* todo_flags_finish */
 }
};
#endif

const pass_data pass_data_switch_shortcut =
{
  GIMPLE_PASS, /* type */
  "switch_shortcut", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  true, /* has_gate */
  true, /* has_execute */
  TV_SWITCH_SHORTCUT, /* tv_id */
  ( PROP_cfg | PROP_ssa ), /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  ( TODO_cleanup_cfg | TODO_verify_all), /* todo_flags_finish */
};

class pass_switch_shortcut : public gimple_opt_pass
{
public:
  pass_switch_shortcut (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_switch_shortcut, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return gate_switch_shortcut (); }
  unsigned int execute () { return do_switch_shortcut (); }

}; // class pass_convert_switch


}

gimple_opt_pass *
make_pass_switch_shortcut (gcc::context *ctxt)
{
  return new pass_switch_shortcut (ctxt);
}
