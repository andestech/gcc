/* This is a test program for kslra32.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long kslra32u (unsigned long ra, unsigned long rb)
{
  return __nds__kslra32_u (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_kslra32u (int32x2_t ra, unsigned long rb)
{
  return __nds64__v_kslra32_u (ra, rb);
}

int
main ()
{
  int32x2_t va_p = {0xf8000000, 0xf8000000};
  unsigned long a = kslra32u (0x0f00f0000f00f000, 12);
  int32x2_t va = v_kslra32u ((int32x2_t) {0x7fff8000, 0x7fff8000}, 12);

  if (a != 0xf0000000f000000)
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
