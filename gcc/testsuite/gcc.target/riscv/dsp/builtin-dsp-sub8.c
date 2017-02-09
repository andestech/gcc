/* This is a test program for sub8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sub8 (unsigned long ra, unsigned long rb)
{
  return __nds__sub8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_usub8 (uint8x4_t ra, uint8x4_t rb)
{
  return __nds__v_usub8 (ra, rb);
}

static __attribute__ ((noinline))
int8x4_t v_ssub8 (int8x4_t ra, int8x4_t rb)
{
  return __nds__v_ssub8 (ra, rb);
}

int
main ()
{
  unsigned long a = sub8 (0x55667788, 0x11223344);
  uint8x4_t v_ua = v_usub8 ((uint8x4_t) {0xff, 0xee, 0xee, 0xcc},
			    (uint8x4_t) {0x1, 0xee, 0xdd, 0xdd});
  int8x4_t v_sa = v_ssub8 ((int8x4_t) {0x81, 0x0, 0xdd, 0xaa},
			   (int8x4_t) {0x80, 0x1, 0xcc, 0xaa});

  if (a != 0x44444444)
    abort ();
  else if (v_ua[0] != 0xfe
	   || v_ua[1] != 0
	   || v_ua[2] != 0x11
	   || v_ua[3] != 0xef)
    abort ();
  else if (v_sa[0] != 1
	   || v_sa[1] != (signed char) 0xff
	   || v_sa[2] != 0x11
	   || v_sa[3] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
