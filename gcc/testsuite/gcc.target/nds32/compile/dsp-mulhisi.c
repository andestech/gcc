/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "smbb" } } */
/* { dg-final { scan-assembler "smbt" } } */
/* { dg-final { scan-assembler "smtt" } } */

typedef signed char v4qi __attribute__ ((vector_size (4)));
typedef short v2hi __attribute__ ((vector_size (4)));

int smbb(v2hi a, v2hi b)
{
  return a[0] * b[0];
}

int smbt(v2hi a, v2hi b)
{
  return a[0] * b[1];
}

int smtt(v2hi a, v2hi b)
{
  return a[1] * b[1];
}
