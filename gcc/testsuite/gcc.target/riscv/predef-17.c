/* { dg-do compile } */
/* { dg-options "-march=rv32i -mext-vector -mabi=ilp32" } */
int main () {

#ifndef __riscv_arch_test
#error "__riscv_arch_test"
#endif

#if defined(__riscv_zfh)
#error "__riscv_zfh"
#endif

  return 0;
}

