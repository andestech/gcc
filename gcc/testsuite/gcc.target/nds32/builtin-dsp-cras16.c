/* This is a test program for cras16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int cras16 (unsigned int ra, unsigned int rb)
{
  return __nds32__cras16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_ucras16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_ucras16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_scras16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_scras16 (ra, rb);
}

int
main ()
{

#ifdef __NDS32_EL__
  uint16x2_t v_ua_p = {1, 0};
  int16x2_t v_sa_p = {0x1000, 0x111};
#else
  uint16x2_t v_ua_p = {0x2469, 0xe000};
  int16x2_t v_sa_p = {0x3000, 0xe111};
#endif

  unsigned int a = cras16 (0x0001f000, 0x0001f000);
  uint16x2_t v_ua = v_ucras16 ((uint16x2_t) {0x1235, 0xf000},
			       (uint16x2_t) {0x1000, 0x1234});
  int16x2_t v_sa = v_scras16 ((int16x2_t) {0x2000, 0xf111},
			      (int16x2_t) {0x1000, 0x1000});

  if (a != 0xf001efff)
    abort ();
  else if (v_ua[0] != v_ua_p[0]
	   || v_ua[1] != v_ua_p[1])
    abort ();
  else if (v_sa[0] != v_sa_p[0]
	   || v_sa[1] != v_sa_p[1])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
