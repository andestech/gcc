/* This is a test program for srli16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long srli16 (unsigned long ra)
{
  return __nds__srl16 (ra, 4);
}

static __attribute__ ((noinline))
uint16x4_t v_srli16 (uint16x4_t ra)
{
  return __nds__v_srl16 (ra, 4);
}

int
main ()
{
  uint16x4_t va_p = {0x7ff, 0x0800, 0x7ff, 0x0800};
  unsigned long a = srli16 (0x0f00f0000f00f000);
  uint16x4_t va = v_srli16 ((uint16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000});

  if (a != 0xf00f0000f00f00)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
