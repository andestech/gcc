/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O3 -mext-dsp" } */

unsigned long pkbbdi_4(unsigned long a, unsigned int b) {
  return (a << 32) | b;
}

/* { dg-final { scan-assembler-times "pkbb32" 1 } } */
