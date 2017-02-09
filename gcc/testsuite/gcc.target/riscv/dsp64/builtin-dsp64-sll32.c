/* This is a test program for sll32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sll32 (unsigned long ra, unsigned long rb)
{
  return __nds__sll32 (ra, rb);
}

static __attribute__ ((noinline))
uint32x2_t v_sll32 (uint32x2_t ra, unsigned long rb)
{
  return __nds__v_sll32 (ra, rb);
}

int
main ()
{
  unsigned long a = sll32 (0x0ffff0000ffff000, 4);
  uint32x2_t va = v_sll32 ((uint32x2_t) {0x7fff8000, 0x7fff8000}, 4);

  if (a != 0xffff0000ffff0000)
    abort ();
  else if (va[0] != 0xfff80000
	   || va[1] != 0xfff80000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
