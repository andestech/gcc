/* This is a test program for smxds instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
int smxds (unsigned int ra, unsigned int rb)
{
  return __nds32__smxds (ra, rb);
}

static __attribute__ ((noinline))
int v_smxds (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_smxds (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  int a_p = 0x8000;
  int va_p = 0xffffffff;
#else
  int a_p = 0x8000;
  int va_p = 1;
#endif

  int a = smxds (0x80000002, 0x80000001);
  int va = v_smxds ((int16x2_t) {0xffff, 0x0002},
		    (int16x2_t) {0xffff, 0x0001});

  if (a != a_p)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
