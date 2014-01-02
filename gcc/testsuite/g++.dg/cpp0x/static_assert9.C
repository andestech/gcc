// PR c++/58837
// { dg-require-effective-target c++11 }
// { dg-options "-fdelete-null-pointer-checks" }

void f();
static_assert(f, "");
struct A {};
static_assert(A::~A, "");      // { dg-error "non-static member function" }
