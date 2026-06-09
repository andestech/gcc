/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfmv_v_f_bf16mf4 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_v_f_bf16mf4 (src, vl);
}

vbfloat16mf2_t
test_vfmv_v_f_bf16mf2 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_v_f_bf16mf2 (src, vl);
}

vbfloat16m1_t
test_vfmv_v_f_bf16m1 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_v_f_bf16m1 (src, vl);
}

vbfloat16m2_t
test_vfmv_v_f_bf16m2 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_v_f_bf16m2 (src, vl);
}

vbfloat16m4_t
test_vfmv_v_f_bf16m4 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_v_f_bf16m4 (src, vl);
}

vbfloat16m8_t
test_vfmv_v_f_bf16m8 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_v_f_bf16m8 (src, vl);
}

vfloat32mf2_t
test_vfmv_v_f_f32mf2 (float src, size_t vl)
{
  return __riscv_vfmv_v_f_f32mf2 (src, vl);
}

vfloat32m1_t
test_vfmv_v_f_f32m1 (float src, size_t vl)
{
  return __riscv_vfmv_v_f_f32m1 (src, vl);
}

vfloat32m2_t
test_vfmv_v_f_f32m2 (float src, size_t vl)
{
  return __riscv_vfmv_v_f_f32m2 (src, vl);
}

vfloat32m4_t
test_vfmv_v_f_f32m4 (float src, size_t vl)
{
  return __riscv_vfmv_v_f_f32m4 (src, vl);
}

vfloat32m8_t
test_vfmv_v_f_f32m8 (float src, size_t vl)
{
  return __riscv_vfmv_v_f_f32m8 (src, vl);
}

vfloat64m1_t
test_vfmv_v_f_f64m1 (double src, size_t vl)
{
  return __riscv_vfmv_v_f_f64m1 (src, vl);
}

vfloat64m2_t
test_vfmv_v_f_f64m2 (double src, size_t vl)
{
  return __riscv_vfmv_v_f_f64m2 (src, vl);
}

vfloat64m4_t
test_vfmv_v_f_f64m4 (double src, size_t vl)
{
  return __riscv_vfmv_v_f_f64m4 (src, vl);
}

vfloat64m8_t
test_vfmv_v_f_f64m8 (double src, size_t vl)
{
  return __riscv_vfmv_v_f_f64m8 (src, vl);
}

__bf16
test_vfmv_f_s_bf16mf4_bf16 (vbfloat16mf4_t src)
{
  return __riscv_vfmv_f_s_bf16mf4_bf16 (src);
}

vbfloat16mf4_t
test_vfmv_s_f_bf16mf4 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_s_f_bf16mf4 (src, vl);
}

__bf16
test_vfmv_f_s_bf16mf2_bf16 (vbfloat16mf2_t src)
{
  return __riscv_vfmv_f_s_bf16mf2_bf16 (src);
}

vbfloat16mf2_t
test_vfmv_s_f_bf16mf2 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_s_f_bf16mf2 (src, vl);
}

__bf16
test_vfmv_f_s_bf16m1_bf16 (vbfloat16m1_t src)
{
  return __riscv_vfmv_f_s_bf16m1_bf16 (src);
}

vbfloat16m1_t
test_vfmv_s_f_bf16m1 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_s_f_bf16m1 (src, vl);
}

__bf16
test_vfmv_f_s_bf16m2_bf16 (vbfloat16m2_t src)
{
  return __riscv_vfmv_f_s_bf16m2_bf16 (src);
}

vbfloat16m2_t
test_vfmv_s_f_bf16m2 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_s_f_bf16m2 (src, vl);
}

__bf16
test_vfmv_f_s_bf16m4_bf16 (vbfloat16m4_t src)
{
  return __riscv_vfmv_f_s_bf16m4_bf16 (src);
}

vbfloat16m4_t
test_vfmv_s_f_bf16m4 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_s_f_bf16m4 (src, vl);
}

__bf16
test_vfmv_f_s_bf16m8_bf16 (vbfloat16m8_t src)
{
  return __riscv_vfmv_f_s_bf16m8_bf16 (src);
}

vbfloat16m8_t
test_vfmv_s_f_bf16m8 (__bf16 src, size_t vl)
{
  return __riscv_vfmv_s_f_bf16m8 (src, vl);
}

float
test_vfmv_f_s_f32mf2_f32 (vfloat32mf2_t src)
{
  return __riscv_vfmv_f_s_f32mf2_f32 (src);
}

vfloat32mf2_t
test_vfmv_s_f_f32mf2 (float src, size_t vl)
{
  return __riscv_vfmv_s_f_f32mf2 (src, vl);
}

float
test_vfmv_f_s_f32m1_f32 (vfloat32m1_t src)
{
  return __riscv_vfmv_f_s_f32m1_f32 (src);
}

vfloat32m1_t
test_vfmv_s_f_f32m1 (float src, size_t vl)
{
  return __riscv_vfmv_s_f_f32m1 (src, vl);
}

float
test_vfmv_f_s_f32m2_f32 (vfloat32m2_t src)
{
  return __riscv_vfmv_f_s_f32m2_f32 (src);
}

vfloat32m2_t
test_vfmv_s_f_f32m2 (float src, size_t vl)
{
  return __riscv_vfmv_s_f_f32m2 (src, vl);
}

float
test_vfmv_f_s_f32m4_f32 (vfloat32m4_t src)
{
  return __riscv_vfmv_f_s_f32m4_f32 (src);
}

vfloat32m4_t
test_vfmv_s_f_f32m4 (float src, size_t vl)
{
  return __riscv_vfmv_s_f_f32m4 (src, vl);
}

float
test_vfmv_f_s_f32m8_f32 (vfloat32m8_t src)
{
  return __riscv_vfmv_f_s_f32m8_f32 (src);
}

vfloat32m8_t
test_vfmv_s_f_f32m8 (float src, size_t vl)
{
  return __riscv_vfmv_s_f_f32m8 (src, vl);
}

double
test_vfmv_f_s_f64m1_f64 (vfloat64m1_t src)
{
  return __riscv_vfmv_f_s_f64m1_f64 (src);
}

vfloat64m1_t
test_vfmv_s_f_f64m1 (double src, size_t vl)
{
  return __riscv_vfmv_s_f_f64m1 (src, vl);
}

double
test_vfmv_f_s_f64m2_f64 (vfloat64m2_t src)
{
  return __riscv_vfmv_f_s_f64m2_f64 (src);
}

vfloat64m2_t
test_vfmv_s_f_f64m2 (double src, size_t vl)
{
  return __riscv_vfmv_s_f_f64m2 (src, vl);
}

double
test_vfmv_f_s_f64m4_f64 (vfloat64m4_t src)
{
  return __riscv_vfmv_f_s_f64m4_f64 (src);
}

vfloat64m4_t
test_vfmv_s_f_f64m4 (double src, size_t vl)
{
  return __riscv_vfmv_s_f_f64m4 (src, vl);
}

double
test_vfmv_f_s_f64m8_f64 (vfloat64m8_t src)
{
  return __riscv_vfmv_f_s_f64m8_f64 (src);
}

vfloat64m8_t
test_vfmv_s_f_f64m8 (double src, size_t vl)
{
  return __riscv_vfmv_s_f_f64m8 (src, vl);
}
