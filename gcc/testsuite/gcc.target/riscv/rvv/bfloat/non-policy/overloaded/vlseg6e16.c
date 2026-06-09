/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3 -mext-zvlsseg" } */

#include "riscv_vector.h"

vbfloat16mf4x6_t
test_vlseg6e16_v_bf16mf4x6_m (vbool64_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vbfloat16mf2x6_t
test_vlseg6e16_v_bf16mf2x6_m (vbool32_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vbfloat16m1x6_t
test_vlseg6e16_v_bf16m1x6_m (vbool16_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vint16mf4x6_t
test_vlseg6e16_v_i16mf4x6_m (vbool64_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vint16mf2x6_t
test_vlseg6e16_v_i16mf2x6_m (vbool32_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vint16m1x6_t
test_vlseg6e16_v_i16m1x6_m (vbool16_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vuint16mf4x6_t
test_vlseg6e16_v_u16mf4x6_m (vbool64_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vuint16mf2x6_t
test_vlseg6e16_v_u16mf2x6_m (vbool32_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}

vuint16m1x6_t
test_vlseg6e16_v_u16m1x6_m (vbool16_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vlseg6e16 (mask, base, vl);
}
