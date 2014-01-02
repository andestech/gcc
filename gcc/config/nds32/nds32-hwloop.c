/* hwloop pass of Andes NDS32 cpu for GNU compiler
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
#include "tree.h"
#include "stor-layout.h"
#include "varasm.h"
#include "calls.h"
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
#include "optabs.h"		/* For GEN_FCN.  */
#include "target.h"
#include "target-def.h"
#include "langhooks.h"		/* For add_builtin_function().  */
#include "ggc.h"
#include "tree-pass.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "context.h"
#include "params.h"
#include "cpplib.h"
#include "hw-doloop.h"

/* A callback for the hw-doloop pass.  This function examines INSN; if
   it is a doloop_end pattern we recognize, return the reg rtx for the
   loop counter.  Otherwise, return NULL_RTX.  */
static rtx
hwloop_pattern_reg (rtx insn)
{
  rtx reg;

  if (!JUMP_P (insn) || recog_memoized (insn) != CODE_FOR_loop_end)
    return NULL_RTX;

  reg = SET_DEST (XVECEXP (PATTERN (insn), 0, 1));
  if (!REG_P (reg))
    return NULL_RTX;
  return reg;
}

/* Optimize Loop */
static bool
hwloop1_optimize (hwloop_info loop)
{
  basic_block bb, new_bb, outer_new_bb;
  edge e, outer_e;
  edge_iterator ei, outer_ei;
  rtx insn, last_insn, cfg_insn, recog_insn;
  rtx start_label;
  rtx iter_reg;
  rtx seq, seq_end;
  hwloop_info inner;
  unsigned ix;
  bool same_depth_p = false;
  int hwloop_id = cfun->machine->hwloop_group_id;

  /* Check unspec_hwloop pattern on first basic block.  */
  for (insn = BB_HEAD (loop->tail) ; insn != BB_END (loop->tail);
       insn = NEXT_INSN (insn))
    {
      if (recog_memoized (insn) == CODE_FOR_unspec_no_hwloop)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; loop %d has bad instruction on first BB\n",
		     loop->loop_no);
	  return false;
	}
    }

  /* Check unspec_hwloop pattern on last basic block.  */
  for (insn = BB_HEAD (loop->head); insn != BB_END (loop->head);
       insn = NEXT_INSN (insn))
    {
      if (recog_memoized (insn) == CODE_FOR_unspec_no_hwloop)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; loop %d has bad instruction on last BB\n",
		     loop->loop_no);
	  return false;
	}
    }

  /* Check inner loop have Hardware loop.  */
  for (ix = 0; loop->loops.iterate (ix, &inner); ix++)
    {
      if ((loop->loop_no != inner->loop_no)
	  && !inner->bad)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; Inner loop %d have HW-loop in loop: %d\n",
		     inner->loop_no, loop->loop_no);
	  return false;
	}
    }

  /* Check if start_label appears before loop_end and.  */
  insn = loop->start_label;
  while (insn && insn != loop->loop_end)
    insn = NEXT_INSN (insn);

  if (!insn)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d start_label not before loop_end\n",
		 loop->loop_no);
      return false;
    }

  /* Scan all the blocks to make sure they don't use iter_reg.  */
  if (loop->iter_reg_used || loop->iter_reg_used_outside)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d uses iterator\n",
		 loop->loop_no);
      return false;
    }

  if (loop->has_call || loop->has_asm)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has invalid insn\n",
		 loop->loop_no);
      return false;
    }

  if (loop->jumps_outof)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d jumps out of loop body.\n",
		 loop->loop_no);
      return false;
    }

  if (!loop->incoming_dest)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has more than one entry\n",
		 loop->loop_no);
      return true;
    }

  if (loop->incoming_dest != loop->head)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d is not entered from head\n",
		 loop->loop_no);
      return true;
    }

  /* Check same loop depth in nesting loop.  */
  for (ix = 0; loop->outermost->loops.iterate (ix, &inner); ix++)
    {
      /* Check real_depth same other loop real_depth.  */
      if ((loop->loop_no != inner->loop_no)
	  && (loop->real_depth == inner->real_depth))
	same_depth_p = true;

      if (dump_file)
	{
	  fprintf (dump_file, ";;loop %d depth: %d",
		   loop->loop_no, loop->depth);
	  fprintf (dump_file, " inner %d depth %d\n",
		   inner->loop_no, inner->real_depth);
	}
    }

  /* Get the loop iteration register.  */
  iter_reg = loop->iter_reg;

  gcc_assert (REG_P (iter_reg));

  if (loop->incoming_src)
    {
      /* Make sure the predecessor is before the loop start label,
	 as required by the loop setup instructions.  */
      insn = BB_END (loop->incoming_src);

      if (vec_safe_length (loop->incoming) > 1
	  || !(loop->incoming->last ()->flags & EDGE_FALLTHRU))
	{
	  gcc_assert (JUMP_P (insn));

	  if (dump_file)
	    fprintf (dump_file, ";; loop %d loop setup space has jump insn,"
		     " before loop_start\n", loop->loop_no);
	  return true;
	}

      while (insn && insn != loop->start_label)
	insn = NEXT_INSN (insn);

      if (!insn)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; loop %d loop setup not before loop_start\n",
		     loop->loop_no);
	  return true;
	}
    }

  /* There should be an instruction before the loop_end instruction
     in the same basic block. And the instruction must not be
     - JUMP
     - CONDITIONAL BRANCH
     - CALL
     - Returns  */

  bb = loop->tail;
  last_insn = PREV_INSN (loop->loop_end);

  while (1)
    {
      for (; last_insn != BB_HEAD (bb);
	   last_insn = PREV_INSN (last_insn))
	if (NONDEBUG_INSN_P (last_insn))
	  break;

      if (last_insn != BB_HEAD (bb))
	break;

      if (single_pred_p (bb)
	  && single_pred_edge (bb)->flags & EDGE_FALLTHRU
	  && single_pred (bb) != ENTRY_BLOCK_PTR_FOR_FN (cfun))
	{
	  bb = single_pred (bb);
	  last_insn = BB_END (bb);
	  continue;
	}
      else
	{
	  last_insn = NULL;
	  break;
	}
    }

  if (!last_insn)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has no last instruction\n",
		 loop->loop_no);
      return true;
    }

  if (JUMP_P (last_insn) && !any_condjump_p (last_insn))
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has bad last instruction\n",
		 loop->loop_no);
      return true;
    }

  /* In all other cases, try to replace a bad last insn with a nop.  */
  if (JUMP_P (last_insn)
      || CALL_P (last_insn)
      || recog_memoized (last_insn) == CODE_FOR_return_internal
      || GET_CODE (PATTERN (last_insn)) == ASM_INPUT
      || asm_noperands (PATTERN (last_insn)) >= 0)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has bad last insn; replace with nop\n",
		 loop->loop_no);

      bb = BLOCK_FOR_INSN (last_insn);
      last_insn = emit_insn_after (gen_unspec_nop (), BB_HEAD (bb->next_bb));
    }

  loop->last_insn = last_insn;

  /* The loop is good for replacement.  */
  start_label = loop->start_label;
  iter_reg = loop->iter_reg;

  SET_REGNO_REG_SET (loop->regs_set_in_loop, LC_REGNUM);

  /* Create a sequence containing the loop setup.  */
  start_sequence ();

  if (loop->loop_no == loop->outermost->loop_no
      || same_depth_p)
    {
      /* Insert start place for LB.  */
      recog_insn = emit_insn (gen_mtlbi_hint (gen_rtx_LABEL_REF (Pmode,
								 start_label),
			      GEN_INT (hwloop_id)));
      recog_memoized (recog_insn);
    }

  /* Insert counter for LC.  */
  seq_end = emit_insn (gen_init_lc (iter_reg, GEN_INT (hwloop_id)));
  recog_memoized (seq_end);

  if (dump_file)
    {
      fprintf (dump_file, ";; replacing loop %d initializer with\n",
	       loop->loop_no);
      print_rtl_single (dump_file, seq_end);
      fprintf (dump_file, ";; replacing loop %d terminator with\n",
	       loop->loop_no);
      print_rtl_single (dump_file, loop->loop_end);
    }

  seq = get_insns ();
  end_sequence ();

  /* Create new basic block, before loop->head.  */
  emit_insn_before (seq, BB_HEAD (loop->head));
  seq = emit_label_before (gen_label_rtx (), seq);

  new_bb = create_basic_block (seq, seq_end, loop->head->prev_bb);

  /* Copy prev BB flags and frequency.  */
  BB_COPY_PARTITION (new_bb, new_bb->prev_bb);
  new_bb->frequency = new_bb->prev_bb->frequency;

  FOR_EACH_EDGE (e, ei, loop->incoming)
    {
      if (!(e->flags & EDGE_FALLTHRU)
	  || e->dest != loop->head)
	redirect_edge_and_branch_force (e, new_bb);
      else
	redirect_edge_succ (e, new_bb);
    }

  /* The new edge from outer_new_bb to loop->head
     is FALLTHRU.  */
  make_single_succ_edge (new_bb, loop->head, EDGE_FALLTHRU);

  /* Get loop_insn note and delete loop_end insn.   */
  rtx note = find_reg_note (loop->loop_end, REG_BR_PROB, 0);
  delete_insn (loop->loop_end);

  /* Insert the CFG information after the last instruction of the loop.  */
  cfg_insn = emit_jump_insn_after (gen_hwloop_cfg (GEN_INT (hwloop_id),
				   gen_rtx_LABEL_REF (Pmode, start_label)),
				   BB_END (loop->tail));
  if (note)
    add_int_reg_note (cfg_insn, REG_BR_PROB, INTVAL (note));

  recog_memoized (cfg_insn);
  JUMP_LABEL (cfg_insn) = loop->start_label;
  LABEL_NUSES (loop->start_label)++;

  if (loop->loop_no != loop->outermost->loop_no
      && !same_depth_p)
    {
      start_sequence ();
      /* Insert start place for LB.  */
      seq_end = emit_insn (gen_mtlbi_hint (gen_rtx_LABEL_REF (Pmode,
							      start_label),
			   GEN_INT (hwloop_id)));
      recog_memoized (seq_end);
      seq = get_insns ();
      end_sequence ();

      emit_insn_before (seq, BB_HEAD (loop->outermost->head));
      seq = emit_label_before (gen_label_rtx (), seq);

      /* Create new basic block, before loop->outermost->head.  */
      outer_new_bb = create_basic_block (seq, seq_end,
					 loop->outermost->head->prev_bb);

      /* Copy prev BB flags and frequency.  */
      BB_COPY_PARTITION (outer_new_bb, outer_new_bb->prev_bb);
      outer_new_bb->frequency = outer_new_bb->prev_bb->frequency;

      FOR_EACH_EDGE (outer_e, outer_ei, loop->outermost->incoming)
	{
	  if (!(outer_e->flags & EDGE_FALLTHRU)
	      || outer_e->dest != loop->outermost->head)
	    redirect_edge_and_branch_force (outer_e, outer_new_bb);
	  else
	    redirect_edge_succ (outer_e, outer_new_bb);
	}

      /* The new edge from outer_new_bb to loop->outermost->head
	 is FALLTHRU.  */
      make_single_succ_edge (outer_new_bb, loop->outermost->head,
			     EDGE_FALLTHRU);
    }
  cfun->machine->hwloop_group_id++;
  return true;
}

