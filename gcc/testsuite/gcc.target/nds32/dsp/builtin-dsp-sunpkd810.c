/* This is a test program for sunpkd810 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int sunpkd810 (unsigned int a)
{
  return __nds32__sunpkd810 (a);
}

static __attribute__ ((noinline))
int16x2_t v_sunpkd810 (int8x4_t a)
{
  return __nds32__v_sunpkd810 (a);
}

int
main ()
{
#ifdef __NDS32_EL__
  int16x2_t va_p = {0xfff8, 0x56};
#else
  int16x2_t va_p = {0, 0};
#endif

  unsigned int a = sunpkd810 (0x000056f8);
  int16x2_t va = v_sunpkd810 ((int8x4_t) {0xf8, 0x56, 0, 0});

  if (a != 0x0056fff8)
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
