/* { dg-do compile } */
/* Verify that store mapping are compatible with Table A.6 & A.7.  */
/* { dg-options "-O3 -matomic" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** foo:
**	lw\ta[0-9]+,0\(a1\)
**	fence\trw,w
**	sw\ta[0-9]+,0\(a0\)
**	fence\trw,rw
**	ret
*/
void foo (int* bar, int* baz)
{
  __atomic_store(bar, baz, __ATOMIC_SEQ_CST);
}
