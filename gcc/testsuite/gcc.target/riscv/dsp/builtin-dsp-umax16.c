/* This is a test program for umax16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long umax16 (unsigned long ra, unsigned long rb)
{
  return __nds__umax16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_umax16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds__v_umax16 (ra, rb);
}

int
main ()
{
  unsigned long a = umax16 (0xfffe0001, 0xffff0000);
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
