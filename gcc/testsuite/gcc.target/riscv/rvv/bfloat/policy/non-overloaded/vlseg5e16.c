/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

vbfloat16mf4x5_t
test_vlseg5e16_v_bf16mf4x5_tu (vbfloat16mf4x5_t maskedoff_tuple,
			       const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf4x5_tu (maskedoff_tuple, base, vl);
}

vbfloat16mf2x5_t
test_vlseg5e16_v_bf16mf2x5_tu (vbfloat16mf2x5_t maskedoff_tuple,
			       const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf2x5_tu (maskedoff_tuple, base, vl);
}

vbfloat16m1x5_t
test_vlseg5e16_v_bf16m1x5_tu (vbfloat16m1x5_t maskedoff_tuple,
			      const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16m1x5_tu (maskedoff_tuple, base, vl);
}

vint16mf4x5_t
test_vlseg5e16_v_i16mf4x5_tu (vint16mf4x5_t maskedoff_tuple,
			      const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf4x5_tu (maskedoff_tuple, base, vl);
}

vint16mf2x5_t
test_vlseg5e16_v_i16mf2x5_tu (vint16mf2x5_t maskedoff_tuple,
			      const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf2x5_tu (maskedoff_tuple, base, vl);
}

vint16m1x5_t
test_vlseg5e16_v_i16m1x5_tu (vint16m1x5_t maskedoff_tuple, const int16_t *base,
			     size_t vl)
{
  return __riscv_vlseg5e16_v_i16m1x5_tu (maskedoff_tuple, base, vl);
}

vuint16mf4x5_t
test_vlseg5e16_v_u16mf4x5_tu (vuint16mf4x5_t maskedoff_tuple,
			      const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf4x5_tu (maskedoff_tuple, base, vl);
}

vuint16mf2x5_t
test_vlseg5e16_v_u16mf2x5_tu (vuint16mf2x5_t maskedoff_tuple,
			      const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf2x5_tu (maskedoff_tuple, base, vl);
}

vuint16m1x5_t
test_vlseg5e16_v_u16m1x5_tu (vuint16m1x5_t maskedoff_tuple,
			     const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16m1x5_tu (maskedoff_tuple, base, vl);
}

vbfloat16mf4x5_t
test_vlseg5e16_v_bf16mf4x5_tum (vbool64_t mask,
				vbfloat16mf4x5_t maskedoff_tuple,
				const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf4x5_tum (mask, maskedoff_tuple, base, vl);
}

vbfloat16mf2x5_t
test_vlseg5e16_v_bf16mf2x5_tum (vbool32_t mask,
				vbfloat16mf2x5_t maskedoff_tuple,
				const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf2x5_tum (mask, maskedoff_tuple, base, vl);
}

vbfloat16m1x5_t
test_vlseg5e16_v_bf16m1x5_tum (vbool16_t mask, vbfloat16m1x5_t maskedoff_tuple,
			       const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16m1x5_tum (mask, maskedoff_tuple, base, vl);
}

vint16mf4x5_t
test_vlseg5e16_v_i16mf4x5_tum (vbool64_t mask, vint16mf4x5_t maskedoff_tuple,
			       const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf4x5_tum (mask, maskedoff_tuple, base, vl);
}

vint16mf2x5_t
test_vlseg5e16_v_i16mf2x5_tum (vbool32_t mask, vint16mf2x5_t maskedoff_tuple,
			       const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf2x5_tum (mask, maskedoff_tuple, base, vl);
}

vint16m1x5_t
test_vlseg5e16_v_i16m1x5_tum (vbool16_t mask, vint16m1x5_t maskedoff_tuple,
			      const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16m1x5_tum (mask, maskedoff_tuple, base, vl);
}

vuint16mf4x5_t
test_vlseg5e16_v_u16mf4x5_tum (vbool64_t mask, vuint16mf4x5_t maskedoff_tuple,
			       const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf4x5_tum (mask, maskedoff_tuple, base, vl);
}

vuint16mf2x5_t
test_vlseg5e16_v_u16mf2x5_tum (vbool32_t mask, vuint16mf2x5_t maskedoff_tuple,
			       const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf2x5_tum (mask, maskedoff_tuple, base, vl);
}

vuint16m1x5_t
test_vlseg5e16_v_u16m1x5_tum (vbool16_t mask, vuint16m1x5_t maskedoff_tuple,
			      const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16m1x5_tum (mask, maskedoff_tuple, base, vl);
}

vbfloat16mf4x5_t
test_vlseg5e16_v_bf16mf4x5_tumu (vbool64_t mask,
				 vbfloat16mf4x5_t maskedoff_tuple,
				 const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf4x5_tumu (mask, maskedoff_tuple, base, vl);
}

