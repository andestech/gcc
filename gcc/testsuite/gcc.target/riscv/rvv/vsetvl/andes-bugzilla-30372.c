/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -O3" } */

#include "riscv_vector.h"

#define VLEN_IN_BIT 512

__attribute__ ((noinline)) void static acei32m1 (vint32m1_t data,
						 const size_t addrCtl,
						 const unsigned int func,
						 const unsigned int mode,
						 size_t vl)
{
  __asm__ __volatile__ (
    "svstore %[data], addrCtl_%[addrCtl], %[func], %[mode], %[avl]"
    :
    : [data] "vr"(data), [addrCtl] "r"(addrCtl), [func] "i"(0), [mode] "i"(0),
      [avl] "ri"(vl), [vtype] "i"(16)
    : "memory");
}

static inline vint32m1_t __attribute__ ((always_inline))
amm_i32m1 (vint32m1_t result_input, vint8m1_t data_b, vint8m1_t data_a,
	   const unsigned int portion, const unsigned int dec)
{
  vint32m1_t result_RetVar;
  __asm__ __volatile__ (
    "amm %[result], %[data_b], %[data_a], %[portion], %[dec]"
    : [result] "=&vr"(result_RetVar)
    : "[result]"(result_input), [data_b] "vr"(data_b), [data_a] "vr"(data_a),
      [portion] "i"(0), [dec] "i"(0)
    : "memory");
  return result_RetVar;
}

static inline vint8m1_t
ace_svload_i8m1_st_idx (const unsigned int addrCtl, const unsigned int func,
			const unsigned int mode, size_t vl)
{
  vint8m1_t data_RetVar;
  (void) vl;
  __asm__ __volatile__ (
#ifndef __clang__
    "vsetvl zero, %[avl], %[vtype]\n\t"
#endif
    "svload %[data], addrCtl_%[addrCtl], %[func], %[mode]"
    : [data] "=&vr"(data_RetVar)
    : [addrCtl] "i"(0), [func] "i"(0), [mode] "i"(0), [avl] "ri"(vl),
      [vtype] "r"(0)
    : "memory");
  return data_RetVar;
}

void
foo (vint32m1_t a, vint32m1_t b, vint32m1_t c, size_t addr, uint64_t k,
     uint64_t count)
{
  size_t vl = __riscv_vsetvl_e32m1 (VLEN_IN_BIT);
  acei32m1 (a, addr, 1, 0, vl);
  vl = __riscv_vsetvl_e8m1 (VLEN_IN_BIT);
  vint8m1_t a8 = ace_svload_i8m1_st_idx (0, 32, 0, vl);
  while (k > 0)
    {
      vint8m1_t b8 = ace_svload_i8m1_st_idx (k, 32, 1, vl);
      a = amm_i32m1 (a, a8, b8, 0, vl);
      k -= count;
      a8 = ace_svload_i8m1_st_idx (k, 32, 0, vl);
    }
  vl = __riscv_ace_vsetvl_e32m1 (VLEN_IN_BIT); // Expected the side-effect ace vsetvl should be emit.
  acei32m1 (c, addr, 3, 0, vl);
}

/* { dg-final { scan-assembler-times {vsetvli} 3 { target { no-opts "-O0" no-opts "-g" no-opts "-funroll-loops" } } } } */
