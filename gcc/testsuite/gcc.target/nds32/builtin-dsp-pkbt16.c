/* This is a test program for pkbt16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int pkbt16 (unsigned int ra, unsigned int rb)
{
  return __nds32__pkbt16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_pkbt16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_pkbt16 (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  uint16x2_t va_p = {0xdddd, 0xaaaa};
#else
  uint16x2_t va_p = {0xbbbb, 0xcccc};
#endif

  unsigned int a = pkbt16 (0x11223344, 0x55667788);
  uint16x2_t va = v_pkbt16 ((uint16x2_t) {0xaaaa, 0xbbbb},
			    (uint16x2_t) {0xcccc, 0xdddd});

  if (a != 0x33445566)
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
