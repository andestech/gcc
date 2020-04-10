/* Test bf16 conversion intrinsics are supported*/
/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mbf16" } */
#include <nds_intrinsic.h>

void main() {
  __bf16 bf_input;
  float sf_result;
  
  bf_input = __nds_fcvt_bf16_s(sf_result);
/* { dg-final { scan-assembler "fcvt.bf16.s" } } */
  sf_result = __nds_fcvt_s_bf16(bf_input);
/* { dg-final { scan-assembler "fcvt.s.bf16" } } */
   
}
