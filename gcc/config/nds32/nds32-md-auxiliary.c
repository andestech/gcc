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
static int
nds32_regno_to_enable4 (unsigned regno)
{
  switch (regno)
    {
    case 28:
      return 0x8;
    case 29:
      return 0x4;
    case 30:
      return 0x2;
    case 31:
      return 0x1;
    default:
      gcc_unreachable ();
    }
}
static char
nds32_byte_to_size (int byte)
{
  switch (byte)
    {
    case 4:
      return 'w';
    case 2:
      return 'h';
    case 1:
      return 'b';
    default:
      gcc_unreachable ();
    }
}
static int
nds32_inverse_cond_code (int code)
{
  switch (code)
    {
      case NE:
 return EQ;
      case EQ:
 return NE;
      case GT:
 return LE;
      case LE:
 return GT;
      case GE:
 return LT;
      case LT:
 return GE;
      default:
 gcc_unreachable ();
    }
}
static const char *
nds32_cond_code_str (int code)
{
  switch (code)
    {
      case NE:
 return "ne";
      case EQ:
 return "eq";
      case GT:
 return "gt";
      case LE:
 return "le";
      case GE:
 return "ge";
      case LT:
 return "lt";
      default:
 gcc_unreachable ();
    }
}
static void
output_cond_branch (int code, const char *suffix, bool r5_p,
      bool long_jump_p, rtx *operands)
{
  char pattern[256];
  const char *cond_code;
  bool align_p = NDS32_ALIGN_P ();
  const char *align = align_p ? "\t.align\t2\n" : "";
  if (r5_p && REGNO (operands[2]) == 5 && TARGET_16_BIT)
    {
      if (code == EQ)
 {
   if (long_jump_p)
     snprintf (pattern, sizeof (pattern),
        "j\t%%3");
   else
     snprintf (pattern, sizeof (pattern),
        "j8\t%%3");
 }
      else
 pattern[0] = '\0';
    }
  else if (long_jump_p)
    {
      int inverse_code = nds32_inverse_cond_code (code);
      cond_code = nds32_cond_code_str (inverse_code);
      if (r5_p && TARGET_16_BIT)
 {
   snprintf (pattern, sizeof (pattern),
      "b%ss38\t %%2, .LCB%%=\n\tj\t%%3\n%s.LCB%%=:",
      cond_code, align);
 }
      else
 {
   snprintf (pattern, sizeof (pattern),
      "b%s%s\t%%1, %%2, .LCB%%=\n\tj\t%%3\n%s.LCB%%=:",
      cond_code, suffix, align);
 }
    }
  else
    {
      cond_code = nds32_cond_code_str (code);
      if (r5_p && TARGET_16_BIT)
 {
   snprintf (pattern, sizeof (pattern),
      "b%ss38\t %%2, %%3", cond_code);
 }
      else
 {
   snprintf (pattern, sizeof (pattern),
      "b%s%s\t%%1, %%2, %%3", cond_code, suffix);
 }
    }
  output_asm_insn (pattern, operands);
}
static void
output_cond_branch_compare_zero (int code, const char *suffix,
     bool long_jump_p, rtx *operands,
     bool ta_implied_p)
{
  char pattern[256];
  const char *cond_code;
  bool align_p = NDS32_ALIGN_P ();
  const char *align = align_p ? "\t.align\t2\n" : "";
  if (long_jump_p)
    {
      int inverse_code = nds32_inverse_cond_code (code);
      cond_code = nds32_cond_code_str (inverse_code);
      if (ta_implied_p && TARGET_16_BIT)
 {
   snprintf (pattern, sizeof (pattern),
      "b%sz%s\t.LCB%%=\n\tj\t%%2\n%s.LCB%%=:",
      cond_code, suffix, align);
 }
      else
 {
   snprintf (pattern, sizeof (pattern),
      "b%sz%s\t%%1, .LCB%%=\n\tj\t%%2\n%s.LCB%%=:",
      cond_code, suffix, align);
 }
    }
  else
    {
      cond_code = nds32_cond_code_str (code);
      if (ta_implied_p && TARGET_16_BIT)
 {
   snprintf (pattern, sizeof (pattern),
      "b%sz%s\t%%2", cond_code, suffix);
 }
      else
 {
   snprintf (pattern, sizeof (pattern),
      "b%sz%s\t%%1, %%2", cond_code, suffix);
 }
    }
  output_asm_insn (pattern, operands);
}
static void
nds32_split_shiftrtdi3 (rtx dst, rtx src, rtx shiftamount, bool logic_shift_p)
{
  rtx src_high_part;
  rtx dst_high_part, dst_low_part;
  dst_high_part = nds32_di_high_part_subreg (dst);
  src_high_part = nds32_di_high_part_subreg (src);
  dst_low_part = nds32_di_low_part_subreg (dst);
  if (CONST_INT_P (shiftamount))
    {
      if (INTVAL (shiftamount) < 32)
 {
   if (logic_shift_p)
     {
       emit_insn (gen_uwext (dst_low_part, src,
        shiftamount));
       emit_insn (gen_lshrsi3 (dst_high_part, src_high_part,
           shiftamount));
     }
   else
     {
       emit_insn (gen_wext (dst_low_part, src,
       shiftamount));
       emit_insn (gen_ashrsi3 (dst_high_part, src_high_part,
           shiftamount));
     }
 }
      else
 {
   rtx new_shift_amout = gen_int_mode(INTVAL (shiftamount) - 32, SImode);
   if (logic_shift_p)
     {
       emit_insn (gen_lshrsi3 (dst_low_part, src_high_part,
          new_shift_amout));
       emit_move_insn (dst_high_part, const0_rtx);
     }
   else
     {
       emit_insn (gen_ashrsi3 (dst_low_part, src_high_part,
          new_shift_amout));
       emit_insn (gen_ashrsi3 (dst_high_part, src_high_part,
           GEN_INT (31)));
     }
 }
    }
  else
    {
      rtx dst_low_part_l32, dst_high_part_l32;
      rtx dst_low_part_g32, dst_high_part_g32;
      rtx new_shift_amout, select_reg;
      dst_low_part_l32 = gen_reg_rtx (SImode);
      dst_high_part_l32 = gen_reg_rtx (SImode);
      dst_low_part_g32 = gen_reg_rtx (SImode);
      dst_high_part_g32 = gen_reg_rtx (SImode);
      new_shift_amout = gen_reg_rtx (SImode);
      select_reg = gen_reg_rtx (SImode);
      if (logic_shift_p)
 {
   emit_insn (gen_uwext (dst_low_part_l32, src, shiftamount));
   emit_insn (gen_lshrsi3 (dst_high_part_l32, src_high_part,
           shiftamount));
   emit_insn (gen_andsi3 (new_shift_amout, shiftamount, GEN_INT (0x1f)));
   emit_insn (gen_lshrsi3 (dst_low_part_g32, src_high_part,
          new_shift_amout));
   emit_move_insn (dst_high_part_g32, const0_rtx);
 }
      else
 {
   emit_insn (gen_wext (dst_low_part_l32, src, shiftamount));
   emit_insn (gen_ashrsi3 (dst_high_part_l32, src_high_part,
           shiftamount));
   emit_insn (gen_andsi3 (new_shift_amout, shiftamount, GEN_INT (0x1f)));
   emit_insn (gen_ashrsi3 (dst_low_part_g32, src_high_part,
          new_shift_amout));
   emit_insn (gen_ashrsi3 (dst_high_part_g32, src_high_part,
           GEN_INT (31)));
 }
      emit_insn (gen_slt_compare (select_reg, shiftamount, GEN_INT (32)));
      emit_insn (gen_cmovnsi (dst_low_part, select_reg,
         dst_low_part_l32, dst_low_part_g32));
      emit_insn (gen_cmovnsi (dst_high_part, select_reg,
         dst_high_part_l32, dst_high_part_g32));
  }
}
enum nds32_expand_result_type
nds32_expand_cbranch (rtx *operands)
{
  rtx tmp_reg;
  enum rtx_code code;
  code = GET_CODE (operands[0]);
  if (GET_CODE (operands[2]) == CONST_INT)
    if (INTVAL (operands[2]) == 0)
      if ((code != GTU)
   && (code != GEU)
   && (code != LTU)
   && (code != LEU))
 return EXPAND_CREATE_TEMPLATE;
  switch (code)
    {
    case GT:
    case GTU:
      if (GET_CODE (operands[2]) == CONST_INT)
 {
   if (optimize_size || optimize == 0)
     tmp_reg = gen_rtx_REG (SImode, TA_REGNUM);
   else
     tmp_reg = gen_reg_rtx (SImode);
   enum rtx_code cmp_code;
   rtx plus1 = gen_int_mode (INTVAL (operands[2]) + 1, SImode);
   if (satisfies_constraint_Is15 (plus1))
     {
       operands[2] = plus1;
       cmp_code = EQ;
       if (code == GT)
  {
    emit_insn (
      gen_slts_compare (tmp_reg, operands[1], operands[2]));
  }
       else
  {
    emit_insn (
      gen_slt_compare (tmp_reg, operands[1], operands[2]));
  }
     }
   else
     {
       cmp_code = NE;
       if (code == GT)
  {
    emit_insn (
      gen_slts_compare (tmp_reg, operands[2], operands[1]));
  }
       else
  {
    emit_insn (
      gen_slt_compare (tmp_reg, operands[2], operands[1]));
  }
     }
   PUT_CODE (operands[0], cmp_code);
   operands[1] = tmp_reg;
   operands[2] = const0_rtx;
   emit_insn (gen_cbranchsi4 (operands[0], operands[1],
         operands[2], operands[3]));
   return EXPAND_DONE;
 }
      else
 {
   if (optimize_size || optimize == 0)
     tmp_reg = gen_rtx_REG (SImode, TA_REGNUM);
   else
     tmp_reg = gen_reg_rtx (SImode);
   if (code == GT)
     {
       emit_insn (gen_slts_compare (tmp_reg, operands[2], operands[1]));
     }
   else
     {
       emit_insn (gen_slt_compare (tmp_reg, operands[2], operands[1]));
     }
   PUT_CODE (operands[0], NE);
   operands[1] = tmp_reg;
   operands[2] = const0_rtx;
   emit_insn (gen_cbranchsi4 (operands[0], operands[1],
         operands[2], operands[3]));
   return EXPAND_DONE;
 }
    case GE:
    case GEU:
      if (optimize_size || optimize == 0)
 tmp_reg = gen_rtx_REG (SImode, TA_REGNUM);
      else
 tmp_reg = gen_reg_rtx (SImode);
      if (code == GE)
 {
   emit_insn (gen_slts_compare (tmp_reg, operands[1], operands[2]));
 }
      else
 {
   emit_insn (gen_slt_compare (tmp_reg, operands[1], operands[2]));
 }
      PUT_CODE (operands[0], EQ);
      operands[1] = tmp_reg;
      operands[2] = const0_rtx;
      emit_insn (gen_cbranchsi4 (operands[0], operands[1],
     operands[2], operands[3]));
      return EXPAND_DONE;
    case LT:
    case LTU:
      if (optimize_size || optimize == 0)
 tmp_reg = gen_rtx_REG (SImode, TA_REGNUM);
      else
 tmp_reg = gen_reg_rtx (SImode);
      if (code == LT)
 {
   emit_insn (gen_slts_compare (tmp_reg, operands[1], operands[2]));
 }
      else
 {
   emit_insn (gen_slt_compare (tmp_reg, operands[1], operands[2]));
 }
      PUT_CODE (operands[0], NE);
      operands[1] = tmp_reg;
      operands[2] = const0_rtx;
      emit_insn (gen_cbranchsi4 (operands[0], operands[1],
     operands[2], operands[3]));
      return EXPAND_DONE;
    case LE:
    case LEU:
      if (GET_CODE (operands[2]) == CONST_INT)
 {
   if (optimize_size || optimize == 0)
     tmp_reg = gen_rtx_REG (SImode, TA_REGNUM);
   else
     tmp_reg = gen_reg_rtx (SImode);
   enum rtx_code cmp_code;
   rtx plus1 = gen_int_mode (INTVAL (operands[2]) + 1, SImode);
   if (satisfies_constraint_Is15 (plus1))
     {
       operands[2] = plus1;
       cmp_code = NE;
       if (code == LE)
  {
    emit_insn (
      gen_slts_compare (tmp_reg, operands[1], operands[2]));
  }
       else
  {
    emit_insn (
      gen_slt_compare (tmp_reg, operands[1], operands[2]));
  }
     }
   else
     {
       cmp_code = EQ;
       if (code == LE)
  {
    emit_insn (
      gen_slts_compare (tmp_reg, operands[2], operands[1]));
  }
       else
  {
    emit_insn (
      gen_slt_compare (tmp_reg, operands[2], operands[1]));
  }
     }
   PUT_CODE (operands[0], cmp_code);
   operands[1] = tmp_reg;
   operands[2] = const0_rtx;
   emit_insn (gen_cbranchsi4 (operands[0], operands[1],
         operands[2], operands[3]));
   return EXPAND_DONE;
 }
      else
 {
   if (optimize_size || optimize == 0)
     tmp_reg = gen_rtx_REG (SImode, TA_REGNUM);
   else
     tmp_reg = gen_reg_rtx (SImode);
   if (code == LE)
     {
       emit_insn (gen_slts_compare (tmp_reg, operands[2], operands[1]));
     }
   else
     {
       emit_insn (gen_slt_compare (tmp_reg, operands[2], operands[1]));
     }
   PUT_CODE (operands[0], EQ);
   operands[1] = tmp_reg;
   operands[2] = const0_rtx;
   emit_insn (gen_cbranchsi4 (operands[0], operands[1],
         operands[2], operands[3]));
   return EXPAND_DONE;
 }
    case EQ:
    case NE:
      if (GET_CODE (operands[2]) == CONST_INT
   && (!satisfies_constraint_Is11 (operands[2])
       || TARGET_ISA_V2))
 operands[2] = force_reg (SImode, operands[2]);
      return EXPAND_CREATE_TEMPLATE;
    default:
      return EXPAND_FAIL;
    }
}
enum nds32_expand_result_type
nds32_expand_cstore (rtx *operands)
{
  rtx tmp_reg;
  enum rtx_code code;
  code = GET_CODE (operands[1]);
  switch (code)
    {
    case EQ:
      if (GET_CODE (operands[3]) == CONST_INT)
 {
   tmp_reg = gen_reg_rtx (SImode);
   operands[3] = gen_int_mode (-INTVAL (operands[3]), SImode);
   if (!satisfies_constraint_Is15 (operands[3]))
     operands[3] = force_reg (SImode, operands[3]);
   emit_insn (gen_addsi3 (tmp_reg, operands[2], operands[3]));
   emit_insn (gen_slt_eq0 (operands[0], tmp_reg));
   return EXPAND_DONE;
 }
      else
 {
   tmp_reg = gen_reg_rtx (SImode);
   emit_insn (gen_xorsi3 (tmp_reg, operands[2], operands[3]));
   emit_insn (gen_slt_eq0 (operands[0], tmp_reg));
   return EXPAND_DONE;
 }
    case NE:
      if (GET_CODE (operands[3]) == CONST_INT)
 {
   tmp_reg = gen_reg_rtx (SImode);
   operands[3] = gen_int_mode (-INTVAL (operands[3]), SImode);
   if (!satisfies_constraint_Is15 (operands[3]))
     operands[3] = force_reg (SImode, operands[3]);
   emit_insn (gen_addsi3 (tmp_reg, operands[2], operands[3]));
   emit_insn (gen_slt_compare (operands[0], const0_rtx, tmp_reg));
   return EXPAND_DONE;
 }
      else
 {
   tmp_reg = gen_reg_rtx (SImode);
   emit_insn (gen_xorsi3 (tmp_reg, operands[2], operands[3]));
   emit_insn (gen_slt_compare (operands[0], const0_rtx, tmp_reg));
   return EXPAND_DONE;
 }
    case GT:
    case GTU:
      if (code == GT)
 {
   emit_insn (gen_slts_compare (operands[0], operands[3], operands[2]));
 }
      else
 {
   emit_insn (gen_slt_compare (operands[0], operands[3], operands[2]));
 }
      return EXPAND_DONE;
    case GE:
    case GEU:
      if (GET_CODE (operands[3]) == CONST_INT)
 {
   tmp_reg = gen_reg_rtx (SImode);
   emit_insn (gen_movsi (tmp_reg,
    gen_int_mode (INTVAL (operands[3]) - 1,
           SImode)));
   if (code == GE)
     {
       emit_insn (gen_slts_compare (operands[0], tmp_reg, operands[2]));
     }
   else
     {
       emit_insn (gen_slt_compare (operands[0], tmp_reg, operands[2]));
     }
   return EXPAND_DONE;
 }
      else
 {
   if (code == GE)
     {
       emit_insn (gen_slts_compare (operands[0],
        operands[2], operands[3]));
     }
   else
     {
       emit_insn (gen_slt_compare (operands[0],
        operands[2], operands[3]));
     }
   emit_insn (gen_xorsi3 (operands[0], operands[0], const1_rtx));
   return EXPAND_DONE;
 }
    case LT:
    case LTU:
      if (code == LT)
 {
   emit_insn (gen_slts_compare (operands[0], operands[2], operands[3]));
 }
      else
 {
   emit_insn (gen_slt_compare (operands[0], operands[2], operands[3]));
 }
      return EXPAND_DONE;
    case LE:
    case LEU:
      if (GET_CODE (operands[3]) == CONST_INT)
 {
   tmp_reg = gen_reg_rtx (SImode);
   emit_insn (gen_movsi (tmp_reg,
    gen_int_mode (INTVAL (operands[3]) + 1,
            SImode)));
   if (code == LE)
     {
       emit_insn (gen_slts_compare (operands[0], operands[2], tmp_reg));
     }
   else
     {
       emit_insn (gen_slt_compare (operands[0], operands[2], tmp_reg));
     }
   return EXPAND_DONE;
 }
      else
 {
   if (code == LE)
     {
       emit_insn (gen_slts_compare (operands[0],
        operands[3], operands[2]));
     }
   else
     {
       emit_insn (gen_slt_compare (operands[0],
        operands[3], operands[2]));
     }
   emit_insn (gen_xorsi3 (operands[0], operands[0], const1_rtx));
   return EXPAND_DONE;
 }
    default:
      gcc_unreachable ();
    }
}
void
nds32_expand_float_cbranch (rtx *operands)
{
  enum rtx_code code = GET_CODE (operands[0]);
  enum rtx_code new_code = code;
  rtx cmp_op0 = operands[1];
  rtx cmp_op1 = operands[2];
  rtx tmp_reg;
  rtx tmp;
  int reverse = 0;
  switch (code)
    {
    case GT:
    case GE:
      tmp = cmp_op0;
      cmp_op0 = cmp_op1;
      cmp_op1 = tmp;
      new_code = swap_condition (new_code);
      break;
    case UNORDERED:
    case LT:
    case LE:
    case EQ:
      break;
    case ORDERED:
    case NE:
      new_code = reverse_condition (new_code);
      reverse = 1;
      break;
    case UNGT:
    case UNGE:
      new_code = reverse_condition_maybe_unordered (new_code);
      reverse = 1;
      break;
    case UNLT:
    case UNLE:
      new_code = reverse_condition_maybe_unordered (new_code);
      tmp = cmp_op0;
      cmp_op0 = cmp_op1;
      cmp_op1 = tmp;
      new_code = swap_condition (new_code);
      reverse = 1;
      break;
    default:
      return;
    }
  tmp_reg = gen_reg_rtx (SImode);
  emit_insn (gen_rtx_SET (VOIDmode, tmp_reg,
     gen_rtx_fmt_ee (new_code, SImode,
       cmp_op0, cmp_op1)));
  PUT_CODE (operands[0], reverse ? EQ : NE);
  emit_insn (gen_cbranchsi4 (operands[0], tmp_reg,
        const0_rtx, operands[3]));
}
void
nds32_expand_float_cstore (rtx *operands)
{
  enum rtx_code code = GET_CODE (operands[1]);
  enum rtx_code new_code = code;
  enum machine_mode mode = GET_MODE (operands[2]);
  rtx cmp_op0 = operands[2];
  rtx cmp_op1 = operands[3];
  rtx tmp;
  switch (code)
    {
    case GT:
    case GE:
      tmp = cmp_op0;
      cmp_op0 = cmp_op1;
      cmp_op1 =tmp;
      new_code = swap_condition (new_code);
      break;
    case UNORDERED:
    case LT:
    case LE:
    case EQ:
      break;
    case ORDERED:
      if (mode == SFmode)
 emit_insn (gen_cmpsf_un (operands[0], cmp_op0, cmp_op1));
      else
 emit_insn (gen_cmpdf_un (operands[0], cmp_op0, cmp_op1));
      emit_insn (gen_xorsi3 (operands[0], operands[0], const1_rtx));
      return;
    case NE:
      if (mode == SFmode)
 emit_insn (gen_cmpsf_eq (operands[0], cmp_op0, cmp_op1));
      else
 emit_insn (gen_cmpdf_eq (operands[0], cmp_op0, cmp_op1));
      emit_insn (gen_xorsi3 (operands[0], operands[0], const1_rtx));
      return;
    default:
      return;
    }
  emit_insn (gen_rtx_SET (VOIDmode, operands[0],
     gen_rtx_fmt_ee (new_code, SImode,
       cmp_op0, cmp_op1)));
}
enum nds32_expand_result_type
nds32_expand_movcc (rtx *operands)
{
  enum rtx_code code = GET_CODE (operands[1]);
  enum rtx_code new_code = code;
  enum machine_mode cmp0_mode = GET_MODE (XEXP (operands[1], 0));
  rtx cmp_op0 = XEXP (operands[1], 0);
  rtx cmp_op1 = XEXP (operands[1], 1);
  rtx tmp;
  if ((GET_CODE (operands[1]) == EQ || GET_CODE (operands[1]) == NE)
      && XEXP (operands[1], 1) == const0_rtx)
    {
      return EXPAND_CREATE_TEMPLATE;
    }
  else if ((TARGET_FPU_SINGLE && cmp0_mode == SFmode)
    || (TARGET_FPU_DOUBLE && cmp0_mode == DFmode))
    {
      nds32_expand_float_movcc (operands);
    }
  else
    {
      int reverse = 0;
      switch (code)
 {
 case GE: case GEU: case LE: case LEU:
   new_code = reverse_condition (code);
   reverse = 1;
   break;
 case EQ:
 case NE:
   break;
 default:
   return EXPAND_FAIL;
 }
      if (new_code == GT || new_code == GTU)
 {
   tmp = cmp_op0;
   cmp_op0 = cmp_op1;
   cmp_op1 = tmp;
   new_code = swap_condition (new_code);
 }
      tmp = gen_reg_rtx (SImode);
      if (new_code == EQ || new_code == NE)
 {
   emit_insn (gen_xorsi3 (tmp, cmp_op0, cmp_op1));
   operands[1] = gen_rtx_fmt_ee (new_code, VOIDmode, tmp, const0_rtx);
 }
      else
 {
   if (new_code == LT)
     emit_insn (gen_slts_compare (tmp, cmp_op0, cmp_op1));
   else if (new_code == LTU)
     emit_insn (gen_slt_compare (tmp, cmp_op0, cmp_op1));
   else
     gcc_unreachable ();
   operands[1] = gen_rtx_fmt_ee (reverse ? EQ : NE,
     VOIDmode, tmp, const0_rtx);
 }
    }
  return EXPAND_CREATE_TEMPLATE;
}
void
nds32_expand_float_movcc (rtx *operands)
{
  if ((GET_CODE (operands[1]) == EQ || GET_CODE (operands[1]) == NE)
      && GET_MODE (XEXP (operands[1], 0)) == SImode
      && XEXP (operands[1], 1) == const0_rtx)
    {
      return;
    }
  else
    {
      enum rtx_code code = GET_CODE (operands[1]);
      enum rtx_code new_code = code;
      enum machine_mode cmp0_mode = GET_MODE (XEXP (operands[1], 0));
      enum machine_mode cmp1_mode = GET_MODE (XEXP (operands[1], 1));
      rtx cmp_op0 = XEXP (operands[1], 0);
      rtx cmp_op1 = XEXP (operands[1], 1);
      rtx tmp;
      int reverse = 0;
      switch (code)
 {
 case GT:
 case GE:
   tmp = cmp_op0;
   cmp_op0 = cmp_op1;
   cmp_op1 =tmp;
   new_code = swap_condition (new_code);
   break;
 case UNORDERED:
 case LT:
 case LE:
 case EQ:
   break;
 case ORDERED:
 case NE:
   reverse = 1;
   new_code = reverse_condition (new_code);
   break;
 case UNGT:
 case UNGE:
   new_code = reverse_condition_maybe_unordered (new_code);
   reverse = 1;
   break;
 case UNLT:
 case UNLE:
   new_code = reverse_condition_maybe_unordered (new_code);
   tmp = cmp_op0;
   cmp_op0 = cmp_op1;
   cmp_op1 = tmp;
   new_code = swap_condition (new_code);
   reverse = 1;
   break;
 default:
   return;
 }
      tmp = gen_reg_rtx (SImode);
      if ((cmp0_mode == DFmode || cmp0_mode == SFmode)
   && (cmp1_mode == DFmode || cmp1_mode == SFmode))
 {
   emit_insn (gen_rtx_SET (VOIDmode, tmp,
      gen_rtx_fmt_ee (new_code, SImode,
        cmp_op0, cmp_op1)));
 }
      else
 {
   PUT_CODE (operands[1], new_code);
   emit_insn (gen_cstoresi4 (tmp, operands[1],
        cmp_op0, cmp_op1));
 }
      operands[1] = gen_rtx_fmt_ee (reverse ? EQ : NE,
        VOIDmode, tmp, const0_rtx);
    }
}
void
nds32_emit_push_fpr_callee_saved (int base_offset)
{
  rtx fpu_insn;
  rtx reg, mem;
  unsigned int regno = cfun->machine->callee_saved_first_fpr_regno;
  unsigned int last_fpr = cfun->machine->callee_saved_last_fpr_regno;
  while (regno <= last_fpr)
    {
      reg = gen_rtx_REG (DFmode, regno);
      mem = gen_frame_mem (DFmode, plus_constant (Pmode,
        stack_pointer_rtx,
        base_offset));
      base_offset += 8;
      regno += 2;
      fpu_insn = emit_move_insn (mem, reg);
      RTX_FRAME_RELATED_P (fpu_insn) = 1;
    }
}
void
nds32_emit_pop_fpr_callee_saved (int gpr_padding_size)
{
  rtx fpu_insn;
  rtx reg, mem, addr;
  rtx dwarf, adjust_sp_rtx;
  unsigned int regno = cfun->machine->callee_saved_first_fpr_regno;
  unsigned int last_fpr = cfun->machine->callee_saved_last_fpr_regno;
  int padding = 0;
  while (regno <= last_fpr)
    {
      if ((regno + 1) >= last_fpr)
 padding = gpr_padding_size;
      reg = gen_rtx_REG (DFmode, (regno));
      addr = gen_rtx_POST_MODIFY (Pmode, stack_pointer_rtx,
      gen_rtx_PLUS (Pmode, stack_pointer_rtx,
      GEN_INT (8 + padding)));
      mem = gen_frame_mem (DFmode, addr);
      regno += 2;
      fpu_insn = emit_move_insn (reg, mem);
      adjust_sp_rtx =
 gen_rtx_SET (VOIDmode, stack_pointer_rtx,
       plus_constant (Pmode, stack_pointer_rtx,
        8 + padding));
      dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, NULL_RTX);
      dwarf = alloc_reg_note (REG_CFA_ADJUST_CFA, copy_rtx (adjust_sp_rtx),
         dwarf);
      RTX_FRAME_RELATED_P (fpu_insn) = 1;
      REG_NOTES (fpu_insn) = dwarf;
    }
}
void
nds32_emit_v3pop_fpr_callee_saved (int base)
{
  int fpu_base_addr = base;
  int regno;
  rtx fpu_insn;
  rtx reg, mem;
  rtx dwarf;
  regno = cfun->machine->callee_saved_first_fpr_regno;
  while (regno <= cfun->machine->callee_saved_last_fpr_regno)
    {
      reg = gen_rtx_REG (DFmode, regno);
      mem = gen_frame_mem (DFmode, plus_constant (Pmode,
        stack_pointer_rtx,
        fpu_base_addr));
      fpu_base_addr += 8;
      regno += 2;
      fpu_insn = emit_move_insn (reg, mem);
      dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, NULL_RTX);
      RTX_FRAME_RELATED_P (fpu_insn) = 1;
      REG_NOTES (fpu_insn) = dwarf;
    }
}
enum nds32_expand_result_type
nds32_expand_extv (rtx *operands)
{
  gcc_assert (CONST_INT_P (operands[2]) && CONST_INT_P (operands[3]));
  HOST_WIDE_INT width = INTVAL (operands[2]);
  HOST_WIDE_INT bitpos = INTVAL (operands[3]);
  rtx dst = operands[0];
  rtx src = operands[1];
  if (MEM_P (src)
      && width == 32
      && (bitpos % BITS_PER_UNIT) == 0
      && GET_MODE_BITSIZE (GET_MODE (dst)) == width)
    {
      rtx newmem = adjust_address (src, GET_MODE (dst),
       bitpos / BITS_PER_UNIT);
      rtx base_addr = force_reg (Pmode, XEXP (newmem, 0));
      emit_insn (gen_unaligned_loadsi (dst, base_addr));
      return EXPAND_DONE;
    }
  return EXPAND_FAIL;
}
enum nds32_expand_result_type
nds32_expand_insv (rtx *operands)
{
  gcc_assert (CONST_INT_P (operands[1]) && CONST_INT_P (operands[2]));
  HOST_WIDE_INT width = INTVAL (operands[1]);
  HOST_WIDE_INT bitpos = INTVAL (operands[2]);
  rtx dst = operands[0];
  rtx src = operands[3];
  if (MEM_P (dst)
      && width == 32
      && (bitpos % BITS_PER_UNIT) == 0
      && GET_MODE_BITSIZE (GET_MODE (src)) == width)
    {
      rtx newmem = adjust_address (dst, GET_MODE (src),
          bitpos / BITS_PER_UNIT);
      rtx base_addr = force_reg (Pmode, XEXP (newmem, 0));
      emit_insn (gen_unaligned_storesi (base_addr, src));
      return EXPAND_DONE;
    }
  return EXPAND_FAIL;
}
const char *
nds32_output_casesi_pc_relative (rtx *operands)
{
  enum machine_mode mode;
  rtx diff_vec;
  diff_vec = PATTERN (NEXT_INSN (operands[1]));
  gcc_assert (GET_CODE (diff_vec) == ADDR_DIFF_VEC);
  if (flag_pic)
    {
      output_asm_insn ("sethi\t$ta, hi20(%l1@GOTOFF)", operands);
      output_asm_insn ("ori\t$ta, $ta, lo12(%l1@GOTOFF)", operands);
      output_asm_insn ("add\t$ta, $ta, $gp", operands);
    }
  else
    output_asm_insn ("la\t$ta, %l1", operands);
  mode = GET_MODE (diff_vec);
  switch (mode)
    {
    case QImode:
      output_asm_insn ("lb\t%2, [$ta + %0 << 0]", operands);
      break;
    case HImode:
      output_asm_insn ("lh\t%2, [$ta + %0 << 1]", operands);
      break;
    case SImode:
      output_asm_insn ("lw\t%2, [$ta + %0 << 2]", operands);
      break;
    default:
      gcc_unreachable ();
    }
  output_asm_insn ("add\t$ta, %2, $ta", operands);
  if (TARGET_16_BIT)
    return "jr5\t$ta";
  else
    return "jr\t$ta";
}
const char *
nds32_output_casesi (rtx *operands)
{
  if (flag_pic)
    {
      output_asm_insn ("sethi\t$ta, hi20(%l1@GOTOFF)", operands);
      output_asm_insn ("ori\t$ta, $ta, lo12(%l1@GOTOFF)", operands);
      output_asm_insn ("add\t$ta, $ta, $gp", operands);
    }
  else
    output_asm_insn ("la\t$ta, %l1", operands);
  output_asm_insn ("lw\t%2, [$ta + %0 << 2]", operands);
  if (TARGET_16_BIT)
    return "jr5\t%2";
  else
    return "jr\t%2";
}
enum nds32_16bit_address_type
nds32_mem_format (rtx op)
{
  enum machine_mode mode_test;
  int val;
  int regno;
  if (!TARGET_16_BIT)
    return ADDRESS_NOT_16BIT_FORMAT;
  mode_test = GET_MODE (op);
  op = XEXP (op, 0);
  if (GET_CODE (op) == REG
      && ((mode_test == SImode) || (mode_test == SFmode)))
    return ADDRESS_REG;
  if (GET_CODE (op) == REG && (REGNO (op) < R8_REGNUM))
    return ADDRESS_LO_REG_IMM3U;
  if ((GET_CODE (op) == POST_INC)
      && ((mode_test == SImode) || (mode_test == SFmode)))
    {
      regno = REGNO(XEXP (op, 0));
      if (regno < 8)
 return ADDRESS_POST_INC_LO_REG_IMM3U;
    }
  if ((GET_CODE (op) == POST_MODIFY)
      && ((mode_test == SImode) || (mode_test == SFmode))
      && (REG_P (XEXP (XEXP (op, 1), 0)))
      && (CONST_INT_P (XEXP (XEXP (op, 1), 1))))
    {
      regno = REGNO (XEXP (XEXP (op, 1), 0));
      val = INTVAL (XEXP (XEXP (op, 1), 1));
      if (regno < 8 && val > 0 && val < 32)
 return ADDRESS_POST_MODIFY_LO_REG_IMM3U;
    }
  if ((GET_CODE (op) == PLUS)
      && (GET_CODE (XEXP (op, 0)) == REG)
      && (GET_CODE (XEXP (op, 1)) == CONST_INT))
    {
      val = INTVAL (XEXP (op, 1));
      regno = REGNO(XEXP (op, 0));
      if (regno > 8
   && regno != SP_REGNUM
   && regno != FP_REGNUM)
 return ADDRESS_NOT_16BIT_FORMAT;
      switch (mode_test)
 {
 case QImode:
   if (val >= 0 && val < 8 && regno < 8)
     return ADDRESS_LO_REG_IMM3U;
   break;
 case HImode:
   if (val >= 0 && val < 16 && (val % 2 == 0) && regno < 8)
     return ADDRESS_LO_REG_IMM3U;
   break;
 case SImode:
 case SFmode:
 case DFmode:
   if ((regno == 8) &&
       (val >= -128 && val <= -4 && (val % 4 == 0)))
     return ADDRESS_R8_IMM7U;
   if ((regno == FP_REGNUM) &&
       (val >= 0 && val < 512 && (val % 4 == 0)))
     return ADDRESS_FP_IMM7U;
   else if ((regno == SP_REGNUM) &&
     (val >= 0 && val < 512 && (val % 4 == 0)))
     return ADDRESS_SP_IMM7U;
   else if (val >= 0 && val < 32 && (val % 4 == 0) && regno < 8)
     return ADDRESS_LO_REG_IMM3U;
   break;
 default:
   break;
 }
    }
  return ADDRESS_NOT_16BIT_FORMAT;
}
const char *
nds32_output_16bit_store (rtx *operands, int byte)
{
  char pattern[100];
  char size;
  rtx code = XEXP (operands[0], 0);
  size = nds32_byte_to_size (byte);
  switch (nds32_mem_format (operands[0]))
    {
    case ADDRESS_REG:
      operands[0] = code;
      output_asm_insn ("swi450\t%1, [%0]", operands);
      break;
    case ADDRESS_LO_REG_IMM3U:
      snprintf (pattern, sizeof (pattern), "s%ci333\t%%1, %%0", size);
      output_asm_insn (pattern, operands);
      break;
    case ADDRESS_POST_INC_LO_REG_IMM3U:
      snprintf (pattern, sizeof (pattern), "swi333.bi\t%%1, %%0, 4");
      output_asm_insn (pattern, operands);
      break;
    case ADDRESS_POST_MODIFY_LO_REG_IMM3U:
      snprintf (pattern, sizeof (pattern), "swi333.bi\t%%1, %%0");
      output_asm_insn (pattern, operands);
      break;
    case ADDRESS_FP_IMM7U:
      output_asm_insn ("swi37\t%1, %0", operands);
      break;
    case ADDRESS_SP_IMM7U:
      operands[0] = XEXP (code, 1);
      output_asm_insn ("swi37.sp\t%1, [ + (%0)]", operands);
      break;
    default:
      break;
    }
  return "";
}
const char *
nds32_output_16bit_load (rtx *operands, int byte)
{
  char pattern[100];
  unsigned char size;
  rtx code = XEXP (operands[1], 0);
  size = nds32_byte_to_size (byte);
  switch (nds32_mem_format (operands[1]))
    {
    case ADDRESS_REG:
      operands[1] = code;
      output_asm_insn ("lwi450\t%0, [%1]", operands);
      break;
    case ADDRESS_LO_REG_IMM3U:
      snprintf (pattern, sizeof (pattern), "l%ci333\t%%0, %%1", size);
      output_asm_insn (pattern, operands);
      break;
    case ADDRESS_POST_INC_LO_REG_IMM3U:
      snprintf (pattern, sizeof (pattern), "lwi333.bi\t%%0, %%1, 4");
      output_asm_insn (pattern, operands);
      break;
    case ADDRESS_POST_MODIFY_LO_REG_IMM3U:
      snprintf (pattern, sizeof (pattern), "lwi333.bi\t%%0, %%1");
      output_asm_insn (pattern, operands);
      break;
    case ADDRESS_R8_IMM7U:
      output_asm_insn ("lwi45.fe\t%0, %e1", operands);
      break;
    case ADDRESS_FP_IMM7U:
      output_asm_insn ("lwi37\t%0, %1", operands);
      break;
    case ADDRESS_SP_IMM7U:
      operands[1] = XEXP (code, 1);
      output_asm_insn ("lwi37.sp\t%0, [ + (%1)]", operands);
      break;
    default:
      break;
    }
  return "";
}
const char *
nds32_output_32bit_store (rtx *operands, int byte)
{
  char pattern[100];
  unsigned char size;
  rtx code = XEXP (operands[0], 0);
  size = nds32_byte_to_size (byte);
  switch (GET_CODE (code))
    {
    case REG:
      snprintf (pattern, sizeof (pattern), "s%ci\t%%1, %%0", size);
      break;
    case SYMBOL_REF:
    case CONST:
      operands[0] = XEXP (operands[0], 0);
      snprintf (pattern, sizeof (pattern), "s%ci.gp\t%%1, [ + %%0]", size);
      break;
    case POST_INC:
      snprintf (pattern, sizeof (pattern),
  "s%ci.bi\t%%1, %%0, %d", size, byte);
      break;
    case POST_DEC:
      snprintf (pattern, sizeof (pattern),
  "s%ci.bi\t%%1, %%0, -%d", size, byte);
      break;
    case POST_MODIFY:
      switch (GET_CODE (XEXP (XEXP (code, 1), 1)))
 {
 case REG:
 case SUBREG:
   snprintf (pattern, sizeof (pattern), "s%c.bi\t%%1, %%0", size);
   break;
 case CONST_INT:
   snprintf (pattern, sizeof (pattern), "s%ci.bi\t%%1, %%0", size);
   break;
 default:
   abort ();
 }
      break;
    case PLUS:
      switch (GET_CODE (XEXP (code, 1)))
 {
 case REG:
 case SUBREG:
   snprintf (pattern, sizeof (pattern), "s%c\t%%1, %%0", size);
   break;
 case CONST_INT:
   snprintf (pattern, sizeof (pattern), "s%ci\t%%1, %%0", size);
   break;
 default:
   abort ();
 }
      break;
    case LO_SUM:
      operands[2] = XEXP (code, 1);
      operands[0] = XEXP (code, 0);
      snprintf (pattern, sizeof (pattern),
  "s%ci\t%%1, [%%0 + lo12(%%2)]", size);
      break;
    default:
      abort ();
    }
  output_asm_insn (pattern, operands);
  return "";
}
const char *
nds32_output_32bit_load (rtx *operands, int byte)
{
  char pattern[100];
  unsigned char size;
  rtx code;
  code = XEXP (operands[1], 0);
  size = nds32_byte_to_size (byte);
  switch (GET_CODE (code))
    {
    case REG:
      snprintf (pattern, sizeof (pattern), "l%ci\t%%0, %%1", size);
      break;
    case SYMBOL_REF:
    case CONST:
      operands[1] = XEXP (operands[1], 0);
      snprintf (pattern, sizeof (pattern), "l%ci.gp\t%%0, [ + %%1]", size);
      break;
    case POST_INC:
      snprintf (pattern, sizeof (pattern),
  "l%ci.bi\t%%0, %%1, %d", size, byte);
      break;
    case POST_DEC:
      snprintf (pattern, sizeof (pattern),
  "l%ci.bi\t%%0, %%1, -%d", size, byte);
      break;
    case POST_MODIFY:
      switch (GET_CODE (XEXP (XEXP (code, 1), 1)))
 {
 case REG:
 case SUBREG:
   snprintf (pattern, sizeof (pattern), "l%c.bi\t%%0, %%1", size);
   break;
 case CONST_INT:
   snprintf (pattern, sizeof (pattern), "l%ci.bi\t%%0, %%1", size);
   break;
 default:
   abort ();
 }
      break;
    case PLUS:
      switch (GET_CODE (XEXP (code, 1)))
 {
 case REG:
 case SUBREG:
   snprintf (pattern, sizeof (pattern), "l%c\t%%0, %%1", size);
   break;
 case CONST_INT:
   snprintf (pattern, sizeof (pattern), "l%ci\t%%0, %%1", size);
   break;
 default:
   abort ();
 }
      break;
    case LO_SUM:
      operands[2] = XEXP (code, 1);
      operands[1] = XEXP (code, 0);
      snprintf (pattern, sizeof (pattern),
  "l%ci\t%%0, [%%1 + lo12(%%2)]", size);
      break;
    default:
      abort ();
    }
  output_asm_insn (pattern, operands);
  return "";
}
const char *
nds32_output_32bit_load_se (rtx *operands, int byte)
{
  char pattern[100];
  unsigned char size;
  rtx code;
  code = XEXP (operands[1], 0);
  size = nds32_byte_to_size (byte);
  switch (GET_CODE (code))
    {
    case REG:
      snprintf (pattern, sizeof (pattern), "l%csi\t%%0, %%1", size);
      break;
    case SYMBOL_REF:
    case CONST:
      operands[1] = XEXP (operands[1], 0);
      snprintf (pattern, sizeof (pattern), "l%csi.gp\t%%0, [ + %%1]", size);
      break;
    case POST_INC:
      snprintf (pattern, sizeof (pattern),
  "l%csi.bi\t%%0, %%1, %d", size, byte);
      break;
    case POST_DEC:
      snprintf (pattern, sizeof (pattern),
  "l%csi.bi\t%%0, %%1, -%d", size, byte);
      break;
    case POST_MODIFY:
      switch (GET_CODE (XEXP (XEXP (code, 1), 1)))
 {
 case REG:
 case SUBREG:
   snprintf (pattern, sizeof (pattern), "l%cs.bi\t%%0, %%1", size);
   break;
 case CONST_INT:
   snprintf (pattern, sizeof (pattern), "l%csi.bi\t%%0, %%1", size);
   break;
 default:
   abort ();
 }
      break;
    case PLUS:
      switch (GET_CODE (XEXP (code, 1)))
 {
 case REG:
 case SUBREG:
   snprintf (pattern, sizeof (pattern), "l%cs\t%%0, %%1", size);
   break;
 case CONST_INT:
   snprintf (pattern, sizeof (pattern), "l%csi\t%%0, %%1", size);
   break;
 default:
   abort ();
 }
      break;
    case LO_SUM:
      operands[2] = XEXP (code, 1);
      operands[1] = XEXP (code, 0);
      snprintf (pattern, sizeof (pattern),
  "l%csi\t%%0, [%%1 + lo12(%%2)]", size);
      break;
    default:
      abort ();
    }
  output_asm_insn (pattern, operands);
  return "";
}
const char *
nds32_output_stack_push (rtx par_rtx)
{
  char pattern[100];
  rtx operands[3];
  int rb_va_args = cfun->machine->va_args_first_regno;
  int re_va_args = cfun->machine->va_args_last_regno;
  int last_argument_regno = NDS32_FIRST_GPR_REGNUM
       + NDS32_MAX_GPR_REGS_FOR_ARGS
       - 1;
  int rb_eh_data = cfun->machine->eh_return_data_first_regno;
  int re_eh_data = cfun->machine->eh_return_data_last_regno;
  int first_eh_data_regno = EH_RETURN_DATA_REGNO (0);
  int rb_callee_saved = cfun->machine->callee_saved_first_gpr_regno;
  int re_callee_saved = cfun->machine->callee_saved_last_gpr_regno;
  if (reg_mentioned_p (gen_rtx_REG (SImode, last_argument_regno), par_rtx))
    {
      operands[0] = gen_rtx_REG (SImode, rb_va_args);
      operands[1] = gen_rtx_REG (SImode, re_va_args);
      snprintf (pattern, sizeof (pattern), "push.s\t%s", "%0, %1, { }");
      output_asm_insn (pattern, operands);
      return "";
    }
  if (reg_mentioned_p (gen_rtx_REG (SImode, first_eh_data_regno), par_rtx))
    {
      operands[0] = gen_rtx_REG (SImode, rb_eh_data);
      operands[1] = gen_rtx_REG (SImode, re_eh_data);
      snprintf (pattern, sizeof (pattern), "push.s\t%s", "%0, %1, { }");
      output_asm_insn (pattern, operands);
      return "";
    }
  if (NDS32_V3PUSH_AVAILABLE_P)
    {
      int sp_adjust;
      operands[0] = gen_rtx_REG (SImode, re_callee_saved);
      sp_adjust = cfun->machine->local_size
    + cfun->machine->out_args_size
    + cfun->machine->callee_saved_area_gpr_padding_bytes
    + cfun->machine->callee_saved_fpr_regs_size;
      if (satisfies_constraint_Iu08 (GEN_INT (sp_adjust))
   && NDS32_DOUBLE_WORD_ALIGN_P (sp_adjust))
 operands[1] = GEN_INT (sp_adjust);
      else
 {
   if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
     {
       sp_adjust = cfun->machine->callee_saved_area_gpr_padding_bytes
     + cfun->machine->callee_saved_fpr_regs_size;
       operands[1] = GEN_INT (sp_adjust);
     }
   else
     {
       operands[1] = GEN_INT (0);
     }
 }
      snprintf (pattern, sizeof (pattern), "push25\t%%0, %%1");
    }
  else
    {
      int push_en4_only_p = 0;
      operands[0] = gen_rtx_REG (SImode, rb_callee_saved);
      operands[1] = gen_rtx_REG (SImode, re_callee_saved);
      if (!cfun->machine->fp_size
   && !cfun->machine->gp_size
   && !cfun->machine->lp_size
   && REGNO (operands[0]) == SP_REGNUM
   && REGNO (operands[1]) == SP_REGNUM)
 {
   return "";
 }
      else
 {
   if (REGNO (operands[0]) == SP_REGNUM
       && REGNO (operands[1]) == SP_REGNUM)
     push_en4_only_p = 1;
   snprintf (pattern, sizeof (pattern),
      "push.s\t%s{%s%s%s }",
      push_en4_only_p ? "" : "%0, %1, ",
      cfun->machine->fp_size ? " $fp" : "",
      cfun->machine->gp_size ? " $gp" : "",
      cfun->machine->lp_size ? " $lp" : "");
 }
    }
  output_asm_insn (pattern, operands);
  return "";
}
const char *
nds32_output_stack_pop (rtx par_rtx ATTRIBUTE_UNUSED)
{
  char pattern[100];
  rtx operands[3];
  int rb_eh_data = cfun->machine->eh_return_data_first_regno;
  int re_eh_data = cfun->machine->eh_return_data_last_regno;
  int first_eh_data_regno = EH_RETURN_DATA_REGNO (0);
  int rb_callee_saved = cfun->machine->callee_saved_first_gpr_regno;
  int re_callee_saved = cfun->machine->callee_saved_last_gpr_regno;
  if (reg_mentioned_p (gen_rtx_REG (SImode, first_eh_data_regno), par_rtx))
    {
      operands[0] = gen_rtx_REG (SImode, rb_eh_data);
      operands[1] = gen_rtx_REG (SImode, re_eh_data);
      snprintf (pattern, sizeof (pattern), "pop.s\t%s", "%0, %1, { }");
      output_asm_insn (pattern, operands);
      return "";
    }
  if (NDS32_V3PUSH_AVAILABLE_P)
    {
      int sp_adjust;
      operands[0] = gen_rtx_REG (SImode, re_callee_saved);
      sp_adjust = cfun->machine->local_size
    + cfun->machine->out_args_size
    + cfun->machine->callee_saved_area_gpr_padding_bytes
    + cfun->machine->callee_saved_fpr_regs_size;
      if (satisfies_constraint_Iu08 (GEN_INT (sp_adjust))
   && NDS32_DOUBLE_WORD_ALIGN_P (sp_adjust)
   && !cfun->calls_alloca)
 operands[1] = GEN_INT (sp_adjust);
      else
 {
   if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
     {
       sp_adjust = cfun->machine->callee_saved_area_gpr_padding_bytes
     + cfun->machine->callee_saved_fpr_regs_size;
       operands[1] = GEN_INT (sp_adjust);
     }
   else
     {
       operands[1] = GEN_INT (0);
     }
 }
      snprintf (pattern, sizeof (pattern), "pop25\t%%0, %%1");
    }
  else
    {
      int pop_en4_only_p = 0;
      operands[0] = gen_rtx_REG (SImode, rb_callee_saved);
      operands[1] = gen_rtx_REG (SImode, re_callee_saved);
      if (!cfun->machine->fp_size
   && !cfun->machine->gp_size
   && !cfun->machine->lp_size
   && REGNO (operands[0]) == SP_REGNUM
   && REGNO (operands[1]) == SP_REGNUM)
 {
   return "";
 }
      else
 {
   if (REGNO (operands[0]) == SP_REGNUM
       && REGNO (operands[1]) == SP_REGNUM)
     pop_en4_only_p = 1;
   snprintf (pattern, sizeof (pattern),
      "pop.s\t%s{%s%s%s }",
      pop_en4_only_p ? "" : "%0, %1, ",
      cfun->machine->fp_size ? " $fp" : "",
      cfun->machine->gp_size ? " $gp" : "",
      cfun->machine->lp_size ? " $lp" : "");
 }
    }
  output_asm_insn (pattern, operands);
  return "";
}
const char *
nds32_output_return (void)
{
  char pattern[100];
  rtx operands[2];
  int re_callee_saved = cfun->machine->callee_saved_last_gpr_regno;
  int sp_adjust;
  operands[0] = gen_rtx_REG (SImode, re_callee_saved);
  sp_adjust = cfun->machine->local_size
    + cfun->machine->out_args_size
    + cfun->machine->callee_saved_area_gpr_padding_bytes
    + cfun->machine->callee_saved_fpr_regs_size;
  if (satisfies_constraint_Iu08 (GEN_INT (sp_adjust))
      && NDS32_DOUBLE_WORD_ALIGN_P (sp_adjust)
      && !cfun->calls_alloca)
    operands[1] = GEN_INT (sp_adjust);
  else
    operands[1] = GEN_INT (0);
  snprintf (pattern, sizeof (pattern), "pop25\t%%0, %%1");
  output_asm_insn (pattern, operands);
  return "";
}
const char *
nds32_output_float_load (rtx *operands)
{
  char buff[100];
  const char *pattern;
  rtx addr, addr_op0, addr_op1;
  int dp = GET_MODE_SIZE (GET_MODE (operands[0])) == 8;
  addr = XEXP (operands[1], 0);
  switch (GET_CODE (addr))
    {
    case REG:
      pattern = "fl%ci\t%%0, %%1";
      break;
    case PLUS:
      addr_op0 = XEXP (addr, 0);
      addr_op1 = XEXP (addr, 1);
      if (REG_P (addr_op0) && REG_P (addr_op1))
 pattern = "fl%c\t%%0, %%1";
      else if (REG_P (addr_op0) && CONST_INT_P (addr_op1))
 pattern = "fl%ci\t%%0, %%1";
      else if (GET_CODE (addr_op0) == MULT && REG_P (addr_op1)
        && REG_P (XEXP (addr_op0, 0))
        && CONST_INT_P (XEXP (addr_op0, 1)))
 pattern = "fl%c\t%%0, %%1";
      else
 gcc_unreachable ();
      break;
    case POST_MODIFY:
      addr_op0 = XEXP (addr, 0);
      addr_op1 = XEXP (addr, 1);
      if (REG_P (addr_op0) && GET_CODE (addr_op1) == PLUS
   && REG_P (XEXP (addr_op1, 1)))
 pattern = "fl%c.bi\t%%0, %%1";
      else if (REG_P (addr_op0) && GET_CODE (addr_op1) == PLUS
        && CONST_INT_P (XEXP (addr_op1, 1)))
 pattern = "fl%ci.bi\t%%0, %%1";
      else
 gcc_unreachable ();
      break;
    case POST_INC:
      if (REG_P (XEXP (addr, 0)))
 {
   if (dp)
     pattern = "fl%ci.bi\t%%0, %%1, 8";
   else
     pattern = "fl%ci.bi\t%%0, %%1, 4";
 }
      else
 gcc_unreachable ();
      break;
    case POST_DEC:
      if (REG_P (XEXP (addr, 0)))
 {
   if (dp)
     pattern = "fl%ci.bi\t%%0, %%1, -8";
   else
     pattern = "fl%ci.bi\t%%0, %%1, -4";
 }
      else
 gcc_unreachable ();
      break;
    default:
      gcc_unreachable ();
    }
  sprintf (buff, pattern, dp ? 'd' : 's');
  output_asm_insn (buff, operands);
  return "";
}
const char *
nds32_output_float_store (rtx *operands)
{
  char buff[100];
  const char *pattern;
  rtx addr, addr_op0, addr_op1;
  int dp = GET_MODE_SIZE (GET_MODE (operands[0])) == 8;
  addr = XEXP (operands[0], 0);
  switch (GET_CODE (addr))
    {
    case REG:
      pattern = "fs%ci\t%%1, %%0";
      break;
    case PLUS:
      addr_op0 = XEXP (addr, 0);
      addr_op1 = XEXP (addr, 1);
      if (REG_P (addr_op0) && REG_P (addr_op1))
 pattern = "fs%c\t%%1, %%0";
      else if (REG_P (addr_op0) && CONST_INT_P (addr_op1))
 pattern = "fs%ci\t%%1, %%0";
      else if (GET_CODE (addr_op0) == MULT && REG_P (addr_op1)
        && REG_P (XEXP (addr_op0, 0))
        && CONST_INT_P (XEXP (addr_op0, 1)))
 pattern = "fs%c\t%%1, %%0";
      else
 gcc_unreachable ();
      break;
    case POST_MODIFY:
      addr_op0 = XEXP (addr, 0);
      addr_op1 = XEXP (addr, 1);
      if (REG_P (addr_op0) && GET_CODE (addr_op1) == PLUS
   && REG_P (XEXP (addr_op1, 1)))
 pattern = "fs%c.bi\t%%1, %%0";
      else if (REG_P (addr_op0) && GET_CODE (addr_op1) == PLUS
        && CONST_INT_P (XEXP (addr_op1, 1)))
 pattern = "fs%ci.bi\t%%1, %%0";
      else
 gcc_unreachable ();
      break;
    case POST_INC:
      if (REG_P (XEXP (addr, 0)))
 {
   if (dp)
     pattern = "fs%ci.bi\t%%1, %%0, 8";
   else
     pattern = "fs%ci.bi\t%%1, %%0, 4";
 }
      else
 gcc_unreachable ();
      break;
    case POST_DEC:
      if (REG_P (XEXP (addr, 0)))
 {
   if (dp)
     pattern = "fs%ci.bi\t%%1, %%0, -8";
   else
     pattern = "fs%ci.bi\t%%1, %%0, -4";
 }
      else
 gcc_unreachable ();
      break;
    default:
      gcc_unreachable ();
    }
  sprintf (buff, pattern, dp ? 'd' : 's');
  output_asm_insn (buff, operands);
  return "";
}
const char *
nds32_output_smw_single_word (rtx *operands)
{
  char buff[100];
  unsigned regno;
  int enable4;
  bool update_base_p;
  rtx base_addr = operands[0];
  rtx base_reg;
  rtx otherops[2];
  if (REG_P (XEXP (base_addr, 0)))
    {
      update_base_p = false;
      base_reg = XEXP (base_addr, 0);
    }
  else
    {
      update_base_p = true;
      base_reg = XEXP (XEXP (base_addr, 0), 0);
    }
  const char *update_base = update_base_p ? "m" : "";
  regno = REGNO (operands[1]);
  otherops[0] = base_reg;
  otherops[1] = operands[1];
  if (regno >= 28)
    {
      enable4 = nds32_regno_to_enable4 (regno);
      sprintf (buff, "smw.bi%s\t$sp, [%%0], $sp, %x", update_base, enable4);
    }
  else
    {
      sprintf (buff, "smw.bi%s\t%%1, [%%0], %%1", update_base);
    }
  output_asm_insn (buff, otherops);
  return "";
}
const char *
nds32_output_lmw_single_word (rtx *operands)
{
  char buff[100];
  unsigned regno;
  bool update_base_p;
  int enable4;
  rtx base_addr = operands[1];
  rtx base_reg;
  rtx otherops[2];
  if (REG_P (XEXP (base_addr, 0)))
    {
      update_base_p = false;
      base_reg = XEXP (base_addr, 0);
    }
  else
    {
      update_base_p = true;
      base_reg = XEXP (XEXP (base_addr, 0), 0);
    }
  const char *update_base = update_base_p ? "m" : "";
  regno = REGNO (operands[0]);
  otherops[0] = operands[0];
  otherops[1] = base_reg;
  if (regno >= 28)
    {
      enable4 = nds32_regno_to_enable4 (regno);
      sprintf (buff, "lmw.bi%s\t$sp, [%%1], $sp, %x", update_base, enable4);
    }
  else
    {
      sprintf (buff, "lmw.bi%s\t%%0, [%%1], %%0", update_base);
    }
  output_asm_insn (buff, otherops);
  return "";
}
void
nds32_expand_unaligned_load (rtx *operands, enum machine_mode mode)
{
  int offset = WORDS_BIG_ENDIAN ? GET_MODE_SIZE (mode) - 1 : 0;
  int offset_adj = WORDS_BIG_ENDIAN ? -1 : 1;
  int shift = 0;
  int width = GET_MODE_SIZE (mode) - 1;
  rtx mem[2];
  rtx reg[2];
  rtx sub_reg;
  rtx temp_reg, temp_sub_reg;
  int num_reg;
  temp_reg = gen_reg_rtx (SImode);
  temp_sub_reg = gen_lowpart (QImode, temp_reg);
  if (mode == DImode)
    {
      reg[0] = simplify_gen_subreg (SImode, operands[0],
        GET_MODE (operands[0]), 0);
      reg[1] = simplify_gen_subreg (SImode, operands[0],
        GET_MODE (operands[0]), 4);
      width = GET_MODE_SIZE (SImode) - 1;
    }
  else
    {
      if (VECTOR_MODE_P (mode))
 reg[0] = gen_reg_rtx (SImode);
      else
 reg[0] = operands[0];
    }
  for (num_reg = (mode == DImode) ? 2 : 1; num_reg > 0; num_reg--)
    {
      sub_reg = gen_lowpart (QImode, reg[0]);
      mem[0] = gen_rtx_MEM (QImode, plus_constant (Pmode, operands[1], offset));
      emit_move_insn (sub_reg, mem[0]);
      emit_insn (gen_zero_extendqisi2 (reg[0], sub_reg));
      while (width > 0)
 {
   offset = offset + offset_adj;
   shift++;
   width--;
   mem[1] = gen_rtx_MEM (QImode, plus_constant (Pmode,
             operands[1],
             offset));
   emit_move_insn (temp_sub_reg, mem[1]);
   emit_insn (gen_ashlsi3 (temp_reg, temp_reg,
      GEN_INT (shift * 8)));
   emit_insn (gen_iorsi3 (reg[0], reg[0], temp_reg));
 }
      if (mode == DImode)
 {
   reg[0] = reg[1];
   shift = 0;
   width = GET_MODE_SIZE (SImode) - 1;
   offset = offset + offset_adj;
 }
    }
    if (VECTOR_MODE_P (mode))
      convert_move (operands[0], reg[0], false);
}
void
nds32_expand_unaligned_store (rtx *operands, enum machine_mode mode)
{
  int offset = WORDS_BIG_ENDIAN ? GET_MODE_SIZE (mode) - 1 : 0;
  int offset_adj = WORDS_BIG_ENDIAN ? -1 : 1;
  int shift = 0;
  int width = GET_MODE_SIZE (mode) - 1;
  rtx mem[2];
  rtx reg[2];
  rtx sub_reg;
  rtx temp_reg, temp_sub_reg;
  int num_reg;
  temp_reg = gen_reg_rtx (SImode);
  temp_sub_reg = gen_lowpart (QImode, temp_reg);
  if (mode == DImode)
    {
      reg[0] = simplify_gen_subreg (SImode, operands[1],
        GET_MODE (operands[1]), 0);
      reg[1] = simplify_gen_subreg (SImode, operands[1],
        GET_MODE (operands[1]), 4);
      width = GET_MODE_SIZE (SImode) - 1;
    }
  else
    {
      if (VECTOR_MODE_P (mode))
 {
   reg[0] = gen_reg_rtx (SImode);
   convert_move (reg[0], operands[1], false);
 }
      else
 reg[0] = operands[1];
    }
  for (num_reg = (mode == DImode) ? 2 : 1; num_reg > 0; num_reg--)
    {
      sub_reg = gen_lowpart (QImode, reg[0]);
      mem[0] = gen_rtx_MEM (QImode, plus_constant (Pmode, operands[0], offset));
      emit_move_insn (mem[0], sub_reg);
      while (width > 0)
 {
   offset = offset + offset_adj;
   shift++;
   width--;
   mem[1] = gen_rtx_MEM (QImode, plus_constant (Pmode,
             operands[0],
             offset));
   emit_insn (gen_lshrsi3 (temp_reg, reg[0],
      GEN_INT (shift * 8)));
   emit_move_insn (mem[1], temp_sub_reg);
 }
      if (mode == DImode)
 {
   reg[0] = reg[1];
   shift = 0;
   width = GET_MODE_SIZE (SImode) - 1;
   offset = offset + offset_adj;
 }
    }
}
const char *
nds32_output_double (rtx *operands, bool load_p)
{
  char pattern[100];
  int reg = load_p ? 0 : 1;
  int mem = load_p ? 1 : 0;
  rtx otherops[3];
  rtx addr = XEXP (operands[mem], 0);
  otherops[0] = gen_rtx_REG (SImode, REGNO (operands[reg]));
  otherops[1] = gen_rtx_REG (SImode, REGNO (operands[reg]) + 1);
  if (GET_CODE (addr) == POST_INC)
    {
      otherops[2] = XEXP (addr, 0);
      snprintf (pattern, sizeof (pattern),
  "%cmw.bim\t%%0, [%%2], %%1, 0", load_p ? 'l' : 's');
    }
  else
    {
      otherops[2] = addr;
      snprintf (pattern, sizeof (pattern),
  "%cmw.bi\t%%0, [%%2], %%1, 0", load_p ? 'l' : 's');
    }
  output_asm_insn (pattern, otherops);
  return "";
}
const char *
nds32_output_cbranchsi4_equality_zero (rtx insn, rtx *operands)
{
  enum rtx_code code;
  bool long_jump_p = false;
  code = GET_CODE (operands[0]);
  switch (get_attr_length (insn))
    {
    case 8:
      long_jump_p = true;
    case 2:
      if (which_alternative == 0)
 {
   output_cond_branch_compare_zero (code, "s8", long_jump_p,
        operands, true);
   return "";
 }
      else if (which_alternative == 1)
 {
   output_cond_branch_compare_zero (code, "38", long_jump_p,
        operands, false);
   return "";
 }
      else
 {
   gcc_unreachable ();
 }
    case 10:
      long_jump_p = true;
    case 4:
      output_cond_branch_compare_zero (code, "", long_jump_p, operands, false);
      return "";
    default:
      gcc_unreachable ();
    }
}
const char *
nds32_output_cbranchsi4_equality_reg (rtx insn, rtx *operands)
{
  enum rtx_code code;
  bool long_jump_p, r5_p;
  int insn_length;
  insn_length = get_attr_length (insn);
  long_jump_p = (insn_length == 10 || insn_length == 8) ? true : false;
  r5_p = (insn_length == 2 || insn_length == 8) ? true : false;
  code = GET_CODE (operands[0]);
  switch (code)
    {
    case EQ:
    case NE:
      output_cond_branch (code, "", r5_p, long_jump_p, operands);
      return "";
    default:
      gcc_unreachable ();
    }
}
const char *
nds32_output_cbranchsi4_equality_reg_or_const_int (rtx insn, rtx *operands)
{
  enum rtx_code code;
  bool long_jump_p, r5_p;
  int insn_length;
  insn_length = get_attr_length (insn);
  long_jump_p = (insn_length == 10 || insn_length == 8) ? true : false;
  r5_p = (insn_length == 2 || insn_length == 8) ? true : false;
  code = GET_CODE (operands[0]);
  switch (code)
    {
    case EQ:
    case NE:
      if (which_alternative == 2)
 {
   output_cond_branch (code, "c", r5_p, long_jump_p, operands);
 }
      else
 {
   output_cond_branch (code, "", r5_p, long_jump_p, operands);
 }
      return "";
    default:
      gcc_unreachable ();
    }
}
const char *
nds32_output_cbranchsi4_greater_less_zero (rtx insn, rtx *operands)
{
  enum rtx_code code;
  bool long_jump_p;
  int insn_length;
  insn_length = get_attr_length (insn);
  gcc_assert (insn_length == 4 || insn_length == 10);
  long_jump_p = (insn_length == 10) ? true : false;
  code = GET_CODE (operands[0]);
  switch (code)
    {
    case GT:
    case GE:
    case LT:
    case LE:
      output_cond_branch_compare_zero (code, "", long_jump_p, operands, false);
      break;
    default:
      gcc_unreachable ();
    }
  return "";
}
const char *
nds32_output_unpkd8 (rtx output, rtx input,
       rtx high_idx_rtx, rtx low_idx_rtx,
       bool signed_p)
{
  char pattern[100];
  rtx output_operands[2];
  HOST_WIDE_INT high_idx, low_idx;
  high_idx = INTVAL (high_idx_rtx);
  low_idx = INTVAL (low_idx_rtx);
  gcc_assert (high_idx >= 0 && high_idx <= 3);
  gcc_assert (low_idx >= 0 && low_idx <= 3);
  if ((low_idx != 0 || high_idx == 0) &&
      !(low_idx == 1 && high_idx == 3))
    return "#";
  char sign_char = signed_p ? 's' : 'z';
  sprintf (pattern,
    "%cunpkd8" HOST_WIDE_INT_PRINT_DEC HOST_WIDE_INT_PRINT_DEC "\t%%0, %%1",
    sign_char, high_idx, low_idx);
  output_operands[0] = output;
  output_operands[1] = input;
  output_asm_insn (pattern, output_operands);
  return "";
}
static bool
nds32_symbol_binds_local_p (const_rtx x)
{
  return (SYMBOL_REF_DECL (x)
   ? targetm.binds_local_p (SYMBOL_REF_DECL (x))
   : SYMBOL_REF_LOCAL_P (x));
}
const char *
nds32_output_call (rtx insn, rtx *operands, rtx symbol, const char *long_call,
     const char *call, bool align_p)
{
  char pattern[100];
  bool noreturn_p;
  if (nds32_long_call_p (symbol))
    strcpy (pattern, long_call);
  else
    strcpy (pattern, call);
  if (flag_pic && CONSTANT_P (symbol)
      && !nds32_symbol_binds_local_p (symbol))
    strcat (pattern, "@PLT");
  if (align_p)
    strcat (pattern, "\n\t.align 2");
  noreturn_p = find_reg_note (insn, REG_NORETURN, NULL_RTX) != NULL_RTX;
  if (noreturn_p)
    {
      if (TARGET_16_BIT)
 strcat (pattern, "\n\tnop16");
      else
 strcat (pattern, "\n\tnop");
    }
  output_asm_insn (pattern, operands);
  return "";
}
bool
nds32_need_split_sms_p (rtx in0_idx0, rtx in1_idx0,
   rtx in0_idx1, rtx in1_idx1)
{
  if (INTVAL (in0_idx0) == INTVAL (in1_idx0)
      && INTVAL (in0_idx1) == INTVAL (in1_idx1)
      && INTVAL (in0_idx0) != INTVAL (in0_idx1))
    return false;
  if (INTVAL (in0_idx0) != INTVAL (in0_idx1)
      && INTVAL (in1_idx0) != INTVAL (in1_idx1))
    return false;
  return true;
}
const char *
nds32_output_sms (rtx in0_idx0, rtx in1_idx0,
    rtx in0_idx1, rtx in1_idx1)
{
  if (nds32_need_split_sms_p (in0_idx0, in1_idx0,
         in0_idx1, in1_idx1))
    return "#";
  if (INTVAL (in0_idx0) == INTVAL (in1_idx0)
      && INTVAL (in0_idx1) == INTVAL (in1_idx1)
      && INTVAL (in0_idx0) != INTVAL (in0_idx1))
    {
      if (INTVAL (in0_idx0) == 0)
 {
   if (TARGET_BIG_ENDIAN)
     return "smds\t%0, %1, %2";
   else
     return "smdrs\t%0, %1, %2";
 }
      else
 {
   if (TARGET_BIG_ENDIAN)
     return "smdrs\t%0, %1, %2";
   else
     return "smds\t%0, %1, %2";
 }
    }
  if (INTVAL (in0_idx0) != INTVAL (in0_idx1)
      && INTVAL (in1_idx0) != INTVAL (in1_idx1))
    {
      if (INTVAL (in0_idx0) == 1)
 {
   if (TARGET_BIG_ENDIAN)
     return "smxds\t%0, %2, %1";
   else
     return "smxds\t%0, %1, %2";
 }
      else
 {
   if (TARGET_BIG_ENDIAN)
     return "smxds\t%0, %1, %2";
   else
     return "smxds\t%0, %2, %1";
 }
    }
  gcc_unreachable ();
  return "";
}
void
nds32_split_sms (rtx out, rtx in0, rtx in1,
   rtx in0_idx0, rtx in1_idx0,
   rtx in0_idx1, rtx in1_idx1)
{
  rtx result0 = gen_reg_rtx (SImode);
  rtx result1 = gen_reg_rtx (SImode);
  emit_insn (gen_mulhisi3v (result0, in0, in1,
       in0_idx0, in1_idx0));
  emit_insn (gen_mulhisi3v (result1, in0, in1,
       in0_idx1, in1_idx1));
  emit_insn (gen_subsi3 (out, result0, result1));
}
void
nds32_spilt_doubleword (rtx *operands, bool load_p)
{
  int reg = load_p ? 0 : 1;
  int mem = load_p ? 1 : 0;
  rtx reg_rtx = load_p ? operands[0] : operands[1];
  rtx mem_rtx = load_p ? operands[1] : operands[0];
  rtx low_part[2], high_part[2];
  rtx sub_mem = XEXP (mem_rtx, 0);
  low_part[reg] = simplify_gen_subreg (SImode, reg_rtx, GET_MODE (reg_rtx), 0);
  high_part[reg] = simplify_gen_subreg (SImode, reg_rtx, GET_MODE (reg_rtx), 4);
  if (GET_CODE (sub_mem) == POST_DEC)
    {
      sub_mem = XEXP (sub_mem, 0);
      low_part[mem] = gen_frame_mem (SImode,
         gen_rtx_POST_MODIFY (Pmode, sub_mem,
         gen_rtx_PLUS (Pmode,
         sub_mem,
         GEN_INT (4))));
      high_part[mem] = gen_frame_mem (SImode,
          gen_rtx_POST_MODIFY (Pmode, sub_mem,
          gen_rtx_PLUS (Pmode,
          sub_mem,
          GEN_INT (-12))));
    }
  else if (GET_CODE (sub_mem) == POST_MODIFY)
    {
      rtx post_mem = XEXP (sub_mem, 0);
      rtx plus_op = XEXP (sub_mem, 1);
      rtx post_val = XEXP (plus_op, 1);
      low_part[mem] = gen_frame_mem (SImode,
         gen_rtx_POST_MODIFY (Pmode, post_mem,
         gen_rtx_PLUS (Pmode,
         post_mem,
         post_val)));
      high_part[mem] = gen_frame_mem (SImode, plus_constant (Pmode,
            post_mem,
            4));
    }
  else
    {
      low_part[mem] = adjust_address (mem_rtx, SImode, 0);
      high_part[mem] = adjust_address (mem_rtx, SImode, 4);
    }
  if ((load_p && reg_overlap_mentioned_p (low_part[0], high_part[1]))
      || GET_CODE (sub_mem) == POST_MODIFY)
    {
      operands[2] = high_part[0];
      operands[3] = high_part[1];
      operands[4] = low_part[0];
      operands[5] = low_part[1];
    }
  else
    {
      operands[2] = low_part[0];
      operands[3] = low_part[1];
      operands[4] = high_part[0];
      operands[5] = high_part[1];
    }
}
void
nds32_split_ashiftdi3 (rtx dst, rtx src, rtx shiftamount)
{
  rtx src_high_part, src_low_part;
  rtx dst_high_part, dst_low_part;
  dst_high_part = nds32_di_high_part_subreg (dst);
  dst_low_part = nds32_di_low_part_subreg (dst);
  src_high_part = nds32_di_high_part_subreg (src);
  src_low_part = nds32_di_low_part_subreg (src);
  if (CONST_INT_P (shiftamount))
    {
      if (INTVAL (shiftamount) < 32)
 {
   rtx ext_start;
   ext_start = gen_int_mode(32 - INTVAL (shiftamount), SImode);
   emit_insn (gen_wext (dst_high_part, src, ext_start));
   emit_insn (gen_ashlsi3 (dst_low_part, src_low_part, shiftamount));
 }
      else
 {
   rtx new_shift_amout = gen_int_mode(INTVAL (shiftamount) - 32, SImode);
   emit_insn (gen_ashlsi3 (dst_high_part, src_low_part,
       new_shift_amout));
   emit_move_insn (dst_low_part, GEN_INT (0));
 }
    }
  else
    {
      rtx dst_low_part_l32, dst_high_part_l32;
      rtx dst_low_part_g32, dst_high_part_g32;
      rtx new_shift_amout, select_reg;
      dst_low_part_l32 = gen_reg_rtx (SImode);
      dst_high_part_l32 = gen_reg_rtx (SImode);
      dst_low_part_g32 = gen_reg_rtx (SImode);
      dst_high_part_g32 = gen_reg_rtx (SImode);
      new_shift_amout = gen_reg_rtx (SImode);
      select_reg = gen_reg_rtx (SImode);
      rtx ext_start;
      ext_start = gen_reg_rtx (SImode);
      emit_insn (gen_subsi3 (ext_start,
        gen_int_mode (32, SImode),
        shiftamount));
      emit_insn (gen_wext (dst_high_part_l32, src, ext_start));
      emit_insn (gen_cmovzsi (dst_high_part_l32, shiftamount,
         src_high_part, dst_high_part_l32));
      emit_insn (gen_ashlsi3 (dst_low_part_l32, src_low_part, shiftamount));
      emit_move_insn (dst_low_part_g32, const0_rtx);
      emit_insn (gen_andsi3 (new_shift_amout, shiftamount, GEN_INT (0x1f)));
      emit_insn (gen_ashlsi3 (dst_high_part_g32, src_low_part,
       new_shift_amout));
      emit_insn (gen_slt_compare (select_reg, shiftamount, GEN_INT (32)));
      emit_insn (gen_cmovnsi (dst_low_part, select_reg,
         dst_low_part_l32, dst_low_part_g32));
      emit_insn (gen_cmovnsi (dst_high_part, select_reg,
         dst_high_part_l32, dst_high_part_g32));
    }
}
void
nds32_split_ashiftrtdi3 (rtx dst, rtx src, rtx shiftamount)
{
  nds32_split_shiftrtdi3 (dst, src, shiftamount, false);
}
void
nds32_split_lshiftrtdi3 (rtx dst, rtx src, rtx shiftamount)
{
  nds32_split_shiftrtdi3 (dst, src, shiftamount, true);
}
void
nds32_split_rotatertdi3 (rtx dst, rtx src, rtx shiftamount)
{
  rtx dst_low_part_l32, dst_high_part_l32;
  rtx dst_low_part_g32, dst_high_part_g32;
  rtx select_reg, low5bit, low5bit_inv, minus32sa;
  rtx dst_low_part_g32_tmph;
  rtx dst_low_part_g32_tmpl;
  rtx dst_high_part_l32_tmph;
  rtx dst_high_part_l32_tmpl;
  rtx src_low_part, src_high_part;
  rtx dst_high_part, dst_low_part;
  shiftamount = force_reg (SImode, shiftamount);
  emit_insn (gen_andsi3 (shiftamount,
    shiftamount,
    gen_int_mode (0x3f, SImode)));
  dst_high_part = nds32_di_high_part_subreg (dst);
  dst_low_part = nds32_di_low_part_subreg (dst);
  src_high_part = nds32_di_high_part_subreg (src);
  src_low_part = nds32_di_low_part_subreg (src);
  dst_low_part_l32 = gen_reg_rtx (SImode);
  dst_high_part_l32 = gen_reg_rtx (SImode);
  dst_low_part_g32 = gen_reg_rtx (SImode);
  dst_high_part_g32 = gen_reg_rtx (SImode);
  low5bit = gen_reg_rtx (SImode);
  low5bit_inv = gen_reg_rtx (SImode);
  minus32sa = gen_reg_rtx (SImode);
  select_reg = gen_reg_rtx (SImode);
  dst_low_part_g32_tmph = gen_reg_rtx (SImode);
  dst_low_part_g32_tmpl = gen_reg_rtx (SImode);
  dst_high_part_l32_tmph = gen_reg_rtx (SImode);
  dst_high_part_l32_tmpl = gen_reg_rtx (SImode);
  emit_insn (gen_slt_compare (select_reg, shiftamount, GEN_INT (32)));
  emit_insn (gen_andsi3 (low5bit, shiftamount, gen_int_mode (0x1f, SImode)));
  emit_insn (gen_subsi3 (low5bit_inv, gen_int_mode (32, SImode), low5bit));
  emit_insn (gen_wext (dst_low_part_l32, src, shiftamount));
  emit_insn (gen_lshrsi3 (dst_low_part_g32_tmpl, src_high_part, low5bit));
  emit_insn (gen_ashlsi3 (dst_low_part_g32_tmph, src_low_part, low5bit_inv));
  emit_insn (gen_iorsi3 (dst_low_part_g32,
    dst_low_part_g32_tmpl,
    dst_low_part_g32_tmph));
  emit_insn (gen_cmovnsi (dst_low_part, select_reg,
     dst_low_part_l32, dst_low_part_g32));
  emit_insn (gen_subsi3 (minus32sa, gen_int_mode (32, SImode), shiftamount));
  emit_insn (gen_lshrsi3 (dst_high_part_l32_tmpl, src_high_part, shiftamount));
  emit_insn (gen_ashlsi3 (dst_high_part_l32_tmph, src_low_part, minus32sa));
  emit_insn (gen_iorsi3 (dst_high_part_l32,
    dst_high_part_l32_tmpl,
    dst_high_part_l32_tmph));
  emit_insn (gen_cmovzsi (dst_high_part_l32, shiftamount,
     src_high_part, dst_high_part_l32));
  emit_insn (gen_wext (dst_high_part_g32, src, low5bit));
  emit_insn (gen_cmovnsi (dst_high_part, select_reg,
     dst_high_part_l32, dst_high_part_g32));
}
bool
symbolic_reference_mentioned_p (rtx op)
{
  const char *fmt;
  int i;
  if (GET_CODE (op) == SYMBOL_REF || GET_CODE (op) == LABEL_REF)
    return true;
  fmt = GET_RTX_FORMAT (GET_CODE (op));
  for (i = GET_RTX_LENGTH (GET_CODE (op)) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'E')
 {
   int j;
   for (j = XVECLEN (op, i) - 1; j >= 0; j--)
     if (symbolic_reference_mentioned_p (XVECEXP (op, i, j)))
       return true;
 }
      else if (fmt[i] == 'e' && symbolic_reference_mentioned_p (XEXP (op, i)))
 return true;
    }
  return false;
}
rtx
nds32_legitimize_pic_address (rtx x)
{
  rtx addr = x;
  rtx reg = gen_reg_rtx (Pmode);
  rtx pat;
  if (GET_CODE (x) == LABEL_REF
      || (GET_CODE (x) == SYMBOL_REF
   && (CONSTANT_POOL_ADDRESS_P (x)
       || SYMBOL_REF_LOCAL_P (x))))
    {
      addr = gen_rtx_UNSPEC (SImode, gen_rtvec (1, x), UNSPEC_GOTOFF);
      addr = gen_rtx_CONST (SImode, addr);
      emit_insn (gen_sethi (reg, addr));
      emit_insn (gen_lo_sum (reg, reg, addr));
      x = gen_rtx_PLUS (Pmode, reg, pic_offset_table_rtx);
    }
  else if (GET_CODE (x) == SYMBOL_REF)
    {
      addr = gen_rtx_UNSPEC (SImode, gen_rtvec (1, x), UNSPEC_GOT);
      addr = gen_rtx_CONST (SImode, addr);
      emit_insn (gen_sethi (reg, addr));
      emit_insn (gen_lo_sum (reg, reg, addr));
      x = gen_const_mem (SImode, gen_rtx_PLUS (Pmode, pic_offset_table_rtx,
            reg));
    }
  else if (GET_CODE (x) == CONST)
    {
      addr = XEXP (x, 0);
      gcc_assert (GET_CODE (addr) == PLUS);
      rtx op0 = XEXP (addr, 0);
      rtx op1 = XEXP (addr, 1);
      if ((GET_CODE (op0) == LABEL_REF
    || (GET_CODE (op0) == SYMBOL_REF
        && (CONSTANT_POOL_ADDRESS_P (op0)
     || SYMBOL_REF_LOCAL_P (op0))))
   && GET_CODE (op1) == CONST_INT)
 {
   pat = gen_rtx_UNSPEC (Pmode, gen_rtvec (1, op0), UNSPEC_GOTOFF);
   pat = gen_rtx_PLUS (Pmode, pat, op1);
   pat = gen_rtx_CONST (Pmode, pat);
   emit_insn (gen_sethi (reg, pat));
   emit_insn (gen_lo_sum (reg, reg, pat));
   x = gen_rtx_PLUS (Pmode, reg, pic_offset_table_rtx);
 }
      else if (GET_CODE (op0) == SYMBOL_REF
        && GET_CODE (op1) == CONST_INT)
 {
   addr = gen_rtx_UNSPEC (SImode, gen_rtvec (1, op0), UNSPEC_GOT);
   addr = gen_rtx_CONST (SImode, addr);
   emit_insn (gen_sethi (reg, addr));
   emit_insn (gen_lo_sum (reg, reg, addr));
   addr = gen_const_mem (SImode, gen_rtx_PLUS (Pmode,
            pic_offset_table_rtx,
            reg));
   emit_move_insn (reg, addr);
   if (satisfies_constraint_Is15 (op1))
     x = gen_rtx_PLUS (Pmode, reg, op1);
   else
     {
       rtx tmp_reg = gen_reg_rtx (SImode);
       emit_insn (gen_movsi (tmp_reg, op1));
       x = gen_rtx_PLUS (Pmode, reg, tmp_reg);
     }
 }
      else
 {
   debug_rtx (x);
   gcc_unreachable ();
 }
    }
  return x;
}
void
nds32_expand_pic_move (rtx *operands)
{
  rtx src;
  src = nds32_legitimize_pic_address (operands[1]);
  emit_move_insn (operands[0], src);
}
rtx
nds32_legitimize_ict_address (rtx x)
{
  rtx symbol = x;
  rtx addr = x;
  rtx reg = gen_reg_rtx (Pmode);
  gcc_assert (GET_CODE (x) == SYMBOL_REF
       && nds32_indirect_call_referenced_p (x));
  addr = gen_rtx_UNSPEC (SImode, gen_rtvec (1, symbol), UNSPEC_ICT);
  addr = gen_rtx_CONST (SImode, addr);
  emit_insn (gen_sethi (reg, addr));
  x = gen_const_mem (SImode, gen_rtx_LO_SUM (Pmode, reg, addr));
  return x;
}
void
nds32_expand_ict_move (rtx *operands)
{
  rtx src = operands[1];
  src = nds32_legitimize_ict_address (src);
  emit_move_insn (operands[0], src);
}
bool
nds32_indirect_call_referenced_p (rtx x)
{
  if (GET_CODE (x) == UNSPEC && XINT (x, 1) == UNSPEC_ICT)
    x = XVECEXP (x, 0, 0);
  if (GET_CODE (x) == SYMBOL_REF)
    {
      tree decl = SYMBOL_REF_DECL (x);
      return decl
      && (lookup_attribute("indirect_call",
      DECL_ATTRIBUTES(decl))
   != NULL);
    }
  return false;
}
bool
nds32_long_call_p (rtx symbol)
{
  if (nds32_indirect_call_referenced_p (symbol))
    return TARGET_ICT_MODEL_LARGE;
  else
    return TARGET_CMODEL_LARGE;
}
bool
nds32_tls_referenced_p (rtx x)
{
  if (!targetm.have_tls)
   return false;
  if (GET_CODE (x) == CONST && GET_CODE (XEXP (x, 0)) == PLUS)
    x = XEXP (XEXP (x, 0), 0);
  if (GET_CODE (x) == SYMBOL_REF && SYMBOL_REF_TLS_MODEL (x))
    return true;
  return false;
}
rtx
nds32_legitimize_tls_address (rtx x)
{
  rtx tmp_reg;
  rtx tp_reg = gen_rtx_REG (Pmode, TP_REGNUM);
  rtx pat, insns, reg0;
  if (GET_CODE (x) == SYMBOL_REF)
    switch (SYMBOL_REF_TLS_MODEL (x))
      {
      case TLS_MODEL_GLOBAL_DYNAMIC:
      case TLS_MODEL_LOCAL_DYNAMIC:
 if (SYMBOL_REF_TLS_MODEL (x) == TLS_MODEL_GLOBAL_DYNAMIC)
   pat = gen_rtx_UNSPEC (SImode, gen_rtvec (1, x), UNSPEC_TLSGD);
 else
   pat = gen_rtx_UNSPEC (SImode, gen_rtvec (1, x), UNSPEC_TLSLD);
 pat = gen_rtx_CONST (SImode, pat);
 reg0 = gen_rtx_REG (Pmode, 0);
 insns = emit_call_insn (gen_tls_desc (pat, GEN_INT (0)));
 use_reg (&CALL_INSN_FUNCTION_USAGE (insns), pic_offset_table_rtx);
 RTL_CONST_CALL_P (insns) = 1;
 tmp_reg = gen_reg_rtx (SImode);
 emit_move_insn (tmp_reg, reg0);
 x = tmp_reg;
 break;
      case TLS_MODEL_INITIAL_EXEC:
 pat = gen_rtx_UNSPEC (SImode, gen_rtvec (1, x), UNSPEC_TLSIE);
 tmp_reg = gen_reg_rtx (SImode);
 pat = gen_rtx_CONST (SImode, pat);
 emit_insn (gen_tls_ie (tmp_reg, pat, GEN_INT (0)));
 if (flag_pic)
   emit_use (pic_offset_table_rtx);
 x = gen_rtx_PLUS (Pmode, tmp_reg, tp_reg);
 break;
      case TLS_MODEL_LOCAL_EXEC:
 tmp_reg = gen_reg_rtx (SImode);
 pat = gen_rtx_UNSPEC (SImode, gen_rtvec (1, x), UNSPEC_TLSLE);
 pat = gen_rtx_CONST (SImode, pat);
 emit_insn (gen_sethi (tmp_reg, pat));
 emit_insn (gen_lo_sum (tmp_reg, tmp_reg, pat));
 x = gen_rtx_PLUS (Pmode, tmp_reg, tp_reg);
 break;
      default:
 gcc_unreachable ();
      }
  else if (GET_CODE (x) == CONST)
    {
      rtx base, addend;
      split_const (x, &base, &addend);
      if (SYMBOL_REF_TLS_MODEL (base) == TLS_MODEL_LOCAL_EXEC)
 {
   tmp_reg = gen_reg_rtx (SImode);
   pat = gen_rtx_UNSPEC (SImode, gen_rtvec (1, base), UNSPEC_TLSLE);
   pat = gen_rtx_PLUS (SImode, pat, addend);
   pat = gen_rtx_CONST (SImode, pat);
   emit_insn (gen_sethi (tmp_reg, pat));
   emit_insn (gen_lo_sum (tmp_reg, tmp_reg, pat));
   x = gen_rtx_PLUS (Pmode, tmp_reg, tp_reg);
 }
    }
  return x;
}
void
nds32_expand_tls_move (rtx *operands)
{
  rtx src = operands[1];
  rtx base, addend;
  if (CONSTANT_P (src))
    split_const (src, &base, &addend);
  if (SYMBOL_REF_TLS_MODEL (base) == TLS_MODEL_LOCAL_EXEC)
    src = nds32_legitimize_tls_address (src);
  else
    {
      src = nds32_legitimize_tls_address (base);
      if (addend != const0_rtx)
 {
   src = gen_rtx_PLUS (SImode, src, addend);
   src = force_operand (src, operands[0]);
 }
    }
  emit_move_insn (operands[0], src);
}
void
nds32_expand_constant (enum machine_mode mode, HOST_WIDE_INT val,
         rtx target, rtx source)
{
  rtx temp = gen_reg_rtx (mode);
  int clear_sign_bit_copies = 0;
  int clear_zero_bit_copies = 0;
  unsigned HOST_WIDE_INT remainder = val & 0xffffffffUL;
  clear_sign_bit_copies = __builtin_clz (remainder);
  clear_zero_bit_copies = __builtin_ctz (remainder);
  HOST_WIDE_INT sign_shift_mask = ((0xffffffffUL
        << (32 - clear_sign_bit_copies))
       & 0xffffffffUL);
  HOST_WIDE_INT zero_shift_mask = (1 << clear_zero_bit_copies) - 1;
  if (clear_sign_bit_copies > 0 && clear_sign_bit_copies < 17
      && (remainder | sign_shift_mask) == 0xffffffffUL)
    {
      rtx shift = GEN_INT (clear_sign_bit_copies);
      emit_insn (gen_ashlsi3 (temp, source, shift));
      emit_insn (gen_lshrsi3 (target, temp, shift));
    }
  else if (clear_zero_bit_copies > 0 && clear_sign_bit_copies < 17
    && (remainder | zero_shift_mask) == 0xffffffffUL)
    {
      rtx shift = GEN_INT (clear_zero_bit_copies);
      emit_insn (gen_lshrsi3 (temp, source, shift));
      emit_insn (gen_ashlsi3 (target, temp, shift));
    }
  else
    {
      emit_move_insn (temp, GEN_INT (val));
      emit_move_insn (target, gen_rtx_fmt_ee (AND, mode, source, temp));
    }
}
bool
nds32_valid_smw_lwm_base_p (rtx op)
{
  rtx base_addr;
  if (!MEM_P (op))
    return false;
  base_addr = XEXP (op, 0);
  if (REG_P (base_addr))
    return true;
  else
    {
      if (GET_CODE (base_addr) == POST_INC
   && REG_P (XEXP (base_addr, 0)))
 return true;
    }
  return false;
}
rtx nds32_di_high_part_subreg(rtx reg)
{
  unsigned high_part_offset = subreg_highpart_offset (SImode, DImode);
  return simplify_gen_subreg (
    SImode, reg,
    DImode, high_part_offset);
}
rtx nds32_di_low_part_subreg(rtx reg)
{
  unsigned low_part_offset = subreg_lowpart_offset (SImode, DImode);
  return simplify_gen_subreg (
    SImode, reg,
    DImode, low_part_offset);
}
const char *
nds32_output_tls_desc (rtx *operands)
{
  char pattern[1000];
  if (TARGET_RELAX_HINT)
    snprintf (pattern, sizeof (pattern),
       ".relax_hint %%1\n\tsethi $r0, hi20(%%0)\n\t"
       ".relax_hint %%1\n\tori $r0, $r0, lo12(%%0)\n\t"
       ".relax_hint %%1\n\tlw $r15, [$r0 + $gp]\n\t"
       ".relax_hint %%1\n\tadd $r0, $r0, $gp\n\t"
       ".relax_hint %%1\n\tjral $r15");
  else
    snprintf (pattern, sizeof (pattern),
       "sethi $r0, hi20(%%0)\n\t"
       "ori $r0, $r0, lo12(%%0)\n\t"
       "lw $r15, [$r0 + $gp]\n\t"
       "add $r0, $r0, $gp\n\t"
       "jral $r15");
  output_asm_insn (pattern, operands);
  return "";
}
const char *
nds32_output_tls_ie (rtx *operands)
{
  char pattern[1000];
  if (flag_pic)
  {
      if (TARGET_RELAX_HINT)
 snprintf (pattern, sizeof (pattern),
    ".relax_hint %%2\n\tsethi %%0, hi20(%%1)\n\t"
    ".relax_hint %%2\n\tori %%0, %%0, lo12(%%1)\n\t"
    ".relax_hint %%2\n\tlw %%0, [%%0 + $gp]");
      else
 snprintf (pattern, sizeof (pattern),
    "sethi %%0, hi20(%%1)\n\t"
    "ori %%0, %%0, lo12(%%1)\n\t"
    "lw %%0, [%%0 + $gp]");
  }
  else
    {
      if (TARGET_RELAX_HINT)
 snprintf (pattern, sizeof (pattern),
    ".relax_hint %%2\n\tsethi %%0, hi20(%%1)\n\t"
    ".relax_hint %%2\n\tlwi %%0, [%%0 + lo12(%%1)]");
      else
 snprintf (pattern, sizeof (pattern),
    "sethi %%0, hi20(%%1)\n\t"
    "lwi %%0, [%%0 + lo12(%%1)]");
    }
  output_asm_insn (pattern, operands);
  return "";
}
