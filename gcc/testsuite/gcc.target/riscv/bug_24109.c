/* This is a test program for wexti instruction */
/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp -O3" } */
/* { dg-final { scan-assembler "wexti" } } */

int foo(long long int a) {
  long long int x = a >> 20;
  int y = (int) x;
  return y;
}
