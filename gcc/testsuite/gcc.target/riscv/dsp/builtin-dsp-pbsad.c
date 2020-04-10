/* This is a test program for pbsad instruction.  */
/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
int
main ()
{
  unsigned int a = 0x09070605;
  unsigned int b = 0x04020301;
  unsigned int r = __nds__pbsad (a, b);

  if (r != 17)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
