/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv64gc -mabi=lp64 -mext-zc" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \".*zba1p0_zbb1p0_zbc1p0_zbs1p0_zca1p0_zcb1p0_zcmp1p0_zcmt1p0\"" } } */
