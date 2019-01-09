/* This is a test program for crsa16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ukcrsa16 (unsigned long ra, unsigned long rb)
{
  return __nds__ukcrsa16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_ukcrsa16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_ukcrsa16 (ra, rb);
}

int
main ()
{
  uint16x4_t v_ua_p = {0x2469, 0xe000, 0x2469, 0xe000};
  unsigned long a = ukcrsa16 (0x0001f0000001f000, 0x0001f0000001f000);
  uint16x4_t v_ua = v_ukcrsa16 ((uint16x4_t) {0x1235, 0xf000, 0x1235, 0xf000},
			        (uint16x4_t) {0x1000, 0x1234, 0x1000, 0x1234});

  if (a != 0xf0010000f001)
    abort ();
  else if (v_ua[0] != v_ua_p[0]
	   || v_ua[1] != v_ua_p[1]
	   || v_ua[2] != v_ua_p[2]
	   || v_ua[3] != v_ua_p[3])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
