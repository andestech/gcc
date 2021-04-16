/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "smmul" } } */

typedef signed char v4qi __attribute__ ((vector_size (4)));
typedef short v2hi __attribute__ ((vector_size (4)));

int smmul(int a, int b)
{
  long long tmp = (long long)a * b;
  return (int)((tmp >> 32) & 0xffffffffll);
}
