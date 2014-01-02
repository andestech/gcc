/* Auxiliary functions for pipeline descriptions pattern of Andes
   NDS32 cpu for GNU compiler
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

#include <set>
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
#include "optabs.h"		/* For GEN_FCN.  */
#include "target.h"
#include "target-def.h"
#include "langhooks.h"		/* For add_builtin_function().  */
#include "ggc.h"
#include "tree-pass.h"

/* ------------------------------------------------------------------------ */

namespace nds32 {
namespace scheduling {

/* Classify the memory access direction.  It's unknown if the offset register
   is not a constant value.  */
enum memory_access_direction
{
  MEM_ACCESS_DIR_POS,
  MEM_ACCESS_DIR_NEG,
  MEM_ACCESS_DIR_UNKNOWN
};

/* This class provides some wrappers of the DFA scheduler.  Due to the design
   drawback of the DFA scheduler, creating two instances at the same time is
   now allowed.  Use the loosest relationship such as 'dependency' instead of
   'aggregation' or 'composition' can minimize this issue.  */
class pipeline_simulator
{
public:
  pipeline_simulator ();
  ~pipeline_simulator ();

  void advance_cycle (int cycles = 1);
  int query_latency (rtx producer, rtx consumer) const;
  int issue_insn (rtx insn);
  int force_issue_insn (rtx insn);

private:
  static int gcc_dfa_initialized_;
  state_t state_;
};

/* Insert pseudo NOPs so that we can see stall cycles caused by structural or
   data hazards in the assembly code.  The design of this class is similar to
   the 'template method' pattern, but we don't need to maintain multiple
   customized algorithms at the same time.  Hence this class has no virtual
   functions providing further customizations.  */
class stall_inserter
{
private:
  enum dep_type { RES_DEP, DATA_DEP };

public:
  void insert_stalls ();

private:
  static rtx emit_pseudo_nop_before (rtx insn, int cycles, enum dep_type type);

  void insert_structural_hazard_stalls ();
  void insert_data_hazard_stalls ();
  void emit_pseudo_nops_for_data_hazards (rtx insn,
					  pipeline_simulator &simulator);
};

class pass_nds32_print_stalls : public rtl_opt_pass
{
public:
  pass_nds32_print_stalls (gcc::context *ctxt);

