/* This is a test program for urcras16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long urcras16 (unsigned long ra, unsigned long rb)
{
  return __nds__urcras16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_urcras16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds__v_urcras16 (ra, rb);
}

int
main ()
{
  uint16x2_t va_p = {0xffff, 0x8000};
  unsigned long a = urcras16 (0x7fff7fff, 0x80007fff);
  uint16x2_t va = v_urcras16 ((uint16x2_t) {0x7fff, 0x8000},
			      (uint16x2_t) {0x8000, 0x8000});

  if (a != 0x7fffffff)
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
