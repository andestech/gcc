/* This is a test program for add16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long add16 (unsigned long ra, unsigned long rb)
{
  return __nds__add16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_uadd16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_uadd16 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_sadd16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_sadd16 (ra, rb);
}

int
main ()
{
  unsigned long a = add16 (0x0001f0000001f000, 0x0001100000011000);
  uint16x4_t v_ua = v_uadd16 ((uint16x4_t) {0xf000, 0xf000, 0xf000, 0xf000},
			      (uint16x4_t) {0x1000, 0x2000, 0x1000, 0x2000});
  int16x4_t v_sa = v_sadd16 ((int16x4_t) {0xf777, 0xf111, 0xf777, 0xf111},
			     (int16x4_t) {0x1000, 0x2000, 0x1000, 0x2000});

  if (a != 0x0002000000020000)
    abort ();
  else if (v_ua[0] != 0x0000
	   || v_ua[1] != 0x1000
	   || v_ua[2] != 0x0000
	   || v_ua[3] != 0x1000)
    abort ();
  else if (v_sa[0] != 0x0777
	   || v_sa[1] != 0x1111
	   || v_sa[2] != 0x0777
	   || v_sa[3] != 0x1111)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
