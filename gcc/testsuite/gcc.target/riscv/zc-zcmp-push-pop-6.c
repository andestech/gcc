/* { dg-do compile } */
/* { dg-options "-march=rv32g_zca_zcmp -mabi=ilp32d -O0 -fomit-frame-pointer" } */

void foo2(char*);

void __attribute__ ((interrupt ("user")))
foo (void)
{
  char buf[560] = {0};
  foo2(buf+1);
}

/* { dg-final { scan-assembler "cm.push\t{ra},-64" } } */
/* { dg-final { scan-assembler "cm.pop\t{ra},64" } } */
