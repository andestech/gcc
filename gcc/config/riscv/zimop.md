(define_insn "riscv_mopr_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec:GPR [(match_operand:GPR 1 "register_operand" "r")
                     (match_operand:GPR 2 "const_int_operand" "n")]
                     UNSPEC_MOPR))]
  "TARGET_ZIMOP"
  "mop.r.%2\t%0,%1"
  [(set_attr "type" "zimop")])

(define_insn "riscv_moprr_<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec:GPR [(match_operand:GPR 1 "register_operand" "r")
                     (match_operand:GPR 2 "register_operand" "r")
                     (match_operand:GPR 3 "const_int_operand" "n")]
                     UNSPEC_MOPRR))]
  "TARGET_ZIMOP"
  "mop.rr.%3\t%0,%1,%2"
  [(set_attr "type" "zimop")])
