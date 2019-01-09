/* This is a test program for zunpkd832 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long zunpkd832 (unsigned long a)
{
  return __nds__zunpkd832 (a);
}

static __attribute__ ((noinline))
uint16x4_t v_zunpkd832 (uint8x8_t a)
{
  return __nds__v_zunpkd832 (a);
}

int
main ()
{
  uint16x4_t va_p = {0xf8, 0x12, 0xf8, 0x12};
  unsigned long a = zunpkd832 (0x12f8000012f80000);
  uint16x4_t va = v_zunpkd832 ((uint8x8_t) {0, 0, 0xf8, 0x12, 0, 0, 0xf8, 0x12});

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
