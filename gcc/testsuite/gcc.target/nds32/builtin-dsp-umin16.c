/* This is a test program for umin16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int umin16 (unsigned int ra, unsigned int rb)
{
  return __nds32__umin16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_umin16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_umin16 (ra, rb);
}

int
main ()
{
  unsigned int a = umin16 (0xfffe0001, 0xffff0000);
  uint16x2_t va = v_umin16 ((uint16x2_t) {0x7fff, 0},
			    (uint16x2_t) {0x7ffe, 1});
  if (a != 0xfffe0000)
    abort ();
  else if (va[0] != 0x7ffe
           || va[1] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
