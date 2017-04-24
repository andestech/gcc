/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "raddw" } } */
/* { dg-final { scan-assembler "rsubw" } } */
/* { dg-final { scan-assembler "uraddw" } } */
/* { dg-final { scan-assembler "ursubw" } } */

int raddw(int a, int b)
{
  return (a + b) >> 1;
}

int rsubw(int a, int b)
{
  return (a - b) >> 1;
}

unsigned uraddw(unsigned a, unsigned b)
{
  return (a + b) >> 1;
}

unsigned ursubw(unsigned a, unsigned b)
{
  return (a - b) >> 1;
}
