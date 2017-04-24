/* This is a test program for cmpeq16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int cmpeq16 (unsigned int ra, unsigned int rb)
{
  return __nds32__cmpeq16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_scmpeq16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_scmpeq16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_ucmpeq16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_ucmpeq16 (ra, rb);
}

int
main ()
{
  unsigned int a = cmpeq16 (0xffff0000, 0xffff0001);
  uint16x2_t v_sa = v_scmpeq16 ((int16x2_t) {0x7fff, 0x8000},
				(int16x2_t) {0x8000, 0x8000});
  uint16x2_t v_ua = v_ucmpeq16 ((uint16x2_t) {0x7fff, 0x8000},
				(uint16x2_t) {0x8000, 0x8000});

  if (a != 0xffff0000)
    abort ();
  else if (v_sa[0] != 0
	   || v_sa[1] != 0xffff)
    abort ();
  else if (v_ua[0] != 0
	   || v_ua[1] != 0xffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
