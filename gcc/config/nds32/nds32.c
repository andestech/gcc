#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "stringpool.h"
#include "stor-layout.h"
#include "varasm.h"
#include "calls.h"
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
#include "basic-block.h"
#include "cfgloop.h"
#include "context.h"
#include "params.h"
#include "cpplib.h"
#include "hw-doloop.h"
namespace nds32 {
namespace scheduling {
rtl_opt_pass *make_pass_nds32_print_stalls (gcc::context *);
}
}
rtl_opt_pass *make_pass_nds32_fp_as_gp (gcc::context *);
rtl_opt_pass *make_pass_nds32_load_store_opt (gcc::context *);
rtl_opt_pass *make_pass_nds32_soft_fp_arith_comm_opt(gcc::context *);
rtl_opt_pass *make_pass_nds32_regrename_opt (gcc::context *);
rtl_opt_pass *make_pass_nds32_gcse_opt (gcc::context *);
rtl_opt_pass *make_pass_nds32_relax_opt (gcc::context *);
rtl_opt_pass *make_pass_nds32_hwloop1_opt (gcc::context *);
rtl_opt_pass *make_pass_nds32_hwloop2_opt (gcc::context *);
static const char * const nds32_intrinsic_register_names[] =
{
  "$CPU_VER",
  "$ICM_CFG",
  "$DCM_CFG",
  "$MMU_CFG",
  "$MSC_CFG",
  "$MSC_CFG2",
  "$CORE_ID",
  "$FUCOP_EXIST",
  "$PSW",
  "$IPSW",
  "$P_IPSW",
  "$IVB",
  "$EVA",
  "$P_EVA",
  "$ITYPE",
  "$P_ITYPE",
  "$MERR",
  "$IPC",
  "$P_IPC",
  "$OIPC",
  "$P_P0",
  "$P_P1",
  "$INT_MASK",
  "$INT_MASK2",
  "$INT_MASK3",
  "$INT_PEND",
  "$INT_PEND2",
  "$INT_PEND3",
  "$SP_USR",
  "$SP_PRIV",
  "$INT_PRI",
  "$INT_PRI2",
  "$INT_PRI3",
  "$INT_PRI4",
  "$INT_CTRL",
  "$INT_TRIGGER",
  "$INT_TRIGGER2",
  "$INT_GPR_PUSH_DIS",
  "$MMU_CTL",
  "$L1_PPTB",
  "$TLB_VPN",
  "$TLB_DATA",
  "$TLB_MISC",
  "$VLPT_IDX",
  "$ILMB",
  "$DLMB",
  "$CACHE_CTL",
  "$HSMP_SADDR",
  "$HSMP_EADDR",
  "$SDZ_CTL",
  "$N12MISC_CTL",
  "$MISC_CTL",
  "$ECC_MISC",
  "$BPC0",
  "$BPC1",
  "$BPC2",
  "$BPC3",
  "$BPC4",
  "$BPC5",
  "$BPC6",
  "$BPC7",
  "$BPA0",
  "$BPA1",
  "$BPA2",
  "$BPA3",
  "$BPA4",
  "$BPA5",
  "$BPA6",
  "$BPA7",
  "$BPAM0",
  "$BPAM1",
  "$BPAM2",
  "$BPAM3",
  "$BPAM4",
  "$BPAM5",
  "$BPAM6",
  "$BPAM7",
  "$BPV0",
  "$BPV1",
  "$BPV2",
  "$BPV3",
  "$BPV4",
  "$BPV5",
  "$BPV6",
  "$BPV7",
  "$BPCID0",
  "$BPCID1",
  "$BPCID2",
  "$BPCID3",
  "$BPCID4",
  "$BPCID5",
  "$BPCID6",
  "$BPCID7",
  "$EDM_CFG",
  "$EDMSW",
  "$EDM_CTL",
  "$EDM_DTR",
  "$BPMTC",
  "$DIMBR",
  "$TECR0",
  "$TECR1",
  "$PFMC0",
  "$PFMC1",
  "$PFMC2",
  "$PFM_CTL",
  "$PFT_CTL",
  "$HSP_CTL",
  "$SP_BOUND",
  "$SP_BOUND_PRIV",
  "$SP_BASE",
  "$SP_BASE_PRIV",
  "$FUCOP_CTL",
  "$PRUSR_ACC_CTL",
  "$DMA_CFG",
  "$DMA_GCSW",
  "$DMA_CHNSEL",
  "$DMA_ACT",
  "$DMA_SETUP",
  "$DMA_ISADDR",
  "$DMA_ESADDR",
  "$DMA_TCNT",
  "$DMA_STATUS",
  "$DMA_2DSET",
  "$DMA_2DSCTL",
  "$DMA_RCNT",
  "$DMA_HSTATUS",
  "$PC",
  "$SP_USR1",
  "$SP_USR2",
  "$SP_USR3",
  "$SP_PRIV1",
  "$SP_PRIV2",
  "$SP_PRIV3",
  "$BG_REGION",
  "$SFCR",
  "$SIGN",
  "$ISIGN",
  "$P_ISIGN",
  "$IFC_LP",
  "$ITB"
};
static const char * const nds32_cctl_names[] =
{
  "L1D_VA_FILLCK",
  "L1D_VA_ULCK",
  "L1I_VA_FILLCK",
  "L1I_VA_ULCK",
  "L1D_IX_WBINVAL",
  "L1D_IX_INVAL",
  "L1D_IX_WB",
  "L1I_IX_INVAL",
  "L1D_VA_INVAL",
  "L1D_VA_WB",
  "L1D_VA_WBINVAL",
  "L1I_VA_INVAL",
  "L1D_IX_RTAG",
  "L1D_IX_RWD",
  "L1I_IX_RTAG",
  "L1I_IX_RWD",
  "L1D_IX_WTAG",
  "L1D_IX_WWD",
  "L1I_IX_WTAG",
  "L1I_IX_WWD"
};
static const char * const nds32_dpref_names[] =
{
  "SRD",
  "MRD",
  "SWR",
  "MWR",
  "PTE",
  "CLWR"
};
static const int nds32_reg_alloc_order_for_speed[] =
{
   0, 1, 2, 3, 4, 5, 16, 17,
  18, 19, 20, 21, 22, 23, 24, 25,
  26, 27, 6, 7, 8, 9, 10, 11,
  12, 13, 14, 15
};
static const struct attribute_spec nds32_attribute_table[] =
{
  { "interrupt", 1, 64, false, false, false, NULL, false },
  { "exception", 1, 8, false, false, false, NULL, false },
  { "reset", 1, 1, false, false, false, NULL, false },
  { "nested", 0, 0, false, false, false, NULL, false },
  { "not_nested", 0, 0, false, false, false, NULL, false },
  { "nested_ready", 0, 0, false, false, false, NULL, false },
  { "critical", 0, 0, false, false, false, NULL, false },
  { "save_all", 0, 0, false, false, false, NULL, false },
  { "partial_save", 0, 0, false, false, false, NULL, false },
  { "nmi", 1, 1, false, false, false, NULL, false },
  { "warm", 1, 1, false, false, false, NULL, false },
  { "naked", 0, 0, false, false, false, NULL, false },
  { "signature", 0, 0, false, false, false, NULL, false },
  { "indirect_call",0, 0, false, false, false, NULL, false },
  { "no_prologue", 0, 0, false, false, false, NULL, false },
  { "no_ext_zol", 0, 0, false, false, false, NULL, false},
  { NULL, 0, 0, false, false, false, NULL, false }
};
static struct machine_function *
nds32_init_machine_status (void)
{
  struct machine_function *machine;
  machine = ggc_alloc_cleared_machine_function ();
  machine->use_eh_return_p = 0;
  machine->naked_p = 0;
  machine->fp_as_gp_p = 0;
  machine->strict_aligned_p = 0;
  machine->attr_naked_p = 0;
  machine->attr_no_prologue_p = 0;
  return machine;
}
static void
nds32_compute_stack_frame (void)
{
  int r;
  int block_size;
  bool v3pushpop_p;
  cfun->machine->naked_p = 0;
  cfun->machine->attr_naked_p
    = lookup_attribute ("naked", DECL_ATTRIBUTES (current_function_decl))
      ? 1 : 0;
  cfun->machine->attr_no_prologue_p
    = lookup_attribute ("no_prologue", DECL_ATTRIBUTES (current_function_decl))
      ? 1 : 0;
  if (crtl->calls_eh_return)
    {
      frame_pointer_needed = 1;
      cfun->machine->eh_return_data_first_regno = EH_RETURN_DATA_REGNO (0);
      for (r = 0; EH_RETURN_DATA_REGNO (r) != INVALID_REGNUM; r++)
 cfun->machine->eh_return_data_last_regno = r;
      cfun->machine->eh_return_data_regs_size
 = 4 * (cfun->machine->eh_return_data_last_regno
        - cfun->machine->eh_return_data_first_regno
        + 1);
      cfun->machine->use_eh_return_p = 1;
    }
  else
    {
      cfun->machine->eh_return_data_first_regno = SP_REGNUM;
      cfun->machine->eh_return_data_last_regno = SP_REGNUM;
      cfun->machine->eh_return_data_regs_size = 0;
      cfun->machine->use_eh_return_p = 0;
    }
  cfun->machine->va_args_size = crtl->args.pretend_args_size;
  if (cfun->machine->va_args_size != 0)
    {
      cfun->machine->va_args_first_regno
 = NDS32_GPR_ARG_FIRST_REGNUM
   + NDS32_MAX_GPR_REGS_FOR_ARGS
   - (crtl->args.pretend_args_size / UNITS_PER_WORD);
      cfun->machine->va_args_last_regno
 = NDS32_GPR_ARG_FIRST_REGNUM + NDS32_MAX_GPR_REGS_FOR_ARGS - 1;
    }
  else
    {
      cfun->machine->va_args_first_regno = SP_REGNUM;
      cfun->machine->va_args_last_regno = SP_REGNUM;
    }
  block_size = cfun->machine->va_args_size;
  if (!NDS32_DOUBLE_WORD_ALIGN_P (block_size))
    {
      cfun->machine->va_args_area_padding_bytes
 = NDS32_ROUND_UP_DOUBLE_WORD (block_size) - block_size;
    }
  cfun->machine->local_size = NDS32_ROUND_UP_DOUBLE_WORD (get_frame_size ());
  cfun->machine->out_args_size = crtl->outgoing_args_size;
  cfun->machine->fp_size = (df_regs_ever_live_p (FP_REGNUM)) ? 4 : 0;
  cfun->machine->gp_size =
    (flag_pic && df_regs_ever_live_p (PIC_OFFSET_TABLE_REGNUM)) ? 4 : 0;
  cfun->machine->lp_size
    = (flag_always_save_lp || df_regs_ever_live_p (LP_REGNUM)) ? 4 : 0;
  cfun->machine->callee_saved_area_gpr_padding_bytes = 0;
  cfun->machine->callee_saved_gpr_regs_size = 0;
  cfun->machine->callee_saved_first_gpr_regno = SP_REGNUM;
  cfun->machine->callee_saved_last_gpr_regno = SP_REGNUM;
  cfun->machine->callee_saved_fpr_regs_size = 0;
  cfun->machine->callee_saved_first_fpr_regno = SP_REGNUM;
  cfun->machine->callee_saved_last_fpr_regno = SP_REGNUM;
  for (r = 0; r < 28; r++)
    {
      if (NDS32_REQUIRED_CALLEE_SAVED_P (r))
 {
   if (cfun->machine->callee_saved_first_gpr_regno == SP_REGNUM)
     cfun->machine->callee_saved_first_gpr_regno = r;
   cfun->machine->callee_saved_last_gpr_regno = r;
 }
    }
  if (TARGET_HARD_FLOAT)
    {
      for (r = NDS32_FIRST_FPR_REGNUM; r < NDS32_LAST_FPR_REGNUM; r++)
 {
   if (NDS32_REQUIRED_CALLEE_SAVED_P (r))
     {
       if (cfun->machine->callee_saved_first_fpr_regno == SP_REGNUM)
  {
    if (!NDS32_FPR_REGNO_OK_FOR_DOUBLE (r))
      cfun->machine->callee_saved_first_fpr_regno = r - 1;
    else
      cfun->machine->callee_saved_first_fpr_regno = r;
  }
       cfun->machine->callee_saved_last_fpr_regno = r;
     }
 }
      int last_fpr = cfun->machine->callee_saved_last_fpr_regno;
      if (NDS32_FPR_REGNO_OK_FOR_DOUBLE (last_fpr))
 cfun->machine->callee_saved_last_fpr_regno++;
    }
  if (lookup_attribute ("no_prologue", DECL_ATTRIBUTES (current_function_decl))
      || lookup_attribute ("naked", DECL_ATTRIBUTES (current_function_decl))
      || (cfun->machine->callee_saved_first_gpr_regno == SP_REGNUM
   && cfun->machine->callee_saved_last_gpr_regno == SP_REGNUM
   && cfun->machine->callee_saved_first_fpr_regno == SP_REGNUM
   && cfun->machine->callee_saved_last_fpr_regno == SP_REGNUM
   && !df_regs_ever_live_p (FP_REGNUM)
   && !df_regs_ever_live_p (LP_REGNUM)
   && cfun->machine->local_size == 0
   && !flag_pic))
    {
      cfun->machine->naked_p = 1;
      cfun->machine->fp_size = 0;
      cfun->machine->gp_size = 0;
      cfun->machine->lp_size = 0;
      if (flag_stack_usage_info)
 current_function_static_stack_size = 0;
      return;
    }
  v3pushpop_p = NDS32_V3PUSH_AVAILABLE_P;
  if (v3pushpop_p)
    {
      cfun->machine->fp_size = 4;
      cfun->machine->gp_size = 4;
      cfun->machine->lp_size = 4;
      cfun->machine->callee_saved_first_gpr_regno = 6;
      if (cfun->machine->callee_saved_last_gpr_regno <= 6)
 {
   cfun->machine->callee_saved_last_gpr_regno = 6;
 }
      else if (cfun->machine->callee_saved_last_gpr_regno <= 8)
 {
   cfun->machine->callee_saved_last_gpr_regno = 8;
 }
      else if (cfun->machine->callee_saved_last_gpr_regno <= 10)
 {
   cfun->machine->callee_saved_last_gpr_regno = 10;
 }
      else if (cfun->machine->callee_saved_last_gpr_regno <= 14)
 {
   cfun->machine->callee_saved_last_gpr_regno = 14;
 }
      else if (cfun->machine->callee_saved_last_gpr_regno == SP_REGNUM)
 {
   cfun->machine->callee_saved_last_gpr_regno = 6;
 }
      else
 {
   gcc_unreachable ();
 }
    }
  int sp_adjust = cfun->machine->local_size
    + cfun->machine->out_args_size
    + cfun->machine->callee_saved_area_gpr_padding_bytes
    + cfun->machine->callee_saved_fpr_regs_size;
  if (!v3pushpop_p
      && nds32_memory_model_option == MEMORY_MODEL_FAST
      && sp_adjust == 0
      && !frame_pointer_needed)
    {
      block_size = cfun->machine->fp_size
     + cfun->machine->gp_size
     + cfun->machine->lp_size
     + (4 * (cfun->machine->callee_saved_last_gpr_regno
      - cfun->machine->callee_saved_first_gpr_regno
      + 1));
      if (!NDS32_DOUBLE_WORD_ALIGN_P (block_size))
 {
   if (cfun->machine->callee_saved_last_gpr_regno
       < NDS32_LAST_CALLEE_SAVE_GPR_REGNUM)
     {
       cfun->machine->callee_saved_last_gpr_regno++;
     }
   else if (cfun->machine->callee_saved_first_gpr_regno == SP_REGNUM)
     {
       cfun->machine->callee_saved_first_gpr_regno
  = NDS32_FIRST_CALLEE_SAVE_GPR_REGNUM;
       cfun->machine->callee_saved_last_gpr_regno
  = NDS32_FIRST_CALLEE_SAVE_GPR_REGNUM;
     }
 }
    }
  if (cfun->machine->callee_saved_last_gpr_regno != SP_REGNUM)
    {
      cfun->machine->callee_saved_gpr_regs_size
 = 4 * (cfun->machine->callee_saved_last_gpr_regno
        - cfun->machine->callee_saved_first_gpr_regno
        + 1);
    }
  if (TARGET_HARD_FLOAT)
    {
      if (cfun->machine->callee_saved_last_fpr_regno != SP_REGNUM)
 {
   cfun->machine->callee_saved_fpr_regs_size
    = 4 * (cfun->machine->callee_saved_last_fpr_regno
    - cfun->machine->callee_saved_first_fpr_regno
    + 1);
 }
    }
  block_size = cfun->machine->fp_size
        + cfun->machine->gp_size
        + cfun->machine->lp_size
        + cfun->machine->callee_saved_gpr_regs_size;
  if (!NDS32_DOUBLE_WORD_ALIGN_P (block_size))
    {
      cfun->machine->callee_saved_area_gpr_padding_bytes
 = NDS32_ROUND_UP_DOUBLE_WORD (block_size) - block_size;
    }
  if (flag_stack_usage_info)
    {
      current_function_static_stack_size
 = NDS32_ROUND_UP_DOUBLE_WORD (block_size)
   + cfun->machine->local_size
   + cfun->machine->out_args_size;
    }
}
static void
nds32_emit_stack_push_multiple (unsigned Rb, unsigned Re,
    bool save_fp_p, bool save_gp_p, bool save_lp_p,
    bool vaarg_p)
{
  unsigned regno;
  int extra_count;
  int num_use_regs;
  int par_index;
  int offset;
  rtx reg;
  rtx mem;
  rtx push_rtx;
  rtx adjust_sp_rtx;
  rtx parallel_insn;
  rtx dwarf;
  extra_count = 0;
  if (save_fp_p)
    extra_count++;
  if (save_gp_p)
    extra_count++;
  if (save_lp_p)
    extra_count++;
  if (Rb == SP_REGNUM && Re == SP_REGNUM)
    num_use_regs = extra_count;
  else
    num_use_regs = Re - Rb + 1 + extra_count;
  parallel_insn = gen_rtx_PARALLEL (VOIDmode,
        rtvec_alloc (num_use_regs + 1));
  par_index = 0;
  offset = -(num_use_regs * 4);
  for (regno = Rb; regno <= Re; regno++)
    {
      if (regno == SP_REGNUM)
 break;
      reg = gen_rtx_REG (SImode, regno);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
      XVECEXP (parallel_insn, 0, par_index) = push_rtx;
      RTX_FRAME_RELATED_P (push_rtx) = 1;
      offset = offset + 4;
      par_index++;
    }
  if (save_fp_p)
    {
      reg = gen_rtx_REG (SImode, FP_REGNUM);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
      XVECEXP (parallel_insn, 0, par_index) = push_rtx;
      RTX_FRAME_RELATED_P (push_rtx) = 1;
      offset = offset + 4;
      par_index++;
    }
  if (save_gp_p)
    {
      reg = gen_rtx_REG (SImode, GP_REGNUM);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
      XVECEXP (parallel_insn, 0, par_index) = push_rtx;
      RTX_FRAME_RELATED_P (push_rtx) = 1;
      offset = offset + 4;
      par_index++;
    }
  if (save_lp_p)
    {
      reg = gen_rtx_REG (SImode, LP_REGNUM);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
      XVECEXP (parallel_insn, 0, par_index) = push_rtx;
      RTX_FRAME_RELATED_P (push_rtx) = 1;
      offset = offset + 4;
      par_index++;
    }
  offset = -(num_use_regs * 4);
  adjust_sp_rtx
    = gen_rtx_SET (VOIDmode,
     stack_pointer_rtx,
     plus_constant (Pmode, stack_pointer_rtx, offset));
  XVECEXP (parallel_insn, 0, par_index) = adjust_sp_rtx;
  RTX_FRAME_RELATED_P (adjust_sp_rtx) = 1;
  parallel_insn = emit_insn (parallel_insn);
  RTX_FRAME_RELATED_P (parallel_insn) = 1;
  if (vaarg_p)
    {
      dwarf = alloc_reg_note (REG_CFA_ADJUST_CFA,
         copy_rtx (adjust_sp_rtx), NULL_RTX);
      REG_NOTES (parallel_insn) = dwarf;
    }
}
static void
nds32_emit_stack_pop_multiple (unsigned Rb, unsigned Re,
          bool save_fp_p, bool save_gp_p, bool save_lp_p)
{
  unsigned regno;
  int extra_count;
  int num_use_regs;
  int par_index;
  int offset;
  rtx reg;
  rtx mem;
  rtx pop_rtx;
  rtx adjust_sp_rtx;
  rtx parallel_insn;
  rtx dwarf = NULL_RTX;
  extra_count = 0;
  if (save_fp_p)
    extra_count++;
  if (save_gp_p)
    extra_count++;
  if (save_lp_p)
    extra_count++;
  if (Rb == SP_REGNUM && Re == SP_REGNUM)
    num_use_regs = extra_count;
  else
    num_use_regs = Re - Rb + 1 + extra_count;
  parallel_insn = gen_rtx_PARALLEL (VOIDmode,
        rtvec_alloc (num_use_regs + 1));
  par_index = 0;
  offset = 0;
  for (regno = Rb; regno <= Re; regno++)
    {
      if (regno == SP_REGNUM)
 break;
      reg = gen_rtx_REG (SImode, regno);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
      XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
      RTX_FRAME_RELATED_P (pop_rtx) = 1;
      offset = offset + 4;
      par_index++;
      dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
    }
  if (save_fp_p)
    {
      reg = gen_rtx_REG (SImode, FP_REGNUM);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
      XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
      RTX_FRAME_RELATED_P (pop_rtx) = 1;
      offset = offset + 4;
      par_index++;
      dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
    }
  if (save_gp_p)
    {
      reg = gen_rtx_REG (SImode, GP_REGNUM);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
      XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
      RTX_FRAME_RELATED_P (pop_rtx) = 1;
      offset = offset + 4;
      par_index++;
      dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
    }
  if (save_lp_p)
    {
      reg = gen_rtx_REG (SImode, LP_REGNUM);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
      XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
      RTX_FRAME_RELATED_P (pop_rtx) = 1;
      offset = offset + 4;
      par_index++;
      dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
    }
  adjust_sp_rtx
    = gen_rtx_SET (VOIDmode,
     stack_pointer_rtx,
     plus_constant (Pmode, stack_pointer_rtx, offset));
  XVECEXP (parallel_insn, 0, par_index) = adjust_sp_rtx;
  dwarf = alloc_reg_note (REG_CFA_ADJUST_CFA, copy_rtx (adjust_sp_rtx), dwarf);
  parallel_insn = emit_insn (parallel_insn);
  RTX_FRAME_RELATED_P (parallel_insn) = 1;
  REG_NOTES (parallel_insn) = dwarf;
}
static void
nds32_emit_stack_v3push (unsigned Rb,
    unsigned Re,
    unsigned imm8u)
{
  unsigned regno;
  int num_use_regs;
  int par_index;
  int offset;
  rtx reg;
  rtx mem;
  rtx push_rtx;
  rtx adjust_sp_rtx;
  rtx parallel_insn;
  num_use_regs = Re - Rb + 1 + 3;
  parallel_insn = gen_rtx_PARALLEL (VOIDmode,
        rtvec_alloc (num_use_regs + 1));
  par_index = 0;
  offset = -(num_use_regs * 4);
  for (regno = Rb; regno <= Re; regno++)
    {
      reg = gen_rtx_REG (SImode, regno);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
      XVECEXP (parallel_insn, 0, par_index) = push_rtx;
      RTX_FRAME_RELATED_P (push_rtx) = 1;
      offset = offset + 4;
      par_index++;
    }
  reg = gen_rtx_REG (SImode, FP_REGNUM);
  mem = gen_frame_mem (SImode, plus_constant (Pmode,
           stack_pointer_rtx,
           offset));
  push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
  XVECEXP (parallel_insn, 0, par_index) = push_rtx;
  RTX_FRAME_RELATED_P (push_rtx) = 1;
  offset = offset + 4;
  par_index++;
  reg = gen_rtx_REG (SImode, GP_REGNUM);
  mem = gen_frame_mem (SImode, plus_constant (Pmode,
           stack_pointer_rtx,
           offset));
  push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
  XVECEXP (parallel_insn, 0, par_index) = push_rtx;
  RTX_FRAME_RELATED_P (push_rtx) = 1;
  offset = offset + 4;
  par_index++;
  reg = gen_rtx_REG (SImode, LP_REGNUM);
  mem = gen_frame_mem (SImode, plus_constant (Pmode,
           stack_pointer_rtx,
           offset));
  push_rtx = gen_rtx_SET (VOIDmode, mem, reg);
  XVECEXP (parallel_insn, 0, par_index) = push_rtx;
  RTX_FRAME_RELATED_P (push_rtx) = 1;
  offset = offset + 4;
  par_index++;
  offset = -(num_use_regs * 4);
  adjust_sp_rtx
    = gen_rtx_SET (VOIDmode,
     stack_pointer_rtx,
     plus_constant (Pmode,
      stack_pointer_rtx,
      offset - imm8u));
  XVECEXP (parallel_insn, 0, par_index) = adjust_sp_rtx;
  RTX_FRAME_RELATED_P (adjust_sp_rtx) = 1;
  parallel_insn = emit_insn (parallel_insn);
  RTX_FRAME_RELATED_P (parallel_insn) = 1;
}
static void
nds32_emit_stack_v3pop (unsigned Rb,
   unsigned Re,
   unsigned imm8u)
{
  unsigned regno;
  int num_use_regs;
  int par_index;
  int offset;
  rtx reg;
  rtx mem;
  rtx pop_rtx;
  rtx adjust_sp_rtx;
  rtx parallel_insn;
  rtx dwarf = NULL_RTX;
  num_use_regs = Re - Rb + 1 + 3;
  parallel_insn = gen_rtx_PARALLEL (VOIDmode,
        rtvec_alloc (num_use_regs + 1));
  par_index = 0;
  offset = 0;
  for (regno = Rb; regno <= Re; regno++)
    {
      reg = gen_rtx_REG (SImode, regno);
      mem = gen_frame_mem (SImode, plus_constant (Pmode,
        stack_pointer_rtx,
        offset));
      pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
      XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
      RTX_FRAME_RELATED_P (pop_rtx) = 1;
      offset = offset + 4;
      par_index++;
      dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
    }
  reg = gen_rtx_REG (SImode, FP_REGNUM);
  mem = gen_frame_mem (SImode, plus_constant (Pmode,
           stack_pointer_rtx,
           offset));
  pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
  XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
  RTX_FRAME_RELATED_P (pop_rtx) = 1;
  offset = offset + 4;
  par_index++;
  dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
  reg = gen_rtx_REG (SImode, GP_REGNUM);
  mem = gen_frame_mem (SImode, plus_constant (Pmode,
           stack_pointer_rtx,
           offset));
  pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
  XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
  RTX_FRAME_RELATED_P (pop_rtx) = 1;
  offset = offset + 4;
  par_index++;
  dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
  reg = gen_rtx_REG (SImode, LP_REGNUM);
  mem = gen_frame_mem (SImode, plus_constant (Pmode,
           stack_pointer_rtx,
           offset));
  pop_rtx = gen_rtx_SET (VOIDmode, reg, mem);
  XVECEXP (parallel_insn, 0, par_index) = pop_rtx;
  RTX_FRAME_RELATED_P (pop_rtx) = 1;
  offset = offset + 4;
  par_index++;
  dwarf = alloc_reg_note (REG_CFA_RESTORE, reg, dwarf);
  adjust_sp_rtx
    = gen_rtx_SET (VOIDmode,
     stack_pointer_rtx,
     plus_constant (Pmode,
      stack_pointer_rtx,
      offset + imm8u));
  XVECEXP (parallel_insn, 0, par_index) = adjust_sp_rtx;
  if (frame_pointer_needed)
    {
      rtx cfa_adjust_rtx = gen_rtx_PLUS (Pmode, stack_pointer_rtx,
      const0_rtx);
      dwarf = alloc_reg_note (REG_CFA_DEF_CFA, cfa_adjust_rtx, dwarf);
    }
  else
    {
      dwarf = alloc_reg_note (REG_CFA_ADJUST_CFA,
         copy_rtx (adjust_sp_rtx), dwarf);
    }
  parallel_insn = emit_insn (parallel_insn);
  RTX_FRAME_RELATED_P (parallel_insn) = 1;
  REG_NOTES (parallel_insn) = dwarf;
}
static void
nds32_emit_load_gp (void)
{
  rtx got_symbol, pat;
  emit_insn (gen_blockage ());
  got_symbol = gen_rtx_SYMBOL_REF (Pmode, "_GLOBAL_OFFSET_TABLE_");
  pat = gen_rtx_UNSPEC (SImode, gen_rtvec (1, got_symbol), UNSPEC_GOTINIT);
  pat = gen_rtx_CONST (SImode, gen_rtx_PLUS (Pmode, pat, GEN_INT (-8)));
  emit_insn (gen_sethi (pic_offset_table_rtx,pat));
  pat = gen_rtx_UNSPEC (SImode, gen_rtvec (1, got_symbol), UNSPEC_GOTINIT);
  pat = gen_rtx_CONST (SImode, gen_rtx_PLUS (Pmode, pat, GEN_INT (-4)));
  emit_insn (gen_lo_sum (pic_offset_table_rtx, pic_offset_table_rtx, pat));
  emit_insn (gen_add_pc (pic_offset_table_rtx, pic_offset_table_rtx));
  emit_insn (gen_blockage ());
}
static void
nds32_emit_adjust_frame (rtx to_reg, rtx from_reg, int adjust_value)
{
  rtx tmp_reg;
  rtx frame_adjust_insn;
  rtx adjust_value_rtx = GEN_INT (adjust_value);
  if (adjust_value == 0)
    return;
  if (!satisfies_constraint_Is15 (adjust_value_rtx))
    {
      tmp_reg = gen_rtx_REG (SImode, TA_REGNUM);
      emit_move_insn (tmp_reg, adjust_value_rtx);
      frame_adjust_insn = gen_addsi3 (to_reg,
          from_reg,
          tmp_reg);
      frame_adjust_insn = emit_insn (frame_adjust_insn);
      rtx plus_rtx;
      rtx set_rtx;
      plus_rtx = plus_constant (Pmode, from_reg, adjust_value);
      set_rtx = gen_rtx_SET (VOIDmode, to_reg, plus_rtx);
      add_reg_note (frame_adjust_insn, REG_FRAME_RELATED_EXPR, set_rtx);
    }
  else
    {
      frame_adjust_insn = gen_addsi3 (to_reg,
          from_reg,
          adjust_value_rtx);
      frame_adjust_insn = emit_insn (frame_adjust_insn);
    }
    RTX_FRAME_RELATED_P (frame_adjust_insn) = 1;
}
static bool
nds32_needs_double_word_align (enum machine_mode mode, const_tree type)
{
  unsigned int align;
  align = NDS32_MODE_TYPE_ALIGN (mode, type);
  return (align > PARM_BOUNDARY);
}
bool
nds32_naked_function_p (tree func)
{
  tree t_naked;
  tree t_no_prologue;
  if (TREE_CODE (func) != FUNCTION_DECL)
    abort ();
  t_naked = lookup_attribute ("naked", DECL_ATTRIBUTES (func));
  t_no_prologue = lookup_attribute ("no_prologue", DECL_ATTRIBUTES (func));
  return ((t_naked != NULL_TREE) || (t_no_prologue != NULL_TREE));
}
static bool
nds32_address_register_rtx_p (rtx x, bool strict)
{
  int regno;
  if (GET_CODE (x) != REG)
    return false;
  regno = REGNO (x);
  if (strict)
    return REGNO_OK_FOR_BASE_P (regno);
  else
    return true;
}
static bool
nds32_legitimate_index_p (enum machine_mode outer_mode,
     rtx index,
     bool strict)
{
  int regno;
  rtx op0;
  rtx op1;
  switch (GET_CODE (index))
    {
    case REG:
      regno = REGNO (index);
      if (strict)
 return REGNO_OK_FOR_INDEX_P (regno);
      else
 return true;
    case CONST_INT:
      switch (GET_MODE_SIZE (outer_mode))
 {
 case 1:
   if (satisfies_constraint_Is15 (index))
     return true;
   break;
 case 2:
   if (satisfies_constraint_Is16 (index))
     {
       if (!cfun->machine->strict_aligned_p)
  return true;
       else if (NDS32_HALF_WORD_ALIGN_P (INTVAL (index)))
  return true;
     }
   break;
 case 4:
   if (satisfies_constraint_Is17 (index))
     {
       if ((TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE))
  {
    if (!satisfies_constraint_Is14 (index))
      return false;
  }
       if (!cfun->machine->strict_aligned_p)
  return true;
       else if (NDS32_SINGLE_WORD_ALIGN_P (INTVAL (index)))
  return true;
     }
   break;
 case 8:
   if (satisfies_constraint_Is17 (gen_int_mode (INTVAL (index) + 4,
             SImode)))
     {
       if ((TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE))
  {
    if (!satisfies_constraint_Is14 (index))
      return false;
  }
       if (!cfun->machine->strict_aligned_p)
  return true;
       else if (NDS32_SINGLE_WORD_ALIGN_P (INTVAL (index)))
  return true;
     }
   break;
 default:
   return false;
 }
      return false;
    case MULT:
      op0 = XEXP (index, 0);
      op1 = XEXP (index, 1);
      if (REG_P (op0) && CONST_INT_P (op1))
 {
   int multiplier;
   multiplier = INTVAL (op1);
   if (multiplier != 1 && multiplier != 2
       && multiplier != 4 && multiplier != 8)
     return false;
   regno = REGNO (op0);
   if(strict)
     return REGNO_OK_FOR_INDEX_P (regno);
   else
     return true;
 }
      return false;
    case ASHIFT:
      op0 = XEXP (index, 0);
      op1 = XEXP (index, 1);
      if (REG_P (op0) && CONST_INT_P (op1))
 {
   int sv;
   sv = INTVAL (op1);
   if (sv != 0 && sv != 1 && sv !=2 && sv != 3)
     return false;
   regno = REGNO (op0);
   if(strict)
     return REGNO_OK_FOR_INDEX_P (regno);
   else
     return true;
 }
      return false;
    default:
      return false;
    }
}
static void
nds32_insert_innermost_loop (void)
{
  struct loop *loop;
  basic_block *bbs, bb;
  compute_bb_for_insn ();
  loop_optimizer_init (AVOID_CFG_MODIFICATIONS);
  FOR_EACH_LOOP (loop, LI_ONLY_INNERMOST)
    {
      bbs = get_loop_body (loop);
      bb = *bbs;
      free (bbs);
      emit_insn_before (gen_innermost_loop_begin (),
   BB_HEAD (bb));
      while (bb)
 {
   if (bb->next_bb == NULL)
     break;
   if (bb->next_bb->loop_father != loop)
     break;
   bb = bb->next_bb;
 }
      emit_insn_before (gen_innermost_loop_end (),
   BB_END (bb));
    }
  loop_optimizer_finalize ();
}
static void
nds32_insert_isps (void)
{
  rtx insn;
  unsigned first = 0;
  if (!lookup_attribute ("signature", DECL_ATTRIBUTES (current_function_decl)))
    return;
  insn = get_insns ();
  while (insn)
    {
      if (!first && INSN_P (insn))
 {
   emit_insn_before (gen_unspec_signature_begin (), insn);
   first = 1;
 }
      if (LABEL_P (insn) || CALL_P (insn) || any_condjump_p (insn)
   || (INSN_P (insn) && GET_CODE (PATTERN (insn)) == UNSPEC_VOLATILE
       && (XINT (PATTERN (insn), 1) == UNSPEC_VOLATILE_SYSCALL
    || XINT (PATTERN (insn), 1) == UNSPEC_VOLATILE_TRAP
    || XINT (PATTERN (insn), 1) == UNSPEC_VOLATILE_TEQZ
    || XINT (PATTERN (insn), 1) == UNSPEC_VOLATILE_TNEZ)))
 {
   emit_insn_after (gen_unspec_signature_begin (), insn);
 }
      insn = NEXT_INSN (insn);
    }
}
static void
nds32_register_pass (
  rtl_opt_pass *(*make_pass_func) (gcc::context *),
  enum pass_positioning_ops pass_pos,
  const char *ref_pass_name)
{
  opt_pass *new_opt_pass = make_pass_func (g);
  struct register_pass_info insert_pass =
    {
      new_opt_pass,
      ref_pass_name,
      1,
      pass_pos
    };
  register_pass (&insert_pass);
}
static void
nds32_register_passes (void)
{
  nds32_register_pass (
    make_pass_nds32_fp_as_gp,
    PASS_POS_INSERT_BEFORE,
    "ira");
  nds32_register_pass (
    make_pass_nds32_relax_opt,
    PASS_POS_INSERT_AFTER,
    "mach");
  nds32_register_pass (
    make_pass_nds32_hwloop2_opt,
    PASS_POS_INSERT_BEFORE,
    "mach");
  nds32_register_pass (
    make_pass_nds32_load_store_opt,
    PASS_POS_INSERT_AFTER,
    "mach");
  nds32_register_pass (
    make_pass_nds32_soft_fp_arith_comm_opt,
    PASS_POS_INSERT_BEFORE,
    "ira");
  nds32_register_pass (
    make_pass_nds32_regrename_opt,
    PASS_POS_INSERT_AFTER,
    "mach");
  nds32_register_pass (
    make_pass_nds32_gcse_opt,
    PASS_POS_INSERT_BEFORE,
    "cprop_hardreg");
  nds32_register_pass (
    make_pass_cprop_hardreg,
    PASS_POS_INSERT_AFTER,
    "mach");
  nds32_register_pass (
    make_pass_nds32_hwloop1_opt,
    PASS_POS_INSERT_BEFORE,
    "ira");
  nds32_register_pass (
    nds32::scheduling::make_pass_nds32_print_stalls,
    PASS_POS_INSERT_BEFORE,
    "final");
}
int
nds32_adjust_insn_length (rtx insn, int length)
{
  int adjust_value = 0;
  switch (recog_memoized (insn))
    {
    case CODE_FOR_call_immediate_align:
    case CODE_FOR_call_value_immediate_align:
    case CODE_FOR_call_register_align:
    case CODE_FOR_call_value_register_align:
      {
 rtx next_insn = next_active_insn (insn);
 if (next_insn && get_attr_length (next_insn) != 2)
   adjust_value += 2;
      }
    case CODE_FOR_call_immediate:
    case CODE_FOR_call_value_immediate:
    case CODE_FOR_call_register:
    case CODE_FOR_call_value_register:
      {
 if (find_reg_note (insn, REG_NORETURN, NULL_RTX))
   {
     if (TARGET_16_BIT)
       adjust_value += 2;
     else
       adjust_value += 4;
   }
      }
      return length + adjust_value;
    default:
      return length;
    }
}
static void
nds32_expand_to_rtl_hook (void)
{
  cfun->machine->strict_aligned_p = 1;
}
static void
nds32_conditional_register_usage (void)
{
  int regno;
  if (TARGET_LINUX_ABI)
    fixed_regs[TP_REGNUM] = 1;
  if (TARGET_HARD_FLOAT)
    {
      for (regno = NDS32_FIRST_FPR_REGNUM;
    regno <= NDS32_LAST_FPR_REGNUM; regno++)
 {
   fixed_regs[regno] = 0;
   if (regno < NDS32_FIRST_FPR_REGNUM + NDS32_MAX_FPR_REGS_FOR_ARGS)
     call_used_regs[regno] = 1;
   else if (regno >= NDS32_FIRST_FPR_REGNUM + 22
     && regno < NDS32_FIRST_FPR_REGNUM + 48)
     call_used_regs[regno] = 1;
   else
     call_used_regs[regno] = 0;
 }
    }
  else if (TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE)
    {
      for (regno = NDS32_FIRST_FPR_REGNUM;
    regno <= NDS32_LAST_FPR_REGNUM;
    regno++)
 fixed_regs[regno] = 0;
    }
}
static reg_class_t
nds32_preferred_rename_class (reg_class_t rclass)
{
  return nds32_preferred_rename_class_impl (rclass);
}
static unsigned char
nds32_class_max_nregs (reg_class_t rclass ATTRIBUTE_UNUSED,
         enum machine_mode mode)
{
  return ((GET_MODE_SIZE (mode) + UNITS_PER_WORD - 1) / UNITS_PER_WORD);
}
static int
nds32_register_priority (int hard_regno)
{
  if (optimize_size)
    {
      if (hard_regno < 8)
 return 4;
      else if (hard_regno < 16)
 return 3;
      else if (hard_regno < 28)
 return 2;
      else
 return 1;
    }
  else
    {
      if (hard_regno > 27)
 return 1;
      else
 return 4;
    }
}
static bool
nds32_can_eliminate (const int from_reg, const int to_reg)
{
  if (from_reg == ARG_POINTER_REGNUM && to_reg == STACK_POINTER_REGNUM)
    return true;
  if (from_reg == ARG_POINTER_REGNUM && to_reg == HARD_FRAME_POINTER_REGNUM)
    return true;
  if (from_reg == FRAME_POINTER_REGNUM && to_reg == STACK_POINTER_REGNUM)
    return true;
  if (from_reg == FRAME_POINTER_REGNUM && to_reg == HARD_FRAME_POINTER_REGNUM)
    return true;
  return false;
}
static rtx
nds32_function_arg (cumulative_args_t ca, enum machine_mode mode,
      const_tree type, bool named)
{
  unsigned int regno;
  CUMULATIVE_ARGS *cum = get_cumulative_args (ca);
  if (mode == VOIDmode)
    return NULL_RTX;
  if (!named)
    {
      if (TARGET_HARD_FLOAT)
 return NULL_RTX;
      if (NDS32_ARG_PARTIAL_IN_GPR_REG_P (cum->gpr_offset, mode, type))
 {
   regno
     = NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type);
   return gen_rtx_REG (mode, regno);
 }
      return NULL_RTX;
    }
  if (TARGET_HARD_FLOAT)
    {
      if (GET_MODE_CLASS (mode) == MODE_FLOAT
   && NDS32_ARG_ENTIRE_IN_FPR_REG_P (cum->fpr_offset, mode, type))
 {
   regno
     = NDS32_AVAILABLE_REGNUM_FOR_FPR_ARG (cum->fpr_offset, mode, type);
   return gen_rtx_REG (mode, regno);
 }
      else if (GET_MODE_CLASS (mode) != MODE_FLOAT
        && NDS32_ARG_ENTIRE_IN_GPR_REG_P (cum->gpr_offset, mode, type))
 {
   regno
     = NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type);
   return gen_rtx_REG (mode, regno);
 }
    }
  else
    {
      if (NDS32_ARG_PARTIAL_IN_GPR_REG_P (cum->gpr_offset, mode, type))
 {
   regno
     = NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type);
   return gen_rtx_REG (mode, regno);
 }
    }
  return NULL_RTX;
}
static bool
nds32_must_pass_in_stack (enum machine_mode mode, const_tree type)
{
  if (TARGET_HARD_FLOAT)
    return must_pass_in_stack_var_size_or_pad (mode, type);
  else
    return must_pass_in_stack_var_size (mode, type);
}
static int
nds32_arg_partial_bytes (cumulative_args_t ca, enum machine_mode mode,
    tree type, bool named ATTRIBUTE_UNUSED)
{
  unsigned int needed_reg_count;
  unsigned int remaining_reg_count;
  CUMULATIVE_ARGS *cum;
  cum = get_cumulative_args (ca);
  if (TARGET_HARD_FLOAT)
    return 0;
  if (NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type)
      >= NDS32_GPR_ARG_FIRST_REGNUM + NDS32_MAX_GPR_REGS_FOR_ARGS)
    return 0;
  needed_reg_count = NDS32_NEED_N_REGS_FOR_ARG (mode, type);
  remaining_reg_count
    = NDS32_MAX_GPR_REGS_FOR_ARGS
      - (NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type)
  - NDS32_GPR_ARG_FIRST_REGNUM);
  if (needed_reg_count > remaining_reg_count)
    return remaining_reg_count * UNITS_PER_WORD;
  return 0;
}
static void
nds32_function_arg_advance (cumulative_args_t ca, enum machine_mode mode,
       const_tree type, bool named)
{
  CUMULATIVE_ARGS *cum = get_cumulative_args (ca);
  if (named)
    {
      if (TARGET_HARD_FLOAT && GET_MODE_CLASS (mode) == MODE_FLOAT)
 {
   cum->fpr_offset
     = NDS32_AVAILABLE_REGNUM_FOR_FPR_ARG (cum->fpr_offset, mode, type)
       - NDS32_FPR_ARG_FIRST_REGNUM
       + NDS32_NEED_N_REGS_FOR_ARG (mode, type);
 }
      else
 {
   cum->gpr_offset
     = NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type)
       - NDS32_GPR_ARG_FIRST_REGNUM
       + NDS32_NEED_N_REGS_FOR_ARG (mode, type);
 }
    }
  else
    {
      if (!TARGET_HARD_FLOAT)
 {
   cum->gpr_offset
     = NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type)
       - NDS32_GPR_ARG_FIRST_REGNUM
       + NDS32_NEED_N_REGS_FOR_ARG (mode, type);
 }
    }
}
static unsigned int
nds32_function_arg_boundary (enum machine_mode mode, const_tree type)
{
  return (nds32_needs_double_word_align (mode, type)
   ? NDS32_DOUBLE_WORD_ALIGNMENT
   : PARM_BOUNDARY);
}
bool
nds32_vector_mode_supported_p (enum machine_mode mode)
{
  if (mode == V4QImode
      || mode == V2HImode)
    return NDS32_EXT_DSP_P ();
  return false;
}
static rtx
nds32_function_value (const_tree ret_type,
        const_tree fn_decl_or_type ATTRIBUTE_UNUSED,
        bool outgoing ATTRIBUTE_UNUSED)
{
  enum machine_mode mode;
  int unsignedp;
  mode = TYPE_MODE (ret_type);
  unsignedp = TYPE_UNSIGNED (ret_type);
  if (INTEGRAL_TYPE_P (ret_type))
    mode = promote_mode (ret_type, mode, &unsignedp);
  if (TARGET_HARD_FLOAT && (mode == SFmode || mode == DFmode))
    return gen_rtx_REG (mode, NDS32_FPR_RET_FIRST_REGNUM);
  else
    return gen_rtx_REG (mode, NDS32_GPR_RET_FIRST_REGNUM);
}
static rtx
nds32_libcall_value (enum machine_mode mode,
       const_rtx fun ATTRIBUTE_UNUSED)
{
  if (TARGET_HARD_FLOAT && (mode == SFmode || mode == DFmode))
    return gen_rtx_REG (mode, NDS32_FPR_RET_FIRST_REGNUM);
  return gen_rtx_REG (mode, NDS32_GPR_RET_FIRST_REGNUM);
}
static bool
nds32_function_value_regno_p (const unsigned int regno)
{
  if (regno == NDS32_GPR_RET_FIRST_REGNUM
      || (TARGET_HARD_FLOAT
   && regno == NDS32_FPR_RET_FIRST_REGNUM))
    return true;
  return false;
}
static bool
nds32_return_in_memory (const_tree type,
   const_tree fntype ATTRIBUTE_UNUSED)
{
  HOST_WIDE_INT size = int_size_in_bytes (type);
  if (TREE_CODE (type) == COMPLEX_TYPE)
    if (size < 0 || size > 2 * UNITS_PER_WORD)
      return true;
  if (TYPE_MODE (type) == BLKmode)
    if (size < 0 || size > 2 * UNITS_PER_WORD)
      return true;
  return false;
}
static void
nds32_asm_function_prologue (FILE *file,
        HOST_WIDE_INT size ATTRIBUTE_UNUSED)
{
  int r;
  const char *func_name;
  tree attrs;
  tree name;
  fprintf (file, "\t! BEGIN PROLOGUE\n");
  fprintf (file, "\t!     fp needed: %d\n", frame_pointer_needed);
  fprintf (file, "\t!  pretend_args: %d\n", cfun->machine->va_args_size);
  fprintf (file, "\t!    local_size: %d\n", cfun->machine->local_size);
  fprintf (file, "\t! out_args_size: %d\n", cfun->machine->out_args_size);
  fprintf (file, "\t! registers ever_live: ");
  for (r = 0; r < 65; r++)
    {
      if (df_regs_ever_live_p (r))
 fprintf (file, "%s, ", reg_names[r]);
    }
  fputc ('\n', file);
  fprintf (file, "\t! function attributes: ");
  attrs = DECL_ATTRIBUTES (current_function_decl);
  if (!attrs)
    fprintf (file, "None");
  func_name = IDENTIFIER_POINTER (DECL_NAME (current_function_decl));
  nds32_construct_isr_vectors_information (attrs, func_name);
  while (attrs)
    {
      name = TREE_PURPOSE (attrs);
      fprintf (file, "%s ", IDENTIFIER_POINTER (name));
      attrs = TREE_CHAIN (attrs);
    }
  fputc ('\n', file);
  if (nds32_isr_function_critical_p (current_function_decl))
    fprintf (file, "\t.no_relax ifc\n");
}
static void
nds32_asm_function_end_prologue (FILE *file)
{
  fprintf (file, "\t! END PROLOGUE\n");
}
static void
nds32_asm_function_begin_epilogue (FILE *file)
{
  fprintf (file, "\t! BEGIN EPILOGUE\n");
}
static void
nds32_asm_function_epilogue (FILE *file,
        HOST_WIDE_INT size ATTRIBUTE_UNUSED)
{
  fprintf (file, "\t! END EPILOGUE\n");
}
static void
nds32_asm_output_mi_thunk (FILE *file, tree thunk ATTRIBUTE_UNUSED,
      HOST_WIDE_INT delta,
      HOST_WIDE_INT vcall_offset ATTRIBUTE_UNUSED,
      tree function)
{
  int this_regno;
  final_start_function (emit_barrier (), file, 1);
  this_regno = (aggregate_value_p (TREE_TYPE (TREE_TYPE (function)), function)
  ? 1
  : 0);
  if (flag_pic)
    {
      fprintf (file, "\tsmw.adm\t$r31, [$r31], $r31, 4\n");
      fprintf (file, "\tsethi\t%s, hi20(_GLOBAL_OFFSET_TABLE_-8)\n",
        reg_names [PIC_OFFSET_TABLE_REGNUM]);
      fprintf (file, "\tori\t%s, %s, lo12(_GLOBAL_OFFSET_TABLE_-4)\n",
        reg_names [PIC_OFFSET_TABLE_REGNUM],
        reg_names [PIC_OFFSET_TABLE_REGNUM]);
      if (TARGET_ISA_V3)
 fprintf (file, "\tadd5.pc\t$gp\n");
      else
 {
   fprintf (file, "\tmfusr\t$ta, $pc\n");
   fprintf (file, "\tadd\t%s, $ta, %s\n",
     reg_names [PIC_OFFSET_TABLE_REGNUM],
     reg_names [PIC_OFFSET_TABLE_REGNUM]);
 }
    }
  if (delta != 0)
    {
      if (satisfies_constraint_Is15 (GEN_INT (delta)))
 {
   fprintf (file, "\taddi\t$r%d, $r%d, " HOST_WIDE_INT_PRINT_DEC "\n",
     this_regno, this_regno, delta);
 }
      else if (satisfies_constraint_Is20 (GEN_INT (delta)))
 {
   fprintf (file, "\tmovi\t$ta, " HOST_WIDE_INT_PRINT_DEC "\n", delta);
   fprintf (file, "\tadd\t$r%d, $r%d, $ta\n", this_regno, this_regno);
 }
      else
 {
   fprintf (file,
     "\tsethi\t$ta, hi20(" HOST_WIDE_INT_PRINT_DEC ")\n",
     delta);
   fprintf (file,
     "\tori\t$ta, $ta, lo12(" HOST_WIDE_INT_PRINT_DEC ")\n",
     delta);
   fprintf (file, "\tadd\t$r%d, $r%d, $ta\n", this_regno, this_regno);
 }
    }
  if (flag_pic)
    {
      fprintf (file, "\tla\t$ta, ");
      assemble_name (file, XSTR (XEXP (DECL_RTL (function), 0), 0));
      fprintf (file, "@PLT\n");
      fprintf (file, "\t! epilogue\n");
      fprintf (file, "\tlwi.bi\t%s, [%s], 4\n",
        reg_names[PIC_OFFSET_TABLE_REGNUM],
        reg_names[STACK_POINTER_REGNUM]);
      fprintf (file, "\tbr\t$ta\n");
    }
  else
    {
      fprintf (file, "\tb\t");
      assemble_name (file, XSTR (XEXP (DECL_RTL (function), 0), 0));
      fprintf (file, "\n");
    }
  final_end_function ();
}
static bool
nds32_function_ok_for_sibcall (tree decl,
          tree exp ATTRIBUTE_UNUSED)
{
  return (!TARGET_V3PUSH
   && !nds32_isr_function_p (current_function_decl)
   && (cfun->machine->va_args_size == 0)
   && decl
   && !flag_pic);
}
static bool
nds32_warn_func_return (tree decl)
{
  return !nds32_naked_function_p (decl);
}
static void
nds32_setup_incoming_varargs (cumulative_args_t ca,
         enum machine_mode mode,
         tree type,
         int *pretend_args_size,
         int second_time ATTRIBUTE_UNUSED)
{
  unsigned int total_args_regs;
  unsigned int num_of_used_regs;
  unsigned int remaining_reg_count;
  CUMULATIVE_ARGS *cum;
  if (TARGET_HARD_FLOAT)
    return;
  cum = get_cumulative_args (ca);
  total_args_regs
    = NDS32_MAX_GPR_REGS_FOR_ARGS + NDS32_GPR_ARG_FIRST_REGNUM;
  num_of_used_regs
    = NDS32_AVAILABLE_REGNUM_FOR_GPR_ARG (cum->gpr_offset, mode, type)
      + NDS32_NEED_N_REGS_FOR_ARG (mode, type);
  remaining_reg_count = total_args_regs - num_of_used_regs;
  *pretend_args_size = remaining_reg_count * UNITS_PER_WORD;
  return;
}
static bool
nds32_strict_argument_naming (cumulative_args_t ca ATTRIBUTE_UNUSED)
{
  return true;
}
static void
nds32_asm_trampoline_template (FILE *f)
{
  if (TARGET_REDUCED_REGS)
    {
      sorry ("a nested function is not supported for reduced registers");
    }
  else
    {
      asm_fprintf (f, "\t! Trampoline code template\n");
      asm_fprintf (f, "\t! This code fragment will be copied "
        "into stack on demand\n");
      asm_fprintf (f, "\tmfusr\t$r16,$pc\n");
      asm_fprintf (f, "\tlwi\t$r15,[$r16 + 20] "
        "! load nested function address\n");
      asm_fprintf (f, "\tlwi\t$r16,[$r16 + 16] "
        "! load chain_value\n");
      asm_fprintf (f, "\tjr\t$r15\n");
    }
  asm_fprintf (f, "\t! space for saving chain_value\n");
  assemble_aligned_integer (UNITS_PER_WORD, const0_rtx);
  asm_fprintf (f, "\t! space for saving nested function address\n");
  assemble_aligned_integer (UNITS_PER_WORD, const0_rtx);
}
static void
nds32_trampoline_init (rtx m_tramp, tree fndecl, rtx chain_value)
{
  int i;
  rtx fnaddr;
  rtx chain_value_mem;
  rtx nested_func_mem;
  rtx sync_cache_addr;
  rtx tmp_reg;
  rtx isync_insn;
  int tramp_align_in_bytes;
  if (TARGET_REDUCED_REGS)
    sorry ("a nested function is not supported for reduced registers");
  fnaddr = XEXP (DECL_RTL (fndecl), 0);
  emit_block_move (m_tramp, assemble_trampoline_template (),
     GEN_INT (TRAMPOLINE_SIZE), BLOCK_OP_NORMAL);
  chain_value_mem = adjust_address (m_tramp, SImode, 16);
  emit_move_insn (chain_value_mem, chain_value);
  nested_func_mem = adjust_address (m_tramp, SImode, 20);
  emit_move_insn (nested_func_mem, fnaddr);
  sync_cache_addr = XEXP (m_tramp, 0);
  tmp_reg = gen_reg_rtx (SImode);
  isync_insn = gen_unspec_volatile_isync (tmp_reg);
  tramp_align_in_bytes = TRAMPOLINE_ALIGNMENT / BITS_PER_UNIT;
  if (tramp_align_in_bytes >= nds32_cache_block_size
      && (tramp_align_in_bytes % nds32_cache_block_size) == 0)
    {
      for (i = 0;
    i < (TRAMPOLINE_SIZE + nds32_cache_block_size - 1)
        / nds32_cache_block_size;
    i++)
 {
   emit_move_insn (tmp_reg,
     plus_constant (Pmode, sync_cache_addr,
      nds32_cache_block_size * i));
   emit_insn (isync_insn);
 }
    }
  else if (TRAMPOLINE_SIZE > nds32_cache_block_size)
    {
      for (i = 0;
    i < (TRAMPOLINE_SIZE + nds32_cache_block_size - 1)
        / nds32_cache_block_size;
    i++)
 {
   emit_move_insn (tmp_reg,
     plus_constant (Pmode, sync_cache_addr,
      nds32_cache_block_size * i));
   emit_insn (isync_insn);
 }
      emit_move_insn (tmp_reg,
        plus_constant (Pmode, sync_cache_addr,
         TRAMPOLINE_SIZE - 4));
      emit_insn (isync_insn);
    }
  else
    {
      emit_move_insn (tmp_reg, sync_cache_addr);
      emit_insn (isync_insn);
      emit_move_insn (tmp_reg,
        plus_constant (Pmode, sync_cache_addr,
         TRAMPOLINE_SIZE - 4));
      emit_insn (isync_insn);
    }
  emit_insn (gen_unspec_volatile_isb ());
}
static bool
nds32_legitimate_address_p (enum machine_mode mode, rtx x, bool strict)
{
  if (TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE)
    {
      if (mode == DFmode
   && (GET_CODE (x) == SYMBOL_REF
   || GET_CODE(x) == CONST))
 return false;
      if (GET_CODE (x) == POST_MODIFY
   && mode == DFmode)
 {
   if (GET_CODE (XEXP (x, 0)) == REG
       && GET_CODE (XEXP (x, 1)) == PLUS)
     {
       rtx plus_op = XEXP (x, 1);
       rtx op0 = XEXP (plus_op, 0);
       rtx op1 = XEXP (plus_op, 1);
       if (nds32_address_register_rtx_p (op0, strict)
    && CONST_INT_P (op1))
  {
    if (satisfies_constraint_Is14 (op1))
      {
        if (!cfun->machine->strict_aligned_p)
   return true;
        else if (NDS32_SINGLE_WORD_ALIGN_P (INTVAL (op1)))
   return true;
      }
  }
     }
 }
    }
  if (mode == DImode || mode == DFmode)
    {
      if (GET_CODE (x) == PLUS)
 {
   if (nds32_address_register_rtx_p (XEXP (x, 0), strict)
       && nds32_legitimate_index_p (mode, XEXP (x, 1), strict)
       && CONST_INT_P (XEXP (x, 1)))
     return true;
   else if (nds32_address_register_rtx_p (XEXP (x, 1), strict)
     && nds32_legitimate_index_p (mode, XEXP (x, 0), strict)
     && CONST_INT_P (XEXP (x, 0)))
     return true;
 }
      if (GET_CODE (x) == POST_INC || GET_CODE (x) == POST_DEC)
 {
   if (nds32_address_register_rtx_p (XEXP (x, 0), strict))
     return true;
 }
      if (GET_CODE (x) != REG
   && GET_CODE (x) != SYMBOL_REF
   && GET_CODE (x) != CONST)
 return false;
    }
  switch (GET_CODE (x))
    {
    case REG:
      return nds32_address_register_rtx_p (x, strict);
    case SYMBOL_REF:
      if (flag_pic || SYMBOL_REF_TLS_MODEL (x))
 return false;
      if (TARGET_ICT_MODEL_LARGE && nds32_indirect_call_referenced_p (x))
 return false;
      if (TARGET_CMODEL_LARGE
   && (reload_completed
       || reload_in_progress
       || lra_in_progress))
 return false;
      if (TARGET_CMODEL_MEDIUM
   && (NDS32_SYMBOL_REF_RODATA_P (x)
       || CONSTANT_POOL_ADDRESS_P (x))
   && (reload_completed
       || reload_in_progress
       || lra_in_progress))
 return false;
      return true;
    case CONST:
      if (GET_CODE (XEXP (x, 0)) == PLUS)
 {
   rtx plus_op = XEXP (x, 0);
   rtx op0 = XEXP (plus_op, 0);
   rtx op1 = XEXP (plus_op, 1);
   if (GET_CODE (op0) == SYMBOL_REF && CONST_INT_P (op1))
     {
       if (flag_pic)
  return false;
       if (TARGET_CMODEL_LARGE
    && (reload_completed
        || reload_in_progress
        || lra_in_progress))
  return false;
       if (TARGET_CMODEL_MEDIUM
    && NDS32_SYMBOL_REF_RODATA_P (op0)
    && (reload_completed
        || reload_in_progress
        || lra_in_progress))
  return false;
       return true;
     }
 }
 return false;
    case POST_MODIFY:
      if (GET_CODE (XEXP (x, 0)) == REG
   && GET_CODE (XEXP (x, 1)) == PLUS)
 {
   rtx plus_op = XEXP (x, 1);
   rtx op0 = XEXP (plus_op, 0);
   rtx op1 = XEXP (plus_op, 1);
   if (nds32_address_register_rtx_p (op0, strict)
       && nds32_legitimate_index_p (mode, op1, strict))
     return true;
   else
     return false;
 }
 return false;
    case POST_INC:
    case POST_DEC:
      if (nds32_address_register_rtx_p (XEXP (x, 0), strict))
 return true;
      else
 return false;
    case PLUS:
      if (nds32_address_register_rtx_p (XEXP (x, 0), strict)
   && nds32_legitimate_index_p (mode, XEXP (x, 1), strict))
 return true;
      else if (nds32_address_register_rtx_p (XEXP (x, 1), strict)
        && nds32_legitimate_index_p (mode, XEXP (x, 0), strict))
 return true;
      else
 return false;
    case LO_SUM:
      {
 rtx sym = NULL_RTX;
 if (flag_pic)
   return false;
 if (!REG_P (XEXP (x, 0)))
   return false;
 if (GET_CODE (XEXP (x, 1)) == SYMBOL_REF)
   sym = XEXP (x, 1);
 else if (GET_CODE (XEXP (x, 1)) == CONST)
   {
     rtx plus = XEXP(XEXP (x, 1), 0);
     if (GET_CODE (plus) == PLUS)
       sym = XEXP (plus, 0);
     else if (GET_CODE (plus) == UNSPEC)
       sym = XEXP(XEXP (plus, 0), 0);
   }
 else
   return false;
 gcc_assert (GET_CODE (sym) == SYMBOL_REF);
 if (TARGET_ICT_MODEL_LARGE
     && nds32_indirect_call_referenced_p (sym))
   return true;
 if (TARGET_CMODEL_LARGE)
   return true;
 else if (TARGET_CMODEL_MEDIUM
   && NDS32_SYMBOL_REF_RODATA_P (sym))
   return true;
 else
   return false;
      }
    default:
      return false;
    }
}
static rtx
nds32_legitimize_address (rtx x,
     rtx oldx ATTRIBUTE_UNUSED,
     enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (nds32_tls_referenced_p (x))
    x = nds32_legitimize_tls_address (x);
  else if (flag_pic && SYMBOLIC_CONST_P (x))
    x = nds32_legitimize_pic_address (x);
  else if (TARGET_ICT_MODEL_LARGE && nds32_indirect_call_referenced_p (x))
    x = nds32_legitimize_ict_address (x);
  return x;
}
static bool
nds32_legitimate_constant_p (enum machine_mode mode, rtx x)
{
  switch (GET_CODE (x))
    {
    case CONST_DOUBLE:
      if ((TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE)
   && (mode == DFmode || mode == SFmode))
 return false;
      break;
    case CONST:
      x = XEXP (x, 0);
      if (GET_CODE (x) == PLUS)
 {
   if (!CONST_INT_P (XEXP (x, 1)))
     return false;
   x = XEXP (x, 0);
 }
      if (GET_CODE (x) == UNSPEC)
 {
   switch (XINT (x, 1))
     {
     case UNSPEC_GOT:
     case UNSPEC_GOTOFF:
     case UNSPEC_PLT:
     case UNSPEC_TLSGD:
     case UNSPEC_TLSLD:
     case UNSPEC_TLSIE:
     case UNSPEC_TLSLE:
     case UNSPEC_ICT:
       return false;
     default:
       return true;
     }
 }
      break;
    case SYMBOL_REF:
      if (SYMBOL_REF_TLS_MODEL (x))
 return false;
      break;
    default:
      return true;
    }
  return true;
}
static rtx
nds32_delegitimize_address (rtx x)
{
  x = delegitimize_mem_from_attrs (x);
  if (GET_CODE(x) == CONST)
    {
      rtx inner = XEXP (x, 0);
      if (GET_CODE (inner) == PLUS)
 inner = XEXP (inner, 0);
      if (GET_CODE (inner) == UNSPEC)
 {
   switch (XINT (inner, 1))
     {
     case UNSPEC_GOTINIT:
     case UNSPEC_GOT:
     case UNSPEC_GOTOFF:
     case UNSPEC_PLT:
     case UNSPEC_TLSGD:
     case UNSPEC_TLSLD:
     case UNSPEC_TLSIE:
     case UNSPEC_TLSLE:
     case UNSPEC_ICT:
       x = XVECEXP (inner, 0, 0);
       break;
     default:
       break;
     }
 }
    }
  return x;
}
static enum machine_mode
nds32_vectorize_preferred_simd_mode (enum machine_mode mode)
{
  if (!NDS32_EXT_DSP_P ())
    return word_mode;
  switch (mode)
    {
    case QImode:
      return V4QImode;
    case HImode:
      return V2HImode;
    default:
      return word_mode;
    }
}
static bool
nds32_cannot_force_const_mem (enum machine_mode mode ATTRIBUTE_UNUSED, rtx x)
{
  switch (GET_CODE (x))
    {
    case CONST:
      return !nds32_legitimate_constant_p (mode, x);
    case SYMBOL_REF:
      if (flag_pic
   || SYMBOL_REF_TLS_MODEL (x)
   || TARGET_CMODEL_SMALL
   || TARGET_CMODEL_MEDIUM)
 return true;
      break;
    case CONST_INT:
    case CONST_DOUBLE:
      if (flag_pic && (lra_in_progress || reload_completed))
 return true;
      break;
    default:
      return false;
    }
  return false;
}
static void
nds32_canonicalize_comparison (int *code,
          rtx *op0 ATTRIBUTE_UNUSED,
          rtx *op1,
          bool op0_preserve_value ATTRIBUTE_UNUSED)
{
  if (*code == LEU && CONST_INT_P (*op1))
    {
      *op1 = gen_int_mode (INTVAL (*op1) + 1, SImode);
      *code = LTU;
    }
}
static int
nds32_register_move_cost (enum machine_mode mode ATTRIBUTE_UNUSED,
     reg_class_t from,
     reg_class_t to)
{
  if ((from == FP_REGS && to != FP_REGS)
      || (from != FP_REGS && to == FP_REGS))
    return 9;
  else if (from == HIGH_REGS || to == HIGH_REGS)
    return optimize_size ? 6 : 2;
  else
    return 2;
}
static int
nds32_memory_move_cost (enum machine_mode mode ATTRIBUTE_UNUSED,
   reg_class_t rclass ATTRIBUTE_UNUSED,
   bool in ATTRIBUTE_UNUSED)
{
  if (nds32_memory_model_option == MEMORY_MODEL_FAST)
    return optimize_size ? 8 : 4;
  else
    return 8;
}
static bool
nds32_rtx_costs (rtx x,
   int code,
   int outer_code,
   int opno,
   int *total,
   bool speed)
{
  return nds32_rtx_costs_impl (x, code, outer_code, opno, total, speed);
}
static int
nds32_address_cost (rtx address,
      enum machine_mode mode,
      addr_space_t as,
      bool speed)
{
  return nds32_address_cost_impl (address, mode, as, speed);
}
static int
nds32_sched_issue_rate (void)
{
  switch (nds32_cpu_option)
  {
  case CPU_GRAYWOLF:
  case CPU_PANTHER:
    return 2;
  default:
    return 1;
  }
}
static int
nds32_sched_adjust_cost (rtx insn ATTRIBUTE_UNUSED,
    rtx link,
    rtx dep ATTRIBUTE_UNUSED,
    int cost)
{
  if (REG_NOTE_KIND (link) == REG_DEP_ANTI
      || REG_NOTE_KIND (link) == REG_DEP_OUTPUT)
    {
      if (nds32_sched_issue_rate () > 1)
        return 1;
      return 0;
    }
  return cost;
}
static void
nds32_encode_section_info (tree decl, rtx rtl, int new_decl_p)
{
  default_encode_section_info (decl, rtl, new_decl_p);
  if (MEM_P (rtl) && MEM_READONLY_P (rtl))
    {
      rtx addr = XEXP (rtl, 0);
      if (GET_CODE (addr) == SYMBOL_REF)
 {
   SYMBOL_REF_FLAGS (addr) |= NDS32_SYMBOL_FLAG_RODATA;
 }
      else if (GET_CODE (addr) == CONST
        && GET_CODE (XEXP (addr, 0)) == PLUS)
 {
   rtx plus_op = XEXP (addr, 0);
   rtx op0 = XEXP (plus_op, 0);
   rtx op1 = XEXP (plus_op, 1);
   if (GET_CODE (op0) == SYMBOL_REF && CONST_INT_P (op1))
     SYMBOL_REF_FLAGS (op0) |= NDS32_SYMBOL_FLAG_RODATA;
 }
    }
}
static void
nds32_asm_file_start (void)
{
  default_file_start ();
  if (flag_pic)
    fprintf (asm_out_file, "\t.pic\n");
  fprintf (asm_out_file, "\t! ABI version\n");
  if (TARGET_HARD_FLOAT)
    fprintf (asm_out_file, "\t.abi_2fp_plus\n");
  else
    fprintf (asm_out_file, "\t.abi_2\n");
  fprintf (asm_out_file, "\t! This asm file is generated by compiler\n");
  fprintf (asm_out_file, "\t.flag\tverbatim\n");
  if (TARGET_ICT_MODEL_LARGE)
    fprintf (asm_out_file, "\t.ict_model\tlarge\n");
  else
    fprintf (asm_out_file, "\t.ict_model\tsmall\n");
  if (!TARGET_LINUX_ABI)
    {
      fprintf (asm_out_file, "\t! This vector size directive is required "
        "for checking inconsistency on interrupt handler\n");
      fprintf (asm_out_file, "\t.vec_size\t%d\n", nds32_isr_vector_size);
    }
  if (!TARGET_LINUX_ABI && (TARGET_FORCE_FP_AS_GP || optimize_size))
    {
      fprintf (asm_out_file, "\t! This weak reference is required to do "
        "fp-as-gp link time optimization\n");
      fprintf (asm_out_file, "\t.weak\t_FP_BASE_\n");
    }
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  if (TARGET_ISA_V2)
    fprintf (asm_out_file, "\t! ISA family\t\t: %s\n", "V2");
  if (TARGET_ISA_V3)
    fprintf (asm_out_file, "\t! ISA family\t\t: %s\n", "V3");
  if (TARGET_ISA_V3M)
    fprintf (asm_out_file, "\t! ISA family\t\t: %s\n", "V3M");
  switch (nds32_cpu_option)
    {
    case CPU_N7:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "N7");
      break;
    case CPU_N8:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "N8");
      break;
    case CPU_N9:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "N9");
      break;
    case CPU_N10:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "N10");
      break;
    case CPU_GRAYWOLF:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "Graywolf");
      break;
    case CPU_N12:
    case CPU_N13:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "N13");
      break;
    case CPU_PANTHER:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "Panther");
      break;
    case CPU_SIMPLE:
      fprintf (asm_out_file, "\t! Pipeline model\t: %s\n", "SIMPLE");
      break;
    default:
      gcc_unreachable ();
    }
  if (TARGET_CMODEL_SMALL)
    fprintf (asm_out_file, "\t! Code model\t\t: %s\n", "SMALL");
  if (TARGET_CMODEL_MEDIUM)
    fprintf (asm_out_file, "\t! Code model\t\t: %s\n", "MEDIUM");
  if (TARGET_CMODEL_LARGE)
    fprintf (asm_out_file, "\t! Code model\t\t: %s\n", "LARGE");
  fprintf (asm_out_file, "\t! Endian setting\t: %s\n",
    ((TARGET_BIG_ENDIAN) ? "big-endian"
           : "little-endian"));
  fprintf (asm_out_file, "\t! Use SP floating-point instruction\t: %s\n",
    ((TARGET_FPU_SINGLE) ? "Yes"
           : "No"));
  fprintf (asm_out_file, "\t! Use DP floating-point instruction\t: %s\n",
    ((TARGET_FPU_DOUBLE) ? "Yes"
           : "No"));
  fprintf (asm_out_file, "\t! ABI version\t\t: %s\n",
    ((TARGET_HARD_FLOAT) ? "ABI2FP+"
           : "ABI2"));
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  fprintf (asm_out_file, "\t! Use conditional move\t\t: %s\n",
    ((TARGET_CMOV) ? "Yes"
     : "No"));
  fprintf (asm_out_file, "\t! Use performance extension\t: %s\n",
    ((TARGET_EXT_PERF) ? "Yes"
         : "No"));
  fprintf (asm_out_file, "\t! Use performance extension 2\t: %s\n",
    ((TARGET_EXT_PERF2) ? "Yes"
          : "No"));
  fprintf (asm_out_file, "\t! Use string extension\t\t: %s\n",
    ((TARGET_EXT_STRING) ? "Yes"
           : "No"));
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  fprintf (asm_out_file, "\t! V3PUSH instructions\t: %s\n",
    ((TARGET_V3PUSH) ? "Yes"
       : "No"));
  fprintf (asm_out_file, "\t! 16-bit instructions\t: %s\n",
    ((TARGET_16_BIT) ? "Yes"
       : "No"));
  fprintf (asm_out_file, "\t! Reduced registers set\t: %s\n",
    ((TARGET_REDUCED_REGS) ? "Yes"
      : "No"));
  fprintf (asm_out_file, "\t! Support unaligned access\t\t: %s\n",
    (flag_unaligned_access ? "Yes"
      : "No"));
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  if (optimize_size)
    fprintf (asm_out_file, "\t! Optimization level\t: -Os\n");
  else if (optimize_fast)
    fprintf (asm_out_file, "\t! Optimization level\t: -Ofast\n");
  else if (optimize_debug)
    fprintf (asm_out_file, "\t! Optimization level\t: -Og\n");
  else
    fprintf (asm_out_file, "\t! Optimization level\t: -O%d\n", optimize);
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  fprintf (asm_out_file, "\t! Cache block size\t: %d\n",
    nds32_cache_block_size);
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  nds32_asm_file_start_for_isr ();
}
static void
nds32_asm_file_end (void)
{
  nds32_asm_file_end_for_isr ();
  fprintf (asm_out_file, "\t! ------------------------------------\n");
}
static bool
nds32_asm_output_addr_const_extra (FILE *file, rtx x)
{
  if (GET_CODE (x) == UNSPEC)
    {
      switch (XINT (x, 1))
 {
 case UNSPEC_GOTINIT:
   output_addr_const (file, XVECEXP (x, 0, 0));
   break;
 case UNSPEC_GOTOFF:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@GOTOFF", file);
   break;
 case UNSPEC_GOT:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@GOT", file);
   break;
 case UNSPEC_PLT:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@PLT", file);
   break;
 case UNSPEC_TLSGD:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@TLSDESC", file);
   break;
 case UNSPEC_TLSLD:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@TLSDESC", file);
   break;
 case UNSPEC_TLSIE:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@GOTTPOFF", file);
   break;
 case UNSPEC_TLSLE:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@TPOFF", file);
   break;
 case UNSPEC_ICT:
   output_addr_const (file, XVECEXP (x, 0, 0));
   fputs ("@ICT", file);
   break;
 default:
   return false;
 }
      return true;
    }
  else
    return false;
}
static void
nds32_asm_globalize_label (FILE *stream, const char *name)
{
  fputs ("\t.global\t", stream);
  assemble_name (stream, name);
  fputs ("\n", stream);
}
static void
nds32_print_operand (FILE *stream, rtx x, int code)
{
  HOST_WIDE_INT op_value = 0;
  HOST_WIDE_INT one_position;
  HOST_WIDE_INT zero_position;
  bool pick_lsb_p = false;
  bool pick_msb_p = false;
  int regno;
  if (CONST_INT_P (x))
    op_value = INTVAL (x);
  switch (code)
    {
    case 0 :
      break;
    case 'b':
      gcc_assert (CONST_INT_P (x));
      zero_position = exact_log2 (~UINTVAL (x) & GET_MODE_MASK (SImode));
      gcc_assert (zero_position != -1);
      fprintf (stream, HOST_WIDE_INT_PRINT_DEC, zero_position);
      return;
    case 'e':
      gcc_assert (MEM_P (x)
    && GET_CODE (XEXP (x, 0)) == PLUS
    && GET_CODE (XEXP (XEXP (x, 0), 1)) == CONST_INT);
      fprintf (stream, HOST_WIDE_INT_PRINT_DEC, INTVAL (XEXP (XEXP (x, 0), 1)));
      return;
    case 'v':
      gcc_assert (CONST_INT_P (x)
    && (INTVAL (x) == 0
        || INTVAL (x) == 8
        || INTVAL (x) == 16
        || INTVAL (x) == 24));
      fprintf (stream, HOST_WIDE_INT_PRINT_DEC, INTVAL (x) / 8);
      return;
    case 'B':
      gcc_assert (CONST_INT_P (x));
      one_position = exact_log2 (UINTVAL (x) & GET_MODE_MASK (SImode));
      gcc_assert (one_position != -1);
      fprintf (stream, HOST_WIDE_INT_PRINT_DEC, one_position);
      return;
    case 'L':
      gcc_assert (REG_P (x));
      pick_lsb_p = true;
      break;
    case 'H':
      gcc_assert (REG_P (x));
      pick_msb_p = true;
      break;
    case 'V':
      gcc_assert (CONST_INT_P (x));
      if (op_value < 1024 && op_value >= 0)
 {
   fprintf (stream, HOST_WIDE_INT_PRINT_DEC, op_value);
 }
      else if (op_value < 0
        || op_value >= ((int) ARRAY_SIZE (nds32_intrinsic_register_names)
          + 1024))
 {
   error ("intrinsic register index is out of range");
 }
      else
 {
   fprintf (stream, "%s",
      nds32_intrinsic_register_names[op_value - 1024]);
 }
      return;
    case 'R':
      if (op_value < 0 || op_value > 4)
 error ("CCTL intrinsic function subtype out of range!");
      fprintf (stream, "%s", nds32_cctl_names[op_value]);
      return;
    case 'T':
      if (op_value < 0 || op_value > 4)
 error ("CCTL intrinsic function subtype out of range!");
      fprintf (stream, "%s", nds32_cctl_names[op_value + 4]);
      return;
    case 'U':
      if (op_value < 0 || op_value > 4)
 error ("CCTL intrinsic function subtype out of range!");
      fprintf (stream, "%s", nds32_cctl_names[op_value + 8]);
      return;
    case 'X':
      if (op_value < 0 || op_value > 4)
 error ("CCTL intrinsic function subtype out of range!");
      fprintf (stream, "%s", nds32_cctl_names[op_value + 12]);
      return;
    case 'W':
      if (op_value < 0 || op_value > 4)
 error ("CCTL intrinsic function subtype out of range!");
      fprintf (stream, "%s", nds32_cctl_names[op_value + 16]);
      return;
    case 'Z':
      fprintf (stream, "%s", nds32_dpref_names[op_value]);
      return;
    default :
      output_operand_lossage ("invalid operand output code");
      break;
    }
  switch (GET_CODE (x))
    {
    case LABEL_REF:
      output_addr_const (stream, x);
      break;
    case SYMBOL_REF:
      output_addr_const (stream, x);
      if (!TARGET_LINUX_ABI && nds32_indirect_call_referenced_p (x))
 fprintf (stream, "@ICT");
      break;
    case REG:
      if ((GET_MODE (x) == DImode || GET_MODE (x) == DFmode)
   && NDS32_IS_FPR_REGNUM (REGNO (x)))
 {
   regno = REGNO (x);
   if (!NDS32_FPR_REGNO_OK_FOR_DOUBLE (regno))
     {
       output_operand_lossage ("invalid operand for code '%c'", code);
       break;
     }
   fprintf (stream, "$fd%d", (regno - NDS32_FIRST_FPR_REGNUM) >> 1);
   break;
 }
      if ((GET_MODE (x) == DImode || GET_MODE (x) == DFmode)
   && NDS32_IS_GPR_REGNUM (REGNO (x)))
 {
   if ((pick_lsb_p && WORDS_BIG_ENDIAN)
       || (pick_msb_p && !WORDS_BIG_ENDIAN))
     {
       regno = REGNO (x);
       regno++;
       fputs (reg_names[regno], stream);
       break;
     }
 }
      if (TARGET_REDUCED_REGS
   && REGNO (x) == STATIC_CHAIN_REGNUM)
 sorry ("a nested function is not supported for reduced registers");
      regno = REGNO (x);
      fputs (reg_names[regno], stream);
      break;
    case MEM:
      output_address (XEXP (x, 0));
      break;
    case HIGH:
      if (GET_CODE (XEXP (x, 0)) == CONST_DOUBLE)
 {
   REAL_VALUE_TYPE rv;
   long val;
   gcc_assert (GET_MODE (x) == SFmode);
   REAL_VALUE_FROM_CONST_DOUBLE (rv, XEXP (x, 0));
   REAL_VALUE_TO_TARGET_SINGLE (rv, val);
   fprintf (stream, "hi20(0x%lx)", val);
 }
      else
 gcc_unreachable ();
      break;
    case CONST_DOUBLE:
      REAL_VALUE_TYPE rv;
      long val;
      gcc_assert (GET_MODE (x) == SFmode);
      REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
      REAL_VALUE_TO_TARGET_SINGLE (rv, val);
      fprintf (stream, "0x%lx", val);
      break;
    case CODE_LABEL:
    case CONST_INT:
    case CONST:
      output_addr_const (stream, x);
      break;
    case CONST_VECTOR:
      fprintf (stream, HOST_WIDE_INT_PRINT_HEX, const_vector_to_hwint (x));
      break;
    case LO_SUM:
      output_addr_const (stream, XEXP(x, 1));
      break;
    default:
      debug_rtx (x);
      gcc_unreachable ();
      break;
    }
}
static void
nds32_print_operand_address (FILE *stream, rtx x)
{
  rtx op0, op1;
  switch (GET_CODE (x))
    {
    case SYMBOL_REF:
    case CONST:
      fputs ("[ + ", stream);
      output_addr_const (stream, x);
      fputs ("]", stream);
      break;
    case LO_SUM:
      fputs ("[ + ", stream);
      op1 = XEXP (x, 1);
      output_addr_const (stream, op1);
      fputs ("]", stream);
      break;
    case REG:
      if (TARGET_REDUCED_REGS
   && REGNO (x) == STATIC_CHAIN_REGNUM)
 sorry ("a nested function is not supported for reduced registers");
      fprintf (stream, "[%s + 0]", reg_names[REGNO (x)]);
      break;
    case PLUS:
      op0 = XEXP (x, 0);
      op1 = XEXP (x, 1);
      if (TARGET_REDUCED_REGS
   && REG_P (op0)
   && REGNO (op0) == STATIC_CHAIN_REGNUM)
 sorry ("a nested function is not supported for reduced registers");
      if (TARGET_REDUCED_REGS
   && REG_P (op1)
   && REGNO (op1) == STATIC_CHAIN_REGNUM)
 sorry ("a nested function is not supported for reduced registers");
      if (REG_P (op0) && CONST_INT_P (op1))
 {
   fprintf (stream, "[%s + (" HOST_WIDE_INT_PRINT_DEC ")]",
      reg_names[REGNO (op0)], INTVAL (op1));
 }
      else if (REG_P (op0) && REG_P (op1))
 {
   fprintf (stream, "[%s + %s]",
      reg_names[REGNO (op0)], reg_names[REGNO (op1)]);
 }
      else if (GET_CODE (op0) == MULT && REG_P (op1))
 {
   int sv;
   if (INTVAL (XEXP (op0, 1)) == 1)
     sv = 0;
   else if (INTVAL (XEXP (op0, 1)) == 2)
     sv = 1;
   else if (INTVAL (XEXP (op0, 1)) == 4)
     sv = 2;
   else if (INTVAL (XEXP (op0, 1)) == 8)
     sv = 3;
   else
     gcc_unreachable ();
   fprintf (stream, "[%s + %s << %d]",
      reg_names[REGNO (op1)],
      reg_names[REGNO (XEXP (op0, 0))],
      sv);
 }
      else
 {
   debug_rtx (x);
   gcc_unreachable ();
 }
      break;
    case POST_MODIFY:
      op0 = XEXP (XEXP (x, 1), 0);
      op1 = XEXP (XEXP (x, 1), 1);
      if (TARGET_REDUCED_REGS
   && REG_P (op0)
   && REGNO (op0) == STATIC_CHAIN_REGNUM)
 sorry ("a nested function is not supported for reduced registers");
      if (TARGET_REDUCED_REGS
   && REG_P (op1)
   && REGNO (op1) == STATIC_CHAIN_REGNUM)
 sorry ("a nested function is not supported for reduced registers");
      if (REG_P (op0) && REG_P (op1))
 {
   fprintf (stream, "[%s], %s",
      reg_names[REGNO (op0)], reg_names[REGNO (op1)]);
 }
      else if (REG_P (op0) && CONST_INT_P (op1))
 {
   fprintf (stream, "[%s], " HOST_WIDE_INT_PRINT_DEC,
      reg_names[REGNO (op0)], INTVAL (op1));
 }
      else
 {
   debug_rtx (x);
   gcc_unreachable ();
 }
      break;
    case POST_INC:
    case POST_DEC:
      op0 = XEXP (x, 0);
      if (TARGET_REDUCED_REGS
   && REG_P (op0)
   && REGNO (op0) == STATIC_CHAIN_REGNUM)
 sorry ("a nested function is not supported for reduced registers");
      if (REG_P (op0))
 {
   fprintf (stream, "[%s]", reg_names[REGNO (op0)]);
 }
      else
 {
   debug_rtx (x);
   gcc_unreachable ();
 }
      break;
    default :
      debug_rtx (x);
      gcc_unreachable ();
      break;
    }
}
static rtx
nds32_dwarf_register_span (rtx reg)
{
  rtx dwarf_high, dwarf_low;
  rtx dwarf_single;
  enum machine_mode mode;
  int regno;
  mode = GET_MODE (reg);
  regno = REGNO (reg);
  if (regno >= NDS32_FIRST_FPR_REGNUM
      && regno <= NDS32_LAST_FPR_REGNUM)
    {
      regno += 4;
      if (mode == DFmode || mode == SCmode)
 {
   dwarf_high = gen_rtx_REG (word_mode, regno);
   dwarf_low = gen_rtx_REG (word_mode, regno + 1);
   return gen_rtx_PARALLEL (VOIDmode,
       gen_rtvec (2, dwarf_low, dwarf_high));
 }
      else if (mode == DCmode)
 {
   rtx dwarf_high_re = gen_rtx_REG (word_mode, regno);
   rtx dwarf_low_re = gen_rtx_REG (word_mode, regno + 1);
   rtx dwarf_high_im = gen_rtx_REG (word_mode, regno);
   rtx dwarf_low_im = gen_rtx_REG (word_mode, regno + 1);
   return gen_rtx_PARALLEL (VOIDmode,
       gen_rtvec (4, dwarf_low_re, dwarf_high_re,
       dwarf_high_im, dwarf_low_im));
 }
      else if (mode == SFmode || mode == SImode)
 {
   dwarf_single = gen_rtx_REG (word_mode, regno);
   return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (1, dwarf_single));
 }
      else
 {
   gcc_unreachable ();
 }
    }
  return NULL_RTX;
}
static tree
nds32_merge_decl_attributes (tree olddecl, tree newdecl)
{
  tree combined_attrs;
  combined_attrs = merge_attributes (DECL_ATTRIBUTES (olddecl),
         DECL_ATTRIBUTES (newdecl));
  if (TREE_CODE (olddecl) == FUNCTION_DECL)
    {
      nds32_check_isr_attrs_conflict (olddecl, combined_attrs);
    }
  return combined_attrs;
}
static void
nds32_insert_attributes (tree decl, tree *attributes)
{
  if (TREE_CODE (decl) == FUNCTION_DECL
      && lookup_attribute ("indirect_call", *attributes) != NULL)
    {
      tree new_attrs = *attributes;
      if (TARGET_LINUX_ABI)
 error("cannot use indirect_call attribute under linux toolchain");
      if (lookup_attribute ("noinline", new_attrs) == NULL)
 new_attrs = tree_cons (get_identifier ("noinline"), NULL, new_attrs);
      if (lookup_attribute ("noclone", new_attrs) == NULL)
 new_attrs = tree_cons (get_identifier ("noclone"), NULL, new_attrs);
      if (!TREE_PUBLIC (decl))
 error("indirect_call attribute can't apply for static function");
      *attributes = new_attrs;
    }
  if (TREE_CODE (decl) == FUNCTION_DECL)
    {
      tree func_attrs;
      tree intr, excp, reset;
      func_attrs = *attributes;
      nds32_check_isr_attrs_conflict (decl, func_attrs);
      intr = lookup_attribute ("interrupt", func_attrs);
      excp = lookup_attribute ("exception", func_attrs);
      reset = lookup_attribute ("reset", func_attrs);
      if ((intr && TREE_VALUE (intr) == NULL)
   || (excp && TREE_VALUE (excp) == NULL)
   || (reset && TREE_VALUE (reset) == NULL))
 return;
      if (intr != NULL_TREE
   && TREE_CODE (TREE_VALUE (TREE_VALUE (intr))) == STRING_CST)
 return;
      if (excp != NULL_TREE
   && TREE_CODE (TREE_VALUE (TREE_VALUE (excp))) == STRING_CST)
 return;
      if (reset != NULL_TREE
   && TREE_CODE (TREE_VALUE (TREE_VALUE (reset))) == STRING_CST)
 return;
      if (intr || excp)
 {
   tree id_list;
   unsigned int lower_bound, upper_bound;
   lower_bound = (intr) ? (0) : (1);
   upper_bound = (intr) ? (63) : (8);
   id_list = (intr) ? (TREE_VALUE (intr)) : (TREE_VALUE (excp));
   while (id_list)
     {
       tree id;
       id = TREE_VALUE (id_list);
       if (TREE_CODE (id) != INTEGER_CST
    || TREE_INT_CST_LOW (id) < lower_bound
    || TREE_INT_CST_LOW (id) > upper_bound)
  error ("invalid id value for interrupt/exception attribute");
       id_list = TREE_CHAIN (id_list);
     }
 }
      else if (reset)
 {
   tree id_list;
   tree id;
   tree nmi, warm;
   unsigned int lower_bound;
   unsigned int upper_bound;
   id_list = TREE_VALUE (reset);
   id = TREE_VALUE (id_list);
   lower_bound = 0;
   upper_bound = 64;
   if (TREE_CODE (id) != INTEGER_CST
       || TREE_INT_CST_LOW (id) < lower_bound
       || TREE_INT_CST_LOW (id) > upper_bound)
     error ("invalid id value for reset attribute");
   nmi = lookup_attribute ("nmi", func_attrs);
   warm = lookup_attribute ("warm", func_attrs);
   if (nmi != NULL_TREE)
     {
       tree nmi_func_list;
       tree nmi_func;
       nmi_func_list = TREE_VALUE (nmi);
       nmi_func = TREE_VALUE (nmi_func_list);
       if (TREE_CODE (nmi_func) != IDENTIFIER_NODE)
  error ("invalid nmi function for reset attribute");
     }
   if (warm != NULL_TREE)
     {
       tree warm_func_list;
       tree warm_func;
       warm_func_list = TREE_VALUE (warm);
       warm_func = TREE_VALUE (warm_func_list);
       if (TREE_CODE (warm_func) != IDENTIFIER_NODE)
  error ("invalid warm function for reset attribute");
     }
 }
      else
 {
   return;
 }
    }
}
static bool
nds32_option_pragma_parse (tree args ATTRIBUTE_UNUSED,
      tree pop_target ATTRIBUTE_UNUSED)
{
  return false;
}
static void
nds32_option_override (void)
{
  if (TARGET_ISA_V2)
    {
      target_flags &= ~MASK_V3PUSH;
      target_flags &= ~MASK_IFC;
      target_flags &= ~MASK_EX9;
      if (nds32_arch_option == ARCH_V2J)
 target_flags |= MASK_REDUCED_REGS;
    }
  if (TARGET_ISA_V3)
    {
      if (nds32_arch_option == ARCH_V3J)
 target_flags |= MASK_REDUCED_REGS;
    }
  if (TARGET_ISA_V3M)
    {
      target_flags |= MASK_REDUCED_REGS;
      if (nds32_arch_option != ARCH_V3M_PLUS)
 {
   target_flags &= ~MASK_IFC;
   target_flags &= ~MASK_EX9;
 }
      target_flags &= ~MASK_EXT_PERF;
      target_flags &= ~MASK_EXT_PERF2;
      target_flags &= ~MASK_EXT_STRING;
      if (flag_pic)
 error ("not support -fpic option for v3m toolchain");
    }
  if (TARGET_REDUCED_REGS)
    {
      int r;
      for (r = 11; r <= 14; r++)
 fixed_regs[r] = call_used_regs[r] = 1;
      for (r = 16; r <= 27; r++)
 fixed_regs[r] = call_used_regs[r] = 1;
    }
  if (TARGET_FORCE_FP_AS_GP)
    fixed_regs[FP_REGNUM] = call_used_regs[FP_REGNUM] = 1;
  if (!TARGET_16_BIT)
    {
      target_flags &= ~MASK_V3PUSH;
    }
  if (TARGET_HARD_FLOAT && !(TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE))
    {
      if (nds32_arch_option == ARCH_V3S || nds32_arch_option == ARCH_V3F)
 error ("Disable FPU ISA, "
        "the ABI option must be enable '-mfloat-abi=soft'");
      else
 error ("'-mabi=2fp+' option only support when FPU available, "
        "must be enable '-mext-fpu-sp' or '-mext-fpu-dp'");
    }
  nds32_register_passes ();
  nds32_init_rtx_costs ();
  if (global_options.x_flag_tree_switch_shortcut)
    {
      maybe_set_param_value
 (PARAM_MAX_AVERAGE_UNROLLED_INSNS,
  200,
  global_options.x_param_values, global_options_set.x_param_values);
      maybe_set_param_value
 (PARAM_MAX_GROW_COPY_BB_INSNS,
  16,
  global_options.x_param_values, global_options_set.x_param_values);
    }
}
static tree
nds32_md_asm_clobbers (tree outputs ATTRIBUTE_UNUSED,
         tree inputs ATTRIBUTE_UNUSED,
         tree clobbers)
{
  clobbers = tree_cons (NULL_TREE, build_string (3, "$ta"),
   clobbers);
  return clobbers;
}
static bool
nds32_hwloop_insert_end_label (rtx loop_id, rtx end_label)
{
  rtx insn = NULL_RTX;
  basic_block bb;
  rtx cfg_id, last_insn;
  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS (bb, insn)
 {
   if (NOTE_P (insn))
     continue;
   if (recog_memoized (insn) == CODE_FOR_hwloop_cfg
       && INSN_P (insn))
     {
       cfg_id = XVECEXP (XVECEXP (PATTERN (insn), 0, 5), 0, 0);
       if (cfg_id == loop_id)
  {
    for (last_insn = PREV_INSN (insn); last_insn != BB_HEAD (bb);
         last_insn = PREV_INSN (last_insn))
      {
        if (NONDEBUG_INSN_P (last_insn))
   {
     emit_label_before (end_label, last_insn);
     return true;
   }
      }
    if (NOTE_INSN_BASIC_BLOCK_P (last_insn))
      {
        rtx nop = emit_insn_before (gen_unspec_nop (), last_insn);
        emit_label_before (end_label, nop);
        return true;
      }
  }
     }
 }
    }
  if (insn != NULL_RTX)
    delete_insn (insn);
  return false;
}
static void
nds32_hwloop_remove (rtx loop_id)
{
  rtx insn, le_id;
  basic_block bb;
  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS (bb, insn)
 {
   if (NOTE_P (insn))
     continue;
   if (recog_memoized (insn) == CODE_FOR_init_lc
       && INSN_P (insn))
     {
       le_id = XVECEXP (XVECEXP (PATTERN (insn), 0, 1), 0, 0);
       if (loop_id == le_id)
  {
    delete_insn (insn);
    return;
  }
     }
 }
    }
}
static void
nds32_hwloop_insert_isb (rtx loop_id)
{
  rtx insn, le_id;
  basic_block bb;
  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS (bb, insn)
 {
   if (NOTE_P (insn))
     continue;
   if (recog_memoized (insn) == CODE_FOR_init_lc
       && INSN_P (insn))
     {
       le_id = XVECEXP (XVECEXP (PATTERN (insn), 0, 1), 0, 0);
       if (loop_id == le_id)
  {
    emit_insn_after (gen_unspec_volatile_isb (), insn);
    return;
  }
     }
 }
    }
}
static void
nds32_hwloop_insert_init_end ()
{
  rtx insn;
  basic_block bb;
  rtx loop_id, end_label;
  bool hwloop_p;
  FOR_EACH_BB_FN (bb, cfun)
    {
      FOR_BB_INSNS (bb, insn)
 {
   if (NOTE_P (insn))
     continue;
   if (recog_memoized (insn) == CODE_FOR_mtlbi_hint
       && INSN_P (insn))
     {
       end_label = gen_label_rtx ();
       loop_id = XVECEXP (XVECEXP (PATTERN (insn), 0, 1), 0, 0);
       hwloop_p = nds32_hwloop_insert_end_label (loop_id, end_label);
       if (!hwloop_p)
  {
    delete_insn (insn);
    nds32_hwloop_remove (loop_id);
  }
       else
  {
    emit_insn_after (gen_mtlei (gen_rtx_LABEL_REF (Pmode, end_label)), insn);
    nds32_hwloop_insert_isb (loop_id);
  }
     }
 }
    }
}
static void
nds32_machine_dependent_reorg (void)
{
  compute_bb_for_insn ();
  df_analyze ();
  if (TARGET_HWLOOP)
    nds32_hwloop_insert_init_end ();
  if (flag_var_tracking)
    {
      timevar_push (TV_VAR_TRACKING);
      variable_tracking_main ();
      timevar_pop (TV_VAR_TRACKING);
      df_finish_pass (false);
    }
  if (TARGET_INNERMOST_LOOP)
    nds32_insert_innermost_loop ();
  nds32_insert_isps ();
}
static void
nds32_init_builtins (void)
{
  nds32_init_builtins_impl ();
}
static tree
nds32_builtin_decl (unsigned code, bool initialize_p)
{
  return nds32_builtin_decl_impl (code, initialize_p);
}
static rtx
nds32_expand_builtin (tree exp,
        rtx target,
        rtx subtarget,
        enum machine_mode mode,
        int ignore)
{
  return nds32_expand_builtin_impl (exp, target, subtarget, mode, ignore);
}
static bool
nds32_have_conditional_execution (void)
{
  return optimize_size;
}
static void
nds32_init_libfuncs (void)
{
  if (TARGET_LINUX_ABI)
    init_sync_libfuncs (UNITS_PER_WORD);
}
static bool
nds32_can_use_doloop_p (double_int, double_int iterations_max,
   unsigned int, bool entered_at_top)
{
  if (!entered_at_top)
    return false;
  if (lookup_attribute ("no_ext_zol", DECL_ATTRIBUTES (current_function_decl)))
    return false;
  if (!NDS32_HW_LOOP_P ()
      || iterations_max.low < 8)
    return false;
  return true;
}
static const char *
nds32_invalid_within_doloop (const_rtx insn)
{
  if (CALL_P (insn))
    return "Function call in the loop.";
  else if (INSN_CODE (insn) == CODE_FOR_pop25return
    || INSN_CODE (insn) == CODE_FOR_return_internal)
    return "Simple return in the loop.";
  return NULL;
}
void
nds32_cpu_cpp_builtins(struct cpp_reader *pfile)
{
#define builtin_define(TXT) cpp_define (pfile, TXT)
#define builtin_assert(TXT) cpp_assert (pfile, TXT)
  builtin_define ("__nds32__");
  builtin_define ("__NDS32__");
  if (!TARGET_LINUX_ABI)
    {
      if (TARGET_ISR_VECTOR_SIZE_4_BYTE)
 builtin_define ("__NDS32_ISR_VECTOR_SIZE_4__");
      else
 builtin_define ("__NDS32_ISR_VECTOR_SIZE_16__");
    }
  if (TARGET_HARD_FLOAT)
    builtin_define ("__NDS32_ABI_2FP_PLUS__");
  else
    builtin_define ("__NDS32_ABI_2__");
  if (TARGET_ISA_V2)
    builtin_define ("__NDS32_ISA_V2__");
  if (TARGET_ISA_V3)
    builtin_define ("__NDS32_ISA_V3__");
  if (TARGET_ISA_V3M)
    builtin_define ("__NDS32_ISA_V3M__");
  if (TARGET_FPU_SINGLE)
    builtin_define ("__NDS32_EXT_FPU_SP__");
  if (TARGET_FPU_DOUBLE)
    builtin_define ("__NDS32_EXT_FPU_DP__");
  if (TARGET_EXT_FPU_FMA)
    builtin_define ("__NDS32_EXT_FPU_FMA__");
  if (NDS32_EXT_FPU_DOT_E)
    builtin_define ("__NDS32_EXT_FPU_DOT_E__");
  if (TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE)
    {
      switch (nds32_fp_regnum)
 {
 case 0:
 case 4:
   builtin_define ("__NDS32_EXT_FPU_CONFIG_0__");
   break;
 case 1:
 case 5:
   builtin_define ("__NDS32_EXT_FPU_CONFIG_1__");
   break;
 case 2:
 case 6:
   builtin_define ("__NDS32_EXT_FPU_CONFIG_2__");
   break;
 case 3:
 case 7:
   builtin_define ("__NDS32_EXT_FPU_CONFIG_3__");
   break;
 default:
   abort ();
 }
    }
  if (TARGET_BIG_ENDIAN)
    builtin_define ("__NDS32_EB__");
  else
    builtin_define ("__NDS32_EL__");
  if (TARGET_REDUCED_REGS)
    builtin_define ("__NDS32_REDUCED_REGS__");
  if (TARGET_CMOV)
    builtin_define ("__NDS32_CMOV__");
  if (TARGET_EXT_PERF)
    builtin_define ("__NDS32_EXT_PERF__");
  if (TARGET_EXT_PERF2)
    builtin_define ("__NDS32_EXT_PERF2__");
  if (TARGET_EXT_STRING)
    builtin_define ("__NDS32_EXT_STRING__");
  if (TARGET_16_BIT)
    builtin_define ("__NDS32_16_BIT__");
  if (TARGET_GP_DIRECT)
    builtin_define ("__NDS32_GP_DIRECT__");
  if (TARGET_VH)
    builtin_define ("__NDS32_VH__");
  if (NDS32_EXT_DSP_P ())
    builtin_define ("__NDS32_EXT_DSP__");
  if (NDS32_HW_LOOP_P ())
    builtin_define ("__NDS32_EXT_ZOL__");
  if (TARGET_ISA_V3 || TARGET_ISA_V3M_PLUS)
    builtin_define ("__NDS32_EXT_IFC__");
  if (TARGET_ISA_V3 || TARGET_ISA_V3M_PLUS)
    builtin_define ("__NDS32_EXT_EX9__");
  if (TARGET_BIG_ENDIAN)
    builtin_define ("__big_endian__");
  builtin_assert ("cpu=nds32");
  builtin_assert ("machine=nds32");
  if (TARGET_ISA_V2)
    builtin_define ("__NDS32_BASELINE_V2__");
  if (TARGET_ISA_V3)
    builtin_define ("__NDS32_BASELINE_V3__");
  if (TARGET_ISA_V3M)
    builtin_define ("__NDS32_BASELINE_V3M__");
  if (TARGET_REDUCED_REGS)
    builtin_define ("__NDS32_REDUCE_REGS__");
  if (TARGET_ISA_V2)
    builtin_define ("NDS32_BASELINE_V2");
  if (TARGET_ISA_V3)
    builtin_define ("NDS32_BASELINE_V3");
  if (TARGET_ISA_V3M)
    builtin_define ("NDS32_BASELINE_V3M");
  if (TARGET_REDUCED_REGS)
    builtin_define ("NDS32_REDUCE_REGS");
  if (TARGET_FPU_SINGLE)
    builtin_define ("NDS32_EXT_FPU_SP");
  if (TARGET_FPU_DOUBLE)
    builtin_define ("NDS32_EXT_FPU_DP");
  if (TARGET_EXT_PERF)
    builtin_define ("NDS32_EXT_PERF");
  if (TARGET_EXT_PERF2)
    builtin_define ("NDS32_EXT_PERF2");
  if (TARGET_EXT_STRING)
    builtin_define ("NDS32_EXT_STRING");
  if (TARGET_ISA_V3)
    builtin_define ("NDS32_EXT_IFC");
  if (TARGET_ISA_V3)
    builtin_define ("NDS32_EXT_EX9");
  if (TARGET_HARD_FLOAT)
    builtin_define ("NDS32_ABI_2FP_PLUS");
  else
    builtin_define ("NDS32_ABI_2");
  if (TARGET_BIG_ENDIAN)
    builtin_define ("NDS32_EB");
  else
    builtin_define ("NDS32_EL");
  if (TARGET_ISA_V2)
    builtin_define ("__NDS32_BASELINE_V2");
  if (TARGET_ISA_V3)
    builtin_define ("__NDS32_BASELINE_V3");
  if (TARGET_ISA_V3M)
    builtin_define ("__NDS32_BASELINE_V3M");
  if (TARGET_REDUCED_REGS)
    builtin_define ("__NDS32_REDUCE_REGS");
  if (TARGET_FPU_SINGLE)
    builtin_define ("__NDS32_EXT_FPU_SP");
  if (TARGET_FPU_DOUBLE)
    builtin_define ("__NDS32_EXT_FPU_DP");
  if (TARGET_EXT_PERF)
    builtin_define ("__NDS32_EXT_PERF");
  if (TARGET_EXT_PERF2)
    builtin_define ("__NDS32_EXT_PERF2");
  if (TARGET_EXT_STRING)
    builtin_define ("__NDS32_EXT_STRING");
  if (TARGET_ISA_V3)
    builtin_define ("__NDS32_EXT_IFC");
  if (TARGET_ISA_V3)
    builtin_define ("__NDS32_EXT_EX9");
  if (TARGET_HARD_FLOAT)
    builtin_define ("__NDS32_ABI_2FP_PLUS");
  else
    builtin_define ("__NDS32_ABI_2");
  if (TARGET_BIG_ENDIAN)
    builtin_define ("__NDS32_EB");
  else
    builtin_define ("__NDS32_EL");
#undef builtin_define
#undef builtin_assert
}
void
nds32_init_expanders (void)
{
  init_machine_status = nds32_init_machine_status;
}
void
nds32_adjust_reg_alloc_order (void)
{
  const int nds32_reg_alloc_order[] = REG_ALLOC_ORDER;
  memcpy(reg_alloc_order, nds32_reg_alloc_order, sizeof (reg_alloc_order));
  if (!optimize_size)
    {
      memcpy (reg_alloc_order, nds32_reg_alloc_order_for_speed,
       sizeof (nds32_reg_alloc_order_for_speed));
    }
}
int
nds32_hard_regno_nregs (int regno ATTRIBUTE_UNUSED,
   enum machine_mode mode)
{
  return ((GET_MODE_SIZE (mode) + UNITS_PER_WORD - 1) / UNITS_PER_WORD);
}
int
nds32_hard_regno_mode_ok (int regno, enum machine_mode mode)
{
  if (regno > FIRST_PSEUDO_REGISTER)
    return true;
  if ((TARGET_FPU_SINGLE || TARGET_FPU_DOUBLE) && NDS32_IS_FPR_REGNUM (regno))
    {
      if (NDS32_IS_EXT_FPR_REGNUM(regno))
 return (NDS32_FPR_REGNO_OK_FOR_DOUBLE(regno) && (mode == DFmode));
      else if (mode == SFmode || mode == SImode)
 return NDS32_FPR_REGNO_OK_FOR_SINGLE (regno);
      else if (mode == DFmode)
 return NDS32_FPR_REGNO_OK_FOR_DOUBLE (regno);
      return false;
    }
  if (regno <= NDS32_LAST_GPR_REGNUM)
    return (HARD_REGNO_NREGS (regno, mode) == 1
     || !((regno) & 1));
  return false;
}
enum reg_class
nds32_regno_reg_class (int regno)
{
  if (regno >= 0 && regno <= 7)
    return LOW_REGS;
  else if (regno >= 8 && regno <= 11)
    return MIDDLE_REGS;
  else if (regno >= 12 && regno <= 14)
    return HIGH_REGS;
  else if (regno == 15)
    return R15_TA_REG;
  else if (regno >= 16 && regno <= 19)
    return MIDDLE_REGS;
  else if (regno >= 20 && regno <= 31)
    return HIGH_REGS;
  else if (regno == 32 || regno == 33)
    {
      return GENERAL_REGS;
    }
  else if (regno >= 34 && regno <= 97)
    return FP_REGS;
  else if (regno >= 98 && regno <= 100)
    return LOOP_REGS;
  else
    return NO_REGS;
}
rtx
nds32_dynamic_chain_address (rtx frameaddr)
{
  if (TARGET_V3PUSH)
    {
      return plus_constant (Pmode, frameaddr, -12);
    }
  else
    {
      return plus_constant (Pmode, frameaddr, -8);
    }
}
rtx
nds32_return_addr_rtx (int count,
         rtx frameaddr)
{
  int offset;
  rtx addr;
  if (count != 0)
    {
      offset = -4;
      addr = plus_constant (Pmode, frameaddr, offset);
      addr = memory_address (Pmode, addr);
      return gen_rtx_MEM (Pmode, addr);
    }
  return get_hard_reg_initial_val (Pmode, LP_REGNUM);
}
HOST_WIDE_INT
nds32_initial_elimination_offset (unsigned int from_reg, unsigned int to_reg)
{
  HOST_WIDE_INT offset;
  nds32_compute_stack_frame ();
  if (from_reg == ARG_POINTER_REGNUM && to_reg == STACK_POINTER_REGNUM)
    {
      offset = (cfun->machine->fp_size
  + cfun->machine->gp_size
  + cfun->machine->lp_size
  + cfun->machine->callee_saved_gpr_regs_size
  + cfun->machine->callee_saved_area_gpr_padding_bytes
  + cfun->machine->callee_saved_fpr_regs_size
  + cfun->machine->eh_return_data_regs_size
  + cfun->machine->local_size
  + cfun->machine->out_args_size);
    }
  else if (from_reg == ARG_POINTER_REGNUM
    && to_reg == HARD_FRAME_POINTER_REGNUM)
    {
      offset = 0;
    }
  else if (from_reg == FRAME_POINTER_REGNUM
    && to_reg == STACK_POINTER_REGNUM)
    {
      offset = (cfun->machine->local_size + cfun->machine->out_args_size);
    }
  else if (from_reg == FRAME_POINTER_REGNUM
    && to_reg == HARD_FRAME_POINTER_REGNUM)
    {
      offset = (-1) * (cfun->machine->fp_size
         + cfun->machine->gp_size
         + cfun->machine->lp_size
         + cfun->machine->callee_saved_gpr_regs_size
         + cfun->machine->callee_saved_area_gpr_padding_bytes
         + cfun->machine->callee_saved_fpr_regs_size
         + cfun->machine->eh_return_data_regs_size);
    }
  else
    {
      gcc_unreachable ();
    }
  return offset;
}
void
nds32_init_cumulative_args (CUMULATIVE_ARGS *cum,
       tree fntype ATTRIBUTE_UNUSED,
       rtx libname ATTRIBUTE_UNUSED,
       tree fndecl ATTRIBUTE_UNUSED,
       int n_named_args ATTRIBUTE_UNUSED)
{
  cum->gpr_offset = 0;
  cum->fpr_offset = 0;
}
void
nds32_expand_prologue (void)
{
  int fp_adjust;
  int sp_adjust;
  unsigned Rb, Re;
  nds32_compute_stack_frame ();
  if (frame_pointer_needed)
    cfun->machine->fp_as_gp_p = false;
  if (cfun->machine->va_args_size != 0)
    {
      Rb = cfun->machine->va_args_first_regno;
      Re = cfun->machine->va_args_last_regno;
      nds32_emit_stack_push_multiple (Rb, Re, false, false, false, true);
      if (cfun->machine->va_args_area_padding_bytes)
 {
   sp_adjust = cfun->machine->va_args_area_padding_bytes;
   nds32_emit_adjust_frame (stack_pointer_rtx,
         stack_pointer_rtx,
       -1 * sp_adjust);
 }
    }
  if (cfun->machine->naked_p && !flag_pic)
    return;
  Rb = cfun->machine->callee_saved_first_gpr_regno;
  Re = cfun->machine->callee_saved_last_gpr_regno;
  if (!(Rb == SP_REGNUM && Re == SP_REGNUM
 && cfun->machine->fp_size == 0
 && cfun->machine->gp_size == 0
 && cfun->machine->lp_size == 0))
    {
      nds32_emit_stack_push_multiple (
 Rb, Re,
 cfun->machine->fp_size, cfun->machine->gp_size, cfun->machine->lp_size,
 false);
    }
  if (cfun->machine->use_eh_return_p)
    {
      Rb = cfun->machine->eh_return_data_first_regno;
      Re = cfun->machine->eh_return_data_last_regno;
      nds32_emit_stack_push_multiple (Rb, Re, false, false, false, false);
    }
  if (frame_pointer_needed)
    {
      fp_adjust = cfun->machine->fp_size
    + cfun->machine->gp_size
    + cfun->machine->lp_size
    + cfun->machine->callee_saved_gpr_regs_size
    + cfun->machine->eh_return_data_regs_size;
      nds32_emit_adjust_frame (hard_frame_pointer_rtx,
          stack_pointer_rtx,
          fp_adjust);
    }
  if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
    {
      int fpr_offset = cfun->machine->local_size
         + cfun->machine->out_args_size
         + cfun->machine->callee_saved_fpr_regs_size;
      if (!satisfies_constraint_Is14 (GEN_INT (fpr_offset)))
 {
   int fpr_space = cfun->machine->callee_saved_area_gpr_padding_bytes
     + cfun->machine->callee_saved_fpr_regs_size;
   nds32_emit_adjust_frame (stack_pointer_rtx,
       stack_pointer_rtx,
       -1 * fpr_space);
   nds32_emit_push_fpr_callee_saved (0);
   sp_adjust = cfun->machine->local_size
        + cfun->machine->out_args_size;
   nds32_emit_adjust_frame (stack_pointer_rtx,
       stack_pointer_rtx,
       -1 * sp_adjust);
 }
      else
 {
   sp_adjust = cfun->machine->local_size
        + cfun->machine->out_args_size
        + cfun->machine->callee_saved_area_gpr_padding_bytes
        + cfun->machine->callee_saved_fpr_regs_size;
   nds32_emit_adjust_frame (stack_pointer_rtx,
       stack_pointer_rtx,
       -1 * sp_adjust);
   int fpr_position = cfun->machine->out_args_size
        + cfun->machine->local_size;
   nds32_emit_push_fpr_callee_saved (fpr_position);
 }
    }
  else
    {
      sp_adjust = cfun->machine->local_size
    + cfun->machine->out_args_size
    + cfun->machine->callee_saved_area_gpr_padding_bytes;
      nds32_emit_adjust_frame (stack_pointer_rtx,
          stack_pointer_rtx,
          -1 * sp_adjust);
    }
  if (flag_pic && df_regs_ever_live_p (PIC_OFFSET_TABLE_REGNUM))
    nds32_emit_load_gp ();
  if (!flag_sched_prolog_epilog)
    emit_insn (gen_blockage ());
}
void
nds32_expand_epilogue (bool sibcall_p)
{
  int sp_adjust;
  unsigned Rb, Re;
  nds32_compute_stack_frame ();
  if (!flag_sched_prolog_epilog)
    emit_insn (gen_blockage ());
  if (cfun->machine->naked_p)
    {
      if (cfun->machine->va_args_size != 0)
 {
   sp_adjust = cfun->machine->va_args_size
        + cfun->machine->va_args_area_padding_bytes;
     nds32_emit_adjust_frame (stack_pointer_rtx,
       stack_pointer_rtx,
       sp_adjust);
 }
      if (!sibcall_p)
 {
   if (cfun->machine->attr_naked_p && !flag_ret_in_naked_func)
     return;
   emit_jump_insn (gen_return_internal ());
 }
      return;
    }
  if (frame_pointer_needed)
    {
      if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
 {
   int gpr_padding = cfun->machine->callee_saved_area_gpr_padding_bytes;
   sp_adjust = cfun->machine->fp_size
        + cfun->machine->gp_size
        + cfun->machine->lp_size
        + cfun->machine->callee_saved_gpr_regs_size
        + cfun->machine->eh_return_data_regs_size
        + cfun->machine->callee_saved_area_gpr_padding_bytes
        + cfun->machine->callee_saved_fpr_regs_size;
   nds32_emit_adjust_frame (stack_pointer_rtx,
       hard_frame_pointer_rtx,
       -1 * sp_adjust);
   nds32_emit_pop_fpr_callee_saved (gpr_padding);
 }
      else
 {
   sp_adjust = cfun->machine->fp_size
        + cfun->machine->gp_size
        + cfun->machine->lp_size
        + cfun->machine->callee_saved_gpr_regs_size
        + cfun->machine->eh_return_data_regs_size;
   nds32_emit_adjust_frame (stack_pointer_rtx,
       hard_frame_pointer_rtx,
       -1 * sp_adjust);
 }
    }
  else
    {
      if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
 {
   int gpr_padding = cfun->machine->callee_saved_area_gpr_padding_bytes;
   sp_adjust = cfun->machine->local_size
        + cfun->machine->out_args_size;
   nds32_emit_adjust_frame (stack_pointer_rtx,
       stack_pointer_rtx,
       sp_adjust);
   nds32_emit_pop_fpr_callee_saved (gpr_padding);
 }
      else
 {
   sp_adjust = cfun->machine->local_size
        + cfun->machine->out_args_size
        + cfun->machine->callee_saved_area_gpr_padding_bytes;
   nds32_emit_adjust_frame (stack_pointer_rtx,
       stack_pointer_rtx,
       sp_adjust);
 }
    }
  if (cfun->machine->use_eh_return_p)
    {
      Rb = cfun->machine->eh_return_data_first_regno;
      Re = cfun->machine->eh_return_data_last_regno;
      nds32_emit_stack_pop_multiple (Rb, Re, false, false, false);
    }
  Rb = cfun->machine->callee_saved_first_gpr_regno;
  Re = cfun->machine->callee_saved_last_gpr_regno;
  if (!(Rb == SP_REGNUM && Re == SP_REGNUM
 && cfun->machine->fp_size == 0
 && cfun->machine->gp_size == 0
 && cfun->machine->lp_size == 0))
    {
      nds32_emit_stack_pop_multiple (
 Rb, Re,
 cfun->machine->fp_size, cfun->machine->gp_size, cfun->machine->lp_size);
    }
  if (cfun->machine->va_args_size != 0)
    {
      sp_adjust = cfun->machine->va_args_size
    + cfun->machine->va_args_area_padding_bytes;
      nds32_emit_adjust_frame (stack_pointer_rtx,
          stack_pointer_rtx,
          sp_adjust);
    }
  if (cfun->machine->use_eh_return_p)
    {
      rtx ta_reg;
      rtx insn;
      ta_reg = gen_rtx_REG (SImode, TA_REGNUM);
      insn = emit_move_insn (ta_reg, stack_pointer_rtx);
      add_reg_note (insn, REG_CFA_DEF_CFA, ta_reg);
      RTX_FRAME_RELATED_P (insn) = 1;
      emit_insn (gen_addsi3 (stack_pointer_rtx,
        stack_pointer_rtx,
        EH_RETURN_STACKADJ_RTX));
      emit_use (ta_reg);
    }
  if (!sibcall_p)
    emit_jump_insn (gen_return_internal ());
}
void
nds32_expand_prologue_v3push (void)
{
  int fp_adjust;
  int sp_adjust;
  int fpr_space = 0;
  unsigned Rb, Re;
  nds32_compute_stack_frame ();
  if (cfun->machine->callee_saved_gpr_regs_size > 0)
    df_set_regs_ever_live (FP_REGNUM, 1);
  if (frame_pointer_needed)
    cfun->machine->fp_as_gp_p = false;
  if (cfun->machine->naked_p && !flag_pic)
    return;
  Rb = cfun->machine->callee_saved_first_gpr_regno;
  Re = cfun->machine->callee_saved_last_gpr_regno;
  sp_adjust = cfun->machine->local_size
       + cfun->machine->out_args_size
       + cfun->machine->callee_saved_area_gpr_padding_bytes
       + cfun->machine->callee_saved_fpr_regs_size;
  if (satisfies_constraint_Iu08 (GEN_INT (sp_adjust))
      && NDS32_DOUBLE_WORD_ALIGN_P (sp_adjust))
    {
      nds32_emit_stack_v3push (Rb, Re, sp_adjust);
      if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
 {
   int fpr_position = cfun->machine->local_size
        + cfun->machine->out_args_size;
   nds32_emit_push_fpr_callee_saved (fpr_position);
 }
      if (frame_pointer_needed)
 {
   fp_adjust = cfun->machine->fp_size
        + cfun->machine->gp_size
        + cfun->machine->lp_size
        + cfun->machine->callee_saved_gpr_regs_size
        + sp_adjust;
   nds32_emit_adjust_frame (hard_frame_pointer_rtx,
       stack_pointer_rtx,
       fp_adjust);
 }
    }
  else
    {
      if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
 {
   fpr_space = cfun->machine->callee_saved_area_gpr_padding_bytes
        + cfun->machine->callee_saved_fpr_regs_size;
   nds32_emit_stack_v3push (Rb, Re, fpr_space);
   nds32_emit_push_fpr_callee_saved (0);
 }
      else
 {
   nds32_emit_stack_v3push (Rb, Re, 0);
 }
      if (frame_pointer_needed)
 {
   fp_adjust = cfun->machine->fp_size
        + cfun->machine->gp_size
        + cfun->machine->lp_size
        + cfun->machine->callee_saved_gpr_regs_size;
   if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
     {
       fp_adjust = fp_adjust + fpr_space;
     }
   nds32_emit_adjust_frame (hard_frame_pointer_rtx,
       stack_pointer_rtx,
       fp_adjust);
 }
      if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
 {
   sp_adjust = sp_adjust - fpr_space;
 }
      nds32_emit_adjust_frame (stack_pointer_rtx,
          stack_pointer_rtx,
          -1 * sp_adjust);
    }
  if (flag_pic && df_regs_ever_live_p (PIC_OFFSET_TABLE_REGNUM))
    nds32_emit_load_gp ();
  emit_insn (gen_blockage ());
}
void
nds32_expand_epilogue_v3pop (bool sibcall_p)
{
  int sp_adjust;
  unsigned Rb, Re;
  nds32_compute_stack_frame ();
  emit_insn (gen_blockage ());
  if (cfun->machine->naked_p)
    {
      if (!sibcall_p)
 {
   if (cfun->machine->attr_naked_p && !flag_ret_in_naked_func)
     return;
   emit_jump_insn (gen_return_internal ());
 }
      return;
    }
  Rb = cfun->machine->callee_saved_first_gpr_regno;
  Re = cfun->machine->callee_saved_last_gpr_regno;
  sp_adjust = cfun->machine->local_size
       + cfun->machine->out_args_size
       + cfun->machine->callee_saved_area_gpr_padding_bytes
       + cfun->machine->callee_saved_fpr_regs_size;
  if (satisfies_constraint_Iu08 (GEN_INT (sp_adjust))
      && NDS32_DOUBLE_WORD_ALIGN_P (sp_adjust)
      && !cfun->calls_alloca)
    {
      if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
 {
   int fpr_position = cfun->machine->local_size
        + cfun->machine->out_args_size;
   nds32_emit_v3pop_fpr_callee_saved (fpr_position);
 }
      nds32_emit_stack_v3pop (Rb, Re, sp_adjust);
    }
  else
    {
      if (frame_pointer_needed)
 {
   sp_adjust = cfun->machine->fp_size
        + cfun->machine->gp_size
        + cfun->machine->lp_size
        + cfun->machine->callee_saved_gpr_regs_size;
   if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
     {
       sp_adjust = sp_adjust
     + cfun->machine->callee_saved_area_gpr_padding_bytes
     + cfun->machine->callee_saved_fpr_regs_size;
       nds32_emit_adjust_frame (stack_pointer_rtx,
           hard_frame_pointer_rtx,
           -1 * sp_adjust);
       nds32_emit_v3pop_fpr_callee_saved (0);
     }
   else
     {
       nds32_emit_adjust_frame (stack_pointer_rtx,
           hard_frame_pointer_rtx,
           -1 * sp_adjust);
     }
 }
      else
 {
   sp_adjust = cfun->machine->local_size
        + cfun->machine->out_args_size
        + cfun->machine->callee_saved_area_gpr_padding_bytes
        + cfun->machine->callee_saved_fpr_regs_size;
   if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
     {
       sp_adjust = sp_adjust
     - cfun->machine->callee_saved_area_gpr_padding_bytes
     - cfun->machine->callee_saved_fpr_regs_size;
       nds32_emit_adjust_frame (stack_pointer_rtx,
           stack_pointer_rtx,
           sp_adjust);
       nds32_emit_v3pop_fpr_callee_saved (0);
     }
   else
     {
       nds32_emit_adjust_frame (stack_pointer_rtx,
           stack_pointer_rtx,
           sp_adjust);
     }
 }
      if (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)
 {
   int fpr_space = cfun->machine->callee_saved_area_gpr_padding_bytes
     + cfun->machine->callee_saved_fpr_regs_size;
   nds32_emit_stack_v3pop (Rb, Re, fpr_space);
 }
      else
 {
   nds32_emit_stack_v3pop (Rb, Re, 0);
 }
    }
  emit_jump_insn (gen_pop25return ());
}
int
nds32_can_use_return_insn (void)
{
  int sp_adjust;
  if (!reload_completed)
    return 0;
  if (cfun->machine->attr_naked_p && !flag_ret_in_naked_func)
    return 0;
  sp_adjust = cfun->machine->local_size
       + cfun->machine->out_args_size
       + cfun->machine->callee_saved_area_gpr_padding_bytes
       + cfun->machine->callee_saved_fpr_regs_size;
  if (!cfun->machine->fp_as_gp_p
      && satisfies_constraint_Iu08 (GEN_INT (sp_adjust))
      && NDS32_DOUBLE_WORD_ALIGN_P (sp_adjust)
      && !cfun->calls_alloca
      && NDS32_V3PUSH_AVAILABLE_P
      && !(TARGET_HARD_FLOAT
    && (cfun->machine->callee_saved_first_fpr_regno != SP_REGNUM)))
    return 1;
  return ((cfun->machine->naked_p && (cfun->machine->va_args_size == 0)));
}
enum machine_mode
nds32_case_vector_shorten_mode (int min_offset, int max_offset,
    rtx body ATTRIBUTE_UNUSED)
{
  if (min_offset < 0 || max_offset >= 0x2000)
    return SImode;
  else
    {
      if (max_offset >= 0xff)
 return HImode;
      else
 return QImode;
    }
}
static bool
nds32_cannot_copy_insn_p (rtx insn)
{
  if (recog_memoized (insn) == CODE_FOR_hwloop_cfg)
    return true;
  return false;
}
int
nds32_target_alignment (rtx label)
{
  rtx insn;
  if (!NDS32_ALIGN_P ())
    return 0;
  insn = next_active_insn (label);
  if (insn == 0
      || (get_attr_length (insn) == 2
   && !JUMP_P (insn) && !CALL_P (insn)))
    return 0;
  else
    return 2;
}
unsigned int
nds32_data_alignment (tree data,
        unsigned int basic_align)
{
  if ((basic_align < BITS_PER_WORD)
      && (TREE_CODE (data) == ARRAY_TYPE
  || TREE_CODE (data) == UNION_TYPE
  || TREE_CODE (data) == RECORD_TYPE))
    return BITS_PER_WORD;
  else
    return basic_align;
}
unsigned int
nds32_constant_alignment (tree constant,
     unsigned int basic_align)
{
  if (((TREE_CODE (constant) == STRING_CST
 || TREE_CODE (constant) == CONSTRUCTOR
 || TREE_CODE (constant) == UNION_TYPE
 || TREE_CODE (constant) == RECORD_TYPE
 || TREE_CODE (constant) == ARRAY_TYPE)
       && basic_align < BITS_PER_WORD))
    return BITS_PER_WORD;
  else
    return basic_align;
}
unsigned int
nds32_local_alignment (tree local ATTRIBUTE_UNUSED,
         unsigned int basic_align)
{
  bool at_least_align_to_word = false;
  switch (TREE_CODE (local))
    {
    case ARRAY_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
      at_least_align_to_word = true;
      break;
    default:
      at_least_align_to_word = false;
      break;
    }
  if (at_least_align_to_word
      && (basic_align < BITS_PER_WORD))
    return BITS_PER_WORD;
  else
    return basic_align;
}
#undef TARGET_PROMOTE_FUNCTION_MODE
#define TARGET_PROMOTE_FUNCTION_MODE \
  default_promote_function_mode_always_promote
