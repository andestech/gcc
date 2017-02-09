/* This is a test program for pktb16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long pktb16 (unsigned long ra, unsigned long rb)
{
  return __nds__pktb16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_pktb16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_pktb16 (ra, rb);
}

int
main ()
{
  uint16x4_t va_p = {0xcccc, 0xbbbb, 0xcccc, 0xbbbb};
  unsigned long a = pktb16 (0x1122334411223344, 0x5566778855667788);
  uint16x4_t va = v_pktb16 ((uint16x4_t) {0xaaaa, 0xbbbb, 0xaaaa, 0xbbbb},
			    (uint16x4_t) {0xcccc, 0xdddd, 0xcccc, 0xdddd});

  if (a != 0x1122778811227788)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
