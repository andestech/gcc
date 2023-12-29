/* { dg-do compile } */
/* { dg-options "-march=rv64g_zca_zcmp -mabi=lp64d -O0 -fomit-frame-pointer" } */

int foo1(int a)
{
    return a;
}

int foo2(int b)
{
    return foo1(b);
}

/* { dg-final { scan-assembler "cm.push\t{ra},-32" } } */
/* { dg-final { scan-assembler "cm.popret\t{ra},32" } } */
