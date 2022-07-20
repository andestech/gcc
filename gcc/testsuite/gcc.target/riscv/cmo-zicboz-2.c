/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-march=rv32gc_zicboz -mabi=ilp32" } */

int var;

void foo1()
{
    __builtin_riscv_zicboz_cbo_zero(0);
    __builtin_riscv_zicboz_cbo_zero(&var);
    __builtin_riscv_zicboz_cbo_zero((void*)0x121);
}

/* { dg-final { scan-assembler-times "cbo.zero" 3 } } */ 
