/* { dg-do compile } */
/* { dg-require-effective-target rv64 } */
/* { dg-options "-march=rv64i_zimop -mabi=lp64" } */

#include <stdint.h>

uint64_t
mopr_0_64 (uint64_t a)
{
  return __builtin_riscv_mopr_64 (a, 0);
}

uint64_t
mopr_31_64 (uint64_t a)
{
  return __builtin_riscv_mopr_64 (a, 31);
}

uint64_t
moprr_0_64 (uint64_t a, uint64_t b)
{
  return __builtin_riscv_moprr_64 (a, b, 0);
}

uint64_t
moprr_7_64 (uint64_t a, uint64_t b)
{
  return __builtin_riscv_moprr_64 (a, b, 7);
}
/* { dg-final { scan-assembler-times {mop\.r\.[0-9]+} 2 } } */
/* { dg-final { scan-assembler-times {mop\.rr\.[0-9]+} 2 } } */
