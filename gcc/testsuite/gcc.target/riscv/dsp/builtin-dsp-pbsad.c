/* This is a test program for pbsad instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
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
