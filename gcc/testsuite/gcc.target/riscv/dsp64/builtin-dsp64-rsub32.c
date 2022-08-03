/* This is a test program for rsub32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long rsub32 (unsigned long ra, unsigned long rb)
{
  return __nds__rsub32 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_rsub32 (int32x2_t ra, int32x2_t rb)
{
  return __nds__v_rsub32 (ra, rb);
}

int
main ()
{
  int32x2_t sa_p = {0xf3bbe888, 0xf3bbe888};
  unsigned long a = rsub32 (0x0001f0000001f000, 0x0001100000011000);
  int32x2_t v_sa = v_rsub32 ((int32x2_t) {0xf777f111, 0xf777f111},
			     (int32x2_t) {0x10002000, 0x10002000});

  if (a != 0x700000007000)
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
