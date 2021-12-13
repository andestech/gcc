/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-O -mriscv-attribute -march=rv32ifd -mabi=ilp32 -mno-ext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_f2p0_d2p0\"" } } */
