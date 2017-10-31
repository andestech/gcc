/* Verify that we generate mfsr/mtsr instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tmfsr" } } */
/* { dg-final { scan-assembler "\\tmtsr" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int ipsw_value;

  ipsw_value = __nds32__mfsr (__NDS32_REG_IPSW__);
  __nds32__mtsr (ipsw_value, __NDS32_REG_IPSW__);
}
