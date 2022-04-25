/* { dg-do compile } */
/* { dg-require-effective-target riscv64 } */
/* { dg-options "-march=rv64gc -mabi=lp64 -O -mno-ext-zbabcs" } */

/* Test for lshrsi3_zero_extend_3+2 pattern that uses
   high_mask_shift_operand.  */
unsigned long
sub1 (unsigned long i)
{
  return (i >> 32) << 32;
}

unsigned long
sub2 (unsigned long i)
{
  return (i >> 63) << 63;
}
/* { dg-final { scan-assembler-times "slli" 2 } } */
/* { dg-final { scan-assembler-times "srli" 2 } } */
