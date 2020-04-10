/* This is a test program for zunpkd820 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long zunpkd820 (unsigned long a)
{
  return __nds__zunpkd820 (a);
}

static __attribute__ ((noinline))
uint16x4_t v_zunpkd820 (uint8x8_t a)
{
  return __nds__v_zunpkd820 (a);
}

int
main ()
{
  uint16x4_t va_p = {0xf8, 0x34, 0xf8, 0x34};
  unsigned long a = zunpkd820 (0x003400f8003400f8);
  uint16x4_t va = v_zunpkd820 ((uint8x8_t) {0xf8, 0, 0x34, 0, 0xf8, 0, 0x34, 0});

  if (a != 0x003400f8003400f8)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