/* Optimize Loop */
static bool
hwloop2_optimize (hwloop_info loop)
{
  basic_block bb, loop_bb;
  rtx insn, last_insn, iter_reg;
  rtx start_label, end_label;
  rtx lc_reg, lb_reg, le_reg;
  rtx seq, seq_end;
  hwloop_info inner;
  unsigned ix;
  bool same_depth_p = false;

  if (loop->jumps_outof)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d jumps out of loop body.\n",
		 loop->loop_no);
      return false;
    }

  if (!loop->incoming_dest)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has more than one entry\n",
		 loop->loop_no);
      return false;
    }

  if (loop->incoming_dest != loop->head)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d is not entered from head\n",
		 loop->loop_no);
      return false;
    }

  if (loop->has_call || loop->has_asm)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has invalid insn\n",
		 loop->loop_no);
      return false;
    }

  /* Scan all the blocks to make sure they don't use iter_reg.  */
  if (loop->iter_reg_used || loop->iter_reg_used_outside)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d uses iterator\n",
		 loop->loop_no);
      return false;
    }

  /* Get the loop iteration register.  */
  iter_reg = loop->iter_reg;

  gcc_assert (REG_P (iter_reg));

  if (loop->incoming_src)
    {
      /* Make sure the predecessor is before the loop start label,
	 as required by the loop setup instructions.  */
      insn = BB_END (loop->incoming_src);

      if (vec_safe_length (loop->incoming) > 1
	  || !(loop->incoming->last ()->flags & EDGE_FALLTHRU))
	{
	  gcc_assert (JUMP_P (insn));

	  if (dump_file)
	    fprintf (dump_file, ";; loop %d loop setup space has jump insn,"
		     " before loop_start\n", loop->loop_no);
	  return false;
	}

      while (insn && insn != loop->start_label)
	insn = NEXT_INSN (insn);

      if (!insn)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; loop %d loop setup not before loop_start\n",
		     loop->loop_no);
	  return false;
	}
    }

  /* Check if start_label appears before loop_end and.  */
  insn = loop->start_label;
  while (insn && insn != loop->loop_end)
    insn = NEXT_INSN (insn);

  if (!insn)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d start_label not before loop_end\n",
		 loop->loop_no);
      return false;
    }

  /* There should be an instruction before the loop_end instruction
     in the same basic block. And the instruction must not be
     - JUMP
     - CONDITIONAL BRANCH
     - CALL
     - Returns  */

  bb = loop->tail;
  last_insn = PREV_INSN (loop->loop_end);

  while (1)
    {
      for (; last_insn != BB_HEAD (bb);
	   last_insn = PREV_INSN (last_insn))
	if (NONDEBUG_INSN_P (last_insn))
	  break;

      if (last_insn != BB_HEAD (bb))
	break;

      if (single_pred_p (bb)
	  && single_pred_edge (bb)->flags & EDGE_FALLTHRU
	  && single_pred (bb) != ENTRY_BLOCK_PTR_FOR_FN (cfun))
	{
	  bb = single_pred (bb);
	  last_insn = BB_END (bb);
	  continue;
	}
      else
	{
	  last_insn = NULL;
	  break;
	}
    }

  if (!last_insn)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has no last instruction\n",
		 loop->loop_no);
      return false;
    }

  if (JUMP_P (last_insn) && !any_condjump_p (last_insn))
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has bad last instruction\n",
		 loop->loop_no);
      return false;
    }

  /* Check unspec_hwloop pattern on first basic block.  */
  for (insn = BB_HEAD (loop->tail) ; insn != BB_END (loop->tail);
       insn = NEXT_INSN (insn))
    {
      if (recog_memoized (insn) == CODE_FOR_unspec_no_hwloop)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; loop %d has bad instruction on first BB\n",
		     loop->loop_no);
	  return false;
	}
    }

  /* Check unspec_hwloop pattern on last basic block.  */
  for (insn = BB_HEAD (loop->head); insn != BB_END (loop->head);
       insn = NEXT_INSN (insn))
    {
      if (recog_memoized (insn) == CODE_FOR_unspec_no_hwloop)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; loop %d has bad instruction on last BB\n",
		     loop->loop_no);
	  return false;
	}
    }

  /* Check inner loop have Hardware loop by hwloop pass.  */
  for (ix = 0; loop->blocks.iterate (ix, &loop_bb); ix++)
    {
      for (insn = BB_HEAD (loop_bb);
	   insn != NEXT_INSN (BB_END (loop_bb));
	   insn = NEXT_INSN (insn))
	{
	  if (recog_memoized (insn) == CODE_FOR_init_lc)
	    {
	      if (dump_file)
		fprintf (dump_file, ";; The inner loop %d has HW-loop\n",
			 loop->loop_no);
	      return false;
	    }
	}
    }

  /* Check inner loop have Hardware loop.  */
  for (ix = 0; loop->loops.iterate (ix, &inner); ix++)
    {
      if ((loop->loop_no != inner->loop_no)
	  && !inner->bad)
	{
	  if (dump_file)
	    fprintf (dump_file, ";; Inner loop %d have HW-loop in loop: %d\n",
		     inner->loop_no, loop->loop_no);
	  return false;
	}
    }

  /* Check outer loop have Hardware loop.  */
  for (ix = 0; loop->outermost->loops.iterate (ix, &inner); ix++)
    {
      if ((loop->loop_no != inner->loop_no)
	  && !inner->bad)
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, ";;loop %d depth: %d",
		       loop->loop_no, loop->depth);
	      fprintf (dump_file, " inner %d depth %d\n",
		       inner->loop_no, inner->real_depth);
	    }
	  return false;
	}
    }

  /* Check same loop depth in nesting loop.  */
  for (ix = 0; loop->outermost->loops.iterate (ix, &inner); ix++)
    {
      /* Check real_depth same other loop real_depth.  */
      if ((loop->loop_no != inner->loop_no)
	  && (loop->real_depth == inner->real_depth))
	same_depth_p = true;

      if (dump_file)
	{
	  fprintf (dump_file, ";;loop %d depth: %d",
		   loop->loop_no, loop->depth);
	  fprintf (dump_file, " inner %d depth %d\n",
		   inner->loop_no, inner->real_depth);
	}
    }

  /* In all other cases, try to replace a bad last insn with a nop.  */
  if (JUMP_P (last_insn)
      || CALL_P (last_insn)
      || recog_memoized (last_insn) == CODE_FOR_return_internal
      || GET_CODE (PATTERN (last_insn)) == ASM_INPUT
      || asm_noperands (PATTERN (last_insn)) >= 0)
    {
      if (dump_file)
	fprintf (dump_file, ";; loop %d has bad last insn; replace with nop\n",
		 loop->loop_no);
      last_insn = emit_insn_after (gen_unspec_nop (), last_insn);
    }

  loop->last_insn = last_insn;

  /* The loop is good for replacement.  */
  start_label = loop->start_label;
  end_label = gen_label_rtx ();
  iter_reg = loop->iter_reg;

  lb_reg = gen_rtx_REG (SImode, LB_REGNUM);
  le_reg = gen_rtx_REG (SImode, LE_REGNUM);
  lc_reg = gen_rtx_REG (SImode, LC_REGNUM);
  SET_REGNO_REG_SET (loop->regs_set_in_loop, LC_REGNUM);

  loop->end_label = end_label;

  /* Create a sequence containing the loop setup.  */
  start_sequence ();

  if (loop->loop_no == loop->outermost->loop_no
      || same_depth_p)
    {
      /* Insert start place for LB.  */
      emit_insn (gen_mtlbi (gen_rtx_LABEL_REF (Pmode, start_label)));
      /* Insert end place for LE.  */
      emit_insn (gen_mtlei (gen_rtx_LABEL_REF (Pmode, end_label)));

      emit_insn (gen_rtx_USE (SImode, lb_reg));
      emit_insn (gen_rtx_USE (SImode, le_reg));
    }

  /* Insert counter for LC.  */
  emit_move_insn (lc_reg, iter_reg);
  emit_insn (gen_rtx_USE (SImode, lc_reg));

 /* Insert ISB instruction.  */
  seq_end = emit_insn (gen_unspec_volatile_isb ());

  if (dump_file)
    {
      fprintf (dump_file, ";; replacing loop %d initializer with\n",
	       loop->loop_no);
      print_rtl_single (dump_file, seq_end);
      fprintf (dump_file, ";; replacing loop %d terminator with\n",
	       loop->loop_no);
      print_rtl_single (dump_file, loop->loop_end);
    }

  seq = get_insns ();
  end_sequence ();

  if (loop->incoming_src)
    {
      rtx prev = BB_END (loop->incoming_src);
      emit_insn_after (seq, prev);
    }
  else
    {
      basic_block new_bb;
      edge e;
      edge_iterator ei;

      emit_insn_before (seq, BB_HEAD (loop->head));
      seq = emit_label_before (gen_label_rtx (), seq);

      new_bb = create_basic_block (seq, seq_end, loop->head->prev_bb);
      FOR_EACH_EDGE (e, ei, loop->incoming)
	{
	  if (!(e->flags & EDGE_FALLTHRU)
	      || e->dest != loop->head)
	    redirect_edge_and_branch_force (e, new_bb);
	  else
	    redirect_edge_succ (e, new_bb);
	}
      e = make_edge (new_bb, loop->head, 0);
    }

  delete_insn (loop->loop_end);
  /* Insert the loop end label before the last instruction of the loop.  */
  emit_label_before (loop->end_label, loop->last_insn);

  /* The last_insn don't do ifcall.  */
  if (TARGET_IFC)
    {
      emit_insn_before (gen_no_ifc_begin (), loop->last_insn);
      emit_insn_after (gen_no_ifc_end (), loop->last_insn);
    }

  /* The last_insn don't do ex9.  */
  if (TARGET_EX9)
    {
      emit_insn_before (gen_no_ex9_begin (), loop->last_insn);
      emit_insn_after (gen_no_ex9_end (), loop->last_insn);
    }

  /* Record last instruction for identify in relax pass.  */
  emit_insn_after (gen_hwloop_last_insn (), loop->last_insn);

  if (loop->loop_no != loop->outermost->loop_no
      && !same_depth_p)
    {
      start_sequence ();
      /* Insert start place for LB.  */
      emit_insn (gen_mtlbi (gen_rtx_LABEL_REF (Pmode, start_label)));
      /* Insert end place for LE.  */
      emit_insn (gen_mtlei (gen_rtx_LABEL_REF (Pmode, end_label)));

      emit_insn (gen_rtx_USE (SImode, lb_reg));
      seq_end = emit_insn (gen_rtx_USE (SImode, le_reg));

      seq = get_insns ();
      end_sequence ();

      if (loop->outermost->incoming_src)
	{
	  rtx prev = BB_END (loop->outermost->incoming_src);
	  if (vec_safe_length (loop->outermost->incoming) > 1
	      || !(loop->outermost->incoming->last ()->flags & EDGE_FALLTHRU))
	    {
	      gcc_assert (JUMP_P (prev));
	      prev = PREV_INSN (prev);
	    }
	  emit_insn_after (seq, prev);
	}
      else
	{
	  basic_block outer_new_bb;
	  edge outer_e;
	  edge_iterator outer_ei;

	  emit_insn_before (seq, BB_HEAD (loop->outermost->head));
	  seq = emit_label_before (gen_label_rtx (), seq);

	  outer_new_bb = create_basic_block (seq, seq_end,
					     loop->outermost->head->prev_bb);
	  FOR_EACH_EDGE (outer_e, outer_ei, loop->outermost->incoming)
	    {
	      if (!(outer_e->flags & EDGE_FALLTHRU)
		  || outer_e->dest != loop->outermost->head)
		redirect_edge_and_branch_force (outer_e, outer_new_bb);
	      else
		redirect_edge_succ (outer_e, outer_new_bb);
	    }
	  outer_e = make_edge (outer_new_bb, loop->outermost->head, 0);
	}
    }
  return true;
}

