/* { dg-do compile } */
/* { dg-options "-march=rv32gv_xandes_xandesvbfhcvt" { target { rv32 } } } */
/* { dg-options "-march=rv64gv_xandes_xandesvbfhcvt" { target { rv64 } } } */

#include <nds_intrinsic.h>
#include <riscv_vector.h>

vfloat32m2_t
test_nds_vfwcvt_s_bf16_f32m2 (vbfloat16m1_t src)
{
  return __riscv_nds_vfwcvt_s_bf16_f32m2 ((src), 0);
}

vfloat32m4_t
test_nds_vfwcvt_s_bf16_f32m4 (vbfloat16m2_t src)
{
  return __riscv_nds_vfwcvt_s_bf16_f32m4 ((src), 0);
}

vfloat32m8_t
test_nds_vfwcvt_s_bf16_f32m8 (vbfloat16m4_t src)
{
  return __riscv_nds_vfwcvt_s_bf16_f32m8 ((src), 0);
}

/* { dg-final { scan-assembler-times {nds\.vfwcvt\.s\.bf16} 3 } } */
