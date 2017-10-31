/* This is a test program for smax16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int smax16 (unsigned int ra, unsigned int rb)
{
  return __nds32__smax16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_smax16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_smax16 (ra, rb);
}

int
main ()
{
  unsigned int a = smax16 (0xfffe0001, 0xffff0000);
  int16x2_t va = v_smax16 ((int16x2_t) {0x7fff, 0},
			   (int16x2_t) {0x7ffe, 1});
  if (a != 0xffff0001)
    abort ();
  else if (va[0] != 0x7fff
           || va[1] != 1)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
