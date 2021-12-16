/* Test that BFmode should locate in define_mode_attr softstore */
/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-options "-O1 -march=rv64ifd -mabi=lp64d -mcmodel=medany -mbf16" } */

__bf16 f;

void
test1 (short i)
{
  f = *(__bf16 *) &i;
}
/* { dg-final { scan-assembler "sh" } } */
