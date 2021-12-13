/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-O -mriscv-attribute -march=rv32i2p0 -mabi=ilp32 -mext-vector -mno-ext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_v1p0\"" } } */
