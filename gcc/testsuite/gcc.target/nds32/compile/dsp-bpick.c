/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "bpick" } } */

int bpick(int a, int b, int mask)
{
  return (a & mask) | (b & ~mask);
}
