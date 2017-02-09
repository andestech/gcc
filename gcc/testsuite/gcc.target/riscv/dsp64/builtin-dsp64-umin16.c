/* This is a test program for umin16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long umin16 (unsigned long ra, unsigned long rb)
{
  return __nds__umin16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_umin16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_umin16 (ra, rb);
}

int
main ()
{
  uint16x4_t va_p = {0x7ffe, 0, 0x7ffe, 0};
  unsigned long a = umin16 (0xfffe0001fffe0001, 0xffff0000ffff0000);
  uint16x4_t va = v_umin16 ((uint16x4_t) {0x7fff, 0, 0x7fff, 0},
			    (uint16x4_t) {0x7ffe, 1, 0x7ffe, 1});
  if (a != 0xfffe0000fffe0000)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