  bool gate ();
  unsigned int execute ();
};

int pipeline_simulator::gcc_dfa_initialized_ = 0;

const pass_data pass_data_nds32_print_stalls =
{
  RTL_PASS,				/* type */
  "print_stalls",			/* name */
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

rtl_opt_pass *
make_pass_nds32_print_stalls (gcc::context *ctxt)
{
  return new pass_nds32_print_stalls (ctxt);
}

/* A safe wrapper to the function reg_overlap_mentioned_p ().  */
bool
reg_overlap_p (rtx x, rtx in)
{
  if (x == NULL_RTX || in == NULL_RTX)
    return false;

  return static_cast <bool> (reg_overlap_mentioned_p (x, in));
}

/* Calculate the cycle distance between two insns in pipeline view.
   Hence each insn can be treated as one cycle.
   TODO: multi-cycle insns should be handled
	 specially, but we haven't done it here.  */
int
cycle_distance (rtx from, rtx to)
{
  int count = 1;

  for (from = NEXT_INSN (from); from && from != to; from = NEXT_INSN (from))
    {
      if (!insn_executable_p (from))
	continue;

      if (insn_pseudo_nop_p (from))
	count += INTVAL (XVECEXP (PATTERN (from), 0, 0));
      else
	++count;
    }

  return count;
}

/* Determine the memory access direction of a load/store insn.  */
memory_access_direction
determine_access_direction (rtx insn)
{
  int post_update_rtx_index;
  rtx plus_rtx;
  rtx mem_rtx;
  rtx offset_rtx;

  switch (get_attr_type (insn))
  {
  case TYPE_LOAD_MULTIPLE:
    gcc_assert (parallel_elements (insn) >= 2);

    post_update_rtx_index = find_post_update_rtx (insn);
    if (post_update_rtx_index != -1)
      plus_rtx = SET_SRC (parallel_element (insn, post_update_rtx_index));
    else
      {
	/* (parallel
	     [(set (reg) (mem (reg)))              : index 0
	      (set (reg) (mem (plus (reg) (...)))) : index 1
	      ...])  */
	mem_rtx = SET_SRC (parallel_element (insn, 1));
	if (GET_CODE (mem_rtx) == UNSPEC)
	  mem_rtx = XVECEXP (mem_rtx, 0, 0);
	gcc_assert (MEM_P (mem_rtx));
	plus_rtx = XEXP (mem_rtx, 0);
      }
    break;

  case TYPE_STORE_MULTIPLE:
    gcc_assert (parallel_elements (insn) >= 2);

    post_update_rtx_index = find_post_update_rtx (insn);
    if (post_update_rtx_index != -1)
      plus_rtx = SET_SRC (parallel_element (insn, post_update_rtx_index));
    else
      {
	/* (parallel
	     [(set (mem (reg))              (reg)) : index 0
	      (set (mem (plus (reg) (...))) (reg)) : index 1
	      ...])  */
	mem_rtx = SET_DEST (parallel_element (insn, 1));
	if (GET_CODE (mem_rtx) == UNSPEC)
	  mem_rtx = XVECEXP (mem_rtx, 0, 0);
	gcc_assert (MEM_P (mem_rtx));
	plus_rtx = XEXP (mem_rtx, 0);
      }
    break;

  case TYPE_LOAD:
  case TYPE_STORE:
    mem_rtx = extract_mem_rtx (insn);

    switch (GET_CODE (XEXP (mem_rtx, 0)))
      {
      case POST_INC:
	/* (mem (post_inc (...)))  */
	return MEM_ACCESS_DIR_POS;

      case POST_DEC:
	/* (mem (post_dec (...)))  */
	return MEM_ACCESS_DIR_NEG;

      case PLUS:
	/* (mem (plus (reg) (...)))  */
	plus_rtx = XEXP (mem_rtx, 0);
	break;

      case POST_MODIFY:
	/* (mem (post_modify (reg) (plus (reg) (...))))  */
	plus_rtx = XEXP (XEXP (mem_rtx, 0), 1);
	break;

      default:
	gcc_unreachable ();
      }
    break;

  default:
    gcc_unreachable ();
  }

  gcc_assert (GET_CODE (plus_rtx) == PLUS);

  offset_rtx = XEXP (plus_rtx, 1);
  if (GET_CODE (offset_rtx) == CONST_INT)
    {
      if (INTVAL (offset_rtx) < 0)
	return MEM_ACCESS_DIR_NEG;
      else
	return MEM_ACCESS_DIR_POS;
    }

  return MEM_ACCESS_DIR_UNKNOWN;
}

/* Return the nth load/store operation in the real micro-operation
   accessing order.  */
rtx
extract_nth_access_rtx (rtx insn, int n)
{
  int n_elems = parallel_elements (insn);
  int post_update_rtx_index = find_post_update_rtx (insn);
  memory_access_direction direction = determine_access_direction (insn);

  gcc_assert (direction != MEM_ACCESS_DIR_UNKNOWN);

  /* Reverse the order if the direction negative.  */
  if (direction == MEM_ACCESS_DIR_NEG)
    n = -1 * n - 1;

  if (post_update_rtx_index != -1)
    {
      if (n >= 0 && post_update_rtx_index <= n)
	++n;
      else if (n < 0 && post_update_rtx_index >= n + n_elems)
	--n;
    }

  return parallel_element (insn, n);
}

/* Returns the register operated by the nth load/store operation in the real
   micro-operation accessing order.  This function assumes INSN must be a
   multiple-word load/store insn.  */
rtx
extract_nth_lmsw_access_reg (rtx insn, int n)
{
  rtx nth_rtx = extract_nth_access_rtx (insn, n);

  if (nth_rtx == NULL_RTX)
    return NULL_RTX;

  switch (get_attr_type (insn))
    {
    case TYPE_LOAD_MULTIPLE:
      return SET_DEST (nth_rtx);

    case TYPE_STORE_MULTIPLE:
      return SET_SRC (nth_rtx);

    default:
      gcc_unreachable ();
    }
}

/* Returns the register operated by the nth load/store operation in the real
   micro-operation accessing order.  This function assumes INSN must be a
   double-word load/store insn.  */
rtx
extract_nth_ls2_access_reg (rtx insn, int n)
{
  rtx reg;
  enum machine_mode mode;

  if (post_update_insn_p (insn))
    {
      memory_access_direction direction = determine_access_direction (insn);
      gcc_assert (direction != MEM_ACCESS_DIR_UNKNOWN);

      /* Reverse the order if the direction negative.  */
      if (direction == MEM_ACCESS_DIR_NEG)
	n = -1 * n - 1;
    }

  /* Handle the out-of-range case.  */
  if (n < -2 || n > 1)
    return NULL_RTX;

  /* Convert the index to a positive one.  */
  if (n < 0)
    n = 2 + n;

  switch (get_attr_type (insn))
    {
    case TYPE_LOAD:
      reg = SET_DEST (PATTERN (insn));
      break;

    case TYPE_STORE:
      reg = SET_SRC (PATTERN (insn));
      break;

    default:
      gcc_unreachable ();
    }

  gcc_assert (REG_P (reg) || GET_CODE (reg) == SUBREG);

  switch (GET_MODE (reg))
    {
    case DImode:
      mode = SImode;
      break;

    case DFmode:
      mode = SFmode;
      break;

    default:
      gcc_unreachable ();
    }

  if (n == 0)
    return gen_lowpart (mode, reg);
  else
    return gen_highpart (mode, reg);
}

/* Returns the register operated by the nth load/store operation in the real
   micro-operation accessing order.  */
rtx
extract_nth_access_reg (rtx insn, int index)
{
  switch (GET_CODE (PATTERN (insn)))
    {
    case PARALLEL:
      return extract_nth_lmsw_access_reg (insn, index);

    case SET:
      return extract_nth_ls2_access_reg (insn, index);

    default:
      gcc_unreachable ();
    }
}

/* Determine if the latency is occured when the consumer BSP_INSN, BSE_INSN
   uses the value of DEF_REG in its Ra, Rb, or Rc fields.  */
bool
bsp_insn_dep_reg_p (rtx use_insn, rtx def_reg)
{
  rtx unspec_rtx = SET_SRC (parallel_element (use_insn, 0));

  gcc_assert (GET_CODE (unspec_rtx) == UNSPEC);

  rtx operand_ra = XVECEXP (unspec_rtx, 0, 0);
  rtx operand_rb = XVECEXP (unspec_rtx, 0, 1);
  rtx operand_rc = XVECEXP (unspec_rtx, 0, 2);

  if (rtx_equal_p (def_reg, operand_ra)
      || rtx_equal_p (def_reg, operand_rb)
      || rtx_equal_p (def_reg, operand_rc))
    return true;

  return false;
}

/* Determine if the latency is occured when the consumer PBSADA_INSN uses the
   value of DEF_REG in its Ra or Rb fields.  */
bool
pbsada_insn_ra_rb_dep_reg_p (rtx pbsada_insn, rtx def_reg)
{
  rtx unspec_rtx = SET_SRC (PATTERN (pbsada_insn));
  gcc_assert (GET_CODE (unspec_rtx) == UNSPEC);

  rtx pbsada_ra = XVECEXP (unspec_rtx, 0, 0);
  rtx pbsada_rb = XVECEXP (unspec_rtx, 0, 1);

  if (rtx_equal_p (def_reg, pbsada_ra)
      || rtx_equal_p (def_reg, pbsada_rb))
    return true;

  return false;
}

/* Determine if the latency is occured when the consumer PBSADA_INSN uses the
   value of DEF_REG in its Rt field.  */
bool
pbsada_insn_rt_dep_reg_p (rtx pbsada_insn, rtx def_reg)
{
  rtx pbsada_rt = SET_DEST (PATTERN (pbsada_insn));

  if (rtx_equal_p (def_reg, pbsada_rt))
    return true;

  return false;
}

/* Check if INSN is a movd44 insn consuming DEF_REG.  */
bool
movd44_even_dep_p (rtx insn, rtx def_reg)
{
  if (!movd44_insn_p (insn))
    return false;

  rtx use_rtx = SET_SRC (PATTERN (insn));

  if (REG_P (def_reg))
    {
      return rtx_equal_p (def_reg, use_rtx);
    }
  else if (GET_CODE (def_reg) == SUBREG
	   && GET_MODE (def_reg) == SImode
	   && rtx_equal_p (SUBREG_REG (def_reg), use_rtx))
    {
      if (TARGET_BIG_ENDIAN && SUBREG_BYTE (def_reg) == 4)
	return true;

      if (!TARGET_BIG_ENDIAN && SUBREG_BYTE (def_reg) == 0)
	return true;

      return false;
    }

  return false;
}

/* Check if INSN is a wext insn consuming DEF_REG.  */
bool
wext_odd_dep_p (rtx insn, rtx def_reg)
{
  rtx shift_rtx = XEXP (SET_SRC (PATTERN (insn)), 0);
  rtx use_reg = XEXP (shift_rtx, 0);
  rtx pos_rtx = XEXP (shift_rtx, 1);

  if (REG_P (pos_rtx) && reg_overlap_p (def_reg, pos_rtx))
    return true;

  if (GET_MODE (def_reg) == DImode)
    return reg_overlap_p (def_reg, use_reg);

  gcc_assert (REG_P (def_reg) || GET_CODE (def_reg) == SUBREG);
  gcc_assert (REG_P (use_reg));

  if (REG_P (def_reg))
    {
      if (!TARGET_BIG_ENDIAN)
	return REGNO (def_reg) == REGNO (use_reg) + 1;
      else
	return  REGNO (def_reg) == REGNO (use_reg);
    }

  if (GET_CODE (def_reg) == SUBREG)
    {
      if (!reg_overlap_p (def_reg, use_reg))
	return false;

      if (!TARGET_BIG_ENDIAN)
	return SUBREG_BYTE (def_reg) == 4;
      else
	return SUBREG_BYTE (def_reg) == 0;
    }

  return false;
}

/* Check if INSN is a bpick insn consuming DEF_REG.  */
bool
bpick_ra_rb_dep_p (rtx insn, rtx def_reg)
{
  rtx ior_rtx = SET_SRC (PATTERN (insn));
  rtx and1_rtx = XEXP (ior_rtx, 0);
  rtx and2_rtx = XEXP (ior_rtx, 1);
  rtx reg1_0 = XEXP (and1_rtx, 0);
  rtx reg1_1 = XEXP (and1_rtx, 1);
  rtx reg2_0 = XEXP (and2_rtx, 0);
  rtx reg2_1 = XEXP (and2_rtx, 1);

  if (GET_CODE (reg1_0) == NOT)
    {
      if (rtx_equal_p (XEXP (reg1_0, 0), reg2_0))
	return reg_overlap_p (def_reg, reg1_1)
	       || reg_overlap_p (def_reg, reg2_1);

      if (rtx_equal_p (XEXP (reg1_0, 0), reg2_1))
	return reg_overlap_p (def_reg, reg1_1)
	       || reg_overlap_p (def_reg, reg2_0);
    }

  if (GET_CODE (reg1_1) == NOT)
    {
      if (rtx_equal_p (XEXP (reg1_1, 0), reg2_0))
	return reg_overlap_p (def_reg, reg1_0)
	       || reg_overlap_p (def_reg, reg2_1);

      if (rtx_equal_p (XEXP (reg1_1, 0), reg2_1))
	return reg_overlap_p (def_reg, reg1_0)
	       || reg_overlap_p (def_reg, reg2_0);
    }

  if (GET_CODE (reg2_0) == NOT)
    {
      if (rtx_equal_p (XEXP (reg2_0, 0), reg1_0))
	return reg_overlap_p (def_reg, reg2_1)
	       || reg_overlap_p (def_reg, reg1_1);

      if (rtx_equal_p (XEXP (reg2_0, 0), reg1_1))
	return reg_overlap_p (def_reg, reg2_1)
	       || reg_overlap_p (def_reg, reg1_0);
    }

  if (GET_CODE (reg2_1) == NOT)
    {
      if (rtx_equal_p (XEXP (reg2_1, 0), reg1_0))
	return reg_overlap_p (def_reg, reg2_0)
	       || reg_overlap_p (def_reg, reg1_1);

      if (rtx_equal_p (XEXP (reg2_1, 0), reg1_1))
	return reg_overlap_p (def_reg, reg2_0)
	       || reg_overlap_p (def_reg, reg1_0);
    }

  gcc_unreachable ();
}

pipeline_simulator::pipeline_simulator ()
{
  /* The design of dfa_start () operates on static global variables and
     allocates memory space without checking whether the function is called
     twice or not.  We add some guards in order to protect it from abusing.  */
  if (!gcc_dfa_initialized_++)
    dfa_start ();

  state_ = xmalloc (state_size());
  state_reset (state_);
}

pipeline_simulator::~pipeline_simulator ()
{
  /* The design of dfa_finish () operates on a static global variable and
     deallocates memory space without checking whether the function is called
     twice or not.  We add some guards in order to protect it from abusing.  */
  free (state_);

  gcc_assert(gcc_dfa_initialized_ > 0);
  if (!--gcc_dfa_initialized_)
    dfa_finish ();
}

void
pipeline_simulator::advance_cycle (int cycles)
{
  gcc_assert (cycles > 0);

  /* The second argument was 'NULL', but we found the expression is directly
     written in insn-automata.c:
       if (insn == 0)
	 insn_code = DFA__ADVANCE_CYCLE;
     Hence we change it to '0' in order to make it consistent.  */
  while (cycles--)
    state_transition (state_, 0);
}

/* A wrapper of insn_latency () provided by the insn-attr.h in the object tree.
   See that file for more information.  */
int
pipeline_simulator::query_latency (rtx producer, rtx consumer) const
{
  return insn_latency (producer, consumer);
}

/* Return 0 or negative if we can issue INSN at the current cycle.  Otherwise,
   return a postive value indicates how many cycles we have to wait.  The
   interface is consistent with state_transition () provided by insn-attr.h
   in the object directory.  See that file for more information.  */
int
pipeline_simulator::issue_insn (rtx insn)
{
  int stalls;

  /* Skip cycles specified by pseudo NOPs.  */
  if (insn_pseudo_nop_p (insn))
    {
      int nop_stalls = INTVAL (XVECEXP (PATTERN (insn), 0, 0));

      gcc_assert (nop_stalls > 0);
      advance_cycle (nop_stalls);
      stalls = -1;
    }
  else
    {
      stalls = state_transition (state_, insn);

      /* All targets are single-issue, so we advance one cycle once after
	 an insn has been issued successfully.  */
      if (stalls <= 0)
	advance_cycle ();
    }

  return stalls;
}

/* This function is similar to issue_insn (), but it advances cycles until INSN
   can be issued successfully.  If INSN can be issued at the current cycle, the
   return value will be 0 or negaitive.  Otherwise, the function will return
   the cycles it has been skipped.  */
int
pipeline_simulator::force_issue_insn (rtx insn)
{
  int stalls;

  stalls = issue_insn (insn);

  /* Skip cycles until we can issue the insn.  */
  if (stalls > 0)
    {
      advance_cycle (stalls);
      issue_insn (insn);
    }

  return stalls;
}

/* The main flow of the class STALL_INSERTER.  We insert NOPs for structural
   hazards because self-stalled instructions also consume the delay cycles
   caused by data hazards.  */
void
stall_inserter::insert_stalls ()
{
  compute_bb_for_insn_safe ();

  insert_structural_hazard_stalls ();
  insert_data_hazard_stalls ();

  /* We have to call the following two functions again after we inserting
     some insns after it has been invoked.  Otherwise, an assert expression
     in final () will be triggered and cause to an internal compiler error.  */
  init_insn_lengths ();
  shorten_branches (get_insns ());

  free_bb_for_insn ();
}

/* A helper function inserting NOPs.  CYCLES indicates how many cycles the NOP
   insn consumes.  TYPE indicates what type of the NOP insn we want to insert;
   now there are two types available: RES_DEP and DATA_DEP.  */
rtx
stall_inserter::emit_pseudo_nop_before (
    rtx insn, int cycles, enum dep_type type)
{
  rtx nop_insn;
  int recog;

  switch (type)
  {
  case RES_DEP:
    nop_insn = gen_nop_res_dep (GEN_INT (cycles));
    break;
  case DATA_DEP:
    nop_insn = gen_nop_data_dep (GEN_INT (cycles));
    break;
  default:
    gcc_unreachable ();
  }

  nop_insn = emit_insn_before (nop_insn, insn);
  recog = recog_memoized (nop_insn);
  gcc_assert(recog != -1);

  return nop_insn;
}

void
stall_inserter::insert_structural_hazard_stalls ()
{
  pipeline_simulator simulator;
  rtx insn;

  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (!insn_executable_p (insn)) continue;

      int stalls = simulator.force_issue_insn (insn);

      if (stalls > 0)
	emit_pseudo_nop_before (insn, stalls, RES_DEP);
    }
}

void
stall_inserter::insert_data_hazard_stalls ()
{
  pipeline_simulator simulator;
  rtx insn;

  /* Calling to df_insn_rescan_all here is required in order to avoid crash
     when some special options are specified by users, such as
     -O0 -fschedule-insns2.  */
  df_chain_add_problem (DF_DU_CHAIN);
  df_insn_rescan_all ();
  df_analyze ();

  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (!insn_executable_p (insn)) continue;

      simulator.force_issue_insn (insn);
      emit_pseudo_nops_for_data_hazards (insn, simulator);
    }

