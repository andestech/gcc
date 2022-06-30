/* PR inline-asm/93202 */
/* { dg-do compile } */
/* { dg-options "-march=rv64i -mabi=lp64 -mcpu=n25" } */
/* { dg-error "unknown cpu" "" { target *-*-* } 0 } */

void
foo (void)
{
}
