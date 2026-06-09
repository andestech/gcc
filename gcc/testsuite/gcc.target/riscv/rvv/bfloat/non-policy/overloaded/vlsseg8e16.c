/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

vbfloat16mf4x8_t
test_vlsseg8e16_v_bf16mf4x8_m (vbool64_t mask, const __bf16 *base,
			       ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vbfloat16mf2x8_t
test_vlsseg8e16_v_bf16mf2x8_m (vbool32_t mask, const __bf16 *base,
			       ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vbfloat16m1x8_t
test_vlsseg8e16_v_bf16m1x8_m (vbool16_t mask, const __bf16 *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vint16mf4x8_t
test_vlsseg8e16_v_i16mf4x8_m (vbool64_t mask, const int16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vint16mf2x8_t
test_vlsseg8e16_v_i16mf2x8_m (vbool32_t mask, const int16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vint16m1x8_t
test_vlsseg8e16_v_i16m1x8_m (vbool16_t mask, const int16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vuint16mf4x8_t
test_vlsseg8e16_v_u16mf4x8_m (vbool64_t mask, const uint16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vuint16mf2x8_t
test_vlsseg8e16_v_u16mf2x8_m (vbool32_t mask, const uint16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}

vuint16m1x8_t
test_vlsseg8e16_v_u16m1x8_m (vbool16_t mask, const uint16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg8e16 (mask, base, bstride, vl);
}
