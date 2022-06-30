/* PR inline-asm/93202 */
/* { dg-do compile } */
/* { dg-options "-march=rv32i -mabi=ilp32 -mcpu=nx25" } */
/* { dg-error "unknown cpu" "" { target *-*-* } 0 } */

void
foo (void)
{
}
