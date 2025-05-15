/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xandesbf -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vint8mf8_t
test_vfncvt_x_f_w_i8mf8 (vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint8mf4_t
test_vfncvt_x_f_w_i8mf4 (vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint8mf2_t
test_vfncvt_x_f_w_i8mf2 (vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint8m1_t
test_vfncvt_x_f_w_i8m1 (vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint8m2_t
test_vfncvt_x_f_w_i8m2 (vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint8m4_t
test_vfncvt_x_f_w_i8m4 (vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vuint8mf8_t
test_vfncvt_xu_f_w_u8mf8 (vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint8mf4_t
test_vfncvt_xu_f_w_u8mf4 (vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint8mf2_t
test_vfncvt_xu_f_w_u8mf2 (vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint8m1_t
test_vfncvt_xu_f_w_u8m1 (vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint8m2_t
test_vfncvt_xu_f_w_u8m2 (vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint8m4_t
test_vfncvt_xu_f_w_u8m4 (vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vint16mf4_t
test_vfncvt_x_f_w_i16mf4 (vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint16mf2_t
test_vfncvt_x_f_w_i16mf2 (vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint16m1_t
test_vfncvt_x_f_w_i16m1 (vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint16m2_t
test_vfncvt_x_f_w_i16m2 (vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint16m4_t
test_vfncvt_x_f_w_i16m4 (vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vuint16mf4_t
test_vfncvt_xu_f_w_u16mf4 (vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint16mf2_t
test_vfncvt_xu_f_w_u16mf2 (vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint16m1_t
test_vfncvt_xu_f_w_u16m1 (vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint16m2_t
test_vfncvt_xu_f_w_u16m2 (vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint16m4_t
test_vfncvt_xu_f_w_u16m4 (vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vbfloat16mf4_t
test_vfncvt_f_x_w_bf16mf4 (vint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16mf2_t
test_vfncvt_f_x_w_bf16mf2 (vint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m1_t
test_vfncvt_f_x_w_bf16m1 (vint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m2_t
test_vfncvt_f_x_w_bf16m2 (vint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m4_t
test_vfncvt_f_x_w_bf16m4 (vint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16mf4_t
test_vfncvt_f_xu_w_bf16mf4 (vuint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16mf2_t
test_vfncvt_f_xu_w_bf16mf2 (vuint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m1_t
test_vfncvt_f_xu_w_bf16m1 (vuint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m2_t
test_vfncvt_f_xu_w_bf16m2 (vuint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m4_t
test_vfncvt_f_xu_w_bf16m4 (vuint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16mf4_t
test_vfncvt_f_f_w_bf16mf4 (vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16mf2_t
test_vfncvt_f_f_w_bf16mf2 (vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m1_t
test_vfncvt_f_f_w_bf16m1 (vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m2_t
test_vfncvt_f_f_w_bf16m2 (vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vbfloat16m4_t
test_vfncvt_f_f_w_bf16m4 (vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, vl);
}

vint32mf2_t
test_vfncvt_x_f_w_i32mf2 (vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint32m1_t
test_vfncvt_x_f_w_i32m1 (vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint32m2_t
test_vfncvt_x_f_w_i32m2 (vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vint32m4_t
test_vfncvt_x_f_w_i32m4 (vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, vl);
}

vuint32mf2_t
test_vfncvt_xu_f_w_u32mf2 (vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint32m1_t
test_vfncvt_xu_f_w_u32m1 (vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint32m2_t
test_vfncvt_xu_f_w_u32m2 (vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vuint32m4_t
test_vfncvt_xu_f_w_u32m4 (vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, vl);
}

vfloat32mf2_t
test_vfncvt_f_x_w_f32mf2 (vint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m1_t
test_vfncvt_f_x_w_f32m1 (vint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m2_t
test_vfncvt_f_x_w_f32m2 (vint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m4_t
test_vfncvt_f_x_w_f32m4 (vint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32mf2_t
test_vfncvt_f_xu_w_f32mf2 (vuint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m1_t
test_vfncvt_f_xu_w_f32m1 (vuint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m2_t
test_vfncvt_f_xu_w_f32m2 (vuint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m4_t
test_vfncvt_f_xu_w_f32m4 (vuint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32mf2_t
test_vfncvt_f_f_w_f32mf2 (vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m1_t
test_vfncvt_f_f_w_f32m1 (vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m2_t
test_vfncvt_f_f_w_f32m2 (vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vfloat32m4_t
test_vfncvt_f_f_w_f32m4 (vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, vl);
}

vint8mf8_t
test_vfncvt_x_f_w_i8mf8_m (vbool64_t mask, vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint8mf4_t
test_vfncvt_x_f_w_i8mf4_m (vbool32_t mask, vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint8mf2_t
test_vfncvt_x_f_w_i8mf2_m (vbool16_t mask, vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint8m1_t
test_vfncvt_x_f_w_i8m1_m (vbool8_t mask, vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint8m2_t
test_vfncvt_x_f_w_i8m2_m (vbool4_t mask, vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint8m4_t
test_vfncvt_x_f_w_i8m4_m (vbool2_t mask, vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vuint8mf8_t
test_vfncvt_xu_f_w_u8mf8_m (vbool64_t mask, vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint8mf4_t
test_vfncvt_xu_f_w_u8mf4_m (vbool32_t mask, vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint8mf2_t
test_vfncvt_xu_f_w_u8mf2_m (vbool16_t mask, vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint8m1_t
test_vfncvt_xu_f_w_u8m1_m (vbool8_t mask, vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint8m2_t
test_vfncvt_xu_f_w_u8m2_m (vbool4_t mask, vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint8m4_t
test_vfncvt_xu_f_w_u8m4_m (vbool2_t mask, vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vint16mf4_t
test_vfncvt_x_f_w_i16mf4_m (vbool64_t mask, vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint16mf2_t
test_vfncvt_x_f_w_i16mf2_m (vbool32_t mask, vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint16m1_t
test_vfncvt_x_f_w_i16m1_m (vbool16_t mask, vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint16m2_t
test_vfncvt_x_f_w_i16m2_m (vbool8_t mask, vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint16m4_t
test_vfncvt_x_f_w_i16m4_m (vbool4_t mask, vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vuint16mf4_t
test_vfncvt_xu_f_w_u16mf4_m (vbool64_t mask, vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint16mf2_t
test_vfncvt_xu_f_w_u16mf2_m (vbool32_t mask, vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint16m1_t
test_vfncvt_xu_f_w_u16m1_m (vbool16_t mask, vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint16m2_t
test_vfncvt_xu_f_w_u16m2_m (vbool8_t mask, vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint16m4_t
test_vfncvt_xu_f_w_u16m4_m (vbool4_t mask, vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vbfloat16mf4_t
test_vfncvt_f_x_w_bf16mf4_m (vbool64_t mask, vint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16mf2_t
test_vfncvt_f_x_w_bf16mf2_m (vbool32_t mask, vint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m1_t
test_vfncvt_f_x_w_bf16m1_m (vbool16_t mask, vint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m2_t
test_vfncvt_f_x_w_bf16m2_m (vbool8_t mask, vint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m4_t
test_vfncvt_f_x_w_bf16m4_m (vbool4_t mask, vint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16mf4_t
test_vfncvt_f_xu_w_bf16mf4_m (vbool64_t mask, vuint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16mf2_t
test_vfncvt_f_xu_w_bf16mf2_m (vbool32_t mask, vuint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m1_t
test_vfncvt_f_xu_w_bf16m1_m (vbool16_t mask, vuint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m2_t
test_vfncvt_f_xu_w_bf16m2_m (vbool8_t mask, vuint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m4_t
test_vfncvt_f_xu_w_bf16m4_m (vbool4_t mask, vuint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16mf4_t
test_vfncvt_f_f_w_bf16mf4_m (vbool64_t mask, vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16mf2_t
test_vfncvt_f_f_w_bf16mf2_m (vbool32_t mask, vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m1_t
test_vfncvt_f_f_w_bf16m1_m (vbool16_t mask, vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m2_t
test_vfncvt_f_f_w_bf16m2_m (vbool8_t mask, vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vbfloat16m4_t
test_vfncvt_f_f_w_bf16m4_m (vbool4_t mask, vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, vl);
}

vint32mf2_t
test_vfncvt_x_f_w_i32mf2_m (vbool64_t mask, vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint32m1_t
test_vfncvt_x_f_w_i32m1_m (vbool32_t mask, vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint32m2_t
test_vfncvt_x_f_w_i32m2_m (vbool16_t mask, vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vint32m4_t
test_vfncvt_x_f_w_i32m4_m (vbool8_t mask, vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, vl);
}

vuint32mf2_t
test_vfncvt_xu_f_w_u32mf2_m (vbool64_t mask, vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint32m1_t
test_vfncvt_xu_f_w_u32m1_m (vbool32_t mask, vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint32m2_t
test_vfncvt_xu_f_w_u32m2_m (vbool16_t mask, vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vuint32m4_t
test_vfncvt_xu_f_w_u32m4_m (vbool8_t mask, vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, vl);
}

vfloat32mf2_t
test_vfncvt_f_x_w_f32mf2_m (vbool64_t mask, vint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m1_t
test_vfncvt_f_x_w_f32m1_m (vbool32_t mask, vint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m2_t
test_vfncvt_f_x_w_f32m2_m (vbool16_t mask, vint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m4_t
test_vfncvt_f_x_w_f32m4_m (vbool8_t mask, vint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32mf2_t
test_vfncvt_f_xu_w_f32mf2_m (vbool64_t mask, vuint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m1_t
test_vfncvt_f_xu_w_f32m1_m (vbool32_t mask, vuint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m2_t
test_vfncvt_f_xu_w_f32m2_m (vbool16_t mask, vuint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m4_t
test_vfncvt_f_xu_w_f32m4_m (vbool8_t mask, vuint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32mf2_t
test_vfncvt_f_f_w_f32mf2_m (vbool64_t mask, vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m1_t
test_vfncvt_f_f_w_f32m1_m (vbool32_t mask, vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m2_t
test_vfncvt_f_f_w_f32m2_m (vbool16_t mask, vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vfloat32m4_t
test_vfncvt_f_f_w_f32m4_m (vbool8_t mask, vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, vl);
}

vint8mf8_t
test_vfncvt_x_f_w_i8mf8_rm (vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint8mf4_t
test_vfncvt_x_f_w_i8mf4_rm (vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint8mf2_t
test_vfncvt_x_f_w_i8mf2_rm (vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint8m1_t
test_vfncvt_x_f_w_i8m1_rm (vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint8m2_t
test_vfncvt_x_f_w_i8m2_rm (vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint8m4_t
test_vfncvt_x_f_w_i8m4_rm (vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t
test_vfncvt_xu_f_w_u8mf8_rm (vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t
test_vfncvt_xu_f_w_u8mf4_rm (vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t
test_vfncvt_xu_f_w_u8mf2_rm (vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint8m1_t
test_vfncvt_xu_f_w_u8m1_rm (vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint8m2_t
test_vfncvt_xu_f_w_u8m2_rm (vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint8m4_t
test_vfncvt_xu_f_w_u8m4_rm (vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vint16mf4_t
test_vfncvt_x_f_w_i16mf4_rm (vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint16mf2_t
test_vfncvt_x_f_w_i16mf2_rm (vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint16m1_t
test_vfncvt_x_f_w_i16m1_rm (vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint16m2_t
test_vfncvt_x_f_w_i16m2_rm (vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint16m4_t
test_vfncvt_x_f_w_i16m4_rm (vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vuint16mf4_t
test_vfncvt_xu_f_w_u16mf4_rm (vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint16mf2_t
test_vfncvt_xu_f_w_u16mf2_rm (vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint16m1_t
test_vfncvt_xu_f_w_u16m1_rm (vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint16m2_t
test_vfncvt_xu_f_w_u16m2_rm (vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint16m4_t
test_vfncvt_xu_f_w_u16m4_rm (vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfncvt_f_x_w_bf16mf4_rm (vint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfncvt_f_x_w_bf16mf2_rm (vint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfncvt_f_x_w_bf16m1_rm (vint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfncvt_f_x_w_bf16m2_rm (vint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfncvt_f_x_w_bf16m4_rm (vint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfncvt_f_xu_w_bf16mf4_rm (vuint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfncvt_f_xu_w_bf16mf2_rm (vuint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfncvt_f_xu_w_bf16m1_rm (vuint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfncvt_f_xu_w_bf16m2_rm (vuint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfncvt_f_xu_w_bf16m4_rm (vuint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfncvt_f_f_w_bf16mf4_rm (vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfncvt_f_f_w_bf16mf2_rm (vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfncvt_f_f_w_bf16m1_rm (vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfncvt_f_f_w_bf16m2_rm (vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfncvt_f_f_w_bf16m4_rm (vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (src, __RISCV_FRM_RNE, vl);
}

vint32mf2_t
test_vfncvt_x_f_w_i32mf2_rm (vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint32m1_t
test_vfncvt_x_f_w_i32m1_rm (vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint32m2_t
test_vfncvt_x_f_w_i32m2_rm (vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vint32m4_t
test_vfncvt_x_f_w_i32m4_rm (vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (src, __RISCV_FRM_RNE, vl);
}

vuint32mf2_t
test_vfncvt_xu_f_w_u32mf2_rm (vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint32m1_t
test_vfncvt_xu_f_w_u32m1_rm (vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint32m2_t
test_vfncvt_xu_f_w_u32m2_rm (vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vuint32m4_t
test_vfncvt_xu_f_w_u32m4_rm (vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (src, __RISCV_FRM_RNE, vl);
}

vfloat32mf2_t
test_vfncvt_f_x_w_f32mf2_rm (vint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfncvt_f_x_w_f32m1_rm (vint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m2_t
test_vfncvt_f_x_w_f32m2_rm (vint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m4_t
test_vfncvt_f_x_w_f32m4_rm (vint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32mf2_t
test_vfncvt_f_xu_w_f32mf2_rm (vuint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfncvt_f_xu_w_f32m1_rm (vuint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m2_t
test_vfncvt_f_xu_w_f32m2_rm (vuint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m4_t
test_vfncvt_f_xu_w_f32m4_rm (vuint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32mf2_t
test_vfncvt_f_f_w_f32mf2_rm (vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfncvt_f_f_w_f32m1_rm (vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m2_t
test_vfncvt_f_f_w_f32m2_rm (vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vfloat32m4_t
test_vfncvt_f_f_w_f32m4_rm (vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (src, __RISCV_FRM_RNE, vl);
}

vint8mf8_t
test_vfncvt_x_f_w_i8mf8_rm_m (vbool64_t mask, vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint8mf4_t
test_vfncvt_x_f_w_i8mf4_rm_m (vbool32_t mask, vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint8mf2_t
test_vfncvt_x_f_w_i8mf2_rm_m (vbool16_t mask, vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint8m1_t
test_vfncvt_x_f_w_i8m1_rm_m (vbool8_t mask, vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint8m2_t
test_vfncvt_x_f_w_i8m2_rm_m (vbool4_t mask, vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint8m4_t
test_vfncvt_x_f_w_i8m4_rm_m (vbool2_t mask, vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t
test_vfncvt_xu_f_w_u8mf8_rm_m (vbool64_t mask, vbfloat16mf4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t
test_vfncvt_xu_f_w_u8mf4_rm_m (vbool32_t mask, vbfloat16mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t
test_vfncvt_xu_f_w_u8mf2_rm_m (vbool16_t mask, vbfloat16m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint8m1_t
test_vfncvt_xu_f_w_u8m1_rm_m (vbool8_t mask, vbfloat16m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint8m2_t
test_vfncvt_xu_f_w_u8m2_rm_m (vbool4_t mask, vbfloat16m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint8m4_t
test_vfncvt_xu_f_w_u8m4_rm_m (vbool2_t mask, vbfloat16m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vint16mf4_t
test_vfncvt_x_f_w_i16mf4_rm_m (vbool64_t mask, vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint16mf2_t
test_vfncvt_x_f_w_i16mf2_rm_m (vbool32_t mask, vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint16m1_t
test_vfncvt_x_f_w_i16m1_rm_m (vbool16_t mask, vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint16m2_t
test_vfncvt_x_f_w_i16m2_rm_m (vbool8_t mask, vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint16m4_t
test_vfncvt_x_f_w_i16m4_rm_m (vbool4_t mask, vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vuint16mf4_t
test_vfncvt_xu_f_w_u16mf4_rm_m (vbool64_t mask, vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint16mf2_t
test_vfncvt_xu_f_w_u16mf2_rm_m (vbool32_t mask, vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint16m1_t
test_vfncvt_xu_f_w_u16m1_rm_m (vbool16_t mask, vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint16m2_t
test_vfncvt_xu_f_w_u16m2_rm_m (vbool8_t mask, vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint16m4_t
test_vfncvt_xu_f_w_u16m4_rm_m (vbool4_t mask, vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfncvt_f_x_w_bf16mf4_rm_m (vbool64_t mask, vint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfncvt_f_x_w_bf16mf2_rm_m (vbool32_t mask, vint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfncvt_f_x_w_bf16m1_rm_m (vbool16_t mask, vint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfncvt_f_x_w_bf16m2_rm_m (vbool8_t mask, vint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfncvt_f_x_w_bf16m4_rm_m (vbool4_t mask, vint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfncvt_f_xu_w_bf16mf4_rm_m (vbool64_t mask, vuint32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfncvt_f_xu_w_bf16mf2_rm_m (vbool32_t mask, vuint32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfncvt_f_xu_w_bf16m1_rm_m (vbool16_t mask, vuint32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfncvt_f_xu_w_bf16m2_rm_m (vbool8_t mask, vuint32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfncvt_f_xu_w_bf16m4_rm_m (vbool4_t mask, vuint32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf4_t
test_vfncvt_f_f_w_bf16mf4_rm_m (vbool64_t mask, vfloat32mf2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16mf2_t
test_vfncvt_f_f_w_bf16mf2_rm_m (vbool32_t mask, vfloat32m1_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m1_t
test_vfncvt_f_f_w_bf16m1_rm_m (vbool16_t mask, vfloat32m2_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m2_t
test_vfncvt_f_f_w_bf16m2_rm_m (vbool8_t mask, vfloat32m4_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vbfloat16m4_t
test_vfncvt_f_f_w_bf16m4_rm_m (vbool4_t mask, vfloat32m8_t src, size_t vl)
{
  return __riscv_vfncvt_bf (mask, src, __RISCV_FRM_RNE, vl);
}

vint32mf2_t
test_vfncvt_x_f_w_i32mf2_rm_m (vbool64_t mask, vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint32m1_t
test_vfncvt_x_f_w_i32m1_rm_m (vbool32_t mask, vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint32m2_t
test_vfncvt_x_f_w_i32m2_rm_m (vbool16_t mask, vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vint32m4_t
test_vfncvt_x_f_w_i32m4_rm_m (vbool8_t mask, vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_x (mask, src, __RISCV_FRM_RNE, vl);
}

vuint32mf2_t
test_vfncvt_xu_f_w_u32mf2_rm_m (vbool64_t mask, vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint32m1_t
test_vfncvt_xu_f_w_u32m1_rm_m (vbool32_t mask, vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint32m2_t
test_vfncvt_xu_f_w_u32m2_rm_m (vbool16_t mask, vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vuint32m4_t
test_vfncvt_xu_f_w_u32m4_rm_m (vbool8_t mask, vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_xu (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32mf2_t
test_vfncvt_f_x_w_f32mf2_rm_m (vbool64_t mask, vint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfncvt_f_x_w_f32m1_rm_m (vbool32_t mask, vint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m2_t
test_vfncvt_f_x_w_f32m2_rm_m (vbool16_t mask, vint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m4_t
test_vfncvt_f_x_w_f32m4_rm_m (vbool8_t mask, vint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32mf2_t
test_vfncvt_f_xu_w_f32mf2_rm_m (vbool64_t mask, vuint64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfncvt_f_xu_w_f32m1_rm_m (vbool32_t mask, vuint64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m2_t
test_vfncvt_f_xu_w_f32m2_rm_m (vbool16_t mask, vuint64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m4_t
test_vfncvt_f_xu_w_f32m4_rm_m (vbool8_t mask, vuint64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32mf2_t
test_vfncvt_f_f_w_f32mf2_rm_m (vbool64_t mask, vfloat64m1_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m1_t
test_vfncvt_f_f_w_f32m1_rm_m (vbool32_t mask, vfloat64m2_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m2_t
test_vfncvt_f_f_w_f32m2_rm_m (vbool16_t mask, vfloat64m4_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}

vfloat32m4_t
test_vfncvt_f_f_w_f32m4_rm_m (vbool8_t mask, vfloat64m8_t src, size_t vl)
{
  return __riscv_vfncvt_f (mask, src, __RISCV_FRM_RNE, vl);
}
