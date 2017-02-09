/* { dg-do run { target { riscv_zfh_hw } } } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-O -mzfh" } */

/* Conversion of infinity to __fp16 and back again should preserve the
   value.  */
volatile float f = __builtin_inf ();

int main ()
{
  __fp16 h = f;
  if (h != __builtin_inf ())
    __builtin_abort ();
  return 0;
}
