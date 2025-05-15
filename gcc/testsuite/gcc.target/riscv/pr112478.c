/* { dg-do compile } */
/* { dg-options "-ffat-lto-objects -mno-save-restore" } */

void foo() {
    asm volatile("# " : ::"ra");
}

/* { dg-final { scan-assembler "s(w|d)\[ \t\]*ra" } } */
