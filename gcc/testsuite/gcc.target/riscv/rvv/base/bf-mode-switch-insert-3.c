/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_xandesbf -mabi=lp64d -O3 -mbf16mscsr" } */
#include "riscv_vector.h"

vfloat16m1_t
fp16_vfadd (vfloat16m1_t op1, vfloat16m1_t op2, size_t vl)
{
  return __riscv_vfadd_vv_f16m1 (op1, op2, vl);
}

vbfloat16m1_t
bf16_vfadd (vbfloat16m1_t op1, vbfloat16m1_t op2, size_t vl)
{
  return __riscv_vfadd_vv_bf16m1 (op1, op2, vl);
}

/* { dg-final { scan-assembler-times {csrrsi\s+zero+,\sumisc_ctl+,\s1} 1 } } */
/* { dg-final { scan-assembler-times {csrrci\s+zero+,\sumisc_ctl+,\s1} 1 } } */