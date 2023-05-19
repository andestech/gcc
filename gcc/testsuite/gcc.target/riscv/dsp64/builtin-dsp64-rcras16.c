/* This is a test program for rcras16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long rcras16 (unsigned long ra, unsigned long rb)
{
  return __nds__rcras16 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_rcras16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_rcras16 (ra, rb);
}

int
main ()
{
  int16x4_t va_p = {0x7fff, 0x8000, 0x7fff, 0x8000};
  unsigned long a = rcras16 (0x0fff00000fff0000, 0x00000fff00000fff);
  int16x4_t va = v_rcras16 ((int16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000},
			    (int16x4_t) {0x8000, 0x8000, 0x8000, 0x8000});

  if (a != 0x0fff00000fff0000)
    abort ();
  else if (va[0] != va_p[0]
	   || va[1] != va_p[1]
	   || va[2] != va_p[2]
	   || va[3] != va_p[3])
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
