/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandes_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_tu (vbfloat16mf4_t maskedoff, vbfloat16mf4_t op1,
			    float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_tu (maskedoff, op1, op2, vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_tu (vbfloat16mf2_t maskedoff, vbfloat16mf2_t op1,
			    float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_tu (maskedoff, op1, op2, vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_tu (vbfloat16m1_t maskedoff, vbfloat16m1_t op1,
			   float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_tu (maskedoff, op1, op2, vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_tu (vbfloat16m2_t maskedoff, vbfloat16m2_t op1,
			   float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_tu (maskedoff, op1, op2, vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_tu (vbfloat16m4_t maskedoff, vbfloat16m4_t op1,
			   float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_tu (maskedoff, op1, op2, vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_tu (vbfloat16m8_t maskedoff, vbfloat16m8_t op1,
			   float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_tu (maskedoff, op1, op2, vl);
}

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_tum (vbool64_t mask, vbfloat16mf4_t maskedoff,
			     vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_tum (mask, maskedoff, op1, op2, vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_tum (vbool32_t mask, vbfloat16mf2_t maskedoff,
			     vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_tum (mask, maskedoff, op1, op2, vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_tum (vbool16_t mask, vbfloat16m1_t maskedoff,
			    vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_tum (mask, maskedoff, op1, op2, vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_tum (vbool8_t mask, vbfloat16m2_t maskedoff,
			    vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_tum (mask, maskedoff, op1, op2, vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_tum (vbool4_t mask, vbfloat16m4_t maskedoff,
			    vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_tum (mask, maskedoff, op1, op2, vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_tum (vbool2_t mask, vbfloat16m8_t maskedoff,
			    vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_tum (mask, maskedoff, op1, op2, vl);
}

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_tumu (vbool64_t mask, vbfloat16mf4_t maskedoff,
			      vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_tumu (mask, maskedoff, op1, op2, vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_tumu (vbool32_t mask, vbfloat16mf2_t maskedoff,
			      vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_tumu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_tumu (vbool16_t mask, vbfloat16m1_t maskedoff,
			     vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_tumu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_tumu (vbool8_t mask, vbfloat16m2_t maskedoff,
			     vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_tumu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_tumu (vbool4_t mask, vbfloat16m4_t maskedoff,
			     vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_tumu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_tumu (vbool2_t mask, vbfloat16m8_t maskedoff,
			     vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_tumu (mask, maskedoff, op1, op2, vl);
}

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_mu (vbool64_t mask, vbfloat16mf4_t maskedoff,
			    vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_mu (mask, maskedoff, op1, op2, vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_mu (vbool32_t mask, vbfloat16mf2_t maskedoff,
			    vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_mu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_mu (vbool16_t mask, vbfloat16m1_t maskedoff,
			   vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_mu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_mu (vbool8_t mask, vbfloat16m2_t maskedoff,
			   vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_mu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_mu (vbool4_t mask, vbfloat16m4_t maskedoff,
			   vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_mu (mask, maskedoff, op1, op2, vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_mu (vbool2_t mask, vbfloat16m8_t maskedoff,
			   vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_mu (mask, maskedoff, op1, op2, vl);
}

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_rm_tu (vbfloat16mf4_t maskedoff, vbfloat16mf4_t op1,
			       float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_rm_tu (maskedoff, op1, op2, __RISCV_FRM_RNE,
					   vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_rm_tu (vbfloat16mf2_t maskedoff, vbfloat16mf2_t op1,
			       float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_rm_tu (maskedoff, op1, op2, __RISCV_FRM_RNE,
					   vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_rm_tu (vbfloat16m1_t maskedoff, vbfloat16m1_t op1,
			      float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_rm_tu (maskedoff, op1, op2, __RISCV_FRM_RNE,
					  vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_rm_tu (vbfloat16m2_t maskedoff, vbfloat16m2_t op1,
			      float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_rm_tu (maskedoff, op1, op2, __RISCV_FRM_RNE,
					  vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_rm_tu (vbfloat16m4_t maskedoff, vbfloat16m4_t op1,
			      float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_rm_tu (maskedoff, op1, op2, __RISCV_FRM_RNE,
					  vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_rm_tu (vbfloat16m8_t maskedoff, vbfloat16m8_t op1,
			      float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_rm_tu (maskedoff, op1, op2, __RISCV_FRM_RNE,
					  vl);
}

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_rm_tum (vbool64_t mask, vbfloat16mf4_t maskedoff,
				vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_rm_tum (mask, maskedoff, op1, op2,
					    __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_rm_tum (vbool32_t mask, vbfloat16mf2_t maskedoff,
				vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_rm_tum (mask, maskedoff, op1, op2,
					    __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_rm_tum (vbool16_t mask, vbfloat16m1_t maskedoff,
			       vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_rm_tum (mask, maskedoff, op1, op2,
					   __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_rm_tum (vbool8_t mask, vbfloat16m2_t maskedoff,
			       vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_rm_tum (mask, maskedoff, op1, op2,
					   __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_rm_tum (vbool4_t mask, vbfloat16m4_t maskedoff,
			       vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_rm_tum (mask, maskedoff, op1, op2,
					   __RISCV_FRM_RNE, vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_rm_tum (vbool2_t mask, vbfloat16m8_t maskedoff,
			       vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_rm_tum (mask, maskedoff, op1, op2,
					   __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_rm_tumu (vbool64_t mask, vbfloat16mf4_t maskedoff,
				 vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_rm_tumu (mask, maskedoff, op1, op2,
					     __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_rm_tumu (vbool32_t mask, vbfloat16mf2_t maskedoff,
				 vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_rm_tumu (mask, maskedoff, op1, op2,
					     __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_rm_tumu (vbool16_t mask, vbfloat16m1_t maskedoff,
				vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_rm_tumu (mask, maskedoff, op1, op2,
					    __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_rm_tumu (vbool8_t mask, vbfloat16m2_t maskedoff,
				vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_rm_tumu (mask, maskedoff, op1, op2,
					    __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_rm_tumu (vbool4_t mask, vbfloat16m4_t maskedoff,
				vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_rm_tumu (mask, maskedoff, op1, op2,
					    __RISCV_FRM_RNE, vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_rm_tumu (vbool2_t mask, vbfloat16m8_t maskedoff,
				vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_rm_tumu (mask, maskedoff, op1, op2,
					    __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfpmadt_vf_bf16mf4_rm_mu (vbool64_t mask, vbfloat16mf4_t maskedoff,
			       vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf4_rm_mu (mask, maskedoff, op1, op2,
					   __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfpmadt_vf_bf16mf2_rm_mu (vbool32_t mask, vbfloat16mf2_t maskedoff,
			       vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16mf2_rm_mu (mask, maskedoff, op1, op2,
					   __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfpmadt_vf_bf16m1_rm_mu (vbool16_t mask, vbfloat16m1_t maskedoff,
			      vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m1_rm_mu (mask, maskedoff, op1, op2,
					  __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfpmadt_vf_bf16m2_rm_mu (vbool8_t mask, vbfloat16m2_t maskedoff,
			      vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m2_rm_mu (mask, maskedoff, op1, op2,
					  __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfpmadt_vf_bf16m4_rm_mu (vbool4_t mask, vbfloat16m4_t maskedoff,
			      vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m4_rm_mu (mask, maskedoff, op1, op2,
					  __RISCV_FRM_RNE, vl);
}

vbfloat16m8_t
test_vfpmadt_vf_bf16m8_rm_mu (vbool2_t mask, vbfloat16m8_t maskedoff,
			      vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadt_vf_bf16m8_rm_mu (mask, maskedoff, op1, op2,
					  __RISCV_FRM_RNE, vl);
}
