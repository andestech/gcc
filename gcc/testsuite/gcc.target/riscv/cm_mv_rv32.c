/* { dg-do compile } */
/* { dg-options " -Os -march=rv32i_zca_zcmp -mabi=ilp32 " } */
/* { dg-skip-if "" { *-*-* } {"-O0" "-O1" "-O2" "-Og" "-O3" "-Oz" "-flto"} } */
/* { dg-final { check-function-bodies "**" "" } } */

int
func (int a, int b);

/*
**sum:
**	...
**	mv	s1,a0
**	sw	a1,12\(sp\)
**	call	func
**  lw	a1,12\(sp\)
**	mv	s0,a0
**	mv	a0,s1
**	call	func
**	...
*/
int
sum (int a, int b)
{
  return func (a, b) + func (a, b);
}
