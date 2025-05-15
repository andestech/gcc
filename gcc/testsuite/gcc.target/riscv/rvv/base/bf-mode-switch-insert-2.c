/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zfh_xandesbf -mabi=lp64d -O3 -mbf16mscsr" } */

typedef _Float16 float16;

float
foo (__bf16 op1, __bf16 op2, float16 c, float16 d)
{
  float res1 = op1 + op2;
  float res2 = c + d;
  return res1 + res2;
}

/* { dg-final { scan-assembler-times {csrrsi\s+zero+,\sumisc_ctl+,\s1} 2 } } */
/* { dg-final { scan-assembler-times {csrrci\s+zero+,\sumisc_ctl+,\s1} 2 } } */