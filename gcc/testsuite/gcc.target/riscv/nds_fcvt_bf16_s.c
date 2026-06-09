/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xandes_xandesbfhcvt" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xandes_xandesbfhcvt" { target { rv64 } } } */

#include <nds_intrinsic.h>

__bf16
nds_fcvt_bf16_s (float a)
{
  return __riscv_nds_fcvt_bf16_s (a);
}

/* { dg-final { scan-assembler-times {nds\.fcvt\.bf16\.s} 1 } } */
