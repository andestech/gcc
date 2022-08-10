/* { dg-do compile } */
/* { dg-options "-march=rv32i -mabi=ilp32" } */

/* __builtin_riscv_prefetch should generate nothing if Zicbop is not enabled. */

int
func (void *addr)
{
  __builtin_riscv_prefetch (addr, 0, 0, 0);
  __builtin_riscv_prefetch (addr, 0, 0, 1);
  __builtin_riscv_prefetch (addr, 1, 0, 0);
  __builtin_riscv_prefetch (addr, 1, 0, 1);
  return 0;
}

/* { dg-final { scan-assembler-not "prefetch.i" } } */
/* { dg-final { scan-assembler-not "prefetch.r" } } */
/* { dg-final { scan-assembler-not "prefetch.w" } } */