/* This is a test program for scmplt8 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int scmplt8 (unsigned int ra, unsigned int rb)
{
  return __nds32__scmplt8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_scmplt8 (int8x4_t ra, int8x4_t rb)
{
  return __nds32__v_scmplt8 (ra, rb);
}

int
main ()
{
  unsigned int a = scmplt8 (0xfefe0101, 0xffff0000);
  uint8x4_t va = v_scmplt8 ((int8x4_t) {0x7e, 0x7e, 0x01, 0x01},
			    (int8x4_t) {0x7f, 0x7f, 0x00, 0x00});

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
