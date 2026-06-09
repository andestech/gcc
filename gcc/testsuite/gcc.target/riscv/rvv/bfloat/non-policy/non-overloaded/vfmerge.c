/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfmerge_vfm_bf16mf4 (vbfloat16mf4_t op1, __bf16 op2, vbool64_t mask,
			  size_t vl)
{
  return __riscv_vfmerge_vfm_bf16mf4 (op1, op2, mask, vl);
}

vbfloat16mf2_t
test_vfmerge_vfm_bf16mf2 (vbfloat16mf2_t op1, __bf16 op2, vbool32_t mask,
			  size_t vl)
{
  return __riscv_vfmerge_vfm_bf16mf2 (op1, op2, mask, vl);
}

vbfloat16m1_t
test_vfmerge_vfm_bf16m1 (vbfloat16m1_t op1, __bf16 op2, vbool16_t mask,
			 size_t vl)
{
  return __riscv_vfmerge_vfm_bf16m1 (op1, op2, mask, vl);
}

vbfloat16m2_t
test_vfmerge_vfm_bf16m2 (vbfloat16m2_t op1, __bf16 op2, vbool8_t mask,
			 size_t vl)
{
  return __riscv_vfmerge_vfm_bf16m2 (op1, op2, mask, vl);
}

vbfloat16m4_t
test_vfmerge_vfm_bf16m4 (vbfloat16m4_t op1, __bf16 op2, vbool4_t mask,
			 size_t vl)
{
  return __riscv_vfmerge_vfm_bf16m4 (op1, op2, mask, vl);
}

vbfloat16m8_t
test_vfmerge_vfm_bf16m8 (vbfloat16m8_t op1, __bf16 op2, vbool2_t mask,
			 size_t vl)
{
  return __riscv_vfmerge_vfm_bf16m8 (op1, op2, mask, vl);
}

vfloat32mf2_t
test_vfmerge_vfm_f32mf2 (vfloat32mf2_t op1, float op2, vbool64_t mask,
			 size_t vl)
{
  return __riscv_vfmerge_vfm_f32mf2 (op1, op2, mask, vl);
}

vfloat32m1_t
test_vfmerge_vfm_f32m1 (vfloat32m1_t op1, float op2, vbool32_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f32m1 (op1, op2, mask, vl);
}

vfloat32m2_t
test_vfmerge_vfm_f32m2 (vfloat32m2_t op1, float op2, vbool16_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f32m2 (op1, op2, mask, vl);
}

vfloat32m4_t
test_vfmerge_vfm_f32m4 (vfloat32m4_t op1, float op2, vbool8_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f32m4 (op1, op2, mask, vl);
}

vfloat32m8_t
test_vfmerge_vfm_f32m8 (vfloat32m8_t op1, float op2, vbool4_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f32m8 (op1, op2, mask, vl);
}

vfloat64m1_t
test_vfmerge_vfm_f64m1 (vfloat64m1_t op1, double op2, vbool64_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f64m1 (op1, op2, mask, vl);
}

vfloat64m2_t
test_vfmerge_vfm_f64m2 (vfloat64m2_t op1, double op2, vbool32_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f64m2 (op1, op2, mask, vl);
}

vfloat64m4_t
test_vfmerge_vfm_f64m4 (vfloat64m4_t op1, double op2, vbool16_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f64m4 (op1, op2, mask, vl);
}

vfloat64m8_t
test_vfmerge_vfm_f64m8 (vfloat64m8_t op1, double op2, vbool8_t mask, size_t vl)
{
  return __riscv_vfmerge_vfm_f64m8 (op1, op2, mask, vl);
}
