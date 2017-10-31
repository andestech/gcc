/* This is a test program for umax16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int umax16 (unsigned int ra, unsigned int rb)
{
  return __nds32__umax16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_umax16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_umax16 (ra, rb);
}

int
main ()
{
  unsigned int a = umax16 (0xfffe0001, 0xffff0000);
  uint16x2_t va = v_umax16 ((uint16x2_t) {0xffff, 0},
			    (uint16x2_t) {0xfffe, 1});
  if (a != 0xffff0001)
    abort ();
  else if (va[0] != 0xffff
           || va[1] != 1)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
