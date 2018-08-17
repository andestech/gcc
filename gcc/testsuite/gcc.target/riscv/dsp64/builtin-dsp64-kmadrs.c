/* This is a test program for kmadrs instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long kmadrs (unsigned long rd, unsigned long ra, unsigned long rb)
{
  return __nds__kmadrs (rd, ra, rb);
}

static __attribute__ ((noinline))
long v_kmadrs (unsigned long rd, int16x4_t ra, int16x4_t rb)
{
  return __nds64__v_kmadrs (rd, ra, rb);
}

int
main ()
{
  long va_p = 0x200000002;
  long a = kmadrs (0x300000003, 0x8000000280000002, 0x8000000180000001);
  long va = v_kmadrs (0x300000003,
		      (int16x4_t) {0xffff, 0x0002, 0xffff, 0x0002},
		      (int16x4_t) {0xffff, 0x0001, 0xffff, 0x0001});

  if (a != 0xc0000005c0000005)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