vbfloat16mf2x5_t
test_vlseg5e16_v_bf16mf2x5_tumu (vbool32_t mask,
				 vbfloat16mf2x5_t maskedoff_tuple,
				 const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf2x5_tumu (mask, maskedoff_tuple, base, vl);
}

vbfloat16m1x5_t
test_vlseg5e16_v_bf16m1x5_tumu (vbool16_t mask, vbfloat16m1x5_t maskedoff_tuple,
				const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16m1x5_tumu (mask, maskedoff_tuple, base, vl);
}

vint16mf4x5_t
test_vlseg5e16_v_i16mf4x5_tumu (vbool64_t mask, vint16mf4x5_t maskedoff_tuple,
				const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf4x5_tumu (mask, maskedoff_tuple, base, vl);
}

vint16mf2x5_t
test_vlseg5e16_v_i16mf2x5_tumu (vbool32_t mask, vint16mf2x5_t maskedoff_tuple,
				const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf2x5_tumu (mask, maskedoff_tuple, base, vl);
}

vint16m1x5_t
test_vlseg5e16_v_i16m1x5_tumu (vbool16_t mask, vint16m1x5_t maskedoff_tuple,
			       const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16m1x5_tumu (mask, maskedoff_tuple, base, vl);
}

vuint16mf4x5_t
test_vlseg5e16_v_u16mf4x5_tumu (vbool64_t mask, vuint16mf4x5_t maskedoff_tuple,
				const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf4x5_tumu (mask, maskedoff_tuple, base, vl);
}

vuint16mf2x5_t
test_vlseg5e16_v_u16mf2x5_tumu (vbool32_t mask, vuint16mf2x5_t maskedoff_tuple,
				const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf2x5_tumu (mask, maskedoff_tuple, base, vl);
}

vuint16m1x5_t
test_vlseg5e16_v_u16m1x5_tumu (vbool16_t mask, vuint16m1x5_t maskedoff_tuple,
			       const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16m1x5_tumu (mask, maskedoff_tuple, base, vl);
}

vbfloat16mf4x5_t
test_vlseg5e16_v_bf16mf4x5_mu (vbool64_t mask, vbfloat16mf4x5_t maskedoff_tuple,
			       const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf4x5_mu (mask, maskedoff_tuple, base, vl);
}

vbfloat16mf2x5_t
test_vlseg5e16_v_bf16mf2x5_mu (vbool32_t mask, vbfloat16mf2x5_t maskedoff_tuple,
			       const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16mf2x5_mu (mask, maskedoff_tuple, base, vl);
}

vbfloat16m1x5_t
test_vlseg5e16_v_bf16m1x5_mu (vbool16_t mask, vbfloat16m1x5_t maskedoff_tuple,
			      const __bf16 *base, size_t vl)
{
  return __riscv_vlseg5e16_v_bf16m1x5_mu (mask, maskedoff_tuple, base, vl);
}

vint16mf4x5_t
test_vlseg5e16_v_i16mf4x5_mu (vbool64_t mask, vint16mf4x5_t maskedoff_tuple,
			      const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf4x5_mu (mask, maskedoff_tuple, base, vl);
}

vint16mf2x5_t
test_vlseg5e16_v_i16mf2x5_mu (vbool32_t mask, vint16mf2x5_t maskedoff_tuple,
			      const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16mf2x5_mu (mask, maskedoff_tuple, base, vl);
}

vint16m1x5_t
test_vlseg5e16_v_i16m1x5_mu (vbool16_t mask, vint16m1x5_t maskedoff_tuple,
			     const int16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_i16m1x5_mu (mask, maskedoff_tuple, base, vl);
}

vuint16mf4x5_t
test_vlseg5e16_v_u16mf4x5_mu (vbool64_t mask, vuint16mf4x5_t maskedoff_tuple,
			      const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf4x5_mu (mask, maskedoff_tuple, base, vl);
}

vuint16mf2x5_t
test_vlseg5e16_v_u16mf2x5_mu (vbool32_t mask, vuint16mf2x5_t maskedoff_tuple,
			      const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16mf2x5_mu (mask, maskedoff_tuple, base, vl);
}

vuint16m1x5_t
test_vlseg5e16_v_u16m1x5_mu (vbool16_t mask, vuint16m1x5_t maskedoff_tuple,
			     const uint16_t *base, size_t vl)
{
  return __riscv_vlseg5e16_v_u16m1x5_mu (mask, maskedoff_tuple, base, vl);
}
