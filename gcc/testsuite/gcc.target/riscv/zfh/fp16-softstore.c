/* Test that HFmode should locate in define_mode_attr softstore */
/* { dg-do compile } */
/* { dg-require-effective-target riscv64 } */
/* { dg-options "-O1 -march=rv64ifd -mabi=lp64d -mcmodel=medany -mzfh" } */

__fp16 f;

void
test1 (short i)
{
  f = *(__fp16 *) &i;
}
/* { dg-final { scan-assembler "sh" } } */
