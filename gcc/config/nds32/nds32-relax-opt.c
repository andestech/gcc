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
static int relax_group_id = 0;
static bool
nds32_reg_base_load_store_p (rtx insn)
{
  rtx mem_src = NULL_RTX;
  switch (get_attr_type (insn))
    {
    case TYPE_LOAD:
      mem_src = SET_SRC (PATTERN (insn));
      break;
    case TYPE_STORE:
      mem_src = SET_DEST (PATTERN (insn));
      break;
    default:
      break;
    }
  if (mem_src != NULL_RTX)
    {
      if ((GET_CODE (mem_src) == ZERO_EXTEND)
   || (GET_CODE (mem_src) == SIGN_EXTEND))
 mem_src = XEXP (mem_src, 0);
      if (GET_CODE (XEXP (mem_src, 0)) == REG)
 return true;
    }
  return false;
}
static bool
nds32_sp_base_or_plus_load_store_p (rtx insn)
{
  rtx mem_src = NULL_RTX;
  switch (get_attr_type (insn))
    {
    case TYPE_LOAD:
      mem_src = SET_SRC (PATTERN (insn));
      break;
    case TYPE_STORE:
      mem_src = SET_DEST (PATTERN (insn));
      break;
    default:
      break;
    }
  if (mem_src != NULL_RTX)
    {
      if ((GET_CODE (mem_src) == ZERO_EXTEND)
   || (GET_CODE (mem_src) == SIGN_EXTEND))
 mem_src = XEXP (mem_src, 0);
      if ((GET_CODE (XEXP (mem_src, 0)) == PLUS))
 mem_src = XEXP (mem_src, 0);
      if (REG_P (XEXP (mem_src, 0))
   && ((frame_pointer_needed
        && REGNO (XEXP (mem_src, 0)) == FP_REGNUM)
       || REGNO (XEXP (mem_src, 0)) == SP_REGNUM))
 return true;
    }
  return false;
}
static bool
nds32_plus_reg_load_store_p (rtx insn)
{
  rtx mem_src = NULL_RTX;
  switch (get_attr_type (insn))
    {
    case TYPE_LOAD:
      mem_src = SET_SRC (PATTERN (insn));
      break;
    case TYPE_STORE:
      mem_src = SET_DEST (PATTERN (insn));
      break;
    default:
      break;
    }
  if (mem_src != NULL_RTX)
    {
      if ((GET_CODE (mem_src) == ZERO_EXTEND)
   || (GET_CODE (mem_src) == SIGN_EXTEND))
 mem_src = XEXP (mem_src, 0);
      if ((GET_CODE (XEXP (mem_src, 0)) == PLUS))
 mem_src = XEXP (mem_src, 0);
      else
 return false;
      if (GET_CODE (XEXP (mem_src, 0)) == REG)
 return true;
    }
  return false;
}
static bool
nds32_ict_const_p (rtx x)
{
  if (GET_CODE (x) == CONST)
    {
      x = XEXP (x, 0);
      return nds32_indirect_call_referenced_p (x);
    }
  return FALSE;
}
static auto_vec<rtx, 32> nds32_group_infos;
static bool
nds32_pic_tls_group (rtx def_insn,
       enum nds32_relax_insn_type relax_type,
       int sym_type)
{
  df_ref *def_record;
  df_link *link;
  rtx use_insn = NULL_RTX, pat, new_pat;
  def_record = DF_INSN_DEFS (def_insn);
  for (link = DF_REF_CHAIN (*def_record); link; link = link->next)
    {
      if (!DF_REF_INSN_INFO (link->ref))
 continue;
      use_insn = DF_REF_INSN (link->ref);
      if (!dominated_by_p (CDI_DOMINATORS,
      BLOCK_FOR_INSN (use_insn),
      BLOCK_FOR_INSN (def_insn)))
 return FALSE;
      if (!active_insn_p (use_insn))
 return FALSE;
      switch (relax_type)
 {
 case RELAX_ORI:
   if ((sym_type == UNSPEC_TLSLE
        || sym_type == UNSPEC_GOTOFF)
       && (recog_memoized (use_insn) == CODE_FOR_addsi3))
     {
       pat = XEXP (PATTERN (use_insn), 1);
       new_pat =
  gen_rtx_UNSPEC (SImode,
    gen_rtvec (2, XEXP (pat, 0), XEXP (pat, 1)),
    UNSPEC_ADD32);
       validate_replace_rtx (pat, new_pat, use_insn);
       nds32_group_infos.safe_push (use_insn);
     }
   else if (nds32_plus_reg_load_store_p (use_insn)
     && !nds32_sp_base_or_plus_load_store_p (use_insn))
     nds32_group_infos.safe_push (use_insn);
   else
     return FALSE;
   break;
 default:
   return FALSE;
 }
    }
  return TRUE;
}
static int
nds32_pic_tls_symbol_type (rtx x)
{
  x = XEXP (SET_SRC (PATTERN (x)), 1);
  if (GET_CODE (x) == CONST)
    {
      x = XEXP (x, 0);
      if (GET_CODE (x) == PLUS)
 x = XEXP (x, 0);
      return XINT (x, 1);
    }
  return XINT (x, 1);
}
static void
nds32_group_insns (rtx sethi)
{
  df_ref *def_record;
  df_link *link;
  rtx use_insn = NULL_RTX, group_id;
  bool valid;
  def_record = DF_INSN_DEFS (sethi);
  for (link = DF_REF_CHAIN (*def_record); link; link = link->next)
    {
      if (!DF_REF_INSN_INFO (link->ref))
 continue;
      use_insn = DF_REF_INSN (link->ref);
      if (!dominated_by_p (CDI_DOMINATORS,
      BLOCK_FOR_INSN (use_insn),
      BLOCK_FOR_INSN (sethi)))
 return;
      if (!active_insn_p (use_insn))
 return;
      if (!(recog_memoized (use_insn) == CODE_FOR_lo_sum
     || nds32_symbol_load_store_p (use_insn)
     || (nds32_reg_base_load_store_p (use_insn)
  &&!nds32_sp_base_or_plus_load_store_p (use_insn))))
 return;
    }
  group_id = GEN_INT (relax_group_id);
  emit_insn_before (gen_relax_group (group_id), sethi);
  for (link = DF_REF_CHAIN (*def_record); link; link = link->next)
    {
      if (!DF_REF_INSN_INFO (link->ref))
 continue;
      use_insn = DF_REF_INSN (link->ref);
      if (active_insn_p (use_insn))
 emit_insn_before (gen_relax_group (group_id), use_insn);
      if (use_insn != NULL_RTX
   && recog_memoized (use_insn) == CODE_FOR_lo_sum
   && !nds32_const_unspec_p (XEXP (SET_SRC (PATTERN (use_insn)), 1)))
 {
   int sym_type = nds32_pic_tls_symbol_type (use_insn);
   valid = nds32_pic_tls_group (use_insn, RELAX_ORI, sym_type);
   while (!nds32_group_infos.is_empty ())
     {
       use_insn = nds32_group_infos.pop ();
       if (valid)
  emit_insn_before (gen_relax_group (group_id), use_insn);
     }
 }
    }
  relax_group_id++;
}
static void
nds32_group_tls_insn (rtx insn)
{
  rtx pat = PATTERN (insn);
  rtx unspec_relax_group = XEXP (XVECEXP (pat, 0, 1), 0);
  while (GET_CODE (pat) != SET && GET_CODE (pat) == PARALLEL)
    {
      pat = XVECEXP (pat, 0, 0);
    }
  if (GET_CODE (unspec_relax_group) == UNSPEC
      && XINT (unspec_relax_group, 1) == UNSPEC_VOLATILE_RELAX_GROUP)
    {
      XVECEXP (unspec_relax_group, 0, 0) = GEN_INT (relax_group_id);
    }
  relax_group_id++;
}
static void
nds32_relax_group (void)
{
  rtx insn;
  compute_bb_for_insn ();
  df_chain_add_problem (DF_DU_CHAIN);
  df_insn_rescan_all ();
  df_analyze ();
  calculate_dominance_info (CDI_DOMINATORS);
  insn = get_insns ();
  gcc_assert (NOTE_P (insn));
  for (insn = next_active_insn (insn); insn; insn = next_active_insn (insn))
    {
      if (NONJUMP_INSN_P (insn))
 {
   if (recog_memoized (insn) == CODE_FOR_sethi
       && nds32_symbolic_operand (XEXP (SET_SRC (PATTERN (insn)), 0),
      SImode)
       && !nds32_ict_const_p (XEXP (SET_SRC (PATTERN (insn)), 0)))
     nds32_group_insns (insn);
   else if (recog_memoized (insn) == CODE_FOR_tls_ie)
     nds32_group_tls_insn (insn);
 }
      else if (CALL_P (insn) && recog_memoized (insn) == CODE_FOR_tls_desc)
 {
   nds32_group_tls_insn (insn);
 }
    }
  df_insn_rescan_all ();
  df_finish_pass (false);
  free_dominance_info (CDI_DOMINATORS);
}
static unsigned int
nds32_relax_opt (void)
{
  if (TARGET_RELAX_HINT)
    nds32_relax_group ();
  return 1;
}
const pass_data pass_data_nds32_relax_opt =
{
  RTL_PASS,
  "relax_opt",
  OPTGROUP_NONE,
  true,
  true,
  TV_MACH_DEP,
  0,
  0,
  0,
  0,
  ( TODO_df_finish | TODO_verify_rtl_sharing),
};
class pass_nds32_relax_opt : public rtl_opt_pass
{
public:
  pass_nds32_relax_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_relax_opt, ctxt)
  {}
  bool gate () { return TARGET_RELAX_HINT; }
  unsigned int execute () { return nds32_relax_opt (); }
};
rtl_opt_pass *
make_pass_nds32_relax_opt (gcc::context *ctxt)
{
  return new pass_nds32_relax_opt (ctxt);
}