/* A callback for the hw-doloop pass.  Called when a loop we have discovered
   turns out not to be optimizable; we have to split the doloop_end pattern
   into a subtract and a test.  */
static void
hwloop_fail (hwloop_info loop)
{
   rtx test;
   rtx insn = loop->loop_end;

   emit_insn_before (gen_addsi3 (loop->iter_reg,
				 loop->iter_reg,
				 constm1_rtx),
		     loop->loop_end);

   test = gen_rtx_NE (VOIDmode, loop->iter_reg, const0_rtx);
   insn = emit_jump_insn_before (gen_cbranchsi4 (test,
						 loop->iter_reg, const0_rtx,
						 loop->start_label),
				 loop->loop_end);

   JUMP_LABEL (insn) = loop->start_label;
   LABEL_NUSES (loop->start_label)++;
   delete_insn (loop->loop_end);
}

static struct hw_doloop_hooks nds32_doloop1_hooks =
{
  hwloop_pattern_reg,
  hwloop1_optimize,
  hwloop_fail
};

static struct hw_doloop_hooks nds32_doloop2_hooks =
{
  hwloop_pattern_reg,
  hwloop2_optimize,
  hwloop_fail
};

/* This pass looks for doloop_end insns and tries to rewrite the RTL
   of these loops so that proper NDS32 hardware loops are generated.  */
