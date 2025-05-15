/* { dg-do compile { target { { *-*-linux* *-*-gnu* } && pie } } } */
/* { dg-options "-fhardened -O -fno-PIE" } */
/* { dg-skip-if "" { *-*-* } { "-static" } { "" } } */

#ifdef __PIE__
# error "PIE enabled when it should not be"
#endif
