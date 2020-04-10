/* { dg-do compile } */
/* { dg-options "-march=rv64i_zbkb_zbkc_zbkx_zknd_zkne_zknh_zksed_zksh -mabi=lp64 -mcmodel=medlow" } */

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

#if !defined(__riscv_zbkb)
#error "__riscv_zbkb"
#endif

#if !defined(__riscv_zbkc)
#error "__riscv_zbkc"
#endif

#if !defined(__riscv_zbkx)
#error "__riscv_zbkx"
#endif

#if !defined(__riscv_zknd)
#error "__riscv_zknd"
#endif

#if !defined(__riscv_zkne)
#error "__riscv_zkne"
#endif

#if !defined(__riscv_zknh)
#error "__riscv_zknh"
#endif

#if !defined(__riscv_zksh)
#error "__riscv_zksh"
#endif

  return 0;
}
