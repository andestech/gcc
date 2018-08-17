/* This is a test program for ksll32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long ksll32 (unsigned long ra, unsigned long rb)
{
  return __nds__ksll32 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_ksll32 (int32x2_t ra, unsigned long rb)
{
  return __nds64__v_ksll32 (ra, rb);
}

int
main ()
{
  int32x2_t va_p = {0xfff80000, 0xfff80000};
  unsigned long a = ksll32 (0x0f00f0000f00f000, 4);
  int32x2_t va = v_ksll32 ((int32x2_t) {0x7fff8000, 0x7fff8000}, 4);

  if (a != 0xf00f0000f00f0000)
    abort ();
  else if (va[0] != va_p[0]
	   || va[1] != va_p[1])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
