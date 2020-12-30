/* This is a test program for cmpeq16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long cmpeq16 (unsigned long ra, unsigned long rb)
{
  return __nds__cmpeq16 (ra, rb);
}
static __attribute__ ((noinline))
uint16x4_t v_scmpeq16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_scmpeq16 (ra, rb);
}
static __attribute__ ((noinline))
uint16x4_t v_ucmpeq16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_ucmpeq16 (ra, rb);
}
int
main ()
{
  uint16x4_t v_ua_p = {0, 0xffff, 0, 0xffff};
  uint16x4_t v_sa_p = {0, 0xffff, 0, 0xffff};
  unsigned long a = cmpeq16 (0xffff0000ffff0000, 0xffff0001ffff0001);
  uint16x4_t v_sa = v_scmpeq16 ((int16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000},
			        (int16x4_t) {0x8000, 0x8000, 0x8000, 0x8000});
  uint16x4_t v_ua = v_ucmpeq16 ((uint16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000},
				(uint16x4_t) {0x8000, 0x8000, 0x8000, 0x8000});
  if (a != 0xffff0000ffff0000)
    abort ();
  else if (!vec64_all_eq (u, 16, v_sa_p, v_sa))
    abort ();
  else if (!vec64_all_eq (u, 16, v_ua_p, v_ua))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
