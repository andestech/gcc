/* This is a test program for urcrsa16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int urcrsa16 (unsigned int ra, unsigned int rb)
{
  return __nds32__urcrsa16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_urcrsa16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_urcrsa16 (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  uint16x2_t va_p = {0x8000, 0xffff};
#else
  uint16x2_t va_p = {0, 0x7fff};
#endif

  unsigned int a = urcrsa16 (0x7fff7fff, 0x7fff8000);
  uint16x2_t va = v_urcrsa16 ((uint16x2_t) {0x8000, 0x7fff},
			      (uint16x2_t) {0x8000, 0x8000});

  if (a != 0xffff7fff)
    abort ();
  else if (va[0] != va_p[0]
	   || va[1] != va_p[1])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
