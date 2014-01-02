/* { dg-do compile } */
/* { dg-final { scan-assembler-not ".v2abi_compatible" } } */

int
foo (int a, int b)
{
  float c = 1.33f;

  c += c;
  return a + b + c;
}
