/* { dg-do compile } */
void
bar ();
void __attribute__ ((interrupt, prestacked ("x1,x5-x7"))) foo (void) { bar (); }
/* Prestacked x1,x5,x6 and x7 even occur interrupt. */
/* { dg-final { scan-assembler-not "s\[wd\]\tra" } } */
/* { dg-final { scan-assembler-not "l\[wd\]\tt0" } } */
/* { dg-final { scan-assembler-not "s\[wd\]\tt1" } } */
/* { dg-final { scan-assembler-not "l\[wd\]\tt2" } } */
