/* { dg-do compile } */
/* { dg-options "-march=rv32imafd_zicsr_zifencei -mabi=ilp32d" } */
/* { dg-skip-if "" { riscv64*-*-* } { "*" } } */
/* { dg-skip-if "" { riscv32*-*-* } { "*" } { "-Os" "-Og" "-Oz" } } */
#include <stdint.h>

int main(double x, double y){

  union
  {
    double f;
    uint64_t i;
  } u1 = {x + y};

  union
  {
    double f;
    uint64_t i;
  } u2 = {y};

  return (u1.i >> 20) >> (u2.i);
}

/* { dg-final { scan-assembler-not {\tfsd\s+f[a-z][0-9]\,8\(sp\)\n\tlw\s+[a-z][0-9]\,12\(sp\)} } } */
