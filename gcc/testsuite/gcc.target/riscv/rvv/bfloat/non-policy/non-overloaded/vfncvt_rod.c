/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfncvt_rod_f_f_w_bf16mf4 (vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16mf4 (src, vl);
}

vbfloat16mf2_t
test_vfncvt_rod_f_f_w_bf16mf2 (vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16mf2 (src, vl);
}

vbfloat16m1_t
test_vfncvt_rod_f_f_w_bf16m1 (vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16m1 (src, vl);
}

vbfloat16m2_t
test_vfncvt_rod_f_f_w_bf16m2 (vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16m2 (src, vl);
}

vbfloat16m4_t
test_vfncvt_rod_f_f_w_bf16m4 (vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16m4 (src, vl);
}

vfloat32mf2_t
test_vfncvt_rod_f_f_w_f32mf2 (vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32mf2 (src, vl);
}

vfloat32m1_t
test_vfncvt_rod_f_f_w_f32m1 (vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32m1 (src, vl);
}

vfloat32m2_t
test_vfncvt_rod_f_f_w_f32m2 (vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32m2 (src, vl);
}

vfloat32m4_t
test_vfncvt_rod_f_f_w_f32m4 (vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32m4 (src, vl);
}

vbfloat16mf4_t
test_vfncvt_rod_f_f_w_bf16mf4_m (vbool64_t mask, vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16mf4_m (mask, src, vl);
}

vbfloat16mf2_t
test_vfncvt_rod_f_f_w_bf16mf2_m (vbool32_t mask, vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16mf2_m (mask, src, vl);
}

vbfloat16m1_t
test_vfncvt_rod_f_f_w_bf16m1_m (vbool16_t mask, vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16m1_m (mask, src, vl);
}

vbfloat16m2_t
test_vfncvt_rod_f_f_w_bf16m2_m (vbool8_t mask, vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16m2_m (mask, src, vl);
}

vbfloat16m4_t
test_vfncvt_rod_f_f_w_bf16m4_m (vbool4_t mask, vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_rod_bf_f_w_bf16m4_m (mask, src, vl);
}

vfloat32mf2_t
test_vfncvt_rod_f_f_w_f32mf2_m (vbool64_t mask, vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32mf2_m (mask, src, vl);
}

vfloat32m1_t
test_vfncvt_rod_f_f_w_f32m1_m (vbool32_t mask, vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32m1_m (mask, src, vl);
}

vfloat32m2_t
test_vfncvt_rod_f_f_w_f32m2_m (vbool16_t mask, vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32m2_m (mask, src, vl);
}

vfloat32m4_t
test_vfncvt_rod_f_f_w_f32m4_m (vbool8_t mask, vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_rod_f_f_w_f32m4_m (mask, src, vl);
}
