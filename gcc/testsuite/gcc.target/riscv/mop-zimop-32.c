/* { dg-do compile } */
/* { dg-require-effective-target rv32 } */
/* { dg-options "-march=rv32i_zimop -mabi=ilp32" } */

#include <stdint.h>

uint32_t
mopr_0_32 (uint32_t a)
{
  return __builtin_riscv_mopr_32 (a, 0);
}

uint32_t
mopr_31_32 (uint32_t a)
{
  return __builtin_riscv_mopr_32 (a, 31);
}

uint32_t
moprr_0_32 (uint32_t a, uint32_t b)
{
  return __builtin_riscv_moprr_32 (a, b, 0);
}

uint32_t
moprr_7_32 (uint32_t a, uint32_t b)
{
  return __builtin_riscv_moprr_32 (a, b, 7);
}

/* { dg-final { scan-assembler-times {mop\.r\.[0-9]+} 2 } } */
/* { dg-final { scan-assembler-times {mop\.rr\.[0-9]+} 2 } } */
