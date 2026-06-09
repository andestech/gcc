/* { dg-do compile } */
/* { dg-options "-march=rv64if_xandesbf -mabi=lp64f -O" } */

int foo1 (__bf16 a, __bf16 b)
{
    /* { dg-final { scan-assembler-times {\mfgt\.h\M} 1 } } */
    return a > b;
}
