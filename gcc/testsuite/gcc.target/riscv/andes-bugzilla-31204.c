/* { dg-do compile } */
/* { dg-options "-march=rv64i -mabi=lp64" } */
__attribute__ ((prestacked ("x1,x5-x7,x10-x17,x28-x31"))) void
isr (void)
{}
