#include <set>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "reload.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "df.h"
#include "tm_p.h"
#include "tm-constrs.h"
#include "optabs.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"
#include "ggc.h"
#include "tree-pass.h"
namespace nds32 {
namespace scheduling {
enum memory_access_direction
{
  MEM_ACCESS_DIR_POS,
  MEM_ACCESS_DIR_NEG,
  MEM_ACCESS_DIR_UNKNOWN
};
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
  RTL_PASS,
  "print_stalls",
  OPTGROUP_NONE,
  true,
  true,
  TV_MACH_DEP,
  0,
  0,
  0,
  0,
  TODO_verify_rtl_sharing,
};
rtl_opt_pass *
make_pass_nds32_print_stalls (gcc::context *ctxt)
{
  return new pass_nds32_print_stalls (ctxt);
}
bool
reg_overlap_p (rtx x, rtx in)
{
  if (x == NULL_RTX || in == NULL_RTX)
    return false;
  return static_cast <bool> (reg_overlap_mentioned_p (x, in));
}
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
 return MEM_ACCESS_DIR_POS;
      case POST_DEC:
 return MEM_ACCESS_DIR_NEG;
      case PLUS:
 plus_rtx = XEXP (mem_rtx, 0);
 break;
      case POST_MODIFY:
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
rtx
extract_nth_access_rtx (rtx insn, int n)
{
  int n_elems = parallel_elements (insn);
  int post_update_rtx_index = find_post_update_rtx (insn);
  memory_access_direction direction = determine_access_direction (insn);
  gcc_assert (direction != MEM_ACCESS_DIR_UNKNOWN);
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
rtx
extract_nth_ls2_access_reg (rtx insn, int n)
{
  rtx reg;
  enum machine_mode mode;
  if (post_update_insn_p (insn))
    {
      memory_access_direction direction = determine_access_direction (insn);
      gcc_assert (direction != MEM_ACCESS_DIR_UNKNOWN);
      if (direction == MEM_ACCESS_DIR_NEG)
 n = -1 * n - 1;
    }
  if (n < -2 || n > 1)
    return NULL_RTX;
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
bool
pbsada_insn_rt_dep_reg_p (rtx pbsada_insn, rtx def_reg)
{
  rtx pbsada_rt = SET_DEST (PATTERN (pbsada_insn));
  if (rtx_equal_p (def_reg, pbsada_rt))
    return true;
  return false;
}
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
pipeline_simulator::pipeline_simulator ()
{
  if (!gcc_dfa_initialized_++)
    dfa_start ();
  state_ = xmalloc (state_size());
  state_reset (state_);
}
pipeline_simulator::~pipeline_simulator ()
{
  free (state_);
  gcc_assert(gcc_dfa_initialized_ > 0);
  if (!--gcc_dfa_initialized_)
    dfa_finish ();
}
void
pipeline_simulator::advance_cycle (int cycles)
{
  gcc_assert (cycles > 0);
  while (cycles--)
    state_transition (state_, 0);
}
int
pipeline_simulator::query_latency (rtx producer, rtx consumer) const
{
  return insn_latency (producer, consumer);
}
int
pipeline_simulator::issue_insn (rtx insn)
{
  int stalls;
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
      if (stalls <= 0)
 advance_cycle ();
    }
  return stalls;
}
int
pipeline_simulator::force_issue_insn (rtx insn)
{
  int stalls;
  stalls = issue_insn (insn);
  if (stalls > 0)
    {
      advance_cycle (stalls);
      issue_insn (insn);
    }
  return stalls;
}
void
stall_inserter::insert_stalls ()
{
  compute_bb_for_insn_safe ();
  insert_structural_hazard_stalls ();
  insert_data_hazard_stalls ();
  init_insn_lengths ();
  shorten_branches (get_insns ());
  free_bb_for_insn ();
}
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
  df_chain_add_problem (DF_DU_CHAIN);
  df_insn_rescan_all ();
  df_analyze ();
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (!insn_executable_p (insn)) continue;
      simulator.force_issue_insn (insn);
      emit_pseudo_nops_for_data_hazards (insn, simulator);
    }
  df_insn_rescan_all ();
  df_finish_pass (false);
}
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
}
}
using namespace nds32;
using namespace nds32::scheduling;
namespace {
bool
n7_consumed_by_ii_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
      if (movd44_even_dep_p (consumer, def_reg))
 return true;
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_MUL:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_MAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;
    case TYPE_DIV:
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
 use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
 use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_LOAD:
      if (post_update_insn_p (consumer))
 use_rtx = extract_base_reg (consumer);
      else
 use_rtx = extract_mem_rtx (consumer);
      break;
    case TYPE_STORE:
      if (post_update_insn_p (consumer))
 use_rtx = extract_base_reg (consumer);
      else
 use_rtx = extract_mem_rtx (consumer);
      if (reg_overlap_p (def_reg, use_rtx))
 return true;
      if (!post_update_insn_p (consumer)
   && !immed_offset_p (extract_mem_rtx (consumer)))
 return false;
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_LOAD_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      if (reg_overlap_p (def_reg, use_rtx))
 return true;
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
bool
n8_consumed_by_ex_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
      if (movd44_even_dep_p (consumer, def_reg))
 return true;
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_MUL:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_MAC:
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;
    case TYPE_DIV:
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
 use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
 use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_BRANCH:
      use_rtx = extract_branch_condition_rtx (consumer);
      break;
    case TYPE_STORE:
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
bool
e8_consumed_by_addr_in_p (rtx consumer, rtx def_reg)
{
  return n8_consumed_by_addr_in_p (consumer, def_reg);
}
bool
e8_consumed_by_ex_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
    case TYPE_STORE:
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
bool
n9_2r1w_consumed_by_ex_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
      if (movd44_even_dep_p (consumer, def_reg))
 return true;
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
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
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
      if (post_update_insn_p (consumer))
 use_rtx = extract_base_reg (consumer);
      else
 use_rtx = extract_mem_rtx (consumer);
      break;
    case TYPE_STORE:
      if (post_update_insn_p (consumer))
 use_rtx = extract_base_reg (consumer);
      else
 use_rtx = extract_mem_rtx (consumer);
      if (reg_overlap_p (def_reg, use_rtx))
 return true;
      if (!post_update_insn_p (consumer)
   && !immed_offset_p (extract_mem_rtx (consumer)))
 return false;
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_LOAD_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      break;
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_base_reg (consumer);
      if (reg_overlap_p (def_reg, use_rtx))
 return true;
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
bool
n9_3r2w_consumed_by_ex_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
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
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;
    case TYPE_DIV:
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
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
    case TYPE_DMUL:
    case TYPE_DPACK:
    case TYPE_DINSB:
    case TYPE_DCMP:
    case TYPE_DCLIP:
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
    case TYPE_DIV:
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
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
bool
gw_consumed_by_ex_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
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
      use_rtx = extract_mac_non_acc_rtx (consumer);
      break;
    case TYPE_DIV:
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
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
bool
n13_alu_e1_insn_dep_reg_p (rtx alu_e1_insn, rtx def_reg)
{
  rtx unspec_rtx, operand_ra, operand_rb;
  rtx src_rtx, dst_rtx;
  switch (INSN_CODE (alu_e1_insn))
    {
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
bool
n13_consumed_by_e1_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
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
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
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
bool
n13_consumed_by_e2_dep_p (rtx consumer, rtx def_reg)
{
  rtx use_rtx;
  switch (get_attr_type (consumer))
    {
    case TYPE_ALU:
    case TYPE_STORE:
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
    case TYPE_PBSAD:
    case TYPE_PBSADA:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_BRANCH:
      return (reg_overlap_p (def_reg, extract_branch_target_rtx (consumer))
       || reg_overlap_p (def_reg,
    extract_branch_condition_rtx (consumer)));
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
    case TYPE_STORE:
      use_rtx = SET_SRC (PATTERN (consumer));
      break;
    case TYPE_STORE_MULTIPLE:
      use_rtx = extract_nth_access_rtx (consumer, 0);
      break;
    case TYPE_DIV:
      if (INSN_CODE (consumer) == CODE_FOR_divmodsi4
   || INSN_CODE (consumer) == CODE_FOR_udivmodsi4)
 use_rtx = SET_SRC (parallel_element (consumer, 0));
      else
 use_rtx = SET_SRC (PATTERN (consumer));
      break;
    default:
      gcc_unreachable ();
    }
  return reg_overlap_p (def_reg, use_rtx);
}
}
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
  if (post_update_insn_p (producer))
    return false;
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  if (last_def_reg == NULL_RTX)
    return false;
  gcc_assert (REG_P (last_def_reg) || GET_CODE (last_def_reg) == SUBREG);
  return n7_consumed_by_ii_dep_p (consumer, last_def_reg);
}
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
      if (movd44_insn_p (producer))
 def_reg = extract_movd44_odd_reg (producer);
      else
 def_reg = SET_DEST (PATTERN (producer));
      break;
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;
    case TYPE_DIV:
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
  if (post_update_insn_p (producer))
    return false;
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  if (last_def_reg == NULL_RTX)
    return false;
  gcc_assert (REG_P (last_def_reg) || GET_CODE (last_def_reg) == SUBREG);
  return n8_consumed_by_ex_p (consumer, last_def_reg);
}
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
      if (GET_CODE (PATTERN (producer)) == SET)
 {
   rtx dest = SET_DEST (PATTERN (producer));
   rtx src = SET_SRC (PATTERN (producer));
   if ((REG_P (dest) || GET_CODE (dest) == SUBREG)
       && (GET_CODE (src) == CONST_INT || GET_CODE (src) == HIGH))
     return false;
 }
      def_reg = SET_DEST (PATTERN (producer));
      break;
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;
    case TYPE_DIV:
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
bool
nds32_n9_2r1w_mm_to_ex_p (rtx producer, rtx consumer)
{
  rtx def_reg;
  switch (get_attr_type (producer))
    {
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
    case TYPE_DIV:
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
bool
nds32_n9_last_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  if (nds32_register_ports_config == REG_PORT_2R1W)
    {
      if (post_update_insn_p (producer))
 return false;
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
    case TYPE_DROUND:
    case TYPE_DBPICK:
    case TYPE_DWEXT:
      def_reg = SET_DEST (PATTERN (producer));
      break;
    case TYPE_DIV:
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
bool
nds32_n10_last_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  return n10_consumed_by_ex_dep_p (consumer, last_def_reg);
}
bool
nds32_gw_mm_to_ex_p (rtx producer, rtx consumer)
{
  rtx def_reg;
  switch (get_attr_type (producer))
    {
    case TYPE_LOAD:
    case TYPE_MUL:
    case TYPE_MAC:
      def_reg = SET_DEST (PATTERN (producer));
      break;
    case TYPE_DIV:
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
bool
nds32_gw_last_load_to_ex_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  return gw_consumed_by_ex_dep_p (consumer, last_def_reg);
}
bool
nds32_n13_e2_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg;
  switch (get_attr_type (producer))
    {
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
      def_reg = SET_DEST (PATTERN (producer));
      break;
    case TYPE_BRANCH:
      return true;
    case TYPE_DIV:
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
bool
nds32_n13_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx def_reg = SET_DEST (PATTERN (producer));
  gcc_assert (get_attr_type (producer) == TYPE_LOAD);
  gcc_assert (REG_P (def_reg) || GET_CODE (def_reg) == SUBREG);
  return n13_consumed_by_e1_dep_p (consumer, def_reg);
}
bool
nds32_n13_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx def_reg = SET_DEST (PATTERN (producer));
  gcc_assert (get_attr_type (producer) == TYPE_LOAD);
  gcc_assert (REG_P (def_reg) || GET_CODE (def_reg) == SUBREG);
  return n13_consumed_by_e2_dep_p (consumer, def_reg);
}
bool
nds32_n13_last_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  return n13_consumed_by_e1_dep_p (consumer, last_def_reg);
}
bool
nds32_n13_last_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  return n13_consumed_by_e2_dep_p (consumer, last_def_reg);
}
bool
nds32_n13_last_two_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_two_def_reg = extract_nth_access_reg (producer, -2);
  if (last_two_def_reg == NULL_RTX)
    return false;
  return n13_consumed_by_e1_dep_p (consumer, last_two_def_reg);
}
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
bool
nds32_pn_e3_to_e1_p (rtx producer, rtx consumer)
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
  return pn_consumed_by_e1_dep_p (consumer, def_reg);
}
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
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
      if (INSN_CODE (producer) == CODE_FOR_divmodsi4
   || INSN_CODE (producer) == CODE_FOR_udivmodsi4)
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
bool
nds32_pn_last_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  return pn_consumed_by_e1_dep_p (consumer, last_def_reg);
}
bool
nds32_pn_last_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  return pn_consumed_by_e2_dep_p (consumer, last_def_reg);
}
bool
nds32_pn_last_load_to_e3_p (rtx producer, rtx consumer)
{
  rtx last_def_reg = extract_nth_access_reg (producer, -1);
  return pn_consumed_by_e3_dep_p (consumer, last_def_reg);
}
bool
nds32_pn_last_two_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_two_def_reg = extract_nth_access_reg (producer, -2);
  if (last_two_def_reg == NULL_RTX)
    return false;
  return pn_consumed_by_e1_dep_p (consumer, last_two_def_reg);
}
bool
nds32_pn_last_two_load_to_e2_p (rtx producer, rtx consumer)
{
  rtx last_two_def_reg = extract_nth_access_reg (producer, -2);
  if (last_two_def_reg == NULL_RTX)
    return false;
  return pn_consumed_by_e2_dep_p (consumer, last_two_def_reg);
}
bool
nds32_pn_last_three_load_to_e1_p (rtx producer, rtx consumer)
{
  rtx last_three_def_reg = extract_nth_access_reg (producer, -3);
  if (last_three_def_reg == NULL_RTX)
    return false;
  return pn_consumed_by_e1_dep_p (consumer, last_three_def_reg);
}
