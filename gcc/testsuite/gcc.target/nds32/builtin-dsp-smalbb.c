/* This is a test program for smalbb instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
long long smalbb (long long t, unsigned int a, unsigned int b)
{
  return __nds32__smalbb (t, a, b);
}

static __attribute__ ((noinline))
long long v_smalbb (long long t, int16x2_t a, int16x2_t b)
{
  return __nds32__v_smalbb (t, a, b);
}


int
main ()
{
#ifdef __NDS32_EL__
  long long a_p = 0x12345679075ca9d3ll;
  long long va_p = 0x12345679075ca9d3ll;
#else
  long long a_p = 0x12345679075ca9d3ll;
  long long va_p = 0x12345678ffffffffll;
#endif

  long long a = smalbb (0x12345678ffffffffll,0x00006789, 0x00001234);
  long long va = v_smalbb (0x12345678ffffffffll, (int16x2_t) {0x6789, 0},
						 (int16x2_t) {0x1234, 0});
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
