/* This is a test program for smin16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int smin16 (unsigned int ra, unsigned int rb)
{
  return __nds32__smin16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_smin16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_smin16 (ra, rb);
}

int
main ()
{
  unsigned int a = smin16 (0xfffe0001, 0xffff0000);
  int16x2_t v_sa = v_smin16 ((int16x2_t) {0x7fff, 0},
			     (int16x2_t) {0x7ffe, 1});
  if (a != 0xfffe0000)
    abort ();
  else if (v_sa[0] != 0x7ffe
           || v_sa[1] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
