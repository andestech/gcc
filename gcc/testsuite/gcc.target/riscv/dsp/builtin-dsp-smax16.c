/* This is a test program for smax16 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long smax16 (unsigned long ra, unsigned long rb)
{
  return __nds__smax16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_smax16 (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_smax16 (ra, rb);
}

int
main ()
{
  unsigned long a = smax16 (0xfffe0001, 0xffff0000);
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
