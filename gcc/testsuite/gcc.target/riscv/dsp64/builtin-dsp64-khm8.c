/* This is a test program for khm8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long khm8 (unsigned long ra, unsigned long rb)
{
  return __nds__khm8 (ra, rb);
}

static __attribute__ ((noinline))
int8x8_t v_khm8 (int8x8_t ra, int8x8_t rb)
{
  return __nds__v_khm8 (ra, rb);
}

int
main ()
{
  int8x8_t v_sa_p = {0x7f, 0x00, 0xc0, 0x0, 0x7f, 0x00, 0xc0, 0x0};
  unsigned long a = khm8 (0x7fff7fff7fff7fff, 0x7fff7fff7fff7fff);
  int8x8_t va = v_khm8 ((int8x8_t) {0x80, 0x5, 0x40, 0x0, 0x80, 0x5, 0x40, 0x0},
			(int8x8_t) {0x80, 0x0, 0x80, 0x5, 0x80, 0x0, 0x80, 0x5});

  if (a != 0x7e007e007e007e00)
    abort ();
  else if (!vec64_all_eq (s, 8, v_sa_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
