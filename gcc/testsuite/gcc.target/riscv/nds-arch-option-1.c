/* { dg-do compile } */
/* { dg-options "-O -mriscv-attribute -march=rv32i2p0 -mabi=ilp32 -mext-vector -mno-ext-dsp -mno-ext-zbabcs" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_zve32x1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0\"" } } */
