/* This is a test program for ucmplt16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ucmplt16 (unsigned long ra, unsigned long rb)
{
  return __nds__ucmplt16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_ucmplt16 (uint16x4_t ra, uint16x4_t rb)
{
  return __nds__v_ucmplt16 (ra, rb);
}

int
main ()
{
  uint16x4_t va_p = {0, 0xffff, 0, 0xffff};
  unsigned long a = ucmplt16 (0xfffe0001fffe0001, 0xffff0000ffff0000);
  uint16x4_t va = v_ucmplt16 ((uint16x4_t) {0x7fff, 0x7ffe, 0x7fff, 0x7ffe},
			      (uint16x4_t) {0x7ffe, 0x7fff, 0x7ffe, 0x7fff});
  if (a != 0xffff0000ffff0000)
    abort ();
  else if (!vec64_all_eq (u, 16, va_p, va))
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
