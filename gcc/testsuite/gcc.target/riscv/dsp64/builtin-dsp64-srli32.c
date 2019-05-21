/* This is a test program for srli32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long srli32 (unsigned long ra)
{
  return __nds__srl32 (ra, 4);
}

static __attribute__ ((noinline))
uint32x2_t v_srli32 (uint32x2_t ra)
{
  return __nds__v_srl32 (ra, 4);
}

int
main ()
{
  uint32x2_t va_p = {0x07fff800, 0x07fff800};
  unsigned long a = srli32 (0x0ffff0000ffff000);
  uint32x2_t va = v_srli32 ((uint32x2_t) {0x7fff8000, 0x7fff8000});

  if (a != 0xffff0000ffff00)
    abort ();
  else if (va[0] != va_p[0]
	   || va[1] != va_p[1])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
