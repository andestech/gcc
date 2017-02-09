/* This is a test program for sunpkd810 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sunpkd810 (unsigned long a)
{
  return __nds__sunpkd810 (a);
}

static __attribute__ ((noinline))
int16x4_t v_sunpkd810 (int8x8_t a)
{
  return __nds__v_sunpkd810 (a);
}

int
main ()
{
  int16x4_t va_p = {0xfff8, 0x56, 0xfff8, 0x56};
  unsigned long a = sunpkd810 (0x000056f8000056f8);
  int16x4_t va = v_sunpkd810 ((int8x8_t) {0xf8, 0x56, 0, 0, 0xf8, 0x56, 0, 0});

  if (a != 0x0056fff80056fff8)
    abort ();
  else if (!vec64_all_eq (s, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
