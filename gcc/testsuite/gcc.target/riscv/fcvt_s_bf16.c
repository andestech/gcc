/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xandes_xandesbfhcvt" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xandes_xandesbfhcvt" { target { rv64 } } } */
/* { dg-prune-output "warning: '.*' will be deprecated .*" } */

#include <nds_intrinsic.h>

float
test_fcvt_s_bf16 (__bf16 a)
{
  return __nds_fcvt_s_bf16 (a);
}

/* { dg-final { scan-assembler-times {nds\.fcvt\.s\.bf16} 1 } } */
