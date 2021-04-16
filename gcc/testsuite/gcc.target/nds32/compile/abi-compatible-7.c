/* { dg-do compile } */
/* { dg-final { scan-assembler ".v2abi_compatible" } } */

int bar (float *a, double b []);

int
foo (void)
{
  float *a;
  double b[10];
  return bar (a, b);
}
