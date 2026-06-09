/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xandes_xandesbfhcvt" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xandes_xandesbfhcvt" { target { rv64 } } } */

#include <nds_intrinsic.h>

float
nds_fcvt_s_bf16 (__bf16 a)
{
  return __riscv_nds_fcvt_s_bf16 (a);
}

/* { dg-final { scan-assembler-times {nds\.fcvt\.s\.bf16} 1 } } */
