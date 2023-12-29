/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d" } */
/* { dg-skip-if "" { *32-*-linux* } { "*" } } */

#include "riscv_vector.h"

vint32m1_t*
fun (vint32m1_t* a) {  return a; }  /* { dg-bogus "the vector type" } */

void
bar ()
{
  vint32m1_t a;
  fun (&a);
}
