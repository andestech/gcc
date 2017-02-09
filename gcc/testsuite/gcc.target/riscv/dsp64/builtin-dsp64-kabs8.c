/* This is a test program for kabs8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long kabs8 (unsigned long ra)
{
  return __nds__kabs8 (ra);
}

static __attribute__ ((noinline))
int8x8_t v_kabs8 (int8x8_t ra)
{
  return __nds__v_kabs8 (ra);
}

int
main ()
{
  int8x8_t v_sa_p = {0x7f, 0x0, 0x40, 0x0, 0x7f, 0x0, 0x40, 0x0};
  unsigned long a = kabs8 (0x7fff7fff7fff7fff);
  int8x8_t va = v_kabs8 ((int8x8_t) {0x80, 0x0, 0x40, 0x0, 0x80, 0x0, 0x40, 0x0});

  if (a != 0x7f017f017f017f01)
    abort ();
  else if (!vec64_all_eq (s, 8, v_sa_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
