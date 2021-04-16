/* { dg-do compile } */
/* { dg-final { scan-assembler-not ".v2abi_compatible" } } */

int
foo (int a, int b, int c, ...)
{
  return a + b + c;
}
