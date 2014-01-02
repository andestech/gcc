/* { dg-do compile } */
/* { dg-final { scan-assembler-not ".v2abi_compatible" } } */

float
foo (void)
{
  return 1.0f;
}
