/* This is a test program for sub8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ksub8 (unsigned long ra, unsigned long rb)
{
  return __nds__ksub8 (ra, rb);
}

static __attribute__ ((noinline))
int8x8_t v_ksub8 (int8x8_t ra, int8x8_t rb)
{
  return __nds__v_ksub8 (ra, rb);
}

int
main ()
{
  unsigned long a = ksub8 (0x5566778855667788, 0x1122334411223344);
  int8x8_t v_sa = v_ksub8 ((int8x8_t) {0x81, 0x0, 0xdd, 0xaa, 0x81, 0x0, 0xdd, 0xaa},
			   (int8x8_t) {0x80, 0x1, 0xcc, 0xaa, 0x80, 0x1, 0xcc, 0xaa});

  if (a != 0x4444448044444480)
    abort ();
  else if (v_sa[0] != 1
	   || v_sa[1] != (signed char) 0xff
	   || v_sa[2] != 0x11
	   || v_sa[3] != 0
	   || v_sa[4] != 1
	   || v_sa[5] != (signed char) 0xff
	   || v_sa[6] != 0x11
	   || v_sa[7] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
