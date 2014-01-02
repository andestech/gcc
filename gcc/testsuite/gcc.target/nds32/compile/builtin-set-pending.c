/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

int
main (void)
{
  __nds32__set_pending_swint ();
}
