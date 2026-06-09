/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbool64_t
test_vmflt_vv_bf16mf4_b64 (vbfloat16mf4_t op1, vbfloat16mf4_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16mf4_b64 (op1, op2, vl);
}

vbool64_t
test_vmflt_vf_bf16mf4_b64 (vbfloat16mf4_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vmflt_vf_bf16mf4_b64 (op1, op2, vl);
}

vbool32_t
test_vmflt_vv_bf16mf2_b32 (vbfloat16mf2_t op1, vbfloat16mf2_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16mf2_b32 (op1, op2, vl);
}

vbool32_t
test_vmflt_vf_bf16mf2_b32 (vbfloat16mf2_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vmflt_vf_bf16mf2_b32 (op1, op2, vl);
}

vbool16_t
test_vmflt_vv_bf16m1_b16 (vbfloat16m1_t op1, vbfloat16m1_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16m1_b16 (op1, op2, vl);
}

vbool16_t
test_vmflt_vf_bf16m1_b16 (vbfloat16m1_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vmflt_vf_bf16m1_b16 (op1, op2, vl);
}

vbool8_t
test_vmflt_vv_bf16m2_b8 (vbfloat16m2_t op1, vbfloat16m2_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16m2_b8 (op1, op2, vl);
}

vbool8_t
test_vmflt_vf_bf16m2_b8 (vbfloat16m2_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vmflt_vf_bf16m2_b8 (op1, op2, vl);
}

vbool4_t
test_vmflt_vv_bf16m4_b4 (vbfloat16m4_t op1, vbfloat16m4_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16m4_b4 (op1, op2, vl);
}

vbool4_t
test_vmflt_vf_bf16m4_b4 (vbfloat16m4_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vmflt_vf_bf16m4_b4 (op1, op2, vl);
}

vbool2_t
test_vmflt_vv_bf16m8_b2 (vbfloat16m8_t op1, vbfloat16m8_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16m8_b2 (op1, op2, vl);
}

vbool2_t
test_vmflt_vf_bf16m8_b2 (vbfloat16m8_t op1, __bf16 op2, size_t vl)
{
  return __riscv_vmflt_vf_bf16m8_b2 (op1, op2, vl);
}

vbool64_t
test_vmflt_vv_f32mf2_b64 (vfloat32mf2_t op1, vfloat32mf2_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f32mf2_b64 (op1, op2, vl);
}

vbool64_t
test_vmflt_vf_f32mf2_b64 (vfloat32mf2_t op1, float op2, size_t vl)
{
  return __riscv_vmflt_vf_f32mf2_b64 (op1, op2, vl);
}

vbool32_t
test_vmflt_vv_f32m1_b32 (vfloat32m1_t op1, vfloat32m1_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f32m1_b32 (op1, op2, vl);
}

vbool32_t
test_vmflt_vf_f32m1_b32 (vfloat32m1_t op1, float op2, size_t vl)
{
  return __riscv_vmflt_vf_f32m1_b32 (op1, op2, vl);
}

vbool16_t
test_vmflt_vv_f32m2_b16 (vfloat32m2_t op1, vfloat32m2_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f32m2_b16 (op1, op2, vl);
}

vbool16_t
test_vmflt_vf_f32m2_b16 (vfloat32m2_t op1, float op2, size_t vl)
{
  return __riscv_vmflt_vf_f32m2_b16 (op1, op2, vl);
}

vbool8_t
test_vmflt_vv_f32m4_b8 (vfloat32m4_t op1, vfloat32m4_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f32m4_b8 (op1, op2, vl);
}

vbool8_t
test_vmflt_vf_f32m4_b8 (vfloat32m4_t op1, float op2, size_t vl)
{
  return __riscv_vmflt_vf_f32m4_b8 (op1, op2, vl);
}

vbool4_t
test_vmflt_vv_f32m8_b4 (vfloat32m8_t op1, vfloat32m8_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f32m8_b4 (op1, op2, vl);
}

vbool4_t
test_vmflt_vf_f32m8_b4 (vfloat32m8_t op1, float op2, size_t vl)
{
  return __riscv_vmflt_vf_f32m8_b4 (op1, op2, vl);
}

vbool64_t
test_vmflt_vv_f64m1_b64 (vfloat64m1_t op1, vfloat64m1_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f64m1_b64 (op1, op2, vl);
}

