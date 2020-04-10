/* { dg-do compile } */
/* { dg-options "-O -mriscv-attribute -march=rv32e1p9 -mabi=ilp32e -mno-ext-dsp -mno-ext-zbabcs" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32e1p9\"" } } */
