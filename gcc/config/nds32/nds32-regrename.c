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
#include "regrename.h"
static reg_class_t current_preferred_rename_class = NO_REGS;
reg_class_t
nds32_preferred_rename_class_impl (reg_class_t rclass)
{
  if (rclass == GENERAL_REGS)
    return current_preferred_rename_class;
  else
    return NO_REGS;
}
static void
print_hard_reg_set (FILE *file, HARD_REG_SET set)
{
  int i;
  fprintf (file, "{ ");
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (TEST_HARD_REG_BIT (set, i))
 fprintf (file, "%d ", i);
    }
  fprintf (file, "}\n");
}
void
dump_hard_reg_set (FILE *file, HARD_REG_SET set)
{
  print_hard_reg_set (file, set);
}
static bool
in_reg_class_p (unsigned regno, enum reg_class clazz)
{
  return TEST_HARD_REG_BIT (reg_class_contents[clazz], regno);
}
static unsigned
try_find_best_rename_reg (du_head_p op_chain, reg_class_t preferred_class)
{
  HARD_REG_SET unavailable;
  unsigned new_reg;
  current_preferred_rename_class = preferred_class;
  COMPL_HARD_REG_SET (unavailable, reg_class_contents[preferred_class]);
  CLEAR_HARD_REG_BIT (unavailable, op_chain->regno);
  new_reg = find_best_rename_reg (op_chain, GENERAL_REGS,
      &unavailable, op_chain->regno);
  current_preferred_rename_class = NO_REGS;
  return new_reg;
}
static bool
try_rename_operand_to (rtx insn, unsigned op_pos,
         reg_class_t preferred_rename_class)
{
  insn_rr_info *info;
  du_head_p op_chain;
  unsigned newreg;
  unsigned oldreg;
  info = &insn_rr[INSN_UID (insn)];
  if (info->op_info == NULL)
    return false;
  if (info->op_info[op_pos].n_chains == 0)
    return false;
  op_chain = regrename_chain_from_id (info->op_info[op_pos].heads[0]->id);
  if (op_chain->cannot_rename)
    return false;
  if (TEST_HARD_REG_BIT (reg_class_contents[preferred_rename_class],
    op_chain->regno))
    return false;
  if (dump_file)
    {
      fprintf (dump_file, "Try to rename operand %d to %s:\n",
        op_pos, reg_class_names[preferred_rename_class]);
      print_rtl_single (dump_file, insn);
    }
  oldreg = op_chain->regno;
  newreg = try_find_best_rename_reg (op_chain, preferred_rename_class);
  if (newreg == oldreg)
    return false;
  regrename_do_replace (op_chain, newreg);
  if (dump_file)
    {
      fprintf (dump_file, "Rename operand %d to %s is Done:\n",
        op_pos, reg_class_names[preferred_rename_class]);
      print_rtl_single (dump_file, insn);
    }
  return true;
}
static bool
rename_slt_profitlable (rtx insn)
{
  rtx pattern;
  pattern = PATTERN (insn);
  rtx src = SET_SRC (pattern);
  rtx op0 = XEXP (src, 0);
  rtx op1 = XEXP (src, 0);
  insn_rr_info *info;
  du_head_p op_chain;
  int op_pos = 0;
  info = &insn_rr[INSN_UID (insn)];
  if (info->op_info == NULL)
    return false;
  if (info->op_info[op_pos].n_chains == 0)
    return false;
  op_chain = regrename_chain_from_id (info->op_info[op_pos].heads[0]->id);
  if (in_reg_class_p (op_chain->regno, R15_TA_REG))
    return false;
  if (!REG_P (op0) || !in_reg_class_p (REGNO (op0), MIDDLE_REGS))
    return false;
  if (REG_P (op1)
      || (CONST_INT_P (op1) && satisfies_constraint_Iu05 (op1)))
    return true;
  return false;
}
static bool
rename_cbranch_eq0_low_reg_profitlable (rtx insn)
{
  insn_rr_info *info;
  du_head_p op_chain;
  int op_pos = 1;
  info = &insn_rr[INSN_UID (insn)];
  if (info->op_info == NULL)
    return false;
  if (info->op_info[op_pos].n_chains == 0)
    return false;
  op_chain = regrename_chain_from_id (info->op_info[op_pos].heads[0]->id);
  if (in_reg_class_p (op_chain->regno, LOW_REGS))
    return false;
  return true;
}
static bool
rename_cbranch_eq0_r15_profitlable (rtx insn)
{
  rtx pattern;
  pattern = PATTERN (insn);
  rtx if_then_else = SET_SRC (pattern);
  rtx cond = XEXP (if_then_else, 0);
  rtx op0 = XEXP (cond, 0);
  insn_rr_info *info;
  du_head_p op_chain;
  int op_pos = 1;
  info = &insn_rr[INSN_UID (insn)];
  if (info->op_info == NULL)
    return false;
  if (info->op_info[op_pos].n_chains == 0)
    return false;
  op_chain = regrename_chain_from_id (info->op_info[op_pos].heads[0]->id);
  if (in_reg_class_p (op_chain->regno, R15_TA_REG))
    return false;
  if (REG_P (op0) && in_reg_class_p (REGNO (op0), LOW_REGS))
    return false;
  return true;
}
static bool
rename_cbranch_eq_reg_profitlable (rtx insn)
{
  rtx pattern;
  pattern = PATTERN (insn);
  rtx if_then_else = SET_SRC (pattern);
  rtx cond = XEXP (if_then_else, 0);
  rtx op1 = XEXP (cond, 1);
  insn_rr_info *info;
  du_head_p op_chain;
  int op_pos = 1;
  info = &insn_rr[INSN_UID (insn)];
  if (info->op_info == NULL)
    return false;
  if (info->op_info[op_pos].n_chains == 0)
    return false;
  op_chain = regrename_chain_from_id (info->op_info[op_pos].heads[0]->id);
  if (in_reg_class_p (op_chain->regno, R5_REG))
    return false;
  if (REG_P (op1) && in_reg_class_p (REGNO (op1), LOW_REGS))
     return true;
  else
    return false;
}
static void
do_regrename ()
{
  basic_block bb;
  rtx insn;
  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS (bb, insn)
 {
   if (!INSN_P (insn))
     continue;
   switch (recog_memoized (insn))
     {
     case CODE_FOR_slts_compare_impl:
     case CODE_FOR_slt_compare_impl:
       if (rename_slt_profitlable (insn))
  try_rename_operand_to (insn, 0, R15_TA_REG);
       break;
     case CODE_FOR_slt_eq0:
       if (rename_slt_profitlable (insn))
  try_rename_operand_to (insn, 0, R15_TA_REG);
       break;
     case CODE_FOR_cbranchsi4_equality_zero:
       if (rename_cbranch_eq0_r15_profitlable (insn))
  if (!try_rename_operand_to (insn, 1, R15_TA_REG))
    if (rename_cbranch_eq0_low_reg_profitlable (insn))
      try_rename_operand_to (insn, 1, LOW_REGS);
       break;
     case CODE_FOR_cbranchsi4_equality_reg:
     case CODE_FOR_cbranchsi4_equality_reg_or_const_int:
       if (rename_cbranch_eq_reg_profitlable (insn))
  try_rename_operand_to (insn, 1, R5_REG);
       break;
     }
 }
    }
}
static unsigned int
nds32_regrename (void)
{
  df_set_flags (DF_LR_RUN_DCE);
  df_note_add_problem ();
  df_analyze ();
  df_set_flags (DF_DEFER_INSN_RESCAN);
  regrename_init (true);
  regrename_analyze (NULL);
  do_regrename ();
  regrename_finish ();
  return 1;
}
const pass_data pass_data_nds32_regrename =
{
  RTL_PASS,
  "nds32-regrename",
  OPTGROUP_NONE,
  true,
  true,
  TV_MACH_DEP,
  0,
  0,
  0,
  0,
  ( TODO_df_finish | TODO_verify_rtl_sharing ),
};
class pass_nds32_regrename_opt : public rtl_opt_pass
{
public:
  pass_nds32_regrename_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_regrename, ctxt)
  {}
  bool gate () { return TARGET_16_BIT && TARGET_REGRENAME_OPT; }
  unsigned int execute () { return nds32_regrename (); }
};
rtl_opt_pass *
make_pass_nds32_regrename_opt (gcc::context *ctxt)
{
  return new pass_nds32_regrename_opt (ctxt);
}
