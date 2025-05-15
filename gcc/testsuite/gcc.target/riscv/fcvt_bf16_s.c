/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xandes_xandesbfhcvt" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xandes_xandesbfhcvt" { target { rv64 } } } */
/* { dg-prune-output "warning: '.*' will be deprecated .*" } */

#include <nds_intrinsic.h>

__bf16
test_fcvt_bf16_s (float a)
{
  return __nds_fcvt_bf16_s (a);
}

/* { dg-final { scan-assembler-times {nds\.fcvt\.bf16\.s} 1 } } */
