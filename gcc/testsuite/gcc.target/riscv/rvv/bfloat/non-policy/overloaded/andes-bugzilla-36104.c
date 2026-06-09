/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */
#include "riscv_vector.h"

vbfloat16mf4_t test_vfsgnj_vf_bf16mf4(vbfloat16mf4_t vs1, size_t vl) {
  return __riscv_vfsgnj(vs1, 0.0, vl);
}
