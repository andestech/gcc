/* This is a test program for ucmplt8 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int ucmplt8 (unsigned int ra, unsigned int rb)
{
  return __nds32__ucmplt8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_ucmplt8 (uint8x4_t ra, uint8x4_t rb)
{
  return __nds32__v_ucmplt8 (ra, rb);
}

int
main ()
{
  unsigned int a = ucmplt8 (0xfefe0101, 0xffff0000);
  uint8x4_t va = v_ucmplt8 ((uint8x4_t) {0x7e, 0x7e, 0x01, 0x01},
			    (uint8x4_t) {0x7f, 0x7f, 0x00, 0x00});

  if (a != 0xffff0000)
    abort ();
  else if (va[0] != 0xff
           || va[1] != 0xff
           || va[2] != 0
	   || va[3] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
