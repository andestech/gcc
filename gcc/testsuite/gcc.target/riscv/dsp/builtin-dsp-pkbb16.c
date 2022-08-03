/* This is a test program for pkbb16 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long pkbb16 (unsigned long ra, unsigned long rb)
{
  return __nds__pkbb16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_pkbb16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds__v_pkbb16 (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  uint16x2_t va_p = {0xcccc, 0xaaaa};
#else
  uint16x2_t va_p = {0xbbbb, 0xdddd};
#endif

  unsigned long a = pkbb16 (0x11223344, 0x55667788);
  uint16x2_t va = v_pkbb16 ((uint16x2_t) {0xaaaa, 0xbbbb},
			    (uint16x2_t) {0xcccc, 0xdddd});

  if (a != 0x33447788)
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
