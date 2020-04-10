/* { dg-do compile { target { riscv32*-*-elf } } } */
/* { dg-options "-march=rv32e -mabi=ilp32e -O1" } */

/* The alignment of long long type is 8 for RV32E,
   and the alignment of struct is the maximum alignment
   of its members, so the alignment of struct s1 should be 8. */
struct s1 { char c; long long n; };

int main ()
{
  struct s1 s;
  return _Alignof (s);
}
/* { dg-final { scan-assembler "4" } } */
