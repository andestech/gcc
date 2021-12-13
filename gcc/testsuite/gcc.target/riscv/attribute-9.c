/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-O -mriscv-attribute -march=rv32i2p0sabc_xbar -mabi=ilp32e -mno-ext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_sabc2p0_xbar2p0\"" } } */
