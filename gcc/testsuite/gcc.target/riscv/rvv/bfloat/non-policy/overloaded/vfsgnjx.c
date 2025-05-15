/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfsgnjx_vv_bf16mf4 (vbfloat16mf4_t op1, vbfloat16mf4_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16mf4_t
test_vfsgnjx_vf_bf16mf4 (vbfloat16mf4_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16mf2_t
test_vfsgnjx_vv_bf16mf2 (vbfloat16mf2_t op1, vbfloat16mf2_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16mf2_t
test_vfsgnjx_vf_bf16mf2 (vbfloat16mf2_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m1_t
test_vfsgnjx_vv_bf16m1 (vbfloat16m1_t op1, vbfloat16m1_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m1_t
test_vfsgnjx_vf_bf16m1 (vbfloat16m1_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m2_t
test_vfsgnjx_vv_bf16m2 (vbfloat16m2_t op1, vbfloat16m2_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m2_t
test_vfsgnjx_vf_bf16m2 (vbfloat16m2_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m4_t
test_vfsgnjx_vv_bf16m4 (vbfloat16m4_t op1, vbfloat16m4_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m4_t
test_vfsgnjx_vf_bf16m4 (vbfloat16m4_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m8_t
test_vfsgnjx_vv_bf16m8 (vbfloat16m8_t op1, vbfloat16m8_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16m8_t
test_vfsgnjx_vf_bf16m8 (vbfloat16m8_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32mf2_t
test_vfsgnjx_vv_f32mf2 (vfloat32mf2_t op1, vfloat32mf2_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32mf2_t
test_vfsgnjx_vf_f32mf2 (vfloat32mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m1_t
test_vfsgnjx_vv_f32m1 (vfloat32m1_t op1, vfloat32m1_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m1_t
test_vfsgnjx_vf_f32m1 (vfloat32m1_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m2_t
test_vfsgnjx_vv_f32m2 (vfloat32m2_t op1, vfloat32m2_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m2_t
test_vfsgnjx_vf_f32m2 (vfloat32m2_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m4_t
test_vfsgnjx_vv_f32m4 (vfloat32m4_t op1, vfloat32m4_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m4_t
test_vfsgnjx_vf_f32m4 (vfloat32m4_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m8_t
test_vfsgnjx_vv_f32m8 (vfloat32m8_t op1, vfloat32m8_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat32m8_t
test_vfsgnjx_vf_f32m8 (vfloat32m8_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m1_t
test_vfsgnjx_vv_f64m1 (vfloat64m1_t op1, vfloat64m1_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m1_t
test_vfsgnjx_vf_f64m1 (vfloat64m1_t op1, double op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m2_t
test_vfsgnjx_vv_f64m2 (vfloat64m2_t op1, vfloat64m2_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m2_t
test_vfsgnjx_vf_f64m2 (vfloat64m2_t op1, double op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m4_t
test_vfsgnjx_vv_f64m4 (vfloat64m4_t op1, vfloat64m4_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m4_t
test_vfsgnjx_vf_f64m4 (vfloat64m4_t op1, double op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m8_t
test_vfsgnjx_vv_f64m8 (vfloat64m8_t op1, vfloat64m8_t op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vfloat64m8_t
test_vfsgnjx_vf_f64m8 (vfloat64m8_t op1, double op2, size_t vl)
{
  return __riscv_vfsgnjx (op1, op2, vl);
}

vbfloat16mf4_t
test_vfsgnjx_vv_bf16mf4_m (vbool64_t mask, vbfloat16mf4_t op1,
			   vbfloat16mf4_t op2, size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16mf4_t
test_vfsgnjx_vf_bf16mf4_m (vbool64_t mask, vbfloat16mf4_t op1, __bf16 op2,
			   size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16mf2_t
test_vfsgnjx_vv_bf16mf2_m (vbool32_t mask, vbfloat16mf2_t op1,
			   vbfloat16mf2_t op2, size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16mf2_t
test_vfsgnjx_vf_bf16mf2_m (vbool32_t mask, vbfloat16mf2_t op1, __bf16 op2,
			   size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m1_t
test_vfsgnjx_vv_bf16m1_m (vbool16_t mask, vbfloat16m1_t op1, vbfloat16m1_t op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m1_t
test_vfsgnjx_vf_bf16m1_m (vbool16_t mask, vbfloat16m1_t op1, __bf16 op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m2_t
test_vfsgnjx_vv_bf16m2_m (vbool8_t mask, vbfloat16m2_t op1, vbfloat16m2_t op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m2_t
test_vfsgnjx_vf_bf16m2_m (vbool8_t mask, vbfloat16m2_t op1, __bf16 op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m4_t
test_vfsgnjx_vv_bf16m4_m (vbool4_t mask, vbfloat16m4_t op1, vbfloat16m4_t op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m4_t
test_vfsgnjx_vf_bf16m4_m (vbool4_t mask, vbfloat16m4_t op1, __bf16 op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m8_t
test_vfsgnjx_vv_bf16m8_m (vbool2_t mask, vbfloat16m8_t op1, vbfloat16m8_t op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vbfloat16m8_t
test_vfsgnjx_vf_bf16m8_m (vbool2_t mask, vbfloat16m8_t op1, __bf16 op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32mf2_t
test_vfsgnjx_vv_f32mf2_m (vbool64_t mask, vfloat32mf2_t op1, vfloat32mf2_t op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32mf2_t
test_vfsgnjx_vf_f32mf2_m (vbool64_t mask, vfloat32mf2_t op1, float op2,
			  size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m1_t
test_vfsgnjx_vv_f32m1_m (vbool32_t mask, vfloat32m1_t op1, vfloat32m1_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m1_t
test_vfsgnjx_vf_f32m1_m (vbool32_t mask, vfloat32m1_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m2_t
test_vfsgnjx_vv_f32m2_m (vbool16_t mask, vfloat32m2_t op1, vfloat32m2_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m2_t
test_vfsgnjx_vf_f32m2_m (vbool16_t mask, vfloat32m2_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m4_t
test_vfsgnjx_vv_f32m4_m (vbool8_t mask, vfloat32m4_t op1, vfloat32m4_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m4_t
test_vfsgnjx_vf_f32m4_m (vbool8_t mask, vfloat32m4_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m8_t
test_vfsgnjx_vv_f32m8_m (vbool4_t mask, vfloat32m8_t op1, vfloat32m8_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat32m8_t
test_vfsgnjx_vf_f32m8_m (vbool4_t mask, vfloat32m8_t op1, float op2, size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m1_t
test_vfsgnjx_vv_f64m1_m (vbool64_t mask, vfloat64m1_t op1, vfloat64m1_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m1_t
test_vfsgnjx_vf_f64m1_m (vbool64_t mask, vfloat64m1_t op1, double op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m2_t
test_vfsgnjx_vv_f64m2_m (vbool32_t mask, vfloat64m2_t op1, vfloat64m2_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m2_t
test_vfsgnjx_vf_f64m2_m (vbool32_t mask, vfloat64m2_t op1, double op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m4_t
test_vfsgnjx_vv_f64m4_m (vbool16_t mask, vfloat64m4_t op1, vfloat64m4_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m4_t
test_vfsgnjx_vf_f64m4_m (vbool16_t mask, vfloat64m4_t op1, double op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m8_t
test_vfsgnjx_vv_f64m8_m (vbool8_t mask, vfloat64m8_t op1, vfloat64m8_t op2,
			 size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}

vfloat64m8_t
test_vfsgnjx_vf_f64m8_m (vbool8_t mask, vfloat64m8_t op1, double op2, size_t vl)
{
  return __riscv_vfsgnjx (mask, op1, op2, vl);
}
