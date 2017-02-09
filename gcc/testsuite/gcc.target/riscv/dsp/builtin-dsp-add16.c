/* This is a test program for add16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long add16 (unsigned long ra, unsigned long rb)
{
  return __nds__add16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_uadd16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds__v_uadd16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_sadd16 (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_sadd16 (ra, rb);
}

int
main ()
{
  unsigned long a = add16 (0x0001f000, 0x00011000);
  uint16x2_t v_ua = v_uadd16 ((uint16x2_t) {0xf000, 0xf000},
			      (uint16x2_t) {0x1000, 0x2000});
  int16x2_t v_sa = v_sadd16 ((int16x2_t) {0xf777, 0xf111},
			     (int16x2_t) {0x1000, 0x2000});

  if (a != 0x00020000)
    abort ();
  else if (v_ua[0] != 0x0000
	   || v_ua[1] != 0x1000)
    abort ();
  else if (v_sa[0] != 0x0777
	   || v_sa[1] != 0x1111)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
