/* This is a test program for add16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ukadd16 (unsigned long ra, unsigned long rb)
{
  return __nds__ukadd16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_ukadd16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_ukadd16 (ra, rb);
}

int
main ()
{
  unsigned long a = ukadd16 (0x0001f0000001f000, 0x0001100000011000);
  uint16x4_t v_ua = v_ukadd16 ((uint16x4_t) {0xf000, 0xf000, 0xf000, 0xf000},
			       (uint16x4_t) {0x1000, 0x2000, 0x1000, 0x2000});

  if (a != 0x0002ffff0002ffff)
    abort ();
  else if (v_ua[0] != 0xffff
	   || v_ua[1] != 0xffff
	   || v_ua[2] != 0xffff
	   || v_ua[3] != 0xffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
