/* { dg-do compile } */
/* { dg-options "-Os -mext-perf2" } */
/* { dg-final { scan-assembler "bsp" } } */
/* { dg-final { scan-assembler "bse" } } */
/* { dg-require-effective-target nds32_ext_perf2 } */

void test(void)
{
  unsigned int *a = 0, b = 0, *c = 0;
  __builtin_nds32_pe2_bsp (a, b, c);
  __builtin_nds32_pe2_bse (a, b, c);
}
