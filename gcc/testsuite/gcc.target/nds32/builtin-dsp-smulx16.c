/* This is a test program for smulx16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned long long smulx16 (unsigned int ra, unsigned int rb)
{
  return __nds32__smulx16 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_smulx16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_smulx16 (ra, rb);
}

int
main ()
{
  unsigned long long a = smulx16 (0xffff0000, 0xffff0001);
  int32x2_t va = v_smulx16 ((int16x2_t) {0xffff, 0xffff},
			    (int16x2_t) {1, 0});
  if (a != 0xffffffff00000000)
    abort ();
  else if (va[0] != 0
           || va[1] != 0xffffffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
