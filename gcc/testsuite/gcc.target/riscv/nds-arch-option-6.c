/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32if -mabi=ilp32f -mext-vector -mno-ext-dsp -mno-ext-zbabcs" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_f2p0_zfh1p0_zve32f1p0_zve32x1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0\"" } } */
