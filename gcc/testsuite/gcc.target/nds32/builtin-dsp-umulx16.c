/* This is a test program for umulx16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned long long umulx16 (unsigned int ra, unsigned int rb)
{
  return __nds32__umulx16 (ra, rb);
}

static __attribute__ ((noinline))
uint32x2_t v_umulx16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_umulx16 (ra, rb);
}

int
main ()
{
  unsigned long long a = umulx16 (0xffff0000, 0xffff0001);
  uint32x2_t va = v_umulx16 ((uint16x2_t) {0xffff, 0xffff},
			     (uint16x2_t) {1, 0});
  if (a != 0xffff00000000)
    abort ();
  else if (va[0] != 0
           || va[1] != 0xffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
