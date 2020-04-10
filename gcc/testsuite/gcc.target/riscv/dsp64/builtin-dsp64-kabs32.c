/* This is a test program for kabs32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long kabs32 (unsigned long ra)
{
  return __nds__kabs32 (ra);
}

static __attribute__ ((noinline))
int32x2_t v_kabs32 (int32x2_t ra)
{
  return __nds__v_kabs32 (ra);
}

int
main ()
{
  int32x2_t va_p = {0x7fffc000, 0x7fffc000};
  unsigned long a = kabs32 (0x7fff7fff7fff7fff);
  int32x2_t va = v_kabs32 ((int32x2_t) {0x80004000, 0x80004000});

  if (a != 0x7fff7fff7fff7fff)
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
