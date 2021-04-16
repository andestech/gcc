/* This is a test program for pkbb16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long pkbb16 (unsigned long ra, unsigned long rb)
{
  return __nds__pkbb16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_pkbb16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_pkbb16 (ra, rb);
}

int
main ()
{
  uint16x4_t va_p = {0xcccc, 0xaaaa, 0xcccc, 0xaaaa};
  unsigned long a = pkbb16 (0x1122334411223344, 0x5566778855667788);
  uint16x4_t va = v_pkbb16 ((uint16x4_t) {0xaaaa, 0xbbbb, 0xaaaa, 0xbbbb},
			    (uint16x4_t) {0xcccc, 0xdddd, 0xcccc, 0xdddd});

  if (a != 0x3344778833447788)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
