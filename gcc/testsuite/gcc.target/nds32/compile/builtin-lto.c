/* Verify that we use -flto option to generate instructions
   with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0 -flto" } */
/* { dg-final { scan-assembler "\\tdsb" } } */
/* { dg-final { scan-assembler "\\tisb" } } */
/* { dg-final { scan-assembler "\\tmsync\\tall" } } */
/* { dg-final { scan-assembler "\\tmsync\\tstore" } } */
/* { dg-final { scan-assembler "\\tnop" } } */
/* { dg-final { scan-assembler "\\tstandby\\tno_wake_grant" } } */
/* { dg-final { scan-assembler "\\tstandby\\twake_grant" } } */
/* { dg-final { scan-assembler "\\tstandby\\twait_done" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  __nds32__dsb ();
  __nds32__isb ();
  __nds32__msync_all ();
  __nds32__msync_store ();
  __nds32__nop ();
  __nds32__standby_no_wake_grant ();
  __nds32__standby_wake_grant ();
  __nds32__standby_wait_done ();
}
