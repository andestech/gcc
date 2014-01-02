/* Verify scalbn transform pass for negative number case.  */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-all" } */
/* { dg-require-effective-target nds32_soft_fp } */

double test_neg_scalbn (double x)
{
  return x * -8;
}

/* { dg-final { scan-tree-dump "(_\[0-9\]+) = __builtin_scalbn \\(x_\[0-9\]+\\(D\\), 3\\);\\s*_\[0-9\]+ = -\\1;" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump-not " \\* -8.0e\\+0" "scalbn_transform" } } */
/* { dg-final { cleanup-tree-dump "*" } } */
