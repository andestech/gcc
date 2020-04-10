/* This is a test program for smalxds instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smalxds (long long t, unsigned long a, unsigned long b)
{
  return __nds__smalxds (t, a, b);
}

static __attribute__ ((noinline))
long long v_smalxds (long long t, int16x4_t a, int16x4_t b)
{
  return __nds__v_smalxds (t, a, b);
}


int
main ()
{
  long long a_p = 0x12345678ffff5555ll;
  long long va_p = 0x12345678ffff5555ll;
  long long a = smalxds (0x12345678ffffffffll, 0x1234000112340001, 0x6789000167890001);
  long long va = v_smalxds (0x12345678ffffffffll, (int16x4_t) {0x0001, 0x1234, 0x0001, 0x1234},
						  (int16x4_t) {0x0001, 0x6789, 0x0001, 0x6789});

  if (a != a_p)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
