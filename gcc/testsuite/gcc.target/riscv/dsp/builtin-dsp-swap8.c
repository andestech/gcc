/* This is a test program for wsbh instruction.  */

/* { dg-do run } */
/* { dg-options "-O0" } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
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
