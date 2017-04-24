/* Verify that we generate setgie.e instruction with builtin function.  */

/* { dg-do compile */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tsetgie.e" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  __nds32__setgie_en ();
}
