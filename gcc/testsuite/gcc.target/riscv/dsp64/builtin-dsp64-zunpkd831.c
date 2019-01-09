/* This is a test program for zunpkd831 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long zunpkd831 (unsigned long a)
{
  return __nds__zunpkd831 (a);
}

static __attribute__ ((noinline))
uint16x4_t v_zunpkd831 (uint8x8_t a)
{
  return __nds__v_zunpkd831 (a);
}

int
main ()
{
  uint16x4_t va_p = {0xf8, 0x12, 0xf8, 0x12};
  unsigned long a = zunpkd831 (0x1200f8001200f800);
  uint16x4_t va = v_zunpkd831 ((uint8x8_t) {0, 0xf8, 0, 0x12, 0, 0xf8, 0, 0x12});

  if (a != 0x001200f8001200f8)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
