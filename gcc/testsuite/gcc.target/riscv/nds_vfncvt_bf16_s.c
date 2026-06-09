/* { dg-do compile } */
/* { dg-options "-march=rv32gv_xandes_xandesvbfhcvt" { target { rv32 } } } */
/* { dg-options "-march=rv64gv_xandes_xandesvbfhcvt" { target { rv64 } } } */

#include <nds_intrinsic.h>
#include <riscv_vector.h>

vbfloat16m1_t
test_nds_vfncvt_bf16_s_bf16m1 (vfloat32m2_t src)
{
  return __riscv_nds_vfncvt_bf16_s_bf16m1 ((src), 0);
}

vbfloat16m2_t
test_nds_vfncvt_bf16_s_bf16m2 (vfloat32m4_t src)
{
  return __riscv_nds_vfncvt_bf16_s_bf16m2 ((src), 0);
}

vbfloat16m4_t
test_nds_vfncvt_bf16_s_bf16m4 (vfloat32m8_t src)
{
  return __riscv_nds_vfncvt_bf16_s_bf16m4 ((src), 0);
}

/* { dg-final { scan-assembler-times {nds\.vfncvt\.bf16\.s} 3 } } */
