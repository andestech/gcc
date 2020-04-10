/* This is a test program for smax16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long smax16 (unsigned long ra, unsigned long rb)
{
  return __nds__smax16 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_smax16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_smax16 (ra, rb);
}

int
main ()
{
  int16x4_t va_p = {0x7fff, 1, 0x7fff, 1};
  unsigned long a = smax16 (0xfffe0001fffe0001, 0xffff0000ffff0000);
  int16x4_t va = v_smax16 ((int16x4_t) {0x7fff, 0, 0x7fff, 0},
			   (int16x4_t) {0x7ffe, 1, 0x7ffe, 1});
  if (a != 0xffff0001ffff0001)
    abort ();
  else if (!vec64_all_eq (s, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
