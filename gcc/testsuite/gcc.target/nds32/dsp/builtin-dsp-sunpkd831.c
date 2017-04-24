/* This is a test program for sunpkd831 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int sunpkd831 (unsigned int a)
{
  return __nds32__sunpkd831 (a);
}

static __attribute__ ((noinline))
int16x2_t v_sunpkd831 (int8x4_t a)
{
  return __nds32__v_sunpkd831 (a);
}

int
main ()
{
#ifdef __NDS32_EL__
  int16x2_t va_p = {0xfff8, 0x12};
#else
  int16x2_t va_p = {0, 0};
#endif

  unsigned int a = sunpkd831 (0x1200f800);
  int16x2_t va = v_sunpkd831 ((int8x4_t) {0, 0xf8, 0, 0x12});

  if (a != 0x0012fff8)
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
