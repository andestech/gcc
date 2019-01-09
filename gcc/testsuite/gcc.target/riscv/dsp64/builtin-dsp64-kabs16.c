/* This is a test program for kabs16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long kabs16 (unsigned long ra)
{
  return __nds__kabs16 (ra);
}

static __attribute__ ((noinline))
int16x4_t v_kabs16 (int16x4_t ra)
{
  return __nds__v_kabs16 (ra);
}

int
main ()
{
  int16x4_t v_sa_p = {0x7fff, 0x4000, 0x7fff, 0x4000};
  unsigned long a = kabs16 (0x7fff7fff7fff7fff);
  int16x4_t va = v_kabs16 ((int16x4_t) {0x8000, 0x4000, 0x8000, 0x4000});

  if (a != 0x7fff7fff7fff7fff)
    abort ();
  else if (!vec64_all_eq (s, 16, v_sa_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
