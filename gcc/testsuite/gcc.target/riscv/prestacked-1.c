/* Verify proper errors are generated for invalid code.  */
void __attribute__ ((prestacked)) sub0 (void)
{ /* { dg-error "wrong number of arguments specified" } */
}

void __attribute__ ((prestacked ("x1"), naked)) sub1 (void)
{ /* { dg-error "are mutually exclusive" } */
}

void __attribute__ ((prestacked ("invalid"))) sub2 (void)
{ /* { dg-warning "is illegal" } */
}

void __attribute__ ((prestacked ("x1-x2"))) sub3 (void)
{ /* { dg-warning "register range must span at least 3 consecutive registers" } */
}

void __attribute__ ((prestacked ("x3-x1"))) sub4 (void)
{ /* { dg-warning "incremental values" } */
}

void __attribute__ ((prestacked ("x1-v1"))) sub5 (void)
{ /* { dg-warning "register range must same register class" } */
}

void __attribute__ ((prestacked (""))) sub6 (void)
{ /* { dg-warning "no whitespaces" } */
}
