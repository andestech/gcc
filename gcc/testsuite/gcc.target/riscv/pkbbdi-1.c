/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O3 -mext-dsp" } */

unsigned long pkbbdi_1(unsigned long a, unsigned long b) {
  return (a & 0x00000000ffffffffUL) | (b << 32);
}

/* { dg-final { scan-assembler-times "pkbb32" 1 } } */
