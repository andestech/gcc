/* Verify that amoswap.w instruction is generated by builtin function.  */

/* { dg-do compile } */
/* { dg-require-effective-target riscv_atomic } */
/* { dg-final { scan-assembler "\tamoswap.w\t" } } */

#include <nds_intrinsic.h>

void
func (void)
{
  /* We want to perform an atomic swap operation.  */
  signed int data, newv, oldv;
  newv = 10;
  /* new value: 10  */
  oldv = __nds__amoswapw (newv, &data, UNORDER);
}
