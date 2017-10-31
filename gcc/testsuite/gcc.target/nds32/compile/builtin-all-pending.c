/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

int
main (void)
{
  int a = __nds32__get_all_pending_int ();
  return a;
}
