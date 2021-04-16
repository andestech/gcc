/* This is a test program for smmwt instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long smmwt (long ra, unsigned long rb)
{
  return __nds__smmwt (ra, rb);
}

static __attribute__ ((noinline))
long v_smmwt (long ra, int16x2_t rb)
{
  return __nds__v_smmwt (ra, rb);
}

int
main ()
{
  int va_p = 0xffffffff;

  long a = smmwt (0x80000002, 0x80000001);

  long va = v_smmwt (0xffff0002, (int16x2_t) {0xffff, 0x0001});

  if (a != 0x3fffffff)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
