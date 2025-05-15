/* { dg-do compile } */
/* { dg-options "-mext-cmo -mext-ntlh" } */

int
func (void *addr)
{
  __builtin_riscv_prefetch (addr, 0, 0, 0);
  __builtin_riscv_prefetch (addr, 0, 1, 1);
  __builtin_riscv_prefetch (addr, 1, 2, 0);
  __builtin_riscv_prefetch (addr, 1, 3, 1);
  return 0;
}

/* { dg-final { scan-assembler-times "prefetch.i" 2 } } */
/* { dg-final { scan-assembler-times "prefetch.r" 1 } } */
/* { dg-final { scan-assembler-times "prefetch.w" 1 } } */
/* { dg-final { scan-assembler-times "ntl.all" 1 } } */
/* { dg-final { scan-assembler-times "ntl.pall" 1 } } */
/* { dg-final { scan-assembler-times "ntl.p1" 1 } } */
