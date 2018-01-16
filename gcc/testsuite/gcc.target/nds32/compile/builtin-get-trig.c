/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

int
main (void)
{
  int a = __nds32__get_trig_type (NDS32_INT_H0);
  int b = __nds32__get_trig_type (NDS32_INT_H15);
  int c = __nds32__get_trig_type (NDS32_INT_H16);
  int d = __nds32__get_trig_type (NDS32_INT_H31);
  return a + b + c + d;
}
