/* Verify that we generate svs instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tsvs" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int a, b;
  char c;

  c = __nds32__svs (a, b);
}
