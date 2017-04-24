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
#include "target.h"
#include "target-def.h"
#include "langhooks.h"
#include "ggc.h"
#include "tree-pass.h"
#include "target-globals.h"
#include "ira-int.h"
static bool
nds32_have_prologue_p (void)
{
  int i;
  for (i = 0; i < 28; i++)
    if (NDS32_REQUIRED_CALLEE_SAVED_P (i))
      return true;
  return (flag_pic
   || NDS32_REQUIRED_CALLEE_SAVED_P (FP_REGNUM)
   || NDS32_REQUIRED_CALLEE_SAVED_P (LP_REGNUM));
}
static int
nds32_get_symbol_count (void)
{
  int symbol_count = 0;
  rtx insn;
  basic_block bb;
  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS (bb, insn)
 {
   if (single_set (insn) && nds32_symbol_load_store_p (insn))
     {
       rtx pattern = PATTERN (insn);
       rtx mem;
       gcc_assert (GET_CODE (pattern) == SET);
       if (GET_CODE (SET_SRC (pattern)) == REG )
  mem = SET_DEST (pattern);
       else
  mem = SET_SRC (pattern);
       if (GET_CODE (mem) == MEM)
  symbol_count++;
     }
 }
    }
  return symbol_count;
}
static bool
nds32_fp_as_gp_check_available (void)
{
  basic_block bb;
  basic_block exit_bb;
  edge_iterator ei;
  edge e;
  bool first_exit_blocks_p;
  if (TARGET_FORBID_FP_AS_GP
      || nds32_naked_function_p (current_function_decl)
      || !optimize_size
      || frame_pointer_needed
      || NDS32_REQUIRED_CALLEE_SAVED_P (FP_REGNUM)
      || (cfun->stdarg == 1)
      || (find_fallthru_edge (EXIT_BLOCK_PTR_FOR_FN (cfun)->preds) == NULL))
    return false;
  first_exit_blocks_p = true;
  exit_bb = NULL;
  FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR_FOR_FN (cfun)->preds)
    {
      if (!first_exit_blocks_p)
 return false;
      exit_bb = e->src;
      first_exit_blocks_p = false;
    }
  if (!exit_bb)
    return false;
  FOR_EACH_BB_FN (bb, cfun)
    {
      if (!dominated_by_p (CDI_POST_DOMINATORS,
      bb,
      exit_bb))
 return false;
    }
  if (TARGET_FORCE_FP_AS_GP)
    {
      return true;
    }
  else
    {
      bool good_gain = false;
      int symbol_count;
      int threshold;
      if (flag_pic
   || nds32_have_prologue_p ())
 {
   threshold = 3;
 }
      else
 {
   threshold = 5 + (TARGET_V3PUSH ? 0 : 2);
 }
      symbol_count = nds32_get_symbol_count ();
      if (symbol_count >= threshold)
 good_gain = true;
      return good_gain;
    }
}
static unsigned int
nds32_fp_as_gp (void)
{
  bool fp_as_gp_p;
  calculate_dominance_info (CDI_POST_DOMINATORS);
  fp_as_gp_p = nds32_fp_as_gp_check_available ();
  if (fp_as_gp_p) {
    SET_HARD_REG_BIT(this_target_ira_int->x_no_unit_alloc_regs, FP_REGNUM);
    df_set_regs_ever_live (FP_REGNUM, 1);
  } else {
    CLEAR_HARD_REG_BIT(this_target_ira_int->x_no_unit_alloc_regs, FP_REGNUM);
  }
  cfun->machine->fp_as_gp_p = fp_as_gp_p;
  free_dominance_info (CDI_POST_DOMINATORS);
  return 1;
}
const pass_data pass_data_nds32_fp_as_gp =
{
  RTL_PASS,
  "fp_as_gp",
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
class pass_nds32_fp_as_gp : public rtl_opt_pass
{
public:
  pass_nds32_fp_as_gp (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_fp_as_gp, ctxt)
  {}
  bool gate () { return !TARGET_LINUX_ABI && TARGET_16_BIT && optimize_size; }
  unsigned int execute () { return nds32_fp_as_gp (); }
};
rtl_opt_pass *
make_pass_nds32_fp_as_gp (gcc::context *ctxt)
{
  return new pass_nds32_fp_as_gp (ctxt);
}