  /* We must call df_finish_pass manually because it should be invoked before
     BB information is destroyed.  Hence we cannot set the TODO_df_finish flag
     to the pass manager.  */
  df_insn_rescan_all ();
  df_finish_pass (false);
}

/* Traverse all insns using the results produced by INSN and ask SIMULATOR
   how many delay cycles between them.  If there are some delay cycles, insert
   corresponding NOP insns there.  */
void
stall_inserter::emit_pseudo_nops_for_data_hazards (
    rtx insn, pipeline_simulator &simulator)
{
  df_ref *def_record;
  df_link *link;
  std::set<rtx> processed_insns;

  for (def_record = DF_INSN_DEFS (insn); *def_record; ++def_record)
    {
      for (link = DF_REF_CHAIN (*def_record); link; link = link->next)
	{
	  if (!DF_REF_INSN_INFO (link->ref))
	    continue;

	  rtx use_insn = DF_REF_INSN (link->ref);

	  if (!insn_executable_p (use_insn)
	      || processed_insns.count (use_insn))
	    continue;

	  int stalls = simulator.query_latency (insn, use_insn);
	  int distance = cycle_distance (insn, use_insn);

	  if (stalls > distance)
	    {
	      stalls -= distance;
	      emit_pseudo_nop_before (use_insn, stalls, DATA_DEP);
	      processed_insns.insert (use_insn);
	    }
	}
    }
}

pass_nds32_print_stalls::pass_nds32_print_stalls (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_print_stalls, ctxt)
{
}

bool pass_nds32_print_stalls::gate ()
{
  return TARGET_PRINT_STALLS;
}

unsigned int
pass_nds32_print_stalls::execute ()
{
  stall_inserter inserter;

  inserter.insert_stalls ();
  return 0;
}

} // namespace scheduling
} // namespace nds32