vbool64_t
test_vmflt_vf_f64m1_b64 (vfloat64m1_t op1, double op2, size_t vl)
{
  return __riscv_vmflt_vf_f64m1_b64 (op1, op2, vl);
}

vbool32_t
test_vmflt_vv_f64m2_b32 (vfloat64m2_t op1, vfloat64m2_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f64m2_b32 (op1, op2, vl);
}

vbool32_t
test_vmflt_vf_f64m2_b32 (vfloat64m2_t op1, double op2, size_t vl)
{
  return __riscv_vmflt_vf_f64m2_b32 (op1, op2, vl);
}

vbool16_t
test_vmflt_vv_f64m4_b16 (vfloat64m4_t op1, vfloat64m4_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f64m4_b16 (op1, op2, vl);
}

vbool16_t
test_vmflt_vf_f64m4_b16 (vfloat64m4_t op1, double op2, size_t vl)
{
  return __riscv_vmflt_vf_f64m4_b16 (op1, op2, vl);
}

vbool8_t
test_vmflt_vv_f64m8_b8 (vfloat64m8_t op1, vfloat64m8_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f64m8_b8 (op1, op2, vl);
}

vbool8_t
test_vmflt_vf_f64m8_b8 (vfloat64m8_t op1, double op2, size_t vl)
{
  return __riscv_vmflt_vf_f64m8_b8 (op1, op2, vl);
}

vbool64_t
test_vmflt_vv_bf16mf4_b64_m (vbool64_t mask, vbfloat16mf4_t op1,
			     vbfloat16mf4_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16mf4_b64_m (mask, op1, op2, vl);
}

vbool64_t
test_vmflt_vf_bf16mf4_b64_m (vbool64_t mask, vbfloat16mf4_t op1, __bf16 op2,
			     size_t vl)
{
  return __riscv_vmflt_vf_bf16mf4_b64_m (mask, op1, op2, vl);
}

vbool32_t
test_vmflt_vv_bf16mf2_b32_m (vbool32_t mask, vbfloat16mf2_t op1,
			     vbfloat16mf2_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16mf2_b32_m (mask, op1, op2, vl);
}

vbool32_t
test_vmflt_vf_bf16mf2_b32_m (vbool32_t mask, vbfloat16mf2_t op1, __bf16 op2,
			     size_t vl)
{
  return __riscv_vmflt_vf_bf16mf2_b32_m (mask, op1, op2, vl);
}

vbool16_t
test_vmflt_vv_bf16m1_b16_m (vbool16_t mask, vbfloat16m1_t op1,
			    vbfloat16m1_t op2, size_t vl)
{
  return __riscv_vmflt_vv_bf16m1_b16_m (mask, op1, op2, vl);
}

vbool16_t
test_vmflt_vf_bf16m1_b16_m (vbool16_t mask, vbfloat16m1_t op1, __bf16 op2,
			    size_t vl)
{
  return __riscv_vmflt_vf_bf16m1_b16_m (mask, op1, op2, vl);
}

vbool8_t
test_vmflt_vv_bf16m2_b8_m (vbool8_t mask, vbfloat16m2_t op1, vbfloat16m2_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_bf16m2_b8_m (mask, op1, op2, vl);
}

vbool8_t
test_vmflt_vf_bf16m2_b8_m (vbool8_t mask, vbfloat16m2_t op1, __bf16 op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_bf16m2_b8_m (mask, op1, op2, vl);
}

vbool4_t
test_vmflt_vv_bf16m4_b4_m (vbool4_t mask, vbfloat16m4_t op1, vbfloat16m4_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_bf16m4_b4_m (mask, op1, op2, vl);
}

vbool4_t
test_vmflt_vf_bf16m4_b4_m (vbool4_t mask, vbfloat16m4_t op1, __bf16 op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_bf16m4_b4_m (mask, op1, op2, vl);
}

vbool2_t
test_vmflt_vv_bf16m8_b2_m (vbool2_t mask, vbfloat16m8_t op1, vbfloat16m8_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_bf16m8_b2_m (mask, op1, op2, vl);
}

vbool2_t
test_vmflt_vf_bf16m8_b2_m (vbool2_t mask, vbfloat16m8_t op1, __bf16 op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_bf16m8_b2_m (mask, op1, op2, vl);
}

