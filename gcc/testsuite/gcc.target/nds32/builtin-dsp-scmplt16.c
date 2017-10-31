/* This is a test program for scmplt16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int scmplt16 (unsigned int ra, unsigned int rb)
{
  return __nds32__scmplt16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_scmplt16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_scmplt16 (ra, rb);
}

int
main ()
{
  unsigned int a = scmplt16 (0xfffe0001, 0xffff0000);
  uint16x2_t va = v_scmplt16 ((int16x2_t) {0x7fff, 0x7ffe},
			      (int16x2_t) {0x7ffe, 0x7fff});

  if (a != 0xffff0000)
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
