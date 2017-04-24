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
typedef bool (*rtx_cost_func) (rtx, int, int, int, int*);
struct rtx_cost_model_t {
  rtx_cost_func speed_prefer;
  rtx_cost_func size_prefer;
};
static rtx_cost_model_t rtx_cost_model;
static int insn_size_16bit;
static const int insn_size_32bit = 4;
static bool
nds32_rtx_costs_speed_prefer (rtx x ATTRIBUTE_UNUSED,
         int code,
         int outer_code ATTRIBUTE_UNUSED,
         int opno ATTRIBUTE_UNUSED,
         int *total)
{
  rtx op0;
  rtx op1;
  enum machine_mode mode = GET_MODE (x);
  int cost = COSTS_N_INSNS (GET_MODE_SIZE (mode) / GET_MODE_SIZE (SImode));
  switch (code)
    {
    case USE:
      *total = 0;
      return true;
    case CONST_INT:
      if (outer_code == SET || outer_code == PLUS)
 *total = satisfies_constraint_Is20 (x) ? 0 : 4;
      else if (outer_code == AND || outer_code == IOR || outer_code == XOR
        || outer_code == MINUS)
 *total = satisfies_constraint_Iu15 (x) ? 0 : 4;
      else if (outer_code == ASHIFT || outer_code == ASHIFTRT
        || outer_code == LSHIFTRT)
 *total = satisfies_constraint_Iu05 (x) ? 0 : 4;
      else if (GET_RTX_CLASS (outer_code) == RTX_COMPARE
        || GET_RTX_CLASS (outer_code) == RTX_COMM_COMPARE)
 *total = satisfies_constraint_Is16 (x) ? 0 : 4;
      else
 *total = COSTS_N_INSNS (1);
      return true;
    case CONST:
    case LO_SUM:
    case HIGH:
    case SYMBOL_REF:
      *total = COSTS_N_INSNS (1);
      return true;
    case MEM:
      *total = COSTS_N_INSNS (1);
      return true;
    case SET:
      op0 = SET_DEST (x);
      op1 = SET_SRC (x);
      mode = GET_MODE (op0);
      cost = COSTS_N_INSNS (GET_MODE_SIZE (mode) / GET_MODE_SIZE (SImode));
      switch (GET_CODE (op1))
 {
 case REG:
 case SUBREG:
   if ((REG_P (op0) || MEM_P (op0))
       && GET_MODE_SIZE (mode) <= GET_MODE_SIZE (DImode))
     *total = COSTS_N_INSNS (1);
   else
     *total = cost;
   return true;
 case MEM:
   if (REG_P (op0) && GET_MODE_SIZE (mode) <= GET_MODE_SIZE (DImode))
     *total = COSTS_N_INSNS (1);
   else
     *total = cost;
   return true;
 case CONST_INT:
   if (REG_P (op0) && GET_MODE_SIZE (mode) < GET_MODE_SIZE (DImode))
     {
       if (satisfies_constraint_Is20 (op1))
  *total = COSTS_N_INSNS (1) - 1;
       else
  *total = COSTS_N_INSNS (2);
     }
   else
     *total = cost;
   return true;
 case CONST:
 case SYMBOL_REF:
 case LABEL_REF:
   if (REG_P (op0) && GET_MODE_SIZE (mode) < GET_MODE_SIZE (DImode))
     *total = COSTS_N_INSNS (1) - 1;
   else
     *total = cost;
   return true;
 default:
   *total = cost;
   return true;
 }
    case PLUS:
      op0 = XEXP (x, 0);
      op1 = XEXP (x, 1);
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (DImode))
 *total = cost;
      else if (GET_CODE (op0) == MULT || GET_CODE (op0) == LSHIFTRT
        || GET_CODE (op1) == MULT || GET_CODE (op1) == LSHIFTRT)
 {
   if (TARGET_PIPELINE_PANTHER)
     *total = COSTS_N_INSNS (1);
   else
     *total = COSTS_N_INSNS (2);
 }
      else if ((GET_CODE (op1) == CONST_INT
  && satisfies_constraint_Is15 (op1))
  || REG_P (op1))
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (2);
      return true;
    case MINUS:
      op0 = XEXP (x, 0);
      op1 = XEXP (x, 1);
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (DImode))
 *total = cost;
      else if (GET_CODE (op0) == MULT || GET_CODE (op0) == LSHIFTRT
        || GET_CODE (op1) == MULT || GET_CODE (op1) == LSHIFTRT)
 {
   if (TARGET_PIPELINE_PANTHER)
     *total = COSTS_N_INSNS (1);
   else
     *total = COSTS_N_INSNS (2);
 }
      else if ((GET_CODE (op0) == CONST_INT
  && satisfies_constraint_Is15 (op0))
  || REG_P (op0))
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (2);
      return true;
    case TRUNCATE:
      *total = COSTS_N_INSNS (1);
      return true;
    case AND:
    case IOR:
    case XOR:
      op0 = XEXP (x, 0);
      op1 = XEXP (x, 1);
      if (NDS32_EXT_DSP_P ())
 {
   if (code == AND && GET_CODE (op0) == IOR && GET_CODE (op1) == IOR)
     return COSTS_N_INSNS (1);
   if (code == IOR && GET_CODE (op0) == AND && GET_CODE (op1) == AND)
     return COSTS_N_INSNS (10);
 }
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (DImode))
 *total = cost;
      else if (GET_CODE (op0) == ASHIFT || GET_CODE (op0) == LSHIFTRT)
 {
   if (TARGET_PIPELINE_PANTHER)
     *total = COSTS_N_INSNS (1);
   else
     *total = COSTS_N_INSNS (2);
 }
      else if ((GET_CODE (op1) == CONST_INT
        && satisfies_constraint_Iu15 (op1))
        || REG_P (op1))
 *total = COSTS_N_INSNS (1);
      else if (code == AND || GET_CODE (op0) == NOT)
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (2);
      return true;
    case MULT:
      if (GET_MODE (x) == DImode
   || GET_CODE (XEXP (x, 1)) == SIGN_EXTEND
   || GET_CODE (XEXP (x, 1)) == ZERO_EXTEND)
 *total = COSTS_N_INSNS (1);
      else if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (DImode))
 *total = cost;
      else if (outer_code == PLUS || outer_code == MINUS)
 {
   if (TARGET_PIPELINE_PANTHER)
     *total = COSTS_N_INSNS (1);
   else
     *total = COSTS_N_INSNS (2);
 }
      else if ((GET_CODE (XEXP (x, 1)) == CONST_INT
        && satisfies_constraint_Iu05 (XEXP (x, 1)))
        || REG_P (XEXP (x, 1)))
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (2);
      if (TARGET_MUL_SLOW)
 *total += COSTS_N_INSNS (4);
      return true;
    case LSHIFTRT:
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (DImode))
 *total = cost;
      else if (outer_code == PLUS || outer_code == MINUS
        || outer_code == AND || outer_code == IOR
        || outer_code == XOR)
 {
   if (TARGET_PIPELINE_PANTHER)
     *total = COSTS_N_INSNS (1);
   else
     *total = COSTS_N_INSNS (2);
 }
      else if ((GET_CODE (XEXP (x, 1)) == CONST_INT
        && satisfies_constraint_Iu05 (XEXP (x, 1)))
        || REG_P (XEXP (x, 1)))
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (2);
      return true;
    case ASHIFT:
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (DImode))
 *total = cost;
      else if (outer_code == AND || outer_code == IOR
        || outer_code == XOR)
 {
   if (TARGET_PIPELINE_PANTHER)
     *total = COSTS_N_INSNS (1);
   else
     *total = COSTS_N_INSNS (2);
 }
      else if ((GET_CODE (XEXP (x, 1)) == CONST_INT
        && satisfies_constraint_Iu05 (XEXP (x, 1)))
        || REG_P (XEXP (x, 1)))
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (2);
      return true;
    case ASHIFTRT:
    case ROTATERT:
      if (GET_MODE_SIZE (mode) >= GET_MODE_SIZE (DImode))
 *total = cost;
      else if ((GET_CODE (XEXP (x, 1)) == CONST_INT
        && satisfies_constraint_Iu05 (XEXP (x, 1)))
        || REG_P (XEXP (x, 1)))
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (2);
      return true;
    case LT:
    case LTU:
      if (outer_code == SET)
 {
   if ((GET_CODE (XEXP (x, 1)) == CONST_INT
       && satisfies_constraint_Iu15 (XEXP (x, 1)))
       || REG_P (XEXP (x, 1)))
     *total = COSTS_N_INSNS (1);
   else
     *total = COSTS_N_INSNS (2);
 }
      else
 *total = COSTS_N_INSNS (2);
      return true;
    case EQ:
    case NE:
    case GE:
    case LE:
    case GT:
      *total = COSTS_N_INSNS (2);
      return true;
    case IF_THEN_ELSE:
      if (GET_CODE (XEXP (x, 1)) == LABEL_REF)
 *total = COSTS_N_INSNS (2);
      else
 *total = COSTS_N_INSNS (1);
      return true;
    case LABEL_REF:
      if (outer_code == IF_THEN_ELSE)
 *total = COSTS_N_INSNS (2);
      else
 *total = COSTS_N_INSNS (1);
      return true;
    case ZERO_EXTEND:
    case SIGN_EXTEND:
      if (MEM_P (XEXP (x, 0)))
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (1);
      return true;
    case NEG:
    case NOT:
      *total = COSTS_N_INSNS (1);
      return true;
    case DIV:
    case UDIV:
    case MOD:
    case UMOD:
      *total = COSTS_N_INSNS (20);
      return true;
    case CALL:
      *total = COSTS_N_INSNS (2);
      return true;
    case CLZ:
    case SMIN:
    case SMAX:
    case ZERO_EXTRACT:
      if (TARGET_EXT_PERF)
 *total = COSTS_N_INSNS (1);
      else
 *total = COSTS_N_INSNS (3);
      return true;
    default:
      *total = COSTS_N_INSNS (3);
      return true;
    }
}
static bool
nds32_rtx_costs_size_prefer (rtx x,
        int code,
        int outer_code,
        int opno ATTRIBUTE_UNUSED,
        int *total)
{
  switch (code)
    {
    case SET:
      return false;
    case USE:
      *total = 0;
      break;
    case CONST_INT:
      if (outer_code == SET)
 {
   if (satisfies_constraint_Is05 (x))
     *total = insn_size_16bit;
   else if (satisfies_constraint_Is20 (x))
     *total = insn_size_32bit;
   else
     *total = insn_size_32bit * 2;
 }
      else if (outer_code == PLUS || outer_code == MINUS)
 {
   if (satisfies_constraint_Iu05 (x))
     *total = insn_size_16bit;
   else
     *total = insn_size_32bit;
 }
      else if (outer_code == ASHIFT)
 {
   if (satisfies_constraint_Iu03 (x))
     *total = insn_size_16bit;
   else
     *total = insn_size_32bit;
 }
      else if (outer_code == ASHIFTRT || outer_code == LSHIFTRT)
 {
   if (satisfies_constraint_Iu05 (x))
     *total = insn_size_16bit;
   else
     *total = insn_size_32bit;
 }
      else
 {
   *total = insn_size_32bit;
 }
      break;
    case CONST_DOUBLE:
      *total = insn_size_32bit * 2;
      break;
    case CONST:
    case SYMBOL_REF:
      *total = insn_size_32bit * 2;
      break;
    default:
      *total = insn_size_32bit;
      break;
    }
  return true;
}
void
nds32_init_rtx_costs (void)
{
  rtx_cost_model.speed_prefer = nds32_rtx_costs_speed_prefer;
  rtx_cost_model.size_prefer = nds32_rtx_costs_size_prefer;
  if (TARGET_16_BIT)
    insn_size_16bit = 2;
  else
    insn_size_16bit = 4;
}
bool
nds32_rtx_costs_impl (rtx x,
        int code,
        int outer_code,
        int opno,
        int *total,
        bool speed)
{
  if (speed)
    return rtx_cost_model.speed_prefer(x, code, outer_code, opno, total);
  else
    return rtx_cost_model.size_prefer(x, code, outer_code, opno, total);
}
int nds32_address_cost_speed_prefer (rtx address)
{
  rtx plus0, plus1;
  enum rtx_code code;
  code = GET_CODE (address);
  switch (code)
    {
    case POST_MODIFY:
    case POST_INC:
    case POST_DEC:
      return COSTS_N_INSNS (1) - 2;
    case SYMBOL_REF:
      return COSTS_N_INSNS (2);
    case CONST:
      return COSTS_N_INSNS (2);
    case REG:
      return COSTS_N_INSNS (1) - 2;
    case PLUS:
      plus0 = XEXP (address, 0);
      plus1 = XEXP (address, 1);
      if (REG_P (plus0) && CONST_INT_P (plus1))
 return COSTS_N_INSNS (1) - 2;
      else if (ARITHMETIC_P (plus0) || ARITHMETIC_P (plus1))
 return COSTS_N_INSNS (1) - 1;
      else if (REG_P (plus0) && REG_P (plus1))
 return COSTS_N_INSNS (1);
      return COSTS_N_INSNS (1);
    default:
      break;
    }
  return COSTS_N_INSNS (4);
}
int nds32_address_cost_speed_fwprop (rtx address)
{
  rtx plus0, plus1;
  enum rtx_code code;
  code = GET_CODE (address);
  switch (code)
    {
    case POST_MODIFY:
    case POST_INC:
    case POST_DEC:
      return 0;
    case SYMBOL_REF:
      return COSTS_N_INSNS (2);
    case CONST:
      return COSTS_N_INSNS (2);
    case REG:
      return COSTS_N_INSNS (1);
    case PLUS:
      plus0 = XEXP (address, 0);
      plus1 = XEXP (address, 1);
      if (REG_P (plus0) && CONST_INT_P (plus1))
 {
   if (satisfies_constraint_Iu03 (plus1))
     return (COSTS_N_INSNS (1) - 2);
   else
     return COSTS_N_INSNS (1);
 }
      if (ARITHMETIC_P (plus0) || ARITHMETIC_P (plus1))
 return COSTS_N_INSNS (1) - 2;
      else if (REG_P (plus0) && REG_P (plus1))
 return COSTS_N_INSNS (1);
      return COSTS_N_INSNS (1);
    default:
      break;
    }
  return COSTS_N_INSNS (4);
}
int nds32_address_cost_size_prefer (rtx address)
{
  rtx plus0, plus1;
  enum rtx_code code;
  code = GET_CODE (address);
  switch (code)
    {
    case POST_MODIFY:
    case POST_INC:
    case POST_DEC:
      return 0;
    case SYMBOL_REF:
      return COSTS_N_INSNS (2);
    case CONST:
      return COSTS_N_INSNS (2);
    case REG:
      return COSTS_N_INSNS (1) - 1;
    case PLUS:
      plus0 = XEXP (address, 0);
      plus1 = XEXP (address, 1);
      if (REG_P (plus0) && CONST_INT_P (plus1))
 {
   if (satisfies_constraint_Iu03 (plus1))
     return (COSTS_N_INSNS (1) - 2);
   else
     return COSTS_N_INSNS (1) - 1;
 }
      if (ARITHMETIC_P (plus0) || ARITHMETIC_P (plus1))
 return (COSTS_N_INSNS (1) - 1);
      return COSTS_N_INSNS (1);
    default:
      break;
    }
  return COSTS_N_INSNS (4);
}
int nds32_address_cost_impl (rtx address,
        enum machine_mode mode ATTRIBUTE_UNUSED,
        addr_space_t as ATTRIBUTE_UNUSED,
        bool speed_p)
{
  if (speed_p)
    {
      if (current_pass->tv_id == TV_FWPROP)
 return nds32_address_cost_speed_fwprop (address);
      else
 return nds32_address_cost_speed_prefer (address);
    }
  else
    return nds32_address_cost_size_prefer (address);
}
