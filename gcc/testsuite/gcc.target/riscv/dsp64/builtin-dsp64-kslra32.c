/* This is a test program for kslra32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long kslra32 (unsigned long ra, unsigned long rb)
{
  return __nds__kslra32 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_kslra32 (int32x2_t ra, unsigned long rb)
{
  return __nds__v_kslra32 (ra, rb);
}

int
main ()
{
  int32x2_t va_p = {0x7fffffff, 0x7fffffff};
  unsigned long a = kslra32 (0x0f00f0000f00f000, 12);
  int32x2_t va = v_kslra32 ((int32x2_t) {0x7fff8000, 0x7fff8000}, 12);

  if (a != 0x7fffffff7fffffff)
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
