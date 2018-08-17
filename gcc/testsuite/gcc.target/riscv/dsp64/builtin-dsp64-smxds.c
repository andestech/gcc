/* This is a test program for smxds instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long smxds (unsigned long ra, unsigned long rb)
{
  return __nds__smxds (ra, rb);
}

static __attribute__ ((noinline))
long v_smxds (int16x4_t ra, int16x4_t rb)
{
  return __nds64__v_smxds (ra, rb);
}

int
main ()
{
  long va_p = 0xffffffffffffffff;
  long a = smxds (0x8000000280000002, 0x8000000180000001);
  long va = v_smxds ((int16x4_t) {0xffff, 0x0002, 0xffff, 0x0002},
		     (int16x4_t) {0xffff, 0x0001, 0xffff, 0x0001});

  if (a != 0x800000008000)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
