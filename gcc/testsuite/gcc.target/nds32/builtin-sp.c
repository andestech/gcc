/* This is a test program for sp intrinsic usage.
   Because we want to use frame pointer to access local variable,
   we need to use -fno-omit-frame-pointer to make sure the existence
   of frame pointer.  */

/* { dg-do run } */
/* { dg-options "-O0 -fno-omit-frame-pointer" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int old_sp, new_sp;

  old_sp = __nds32__get_current_sp ();
  new_sp = old_sp - 4;
  __nds32__set_current_sp (new_sp);
  new_sp = __nds32__get_current_sp ();

  if (new_sp != (old_sp - 4))
    abort ();

  new_sp = new_sp + 4;
  __nds32__set_current_sp (new_sp);
  new_sp = __nds32__get_current_sp ();

  if (new_sp != old_sp)
    abort ();
  else
    exit (0);
}