/* ------------------------------------------------------------------------ */

using namespace nds32;
using namespace nds32::scheduling;

namespace { // anonymous namespace

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at II.  */
bool
n7_consumed_by_ii_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    /* MOVD44_E */
    case TYPE_ALU:
      if (movd44_even_dep_p (consumer, def_reg))
	return true;

      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MUL:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  It requires two micro-
      operations in order to write two registers. We have to check the
      dependency from the producer to the first micro-operation.  */
    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_LOAD:
      /* ADDR_IN_bi_Ra, ADDR_IN_!bi */
      if (post_update_insn_p (consumer))
	use_rtx = extract_base_reg (consumer);
      else
	use_rtx = extract_mem_rtx (consumer);
      break;

    case TYPE_STORE:
      /* ADDR_IN_bi_Ra, ADDR_IN_!bi */
      if (post_update_insn_p (consumer))
	use_rtx = extract_base_reg (consumer);
      else
	use_rtx = extract_mem_rtx (consumer);

      if (reg_overlap_p (def_reg, use_rtx))
	return true;

      /* ST_bi, ST_!bi_RI */
      if (!post_update_insn_p (consumer)
	  && !immed_offset_p (extract_mem_rtx (consumer)))
	return false;

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_LOAD_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_STORE_MULTIPLE:
      /* ADDR_IN */
      use_rtx = extract_base_reg (consumer);
      if (reg_overlap_p (def_reg, use_rtx))
	return true;

      /* SMW (N, 1) */
      use_rtx = extract_nth_access_rtx (consumer, 0);
      break;

    case TYPE_BRANCH:
      use_rtx = PATTERN (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  if (reg_overlap_p (def_reg, use_rtx))
    return true;

  return false;
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at AG (II).  */
bool
n8_consumed_by_addr_in_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_BRANCH:
      use_rtx = extract_branch_target_rtx (consumer);
      break;

    case TYPE_LOAD:
      if (load_single_p (consumer))
	use_rtx = extract_mem_rtx (consumer);
      else
	use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_STORE:
      if (store_single_p (consumer)
	  && (!post_update_insn_p (consumer)
	      || immed_offset_p (extract_mem_rtx (consumer))))
	use_rtx = extract_mem_rtx (consumer);
      else
	use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  return reg_overlap_p (def_reg, use_rtx);
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at EX.  */
bool
n8_consumed_by_ex_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
      if (movd44_even_dep_p (consumer, def_reg))
	return true;

      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MUL:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  It requires two micro-
      operations in order to write two registers. We have to check the
      dependency from the producer to the first micro-operation.  */
    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_BRANCH:
      use_rtx = extract_branch_condition_rtx (consumer);
      break;

    case TYPE_STORE:
      /* exclude ST_!bi_RR */
      if (!post_update_insn_p (consumer)
	  && !immed_offset_p (extract_mem_rtx (consumer)))
	return false;

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_nth_access_rtx (consumer, 0);
      break;

    default:
      gcc_unreachable ();
    }

  return reg_overlap_p (def_reg, use_rtx);
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at AG (II).  */
bool
e8_consumed_by_addr_in_p (rtx consumer, rtx def_reg)
{
  return n8_consumed_by_addr_in_p (consumer, def_reg);
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at EX.  */
bool
e8_consumed_by_ex_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
    case TYPE_STORE:
      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MUL:
    case TYPE_MAC:
    case TYPE_DIV:
    case TYPE_BRANCH:
    case TYPE_STORE_MULTIPLE:
      return n8_consumed_by_ex_p (consumer, def_reg);

    default:
      gcc_unreachable ();
    }

  return reg_overlap_p (def_reg, use_rtx);
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at EX.  */
bool
n9_2r1w_consumed_by_ex_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
      if (movd44_even_dep_p (consumer, def_reg))
	return true;

      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_PBSAD:
    case TYPE_MUL:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_ALU_SHIFT:
      use_rtx = extract_shift_reg (consumer);
      break;

    case TYPE_PBSADA:
      return pbsada_insn_ra_rb_dep_reg_p (consumer, def_reg);

    case TYPE_MAC:
      use_rtx = PATTERN (consumer);
      break;

    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MMU:
      if (GET_CODE (PATTERN (consumer)) == SET)
	use_rtx = SET_SRC (PATTERN (consumer));
      else
	return true;
      break;

    case TYPE_LOAD:
      /* ADDR_IN_bi_Ra, ADDR_IN_!bi */
      if (post_update_insn_p (consumer))
	use_rtx = extract_base_reg (consumer);
      else
	use_rtx = extract_mem_rtx (consumer);
      break;

    case TYPE_STORE:
      /* ADDR_IN_bi_Ra, ADDR_IN_!bi */
      if (post_update_insn_p (consumer))
	use_rtx = extract_base_reg (consumer);
      else
	use_rtx = extract_mem_rtx (consumer);

      if (reg_overlap_p (def_reg, use_rtx))
	return true;

      /* exclude ST_!bi_RR */
      if (!post_update_insn_p (consumer)
	  && !immed_offset_p (extract_mem_rtx (consumer)))
	return false;

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_LOAD_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_STORE_MULTIPLE:
      /* ADDR_IN */
      use_rtx = extract_base_reg (consumer);
      if (reg_overlap_p (def_reg, use_rtx))
	return true;

      /* SMW (N, 1) */
      use_rtx = extract_nth_access_rtx (consumer, 0);
      break;

