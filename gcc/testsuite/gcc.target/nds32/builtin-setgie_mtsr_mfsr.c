/* This is a test program for checking gie with
   mtsr/mfsr instruction.  */

/* { dg-do run } */
/* { dg-options "-O0" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int psw;
  unsigned int gie;
  unsigned int pfm_ctl;
  unsigned int real_psw;

  /* Keep PSW value.  */
  real_psw = __nds32__mfsr (NDS32_SR_PSW);

  __nds32__setgie_en ();
  __nds32__dsb(); /* This is needed for waiting pipeline.  */
  psw = __nds32__mfsr (NDS32_SR_PSW);

  gie = psw & 0x00000001;

  if (gie != 1)
    abort ();

  psw = psw & 0xFFFFFFFE;
  __nds32__mtsr (psw, NDS32_SR_PSW);
  __nds32__dsb(); /* This is needed for waiting pipeline.  */
  psw = __nds32__mfsr (NDS32_SR_PSW);
  gie = psw & 0x00000001;

  /* Recover PSW value.  */
  __nds32__mtsr (real_psw, NDS32_SR_PSW);

  if (gie != 0)
    abort ();
  else
   exit (0);
}
