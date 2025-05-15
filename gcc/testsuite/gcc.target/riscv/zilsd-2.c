/* { dg-do compile } */
/* { dg-require-effective-target rv32 } */
/* { dg-options "-O1 -fno-optimize-sibling-calls -march=rv32i_zilsd -mabi=ilp32" } */

typedef struct
{
  short x;
  char y[6];
} pack;

extern void bar (pack, pack);

void foo (pack a, pack b)
{
  bar (b, a);
}

/* { dg-final { scan-assembler {ld\s+a[0,2,4,6],\s*[0-9]+\(..\)} } } */
/* { dg-final { scan-assembler {sd\s+a[0,2,4,6],\s*[0-9]+\(..\)} } } */
