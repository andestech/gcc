/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

vbfloat16mf4x3_t
test_vlseg3e16ff_v_bf16mf4x3 (const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16mf4x3 (base, new_vl, vl);
}

vbfloat16mf2x3_t
test_vlseg3e16ff_v_bf16mf2x3 (const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16mf2x3 (base, new_vl, vl);
}

vbfloat16m1x3_t
test_vlseg3e16ff_v_bf16m1x3 (const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16m1x3 (base, new_vl, vl);
}

vbfloat16m2x3_t
test_vlseg3e16ff_v_bf16m2x3 (const __bf16 *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16m2x3 (base, new_vl, vl);
}

vint16mf4x3_t
test_vlseg3e16ff_v_i16mf4x3 (const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16mf4x3 (base, new_vl, vl);
}

vint16mf2x3_t
test_vlseg3e16ff_v_i16mf2x3 (const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16mf2x3 (base, new_vl, vl);
}

vint16m1x3_t
test_vlseg3e16ff_v_i16m1x3 (const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16m1x3 (base, new_vl, vl);
}

vint16m2x3_t
test_vlseg3e16ff_v_i16m2x3 (const int16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16m2x3 (base, new_vl, vl);
}

vuint16mf4x3_t
test_vlseg3e16ff_v_u16mf4x3 (const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16mf4x3 (base, new_vl, vl);
}

vuint16mf2x3_t
test_vlseg3e16ff_v_u16mf2x3 (const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16mf2x3 (base, new_vl, vl);
}

vuint16m1x3_t
test_vlseg3e16ff_v_u16m1x3 (const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16m1x3 (base, new_vl, vl);
}

vuint16m2x3_t
test_vlseg3e16ff_v_u16m2x3 (const uint16_t *base, size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16m2x3 (base, new_vl, vl);
}

vbfloat16mf4x3_t
test_vlseg3e16ff_v_bf16mf4x3_m (vbool64_t mask, const __bf16 *base,
				size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16mf4x3_m (mask, base, new_vl, vl);
}

vbfloat16mf2x3_t
test_vlseg3e16ff_v_bf16mf2x3_m (vbool32_t mask, const __bf16 *base,
				size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16mf2x3_m (mask, base, new_vl, vl);
}

vbfloat16m1x3_t
test_vlseg3e16ff_v_bf16m1x3_m (vbool16_t mask, const __bf16 *base,
			       size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16m1x3_m (mask, base, new_vl, vl);
}

vbfloat16m2x3_t
test_vlseg3e16ff_v_bf16m2x3_m (vbool8_t mask, const __bf16 *base,
			       size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_bf16m2x3_m (mask, base, new_vl, vl);
}

vint16mf4x3_t
test_vlseg3e16ff_v_i16mf4x3_m (vbool64_t mask, const int16_t *base,
			       size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16mf4x3_m (mask, base, new_vl, vl);
}

vint16mf2x3_t
test_vlseg3e16ff_v_i16mf2x3_m (vbool32_t mask, const int16_t *base,
			       size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16mf2x3_m (mask, base, new_vl, vl);
}

vint16m1x3_t
test_vlseg3e16ff_v_i16m1x3_m (vbool16_t mask, const int16_t *base,
			      size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16m1x3_m (mask, base, new_vl, vl);
}

vint16m2x3_t
test_vlseg3e16ff_v_i16m2x3_m (vbool8_t mask, const int16_t *base,
			      size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_i16m2x3_m (mask, base, new_vl, vl);
}

vuint16mf4x3_t
test_vlseg3e16ff_v_u16mf4x3_m (vbool64_t mask, const uint16_t *base,
			       size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16mf4x3_m (mask, base, new_vl, vl);
}

vuint16mf2x3_t
test_vlseg3e16ff_v_u16mf2x3_m (vbool32_t mask, const uint16_t *base,
			       size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16mf2x3_m (mask, base, new_vl, vl);
}

vuint16m1x3_t
test_vlseg3e16ff_v_u16m1x3_m (vbool16_t mask, const uint16_t *base,
			      size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16m1x3_m (mask, base, new_vl, vl);
}

vuint16m2x3_t
test_vlseg3e16ff_v_u16m2x3_m (vbool8_t mask, const uint16_t *base,
			      size_t *new_vl, size_t vl)
{
  return __riscv_vlseg3e16ff_v_u16m2x3_m (mask, base, new_vl, vl);
}
