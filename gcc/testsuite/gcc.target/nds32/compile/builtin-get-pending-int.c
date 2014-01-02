/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

int
main (void)
{
  int a = __nds32__get_pending_int (NDS32_INT_H15);
  int b = __nds32__get_pending_int (NDS32_INT_SWI);
  int c = __nds32__get_pending_int (NDS32_INT_H16);

  return a + b + c;
}
