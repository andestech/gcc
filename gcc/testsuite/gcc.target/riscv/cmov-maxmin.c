/* { dg-do compile } */
/* { dg-options "-O1 -funroll-loops -mtune=andes-45-series" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** max:
** blt a1, a0, 0f
** add a0, a1, zero\n0:
** ret
*/

long max(long x, long y) {
  return x > y ? x : y;
}

/*
** min:
** bgt a1, a0, 0f
** add a0, a1, zero\n0:
** ret
*/

long min(long x, long y) {
  return x < y ? x : y;
}
