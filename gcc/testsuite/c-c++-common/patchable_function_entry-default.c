/* { dg-do compile { target { ! { nvptx*-*-* visium-*-* } } } } */
/* { dg-options "-O2 -fpatchable-function-entry=3,1" } */
/* { dg-additional-options "-fno-pie" { target sparc*-*-* } } */
/* See PR99888, one single preceding nop isn't allowed on powerpc_elfv2,
   so overriding with two preceding nops to make it pass there.  */
/* { dg-additional-options "-fpatchable-function-entry=3,2" { target powerpc_elfv2 } } */
/* { dg-final { scan-assembler-times "\tnop|\tNOP|\tSWYM|\t.2byte 0x0001|\t.4byte 0x00000013" 3 { target { ! { alpha*-*-* } } } } } */
/* { dg-final { scan-assembler-times "bis" 3 { target alpha*-*-* } } } */

extern int a;

/* Nothing declared must not mean anything.  */
int f3 (void);

/* F3 should get a default-sized NOP area.  */
int
__attribute__((noinline))
f3 (void)
{
  return 5*a;
}
