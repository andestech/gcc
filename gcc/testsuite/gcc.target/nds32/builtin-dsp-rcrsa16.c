/* This is a test program for rcrsa16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int rcrsa16 (unsigned int ra, unsigned int rb)
{
  return __nds32__rcrsa16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_rcrsa16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_rcrsa16 (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  int16x2_t va_p = {0x8000, 0x8000};
#else
  int16x2_t va_p = {0, 0xffff};
#endif

  unsigned int a = rcrsa16 (0x7fff7fff, 0x7fff8000);
  int16x2_t va = v_rcrsa16 ((int16x2_t) {0x8000, 0x8000},
			    (int16x2_t) {0x7fff, 0x8000});

  if (a != 0x7fff7fff)
    abort ();
  else if (va[0] != va_p [0]
	   || va[1] != va_p [1])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
