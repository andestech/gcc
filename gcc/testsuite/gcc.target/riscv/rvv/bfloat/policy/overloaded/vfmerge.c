/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfmerge_vfm_bf16mf4_tu (vbfloat16mf4_t maskedoff, vbfloat16mf4_t op1,
			     __bf16 op2, vbool64_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vbfloat16mf2_t
test_vfmerge_vfm_bf16mf2_tu (vbfloat16mf2_t maskedoff, vbfloat16mf2_t op1,
			     __bf16 op2, vbool32_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vbfloat16m1_t
test_vfmerge_vfm_bf16m1_tu (vbfloat16m1_t maskedoff, vbfloat16m1_t op1,
			    __bf16 op2, vbool16_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vbfloat16m2_t
test_vfmerge_vfm_bf16m2_tu (vbfloat16m2_t maskedoff, vbfloat16m2_t op1,
			    __bf16 op2, vbool8_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vbfloat16m4_t
test_vfmerge_vfm_bf16m4_tu (vbfloat16m4_t maskedoff, vbfloat16m4_t op1,
			    __bf16 op2, vbool4_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vbfloat16m8_t
test_vfmerge_vfm_bf16m8_tu (vbfloat16m8_t maskedoff, vbfloat16m8_t op1,
			    __bf16 op2, vbool2_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat32mf2_t
test_vfmerge_vfm_f32mf2_tu (vfloat32mf2_t maskedoff, vfloat32mf2_t op1,
			    float op2, vbool64_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat32m1_t
test_vfmerge_vfm_f32m1_tu (vfloat32m1_t maskedoff, vfloat32m1_t op1, float op2,
			   vbool32_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat32m2_t
test_vfmerge_vfm_f32m2_tu (vfloat32m2_t maskedoff, vfloat32m2_t op1, float op2,
			   vbool16_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat32m4_t
test_vfmerge_vfm_f32m4_tu (vfloat32m4_t maskedoff, vfloat32m4_t op1, float op2,
			   vbool8_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat32m8_t
test_vfmerge_vfm_f32m8_tu (vfloat32m8_t maskedoff, vfloat32m8_t op1, float op2,
			   vbool4_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat64m1_t
test_vfmerge_vfm_f64m1_tu (vfloat64m1_t maskedoff, vfloat64m1_t op1, double op2,
			   vbool64_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat64m2_t
test_vfmerge_vfm_f64m2_tu (vfloat64m2_t maskedoff, vfloat64m2_t op1, double op2,
			   vbool32_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat64m4_t
test_vfmerge_vfm_f64m4_tu (vfloat64m4_t maskedoff, vfloat64m4_t op1, double op2,
			   vbool16_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}

vfloat64m8_t
test_vfmerge_vfm_f64m8_tu (vfloat64m8_t maskedoff, vfloat64m8_t op1, double op2,
			   vbool8_t mask, size_t vl)
{
  return __riscv_vfmerge_tu (maskedoff, op1, op2, mask, vl);
}