    case TYPE_BRANCH:
      use_rtx = PATTERN (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  if (reg_overlap_p (def_reg, use_rtx))
    return true;

  return false;
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at EX.  */
bool
n9_3r2w_consumed_by_ex_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
    case TYPE_PBSAD:
    case TYPE_MUL:
      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_ALU_SHIFT:
      use_rtx = extract_shift_reg (consumer);
      break;

    case TYPE_PBSADA:
      return pbsada_insn_ra_rb_dep_reg_p (consumer, def_reg);

    case TYPE_MAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  In 2R1W configuration,
      it requires two micro-operations in order to write two registers.
      We have to check the dependency from the producer to the first
      micro-operation.  */
    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MMU:
      if (GET_CODE (PATTERN (consumer)) == SET)
	use_rtx = SET_SRC (PATTERN (consumer));
      else
	return true;
      break;

    case TYPE_LOAD:
    case TYPE_STORE:
      use_rtx = extract_mem_rtx (consumer);
      break;

    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_BRANCH:
      use_rtx = PATTERN (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  if (reg_overlap_p (def_reg, use_rtx))
    return true;

  return false;
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at EX.  */
bool
n10_consumed_by_ex_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
    case TYPE_PBSAD:
    case TYPE_MUL:
    case TYPE_DALU:
    case TYPE_DALU64:
    case TYPE_DMUL:
    case TYPE_DPACK:
    case TYPE_DINSB:
    case TYPE_DCMP:
    case TYPE_DCLIP:
    case TYPE_DALUROUND:
      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_ALU_SHIFT:
      use_rtx = extract_shift_reg (consumer);
      break;

    case TYPE_PBSADA:
      return pbsada_insn_ra_rb_dep_reg_p (consumer, def_reg);

    case TYPE_MAC:
    case TYPE_DMAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  */
    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_DWEXT:
      return wext_odd_dep_p (consumer, def_reg);

    case TYPE_DBPICK:
      return bpick_ra_rb_dep_p (consumer, def_reg);

    case TYPE_MMU:
      if (GET_CODE (PATTERN (consumer)) == SET)
	use_rtx = SET_SRC (PATTERN (consumer));
      else
	return true;
      break;

    case TYPE_LOAD:
    case TYPE_STORE:
      use_rtx = extract_mem_rtx (consumer);
      break;

    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_BRANCH:
      use_rtx = PATTERN (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  if (reg_overlap_p (def_reg, use_rtx))
    return true;

  return false;
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at EX.  */
bool
gw_consumed_by_ex_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
    case TYPE_PBSAD:
    case TYPE_MUL:
    case TYPE_DALU:
    case TYPE_DALU64:
    case TYPE_DMUL:
    case TYPE_DPACK:
    case TYPE_DINSB:
    case TYPE_DCMP:
    case TYPE_DCLIP:
    case TYPE_DALUROUND:
      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_ALU_SHIFT:
      use_rtx = extract_shift_reg (consumer);
      break;

    case TYPE_PBSADA:
      return pbsada_insn_ra_rb_dep_reg_p (consumer, def_reg);

    case TYPE_MAC:
    case TYPE_DMAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  We have to check the
      dependency from the producer to the first micro-operation.  */
    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_DWEXT:
      return wext_odd_dep_p (consumer, def_reg);

    case TYPE_DBPICK:
      return bpick_ra_rb_dep_p (consumer, def_reg);

    case TYPE_MMU:
      if (GET_CODE (PATTERN (consumer)) == SET)
	use_rtx = SET_SRC (PATTERN (consumer));
      else
	return true;
      break;

    case TYPE_LOAD:
    case TYPE_STORE:
      use_rtx = extract_mem_rtx (consumer);
      break;

    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_BRANCH:
      use_rtx = PATTERN (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  if (reg_overlap_p (def_reg, use_rtx))
    return true;

  return false;
}

/* Check dependencies from any stages to ALU_E1 (E1).  This is a helper
   function of n13_consumed_by_e1_dep_p ().  */
bool
n13_alu_e1_insn_dep_reg_p (rtx alu_e1_insn, rtx def_reg)
{
  rtx unspec_rtx, operand_ra, operand_rb;
  rtx src_rtx, dst_rtx;

  switch (INSN_CODE (alu_e1_insn))
    {
    /* BSP and BSE are supported by built-in functions, the corresponding
       patterns are formed by UNSPEC RTXs.  We have to handle them
       individually.  */
    case CODE_FOR_unspec_bsp:
    case CODE_FOR_unspec_bse:
      unspec_rtx = SET_SRC (parallel_element (alu_e1_insn, 0));
      gcc_assert (GET_CODE (unspec_rtx) == UNSPEC);

      operand_ra = XVECEXP (unspec_rtx, 0, 0);
      operand_rb = XVECEXP (unspec_rtx, 0, 1);

      if (rtx_equal_p (def_reg, operand_ra)
	  || rtx_equal_p (def_reg, operand_rb))
	return true;

      return false;

    /* Unlink general ALU instructions, MOVD44 requires operands at E1.  */
    case CODE_FOR_move_di:
    case CODE_FOR_move_df:
      src_rtx = SET_SRC (PATTERN (alu_e1_insn));
      dst_rtx = SET_DEST (PATTERN (alu_e1_insn));

      if (REG_P (dst_rtx) && REG_P (src_rtx)
	  && rtx_equal_p (src_rtx, def_reg))
	return true;

      return false;

    default:
      return false;
    }
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at E1.  Because the address generation unti is
   at E1, the address input should be ready at E1.  Note that the branch
   target is also a kind of addresses, so we have to check it.  */
bool
n13_consumed_by_e1_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    /* ALU_E1 */
    case TYPE_ALU:
      return n13_alu_e1_insn_dep_reg_p (consumer, def_reg);

    case TYPE_PBSADA:
      return pbsada_insn_ra_rb_dep_reg_p (consumer, def_reg);

    case TYPE_PBSAD:
    case TYPE_MUL:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;

    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MMU:
      if (GET_CODE (PATTERN (consumer)) == SET)
	use_rtx = SET_SRC (PATTERN (consumer));
      else
	return true;
      break;

    case TYPE_BRANCH:
      use_rtx = extract_branch_target_rtx (consumer);
      break;

    case TYPE_LOAD:
    case TYPE_STORE:
      use_rtx = extract_mem_rtx (consumer);
      break;

    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    default:
      return false;
    }

  if (reg_overlap_p (def_reg, use_rtx))
    return true;

  return false;
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at E2.  */
bool
n13_consumed_by_e2_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
    case TYPE_STORE:
      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_ALU_SHIFT:
      use_rtx = extract_shift_reg (consumer);
      break;

    case TYPE_PBSADA:
      return pbsada_insn_rt_dep_reg_p (consumer, def_reg);

    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_nth_access_rtx (consumer, 0);
      break;

    case TYPE_BRANCH:
      use_rtx = extract_branch_condition_rtx (consumer);
      break;

    default:
      gcc_unreachable();
    }

  if (reg_overlap_p (def_reg, use_rtx))
    return true;

  return false;
}

/* Check the dependency between the producer defining DEF_REG and CONSUMER
   requiring input operand at AG (E1).  */
bool
pn_consumed_by_e1_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_LOAD:
      if (load_single_p (consumer))
	use_rtx = extract_mem_rtx (consumer);
      else
	use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_STORE:
      if (store_single_p (consumer)
	  && (!post_update_insn_p (consumer)
	      || immed_offset_p (extract_mem_rtx (consumer))))
	use_rtx = extract_mem_rtx (consumer);
      else
	use_rtx = extract_base_reg (consumer);
      break;

    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  return reg_overlap_p (def_reg, use_rtx);
}

bool
pn_consumed_by_e2_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
      if (get_attr_subtype (consumer) != SUBTYPE_SHIFT)
	return false;
    case TYPE_PBSAD:
    case TYPE_PBSADA:
    case TYPE_MUL:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_MAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;

