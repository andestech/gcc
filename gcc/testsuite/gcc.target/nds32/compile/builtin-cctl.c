/* Verify that we generate cache control instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "L1D_VA_INVAL" } } */
/* { dg-final { scan-assembler "L1D_VA_INVAL" } } */
/* { dg-final { scan-assembler "L1D_INVALALL" } } */
/* { dg-final { scan-assembler "L1D_IX_WWD" } } */
/* { dg-final { scan-assembler "L1D_IX_RWD" } } */
/* { dg-final { scan-assembler "PFM_CTL" } } */
/* { dg-final { scan-assembler "PFM_CTL" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int va = 0;

  __nds32__cctlva_lck (NDS32_CCTL_L1D_VA_FILLCK, &va);
  __nds32__cctlidx_wbinval (NDS32_CCTL_L1D_IX_WBINVAL, va);
  __nds32__cctlva_wbinval_alvl (NDS32_CCTL_L1D_VA_INVAL, &va);
  __nds32__cctlva_wbinval_one_lvl (NDS32_CCTL_L1D_VA_INVAL, &va);
  __nds32__cctl_l1d_invalall ();
  __nds32__cctlidx_write (NDS32_CCTL_L1D_IX_WWD, va, 1);
  __nds32__cctlidx_read (NDS32_CCTL_L1D_IX_RWD, 1);
  __nds32__mtusr (0, NDS32_USR_PFM_CTL);
  __nds32__mfusr (NDS32_USR_PFM_CTL);
}
