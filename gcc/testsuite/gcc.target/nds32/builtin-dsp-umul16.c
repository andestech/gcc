/* This is a test program for umul16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned long long umul16 (unsigned int ra, unsigned int rb)
{
  return __nds32__umul16 (ra, rb);
}

static __attribute__ ((noinline))
uint32x2_t v_umul16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_umul16 (ra, rb);
}

int
main ()
{
  unsigned long long a = umul16 (0xffff0000, 0x0001ffff);
  uint32x2_t va = v_umul16 ((uint16x2_t) {0xffff, 0},
			    (uint16x2_t) {0x0001, 0xffff});
  if (a != 0xffff00000000)
    abort ();
  else if (va[0] != 0xffff
           || va[1] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