    default:
      gcc_unreachable ();
    }

  return reg_overlap_p (def_reg, use_rtx);
}

bool
pn_consumed_by_e3_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
      if (get_attr_subtype (consumer) == SUBTYPE_SHIFT)
	return false;
      if (INSN_CODE (consumer) == CODE_FOR_unspec_bse
	  || INSN_CODE (consumer) == CODE_FOR_unspec_bsp)
	return bsp_insn_dep_reg_p (consumer, def_reg);

    case TYPE_PBSAD:
    case TYPE_PBSADA:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_BRANCH:
      return (reg_overlap_p (def_reg, extract_branch_target_rtx (consumer))
	      || reg_overlap_p (def_reg,
				extract_branch_condition_rtx (consumer)));
      break;

    case TYPE_STORE:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;

    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_nth_access_rtx (consumer, 0);
      break;

    default:
      gcc_unreachable ();
    }

  return reg_overlap_p (def_reg, use_rtx);
}

bool
pn_consumed_by_e4_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;

  switch (get_attr_type (consumer))
    {
    case TYPE_MAC:
      use_rtx = SET_DEST (PATTERN (consumer));
      break;

    case TYPE_DIV:
      if (divmod_p (consumer))
	use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
	use_rtx = SET_SRC (PATTERN (consumer));
      break;

    default:
      gcc_unreachable ();
    }

  return reg_overlap_p (def_reg, use_rtx);
}

} // anonymous namespace

/* ------------------------------------------------------------------------ */

/* Guard functions for N7 core.  */

bool
nds32_n7_load_to_ii_p (rtx producer, rtx consumer)
{
  if (post_update_insn_p (producer))
    return false;

  rtx def_reg = SET_DEST (PATTERN (producer));

  return n7_consumed_by_ii_dep_p (consumer, def_reg);
}

bool
nds32_n7_last_load_to_ii_p (rtx producer, rtx consumer)
{
  /* If PRODUCER is a post-update LMW insn, the last micro-operation updates
     the base register and the result is ready in II stage, so we don't need
     to handle that case in this guard function and the corresponding bypass
     rule.  */
  if (post_update_insn_p (producer))
    return false;

  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  if (last_def_reg == NULL_RTX)
    return false;

  gcc_assert (REG_P (last_def_reg) || GET_CODE (last_def_reg) == SUBREG);

  return n7_consumed_by_ii_dep_p (consumer, last_def_reg);
}

/* Guard functions for N8 core.  */

bool
nds32_n8_load_to_ii_p (rtx producer, rtx consumer)
{
  if (post_update_insn_p (producer))
    return false;

  rtx def_reg = SET_DEST (PATTERN (producer));

  return n8_consumed_by_addr_in_p (consumer, def_reg);
}

bool
nds32_n8_load_bi_to_ii_p (rtx producer, rtx consumer)
{
  if (!post_update_insn_p (producer))
    return false;

  rtx def_reg = SET_DEST (PATTERN (producer));

  return n8_consumed_by_addr_in_p (consumer, def_reg);
}

bool
nds32_n8_load_to_ex_p (rtx producer, rtx consumer)
{
  if (post_update_insn_p (producer))
    return false;

  rtx def_reg = SET_DEST (PATTERN (producer));

  return n8_consumed_by_ex_p (consumer, def_reg);
}

bool
nds32_n8_ex_to_ii_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_ALU:
      {
	if (INSN_CODE (producer) == CODE_FOR_unspec_bse
	    || INSN_CODE (producer) == CODE_FOR_unspec_bsp)
	  {
	    rtx bsp_def_reg = SET_DEST (parallel_element (producer, 0));
	    rtx bsp_def_reg2 = SET_DEST (parallel_element (producer, 1));

	    return n8_consumed_by_addr_in_p (consumer, bsp_def_reg)
		   && n8_consumed_by_addr_in_p (consumer, bsp_def_reg2);
	  }

	if (movd44_insn_p (producer))
	  def_reg = extract_movd44_odd_reg (producer);
	else
	  def_reg = SET_DEST (PATTERN (producer));
	break;
      }
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_DIV:
      if (divmod_p (producer))
	def_reg = SET_DEST (parallel_element (producer, 1));
      else
	def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_LOAD:
    case TYPE_STORE:
    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      if (!post_update_insn_p (producer))
	return false;

      def_reg = extract_base_reg (producer);
      break;

    default:
      gcc_unreachable ();
    }

  return n8_consumed_by_addr_in_p (consumer, def_reg);
}

bool
nds32_n8_last_load_to_ii_p (rtx producer, rtx consumer)
{
  /* If PRODUCER is a post-update LMW insn, the last micro-operation updates
     the base register and the result is ready in EX stage, so we don't need
     to handle that case in this guard function and the corresponding bypass
     rule.  */
  if (post_update_insn_p (producer))
    return false;

  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  if (last_def_reg == NULL_RTX)
    return false;

  gcc_assert (REG_P (last_def_reg) || GET_CODE (last_def_reg) == SUBREG);

  return n8_consumed_by_addr_in_p (consumer, last_def_reg);
}

bool
nds32_n8_last_load_two_to_ii_p (rtx producer, rtx consumer)
{
  int index = -2;

  /* If PRODUCER is a post-update insn, there is an additional one micro-
     operation inserted in the end, so the last memory access operation should
     be handled by this guard function and the corresponding bypass rule.  */
  if (post_update_insn_p (producer))
    index = -1;

  rtx last_two_def_reg = extract_nth_access_reg (producer, index);

  if (last_two_def_reg == NULL_RTX)
    return false;

  gcc_assert (REG_P (last_two_def_reg)
	      || GET_CODE (last_two_def_reg) == SUBREG);

  return n8_consumed_by_addr_in_p (consumer, last_two_def_reg);
}

bool
nds32_n8_last_load_to_ex_p (rtx producer, rtx consumer)
{
  /* If PRODUCER is a post-update LMW insn, the last micro-operation updates
     the base register and the result is ready in EX stage, so we don't need
     to handle that case in this guard function and the corresponding bypass
     rule.  */
  if (post_update_insn_p (producer))
    return false;

  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  if (last_def_reg == NULL_RTX)
    return false;

  gcc_assert (REG_P (last_def_reg) || GET_CODE (last_def_reg) == SUBREG);

  return n8_consumed_by_ex_p (consumer, last_def_reg);
}

/* Guard functions for E8 cores.  */

bool
nds32_e8_load_to_ii_p (rtx producer, rtx consumer)
{
  rtx def_reg = SET_DEST (PATTERN (producer));

  return e8_consumed_by_addr_in_p (consumer, def_reg);
}

bool
nds32_e8_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx def_reg = SET_DEST (PATTERN (producer));

  return e8_consumed_by_ex_p (consumer, def_reg);
}

