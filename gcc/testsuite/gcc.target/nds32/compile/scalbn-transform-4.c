/* Verify scalbn transform pass for cases that can't be optimized.  */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-all" } */
/* { dg-require-effective-target nds32_soft_fp } */

#include "math.h"

double test_filter_condition_1 (double x)
{
  return x * 0;
}

double test_filter_condition_2 (double x)
{
  return x * -0;
}

double test_filter_condition_3 (double x)
{
  return x * 485;
}

double test_filter_condition_4 (double x)
{
  return x * -85;
}

double test_filter_condition_5 (double x)
{
  return x * 0.12;
}

double test_filter_condition_6 (double x)
{
  return x * -INFINITY;
}

double test_filter_condition_7 (double x)
{
  return x * NAN;
}

/* { dg-final { scan-tree-dump-times "x_\[0-9\]+\\(D\\) \\* 0.0" 2 "scalbn_transform" } } */
/* { dg-final { scan-tree-dump " \\* 4.85e\\+2" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump " \\* -8.5e\\+1" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump " \\* 1.19999" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump " \\*  -Inf" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump " \\*  Nan" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump-not "__builtin_scalbn" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump-times "No multiplication stmt is transformed" 7  "scalbn_transform" } } */
/* { dg-final { cleanup-tree-dump "*" } } */
