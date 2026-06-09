/* Test that we do not use QI vector to initilize the memory if the
 * size of QI vector isn't larger than UNITS_PER_WORD */
/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -O3" } */

struct s {
  char* a;
  char* b;
  char* c;
};

void q(struct s*);

void g() {
  struct s r = { 0 };
  q(&r);
}

/* { dg-final { scan-assembler-times {sw\tzero,4\(sp\)} 1 } } */
/* { dg-final { scan-assembler-times {sw\tzero,8\(sp\)} 1 } } */
/* { dg-final { scan-assembler-times {sw\tzero,12\(sp\)} 1 } } */
