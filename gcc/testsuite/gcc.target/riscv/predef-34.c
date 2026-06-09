/* { dg-do compile } */
/* { dg-options "-march=rv64ic_zilsd_zclsd -mabi=lp64" } */

int main () {

#ifndef __riscv_arch_test
#error "__riscv_arch_test"
#endif

#if __riscv_xlen != 64
#error "__riscv_xlen"
#endif

#if !defined(__riscv_i)
#error "__riscv_i"
#endif

#if !defined(__riscv_zca)
#error "__riscv_zca"
#endif

#if !defined(__riscv_zilsd)
#error "__riscv_zilsd"
#endif

#if !defined(__riscv_zclsd)
#error "__riscv_zclsd"
#endif

  return 0;
}
