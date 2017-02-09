/* This is a test program for smaldrs instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smaldrs (long long t, unsigned long a, unsigned long b)
{
  return __nds__smaldrs (t, a, b);
}

static __attribute__ ((noinline))
long long v_smaldrs (long long t, int16x2_t a, int16x2_t b)
{
  return __nds__v_smaldrs (t, a, b);
}


int
main ()
{
  long long a_p = 0x12345678ffffaaaall;
  long long va_p = 0x12345678ffffaaaall;
  long long a = smaldrs (0x12345678ffffffffll, 0x67890001, 0x00011234);
  long long va = v_smaldrs (0x12345678ffffffffll, (int16x2_t) {0x0001, 0x6789},
						  (int16x2_t) {0x1234, 0x0001});

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
