/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zfh_xandesbf -mabi=lp64d -O3 -mbf16mscsr" } */

typedef _Float16 float16;

void
foo ();

float16
fp16_add (float16 op1, float16 op2)
{
  foo ();
  return op1 + op2;
}

__bf16
bf16_add (__bf16 op1, __bf16 op2)
{
  foo ();
  return op1 + op2;
}

/* { dg-final { scan-assembler-times {csrrsi\s+zero+,\sumisc_ctl+,\s1} 2 } } */
/* { dg-final { scan-assembler-times {csrrci\s+zero+,\sumisc_ctl+,\s1} 2 } } */