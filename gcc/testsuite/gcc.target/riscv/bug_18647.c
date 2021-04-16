/* This is a test program for khm*16 instruction.  */
/* { dg-do compile } */
/* { dg-require-effective-target riscv64 } */
/* { dg-options "-mext-dsp -O3" } */
/* { dg-final { scan-assembler "khmbb16" } } */
/* { dg-final { scan-assembler "khmbt16" } } */
/* { dg-final { scan-assembler "khmtt16" } } */

#include <nds_intrinsic.h>

int main()
{
  long a = 0x2222222222222222;
  long b = 0x1111111133333333;
  long c = __nds__khmbb16(a, b);
  long d = __nds__khmbt16(a, b);
  long e = __nds__khmtt16(a, b);
  return (int) (c + d + e);
}
