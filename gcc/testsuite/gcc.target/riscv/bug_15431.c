/* { dg-do run } */
/* { dg-options "-O2" } */

unsigned int op1 = 0x30e06c5;

unsigned int
bar (unsigned int op1)
{
  return ((op1 << 31) % 26);
}

int
main ()
{
  unsigned int foo = 0;

  foo = bar (op1);

  if (foo != 0x18u)
    __builtin_abort();

  return 0;
}
