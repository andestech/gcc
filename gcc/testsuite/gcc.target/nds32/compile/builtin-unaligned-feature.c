/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

int
main ()
{
  unsigned unalign = __nds32__unaligned_feature ();
   __nds32__enable_unaligned ();
   __nds32__disable_unaligned ();
  return unalign;
}
