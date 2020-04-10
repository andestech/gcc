/* This is a test program for smslda instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smslda (long long rt, unsigned long ra, unsigned long rb)
{
  return __nds__smslda (rt, ra, rb);
}

static __attribute__ ((noinline))
long long v_smslda (long long rt, int16x2_t ra, int16x2_t rb)
{
  return __nds__v_smslda (rt, ra, rb);
}

int
main ()
{
  long long a = smslda (0xff0000000000ll, 0xffffffff, 0x2);
  long long va = v_smslda (0x100000000ll,
			   (int16x2_t) {0xf000, 0}, (int16x2_t) {0, 3});

  if (a != 0xff0000000002ll)
    abort ();
  else if (va != 0x100000000ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
