/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfmv_v_f_bf16mf4_tu (vbfloat16mf4_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vbfloat16mf2_t
test_vfmv_v_f_bf16mf2_tu (vbfloat16mf2_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vbfloat16m1_t
test_vfmv_v_f_bf16m1_tu (vbfloat16m1_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vbfloat16m2_t
test_vfmv_v_f_bf16m2_tu (vbfloat16m2_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vbfloat16m4_t
test_vfmv_v_f_bf16m4_tu (vbfloat16m4_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vbfloat16m8_t
test_vfmv_v_f_bf16m8_tu (vbfloat16m8_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat32mf2_t
test_vfmv_v_f_f32mf2_tu (vfloat32mf2_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat32m1_t
test_vfmv_v_f_f32m1_tu (vfloat32m1_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat32m2_t
test_vfmv_v_f_f32m2_tu (vfloat32m2_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat32m4_t
test_vfmv_v_f_f32m4_tu (vfloat32m4_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat32m8_t
test_vfmv_v_f_f32m8_tu (vfloat32m8_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat64m1_t
test_vfmv_v_f_f64m1_tu (vfloat64m1_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat64m2_t
test_vfmv_v_f_f64m2_tu (vfloat64m2_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat64m4_t
test_vfmv_v_f_f64m4_tu (vfloat64m4_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vfloat64m8_t
test_vfmv_v_f_f64m8_tu (vfloat64m8_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_v_tu (maskedoff, src, vl);
}

vbfloat16mf4_t
test_vfmv_s_f_bf16mf4_tu (vbfloat16mf4_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vbfloat16mf2_t
test_vfmv_s_f_bf16mf2_tu (vbfloat16mf2_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vbfloat16m1_t
test_vfmv_s_f_bf16m1_tu (vbfloat16m1_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vbfloat16m2_t
test_vfmv_s_f_bf16m2_tu (vbfloat16m2_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vbfloat16m4_t
test_vfmv_s_f_bf16m4_tu (vbfloat16m4_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vbfloat16m8_t
test_vfmv_s_f_bf16m8_tu (vbfloat16m8_t maskedoff, __bf16 src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat32mf2_t
test_vfmv_s_f_f32mf2_tu (vfloat32mf2_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat32m1_t
test_vfmv_s_f_f32m1_tu (vfloat32m1_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat32m2_t
test_vfmv_s_f_f32m2_tu (vfloat32m2_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat32m4_t
test_vfmv_s_f_f32m4_tu (vfloat32m4_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat32m8_t
test_vfmv_s_f_f32m8_tu (vfloat32m8_t maskedoff, float src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat64m1_t
test_vfmv_s_f_f64m1_tu (vfloat64m1_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat64m2_t
test_vfmv_s_f_f64m2_tu (vfloat64m2_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat64m4_t
test_vfmv_s_f_f64m4_tu (vfloat64m4_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}

vfloat64m8_t
test_vfmv_s_f_f64m8_tu (vfloat64m8_t maskedoff, double src, size_t vl)
{
  return __riscv_vfmv_s_tu (maskedoff, src, vl);
}
