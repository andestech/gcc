/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O3 -mext-dsp" } */

unsigned long pkbbdi_3(unsigned int a, unsigned long b) {
  return a | (b << 32);
}

/* { dg-final { scan-assembler-times "pkbb32" 1 } } */
