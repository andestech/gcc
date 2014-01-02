/* { dg-do compile } */
/* { dg-final { scan-assembler ".v2abi_compatible" } } */

int
add (int a, int b)
{
  return a + b;
}

short
sub (short a, short b)
{
  return a - b;
}

long
mul (long a, long b)
{
  return a * b;
}

char
size (char a)
{
  return a;
}
