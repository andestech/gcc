/* { dg-do compile { target *-*-linux* *-*-gnu* } } */
/* { dg-options "-fhardened -O -fdump-tree-gimple" } */
/* { dg-skip-if "" { *-*-* } { "-static" } { "" } } */

int
foo ()
{
  int i;
  return i;
}

/* { dg-final { scan-tree-dump ".DEFERRED_INIT" "gimple" } } */
