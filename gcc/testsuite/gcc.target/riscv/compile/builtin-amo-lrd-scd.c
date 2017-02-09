/* Verify that lr.d and sc.d instruction are generated by builtin function.  */

/* { dg-do compile } */
/* { dg-require-effective-target lp64 } */
/* { dg-require-effective-target riscv_atomic } */
/* { dg-options "-std=gnu99" } */
/* { dg-final { scan-assembler "\tlr.d\t" } } */
/* { dg-final { scan-assembler "\tsc.d\t" } } */

#include <nds_intrinsic.h>

void
func (void)
{
  /* We want to perform atomic read-modify-write operations for variable rmw.  */
  unsigned int fail;
  signed long long data;
  signed long long rmw = 0x0000FFFF; /* The initial value of rmw  */
  fail = 1;
  while (fail)
    {
      data = __nds__lrd (&rmw, UNORDER); /* read  */
      data = data + 1;
      fail = __nds__scd (data, &rmw, UNORDER); /* write  */
    }
}
