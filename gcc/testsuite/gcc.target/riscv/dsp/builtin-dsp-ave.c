/* This is a test program for ave instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
int
main ()
{
  int a = 4;
  int b = 2;
  int ave = __nds__ave (a, b);

  if (ave != 3)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
