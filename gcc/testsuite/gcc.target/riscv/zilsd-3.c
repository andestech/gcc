/* { dg-do compile } */
/* { dg-require-effective-target rv32 } */
/* { dg-options "-O1 -march=rv32i_zilsd -mabi=ilp32" } */

/* Check that an odd-even register pair is not used due to ABI handling. */

extern void bar(void *);
void foo(int dummy, unsigned long long src)
{
    bar(&src);
}

/* Test the rtl pattern load_multiple/store_multiple. */

struct {
  char x[5];
} dummy = {{1, 2, 3, 4, 5}};

extern void barbar(int);

int vargs(int fix, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, fix);
  __builtin_va_end(ap);
  return 0;
}

void foofoo(void) {
  barbar(vargs(0, dummy));
}
