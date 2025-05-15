/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xandes_xandesperf" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xandes_xandesperf" { target { rv64 } } } */
/* { dg-prune-output "warning: '.*' will be deprecated .*" } */

#include <nds_intrinsic.h>

long
test__nds__ffb (unsigned long a, unsigned long b)
{
  return __nds__ffb (a, b);
}

long
test__nds__ffmism (unsigned long a, unsigned long b)
{
  return __nds__ffmism (a, b);
}

long
test__nds__flmism (unsigned long a, unsigned long b)
{
  return __nds__flmism (a, b);
}

/* { dg-final { scan-assembler-times {nds\.ffb} 1 } } */
/* { dg-final { scan-assembler-times {nds\.ffmism} 1 } } */
/* { dg-final { scan-assembler-times {nds\.flmism} 1 } } */
