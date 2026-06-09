/* { dg-do compile } */
/* { dg-options "-march=rv64ifd -mabi=lp64d -O" } */

_Float16 gvar = 9.87654;
float fa = 2.5;
_Float16 hfa;

union U {
  unsigned short i16;
  _Float16 f16;
};

_Float16 test1(unsigned short input)
{
  union U tmp;
  tmp.i16 = input;

  return tmp.f16;
}

_Float16 test2()
{
  return 1.234f;
}

_Float16 test3()
{
  return gvar;
}

_Float16 test()
{
  return 0.0f;
}

void
test4 ()
{
  asm ("fcvt.h.s\t%0,%1":"=f"(fa):"f"(hfa)) ;
  return;
}

/* { dg-final { scan-assembler-times "li\[ \t\]" 5 } } */
/* { dg-final { scan-assembler-times "fmv\.w\.x\[ \t\]" 5 } } */

