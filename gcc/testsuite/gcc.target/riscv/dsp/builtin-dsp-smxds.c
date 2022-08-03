/* This is a test program for smxds instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long smxds (unsigned long ra, unsigned long rb)
{
  return __nds__smxds (ra, rb);
}

static __attribute__ ((noinline))
long v_smxds (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_smxds (ra, rb);
}

int
main ()
{
  long a_p = 0x8000;
  long va_p = 0xffffffff;
  long a = smxds (0x80000002, 0x80000001);
  long va = v_smxds ((int16x2_t) {0xffff, 0x0002},
		     (int16x2_t) {0xffff, 0x0001});

  if (a != a_p)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
