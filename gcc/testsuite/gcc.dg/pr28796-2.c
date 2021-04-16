/* { dg-do run } */
/* { dg-options "-O2 -funsafe-math-optimizations -fno-finite-math-only -DUNSAFE" } */
/* { dg-add-options ieee } */
/* { dg-skip-if "No Inf/NaN support" { spu-*-* } } */
/* { dg-skip-if "No Denormmalized support" { nds32_ext_fpu } } */

#include "tg-tests.h"

int main(void)
{
  return main_tests ();
}
