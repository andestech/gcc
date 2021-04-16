/* This is a test program for smalda instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
long long smalda (long long t, unsigned int a, unsigned int b)
{
  return __nds32__smalda (t, a, b);
}

static __attribute__ ((noinline))
long long v_smalda (long long t, int16x2_t a, int16x2_t b)
{
  return __nds32__v_smalda (t, a, b);
}


int
main ()
{
  long long a = smalda (0x12345678ffffffffll, 0x67890000, 0x12340000);
  long long va = v_smalda (0x12345678ffffffffll, (int16x2_t) {0, 0x6789},
						 (int16x2_t) {0, 0x1234});

  if (a != 0x12345679075CA9D3ll)
    abort ();
  else if (va != 0x12345679075CA9D3ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
