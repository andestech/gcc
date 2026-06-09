/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vbfloat16mf4_t
test_vle16_v_bf16mf4_m (vbool64_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vbfloat16mf2_t
test_vle16_v_bf16mf2_m (vbool32_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vbfloat16m1_t
test_vle16_v_bf16m1_m (vbool16_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vbfloat16m2_t
test_vle16_v_bf16m2_m (vbool8_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vbfloat16m4_t
test_vle16_v_bf16m4_m (vbool4_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vbfloat16m8_t
test_vle16_v_bf16m8_m (vbool2_t mask, const __bf16 *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vint16mf4_t
test_vle16_v_i16mf4_m (vbool64_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vint16mf2_t
test_vle16_v_i16mf2_m (vbool32_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vint16m1_t
test_vle16_v_i16m1_m (vbool16_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vint16m2_t
test_vle16_v_i16m2_m (vbool8_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vint16m4_t
test_vle16_v_i16m4_m (vbool4_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vint16m8_t
test_vle16_v_i16m8_m (vbool2_t mask, const int16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vuint16mf4_t
test_vle16_v_u16mf4_m (vbool64_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vuint16mf2_t
test_vle16_v_u16mf2_m (vbool32_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vuint16m1_t
test_vle16_v_u16m1_m (vbool16_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vuint16m2_t
test_vle16_v_u16m2_m (vbool8_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vuint16m4_t
test_vle16_v_u16m4_m (vbool4_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}

vuint16m8_t
test_vle16_v_u16m8_m (vbool2_t mask, const uint16_t *base, size_t vl)
{
  return __riscv_vle16 (mask, base, vl);
}
