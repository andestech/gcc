/* { dg-do compile } */
/* { dg-options "-O2 -mcmov" } */

void *
test1 (void *base, int sp, unsigned size)
{
  void *ret;
  for (int i = 0; i < 9; i++)
    {
      ret = (void *) ((unsigned) base + size);
      size = 180 * (sp == 44100 ? 48000 : sp);
    }
  return ret;
}

void *
test2 (void *base, int sp, unsigned size)
{
  void *ret;
  for (int i = 0; i < 9; i++)
    {
      ret = (void *) ((unsigned) base + size);
      size = 180 * (sp != 44100 ? 48000 : sp);
    }
  return ret;
}
