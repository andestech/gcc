/* This is a test program for smbb32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long smbb32 (unsigned long ra, unsigned long rb)
{
  return __nds__smbb32 (ra, rb);
}

static __attribute__ ((noinline))
long v_smbb32 (int32x2_t ra, int32x2_t rb)
{
  return __nds__v_smbb32 (ra, rb);
}

int
main ()
{
  long va_p = 0x00000000fffd0002;
  long a = smbb32 (0x8000000280000002, 0x8000000180000001);
  long va = v_smbb32 ((int32x2_t) {0xffff0002, 0xffff0002},
		      (int32x2_t) {0xffff0001, 0xffff0001});

  if (a != 0x3ffffffe80000002)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
