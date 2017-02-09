/* Verify that csrrs and csrrsi instruction are generated by builtin function.  */

/* { dg-do compile } */
/* { dg-final { scan-assembler-times "\tcsrrs\t" 1 } } */
/* { dg-final { scan-assembler-times "\tcsrrsi\t" 1 } } */

#include <nds_intrinsic.h>

void
func (unsigned long src)
{
  unsigned long data;
  data = __nds__read_and_set_csr (src, 0x304);
}

void
func_i (void)
{
  unsigned long data;
  data = __nds__read_and_set_csr (0x1, 0x304);
}