bool
nds32_e8_ex_to_ii_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_ALU:
      /* No data hazards if AGEN's input is produced by MOVI or SETHI.  */
      if (GET_CODE (PATTERN (producer)) == SET)
	{
	  rtx dest = SET_DEST (PATTERN (producer));
	  rtx src = SET_SRC (PATTERN (producer));

	  if ((REG_P (dest) || GET_CODE (dest) == SUBREG)
	      && (GET_CODE (src) == CONST_INT || GET_CODE (src) == HIGH))
	    return false;
	}

      if (INSN_CODE (producer) == CODE_FOR_unspec_bse
	  || INSN_CODE (producer) == CODE_FOR_unspec_bsp)
	{
	  rtx bsp_def_reg = SET_DEST (parallel_element (producer, 0));
	  rtx bsp_def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return n8_consumed_by_addr_in_p (consumer, bsp_def_reg)
		 && n8_consumed_by_addr_in_p (consumer, bsp_def_reg2);
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_DIV:
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (e8_consumed_by_addr_in_p (consumer, def_reg1)
		  || e8_consumed_by_addr_in_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_LOAD:
    case TYPE_STORE:
    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      if (!post_update_insn_p (producer))
	return false;

      def_reg = extract_base_reg (producer);
      break;

    default:
      gcc_unreachable ();
    }

  return e8_consumed_by_addr_in_p (consumer, def_reg);
}

bool
nds32_e8_last_load_to_ii_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  if (last_def_reg == NULL_RTX)
    return false;

  gcc_assert (REG_P (last_def_reg) || GET_CODE (last_def_reg) == SUBREG);

  return e8_consumed_by_addr_in_p (consumer, last_def_reg);
}

bool
nds32_e8_last_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  if (last_def_reg == NULL_RTX)
    return false;

  gcc_assert (REG_P (last_def_reg) || GET_CODE (last_def_reg) == SUBREG);

  return e8_consumed_by_ex_p (consumer, last_def_reg);
}

/* Guard functions for N9 cores.  */

/* Check dependencies from MM to EX.  */
bool
nds32_n9_2r1w_mm_to_ex_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    /* LD_!bi */
    case TYPE_LOAD:
      if (post_update_insn_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

    return n9_2r1w_consumed_by_ex_dep_p (consumer, def_reg);
}

/* Check dependencies from MM to EX.  */
bool
nds32_n9_3r2w_mm_to_ex_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  We have to handle them
      individually.  */
    case TYPE_DIV:
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (n9_3r2w_consumed_by_ex_dep_p (consumer, def_reg1)
		  || n9_3r2w_consumed_by_ex_dep_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

    return n9_3r2w_consumed_by_ex_dep_p (consumer, def_reg);
}

/* Check dependencies from LMW(N, N) to EX.  */
bool
nds32_n9_last_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  if (nds32_register_ports_config == REG_PORT_2R1W)
    {
      /* The base-update micro operation occupies the last cycle.  */
      if (post_update_insn_p (producer))
	return false;

      /* When the base register is in the list of a load multiple insn and the
	 access order of the base register is not the last one, we need an
	 additional micro operation to commit the load result to the base
	 register -- we can treat the base register as the last defined
	 register.  */
      size_t i;
      size_t n_elems = parallel_elements (producer);
      rtx base_reg = extract_base_reg (producer);

      for (i = 0; i < n_elems; ++i)
	{
	  rtx load_rtx = extract_nth_access_rtx (producer, i);
	  rtx list_element = SET_DEST (load_rtx);

	  if (rtx_equal_p (base_reg, list_element) && i != n_elems - 1)
	    {
	      last_def_reg = base_reg;
	      break;
	    }
	}

      return n9_2r1w_consumed_by_ex_dep_p (consumer, last_def_reg);
    }
  else
    return n9_3r2w_consumed_by_ex_dep_p (consumer, last_def_reg);
}

/* Guard functions for N10 cores.  */

/* Check dependencies from EX to EX (ADDR_OUT -> ADDR_IN).  */
bool
nds32_n10_ex_to_ex_p (rtx producer, rtx consumer)
{
  gcc_assert (get_attr_type (producer) == TYPE_FLOAD
	      || get_attr_type (producer) == TYPE_FSTORE);
  gcc_assert (get_attr_type (consumer) == TYPE_FLOAD
	      || get_attr_type (consumer) == TYPE_FSTORE);

  if (!post_update_insn_p (producer))
    return false;

  return reg_overlap_p (extract_base_reg (producer),
			extract_mem_rtx (consumer));
}

/* Check dependencies from MM to EX.  */
bool
nds32_n10_mm_to_ex_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
    case TYPE_MUL:
    case TYPE_MAC:
    case TYPE_DALU64:
    case TYPE_DMUL:
    case TYPE_DMAC:
    case TYPE_DALUROUND:
    case TYPE_DBPICK:
    case TYPE_DWEXT:
      def_reg = SET_DEST (PATTERN (producer));
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  We have to handle them
      individually.  */
    case TYPE_DIV:
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (n10_consumed_by_ex_dep_p (consumer, def_reg1)
		  || n10_consumed_by_ex_dep_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

    return n10_consumed_by_ex_dep_p (consumer, def_reg);
}

/* Check dependencies from LMW(N, N) to EX.  */
bool
nds32_n10_last_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  return n10_consumed_by_ex_dep_p (consumer, last_def_reg);
}

/* Guard functions for Graywolf cores.  */

/* Check dependencies from EX to EX (ADDR_OUT -> ADDR_IN).  */
bool
nds32_gw_ex_to_ex_p (rtx producer, rtx consumer)
{
  return nds32_n10_ex_to_ex_p (producer, consumer);
}

/* Check dependencies from MM to EX.  */
bool
nds32_gw_mm_to_ex_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
    case TYPE_MUL:
    case TYPE_MAC:
    case TYPE_DALU64:
    case TYPE_DMUL:
    case TYPE_DMAC:
    case TYPE_DALUROUND:
    case TYPE_DBPICK:
    case TYPE_DWEXT:
      def_reg = SET_DEST (PATTERN (producer));
      break;

   /* Some special instructions, divmodsi4 and udivmodsi4, produce two
      results, the quotient and the remainder.  We have to handle them
      individually.  */
    case TYPE_DIV:
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (gw_consumed_by_ex_dep_p (consumer, def_reg1)
		  || gw_consumed_by_ex_dep_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

    return gw_consumed_by_ex_dep_p (consumer, def_reg);
}

/* Check dependencies from LMW(N, N) to EX.  */
bool
nds32_gw_last_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  return gw_consumed_by_ex_dep_p (consumer, last_def_reg);
}

/* Guard functions for N12/N13 cores.  */

/* Check dependencies from E2 to E1.  */
bool
nds32_n13_e2_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    /* Only post-update load/store instructions are considered.  These
       instructions produces address output at E2.  */
    case TYPE_LOAD:
    case TYPE_STORE:
    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      if (!post_update_insn_p (producer))
	return false;

      def_reg = extract_base_reg (producer);
      break;

    case TYPE_ALU:
    case TYPE_ALU_SHIFT:
    case TYPE_PBSAD:
    case TYPE_PBSADA:
    case TYPE_MUL:
    case TYPE_MAC:
      {
	if (INSN_CODE (producer) == CODE_FOR_unspec_bse
	    || INSN_CODE (producer) == CODE_FOR_unspec_bsp)
	  {
	    rtx bsp_def_reg = SET_DEST (parallel_element (producer, 0));
	    rtx bsp_def_reg2 = SET_DEST (parallel_element (producer, 1));

	    return n13_consumed_by_e1_dep_p (consumer, bsp_def_reg)
		   && n13_consumed_by_e1_dep_p (consumer, bsp_def_reg2);
	  }

	def_reg = SET_DEST (PATTERN (producer));
	break;
      }
    case TYPE_BRANCH:
      return true;

    case TYPE_DIV:
      /* Some special instructions, divmodsi4 and udivmodsi4, produce two
	 results, the quotient and the remainder.  We have to handle them
	 individually.  */
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (n13_consumed_by_e1_dep_p (consumer, def_reg1)
		  || n13_consumed_by_e1_dep_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

  return n13_consumed_by_e1_dep_p (consumer, def_reg);
}

