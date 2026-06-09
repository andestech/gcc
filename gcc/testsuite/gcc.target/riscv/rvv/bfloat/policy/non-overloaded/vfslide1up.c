/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vfslide1up_vf_bf16mf4_tu (vbfloat16mf4_t maskedoff, vbfloat16mf4_t src,
			       __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf4_tu (maskedoff, src, value, vl);
}

vbfloat16mf2_t
test_vfslide1up_vf_bf16mf2_tu (vbfloat16mf2_t maskedoff, vbfloat16mf2_t src,
			       __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf2_tu (maskedoff, src, value, vl);
}

vbfloat16m1_t
test_vfslide1up_vf_bf16m1_tu (vbfloat16m1_t maskedoff, vbfloat16m1_t src,
			      __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m1_tu (maskedoff, src, value, vl);
}

vbfloat16m2_t
test_vfslide1up_vf_bf16m2_tu (vbfloat16m2_t maskedoff, vbfloat16m2_t src,
			      __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m2_tu (maskedoff, src, value, vl);
}

vbfloat16m4_t
test_vfslide1up_vf_bf16m4_tu (vbfloat16m4_t maskedoff, vbfloat16m4_t src,
			      __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m4_tu (maskedoff, src, value, vl);
}

vbfloat16m8_t
test_vfslide1up_vf_bf16m8_tu (vbfloat16m8_t maskedoff, vbfloat16m8_t src,
			      __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m8_tu (maskedoff, src, value, vl);
}

vfloat32mf2_t
test_vfslide1up_vf_f32mf2_tu (vfloat32mf2_t maskedoff, vfloat32mf2_t src,
			      float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32mf2_tu (maskedoff, src, value, vl);
}

vfloat32m1_t
test_vfslide1up_vf_f32m1_tu (vfloat32m1_t maskedoff, vfloat32m1_t src,
			     float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m1_tu (maskedoff, src, value, vl);
}

vfloat32m2_t
test_vfslide1up_vf_f32m2_tu (vfloat32m2_t maskedoff, vfloat32m2_t src,
			     float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m2_tu (maskedoff, src, value, vl);
}

vfloat32m4_t
test_vfslide1up_vf_f32m4_tu (vfloat32m4_t maskedoff, vfloat32m4_t src,
			     float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m4_tu (maskedoff, src, value, vl);
}

vfloat32m8_t
test_vfslide1up_vf_f32m8_tu (vfloat32m8_t maskedoff, vfloat32m8_t src,
			     float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m8_tu (maskedoff, src, value, vl);
}

vfloat64m1_t
test_vfslide1up_vf_f64m1_tu (vfloat64m1_t maskedoff, vfloat64m1_t src,
			     double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m1_tu (maskedoff, src, value, vl);
}

vfloat64m2_t
test_vfslide1up_vf_f64m2_tu (vfloat64m2_t maskedoff, vfloat64m2_t src,
			     double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m2_tu (maskedoff, src, value, vl);
}

vfloat64m4_t
test_vfslide1up_vf_f64m4_tu (vfloat64m4_t maskedoff, vfloat64m4_t src,
			     double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m4_tu (maskedoff, src, value, vl);
}

vfloat64m8_t
test_vfslide1up_vf_f64m8_tu (vfloat64m8_t maskedoff, vfloat64m8_t src,
			     double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m8_tu (maskedoff, src, value, vl);
}

vbfloat16mf4_t
test_vfslide1up_vf_bf16mf4_tum (vbool64_t mask, vbfloat16mf4_t maskedoff,
				vbfloat16mf4_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf4_tum (mask, maskedoff, src, value, vl);
}

vbfloat16mf2_t
test_vfslide1up_vf_bf16mf2_tum (vbool32_t mask, vbfloat16mf2_t maskedoff,
				vbfloat16mf2_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf2_tum (mask, maskedoff, src, value, vl);
}

vbfloat16m1_t
test_vfslide1up_vf_bf16m1_tum (vbool16_t mask, vbfloat16m1_t maskedoff,
			       vbfloat16m1_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m1_tum (mask, maskedoff, src, value, vl);
}

vbfloat16m2_t
test_vfslide1up_vf_bf16m2_tum (vbool8_t mask, vbfloat16m2_t maskedoff,
			       vbfloat16m2_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m2_tum (mask, maskedoff, src, value, vl);
}

vbfloat16m4_t
test_vfslide1up_vf_bf16m4_tum (vbool4_t mask, vbfloat16m4_t maskedoff,
			       vbfloat16m4_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m4_tum (mask, maskedoff, src, value, vl);
}

vbfloat16m8_t
test_vfslide1up_vf_bf16m8_tum (vbool2_t mask, vbfloat16m8_t maskedoff,
			       vbfloat16m8_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m8_tum (mask, maskedoff, src, value, vl);
}

