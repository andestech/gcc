/* This is a test program for wsbh instruction.  */
/* { dg-do run } */
/* { dg-options "-O0" } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
int
main ()
{
  unsigned int a = 0x03020100;
  unsigned int b;

  b = __nds__swap8 (a);

  if (b != 0x02030001)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
