/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "smalxda" } } */
/* { dg-final { scan-assembler "smalxds" } } */

typedef signed char v4qi __attribute__ ((vector_size (4)));
typedef short v2hi __attribute__ ((vector_size (4)));

long long smalxda(long long acc, v2hi a, v2hi b)
{
  return acc + (a[0] * b[1] + a[1] * b[0]);
}

long long smalxds(long long acc, v2hi a, v2hi b)
{
  return acc + (a[1] * b[0] - a[0] * b[1]);
}
