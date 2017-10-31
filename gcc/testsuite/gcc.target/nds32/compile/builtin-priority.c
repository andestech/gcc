/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

int
main (void)
{
  __nds32__set_int_priority (NDS32_INT_H0,  0);
  __nds32__set_int_priority (NDS32_INT_H15, 3);
  __nds32__set_int_priority (NDS32_INT_H31, 3);

  int a =  __nds32__get_int_priority (NDS32_INT_H0);
  int b =  __nds32__get_int_priority (NDS32_INT_H15);
  int c =  __nds32__get_int_priority (NDS32_INT_H31);

  return a + b + c;
}
