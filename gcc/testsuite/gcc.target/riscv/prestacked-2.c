/* { dg-do compile } */
int
bar (int);
void __attribute__ ((prestacked ("x8"))) foo (void)
{
  int ret, idx = 0;
  for (idx; idx < 10; idx++)
    ret = bar (idx);
}
/* Verify x8 is prestacked. */
/* { dg-final { scan-assembler-not "s\[wd\]\ts0" } } */
/* { dg-final { scan-assembler-not "l\[wd\]\ts0" } } */
