/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-O -mriscv-attribute -march=rv32ifzfh -mabi=ilp32 -mno-zfh -mno-ext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_f2p0\"" } } */