vfloat32mf2_t
test_vfslide1up_vf_f32mf2_tum (vbool64_t mask, vfloat32mf2_t maskedoff,
			       vfloat32mf2_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32mf2_tum (mask, maskedoff, src, value, vl);
}

vfloat32m1_t
test_vfslide1up_vf_f32m1_tum (vbool32_t mask, vfloat32m1_t maskedoff,
			      vfloat32m1_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m1_tum (mask, maskedoff, src, value, vl);
}

vfloat32m2_t
test_vfslide1up_vf_f32m2_tum (vbool16_t mask, vfloat32m2_t maskedoff,
			      vfloat32m2_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m2_tum (mask, maskedoff, src, value, vl);
}

vfloat32m4_t
test_vfslide1up_vf_f32m4_tum (vbool8_t mask, vfloat32m4_t maskedoff,
			      vfloat32m4_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m4_tum (mask, maskedoff, src, value, vl);
}

vfloat32m8_t
test_vfslide1up_vf_f32m8_tum (vbool4_t mask, vfloat32m8_t maskedoff,
			      vfloat32m8_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m8_tum (mask, maskedoff, src, value, vl);
}

vfloat64m1_t
test_vfslide1up_vf_f64m1_tum (vbool64_t mask, vfloat64m1_t maskedoff,
			      vfloat64m1_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m1_tum (mask, maskedoff, src, value, vl);
}

vfloat64m2_t
test_vfslide1up_vf_f64m2_tum (vbool32_t mask, vfloat64m2_t maskedoff,
			      vfloat64m2_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m2_tum (mask, maskedoff, src, value, vl);
}

vfloat64m4_t
test_vfslide1up_vf_f64m4_tum (vbool16_t mask, vfloat64m4_t maskedoff,
			      vfloat64m4_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m4_tum (mask, maskedoff, src, value, vl);
}

vfloat64m8_t
test_vfslide1up_vf_f64m8_tum (vbool8_t mask, vfloat64m8_t maskedoff,
			      vfloat64m8_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m8_tum (mask, maskedoff, src, value, vl);
}

vbfloat16mf4_t
test_vfslide1up_vf_bf16mf4_tumu (vbool64_t mask, vbfloat16mf4_t maskedoff,
				 vbfloat16mf4_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf4_tumu (mask, maskedoff, src, value, vl);
}

vbfloat16mf2_t
test_vfslide1up_vf_bf16mf2_tumu (vbool32_t mask, vbfloat16mf2_t maskedoff,
				 vbfloat16mf2_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf2_tumu (mask, maskedoff, src, value, vl);
}

vbfloat16m1_t
test_vfslide1up_vf_bf16m1_tumu (vbool16_t mask, vbfloat16m1_t maskedoff,
				vbfloat16m1_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m1_tumu (mask, maskedoff, src, value, vl);
}

vbfloat16m2_t
test_vfslide1up_vf_bf16m2_tumu (vbool8_t mask, vbfloat16m2_t maskedoff,
				vbfloat16m2_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m2_tumu (mask, maskedoff, src, value, vl);
}

vbfloat16m4_t
test_vfslide1up_vf_bf16m4_tumu (vbool4_t mask, vbfloat16m4_t maskedoff,
				vbfloat16m4_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m4_tumu (mask, maskedoff, src, value, vl);
}

vbfloat16m8_t
test_vfslide1up_vf_bf16m8_tumu (vbool2_t mask, vbfloat16m8_t maskedoff,
				vbfloat16m8_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m8_tumu (mask, maskedoff, src, value, vl);
}

vfloat32mf2_t
test_vfslide1up_vf_f32mf2_tumu (vbool64_t mask, vfloat32mf2_t maskedoff,
				vfloat32mf2_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32mf2_tumu (mask, maskedoff, src, value, vl);
}

vfloat32m1_t
test_vfslide1up_vf_f32m1_tumu (vbool32_t mask, vfloat32m1_t maskedoff,
			       vfloat32m1_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m1_tumu (mask, maskedoff, src, value, vl);
}

vfloat32m2_t
test_vfslide1up_vf_f32m2_tumu (vbool16_t mask, vfloat32m2_t maskedoff,
			       vfloat32m2_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m2_tumu (mask, maskedoff, src, value, vl);
}

vfloat32m4_t
test_vfslide1up_vf_f32m4_tumu (vbool8_t mask, vfloat32m4_t maskedoff,
			       vfloat32m4_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m4_tumu (mask, maskedoff, src, value, vl);
}

vfloat32m8_t
test_vfslide1up_vf_f32m8_tumu (vbool4_t mask, vfloat32m8_t maskedoff,
			       vfloat32m8_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m8_tumu (mask, maskedoff, src, value, vl);
}

