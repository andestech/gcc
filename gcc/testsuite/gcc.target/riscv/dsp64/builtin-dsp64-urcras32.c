/* This is a test program for urcras32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long urcras32 (unsigned long ra, unsigned long rb)
{
  return __nds__urcras32 (ra, rb);
}

static __attribute__ ((noinline))
uint32x2_t v_urcras32 (uint32x2_t ra, uint32x2_t rb)
{
  return __nds64__v_urcras32 (ra, rb);
}

int
main ()
{
  uint32x2_t sa_p = {0x83bc0888, 0x73bbe888};
  unsigned long a = urcras32 (0x0001f0000001f000, 0x0001100000011000);
  uint32x2_t v_sa = v_urcras32 ((uint32x2_t) {0xf777f111, 0xf777f111},
				(uint32x2_t) {0x10002000, 0x10002000});

  if (a != 0x700000018000)
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
