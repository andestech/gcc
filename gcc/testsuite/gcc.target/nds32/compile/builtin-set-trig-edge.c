/* { dg-do compile } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>

void
main (void)
{
  __nds32__set_trig_type_edge (NDS32_INT_H0);
  __nds32__set_trig_type_edge (NDS32_INT_H15);
  __nds32__set_trig_type_edge (NDS32_INT_H16);
  __nds32__set_trig_type_edge (NDS32_INT_H31);
}
