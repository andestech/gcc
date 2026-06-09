/* { dg-do compile } */
/* { dg-require-effective-target rv64 } */
/* { dg-options "-march=rv64gc_zihintntl_zfhmin -mabi=lp64d -fno-optimize-sibling-calls" } */
/* { dg-skip-if "" { *-*-* } {"-Os" "-Oz"} } */
/* FIXME: unknown why -fno-optimize-sibling-calls is not working on Andes GCC,
   causing testcase to fail when compiled with -Os or -Oz due to tail call
   optimization.  */

#include <stdint.h>
#include <riscv_ntlh.h>

/* Each TEST should generate 3 ntl.all and 1 ntl.[p1, pall, s1].  */
#define TEST(NAME, TYPE)                                                       \
  void load_##NAME (TYPE *in, TYPE *out)                                       \
  {                                                                            \
    TYPE tmp;                                                                  \
    tmp = __riscv_ntl_load (in);                                               \
    __riscv_ntl_store (out, tmp);                                              \
                                                                               \
    tmp = __riscv_ntl_load (in + 1, __RISCV_NTLH_INNERMOST_PRIVATE);           \
    __riscv_ntl_store (out + 1, tmp, __RISCV_NTLH_ALL_PRIVATE);                \
                                                                               \
    tmp = __riscv_ntl_load (in + 2, __RISCV_NTLH_INNERMOST_SHARED);            \
    __riscv_ntl_store (out + 2, tmp, __RISCV_NTLH_ALL);                        \
  }

TEST (int8_t, int8_t)
TEST (int16_t, int16_t)
TEST (int32_t, int32_t)
TEST (int64_t, int64_t)
TEST (uint8_t, uint8_t)
TEST (uint16_t, uint16_t)
TEST (uint32_t, uint32_t)
TEST (uint64_t, uint64_t)

TEST (char, char)
TEST (short, short)
TEST (int, int)
TEST (long_int, long int)
TEST (unsigned_char, unsigned char)
TEST (unsigned_short, unsigned short)
TEST (unsigned_int, unsigned int)
TEST (unsigned_long_int, unsigned long int)

TEST (float16, _Float16)
TEST (float, float)
TEST (double, double)

/* FIXME: long long is not registered on rv64, see register_builtin_types()
   in riscv-vector-builtins.cc for more info.

   TEST(long_long, long long)
   TEST(unsigned_long_long, unsigned long long)
*/

/* { dg-final { scan-assembler-times "ntl.p1" 19 } } */
/* { dg-final { scan-assembler-times "ntl.pall" 19 } } */
/* { dg-final { scan-assembler-times "ntl.s1" 19 } } */
/* { dg-final { scan-assembler-times "ntl.all" 57 } } */
