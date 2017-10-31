/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "smalbb" } } */
/* { dg-final { scan-assembler "smalbt" } } */
/* { dg-final { scan-assembler "smaltt" } } */
/* { dg-final { scan-assembler "smal" } } */

typedef signed char v4qi __attribute__ ((vector_size (4)));
typedef short v2hi __attribute__ ((vector_size (4)));


long long smalbb(long long acc, v2hi a, v2hi b)
{
  return acc + a[0] * b[0];
}

long long smalbt(long long acc, v2hi a, v2hi b)
{
  return acc + a[1] * b[0];
}

long long smaltt(long long acc, v2hi a, v2hi b)
{
  return acc + a[1] * b[1];
}

long long smal(v2hi a, long long b)
{
  return b + (long long)(a[0] * a[1]);
}
