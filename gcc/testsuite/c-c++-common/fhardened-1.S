/* { dg-do preprocess { target { { *-*-linux* *-*-gnu* } && pie } } } */
/* { dg-options "-fhardened -O" } */
/* { dg-skip-if "" { *-*-* } { "-static" } { "" } } */

#if __PIE__ != 2
# error "-fPIE not enabled"
#endif
