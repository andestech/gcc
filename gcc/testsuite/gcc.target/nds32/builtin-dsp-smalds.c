/* This is a test program for smalds instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
long long smalds (long long t, unsigned int a, unsigned int b)
{
  return __nds32__smalds (t, a, b);
}

static __attribute__ ((noinline))
long long v_smalds (long long t, int16x2_t a, int16x2_t b)
{
  return __nds32__v_smalds (t, a, b);
}


int
main ()
{
#ifdef __NDS32_EL__
  long long a_p = 0x12345678ffffaaaall;
  long long va_p = 0x12345678ffffaaaall;
#else
  long long a_p = 0x12345678ffffaaaall;
  long long va_p = 0x1234567900005554ll;
#endif

  long long a = smalds (0x12345678ffffffffll, 0x12340001, 0x00016789);
  long long va = v_smalds (0x12345678ffffffffll, (int16x2_t) {0x0001, 0x1234},
						 (int16x2_t) {0x6789, 0x0001});

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
