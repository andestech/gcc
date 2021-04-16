/* This is a test program for smax8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long smax8 (unsigned long ra, unsigned long rb)
{
  return __nds__smax8 (ra, rb);
}

static __attribute__ ((noinline))
int8x8_t v_smax8 (int8x8_t ra, int8x8_t rb)
{
  return __nds__v_smax8 (ra, rb);
}

int
main ()
{
  int8x8_t v_sa_p = {0x7f, 0xff, 0x1, 0x23, 0x7f, 0xff, 0x1, 0x23};
  unsigned long a = smax8 (0xfffe0001fffe0001, 0xffff0000ffff0000);
  int8x8_t v_sa = v_smax8 ((int8x8_t) {0x7f, 0xff, 0x0, 0x15, 0x7f, 0xff, 0x0, 0x15},
			   (int8x8_t) {0x7f, 0xfe, 0x1, 0x23, 0x7f, 0xfe, 0x1, 0x23});
  if (a != 0xffff0001ffff0001)
    abort ();
  else if (!vec64_all_eq (s, 8, v_sa_p, v_sa))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
