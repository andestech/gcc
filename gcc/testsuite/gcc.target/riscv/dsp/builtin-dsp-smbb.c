/* This is a test program for smbb instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long smbb (unsigned long ra, unsigned long rb)
{
  return __nds__smbb16 (ra, rb);
}

static __attribute__ ((noinline))
int v_smbb (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_smbb16 (ra, rb);
}

int
main ()
{
  long va_p = 1;

  long a = smbb (0x80000002, 0x80000001);

  long va = v_smbb ((int16x2_t) {0xffff, 0x0002},
		   (int16x2_t) {0xffff, 0x0001});

  if (a != 2)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
