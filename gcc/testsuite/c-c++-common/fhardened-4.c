/* { dg-do compile { target *-*-linux* *-*-gnu* } } */
/* { dg-options "-fhardened -O0 -Wno-hardened" } */
/* { dg-skip-if "" { *-*-* } { "-static" } { "" } } */

/* { dg-bogus "._FORTIFY_SOURCE. is not enabled" "" { target *-*-* } 0 } */
