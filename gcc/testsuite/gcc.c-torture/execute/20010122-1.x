# This test relies on __builtin_return_address(1) returning something
# useful or NULL.  This is not guaranteed to be be so, especially when 
# -fomit-frame-pointer is used.  So do not test with it.

set torture_eval_before_compile {
  if {[string match {*-fomit-frame-pointer*} "$option"]} {
    continue
  }
}

# Please see Andes Bugzilla #10942 for the details.
if { [istarget "nds32*-*-*"] } {
  # The __builtin_return_address(1) on nds32 target is able to
  # return something useful as long as we always save $lp register
  # in the stack.
  set additional_flags "-malways-save-lp"
}

return 0
