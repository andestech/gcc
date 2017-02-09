/* This is a test program for smtt instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long smtt (unsigned long ra, unsigned long rb)
{
  return __nds__smtt16 (ra, rb);
}

static __attribute__ ((noinline))
long v_smtt (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_smtt16 (ra, rb);
}

int
main ()
{
  long va_p = 2;

  long a = smtt (0x80000002, 0x80000001);

  long va = v_smtt ((int16x2_t) {0xffff, 0x0002},
		   (int16x2_t) {0xffff, 0x0001});

  if (a != 0x40000000)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
