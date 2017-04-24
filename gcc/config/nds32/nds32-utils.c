#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tm_p.h"
#include "rtl.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "basic-block.h"
namespace nds32 {
rtx
extract_pattern_from_insn (rtx insn)
{
  if (INSN_P (insn))
    return PATTERN (insn);
  return insn;
}
size_t
parallel_elements (rtx parallel_rtx)
{
  parallel_rtx = extract_pattern_from_insn (parallel_rtx);
  gcc_assert (GET_CODE (parallel_rtx) == PARALLEL);
  return XVECLEN (parallel_rtx, 0);
}
rtx
parallel_element (rtx parallel_rtx, int nth)
{
  parallel_rtx = extract_pattern_from_insn (parallel_rtx);
  gcc_assert (GET_CODE (parallel_rtx) == PARALLEL);
  int len = parallel_elements (parallel_rtx);
  if (nth >= 0)
    {
      if (nth >= len)
 return NULL_RTX;
      return XVECEXP (parallel_rtx, 0, nth);
    }
  else
    {
      if (len + nth < 0)
 return NULL_RTX;
      return XVECEXP (parallel_rtx, 0, len + nth);
    }
}
bool
insn_pseudo_nop_p (rtx insn)
{
  if (INSN_CODE (insn) == CODE_FOR_nop_data_dep
      || INSN_CODE (insn) == CODE_FOR_nop_res_dep)
    return true;
  return false;
}
bool
insn_executable_p (rtx insn)
{
  if (!INSN_P (insn))
    return false;
  if (insn_pseudo_nop_p (insn))
    return true;
  if (get_attr_length (insn) == 0)
    return false;
  switch (GET_CODE (PATTERN (insn)))
    {
    case CONST_INT:
    case USE:
    case CLOBBER:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
    case UNSPEC:
    case UNSPEC_VOLATILE:
      return false;
    default:
      return true;
    }
  return true;
}
bool
insn_deleted_p (rtx insn)
{
  if (INSN_DELETED_P (insn))
    return true;
  if (NOTE_P (insn) && NOTE_KIND (insn) == NOTE_INSN_DELETED)
    return true;
  return false;
}
bool
load_single_p (rtx insn)
{
  if (get_attr_type (insn) != TYPE_LOAD)
    return false;
  if (INSN_CODE (insn) == CODE_FOR_move_di ||
      INSN_CODE (insn) == CODE_FOR_move_df)
    return false;
  return true;
}
bool
store_single_p (rtx insn)
{
  if (get_attr_type (insn) != TYPE_STORE)
    return false;
  if (INSN_CODE (insn) == CODE_FOR_move_di ||
      INSN_CODE (insn) == CODE_FOR_move_df)
    return false;
  return true;
}
bool
load_double_p (rtx insn)
{
  if (get_attr_type (insn) != TYPE_LOAD)
    return false;
  if (INSN_CODE (insn) != CODE_FOR_move_di &&
      INSN_CODE (insn) != CODE_FOR_move_df)
    return false;
  return true;
}
bool
store_double_p (rtx insn)
{
  if (get_attr_type (insn) != TYPE_STORE)
    return false;
  if (INSN_CODE (insn) != CODE_FOR_move_di &&
      INSN_CODE (insn) != CODE_FOR_move_df)
    return false;
  return true;
}
bool
load_full_word_p (rtx insn)
{
  if (!nds32::load_single_p (insn))
    return false;
  if (GET_MODE (SET_SRC (PATTERN (insn))) == SImode)
    return true;
  return false;
}
bool
load_partial_word_p (rtx insn)
{
  if (!nds32::load_single_p (insn))
    return false;
  if (GET_MODE (SET_SRC (PATTERN (insn))) == HImode
      || GET_MODE (SET_SRC (PATTERN (insn))) == QImode)
    return true;
  return false;
}
bool
post_update_insn_p (rtx insn)
{
  if (find_post_update_rtx (insn) == -1)
    return false;
  else
    return true;
}
bool
immed_offset_p (rtx mem_rtx)
{
  gcc_assert (MEM_P (mem_rtx));
  rtx addr_rtx = XEXP (mem_rtx, 0);
  if (REG_P (addr_rtx))
    return true;
  if (GET_CODE (addr_rtx) == PLUS
      && GET_CODE (XEXP (addr_rtx, 1)) == CONST_INT)
    return true;
  return false;
}
int
find_post_update_rtx (rtx insn)
{
  rtx mem_rtx;
  int i, len;
  switch (get_attr_type (insn))
    {
    case TYPE_LOAD_MULTIPLE:
    case TYPE_STORE_MULTIPLE:
      len = parallel_elements (insn);
      for (i = 0; i < len; ++i)
 {
   rtx curr_insn = parallel_element (insn, i);
   if (GET_CODE (curr_insn) == SET
       && REG_P (SET_DEST (curr_insn))
       && GET_CODE (SET_SRC (curr_insn)) == PLUS)
  return i;
 }
      return -1;
    case TYPE_LOAD:
    case TYPE_FLOAD:
    case TYPE_STORE:
    case TYPE_FSTORE:
      mem_rtx = extract_mem_rtx (insn);
      switch (GET_CODE (XEXP (mem_rtx, 0)))
 {
 case POST_INC:
 case POST_DEC:
 case POST_MODIFY:
   return 0;
 default:
   return -1;
 }
    default:
      gcc_unreachable ();
    }
}
rtx
extract_mem_rtx (rtx insn)
{
  rtx body = PATTERN (insn);
  switch (get_attr_type (insn))
    {
    case TYPE_LOAD:
    case TYPE_FLOAD:
      if (MEM_P (SET_SRC (body)))
 return SET_SRC (body);
      if (GET_CODE (SET_SRC (body)) == UNSPEC)
 {
   gcc_assert (MEM_P (XVECEXP (SET_SRC (body), 0, 0)));
   return XVECEXP (SET_SRC (body), 0, 0);
 }
      gcc_assert (MEM_P (XEXP (SET_SRC (body), 0)));
      return XEXP (SET_SRC (body), 0);
    case TYPE_STORE:
    case TYPE_FSTORE:
      if (MEM_P (SET_DEST (body)))
 return SET_DEST (body);
      if (GET_CODE (SET_DEST (body)) == UNSPEC)
 {
   gcc_assert (MEM_P (XVECEXP (SET_DEST (body), 0, 0)));
   return XVECEXP (SET_DEST (body), 0, 0);
 }
      gcc_assert (MEM_P (XEXP (SET_DEST (body), 0)));
      return XEXP (SET_DEST (body), 0);
    default:
      gcc_unreachable ();
    }
}
rtx
extract_base_reg (rtx insn)
{
  int post_update_rtx_index;
  rtx mem_rtx;
  rtx plus_rtx;
  switch (get_attr_type (insn))
    {
    case TYPE_LOAD_MULTIPLE:
      post_update_rtx_index = find_post_update_rtx (insn);
      if (post_update_rtx_index != -1)
        return SET_DEST (parallel_element (insn, post_update_rtx_index));
      mem_rtx = SET_SRC (parallel_element (insn, 0));
      break;
    case TYPE_STORE_MULTIPLE:
      post_update_rtx_index = find_post_update_rtx (insn);
      if (post_update_rtx_index != -1)
        return SET_DEST (parallel_element (insn, post_update_rtx_index));
      mem_rtx = SET_DEST (parallel_element (insn, 0));
      break;
    case TYPE_LOAD:
    case TYPE_FLOAD:
    case TYPE_STORE:
    case TYPE_FSTORE:
      mem_rtx = extract_mem_rtx (insn);
      break;
    default:
      gcc_unreachable ();
    }
  gcc_assert (MEM_P (mem_rtx));
  if (REG_P (XEXP (mem_rtx, 0)))
    return XEXP (mem_rtx, 0);
  plus_rtx = XEXP (mem_rtx, 0);
  if (GET_CODE (plus_rtx) == SYMBOL_REF
      || GET_CODE (plus_rtx) == CONST)
    return NULL_RTX;
  gcc_assert (GET_CODE (plus_rtx) == PLUS
       || GET_CODE (plus_rtx) == POST_INC
       || GET_CODE (plus_rtx) == POST_DEC
       || GET_CODE (plus_rtx) == POST_MODIFY);
  gcc_assert (REG_P (XEXP (plus_rtx, 0)));
  return XEXP (plus_rtx, 0);
}
rtx
extract_shift_reg (rtx alu_shift_rtx)
{
  alu_shift_rtx = extract_pattern_from_insn (alu_shift_rtx);
  rtx alu_rtx = SET_SRC (alu_shift_rtx);
  rtx shift_rtx;
  if (REG_P (XEXP (alu_rtx, 0)))
    shift_rtx = XEXP (alu_rtx, 1);
  else
    shift_rtx = XEXP (alu_rtx, 0);
  return XEXP (shift_rtx, 0);
}
bool
movd44_insn_p (rtx insn)
{
  if (get_attr_type (insn) == TYPE_ALU
      && (INSN_CODE (insn) == CODE_FOR_move_di
   || INSN_CODE (insn) == CODE_FOR_move_df))
    {
      rtx body = PATTERN (insn);
      gcc_assert (GET_CODE (body) == SET);
      rtx src = SET_SRC (body);
      rtx dest = SET_DEST (body);
      if ((REG_P (src) || GET_CODE (src) == SUBREG)
   && (REG_P (dest) || GET_CODE (dest) == SUBREG))
 return true;
      return false;
    }
  return false;
}
rtx
extract_movd44_even_reg (rtx insn)
{
  gcc_assert (movd44_insn_p (insn));
  rtx def_reg = SET_DEST (PATTERN (insn));
  enum machine_mode mode;
  gcc_assert (REG_P (def_reg) || GET_CODE (def_reg) == SUBREG);
  switch (GET_MODE (def_reg))
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
  return gen_lowpart (mode, def_reg);
}
rtx
extract_movd44_odd_reg (rtx insn)
{
  gcc_assert (movd44_insn_p (insn));
  rtx def_reg = SET_DEST (PATTERN (insn));
  enum machine_mode mode;
  gcc_assert (REG_P (def_reg) || GET_CODE (def_reg) == SUBREG);
  switch (GET_MODE (def_reg))
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
  return gen_highpart (mode, def_reg);
}
rtx
extract_mac_acc_rtx (rtx insn)
{
  return SET_DEST (PATTERN (insn));
}
rtx
extract_mac_non_acc_rtx (rtx insn)
{
  rtx exp = SET_SRC (PATTERN (insn));
  switch (get_attr_type (insn))
    {
    case TYPE_MAC:
    case TYPE_DMAC:
      if (REG_P (XEXP (exp, 0)))
 return XEXP (exp, 1);
      else
 return XEXP (exp, 0);
    default:
      gcc_unreachable ();
    }
}
rtx
extract_branch_target_rtx (rtx insn)
{
  gcc_assert (CALL_P (insn) || JUMP_P (insn));
  rtx body = PATTERN (insn);
  if (GET_CODE (body) == SET)
    {
      if (GET_CODE (SET_SRC (body)) == IF_THEN_ELSE)
        return NULL_RTX;
      return SET_SRC (body);
    }
  if (GET_CODE (body) == CALL)
    return XEXP (body, 0);
  if (GET_CODE (body) == PARALLEL)
    {
      rtx first_rtx = parallel_element (body, 0);
      if (GET_CODE (first_rtx) == SET)
 return SET_SRC (first_rtx);
      if (GET_CODE (first_rtx) == CALL)
 return XEXP (first_rtx, 0);
    }
  if (GET_CODE (body) == COND_EXEC)
    {
      rtx addr_rtx = XEXP (body, 1);
      if (GET_CODE (addr_rtx) == SET)
 return SET_SRC (addr_rtx);
      if (GET_CODE (addr_rtx) == PARALLEL)
 {
   rtx first_rtx = parallel_element (addr_rtx, 0);
   if (GET_CODE (first_rtx) == SET)
     {
       rtx call_rtx = SET_SRC (first_rtx);
       gcc_assert (GET_CODE (call_rtx) == CALL);
       return XEXP (call_rtx, 0);
     }
   if (GET_CODE (first_rtx) == CALL)
     return XEXP (first_rtx, 0);
 }
    }
  gcc_unreachable ();
}
rtx
extract_branch_condition_rtx (rtx insn)
{
  gcc_assert (CALL_P (insn) || JUMP_P (insn));
  rtx body = PATTERN (insn);
  if (GET_CODE (body) == SET)
    {
      rtx if_then_else_rtx = SET_SRC (body);
      if (GET_CODE (if_then_else_rtx) == IF_THEN_ELSE)
        return XEXP (if_then_else_rtx, 0);
      return NULL_RTX;
    }
  if (GET_CODE (body) == COND_EXEC)
    return XEXP (body, 0);
  return NULL_RTX;
}
void
compute_bb_for_insn_safe ()
{
  basic_block bb;
  FOR_EACH_BB_FN (bb, cfun)
    {
      rtx insn, next_insn, last_insn;
      bool after_last_insn = false;
      for (last_insn = BB_END (bb);
    PREV_INSN (last_insn) && insn_deleted_p (last_insn);
    last_insn = PREV_INSN (last_insn));
      for (insn = BB_HEAD (bb); insn; insn = NEXT_INSN (insn))
 {
   BLOCK_FOR_INSN (insn) = bb;
   if (insn == last_insn)
     after_last_insn = true;
   next_insn = NEXT_INSN (insn);
   if (after_last_insn
       && (!next_insn
    || LABEL_P (next_insn)
    || NOTE_INSN_BASIC_BLOCK_P (next_insn)))
     {
       BB_END (bb) = insn;
       break;
     }
 }
    }
}
}
