/* { dg-do compile } */
/* { dg-options "-march=rv32gv_xandes_xandesvbfhcvt" { target { rv32 } } } */
/* { dg-options "-march=rv64gv_xandes_xandesvbfhcvt" { target { rv64 } } } */
/* { dg-prune-output "warning: '.*' will be deprecated .*" } */

#include <nds_intrinsic.h>
#include <riscv_vector.h>

vfloat32m2_t
test_vfwcvtsbf16_bf16m1 (vbfloat16m1_t src)
{
  return vfwcvtsbf16_bf16m1 ((src));
}

vfloat32m4_t
test_vfwcvtsbf16_bf16m2 (vbfloat16m2_t src)
{
  return vfwcvtsbf16_bf16m2 ((src));
}

vfloat32m8_t
test_vfwcvtsbf16_bf16m4 (vbfloat16m4_t src)
{
  return vfwcvtsbf16_bf16m4 ((src));
}

/* { dg-final { scan-assembler-times {nds\.vfwcvt\.s\.bf16} 3 } } */
