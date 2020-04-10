/* This is a test program for ucmplt16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ucmplt16 (unsigned long ra, unsigned long rb)
{
  return __nds__ucmplt16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_ucmplt16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds__v_ucmplt16 (ra, rb);
}

int
main ()
{
  unsigned long a = ucmplt16 (0xfffe0001, 0xffff0000);
  uint16x2_t va = v_ucmplt16 ((uint16x2_t) {0x7fff, 0x7ffe},
			      (uint16x2_t) {0x7ffe, 0x7fff});
  if (a != 0xffff0000)
    abort ();
  else if (va[0] != 0
           || va[1] != 0xffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
