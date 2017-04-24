#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "function.h"
#include "expr.h"
#include "df.h"
#include "tree-pass.h"
#define ARG0_REGNO 0
#define ARG1_REGNO 1
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
   rtx call_rtx = XVECEXP (pat, 0, 0);
   if (GET_CODE (call_rtx) == SET)
     call_rtx = SET_SRC (call_rtx);
   rtx func_mem = XEXP (call_rtx, 0);
   rtx symbol = XEXP (func_mem, 0);
   const char *func_name = XSTR (symbol, 0);
   if (!((strcmp("__mulsf3", func_name) == 0)
  || (strcmp("__addsf3", func_name) == 0)))
     continue;
   rtx prev_insn = insn;
   rtx arg0_insn = NULL_RTX;
   rtx arg1_insn = NULL_RTX;
   while ((prev_insn = PREV_INSN (prev_insn)) && prev_insn)
     {
       if (BLOCK_FOR_INSN (prev_insn) != BLOCK_FOR_INSN (insn))
  break;
       rtx set = PATTERN (prev_insn);
       rtx dst_reg = SET_DEST (set);
       if (!REG_P (dst_reg))
  break;
       unsigned regno = REGNO (dst_reg);
       if (regno == ARG0_REGNO)
  {
    arg0_insn = prev_insn;
    continue;
  }
       else if (regno == ARG1_REGNO)
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
   if ((REG_P (arg0_src) && REGNO (arg0_src) == ARG1_REGNO)
       || (REG_P (arg1_src) && REGNO (arg1_src) == ARG0_REGNO))
     {
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
  RTL_PASS,
  "soft_fp_arith_comm",
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
class pass_nds32_soft_fp_arith_comm_opt : public rtl_opt_pass
{
public:
  pass_nds32_soft_fp_arith_comm_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_soft_fp_arith_comm_opt, ctxt)
  {}
  bool gate () { return TARGET_SOFT_FP_ARITH_COMM && !TARGET_FPU_SINGLE; }
  unsigned int execute () { return nds32_soft_fp_arith_comm_opt (); }
};
rtl_opt_pass *
make_pass_nds32_soft_fp_arith_comm_opt (gcc::context *ctxt)
{
  return new pass_nds32_soft_fp_arith_comm_opt (ctxt);
}
