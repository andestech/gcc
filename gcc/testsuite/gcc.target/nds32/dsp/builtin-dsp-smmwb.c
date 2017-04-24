/* This is a test program for smmwb instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
int smmwb (int ra, unsigned int rb)
{
  return __nds32__smmwb (ra, rb);
}

static __attribute__ ((noinline))
int v_smmwb (int ra, int16x2_t rb)
{
  return __nds32__v_smmwb (ra, rb);
}

int
main ()
{
#ifdef __NDS32_EL__
  int va_p = 0;
#else
  int va_p = 0xffffffff;
#endif

  int a = smmwb (0x80000002, 0x80000001);

  int va = v_smmwb (0xffff0002, (int16x2_t) {0xffff, 0x0001});

  if (a != 0xffff8000)
    abort ();
  else if (va != va_p)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
