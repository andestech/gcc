/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "add8" } } */
/* { dg-final { scan-assembler "add16" } } */
/* { dg-final { scan-assembler "add64" } } */
/* { dg-final { scan-assembler "sub8" } } */
/* { dg-final { scan-assembler "sub16" } } */
/* { dg-final { scan-assembler "sub64" } } */

typedef signed char v4qi __attribute__ ((vector_size (4)));
typedef short v2hi __attribute__ ((vector_size (4)));

v4qi  __attribute__ ((noinline))
add8 (v4qi a, v4qi b)
{
  return a + b;
}

v4qi  __attribute__ ((noinline))
sub8 (v4qi a, v4qi b)
{
  return a - b;
}

v2hi  __attribute__ ((noinline))
add16 (v2hi a, v2hi b)
{
  return a + b;
}

v2hi  __attribute__ ((noinline))
sub16 (v2hi a, v2hi b)
{
  return a - b;
}

long long
add64 (long long a, long long b)
{
  return a + b;
}

long long
sub64 (long long a, long long b)
{
  return a - b;
}
