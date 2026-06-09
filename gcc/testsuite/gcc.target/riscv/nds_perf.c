/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xandes_xandesperf" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xandes_xandesperf" { target { rv64 } } } */

#include <nds_intrinsic.h>

long
test_nds_ffb (unsigned long a, unsigned long b)
{
  return __riscv_nds_ffb (a, b);
}

long
test_nds_ffmism (unsigned long a, unsigned long b)
{
  return __riscv_nds_ffmism (a, b);
}

long
test_nds_flmism (unsigned long a, unsigned long b)
{
  return __riscv_nds_flmism (a, b);
}

/* { dg-final { scan-assembler-times {nds\.ffb} 1 } } */
/* { dg-final { scan-assembler-times {nds\.ffmism} 1 } } */
/* { dg-final { scan-assembler-times {nds\.flmism} 1 } } */
