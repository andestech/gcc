/* This is a test program for cmpeq8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long cmpeq8 (unsigned long ra, unsigned long rb)
{
  return __nds__cmpeq8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x8_t v_scmpeq8 (int8x8_t ra, int8x8_t rb)
{
  return __nds__v_scmpeq8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x8_t v_ucmpeq8 (uint8x8_t ra, uint8x8_t rb)
{
  return __nds__v_ucmpeq8 (ra, rb);
}

int
main ()
{
  uint8x8_t v_ua_p = {0xff, 0xff, 0, 0, 0xff, 0xff, 0, 0};
  uint8x8_t v_sa_p = {0xff, 0xff, 0, 0, 0xff, 0xff, 0, 0};
  unsigned long a = cmpeq8 (0xffff0000ffff0000, 0xffff0101ffff0101);
  uint8x8_t v_sa = v_scmpeq8 ((int8x8_t) {0x7f, 0x7f, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01},
			      (int8x8_t) {0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x00, 0x00});
  uint8x8_t v_ua = v_ucmpeq8 ((uint8x8_t) {0x7f, 0x7f, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01},
			      (uint8x8_t) {0x7f, 0x7f, 0x00, 0x00, 0x7f, 0x7f, 0x00, 0x00});

  if (a != 0xffff0000ffff0000)
    abort ();
  else if (!vec64_all_eq (u, 8, v_sa_p, v_sa))
    abort ();
  else if (!vec64_all_eq (u, 8, v_ua_p, v_ua))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
