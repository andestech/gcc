/* This is a test program for srai16.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long srai16u (unsigned long ra)
{
  return __nds__sra16_u (ra, 4);
}

static __attribute__ ((noinline))
int16x4_t v_srai16u (int16x4_t ra)
{
  return __nds__v_sra16_u (ra, 4);
}

int
main ()
{
  int16x4_t va_p = {0x800, 0xf800, 0x800, 0xf800};
  unsigned long a = srai16u (0x0ffff0000ffff000);
  int16x4_t va = v_srai16u ((int16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000});

  if (a != 0x100ff000100ff00)
    abort ();
  else if (!vec64_all_eq (s, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
