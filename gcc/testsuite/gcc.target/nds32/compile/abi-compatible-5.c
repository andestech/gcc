/* { dg-do compile } */
/* { dg-final { scan-assembler-not ".v2abi_compatible" } } */

struct test
{
  int i;
  short s;
  char c;
};

void
foo (struct test a, int b)
{
  a.i = b;
}
