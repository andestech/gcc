/* This is a test program for crsa16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long kcrsa16 (unsigned long ra, unsigned long rb)
{
  return __nds__kcrsa16 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_kcrsa16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_kcrsa16 (ra, rb);
}

int
main ()
{
  int16x4_t v_sa_p = {0x3000, 0x110, 0x3000, 0x110};
  unsigned long a = kcrsa16 (0x0001f0000001f000, 0x0001f0000001f000);
  int16x4_t v_sa = v_kcrsa16 ((int16x4_t) {0x2000, 0x0111, 0x2000, 0x0111},
			      (int16x4_t) {0x0001, 0x1000, 0x0001, 0x1000});

  if (a != 0x1001f0011001f001)
    abort ();
  else if (v_sa[0] != v_sa_p[0]
	   || v_sa[1] != v_sa_p[1]
	   || v_sa[2] != v_sa_p[2]
	   || v_sa[3] != v_sa_p[3])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