vfloat64m1_t
test_vfslide1up_vf_f64m1_tumu (vbool64_t mask, vfloat64m1_t maskedoff,
			       vfloat64m1_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m1_tumu (mask, maskedoff, src, value, vl);
}

vfloat64m2_t
test_vfslide1up_vf_f64m2_tumu (vbool32_t mask, vfloat64m2_t maskedoff,
			       vfloat64m2_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m2_tumu (mask, maskedoff, src, value, vl);
}

vfloat64m4_t
test_vfslide1up_vf_f64m4_tumu (vbool16_t mask, vfloat64m4_t maskedoff,
			       vfloat64m4_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m4_tumu (mask, maskedoff, src, value, vl);
}

vfloat64m8_t
test_vfslide1up_vf_f64m8_tumu (vbool8_t mask, vfloat64m8_t maskedoff,
			       vfloat64m8_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m8_tumu (mask, maskedoff, src, value, vl);
}

vbfloat16mf4_t
test_vfslide1up_vf_bf16mf4_mu (vbool64_t mask, vbfloat16mf4_t maskedoff,
			       vbfloat16mf4_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf4_mu (mask, maskedoff, src, value, vl);
}

vbfloat16mf2_t
test_vfslide1up_vf_bf16mf2_mu (vbool32_t mask, vbfloat16mf2_t maskedoff,
			       vbfloat16mf2_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16mf2_mu (mask, maskedoff, src, value, vl);
}

vbfloat16m1_t
test_vfslide1up_vf_bf16m1_mu (vbool16_t mask, vbfloat16m1_t maskedoff,
			      vbfloat16m1_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m1_mu (mask, maskedoff, src, value, vl);
}

vbfloat16m2_t
test_vfslide1up_vf_bf16m2_mu (vbool8_t mask, vbfloat16m2_t maskedoff,
			      vbfloat16m2_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m2_mu (mask, maskedoff, src, value, vl);
}

vbfloat16m4_t
test_vfslide1up_vf_bf16m4_mu (vbool4_t mask, vbfloat16m4_t maskedoff,
			      vbfloat16m4_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m4_mu (mask, maskedoff, src, value, vl);
}

vbfloat16m8_t
test_vfslide1up_vf_bf16m8_mu (vbool2_t mask, vbfloat16m8_t maskedoff,
			      vbfloat16m8_t src, __bf16 value, size_t vl)
{
  return __riscv_vfslide1up_vf_bf16m8_mu (mask, maskedoff, src, value, vl);
}

vfloat32mf2_t
test_vfslide1up_vf_f32mf2_mu (vbool64_t mask, vfloat32mf2_t maskedoff,
			      vfloat32mf2_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32mf2_mu (mask, maskedoff, src, value, vl);
}

vfloat32m1_t
test_vfslide1up_vf_f32m1_mu (vbool32_t mask, vfloat32m1_t maskedoff,
			     vfloat32m1_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m1_mu (mask, maskedoff, src, value, vl);
}

vfloat32m2_t
test_vfslide1up_vf_f32m2_mu (vbool16_t mask, vfloat32m2_t maskedoff,
			     vfloat32m2_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m2_mu (mask, maskedoff, src, value, vl);
}

vfloat32m4_t
test_vfslide1up_vf_f32m4_mu (vbool8_t mask, vfloat32m4_t maskedoff,
			     vfloat32m4_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m4_mu (mask, maskedoff, src, value, vl);
}

vfloat32m8_t
test_vfslide1up_vf_f32m8_mu (vbool4_t mask, vfloat32m8_t maskedoff,
			     vfloat32m8_t src, float value, size_t vl)
{
  return __riscv_vfslide1up_vf_f32m8_mu (mask, maskedoff, src, value, vl);
}

vfloat64m1_t
test_vfslide1up_vf_f64m1_mu (vbool64_t mask, vfloat64m1_t maskedoff,
			     vfloat64m1_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m1_mu (mask, maskedoff, src, value, vl);
}

vfloat64m2_t
test_vfslide1up_vf_f64m2_mu (vbool32_t mask, vfloat64m2_t maskedoff,
			     vfloat64m2_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m2_mu (mask, maskedoff, src, value, vl);
}

vfloat64m4_t
test_vfslide1up_vf_f64m4_mu (vbool16_t mask, vfloat64m4_t maskedoff,
			     vfloat64m4_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m4_mu (mask, maskedoff, src, value, vl);
}

vfloat64m8_t
test_vfslide1up_vf_f64m8_mu (vbool8_t mask, vfloat64m8_t maskedoff,
			     vfloat64m8_t src, double value, size_t vl)
{
  return __riscv_vfslide1up_vf_f64m8_mu (mask, maskedoff, src, value, vl);
}
