/* This is a test program for clo instruction.  */
/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
int
main ()
{
  int c = 0xFFFF0000;
  c =  __nds__clo (c);

  if (c != 16)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
