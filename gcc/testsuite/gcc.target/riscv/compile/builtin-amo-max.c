/* Verify that amomax.w instruction is generated by builtin function.  */

/* { dg-do compile } */
/* { dg-require-effective-target riscv_atomic } */
/* { dg-final { scan-assembler "\tamomax.w\t" } } */

#include <nds_intrinsic.h>

void
func (void)
{
  /* We want to perform an atomic max operation.  */
  signed int data, cmpv, oldv;
  cmpv = 10;
  /* new value: maximum (data, cmpv)  */
  oldv = __nds__amomaxw (cmpv, &data, UNORDER);
}
