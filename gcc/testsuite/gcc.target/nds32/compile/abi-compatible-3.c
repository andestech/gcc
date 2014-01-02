/* { dg-do compile } */
/* { dg-final { scan-assembler-not ".v2abi_compatible" } } */

int x;

void
foo (int a, double b)
{
  x = a + (int)b;
}
