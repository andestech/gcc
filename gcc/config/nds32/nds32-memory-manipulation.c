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
static void
nds32_emit_load_store (rtx reg, rtx mem,
         enum machine_mode mode,
         int offset, bool load_p)
{
  rtx new_mem;
  new_mem = adjust_address (mem, mode, offset);
  if (load_p)
    emit_move_insn (reg, new_mem);
  else
    emit_move_insn (new_mem, reg);
}
static void
nds32_emit_post_inc_load_store (rtx reg, rtx base_reg,
    enum machine_mode mode,
    bool load_p)
{
  gcc_assert (GET_MODE (reg) == mode);
  gcc_assert (GET_MODE (base_reg) == Pmode);
  if (load_p)
    emit_move_insn (reg,
      gen_rtx_MEM (mode,
     base_reg));
  else
    emit_move_insn (gen_rtx_MEM (mode,
     base_reg),
      reg);
  emit_move_insn (base_reg,
    plus_constant(Pmode, base_reg, GET_MODE_SIZE (mode)));
}
static void
nds32_emit_mem_move (rtx src, rtx dst,
       enum machine_mode mode,
       int addr_offset)
{
  gcc_assert (MEM_P (src) && MEM_P (dst));
  rtx tmp_reg = gen_reg_rtx (mode);
  nds32_emit_load_store (tmp_reg, src, mode,
    addr_offset, true);
  nds32_emit_load_store (tmp_reg, dst, mode,
    addr_offset, false);
}
static void
nds32_emit_mem_move_block (int base_regno, int count,
      rtx *dst_base_reg, rtx *dst_mem,
      rtx *src_base_reg, rtx *src_mem,
      bool update_base_reg_p)
{
  rtx new_base_reg;
  emit_insn (nds32_expand_load_multiple (base_regno, count,
      *src_base_reg, *src_mem,
      update_base_reg_p, &new_base_reg));
  if (update_base_reg_p)
    {
      *src_base_reg = new_base_reg;
      *src_mem = gen_rtx_MEM (SImode, *src_base_reg);
    }
  emit_insn (nds32_expand_store_multiple (base_regno, count,
       *dst_base_reg, *dst_mem,
       update_base_reg_p, &new_base_reg));
  if (update_base_reg_p)
    {
      *dst_base_reg = new_base_reg;
      *dst_mem = gen_rtx_MEM (SImode, *dst_base_reg);
    }
}
static bool
nds32_expand_movmemsi_loop_unknown_size (rtx dstmem, rtx srcmem,
      rtx size,
      rtx alignment)
{
  rtx dst_base_reg, src_base_reg;
  rtx dst_itr, src_itr;
  rtx dstmem_m, srcmem_m, dst_itr_m, src_itr_m;
  rtx dst_end;
  rtx size_least_3_bit;
  rtx double_word_end;
  rtx double_word_mode_loop, byte_mode_entry, byte_mode_loop, end_label;
  rtx tmp;
  rtx mask_least_3_bit;
  int start_regno;
  bool align_to_4_bytes = (INTVAL (alignment) & 3) == 0;
  if (TARGET_ISA_V3M && !align_to_4_bytes)
    return 0;
  if (TARGET_REDUCED_REGS)
    start_regno = 2;
  else
    start_regno = 16;
  dst_itr = gen_reg_rtx (Pmode);
  src_itr = gen_reg_rtx (Pmode);
  dst_end = gen_reg_rtx (Pmode);
  tmp = gen_reg_rtx (QImode);
  mask_least_3_bit = GEN_INT (~7);
  double_word_mode_loop = gen_label_rtx ();
  byte_mode_entry = gen_label_rtx ();
  byte_mode_loop = gen_label_rtx ();
  end_label = gen_label_rtx ();
  dst_base_reg = copy_to_mode_reg (Pmode, XEXP (dstmem, 0));
  src_base_reg = copy_to_mode_reg (Pmode, XEXP (srcmem, 0));
  size_least_3_bit = expand_binop (SImode, and_optab, size, mask_least_3_bit,
       NULL_RTX, 0, OPTAB_WIDEN);
  dst_end = expand_binop (Pmode, add_optab, dst_base_reg, size,
     NULL_RTX, 0, OPTAB_WIDEN);
  emit_move_insn (dst_itr, dst_base_reg);
  emit_move_insn (src_itr, src_base_reg);
  emit_cmp_and_jump_insns (size_least_3_bit, const0_rtx, EQ, NULL,
      SImode, 1, byte_mode_entry);
  double_word_end = expand_binop (Pmode, add_optab,
      dst_base_reg, size_least_3_bit,
      NULL_RTX, 0, OPTAB_WIDEN);
  emit_label (double_word_mode_loop);
  src_itr_m = src_itr;
  dst_itr_m = dst_itr;
  srcmem_m = srcmem;
  dstmem_m = dstmem;
  nds32_emit_mem_move_block (start_regno, 2,
        &dst_itr_m, &dstmem_m,
        &src_itr_m, &srcmem_m,
        true);
  emit_move_insn (dst_itr, dst_itr_m);
  emit_move_insn (src_itr, src_itr_m);
  emit_cmp_and_jump_insns (double_word_end, dst_itr, NE, NULL,
      Pmode, 1, double_word_mode_loop);
  emit_label (byte_mode_entry);
  emit_cmp_and_jump_insns (dst_itr, dst_end, EQ, NULL,
      Pmode, 1, end_label);
  emit_label (byte_mode_loop);
  emit_insn (gen_no_hwloop ());
  nds32_emit_post_inc_load_store (tmp, src_itr, QImode, true);
  nds32_emit_post_inc_load_store (tmp, dst_itr, QImode, false);
  emit_cmp_and_jump_insns (dst_itr, dst_end, NE, NULL,
      SImode, 1, byte_mode_loop);
  emit_label (end_label);
  return true;
}
static bool
nds32_expand_movmemsi_loop_known_size (rtx dstmem, rtx srcmem,
           rtx size, rtx alignment)
{
  return nds32_expand_movmemsi_loop_unknown_size (dstmem, srcmem,
        size, alignment);
}
static bool
nds32_expand_movmemsi_loop (rtx dstmem, rtx srcmem,
       rtx size, rtx alignment)
{
  if (CONST_INT_P (size))
    return nds32_expand_movmemsi_loop_known_size (dstmem, srcmem,
        size, alignment);
  else
    return nds32_expand_movmemsi_loop_unknown_size (dstmem, srcmem,
          size, alignment);
}
static bool
nds32_expand_movmemsi_unroll (rtx dstmem, rtx srcmem,
         rtx total_bytes, rtx alignment)
{
  rtx dst_base_reg, src_base_reg;
  rtx tmp_reg;
  int maximum_bytes;
  int maximum_bytes_per_inst;
  int maximum_regs;
  int start_regno;
  int i, inst_num;
  HOST_WIDE_INT remain_bytes, remain_words;
  bool align_to_4_bytes = (INTVAL (alignment) & 3) == 0;
  bool align_to_2_bytes = (INTVAL (alignment) & 1) == 0;
  if (TARGET_REDUCED_REGS)
    {
      maximum_regs = 4;
      maximum_bytes = 64;
      start_regno = 2;
    }
  else
    {
      if (TARGET_LINUX_ABI)
 {
   maximum_regs = 8;
   maximum_bytes = 160;
   start_regno = 16;
 }
      else
 {
   maximum_regs = 10;
   maximum_bytes = 160;
   start_regno = 16;
 }
    }
  maximum_bytes_per_inst = maximum_regs * UNITS_PER_WORD;
  if (GET_CODE (total_bytes) != CONST_INT
      || GET_CODE (alignment) != CONST_INT
      || INTVAL (total_bytes) > maximum_bytes
      || MEM_VOLATILE_P (dstmem)
      || MEM_VOLATILE_P (srcmem)
      || (TARGET_ISA_V3M && !align_to_4_bytes))
    return false;
  dst_base_reg = copy_to_mode_reg (SImode, XEXP (dstmem, 0));
  src_base_reg = copy_to_mode_reg (SImode, XEXP (srcmem, 0));
  remain_bytes = INTVAL (total_bytes);
  inst_num = ((INTVAL (total_bytes) + (maximum_bytes_per_inst - 1))
       / maximum_bytes_per_inst) - 1;
  for (i = 0; i < inst_num; i++)
    {
      nds32_emit_mem_move_block (start_regno, maximum_regs,
     &dst_base_reg, &dstmem,
     &src_base_reg, &srcmem,
     true);
    }
  remain_bytes -= maximum_bytes_per_inst * inst_num;
  remain_words = remain_bytes / UNITS_PER_WORD;
  remain_bytes = remain_bytes - (remain_words * UNITS_PER_WORD);
  if (remain_words != 0)
    {
      if (remain_bytes != 0)
 nds32_emit_mem_move_block (start_regno, remain_words,
       &dst_base_reg, &dstmem,
       &src_base_reg, &srcmem,
       true);
      else
 {
   if (remain_words == 1)
    {
       if (align_to_4_bytes)
  nds32_emit_mem_move (srcmem, dstmem, SImode, 0);
       else
  {
    tmp_reg = gen_reg_rtx (SImode);
    emit_insn (
      gen_unaligned_load_w (tmp_reg,
       gen_rtx_MEM (SImode, src_base_reg)));
    emit_insn (
      gen_unaligned_store_w (gen_rtx_MEM (SImode, dst_base_reg),
        tmp_reg));
  }
     }
   else
     nds32_emit_mem_move_block (start_regno, remain_words,
           &dst_base_reg, &dstmem,
           &src_base_reg, &srcmem,
           false);
 }
    }
  switch (remain_bytes)
    {
    case 3:
    case 2:
      {
 if (align_to_2_bytes)
   nds32_emit_mem_move (srcmem, dstmem, HImode, 0);
 else
   {
     nds32_emit_mem_move (srcmem, dstmem, QImode, 0);
     nds32_emit_mem_move (srcmem, dstmem, QImode, 1);
   }
 if (remain_bytes == 3)
   nds32_emit_mem_move (srcmem, dstmem, QImode, 2);
 break;
      }
    case 1:
      nds32_emit_mem_move (srcmem, dstmem, QImode, 0);
      break;
    case 0:
      break;
    default:
      gcc_unreachable ();
    }
  return true;
}
bool
nds32_expand_movmemsi (rtx dstmem, rtx srcmem, rtx total_bytes, rtx alignment)
{
  if (nds32_expand_movmemsi_unroll (dstmem, srcmem, total_bytes, alignment))
    return true;
  if (!optimize_size && optimize > 2)
    return nds32_expand_movmemsi_loop (dstmem, srcmem, total_bytes, alignment);
  return false;
}
static rtx
nds32_gen_dup_4_byte_to_word_value (rtx value)
{
  rtx value4word = gen_reg_rtx (SImode);
  gcc_assert (GET_MODE (value) == QImode || CONST_INT_P (value));
  if (CONST_INT_P (value))
    {
      unsigned HOST_WIDE_INT val = UINTVAL (value) & GET_MODE_MASK(QImode);
      rtx new_val = gen_int_mode (val | (val << 8)
      | (val << 16) | (val << 24), SImode);
      emit_move_insn (value4word, new_val);
    }
  else
    {
      if (NDS32_EXT_DSP_P ())
 {
   rtx tmp;
   tmp = expand_binop (SImode, and_optab, value,
          gen_int_mode (0xff, SImode),
          NULL_RTX, 0, OPTAB_WIDEN);
   emit_insn (
     gen_insvsi_internal (tmp, gen_int_mode (0x8, SImode), tmp));
   emit_insn (gen_pkbbsi_1 (value4word, tmp, tmp));
 }
      else
 {
   rtx tmp1, tmp2, tmp3, tmp4, final_value;
   tmp1 = expand_binop (SImode, and_optab, value,
          gen_int_mode (0xff, SImode),
          NULL_RTX, 0, OPTAB_WIDEN);
   tmp2 = expand_binop (SImode, ashl_optab, tmp1,
          gen_int_mode (8, SImode),
          NULL_RTX, 0, OPTAB_WIDEN);
   tmp3 = expand_binop (SImode, ior_optab, tmp1, tmp2,
          NULL_RTX, 0, OPTAB_WIDEN);
   tmp4 = expand_binop (SImode, ashl_optab, tmp3,
          gen_int_mode (16, SImode),
          NULL_RTX, 0, OPTAB_WIDEN);
   final_value = expand_binop (SImode, ior_optab, tmp3, tmp4,
          NULL_RTX, 0, OPTAB_WIDEN);
   emit_move_insn (value4word, final_value);
 }
    }
  return value4word;
}
static rtx
emit_setmem_word_loop (rtx itr, rtx size, rtx value)
{
  rtx word_mode_label = gen_label_rtx ();
  rtx word_mode_end_label = gen_label_rtx ();
  rtx byte_mode_size = gen_reg_rtx (SImode);
  rtx byte_mode_size_tmp = gen_reg_rtx (SImode);
  rtx word_mode_end = gen_reg_rtx (SImode);
  rtx size_for_word = gen_reg_rtx (SImode);
  size_for_word = expand_binop (SImode, and_optab, size,
    gen_int_mode (~3, SImode),
    NULL_RTX, 0, OPTAB_WIDEN);
  emit_move_insn (byte_mode_size, size);
  emit_cmp_and_jump_insns (size_for_word, const0_rtx, EQ, NULL,
      SImode, 1, word_mode_end_label);
  word_mode_end = expand_binop (Pmode, add_optab, itr, size_for_word,
    NULL_RTX, 0, OPTAB_WIDEN);
  byte_mode_size_tmp = expand_binop (SImode, and_optab, size, GEN_INT (3),
         NULL_RTX, 0, OPTAB_WIDEN);
  emit_move_insn (byte_mode_size, byte_mode_size_tmp);
  emit_label (word_mode_label);
  emit_insn (gen_unaligned_store_update_base_w (itr,
      itr,
      value));
  emit_cmp_and_jump_insns (word_mode_end, itr, NE, NULL,
      Pmode, 1, word_mode_label);
  emit_label (word_mode_end_label);
  return byte_mode_size;
}
static rtx
emit_setmem_byte_loop (rtx itr, rtx size, rtx value, bool need_end)
{
  rtx end = gen_reg_rtx (Pmode);
  rtx byte_mode_label = gen_label_rtx ();
  rtx end_label = gen_label_rtx ();
  value = force_reg (QImode, value);
  if (need_end)
    end = expand_binop (Pmode, add_optab, itr, size,
   NULL_RTX, 0, OPTAB_WIDEN);
  emit_cmp_and_jump_insns (size, const0_rtx, EQ, NULL,
      SImode, 1, end_label);
  if (!need_end)
    end = expand_binop (Pmode, add_optab, itr, size,
   NULL_RTX, 0, OPTAB_WIDEN);
  emit_label (byte_mode_label);
  emit_insn (gen_no_hwloop ());
  nds32_emit_post_inc_load_store (value, itr, QImode, false);
  emit_cmp_and_jump_insns (end, itr, NE, NULL,
      Pmode, 1, byte_mode_label);
  emit_label (end_label);
  if (need_end)
    return end;
  else
    return NULL_RTX;
}
static bool
nds32_expand_setmem_loop (rtx dstmem, rtx size, rtx value)
{
  rtx value4word;
  rtx value4byte;
  rtx dst;
  rtx byte_mode_size;
  dst = copy_to_mode_reg (SImode, XEXP (dstmem, 0));
  value4word = nds32_gen_dup_4_byte_to_word_value (value);
  byte_mode_size = emit_setmem_word_loop (dst, size, value4word);
  value4byte = simplify_gen_subreg (QImode, value4word, SImode,
        subreg_lowpart_offset (QImode, SImode));
  emit_setmem_byte_loop (dst, byte_mode_size, value4byte, false);
  return true;
}
static bool
nds32_expand_setmem_loop_v3m (rtx dstmem, rtx size, rtx value)
{
  rtx base_reg = copy_to_mode_reg (Pmode, XEXP (dstmem, 0));
  rtx need_align_bytes = gen_reg_rtx (SImode);
  rtx last_2_bit = gen_reg_rtx (SImode);
  rtx byte_loop_base = gen_reg_rtx (SImode);
  rtx byte_loop_size = gen_reg_rtx (SImode);
  rtx remain_size = gen_reg_rtx (SImode);
  rtx new_base_reg;
  rtx value4byte, value4word;
  rtx byte_mode_size;
  rtx last_byte_loop_label = gen_label_rtx ();
  size = force_reg (SImode, size);
  value4word = nds32_gen_dup_4_byte_to_word_value (value);
  value4byte = simplify_gen_subreg (QImode, value4word, SImode,
        subreg_lowpart_offset (QImode, SImode));
  emit_move_insn (byte_loop_size, size);
  emit_move_insn (byte_loop_base, base_reg);
  emit_cmp_and_jump_insns (size, gen_int_mode (16, SImode), LE, NULL,
      SImode, 1, last_byte_loop_label);
  emit_insn (gen_andsi3 (last_2_bit,
    base_reg,
    gen_int_mode (0x3, SImode)));
  emit_insn (gen_subsi3 (need_align_bytes,
    gen_int_mode (4, SImode),
    last_2_bit));
  new_base_reg = emit_setmem_byte_loop (base_reg,
     need_align_bytes,
     value4byte,
     true);
  emit_insn (gen_subsi3 (remain_size, size, need_align_bytes));
  byte_mode_size = emit_setmem_word_loop (new_base_reg,
       remain_size,
       value4word);
  emit_move_insn (byte_loop_base, new_base_reg);
  emit_move_insn (byte_loop_size, byte_mode_size);
  emit_label (last_byte_loop_label);
  emit_setmem_byte_loop (byte_loop_base, byte_loop_size, value4byte, false);
  return true;
}
static bool
nds32_expand_setmem_unroll (rtx dstmem, rtx size, rtx value,
       rtx align ATTRIBUTE_UNUSED,
       rtx expected_align ATTRIBUTE_UNUSED,
       rtx expected_size ATTRIBUTE_UNUSED)
{
  unsigned maximum_regs, maximum_bytes, start_regno, regno;
  rtx value4word;
  rtx dst_base_reg, new_base_reg;
  unsigned HOST_WIDE_INT remain_bytes, remain_words, prepare_regs, fill_per_smw;
  unsigned HOST_WIDE_INT real_size;
  if (TARGET_REDUCED_REGS)
    {
      maximum_regs = 4;
      maximum_bytes = 64;
      start_regno = 2;
    }
  else
    {
      maximum_regs = 8;
      maximum_bytes = 128;
      start_regno = 16;
    }
  real_size = UINTVAL (size) & GET_MODE_MASK(SImode);
  if (!(CONST_INT_P (size) && real_size <= maximum_bytes))
    return false;
  remain_bytes = real_size;
  gcc_assert (GET_MODE (value) == QImode || CONST_INT_P (value));
  value4word = nds32_gen_dup_4_byte_to_word_value (value);
  prepare_regs = remain_bytes / UNITS_PER_WORD;
  dst_base_reg = copy_to_mode_reg (SImode, XEXP (dstmem, 0));
  if (prepare_regs > maximum_regs)
    prepare_regs = maximum_regs;
  fill_per_smw = prepare_regs * UNITS_PER_WORD;
  regno = start_regno;
  switch (prepare_regs)
    {
    case 2:
    default:
      {
 rtx reg0 = gen_rtx_REG (SImode, regno);
 rtx reg1 = gen_rtx_REG (SImode, regno+1);
 unsigned last_regno = start_regno + prepare_regs - 1;
 emit_move_insn (reg0, value4word);
 emit_move_insn (reg1, value4word);
 rtx regd = gen_rtx_REG (DImode, regno);
 regno += 2;
 while (regno <= last_regno)
   {
     if ((regno + 1) <=last_regno)
       {
  rtx reg = gen_rtx_REG (DImode, regno);
  emit_move_insn (reg, regd);
  regno += 2;
       }
     else
       {
  rtx reg = gen_rtx_REG (SImode, regno);
  emit_move_insn (reg, reg0);
  regno += 1;
       }
   }
 break;
      }
    case 1:
      {
 rtx reg = gen_rtx_REG (SImode, regno++);
 emit_move_insn (reg, value4word);
      }
      break;
    case 0:
      break;
    }
  if (fill_per_smw)
    for (;remain_bytes >= fill_per_smw;remain_bytes -= fill_per_smw)
      {
 emit_insn (nds32_expand_store_multiple (start_regno, prepare_regs,
      dst_base_reg, dstmem,
      true, &new_base_reg));
 dst_base_reg = new_base_reg;
 dstmem = gen_rtx_MEM (SImode, dst_base_reg);
      }
  remain_words = remain_bytes / UNITS_PER_WORD;
  if (remain_words)
    {
      emit_insn (nds32_expand_store_multiple (start_regno, remain_words,
           dst_base_reg, dstmem,
           true, &new_base_reg));
      dst_base_reg = new_base_reg;
      dstmem = gen_rtx_MEM (SImode, dst_base_reg);
    }
  remain_bytes = remain_bytes - (remain_words * UNITS_PER_WORD);
  if (remain_bytes)
    {
      value = simplify_gen_subreg (QImode, value4word, SImode,
       subreg_lowpart_offset(QImode, SImode));
      int offset = 0;
      for (;remain_bytes;--remain_bytes, ++offset)
 {
   nds32_emit_load_store (value, dstmem, QImode, offset, false);
 }
    }
  return true;
}
bool
nds32_expand_setmem (rtx dstmem, rtx size, rtx value, rtx align,
       rtx expected_align,
       rtx expected_size)
{
  bool align_to_4_bytes = (INTVAL (align) & 3) == 0;
  if (optimize_size || optimize < 3)
    return false;
  if (TARGET_ISA_V3M && !align_to_4_bytes)
    return nds32_expand_setmem_loop_v3m (dstmem, size, value);
  if (nds32_expand_setmem_unroll (dstmem, size, value,
      align, expected_align, expected_size))
    return true;
  return nds32_expand_setmem_loop (dstmem, size, value);
}
bool
nds32_expand_movstr (rtx dst_end_ptr,
       rtx dstmem,
       rtx srcmem)
{
  rtx tmp;
  rtx dst_base_reg, src_base_reg;
  rtx new_dst_base_reg, new_src_base_reg;
  rtx last_non_null_char_ptr;
  rtx ffbi_result;
  rtx loop_label;
  if (optimize_size || optimize < 3)
    return false;
  tmp = gen_reg_rtx (SImode);
  ffbi_result = gen_reg_rtx (Pmode);
  new_dst_base_reg = gen_reg_rtx (Pmode);
  new_src_base_reg = gen_reg_rtx (Pmode);
  dst_base_reg = copy_to_mode_reg (SImode, XEXP (dstmem, 0));
  src_base_reg = copy_to_mode_reg (SImode, XEXP (srcmem, 0));
  loop_label = gen_label_rtx ();
  emit_label (loop_label);
  emit_insn (gen_lmwzb (new_src_base_reg, src_base_reg, tmp));
  emit_insn (gen_smwzb (new_dst_base_reg, dst_base_reg, tmp));
  emit_insn (gen_unspec_ffb (ffbi_result, tmp, const0_rtx));
  emit_move_insn (src_base_reg, new_src_base_reg);
  emit_move_insn (dst_base_reg, new_dst_base_reg);
  emit_cmp_and_jump_insns (ffbi_result, const0_rtx, EQ, NULL,
      SImode, 1, loop_label);
  last_non_null_char_ptr = expand_binop (Pmode, add_optab, dst_base_reg,
      ffbi_result, NULL_RTX, 0, OPTAB_WIDEN);
  emit_move_insn (dst_end_ptr, last_non_null_char_ptr);
  return true;
}
bool
nds32_expand_strlen (rtx result, rtx str,
       rtx target_char, rtx align ATTRIBUTE_UNUSED)
{
  rtx base_reg, backup_base_reg;
  rtx ffb_result;
  rtx target_char_ptr, length;
  rtx loop_label, tmp;
  if (optimize_size || optimize < 3)
    return false;
  gcc_assert (MEM_P (str));
  gcc_assert (CONST_INT_P (target_char) || REG_P (target_char));
  base_reg = copy_to_mode_reg (SImode, XEXP (str, 0));
  loop_label = gen_label_rtx ();
  ffb_result = gen_reg_rtx (Pmode);
  tmp = gen_reg_rtx (SImode);
  backup_base_reg = gen_reg_rtx (SImode);
  emit_move_insn (backup_base_reg, base_reg);
  emit_label (loop_label);
  emit_insn (gen_unaligned_load_update_base_w (base_reg, tmp, base_reg));
  emit_insn (gen_unspec_ffb (ffb_result, tmp, target_char));
  emit_cmp_and_jump_insns (ffb_result, const0_rtx, EQ, NULL,
      SImode, 1, loop_label);
  target_char_ptr = expand_binop (Pmode, add_optab, base_reg,
    ffb_result, NULL_RTX, 0, OPTAB_WIDEN);
  length = expand_binop (Pmode, sub_optab, target_char_ptr,
    backup_base_reg, NULL_RTX, 0, OPTAB_WIDEN);
  emit_move_insn (result, length);
  return true;
}
rtx
nds32_expand_load_multiple (int base_regno, int count,
       rtx base_addr, rtx basemem,
       bool update_base_reg_p,
       rtx *update_base_reg)
{
  int par_index;
  int offset;
  int start_idx;
  rtx result;
  rtx new_addr, mem, reg;
  if (count == 1)
    {
      reg = gen_rtx_REG (SImode, base_regno);
      if (update_base_reg_p)
 {
   *update_base_reg = gen_reg_rtx (SImode);
   return gen_unaligned_load_update_base_w (*update_base_reg, reg, base_addr);
 }
      else
 return gen_unaligned_load_w (reg, gen_rtx_MEM (SImode, base_addr));
    }
  if (update_base_reg_p)
    {
      result = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count + 1));
      start_idx = 1;
    }
  else
    {
      result = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count));
      start_idx = 0;
    }
  if (update_base_reg_p)
    {
      offset = count * 4;
      new_addr = plus_constant (Pmode, base_addr, offset);
      *update_base_reg = gen_reg_rtx (SImode);
      XVECEXP (result, 0, 0) = gen_rtx_SET (VOIDmode,
         *update_base_reg, new_addr);
    }
  for (par_index = 0; par_index < count; par_index++)
    {
      offset = par_index * 4;
      new_addr = plus_constant (Pmode, base_addr, offset);
      mem = adjust_automodify_address_nv (basemem, SImode,
            new_addr, offset);
      reg = gen_rtx_REG (SImode, base_regno + par_index);
      XVECEXP (result, 0, (par_index + start_idx)) = gen_rtx_SET (VOIDmode, reg, mem);
    }
  return result;
}
rtx
nds32_expand_store_multiple (int base_regno, int count,
        rtx base_addr, rtx basemem,
        bool update_base_reg_p,
        rtx *update_base_reg)
{
  int par_index;
  int offset;
  int start_idx;
  rtx result;
  rtx new_addr, mem, reg;
  if (count == 1)
    {
      reg = gen_rtx_REG (SImode, base_regno);
      if (update_base_reg_p)
 {
   *update_base_reg = gen_reg_rtx (SImode);
   return gen_unaligned_store_update_base_w (*update_base_reg, base_addr, reg);
 }
      else
 return gen_unaligned_store_w (gen_rtx_MEM (SImode, base_addr), reg);
    }
  if (update_base_reg_p)
    {
      result = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count + 1));
      start_idx = 1;
    }
  else
    {
      result = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count));
      start_idx = 0;
    }
  if (update_base_reg_p)
    {
      offset = count * 4;
      new_addr = plus_constant (Pmode, base_addr, offset);
      *update_base_reg = gen_reg_rtx (SImode);
      XVECEXP (result, 0, 0) = gen_rtx_SET (VOIDmode,
         *update_base_reg, new_addr);
    }
  for (par_index = 0; par_index < count; par_index++)
    {
      offset = par_index * 4;
      new_addr = plus_constant (Pmode, base_addr, offset);
      mem = adjust_automodify_address_nv (basemem, SImode,
            new_addr, offset);
      reg = gen_rtx_REG (SImode, base_regno + par_index);
      XVECEXP (result, 0, par_index + start_idx) = gen_rtx_SET (VOIDmode, mem, reg);
    }
  return result;
}
