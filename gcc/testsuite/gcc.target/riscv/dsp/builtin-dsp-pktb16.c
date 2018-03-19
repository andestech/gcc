/* This is a test program for pktb16 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long pktb16 (unsigned long ra, unsigned long rb)
{
  return __nds__pktb16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_pktb16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds__v_pktb16 (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  uint16x2_t va_p = {0xcccc, 0xbbbb};
#else
  uint16x2_t va_p = {0xaaaa, 0xdddd};
#endif

  unsigned long a = pktb16 (0x11223344, 0x55667788);
  uint16x2_t va = v_pktb16 ((uint16x2_t) {0xaaaa, 0xbbbb},
			    (uint16x2_t) {0xcccc, 0xdddd});

  if (a != 0x11227788)
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
