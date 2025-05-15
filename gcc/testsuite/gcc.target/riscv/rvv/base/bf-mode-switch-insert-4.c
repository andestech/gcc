/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_xandesbf -mabi=lp64d -O3 -mbf16mscsr" } */
#include "riscv_vector.h"

vfloat32m2_t
foo (vbfloat16m1_t bfop1, vbfloat16m1_t bfop2, vfloat16m1_t hfop1,
     vfloat16m1_t hfop2, size_t vl)
{
  vfloat32m2_t result1, result2;
  result1 = __riscv_vfwadd_vv_f32m2 (bfop1, bfop2, vl);
  result2 = __riscv_vfwadd_vv_f32m2 (hfop1, hfop2, vl);

  return __riscv_vfadd_vv_f32m2 (result1, result2, vl);
}

/* { dg-final { scan-assembler-times {csrrsi\s+zero+,\sumisc_ctl+,\s1} 1 } } */
/* { dg-final { scan-assembler-times {csrrci\s+zero+,\sumisc_ctl+,\s1} 1 } } */