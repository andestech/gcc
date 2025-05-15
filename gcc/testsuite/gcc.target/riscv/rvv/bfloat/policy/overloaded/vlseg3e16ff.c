/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

vbfloat16mf4x3_t
test_vlseg3e16ff_v_bf16mf4x3_tu (vbfloat16mf4x3_t maskedoff_tuple,
				 const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vbfloat16mf2x3_t
test_vlseg3e16ff_v_bf16mf2x3_tu (vbfloat16mf2x3_t maskedoff_tuple,
				 const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m1x3_t
test_vlseg3e16ff_v_bf16m1x3_tu (vbfloat16m1x3_t maskedoff_tuple,
				const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m2x3_t
test_vlseg3e16ff_v_bf16m2x3_tu (vbfloat16m2x3_t maskedoff_tuple,
				const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vint16mf4x3_t
test_vlseg3e16ff_v_i16mf4x3_tu (vint16mf4x3_t maskedoff_tuple,
				const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vint16mf2x3_t
test_vlseg3e16ff_v_i16mf2x3_tu (vint16mf2x3_t maskedoff_tuple,
				const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vint16m1x3_t
test_vlseg3e16ff_v_i16m1x3_tu (vint16m1x3_t maskedoff_tuple,
			       const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vint16m2x3_t
test_vlseg3e16ff_v_i16m2x3_tu (vint16m2x3_t maskedoff_tuple,
			       const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vuint16mf4x3_t
test_vlseg3e16ff_v_u16mf4x3_tu (vuint16mf4x3_t maskedoff_tuple,
				const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vuint16mf2x3_t
test_vlseg3e16ff_v_u16mf2x3_tu (vuint16mf2x3_t maskedoff_tuple,
				const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vuint16m1x3_t
test_vlseg3e16ff_v_u16m1x3_tu (vuint16m1x3_t maskedoff_tuple,
			       const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vuint16m2x3_t
test_vlseg3e16ff_v_u16m2x3_tu (vuint16m2x3_t maskedoff_tuple,
			       const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tu (maskedoff_tuple, base, new_vl, vl);
}

vbfloat16mf4x3_t
test_vlseg3e16ff_v_bf16mf4x3_tum (vbool64_t mask,
				  vbfloat16mf4x3_t maskedoff_tuple,
				  const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16mf2x3_t
test_vlseg3e16ff_v_bf16mf2x3_tum (vbool32_t mask,
				  vbfloat16mf2x3_t maskedoff_tuple,
				  const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m1x3_t
test_vlseg3e16ff_v_bf16m1x3_tum (vbool16_t mask,
				 vbfloat16m1x3_t maskedoff_tuple,
				 const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m2x3_t
test_vlseg3e16ff_v_bf16m2x3_tum (vbool8_t mask, vbfloat16m2x3_t maskedoff_tuple,
				 const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16mf4x3_t
test_vlseg3e16ff_v_i16mf4x3_tum (vbool64_t mask, vint16mf4x3_t maskedoff_tuple,
				 const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16mf2x3_t
test_vlseg3e16ff_v_i16mf2x3_tum (vbool32_t mask, vint16mf2x3_t maskedoff_tuple,
				 const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16m1x3_t
test_vlseg3e16ff_v_i16m1x3_tum (vbool16_t mask, vint16m1x3_t maskedoff_tuple,
				const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16m2x3_t
test_vlseg3e16ff_v_i16m2x3_tum (vbool8_t mask, vint16m2x3_t maskedoff_tuple,
				const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16mf4x3_t
test_vlseg3e16ff_v_u16mf4x3_tum (vbool64_t mask, vuint16mf4x3_t maskedoff_tuple,
				 const uint16_t *base, size_t *new_vl,
				 size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16mf2x3_t
test_vlseg3e16ff_v_u16mf2x3_tum (vbool32_t mask, vuint16mf2x3_t maskedoff_tuple,
				 const uint16_t *base, size_t *new_vl,
				 size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16m1x3_t
test_vlseg3e16ff_v_u16m1x3_tum (vbool16_t mask, vuint16m1x3_t maskedoff_tuple,
				const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16m2x3_t
test_vlseg3e16ff_v_u16m2x3_tum (vbool8_t mask, vuint16m2x3_t maskedoff_tuple,
				const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tum (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16mf4x3_t
test_vlseg3e16ff_v_bf16mf4x3_tumu (vbool64_t mask,
				   vbfloat16mf4x3_t maskedoff_tuple,
				   const __bf16 *base, size_t *new_vl,
				   size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16mf2x3_t
test_vlseg3e16ff_v_bf16mf2x3_tumu (vbool32_t mask,
				   vbfloat16mf2x3_t maskedoff_tuple,
				   const __bf16 *base, size_t *new_vl,
				   size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m1x3_t
test_vlseg3e16ff_v_bf16m1x3_tumu (vbool16_t mask,
				  vbfloat16m1x3_t maskedoff_tuple,
				  const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m2x3_t
test_vlseg3e16ff_v_bf16m2x3_tumu (vbool8_t mask,
				  vbfloat16m2x3_t maskedoff_tuple,
				  const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16mf4x3_t
test_vlseg3e16ff_v_i16mf4x3_tumu (vbool64_t mask, vint16mf4x3_t maskedoff_tuple,
				  const int16_t *base, size_t *new_vl,
				  size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16mf2x3_t
test_vlseg3e16ff_v_i16mf2x3_tumu (vbool32_t mask, vint16mf2x3_t maskedoff_tuple,
				  const int16_t *base, size_t *new_vl,
				  size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16m1x3_t
test_vlseg3e16ff_v_i16m1x3_tumu (vbool16_t mask, vint16m1x3_t maskedoff_tuple,
				 const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16m2x3_t
test_vlseg3e16ff_v_i16m2x3_tumu (vbool8_t mask, vint16m2x3_t maskedoff_tuple,
				 const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16mf4x3_t
test_vlseg3e16ff_v_u16mf4x3_tumu (vbool64_t mask,
				  vuint16mf4x3_t maskedoff_tuple,
				  const uint16_t *base, size_t *new_vl,
				  size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16mf2x3_t
test_vlseg3e16ff_v_u16mf2x3_tumu (vbool32_t mask,
				  vuint16mf2x3_t maskedoff_tuple,
				  const uint16_t *base, size_t *new_vl,
				  size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16m1x3_t
test_vlseg3e16ff_v_u16m1x3_tumu (vbool16_t mask, vuint16m1x3_t maskedoff_tuple,
				 const uint16_t *base, size_t *new_vl,
				 size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16m2x3_t
test_vlseg3e16ff_v_u16m2x3_tumu (vbool8_t mask, vuint16m2x3_t maskedoff_tuple,
				 const uint16_t *base, size_t *new_vl,
				 size_t vl)
{
  return __riscv_vlseg3e16ff_tumu (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16mf4x3_t
test_vlseg3e16ff_v_bf16mf4x3_mu (vbool64_t mask,
				 vbfloat16mf4x3_t maskedoff_tuple,
				 const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16mf2x3_t
test_vlseg3e16ff_v_bf16mf2x3_mu (vbool32_t mask,
				 vbfloat16mf2x3_t maskedoff_tuple,
				 const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m1x3_t
test_vlseg3e16ff_v_bf16m1x3_mu (vbool16_t mask, vbfloat16m1x3_t maskedoff_tuple,
				const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vbfloat16m2x3_t
test_vlseg3e16ff_v_bf16m2x3_mu (vbool8_t mask, vbfloat16m2x3_t maskedoff_tuple,
				const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16mf4x3_t
test_vlseg3e16ff_v_i16mf4x3_mu (vbool64_t mask, vint16mf4x3_t maskedoff_tuple,
				const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16mf2x3_t
test_vlseg3e16ff_v_i16mf2x3_mu (vbool32_t mask, vint16mf2x3_t maskedoff_tuple,
				const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16m1x3_t
test_vlseg3e16ff_v_i16m1x3_mu (vbool16_t mask, vint16m1x3_t maskedoff_tuple,
			       const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vint16m2x3_t
test_vlseg3e16ff_v_i16m2x3_mu (vbool8_t mask, vint16m2x3_t maskedoff_tuple,
			       const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16mf4x3_t
test_vlseg3e16ff_v_u16mf4x3_mu (vbool64_t mask, vuint16mf4x3_t maskedoff_tuple,
				const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16mf2x3_t
test_vlseg3e16ff_v_u16mf2x3_mu (vbool32_t mask, vuint16mf2x3_t maskedoff_tuple,
				const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16m1x3_t
test_vlseg3e16ff_v_u16m1x3_mu (vbool16_t mask, vuint16m1x3_t maskedoff_tuple,
			       const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}

vuint16m2x3_t
test_vlseg3e16ff_v_u16m2x3_mu (vbool8_t mask, vuint16m2x3_t maskedoff_tuple,
			       const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_mu (mask, maskedoff_tuple, base, new_vl, vl);
}