static unsigned int
nds32_hwloop1 (void)
{
  compute_bb_for_insn ();
  reorg_loops (false, &nds32_doloop1_hooks);
  return 1;
}

const pass_data pass_data_nds32_hwloop1_opt =
{
  RTL_PASS,				/* type */
  "hwloop1_opt",			/* name */
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

class pass_nds32_hwloop1_opt : public rtl_opt_pass
{
public:
  pass_nds32_hwloop1_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_hwloop1_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return NDS32_HW_LOOP_P(); }
  unsigned int execute () { return nds32_hwloop1 (); }
};

rtl_opt_pass *
make_pass_nds32_hwloop1_opt (gcc::context *ctxt)
{
  return new pass_nds32_hwloop1_opt (ctxt);
}

/* This pass looks for doloop_end insns and tries to rewrite the RTL
   of these loops so that proper NDS32 hardware loops are generated.  */
static unsigned int
nds32_hwloop2 (void)
{
  compute_bb_for_insn ();
  reorg_loops (false, &nds32_doloop2_hooks);
  return 1;
}

const pass_data pass_data_nds32_hwloop2_opt =
{
  RTL_PASS,				/* type */
  "hwloop2_opt",			/* name */
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

class pass_nds32_hwloop2_opt : public rtl_opt_pass
{
public:
  pass_nds32_hwloop2_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_hwloop2_opt, ctxt)
  {}

  /* opt_pass methods: */
  bool gate () { return NDS32_HW_LOOP_P(); }
  unsigned int execute () { return nds32_hwloop2 (); }
};

rtl_opt_pass *
make_pass_nds32_hwloop2_opt (gcc::context *ctxt)
{
  return new pass_nds32_hwloop2_opt (ctxt);
}
