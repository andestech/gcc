/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16m1_t
test_vfredusum_vs_bf16mf4_bf16m1 (vbfloat16mf4_t vector, vbfloat16m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16mf2_bf16m1 (vbfloat16mf2_t vector, vbfloat16m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m1_bf16m1 (vbfloat16m1_t vector, vbfloat16m1_t scalar,
				 size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m2_bf16m1 (vbfloat16m2_t vector, vbfloat16m1_t scalar,
				 size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m4_bf16m1 (vbfloat16m4_t vector, vbfloat16m1_t scalar,
				 size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m8_bf16m1 (vbfloat16m8_t vector, vbfloat16m1_t scalar,
				 size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32mf2_f32m1 (vfloat32mf2_t vector, vfloat32m1_t scalar,
				size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m1_f32m1 (vfloat32m1_t vector, vfloat32m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m2_f32m1 (vfloat32m2_t vector, vfloat32m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m4_f32m1 (vfloat32m4_t vector, vfloat32m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m8_f32m1 (vfloat32m8_t vector, vfloat32m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m1_f64m1 (vfloat64m1_t vector, vfloat64m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m2_f64m1 (vfloat64m2_t vector, vfloat64m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m4_f64m1 (vfloat64m4_t vector, vfloat64m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m8_f64m1 (vfloat64m8_t vector, vfloat64m1_t scalar,
			       size_t vl)
{
  return __riscv_vfredusum (vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16mf4_bf16m1_m (vbool64_t mask, vbfloat16mf4_t vector,
				    vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16mf2_bf16m1_m (vbool32_t mask, vbfloat16mf2_t vector,
				    vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m1_bf16m1_m (vbool16_t mask, vbfloat16m1_t vector,
				   vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m2_bf16m1_m (vbool8_t mask, vbfloat16m2_t vector,
				   vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m4_bf16m1_m (vbool4_t mask, vbfloat16m4_t vector,
				   vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m8_bf16m1_m (vbool2_t mask, vbfloat16m8_t vector,
				   vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32mf2_f32m1_m (vbool64_t mask, vfloat32mf2_t vector,
				  vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m1_f32m1_m (vbool32_t mask, vfloat32m1_t vector,
				 vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m2_f32m1_m (vbool16_t mask, vfloat32m2_t vector,
				 vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m4_f32m1_m (vbool8_t mask, vfloat32m4_t vector,
				 vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m8_f32m1_m (vbool4_t mask, vfloat32m8_t vector,
				 vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m1_f64m1_m (vbool64_t mask, vfloat64m1_t vector,
				 vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m2_f64m1_m (vbool32_t mask, vfloat64m2_t vector,
				 vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m4_f64m1_m (vbool16_t mask, vfloat64m4_t vector,
				 vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m8_f64m1_m (vbool8_t mask, vfloat64m8_t vector,
				 vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16mf4_bf16m1_rm (vbfloat16mf4_t vector,
				     vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16mf2_bf16m1_rm (vbfloat16mf2_t vector,
				     vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m1_bf16m1_rm (vbfloat16m1_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m2_bf16m1_rm (vbfloat16m2_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m4_bf16m1_rm (vbfloat16m4_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m8_bf16m1_rm (vbfloat16m8_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32mf2_f32m1_rm (vfloat32mf2_t vector, vfloat32m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m1_f32m1_rm (vfloat32m1_t vector, vfloat32m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m2_f32m1_rm (vfloat32m2_t vector, vfloat32m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m4_f32m1_rm (vfloat32m4_t vector, vfloat32m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m8_f32m1_rm (vfloat32m8_t vector, vfloat32m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m1_f64m1_rm (vfloat64m1_t vector, vfloat64m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m2_f64m1_rm (vfloat64m2_t vector, vfloat64m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m4_f64m1_rm (vfloat64m4_t vector, vfloat64m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m8_f64m1_rm (vfloat64m8_t vector, vfloat64m1_t scalar,
				  size_t vl)
{
  return __riscv_vfredusum (vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16mf4_bf16m1_rm_m (vbool64_t mask, vbfloat16mf4_t vector,
				       vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16mf2_bf16m1_rm_m (vbool32_t mask, vbfloat16mf2_t vector,
				       vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m1_bf16m1_rm_m (vbool16_t mask, vbfloat16m1_t vector,
				      vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m2_bf16m1_rm_m (vbool8_t mask, vbfloat16m2_t vector,
				      vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m4_bf16m1_rm_m (vbool4_t mask, vbfloat16m4_t vector,
				      vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredusum_vs_bf16m8_bf16m1_rm_m (vbool2_t mask, vbfloat16m8_t vector,
				      vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32mf2_f32m1_rm_m (vbool64_t mask, vfloat32mf2_t vector,
				     vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m1_f32m1_rm_m (vbool32_t mask, vfloat32m1_t vector,
				    vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m2_f32m1_rm_m (vbool16_t mask, vfloat32m2_t vector,
				    vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m4_f32m1_rm_m (vbool8_t mask, vfloat32m4_t vector,
				    vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredusum_vs_f32m8_f32m1_rm_m (vbool4_t mask, vfloat32m8_t vector,
				    vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m1_f64m1_rm_m (vbool64_t mask, vfloat64m1_t vector,
				    vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m2_f64m1_rm_m (vbool32_t mask, vfloat64m2_t vector,
				    vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m4_f64m1_rm_m (vbool16_t mask, vfloat64m4_t vector,
				    vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredusum_vs_f64m8_f64m1_rm_m (vbool8_t mask, vfloat64m8_t vector,
				    vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredusum (mask, vector, scalar, __RISCV_FRM_RNE, vl);
}
