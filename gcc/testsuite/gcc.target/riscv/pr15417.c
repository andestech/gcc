/* { dg-do run } */
/* { dg-options "-O2" } */

unsigned long long
foo(unsigned long long bar) {
  unsigned long long ret = bar;
  ret +=  0xf2 & ( 0xbafc * bar);
  return ret;
}

int main() {
  int i = 0;
  unsigned long long op0 = 0x22a67f49fa8ba839llu;

  for (i = 0; i < 7; ++i) {
    op0 = foo(op0);
  }

  if (op0 != 0x22a67f49fa8bab69llu)
    __builtin_abort();

  return 0;
}
