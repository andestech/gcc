/* Test bf16 conversion intrinsics are working correctly*/
/* { dg-do run { target riscv_xebfhw_hw } } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mbf16" } */

#include <nds_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  float fa = 2.5;
  __bf16 bfa;
  
  bfa = __nds_fcvt_bf16_s(fa);
  
  fa =  __nds_fcvt_s_bf16(bfa);
  
  if(fa != 2.5)
    abort ();
  else 
    exit (0);
}
