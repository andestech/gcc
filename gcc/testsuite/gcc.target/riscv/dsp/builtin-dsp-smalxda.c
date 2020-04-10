/* This is a test program for smalxda instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smalxda (long long t, unsigned long a, unsigned long b)
{
  return __nds__smalxda (t, a, b);
}

static __attribute__ ((noinline))
long long v_smalxda (long long t, int16x2_t a, int16x2_t b)
{
  return __nds__v_smalxda (t, a, b);
}


int
main ()
{
  long long a = smalxda (0x12345678ffffffffll, 0x67890000, 0x00001234);
  long long va = v_smalxda (0x12345678ffffffffll, (int16x2_t) {0, 0x6789},
						  (int16x2_t) {0x1234, 0});

  if (a != 0x12345679075CA9D3)
    abort ();
  else if (va != 0x12345679075CA9D3)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
