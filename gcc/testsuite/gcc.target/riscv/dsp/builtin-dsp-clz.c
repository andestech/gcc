/* This is a test program for clz instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
int
main ()
{
  int c = 0x0000FFFF;
  c =  __nds__clz (c);

  if (c != 16)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
