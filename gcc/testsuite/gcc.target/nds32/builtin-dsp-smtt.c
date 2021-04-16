/* This is a test program for smtt instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
int smtt (unsigned int ra, unsigned int rb)
{
  return __nds32__smtt (ra, rb);
}

static __attribute__ ((noinline))
int v_smtt (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_smtt (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  int va_p = 2;
#else
  int va_p = 1;
#endif

  int a = smtt (0x80000002, 0x80000001);

  int va = v_smtt ((int16x2_t) {0xffff, 0x0002},
		   (int16x2_t) {0xffff, 0x0001});

  if (a != 0x40000000)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
