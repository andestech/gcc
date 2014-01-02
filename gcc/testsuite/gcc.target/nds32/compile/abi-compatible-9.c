/* { dg-do compile } */
/* { dg-final { scan-assembler-not ".v2abi_compatible" } } */

#include <math.h>

int x;
int bar (int a, int b);

void
foo (int a, int b)
{
  x = bar (a, b);
}

int
bar (int a, int b)
{
  double c = 0.78;
  return a + b + (int)sin (c);
}
