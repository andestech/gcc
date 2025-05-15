/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

void
test_vsseg4e16_v_bf16mf4x4 (__bf16 *base, vbfloat16mf4x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_bf16mf2x4 (__bf16 *base, vbfloat16mf2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_bf16m1x4 (__bf16 *base, vbfloat16m1x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_bf16m2x4 (__bf16 *base, vbfloat16m2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16mf4x4 (int16_t *base, vint16mf4x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16mf2x4 (int16_t *base, vint16mf2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16m1x4 (int16_t *base, vint16m1x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16m2x4 (int16_t *base, vint16m2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16mf4x4 (uint16_t *base, vuint16mf4x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16mf2x4 (uint16_t *base, vuint16mf2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16m1x4 (uint16_t *base, vuint16m1x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16m2x4 (uint16_t *base, vuint16m2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (base, v_tuple, vl);
}

void
test_vsseg4e16_v_bf16mf4x4_m (vbool64_t mask, __bf16 *base,
			      vbfloat16mf4x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_bf16mf2x4_m (vbool32_t mask, __bf16 *base,
			      vbfloat16mf2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_bf16m1x4_m (vbool16_t mask, __bf16 *base,
			     vbfloat16m1x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_bf16m2x4_m (vbool8_t mask, __bf16 *base,
			     vbfloat16m2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16mf4x4_m (vbool64_t mask, int16_t *base,
			     vint16mf4x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16mf2x4_m (vbool32_t mask, int16_t *base,
			     vint16mf2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16m1x4_m (vbool16_t mask, int16_t *base, vint16m1x4_t v_tuple,
			    size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_i16m2x4_m (vbool8_t mask, int16_t *base, vint16m2x4_t v_tuple,
			    size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16mf4x4_m (vbool64_t mask, uint16_t *base,
			     vuint16mf4x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16mf2x4_m (vbool32_t mask, uint16_t *base,
			     vuint16mf2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16m1x4_m (vbool16_t mask, uint16_t *base,
			    vuint16m1x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}

void
test_vsseg4e16_v_u16m2x4_m (vbool8_t mask, uint16_t *base,
			    vuint16m2x4_t v_tuple, size_t vl)
{
  return __riscv_vsseg4e16 (mask, base, v_tuple, vl);
}
