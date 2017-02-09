/* This is a test program for umin8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long umin8 (unsigned long ra, unsigned long rb)
{
  return __nds__umin8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x8_t v_umin8 (uint8x8_t ra, uint8x8_t rb)
{
  return __nds__v_umin8 (ra, rb);
}

int
main ()
{
  uint8x8_t v_sa_p = {0x7f, 0xfe, 0, 0x15, 0x7f, 0xfe, 0, 0x15};
  unsigned long a = umin8 (0xfffe0001fffe0001, 0xffff0000ffff0000);
  uint8x8_t v_sa = v_umin8 ((uint8x8_t) {0x7f, 0xff, 0x0, 0x15, 0x7f, 0xff, 0x0, 0x15},
			    (uint8x8_t) {0x7f, 0xfe, 0x1, 0x23, 0x7f, 0xfe, 0x1, 0x23});
  if (a != 0xfffe0000fffe0000)
    abort ();
  else if (!vec64_all_eq (u, 8, v_sa_p, v_sa))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
