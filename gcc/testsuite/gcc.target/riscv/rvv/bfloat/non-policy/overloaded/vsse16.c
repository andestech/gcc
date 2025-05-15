/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

void
test_vsse16_v_bf16mf4 (__bf16 *base, ptrdiff_t bstride, vbfloat16mf4_t value,
		       size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_bf16mf2 (__bf16 *base, ptrdiff_t bstride, vbfloat16mf2_t value,
		       size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_bf16m1 (__bf16 *base, ptrdiff_t bstride, vbfloat16m1_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_bf16m2 (__bf16 *base, ptrdiff_t bstride, vbfloat16m2_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_bf16m4 (__bf16 *base, ptrdiff_t bstride, vbfloat16m4_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_bf16m8 (__bf16 *base, ptrdiff_t bstride, vbfloat16m8_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_i16mf4 (int16_t *base, ptrdiff_t bstride, vint16mf4_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_i16mf2 (int16_t *base, ptrdiff_t bstride, vint16mf2_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_i16m1 (int16_t *base, ptrdiff_t bstride, vint16m1_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_i16m2 (int16_t *base, ptrdiff_t bstride, vint16m2_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_i16m4 (int16_t *base, ptrdiff_t bstride, vint16m4_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_i16m8 (int16_t *base, ptrdiff_t bstride, vint16m8_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_u16mf4 (uint16_t *base, ptrdiff_t bstride, vuint16mf4_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_u16mf2 (uint16_t *base, ptrdiff_t bstride, vuint16mf2_t value,
		      size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_u16m1 (uint16_t *base, ptrdiff_t bstride, vuint16m1_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_u16m2 (uint16_t *base, ptrdiff_t bstride, vuint16m2_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_u16m4 (uint16_t *base, ptrdiff_t bstride, vuint16m4_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_u16m8 (uint16_t *base, ptrdiff_t bstride, vuint16m8_t value,
		     size_t vl)
{
  return __riscv_vsse16 (base, bstride, value, vl);
}

void
test_vsse16_v_bf16mf4_m (vbool64_t mask, __bf16 *base, ptrdiff_t bstride,
			 vbfloat16mf4_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_bf16mf2_m (vbool32_t mask, __bf16 *base, ptrdiff_t bstride,
			 vbfloat16mf2_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_bf16m1_m (vbool16_t mask, __bf16 *base, ptrdiff_t bstride,
			vbfloat16m1_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_bf16m2_m (vbool8_t mask, __bf16 *base, ptrdiff_t bstride,
			vbfloat16m2_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_bf16m4_m (vbool4_t mask, __bf16 *base, ptrdiff_t bstride,
			vbfloat16m4_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_bf16m8_m (vbool2_t mask, __bf16 *base, ptrdiff_t bstride,
			vbfloat16m8_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_i16mf4_m (vbool64_t mask, int16_t *base, ptrdiff_t bstride,
			vint16mf4_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_i16mf2_m (vbool32_t mask, int16_t *base, ptrdiff_t bstride,
			vint16mf2_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_i16m1_m (vbool16_t mask, int16_t *base, ptrdiff_t bstride,
		       vint16m1_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_i16m2_m (vbool8_t mask, int16_t *base, ptrdiff_t bstride,
		       vint16m2_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_i16m4_m (vbool4_t mask, int16_t *base, ptrdiff_t bstride,
		       vint16m4_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_i16m8_m (vbool2_t mask, int16_t *base, ptrdiff_t bstride,
		       vint16m8_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_u16mf4_m (vbool64_t mask, uint16_t *base, ptrdiff_t bstride,
			vuint16mf4_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_u16mf2_m (vbool32_t mask, uint16_t *base, ptrdiff_t bstride,
			vuint16mf2_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_u16m1_m (vbool16_t mask, uint16_t *base, ptrdiff_t bstride,
		       vuint16m1_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_u16m2_m (vbool8_t mask, uint16_t *base, ptrdiff_t bstride,
		       vuint16m2_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_u16m4_m (vbool4_t mask, uint16_t *base, ptrdiff_t bstride,
		       vuint16m4_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}

void
test_vsse16_v_u16m8_m (vbool2_t mask, uint16_t *base, ptrdiff_t bstride,
		       vuint16m8_t value, size_t vl)
{
  return __riscv_vsse16 (mask, base, bstride, value, vl);
}
