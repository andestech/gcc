/* This is a test program for kdmbb instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long kdmbb (int ra, int rb)
{
  return __nds__kdmbb (ra, rb);
}

static __attribute__ ((noinline))
long v_kdmbb (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_kdmbb (ra, rb);
}

int
main ()
{
  long a = kdmbb (0x0001f000, 0x00011000);
  long v_sa = v_kdmbb ((int16x2_t) {0xf777, 0xf111},
		       (int16x2_t) {0x1000, 0x2000});
  if (a != 0xfffffffffe000000)
    abort ();
  else if (v_sa != 0xfffffffffeeee000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
