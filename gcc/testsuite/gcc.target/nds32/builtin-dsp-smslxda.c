/* This is a test program for smslxda instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
long long smslxda (long long rt, unsigned int ra, unsigned int rb)
{
  return __nds32__smslxda (rt, ra, rb);
}

static __attribute__ ((noinline))
long long v_smslxda (long long rt, int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_smslxda (rt, ra, rb);
}

int
main ()
{
  long long a = smslxda (0xff0000000000ll, 0xffffffff, 0x2);
  long long va = v_smslxda (0x100000000ll,
			    (int16x2_t) {0xf000, 0}, (int16x2_t) {0, 3});

  if (a != 0xff0000000002ll)
    abort ();
  else if (va != 0x100003000ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