/* Check dependencies from Load-Store Unit (E3) to E1.  */
bool
nds32_n13_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg = SET_DEST (PATTERN (producer));

  gcc_assert (get_attr_type (producer) == TYPE_LOAD);
  gcc_assert (REG_P (def_reg) || GET_CODE (def_reg) == SUBREG);

  return n13_consumed_by_e1_dep_p (consumer, def_reg);
}

/* Check dependencies from Load-Store Unit (E3) to E2.  */
bool
nds32_n13_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx def_reg = SET_DEST (PATTERN (producer));

  gcc_assert (get_attr_type (producer) == TYPE_LOAD);
  gcc_assert (REG_P (def_reg) || GET_CODE (def_reg) == SUBREG);

  return n13_consumed_by_e2_dep_p (consumer, def_reg);
}

/* Check dependencies from LMW(N, N) to E1.  */
bool
nds32_n13_last_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  return n13_consumed_by_e1_dep_p (consumer, last_def_reg);
}

/* Check dependencies from LMW(N, N) to E2.  */
bool
nds32_n13_last_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  return n13_consumed_by_e2_dep_p (consumer, last_def_reg);
}

/* Check dependencies from LMW(N, N-1) to E2.  */
bool
nds32_n13_last_two_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_two_def_reg = extract_nth_access_reg (producer, -2);

  if (last_two_def_reg == NULL_RTX)
    return false;

  return n13_consumed_by_e1_dep_p (consumer, last_two_def_reg);
}

/* Guard functions for Panther cores.  */

/* Check dependencies from E2 to E1.  */
bool
nds32_pn_e2_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_ALU:
      gcc_assert (get_attr_subtype (producer) == SUBTYPE_SHIFT);
      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e1_dep_p (consumer, def_reg);
}

/* Check dependencies from E3 to E1.  */
bool
nds32_pn_e3_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_ALU:
      {
	if (INSN_CODE (producer) == CODE_FOR_unspec_bse
	    || INSN_CODE (producer) == CODE_FOR_unspec_bsp)
	  {
	    rtx bsp_def_reg = SET_DEST (parallel_element (producer, 0));
	    rtx bsp_def_reg2 = SET_DEST (parallel_element (producer, 1));

	    return pn_consumed_by_e1_dep_p (consumer, bsp_def_reg)
		   && pn_consumed_by_e1_dep_p (consumer, bsp_def_reg2);
	  }

	def_reg = SET_DEST (PATTERN (producer));
	break;
      }
    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e1_dep_p (consumer, def_reg);
}

/* Check dependencies from E3 to E2.  */
bool
nds32_pn_e3_to_e2_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_ALU:
      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e2_dep_p (consumer, def_reg);
}

/* Check dependencies from E4 to E1.  */
bool
nds32_pn_e4_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_DIV:
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (pn_consumed_by_e1_dep_p (consumer, def_reg1)
		  || pn_consumed_by_e1_dep_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_LOAD:
      if (post_update_insn_p (producer)
	  && pn_consumed_by_e1_dep_p (consumer, extract_base_reg (producer)))
	return true;

      if (!load_full_word_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_STORE:
    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      if (!post_update_insn_p (producer))
	return false;

      def_reg = extract_base_reg (producer);
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e1_dep_p (consumer, def_reg);
}

/* Check dependencies from E4 to E2.  */
bool
nds32_pn_e4_to_e2_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_DIV:
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (pn_consumed_by_e2_dep_p (consumer, def_reg1)
		  || pn_consumed_by_e2_dep_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_LOAD:
      if (post_update_insn_p (producer)
	  && pn_consumed_by_e2_dep_p (consumer, extract_base_reg (producer)))
	return true;

      if (!load_full_word_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_STORE:
    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      if (!post_update_insn_p (producer))
	return false;

      def_reg = extract_base_reg (producer);
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e2_dep_p (consumer, def_reg);
}

/* Check dependencies from E4 to E3.  */
bool
nds32_pn_e4_to_e3_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_DIV:
      if (divmod_p (producer))
	{
	  rtx def_reg1 = SET_DEST (parallel_element (producer, 0));
	  rtx def_reg2 = SET_DEST (parallel_element (producer, 1));

	  return (pn_consumed_by_e3_dep_p (consumer, def_reg1)
		  || pn_consumed_by_e3_dep_p (consumer, def_reg2));
	}

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_LOAD:
      if (post_update_insn_p (producer)
	  && pn_consumed_by_e3_dep_p (consumer, extract_base_reg (producer)))
	return true;

      if (load_partial_word_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    case TYPE_STORE:
    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      if (!post_update_insn_p (producer))
	return false;

      def_reg = extract_base_reg (producer);
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e3_dep_p (consumer, def_reg);
}

/* Check dependencies from WB to E1.  */
bool
nds32_pn_wb_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
      if (!load_partial_word_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e1_dep_p (consumer, def_reg);
}

/* Check dependencies from WB to E2.  */
bool
nds32_pn_wb_to_e2_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
      if (!load_partial_word_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e2_dep_p (consumer, def_reg);
}

/* Check dependencies from WB to E3.  */
bool
nds32_pn_wb_to_e3_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
      if (!load_partial_word_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e3_dep_p (consumer, def_reg);
}

/* Check dependencies from WB to E4.  */
bool
nds32_pn_wb_to_e4_p (rtx producer, rtx consumer)
{
  rtx def_reg;

  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
      if (!load_partial_word_p (producer))
	return false;

      def_reg = SET_DEST (PATTERN (producer));
      break;

    default:
      gcc_unreachable ();
    }

  return pn_consumed_by_e4_dep_p (consumer, def_reg);
}

/* Check dependencies from LMW(N, N) to E1.  */
bool
nds32_pn_last_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  return pn_consumed_by_e1_dep_p (consumer, last_def_reg);
}

/* Check dependencies from LMW(N, N) to E2.  */
bool
nds32_pn_last_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  return pn_consumed_by_e2_dep_p (consumer, last_def_reg);
}

/* Check dependencies from LMW(N, N) to E3.  */
bool
nds32_pn_last_load_to_e3_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);

  return pn_consumed_by_e3_dep_p (consumer, last_def_reg);
}

/* Check dependencies from LMW(N, N - 1) to E1.  */
bool
nds32_pn_last_two_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_two_def_reg = extract_nth_access_reg (producer, -2);

  if (last_two_def_reg == NULL_RTX)
    return false;

  return pn_consumed_by_e1_dep_p (consumer, last_two_def_reg);
}

/* Check dependencies from LMW(N, N - 1) to E2.  */
bool
nds32_pn_last_two_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx last_two_def_reg = extract_nth_access_reg (producer, -2);

  if (last_two_def_reg == NULL_RTX)
    return false;

  return pn_consumed_by_e2_dep_p (consumer, last_two_def_reg);
}

/* Check dependencies from LMW(N, N - 2) to E1.  */
bool
nds32_pn_last_three_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_three_def_reg = extract_nth_access_reg (producer, -3);

  if (last_three_def_reg == NULL_RTX)
    return false;

  return pn_consumed_by_e1_dep_p (consumer, last_three_def_reg);
}

/* ------------------------------------------------------------------------ */
