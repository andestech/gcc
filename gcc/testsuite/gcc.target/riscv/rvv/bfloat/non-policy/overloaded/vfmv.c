/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

__bf16
test_vfmv_f_s_bf16mf4_bf16 (vbfloat16mf4_t src)
{
  return __riscv_vfmv_f (src);
}

__bf16
test_vfmv_f_s_bf16mf2_bf16 (vbfloat16mf2_t src)
{
  return __riscv_vfmv_f (src);
}

__bf16
test_vfmv_f_s_bf16m1_bf16 (vbfloat16m1_t src)
{
  return __riscv_vfmv_f (src);
}

__bf16
test_vfmv_f_s_bf16m2_bf16 (vbfloat16m2_t src)
{
  return __riscv_vfmv_f (src);
}

__bf16
test_vfmv_f_s_bf16m4_bf16 (vbfloat16m4_t src)
{
  return __riscv_vfmv_f (src);
}

__bf16
test_vfmv_f_s_bf16m8_bf16 (vbfloat16m8_t src)
{
  return __riscv_vfmv_f (src);
}

float
test_vfmv_f_s_f32mf2_f32 (vfloat32mf2_t src)
{
  return __riscv_vfmv_f (src);
}

float
test_vfmv_f_s_f32m1_f32 (vfloat32m1_t src)
{
  return __riscv_vfmv_f (src);
}

float
test_vfmv_f_s_f32m2_f32 (vfloat32m2_t src)
{
  return __riscv_vfmv_f (src);
}

float
test_vfmv_f_s_f32m4_f32 (vfloat32m4_t src)
{
  return __riscv_vfmv_f (src);
}

float
test_vfmv_f_s_f32m8_f32 (vfloat32m8_t src)
{
  return __riscv_vfmv_f (src);
}

double
test_vfmv_f_s_f64m1_f64 (vfloat64m1_t src)
{
  return __riscv_vfmv_f (src);
}

double
test_vfmv_f_s_f64m2_f64 (vfloat64m2_t src)
{
  return __riscv_vfmv_f (src);
}

double
test_vfmv_f_s_f64m4_f64 (vfloat64m4_t src)
{
  return __riscv_vfmv_f (src);
}

double
test_vfmv_f_s_f64m8_f64 (vfloat64m8_t src)
{
  return __riscv_vfmv_f (src);
}
