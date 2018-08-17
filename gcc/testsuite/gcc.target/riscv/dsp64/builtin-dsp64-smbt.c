/* This is a test program for smbt instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long smbt (unsigned long ra, unsigned long rb)
{
  return __nds__smbt (ra, rb);
}

static __attribute__ ((noinline))
long v_smbt (int16x4_t ra, int16x4_t rb)
{
  return __nds64__v_smbt (ra, rb);
}

int
main ()
{
  long va_p = 0xffffffffffffffff;
  long a = smbt (0x8000000280000002, 0x8000000180000001);
  long va = v_smbt ((int16x4_t) {0xffff, 0x0002, 0xffff, 0x0002},
		   (int16x4_t) {0xffff, 0x0001, 0xffff, 0x0001});

  if (a != 0xffff0000ffff0000)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
