/* Verify that we generate mfusr/mtusr instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tmfusr" } } */
/* { dg-final { scan-assembler "\\tmtusr" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int itype_value;

  itype_value = __nds32__mfusr (__NDS32_REG_ITYPE__);
  __nds32__mtusr (itype_value, __NDS32_REG_ITYPE__);
}
