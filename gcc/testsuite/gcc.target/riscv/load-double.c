/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-march=rv32gc -mabi=ilp32d -O -mcmodel=medlow -mstrict-align" }
 */

/* Check that we merge %lo into offset of fld */
double
foo ()
{
  return .1;
}

/* { dg-final { scan-assembler "fld\tfa\[0-9\],%lo" } } */
