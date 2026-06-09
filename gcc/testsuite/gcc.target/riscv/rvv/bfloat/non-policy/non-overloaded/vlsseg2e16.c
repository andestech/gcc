/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

vbfloat16mf4x2_t
test_vlsseg2e16_v_bf16mf4x2 (const __bf16 *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16mf4x2 (base, bstride, vl);
}

vbfloat16mf2x2_t
test_vlsseg2e16_v_bf16mf2x2 (const __bf16 *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16mf2x2 (base, bstride, vl);
}

vbfloat16m1x2_t
test_vlsseg2e16_v_bf16m1x2 (const __bf16 *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16m1x2 (base, bstride, vl);
}

vbfloat16m2x2_t
test_vlsseg2e16_v_bf16m2x2 (const __bf16 *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16m2x2 (base, bstride, vl);
}

vbfloat16m4x2_t
test_vlsseg2e16_v_bf16m4x2 (const __bf16 *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16m4x2 (base, bstride, vl);
}

vint16mf4x2_t
test_vlsseg2e16_v_i16mf4x2 (const int16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16mf4x2 (base, bstride, vl);
}

vint16mf2x2_t
test_vlsseg2e16_v_i16mf2x2 (const int16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16mf2x2 (base, bstride, vl);
}

vint16m1x2_t
test_vlsseg2e16_v_i16m1x2 (const int16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16m1x2 (base, bstride, vl);
}

vint16m2x2_t
test_vlsseg2e16_v_i16m2x2 (const int16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16m2x2 (base, bstride, vl);
}

vint16m4x2_t
test_vlsseg2e16_v_i16m4x2 (const int16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16m4x2 (base, bstride, vl);
}

vuint16mf4x2_t
test_vlsseg2e16_v_u16mf4x2 (const uint16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16mf4x2 (base, bstride, vl);
}

vuint16mf2x2_t
test_vlsseg2e16_v_u16mf2x2 (const uint16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16mf2x2 (base, bstride, vl);
}

vuint16m1x2_t
test_vlsseg2e16_v_u16m1x2 (const uint16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16m1x2 (base, bstride, vl);
}

vuint16m2x2_t
test_vlsseg2e16_v_u16m2x2 (const uint16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16m2x2 (base, bstride, vl);
}

vuint16m4x2_t
test_vlsseg2e16_v_u16m4x2 (const uint16_t *base, ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16m4x2 (base, bstride, vl);
}

vbfloat16mf4x2_t
test_vlsseg2e16_v_bf16mf4x2_m (vbool64_t mask, const __bf16 *base,
			       ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16mf4x2_m (mask, base, bstride, vl);
}

vbfloat16mf2x2_t
test_vlsseg2e16_v_bf16mf2x2_m (vbool32_t mask, const __bf16 *base,
			       ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16mf2x2_m (mask, base, bstride, vl);
}

vbfloat16m1x2_t
test_vlsseg2e16_v_bf16m1x2_m (vbool16_t mask, const __bf16 *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16m1x2_m (mask, base, bstride, vl);
}

vbfloat16m2x2_t
test_vlsseg2e16_v_bf16m2x2_m (vbool8_t mask, const __bf16 *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16m2x2_m (mask, base, bstride, vl);
}

vbfloat16m4x2_t
test_vlsseg2e16_v_bf16m4x2_m (vbool4_t mask, const __bf16 *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_bf16m4x2_m (mask, base, bstride, vl);
}

vint16mf4x2_t
test_vlsseg2e16_v_i16mf4x2_m (vbool64_t mask, const int16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16mf4x2_m (mask, base, bstride, vl);
}

vint16mf2x2_t
test_vlsseg2e16_v_i16mf2x2_m (vbool32_t mask, const int16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16mf2x2_m (mask, base, bstride, vl);
}

vint16m1x2_t
test_vlsseg2e16_v_i16m1x2_m (vbool16_t mask, const int16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16m1x2_m (mask, base, bstride, vl);
}

vint16m2x2_t
test_vlsseg2e16_v_i16m2x2_m (vbool8_t mask, const int16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16m2x2_m (mask, base, bstride, vl);
}

vint16m4x2_t
test_vlsseg2e16_v_i16m4x2_m (vbool4_t mask, const int16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_i16m4x2_m (mask, base, bstride, vl);
}

vuint16mf4x2_t
test_vlsseg2e16_v_u16mf4x2_m (vbool64_t mask, const uint16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16mf4x2_m (mask, base, bstride, vl);
}

vuint16mf2x2_t
test_vlsseg2e16_v_u16mf2x2_m (vbool32_t mask, const uint16_t *base,
			      ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16mf2x2_m (mask, base, bstride, vl);
}

vuint16m1x2_t
test_vlsseg2e16_v_u16m1x2_m (vbool16_t mask, const uint16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16m1x2_m (mask, base, bstride, vl);
}

vuint16m2x2_t
test_vlsseg2e16_v_u16m2x2_m (vbool8_t mask, const uint16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16m2x2_m (mask, base, bstride, vl);
}

vuint16m4x2_t
test_vlsseg2e16_v_u16m4x2_m (vbool4_t mask, const uint16_t *base,
			     ptrdiff_t bstride, size_t vl)
{
  return __riscv_vlsseg2e16_v_u16m4x2_m (mask, base, bstride, vl);
}
