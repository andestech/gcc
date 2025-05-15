/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

vbfloat16mf4x8_t
test_vlsseg8e16_v_bf16mf4x8_tu (vbfloat16mf4x8_t maskedoff_tuple,
				const __bf16 *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vbfloat16mf2x8_t
test_vlsseg8e16_v_bf16mf2x8_tu (vbfloat16mf2x8_t maskedoff_tuple,
				const __bf16 *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vbfloat16m1x8_t
test_vlsseg8e16_v_bf16m1x8_tu (vbfloat16m1x8_t maskedoff_tuple,
			       const __bf16 *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vint16mf4x8_t
test_vlsseg8e16_v_i16mf4x8_tu (vint16mf4x8_t maskedoff_tuple,
			       const int16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vint16mf2x8_t
test_vlsseg8e16_v_i16mf2x8_tu (vint16mf2x8_t maskedoff_tuple,
			       const int16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vint16m1x8_t
test_vlsseg8e16_v_i16m1x8_tu (vint16m1x8_t maskedoff_tuple, const int16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vuint16mf4x8_t
test_vlsseg8e16_v_u16mf4x8_tu (vuint16mf4x8_t maskedoff_tuple,
			       const uint16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vuint16mf2x8_t
test_vlsseg8e16_v_u16mf2x8_tu (vuint16mf2x8_t maskedoff_tuple,
			       const uint16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vuint16m1x8_t
test_vlsseg8e16_v_u16m1x8_tu (vuint16m1x8_t maskedoff_tuple,
			      const uint16_t *base, ptrdiff_t bstride,
			      size_t vl)
{
  return __riscv_vlsseg8e16_tu (maskedoff_tuple, base, bstride, vl);
}

vbfloat16mf4x8_t
test_vlsseg8e16_v_bf16mf4x8_tum (vbool64_t mask,
				 vbfloat16mf4x8_t maskedoff_tuple,
				 const __bf16 *base, ptrdiff_t bstride,
				 size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16mf2x8_t
test_vlsseg8e16_v_bf16mf2x8_tum (vbool32_t mask,
				 vbfloat16mf2x8_t maskedoff_tuple,
				 const __bf16 *base, ptrdiff_t bstride,
				 size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16m1x8_t
test_vlsseg8e16_v_bf16m1x8_tum (vbool16_t mask, vbfloat16m1x8_t maskedoff_tuple,
				const __bf16 *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vint16mf4x8_t
test_vlsseg8e16_v_i16mf4x8_tum (vbool64_t mask, vint16mf4x8_t maskedoff_tuple,
				const int16_t *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vint16mf2x8_t
test_vlsseg8e16_v_i16mf2x8_tum (vbool32_t mask, vint16mf2x8_t maskedoff_tuple,
				const int16_t *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vint16m1x8_t
test_vlsseg8e16_v_i16m1x8_tum (vbool16_t mask, vint16m1x8_t maskedoff_tuple,
			       const int16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16mf4x8_t
test_vlsseg8e16_v_u16mf4x8_tum (vbool64_t mask, vuint16mf4x8_t maskedoff_tuple,
				const uint16_t *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16mf2x8_t
test_vlsseg8e16_v_u16mf2x8_tum (vbool32_t mask, vuint16mf2x8_t maskedoff_tuple,
				const uint16_t *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16m1x8_t
test_vlsseg8e16_v_u16m1x8_tum (vbool16_t mask, vuint16m1x8_t maskedoff_tuple,
			       const uint16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_tum (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16mf4x8_t
test_vlsseg8e16_v_bf16mf4x8_tumu (vbool64_t mask,
				  vbfloat16mf4x8_t maskedoff_tuple,
				  const __bf16 *base, ptrdiff_t bstride,
				  size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16mf2x8_t
test_vlsseg8e16_v_bf16mf2x8_tumu (vbool32_t mask,
				  vbfloat16mf2x8_t maskedoff_tuple,
				  const __bf16 *base, ptrdiff_t bstride,
				  size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16m1x8_t
test_vlsseg8e16_v_bf16m1x8_tumu (vbool16_t mask,
				 vbfloat16m1x8_t maskedoff_tuple,
				 const __bf16 *base, ptrdiff_t bstride,
				 size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vint16mf4x8_t
test_vlsseg8e16_v_i16mf4x8_tumu (vbool64_t mask, vint16mf4x8_t maskedoff_tuple,
				 const int16_t *base, ptrdiff_t bstride,
				 size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vint16mf2x8_t
test_vlsseg8e16_v_i16mf2x8_tumu (vbool32_t mask, vint16mf2x8_t maskedoff_tuple,
				 const int16_t *base, ptrdiff_t bstride,
				 size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vint16m1x8_t
test_vlsseg8e16_v_i16m1x8_tumu (vbool16_t mask, vint16m1x8_t maskedoff_tuple,
				const int16_t *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16mf4x8_t
test_vlsseg8e16_v_u16mf4x8_tumu (vbool64_t mask, vuint16mf4x8_t maskedoff_tuple,
				 const uint16_t *base, ptrdiff_t bstride,
				 size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16mf2x8_t
test_vlsseg8e16_v_u16mf2x8_tumu (vbool32_t mask, vuint16mf2x8_t maskedoff_tuple,
				 const uint16_t *base, ptrdiff_t bstride,
				 size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16m1x8_t
test_vlsseg8e16_v_u16m1x8_tumu (vbool16_t mask, vuint16m1x8_t maskedoff_tuple,
				const uint16_t *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_tumu (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16mf4x8_t
test_vlsseg8e16_v_bf16mf4x8_mu (vbool64_t mask,
				vbfloat16mf4x8_t maskedoff_tuple,
				const __bf16 *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16mf2x8_t
test_vlsseg8e16_v_bf16mf2x8_mu (vbool32_t mask,
				vbfloat16mf2x8_t maskedoff_tuple,
				const __bf16 *base, ptrdiff_t bstride,
				size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vbfloat16m1x8_t
test_vlsseg8e16_v_bf16m1x8_mu (vbool16_t mask, vbfloat16m1x8_t maskedoff_tuple,
			       const __bf16 *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vint16mf4x8_t
test_vlsseg8e16_v_i16mf4x8_mu (vbool64_t mask, vint16mf4x8_t maskedoff_tuple,
			       const int16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vint16mf2x8_t
test_vlsseg8e16_v_i16mf2x8_mu (vbool32_t mask, vint16mf2x8_t maskedoff_tuple,
			       const int16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vint16m1x8_t
test_vlsseg8e16_v_i16m1x8_mu (vbool16_t mask, vint16m1x8_t maskedoff_tuple,
			      const int16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16mf4x8_t
test_vlsseg8e16_v_u16mf4x8_mu (vbool64_t mask, vuint16mf4x8_t maskedoff_tuple,
			       const uint16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16mf2x8_t
test_vlsseg8e16_v_u16mf2x8_mu (vbool32_t mask, vuint16mf2x8_t maskedoff_tuple,
			       const uint16_t *base, ptrdiff_t bstride,
			       size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}

vuint16m1x8_t
test_vlsseg8e16_v_u16m1x8_mu (vbool16_t mask, vuint16m1x8_t maskedoff_tuple,
			      const uint16_t *base, ptrdiff_t bstride,
			      size_t vl)
{
  return __riscv_vlsseg8e16_mu (mask, maskedoff_tuple, base, bstride, vl);
}
