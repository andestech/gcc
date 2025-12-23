/* { dg-options " -mext-zbabcs -fno-inline -mcmov " } */
/* { dg-do run } */

extern void abort (void);
extern void exit (int);
int lala = 0xff;
int bbcs (int a, int mask)
{
  lala++;
  if (a & 0x100)
    lala = lala | ~mask;
  else
    lala = lala & ~mask;
  return lala;
}

int main ()
{
  if (bbcs (0, 0xffffff00) != 0)
    abort ();
   exit (0);
}
