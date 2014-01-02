/* Verify scalbn transform pass for bug 11424 case.  */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-all" } */
/* { dg-require-effective-target nds32_soft_fp } */

typedef float float32_t;
float32_t test_case (float32_t *pIn)
{
  float32_t in;
  in = *pIn++;
  in = (in * 128);
  in += in > 0.0f ? 0.5f : -0.5f;

  return in;
}

/* { dg-final { scan-tree-dump "(_\[0-9\]+) = __builtin_scalbnf \\(in_\[0-9\]+, 7\\);\\s*in_\[0-9\]+ = \\(float32_t\\) \\1;" "scalbn_transform" } } */
/* { dg-final { scan-tree-dump-not "in_\[0-9\]+ = in_\[0-9\]+ \\* 1.28e\\+2" "scalbn_transform" } } */
/* { dg-final { cleanup-tree-dump "*" } } */
