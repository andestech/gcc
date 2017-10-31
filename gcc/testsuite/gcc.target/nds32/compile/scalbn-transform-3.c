/* Verify scalbn transform pass for negative-exponent case.  */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-all" } */
/* { dg-require-effective-target nds32_soft_fp } */

double test_neg_exp_scalbnf (double x)
{
  return x * 0.0625;
}

/* { dg-final { scan-tree-dump "(_\[0-9\]+) = __builtin_scalbn \\(x_\[0-9\]+\\(D\\), -4\\);\\s*_\[0-9\]+ = \\(double\\) \\1;" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump-not " \\* 6.25e\\-2" "scalbn_transform" } } */
/* { dg-final { cleanup-tree-dump "*" } } */
