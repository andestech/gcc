/* This is a test program for sra32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sra32 (unsigned long ra, unsigned long rb)
{
  return __nds__sra32 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_sra32 (int32x2_t ra, unsigned long rb)
{
  return __nds__v_sra32 (ra, rb);
}

int
main ()
{
  unsigned long a = sra32 (0x0ffff0000ffff000, 4);
  int32x2_t va = v_sra32 ((int32x2_t) {0x7fff8000, 0x7fff8000}, 4);

  if (a != 0xffff0000ffff00)
    abort ();
  else if (va[0] != 0x07fff800
	   || va[1] != 0x7fff800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
