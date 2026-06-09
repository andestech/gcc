/* { dg-do compile } */
/* { dg-require-effective-target rv64 } */
/* { dg-options "-march=rv64gcv_zihintntl_zvfhmin -mabi=lp64d -fno-optimize-sibling-calls" } */

#include <riscv_vector.h>
#include <riscv_ntlh.h>

/* Each TEST should generate 3 ntl.all and 1 ntl.[p1, pall, s1].  */
#define TEST(TYPE)                                                             \
  void load_##TYPE (TYPE *in, TYPE *out)                                       \
  {                                                                            \
    TYPE tmp;                                                                  \
    tmp = __riscv_ntl_load (in);                                               \
    __riscv_ntl_store (out, tmp);                                              \
                                                                               \
    tmp = __riscv_ntl_load (in, __RISCV_NTLH_INNERMOST_PRIVATE);               \
    __riscv_ntl_store (out, tmp, __RISCV_NTLH_ALL_PRIVATE);                    \
                                                                               \
    tmp = __riscv_ntl_load (in, __RISCV_NTLH_INNERMOST_SHARED);                \
    __riscv_ntl_store (out, tmp, __RISCV_NTLH_ALL);                            \
  }

TEST (vint8m2_t)
TEST (vint16m2_t)
TEST (vint32m2_t)
TEST (vint64m2_t)
TEST (vuint8m1_t)
TEST (vuint16m1_t)
TEST (vuint32m1_t)
TEST (vuint64m1_t)
TEST (vfloat16m4_t)
TEST (vfloat32m4_t)
TEST (vfloat64m4_t)

/* { dg-final { scan-assembler-times "ntl.p1" 11 } } */
/* { dg-final { scan-assembler-times "ntl.pall" 11 } } */
/* { dg-final { scan-assembler-times "ntl.s1" 11 } } */
/* { dg-final { scan-assembler-times "ntl.all" 33 } } */
