/* This is a test program for kmadrs32 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kmadrs32 (unsigned long rd, unsigned long ra, unsigned long rb)
{
  return __nds__kmadrs32 (rd, ra, rb);
}

static __attribute__ ((noinline))
long v_kmadrs32 (long rd, int32x2_t ra, int32x2_t rb)
{
  return __nds__v_kmadrs32 (rd, ra, rb);
}

int
main ()
{
  long va_p = 0x2;
  long a = kmadrs32 (0x2, 0x8000000280000002, 0x8000000180000001);
  long va = v_kmadrs32 (0x2, (int32x2_t) {0xffff0002, 0xffff0002},
			(int32x2_t) {0xffff0001, 0xffff0001});

  if (a != 0x2)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
