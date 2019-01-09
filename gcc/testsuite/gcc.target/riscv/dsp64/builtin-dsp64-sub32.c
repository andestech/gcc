/* This is a test program for sub32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sub32 (unsigned long ra, unsigned long rb)
{
  return __nds__sub32 (ra, rb);
}

static __attribute__ ((noinline))
uint32x2_t v_usub32 (uint32x2_t ra, uint32x2_t rb)
{
  return __nds__v_usub32 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_ssub32 (int32x2_t ra, int32x2_t rb)
{
  return __nds__v_ssub32 (ra, rb);
}

int
main ()
{
  uint32x2_t ua_p = {0xe000d000, 0xe000d000};
  int32x2_t sa_p = {0xe777d111, 0xe777d111};
  unsigned long a = sub32 (0x0001f0000001f000, 0x0001100000011000);
  uint32x2_t v_ua = v_usub32 ((uint32x2_t) {0xf000f000, 0xf000f000},
			      (uint32x2_t) {0x10002000, 0x10002000});
  int32x2_t v_sa = v_ssub32 ((int32x2_t) {0xf777f111, 0xf777f111},
			     (int32x2_t) {0x10002000, 0x10002000});

  if (a != 0xe0000000e000)
    abort ();
  else if (ua_p[0] != v_ua[0]
	   || ua_p[1] != v_ua[1])
    abort ();
  else if (sa_p[0] != v_sa[0]
	   || sa_p[1] != v_sa[1])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
