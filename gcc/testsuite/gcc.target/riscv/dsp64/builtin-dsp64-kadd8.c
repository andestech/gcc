/* This is a test program for add8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long kadd8 (unsigned long ra, unsigned long rb)
{
  return __nds__kadd8 (ra, rb);
}

static __attribute__ ((noinline))
int8x8_t v_kadd8 (int8x8_t ra, int8x8_t rb)
{
  return __nds__v_kadd8 (ra, rb);
}

int
main ()
{
  unsigned long a = kadd8 (0x1122334411223344, 0x5566778855667788);
  int8x8_t v_sa = v_kadd8 ((int8x8_t) {0x80, 0x7f, 0xbb, 0xaa, 0x80, 0x7f, 0xbb, 0xaa},
			   (int8x8_t) {0x80, 0x7f, 0xbb, 0xaa, 0x80, 0x7f, 0xbb, 0xaa});

  if (a != 0x667f7fcc667f7fcc)
    abort ();
  else if (v_sa[0] != (signed char) 0x80
	   || v_sa[1] != 0x7f
	   || v_sa[2] != (signed char) 0x80
	   || v_sa[3] != (signed char) 0x80
	   || v_sa[4] != (signed char) 0x80
	   || v_sa[5] != 0x7f
	   || v_sa[6] != (signed char) 0x80
	   || v_sa[7] != (signed char) 0x80)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
