/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16m1_t
test_vfredosum_vs_bf16mf4_bf16m1_tu (vbfloat16m1_t maskedoff,
				     vbfloat16mf4_t vector,
				     vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16mf2_bf16m1_tu (vbfloat16m1_t maskedoff,
				     vbfloat16mf2_t vector,
				     vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m1_bf16m1_tu (vbfloat16m1_t maskedoff,
				    vbfloat16m1_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m2_bf16m1_tu (vbfloat16m1_t maskedoff,
				    vbfloat16m2_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m4_bf16m1_tu (vbfloat16m1_t maskedoff,
				    vbfloat16m4_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m8_bf16m1_tu (vbfloat16m1_t maskedoff,
				    vbfloat16m8_t vector, vbfloat16m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32mf2_f32m1_tu (vfloat32m1_t maskedoff, vfloat32mf2_t vector,
				   vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m1_f32m1_tu (vfloat32m1_t maskedoff, vfloat32m1_t vector,
				  vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m2_f32m1_tu (vfloat32m1_t maskedoff, vfloat32m2_t vector,
				  vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m4_f32m1_tu (vfloat32m1_t maskedoff, vfloat32m4_t vector,
				  vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m8_f32m1_tu (vfloat32m1_t maskedoff, vfloat32m8_t vector,
				  vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m1_f64m1_tu (vfloat64m1_t maskedoff, vfloat64m1_t vector,
				  vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m2_f64m1_tu (vfloat64m1_t maskedoff, vfloat64m2_t vector,
				  vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m4_f64m1_tu (vfloat64m1_t maskedoff, vfloat64m4_t vector,
				  vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m8_f64m1_tu (vfloat64m1_t maskedoff, vfloat64m8_t vector,
				  vfloat64m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16mf4_bf16m1_tum (vbool64_t mask, vbfloat16m1_t maskedoff,
				      vbfloat16mf4_t vector,
				      vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16mf2_bf16m1_tum (vbool32_t mask, vbfloat16m1_t maskedoff,
				      vbfloat16mf2_t vector,
				      vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m1_bf16m1_tum (vbool16_t mask, vbfloat16m1_t maskedoff,
				     vbfloat16m1_t vector, vbfloat16m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m2_bf16m1_tum (vbool8_t mask, vbfloat16m1_t maskedoff,
				     vbfloat16m2_t vector, vbfloat16m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m4_bf16m1_tum (vbool4_t mask, vbfloat16m1_t maskedoff,
				     vbfloat16m4_t vector, vbfloat16m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m8_bf16m1_tum (vbool2_t mask, vbfloat16m1_t maskedoff,
				     vbfloat16m8_t vector, vbfloat16m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32mf2_f32m1_tum (vbool64_t mask, vfloat32m1_t maskedoff,
				    vfloat32mf2_t vector, vfloat32m1_t scalar,
				    size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m1_f32m1_tum (vbool32_t mask, vfloat32m1_t maskedoff,
				   vfloat32m1_t vector, vfloat32m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m2_f32m1_tum (vbool16_t mask, vfloat32m1_t maskedoff,
				   vfloat32m2_t vector, vfloat32m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m4_f32m1_tum (vbool8_t mask, vfloat32m1_t maskedoff,
				   vfloat32m4_t vector, vfloat32m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m8_f32m1_tum (vbool4_t mask, vfloat32m1_t maskedoff,
				   vfloat32m8_t vector, vfloat32m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m1_f64m1_tum (vbool64_t mask, vfloat64m1_t maskedoff,
				   vfloat64m1_t vector, vfloat64m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m2_f64m1_tum (vbool32_t mask, vfloat64m1_t maskedoff,
				   vfloat64m2_t vector, vfloat64m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m4_f64m1_tum (vbool16_t mask, vfloat64m1_t maskedoff,
				   vfloat64m4_t vector, vfloat64m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m8_f64m1_tum (vbool8_t mask, vfloat64m1_t maskedoff,
				   vfloat64m8_t vector, vfloat64m1_t scalar,
				   size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16mf4_bf16m1_rm_tu (vbfloat16m1_t maskedoff,
					vbfloat16mf4_t vector,
					vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16mf2_bf16m1_rm_tu (vbfloat16m1_t maskedoff,
					vbfloat16mf2_t vector,
					vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m1_bf16m1_rm_tu (vbfloat16m1_t maskedoff,
				       vbfloat16m1_t vector,
				       vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m2_bf16m1_rm_tu (vbfloat16m1_t maskedoff,
				       vbfloat16m2_t vector,
				       vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m4_bf16m1_rm_tu (vbfloat16m1_t maskedoff,
				       vbfloat16m4_t vector,
				       vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m8_bf16m1_rm_tu (vbfloat16m1_t maskedoff,
				       vbfloat16m8_t vector,
				       vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32mf2_f32m1_rm_tu (vfloat32m1_t maskedoff,
				      vfloat32mf2_t vector, vfloat32m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m1_f32m1_rm_tu (vfloat32m1_t maskedoff,
				     vfloat32m1_t vector, vfloat32m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m2_f32m1_rm_tu (vfloat32m1_t maskedoff,
				     vfloat32m2_t vector, vfloat32m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m4_f32m1_rm_tu (vfloat32m1_t maskedoff,
				     vfloat32m4_t vector, vfloat32m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m8_f32m1_rm_tu (vfloat32m1_t maskedoff,
				     vfloat32m8_t vector, vfloat32m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m1_f64m1_rm_tu (vfloat64m1_t maskedoff,
				     vfloat64m1_t vector, vfloat64m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m2_f64m1_rm_tu (vfloat64m1_t maskedoff,
				     vfloat64m2_t vector, vfloat64m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m4_f64m1_rm_tu (vfloat64m1_t maskedoff,
				     vfloat64m4_t vector, vfloat64m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m8_f64m1_rm_tu (vfloat64m1_t maskedoff,
				     vfloat64m8_t vector, vfloat64m1_t scalar,
				     size_t vl)
{
  return __riscv_vfredosum_tu (maskedoff, vector, scalar, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16mf4_bf16m1_rm_tum (vbool64_t mask,
					 vbfloat16m1_t maskedoff,
					 vbfloat16mf4_t vector,
					 vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16mf2_bf16m1_rm_tum (vbool32_t mask,
					 vbfloat16m1_t maskedoff,
					 vbfloat16mf2_t vector,
					 vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m1_bf16m1_rm_tum (vbool16_t mask, vbfloat16m1_t maskedoff,
					vbfloat16m1_t vector,
					vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m2_bf16m1_rm_tum (vbool8_t mask, vbfloat16m1_t maskedoff,
					vbfloat16m2_t vector,
					vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m4_bf16m1_rm_tum (vbool4_t mask, vbfloat16m1_t maskedoff,
					vbfloat16m4_t vector,
					vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfredosum_vs_bf16m8_bf16m1_rm_tum (vbool2_t mask, vbfloat16m1_t maskedoff,
					vbfloat16m8_t vector,
					vbfloat16m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32mf2_f32m1_rm_tum (vbool64_t mask, vfloat32m1_t maskedoff,
				       vfloat32mf2_t vector,
				       vfloat32m1_t scalar, size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m1_f32m1_rm_tum (vbool32_t mask, vfloat32m1_t maskedoff,
				      vfloat32m1_t vector, vfloat32m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m2_f32m1_rm_tum (vbool16_t mask, vfloat32m1_t maskedoff,
				      vfloat32m2_t vector, vfloat32m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m4_f32m1_rm_tum (vbool8_t mask, vfloat32m1_t maskedoff,
				      vfloat32m4_t vector, vfloat32m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfredosum_vs_f32m8_f32m1_rm_tum (vbool4_t mask, vfloat32m1_t maskedoff,
				      vfloat32m8_t vector, vfloat32m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m1_f64m1_rm_tum (vbool64_t mask, vfloat64m1_t maskedoff,
				      vfloat64m1_t vector, vfloat64m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m2_f64m1_rm_tum (vbool32_t mask, vfloat64m1_t maskedoff,
				      vfloat64m2_t vector, vfloat64m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m4_f64m1_rm_tum (vbool16_t mask, vfloat64m1_t maskedoff,
				      vfloat64m4_t vector, vfloat64m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}

vfloat64m1_t
test_vfredosum_vs_f64m8_f64m1_rm_tum (vbool8_t mask, vfloat64m1_t maskedoff,
				      vfloat64m8_t vector, vfloat64m1_t scalar,
				      size_t vl)
{
  return __riscv_vfredosum_tum (mask, maskedoff, vector, scalar,
				__RISCV_FRM_RNE, vl);
}
