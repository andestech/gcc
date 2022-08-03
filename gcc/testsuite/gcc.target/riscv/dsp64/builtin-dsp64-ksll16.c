/* This is a test program for ksll16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ksll16 (unsigned long ra, unsigned long rb)
{
  return __nds__ksll16 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_ksll16 (int16x4_t ra, unsigned long rb)
{
  return __nds__v_ksll16 (ra, rb);
}

int
main ()
{
  int16x4_t va_p = {0x7fff, 0x8000, 0x7fff, 0x8000};
  unsigned long a = ksll16 (0x0f00f0000f00f000, 4);
  int16x4_t va = v_ksll16 ((int16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000}, 4);

  if (a != 0x7fff80007fff8000)
    abort ();
  else if (!vec64_all_eq (s, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
