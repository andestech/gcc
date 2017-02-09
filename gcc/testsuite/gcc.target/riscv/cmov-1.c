/* { dg-do compile } */
/* { dg-options "-O2 -mcmov" } */

long
foo (double d1, double d2, long *l)
{
  return (d1 <= d2) * (*l--);
}
