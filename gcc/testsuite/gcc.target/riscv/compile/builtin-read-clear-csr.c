/* Verify that csrrc and csrrci instruction are generated by builtin function.  */

/* { dg-do compile } */
/* { dg-final { scan-assembler-times "\tcsrrc\t" 1 } } */
/* { dg-final { scan-assembler-times "\tcsrrci\t" 1 } } */

#include <nds_intrinsic.h>

void
func (unsigned long src)
{
  unsigned long data;
  data = __nds__read_and_clear_csr (src, 0x304);
}

void
func_i (void)
{
  unsigned long data;
  data = __nds__read_and_clear_csr (0x1, 0x304);
}
