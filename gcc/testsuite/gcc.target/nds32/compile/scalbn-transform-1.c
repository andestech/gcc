/* Verify scalbn transform pass for normal case.  */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-all -lm" } */
/* { dg-require-effective-target nds32_soft_fp } */

float test_scalbnf (float x)
{
  return x * 128;
}

double test_scalbn (double x)
{
  return x * 256;
}

/* { dg-final { scan-tree-dump "(_\[0-9\]+) = __builtin_scalbnf \\(x_\[0-9\]+\\(D\\), 7\\);\\s*_\[0-9\]+ = \\(float\\) \\1;" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump "(_\[0-9\]+) = __builtin_scalbn \\(x_\[0-9\]+\\(D\\), 8\\);\\s*_\[0-9\]+ = \\(double\\) \\1;" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump-not " \\* 1.28e\\+2" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump-not " \\* 2.56e\\+2" "scalbn_transform" } } */
/* { dg-final { cleanup-tree-dump "*" } } */
