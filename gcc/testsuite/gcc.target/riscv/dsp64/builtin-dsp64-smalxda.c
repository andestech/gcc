/* This is a test program for smalxda instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long long smalxda (long long t, unsigned long a, unsigned long b)
{
  return __nds__smalxda (t, a, b);
}

static __attribute__ ((noinline))
long long v_smalxda (long long t, int16x4_t a, int16x4_t b)
{
  return __nds64__v_smalxda (t, a, b);
}


int
main ()
{
  long long a = smalxda (0x12345678ffffffffll, 0x6789000067890000, 0x1234000012340000);
  long long va = v_smalxda (0x12345678ffffffffll, (int16x4_t) {0, 0x6789, 0, 0x6789},
						 (int16x4_t) {0, 0x1234, 0, 0x1234});

  if (a != 0x12345678ffffffffll)
    abort ();
  else if (va != 0x12345678ffffffffll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
