/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandes_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfpmadb_vf_bf16mf4 (vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, vl);
}

vbfloat16mf2_t
test_vfpmadb_vf_bf16mf2 (vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, vl);
}

vbfloat16m1_t
test_vfpmadb_vf_bf16m1 (vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, vl);
}

vbfloat16m2_t
test_vfpmadb_vf_bf16m2 (vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, vl);
}

vbfloat16m4_t
test_vfpmadb_vf_bf16m4 (vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, vl);
}

vbfloat16m8_t
test_vfpmadb_vf_bf16m8 (vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, vl);
}

vbfloat16mf4_t
test_vfpmadb_vf_bf16mf4_m (vbool64_t mask, vbfloat16mf4_t op1, float op2,
			   size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, vl);
}

vbfloat16mf2_t
test_vfpmadb_vf_bf16mf2_m (vbool32_t mask, vbfloat16mf2_t op1, float op2,
			   size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, vl);
}

vbfloat16m1_t
test_vfpmadb_vf_bf16m1_m (vbool16_t mask, vbfloat16m1_t op1, float op2,
			  size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, vl);
}

vbfloat16m2_t
test_vfpmadb_vf_bf16m2_m (vbool8_t mask, vbfloat16m2_t op1, float op2,
			  size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, vl);
}

vbfloat16m4_t
test_vfpmadb_vf_bf16m4_m (vbool4_t mask, vbfloat16m4_t op1, float op2,
			  size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, vl);
}

vbfloat16m8_t
test_vfpmadb_vf_bf16m8_m (vbool2_t mask, vbfloat16m8_t op1, float op2,
			  size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, vl);
}

vbfloat16mf4_t
test_vfpmadb_vf_bf16mf4_rm (vbfloat16mf4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfpmadb_vf_bf16mf2_rm (vbfloat16mf2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfpmadb_vf_bf16m1_rm (vbfloat16m1_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfpmadb_vf_bf16m2_rm (vbfloat16m2_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfpmadb_vf_bf16m4_rm (vbfloat16m4_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m8_t
test_vfpmadb_vf_bf16m8_rm (vbfloat16m8_t op1, float op2, size_t vl)
{
  return __riscv_vfpmadb (op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfpmadb_vf_bf16mf4_rm_m (vbool64_t mask, vbfloat16mf4_t op1, float op2,
			      size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfpmadb_vf_bf16mf2_rm_m (vbool32_t mask, vbfloat16mf2_t op1, float op2,
			      size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfpmadb_vf_bf16m1_rm_m (vbool16_t mask, vbfloat16m1_t op1, float op2,
			     size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfpmadb_vf_bf16m2_rm_m (vbool8_t mask, vbfloat16m2_t op1, float op2,
			     size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfpmadb_vf_bf16m4_rm_m (vbool4_t mask, vbfloat16m4_t op1, float op2,
			     size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, __RISCV_FRM_RNE, vl);
}

vbfloat16m8_t
test_vfpmadb_vf_bf16m8_rm_m (vbool2_t mask, vbfloat16m8_t op1, float op2,
			     size_t vl)
{
  return __riscv_vfpmadb (mask, op1, op2, __RISCV_FRM_RNE, vl);
}
