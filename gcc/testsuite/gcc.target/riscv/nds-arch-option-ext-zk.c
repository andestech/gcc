/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32i -mabi=ilp32 -mext-zk" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \".*_zbkb1p0_zbkc1p0_zbkx1p0_zk1p0_zkn1p0_zknd1p0_zkne1p0_zknh1p0_zkr1p0_zkt1p0\"" } } */
