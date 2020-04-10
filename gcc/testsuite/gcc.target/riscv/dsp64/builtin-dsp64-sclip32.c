/* This is a test program for add8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
signed long sclip32 (signed long ra)
{
  return __nds__sclip32 (ra, 5);
}

int
main ()
{
  signed long a = sclip32 (0x0000000100000100);

  if (a != 0x10000001f)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
