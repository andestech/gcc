/* Verify that we generate sva instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tsva" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int a, b;
  char c;

  c = __nds32__sva (a, b);
}
