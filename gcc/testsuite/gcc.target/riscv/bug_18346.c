/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc -mabi=lp64 -mext-dsp -O0" } */
long test (long a, long b, long c)
{
  long ret = (long) __builtin_riscv_kmmac_u ((long) a, (long) b, (long) c);
  return ret;
}
/* { dg-final { scan-assembler-times "lw" 0 } } */
