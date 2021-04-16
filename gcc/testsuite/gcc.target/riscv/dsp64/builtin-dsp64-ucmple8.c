/* This is a test program for ucmple8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ucmple8 (unsigned long ra, unsigned long rb)
{
  return __nds__ucmple8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x8_t v_ucmple8 (uint8x8_t ra, uint8x8_t rb)
{
  return __nds__v_ucmple8 (ra, rb);
}

int
main ()
{
  uint8x8_t va_p = {0xff, 0xff, 0, 0, 0xff, 0xff, 0, 0};
  unsigned long a = ucmple8 (0xfefe0101fefe0101, 0xffff0000ffff0000);
  uint8x8_t va = v_ucmple8 ((uint8x8_t) {0x7e, 0x7e, 0x01, 0x01, 0x7e, 0x7e, 0x01, 0x01},
			    (uint8x8_t) {0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x00, 0x00});

  if (a != 0xffff0000ffff0000)
    abort ();
  else if (!vec64_all_eq (u, 8, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
