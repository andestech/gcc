/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc -mabi=lp64 -mno-ext-zc" } */
int
sub (unsigned int i, unsigned int j, unsigned int k, int *array)
{
  return array[i] + array[j] + array[k];
}
/* { dg-final { scan-assembler-times "slli" 3 } } */