vbool64_t
test_vmflt_vv_f32mf2_b64_m (vbool64_t mask, vfloat32mf2_t op1,
			    vfloat32mf2_t op2, size_t vl)
{
  return __riscv_vmflt_vv_f32mf2_b64_m (mask, op1, op2, vl);
}

vbool64_t
test_vmflt_vf_f32mf2_b64_m (vbool64_t mask, vfloat32mf2_t op1, float op2,
			    size_t vl)
{
  return __riscv_vmflt_vf_f32mf2_b64_m (mask, op1, op2, vl);
}

vbool32_t
test_vmflt_vv_f32m1_b32_m (vbool32_t mask, vfloat32m1_t op1, vfloat32m1_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_f32m1_b32_m (mask, op1, op2, vl);
}

vbool32_t
test_vmflt_vf_f32m1_b32_m (vbool32_t mask, vfloat32m1_t op1, float op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_f32m1_b32_m (mask, op1, op2, vl);
}

vbool16_t
test_vmflt_vv_f32m2_b16_m (vbool16_t mask, vfloat32m2_t op1, vfloat32m2_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_f32m2_b16_m (mask, op1, op2, vl);
}

vbool16_t
test_vmflt_vf_f32m2_b16_m (vbool16_t mask, vfloat32m2_t op1, float op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_f32m2_b16_m (mask, op1, op2, vl);
}

vbool8_t
test_vmflt_vv_f32m4_b8_m (vbool8_t mask, vfloat32m4_t op1, vfloat32m4_t op2,
			  size_t vl)
{
  return __riscv_vmflt_vv_f32m4_b8_m (mask, op1, op2, vl);
}

vbool8_t
test_vmflt_vf_f32m4_b8_m (vbool8_t mask, vfloat32m4_t op1, float op2, size_t vl)
{
  return __riscv_vmflt_vf_f32m4_b8_m (mask, op1, op2, vl);
}

vbool4_t
test_vmflt_vv_f32m8_b4_m (vbool4_t mask, vfloat32m8_t op1, vfloat32m8_t op2,
			  size_t vl)
{
  return __riscv_vmflt_vv_f32m8_b4_m (mask, op1, op2, vl);
}

vbool4_t
test_vmflt_vf_f32m8_b4_m (vbool4_t mask, vfloat32m8_t op1, float op2, size_t vl)
{
  return __riscv_vmflt_vf_f32m8_b4_m (mask, op1, op2, vl);
}

vbool64_t
test_vmflt_vv_f64m1_b64_m (vbool64_t mask, vfloat64m1_t op1, vfloat64m1_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_f64m1_b64_m (mask, op1, op2, vl);
}

vbool64_t
test_vmflt_vf_f64m1_b64_m (vbool64_t mask, vfloat64m1_t op1, double op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_f64m1_b64_m (mask, op1, op2, vl);
}

vbool32_t
test_vmflt_vv_f64m2_b32_m (vbool32_t mask, vfloat64m2_t op1, vfloat64m2_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_f64m2_b32_m (mask, op1, op2, vl);
}

vbool32_t
test_vmflt_vf_f64m2_b32_m (vbool32_t mask, vfloat64m2_t op1, double op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_f64m2_b32_m (mask, op1, op2, vl);
}

vbool16_t
test_vmflt_vv_f64m4_b16_m (vbool16_t mask, vfloat64m4_t op1, vfloat64m4_t op2,
			   size_t vl)
{
  return __riscv_vmflt_vv_f64m4_b16_m (mask, op1, op2, vl);
}

vbool16_t
test_vmflt_vf_f64m4_b16_m (vbool16_t mask, vfloat64m4_t op1, double op2,
			   size_t vl)
{
  return __riscv_vmflt_vf_f64m4_b16_m (mask, op1, op2, vl);
}

vbool8_t
test_vmflt_vv_f64m8_b8_m (vbool8_t mask, vfloat64m8_t op1, vfloat64m8_t op2,
			  size_t vl)
{
  return __riscv_vmflt_vv_f64m8_b8_m (mask, op1, op2, vl);
}

vbool8_t
test_vmflt_vf_f64m8_b8_m (vbool8_t mask, vfloat64m8_t op1, double op2,
			  size_t vl)
{
  return __riscv_vmflt_vf_f64m8_b8_m (mask, op1, op2, vl);
}