#undef TARGET_EXPAND_TO_RTL_HOOK
#define TARGET_EXPAND_TO_RTL_HOOK nds32_expand_to_rtl_hook
#undef TARGET_CONDITIONAL_REGISTER_USAGE
#define TARGET_CONDITIONAL_REGISTER_USAGE nds32_conditional_register_usage
#undef TARGET_PREFERRED_RENAME_CLASS
#define TARGET_PREFERRED_RENAME_CLASS nds32_preferred_rename_class
#undef TARGET_CLASS_MAX_NREGS
#define TARGET_CLASS_MAX_NREGS nds32_class_max_nregs
#undef TARGET_LRA_P
#define TARGET_LRA_P hook_bool_void_true
#undef TARGET_REGISTER_PRIORITY
#define TARGET_REGISTER_PRIORITY nds32_register_priority
#undef TARGET_CAN_ELIMINATE
#define TARGET_CAN_ELIMINATE nds32_can_eliminate
#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG nds32_function_arg
#undef TARGET_MUST_PASS_IN_STACK
#define TARGET_MUST_PASS_IN_STACK nds32_must_pass_in_stack
#undef TARGET_ARG_PARTIAL_BYTES
#define TARGET_ARG_PARTIAL_BYTES nds32_arg_partial_bytes
#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE nds32_function_arg_advance
#undef TARGET_FUNCTION_ARG_BOUNDARY
#define TARGET_FUNCTION_ARG_BOUNDARY nds32_function_arg_boundary
#undef TARGET_VECTOR_MODE_SUPPORTED_P
#define TARGET_VECTOR_MODE_SUPPORTED_P nds32_vector_mode_supported_p
#undef TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE nds32_function_value
#undef TARGET_LIBCALL_VALUE
#define TARGET_LIBCALL_VALUE nds32_libcall_value
#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P nds32_function_value_regno_p
#undef TARGET_RETURN_IN_MEMORY
#define TARGET_RETURN_IN_MEMORY nds32_return_in_memory
#undef TARGET_ASM_FUNCTION_PROLOGUE
#define TARGET_ASM_FUNCTION_PROLOGUE nds32_asm_function_prologue
#undef TARGET_ASM_FUNCTION_END_PROLOGUE
#define TARGET_ASM_FUNCTION_END_PROLOGUE nds32_asm_function_end_prologue
#undef TARGET_ASM_FUNCTION_BEGIN_EPILOGUE
#define TARGET_ASM_FUNCTION_BEGIN_EPILOGUE nds32_asm_function_begin_epilogue
#undef TARGET_ASM_FUNCTION_EPILOGUE
#define TARGET_ASM_FUNCTION_EPILOGUE nds32_asm_function_epilogue
#undef TARGET_ASM_OUTPUT_MI_THUNK
#define TARGET_ASM_OUTPUT_MI_THUNK nds32_asm_output_mi_thunk
#undef TARGET_ASM_CAN_OUTPUT_MI_THUNK
#define TARGET_ASM_CAN_OUTPUT_MI_THUNK default_can_output_mi_thunk_no_vcall
#undef TARGET_FUNCTION_OK_FOR_SIBCALL
#define TARGET_FUNCTION_OK_FOR_SIBCALL nds32_function_ok_for_sibcall
#undef TARGET_WARN_FUNC_RETURN
#define TARGET_WARN_FUNC_RETURN nds32_warn_func_return
#undef TARGET_SETUP_INCOMING_VARARGS
#define TARGET_SETUP_INCOMING_VARARGS nds32_setup_incoming_varargs
#undef TARGET_STRICT_ARGUMENT_NAMING
#define TARGET_STRICT_ARGUMENT_NAMING nds32_strict_argument_naming
#undef TARGET_ASM_TRAMPOLINE_TEMPLATE
#define TARGET_ASM_TRAMPOLINE_TEMPLATE nds32_asm_trampoline_template
#undef TARGET_TRAMPOLINE_INIT
#define TARGET_TRAMPOLINE_INIT nds32_trampoline_init
#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P nds32_legitimate_address_p
#undef TARGET_LEGITIMIZE_ADDRESS
#define TARGET_LEGITIMIZE_ADDRESS nds32_legitimize_address
#undef TARGET_LEGITIMATE_CONSTANT_P
#define TARGET_LEGITIMATE_CONSTANT_P nds32_legitimate_constant_p
#undef TARGET_VECTORIZE_PREFERRED_SIMD_MODE
#define TARGET_VECTORIZE_PREFERRED_SIMD_MODE nds32_vectorize_preferred_simd_mode
#undef TARGET_CANNOT_FORCE_CONST_MEM
#define TARGET_CANNOT_FORCE_CONST_MEM nds32_cannot_force_const_mem
#undef TARGET_DELEGITIMIZE_ADDRESS
#define TARGET_DELEGITIMIZE_ADDRESS nds32_delegitimize_address
#undef TARGET_CANONICALIZE_COMPARISON
#define TARGET_CANONICALIZE_COMPARISON nds32_canonicalize_comparison
#undef TARGET_REGISTER_MOVE_COST
#define TARGET_REGISTER_MOVE_COST nds32_register_move_cost
#undef TARGET_MEMORY_MOVE_COST
#define TARGET_MEMORY_MOVE_COST nds32_memory_move_cost
#undef TARGET_RTX_COSTS
#define TARGET_RTX_COSTS nds32_rtx_costs
#undef TARGET_ADDRESS_COST
#define TARGET_ADDRESS_COST nds32_address_cost
#undef TARGET_SCHED_ISSUE_RATE
#define TARGET_SCHED_ISSUE_RATE nds32_sched_issue_rate
#undef TARGET_SCHED_ADJUST_COST
#define TARGET_SCHED_ADJUST_COST nds32_sched_adjust_cost
#undef TARGET_ENCODE_SECTION_INFO
#define TARGET_ENCODE_SECTION_INFO nds32_encode_section_info
#undef TARGET_ASM_FILE_START
#define TARGET_ASM_FILE_START nds32_asm_file_start
#undef TARGET_ASM_FILE_END
#define TARGET_ASM_FILE_END nds32_asm_file_end
#undef TARGET_ASM_ALIGNED_HI_OP
#define TARGET_ASM_ALIGNED_HI_OP "\t.hword\t"
#undef TARGET_ASM_ALIGNED_SI_OP
#define TARGET_ASM_ALIGNED_SI_OP "\t.word\t"
#undef TARGET_ASM_OUTPUT_ADDR_CONST_EXTRA
#define TARGET_ASM_OUTPUT_ADDR_CONST_EXTRA nds32_asm_output_addr_const_extra
#undef TARGET_ASM_GLOBALIZE_LABEL
#define TARGET_ASM_GLOBALIZE_LABEL nds32_asm_globalize_label
#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND nds32_print_operand
#undef TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS nds32_print_operand_address
#undef TARGET_DWARF_REGISTER_SPAN
#define TARGET_DWARF_REGISTER_SPAN nds32_dwarf_register_span
#undef TARGET_DELAY_VARTRACK
#define TARGET_DELAY_VARTRACK true
#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE nds32_attribute_table
#undef TARGET_MERGE_DECL_ATTRIBUTES
#define TARGET_MERGE_DECL_ATTRIBUTES nds32_merge_decl_attributes
#undef TARGET_INSERT_ATTRIBUTES
#define TARGET_INSERT_ATTRIBUTES nds32_insert_attributes
#undef TARGET_OPTION_PRAGMA_PARSE
#define TARGET_OPTION_PRAGMA_PARSE nds32_option_pragma_parse
#undef TARGET_OPTION_OVERRIDE
#define TARGET_OPTION_OVERRIDE nds32_option_override
#undef TARGET_HAVE_TLS
#define TARGET_HAVE_TLS TARGET_LINUX_ABI
#undef TARGET_MD_ASM_CLOBBERS
#define TARGET_MD_ASM_CLOBBERS nds32_md_asm_clobbers
#undef TARGET_MACHINE_DEPENDENT_REORG
#define TARGET_MACHINE_DEPENDENT_REORG nds32_machine_dependent_reorg
#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS nds32_init_builtins
#undef TARGET_BUILTIN_DECL
#define TARGET_BUILTIN_DECL nds32_builtin_decl
#undef TARGET_EXPAND_BUILTIN
#define TARGET_EXPAND_BUILTIN nds32_expand_builtin
#undef TARGET_HAVE_CONDITIONAL_EXECUTION
#define TARGET_HAVE_CONDITIONAL_EXECUTION nds32_have_conditional_execution
#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS nds32_init_libfuncs
#undef TARGET_CAN_USE_DOLOOP_P
#define TARGET_CAN_USE_DOLOOP_P nds32_can_use_doloop_p
#undef TARGET_INVALID_WITHIN_DOLOOP
#define TARGET_INVALID_WITHIN_DOLOOP nds32_invalid_within_doloop
#undef TARGET_CANNOT_COPY_INSN_P
#define TARGET_CANNOT_COPY_INSN_P nds32_cannot_copy_insn_p
struct gcc_target targetm = TARGET_INITIALIZER;
