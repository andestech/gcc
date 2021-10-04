/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O3 -mext-dsp" } */

unsigned long pkbbdi_2(unsigned long a, unsigned long b) {
  return (b << 32) | (a & 0x00000000ffffffffUL);
}

/* { dg-final { scan-assembler-times "pkbb32" 1 } } */
