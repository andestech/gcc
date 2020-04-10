/* This is a test program for sclip16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
signed long sclip16 (signed long ra)
{
  return __nds__sclip16 (ra, 2);
}

static __attribute__ ((noinline))
int16x4_t v_sclip16 (int16x4_t ra)
{
  return __nds__v_sclip16 (ra, 4);
}

int
main ()
{
  int16x4_t v_sa_p = {0, -16, 15, 3};
  signed long a = sclip16 (0x1010101010101010);
  int16x4_t v_sa = v_sclip16 ((int16x4_t) {0, -20, 20, 3});

  if (a != 0x3000300030003)
    abort ();
  else if (!vec64_all_eq (s, 16, v_sa_p, v_sa))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
