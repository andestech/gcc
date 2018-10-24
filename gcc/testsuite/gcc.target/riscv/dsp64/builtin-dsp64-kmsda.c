/* This is a test program for kmsda instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long kmsda (unsigned long rd, unsigned long ra, unsigned long rb)
{
  return __nds__kmsda (rd, ra, rb);
}

static __attribute__ ((noinline))
long v_kmsda (unsigned long rd, int16x4_t ra, int16x4_t rb)
{
  return __nds__v_kmsda (rd, ra, rb);
}

int
main ()
{
  long va_p = 0;
  long a = kmsda (0x300000003, 0x8000000280000002, 0x8000000180000001);
  long va = v_kmsda (0x300000003,
		     (int16x4_t) {0xffff, 0x0002, 0xffff, 0x0002},
		     (int16x4_t) {0xffff, 0x0001, 0xffff, 0x0001});

  if (a != 0xc0000001c0000001)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
