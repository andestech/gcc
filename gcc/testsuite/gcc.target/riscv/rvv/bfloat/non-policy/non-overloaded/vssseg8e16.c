/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

void
test_vssseg8e16_v_bf16mf4x8 (__bf16 *base, ptrdiff_t bstride,
			     vbfloat16mf4x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_bf16mf4x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_bf16mf2x8 (__bf16 *base, ptrdiff_t bstride,
			     vbfloat16mf2x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_bf16mf2x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_bf16m1x8 (__bf16 *base, ptrdiff_t bstride,
			    vbfloat16m1x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_bf16m1x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_i16mf4x8 (int16_t *base, ptrdiff_t bstride,
			    vint16mf4x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_i16mf4x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_i16mf2x8 (int16_t *base, ptrdiff_t bstride,
			    vint16mf2x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_i16mf2x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_i16m1x8 (int16_t *base, ptrdiff_t bstride,
			   vint16m1x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_i16m1x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_u16mf4x8 (uint16_t *base, ptrdiff_t bstride,
			    vuint16mf4x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_u16mf4x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_u16mf2x8 (uint16_t *base, ptrdiff_t bstride,
			    vuint16mf2x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_u16mf2x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_u16m1x8 (uint16_t *base, ptrdiff_t bstride,
			   vuint16m1x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_u16m1x8 (base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_bf16mf4x8_m (vbool64_t mask, __bf16 *base, ptrdiff_t bstride,
			       vbfloat16mf4x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_bf16mf4x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_bf16mf2x8_m (vbool32_t mask, __bf16 *base, ptrdiff_t bstride,
			       vbfloat16mf2x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_bf16mf2x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_bf16m1x8_m (vbool16_t mask, __bf16 *base, ptrdiff_t bstride,
			      vbfloat16m1x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_bf16m1x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_i16mf4x8_m (vbool64_t mask, int16_t *base, ptrdiff_t bstride,
			      vint16mf4x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_i16mf4x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_i16mf2x8_m (vbool32_t mask, int16_t *base, ptrdiff_t bstride,
			      vint16mf2x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_i16mf2x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_i16m1x8_m (vbool16_t mask, int16_t *base, ptrdiff_t bstride,
			     vint16m1x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_i16m1x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_u16mf4x8_m (vbool64_t mask, uint16_t *base, ptrdiff_t bstride,
			      vuint16mf4x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_u16mf4x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_u16mf2x8_m (vbool32_t mask, uint16_t *base, ptrdiff_t bstride,
			      vuint16mf2x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_u16mf2x8_m (mask, base, bstride, v_tuple, vl);
}

void
test_vssseg8e16_v_u16m1x8_m (vbool16_t mask, uint16_t *base, ptrdiff_t bstride,
			     vuint16m1x8_t v_tuple, size_t vl)
{
  return __riscv_vssseg8e16_v_u16m1x8_m (mask, base, bstride, v_tuple, vl);
}
