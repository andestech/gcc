/* This is a test program for smin16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long smin16 (unsigned long ra, unsigned long rb)
{
  return __nds__smin16 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_smin16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_smin16 (ra, rb);
}

int
main ()
{
  int16x4_t v_sa_p = {0x7ffe, 0, 0x7ffe, 0};
  unsigned long a = smin16 (0xfffe0001fffe0001, 0xffff0000ffff0000);
  int16x4_t v_sa = v_smin16 ((int16x4_t) {0x7fff, 0, 0x7fff, 0},
			     (int16x4_t) {0x7ffe, 1, 0x7ffe, 1});
  if (a != 0xfffe0000fffe0000)
    abort ();
  else if (!vec64_all_eq (s, 16, v_sa_p, v_sa))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
