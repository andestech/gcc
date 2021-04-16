/* This is a test program for sll16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sll16 (unsigned long ra, unsigned long rb)
{
  return __nds__sll16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_sll16 (uint16x4_t ra, unsigned long rb)
{
  return __nds__v_sll16 (ra, rb);
}

int
main ()
{
  uint16x4_t va_p = {0xfff0, 0, 0xfff0, 0};
  unsigned long a = sll16 (0x0f00f0000f00f000, 4);
  uint16x4_t va = v_sll16 ((uint16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000}, 4);

  if (a != 0xf0000000f0000000)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
