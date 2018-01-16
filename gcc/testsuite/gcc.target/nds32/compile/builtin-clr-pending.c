/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

void
main (void)
{
  __nds32__clr_pending_swint ();
}
