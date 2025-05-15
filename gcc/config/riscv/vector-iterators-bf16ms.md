;; Iterators for RISC-V 'V' Extension for GNU compiler.
;; Copyright (C) 2022-2024 Free Software Foundation, Inc.
;; Contributed by Juzhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Above message is copy from vector-iterators.md and the file used for BF16 mode switch.

(define_mode_iterator VBF16 [
  (RVVM8BF "(TARGET_ZVFBFMIN || TARGET_BF16 || TARGET_BF16MS)")
  (RVVM4BF "(TARGET_ZVFBFMIN || TARGET_BF16 || TARGET_BF16MS)")
  (RVVM2BF "(TARGET_ZVFBFMIN || TARGET_BF16 || TARGET_BF16MS)")
  (RVVM1BF "(TARGET_ZVFBFMIN || TARGET_BF16 || TARGET_BF16MS)")
  (RVVMF2BF "(TARGET_ZVFBFMIN || TARGET_BF16 || TARGET_BF16MS)")
  (RVVMF4BF "(TARGET_ZVFBFMIN || TARGET_BF16 || TARGET_BF16MS) && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VWEXTF_XANDESBF [
  (RVVM8SF "TARGET_BF16MS && TARGET_VECTOR_ELEN_FP_32")
  (RVVM4SF "TARGET_BF16MS && TARGET_VECTOR_ELEN_FP_32")
  (RVVM2SF "TARGET_BF16MS && TARGET_VECTOR_ELEN_FP_32")
  (RVVM1SF "TARGET_BF16MS && TARGET_VECTOR_ELEN_FP_32")
  (RVVMF2SF "TARGET_BF16MS && TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
])

(define_mode_attr VCONVERT_BF [
  (RVVM8BF "RVVM8HI") (RVVM4BF "RVVM4HI") (RVVM2BF "RVVM2HI") (RVVM1BF "RVVM1HI") (RVVMF2BF "RVVMF2HI") (RVVMF4BF "RVVMF4HI")
])

;; Conflict VNCONVERT of attribute and VWCONVERTI of iterator between HF and BF mode, so create VWCONVERTI_BF and VNCONVERT_BF.
(define_mode_attr VNCONVERT_BF [
  (RVVM8BF "RVVM4QI") (RVVM4BF "RVVM2QI") (RVVM2BF "RVVM1QI") (RVVM1BF "RVVMF2QI") (RVVMF2BF "RVVMF4QI") (RVVMF4BF "RVVMF8QI")

  (RVVM8SI "RVVM4BF") (RVVM4SI "RVVM2BF") (RVVM2SI "RVVM1BF") (RVVM1SI "RVVMF2BF") (RVVMF2SI "RVVMF4BF")

  (V1SI "V1BF")
  (V2SI "V2BF")
  (V4SI "V4BF")
  (V8SI "V8BF")
  (V16SI "V16BF")
  (V32SI "V32BF")
  (V64SI "V64BF")
  (V128SI "V128BF")
  (V256SI "V256BF")
  (V512SI "V512BF")
  (V1024SI "V1024BF")

  (V1BF "V1QI")
  (V2BF "V2QI")
  (V4BF "V4QI")
  (V8BF "V8QI")
  (V16BF "V16QI")
  (V32BF "V32QI")
  (V64BF "V64QI")
  (V128BF "V128QI")
  (V256BF "V256QI")
  (V512BF "V512QI")
  (V1024BF "V1024QI")
  (V2048BF "V2048QI")
])

(define_mode_attr vnconvert_bf [
  (RVVM8BF "rvvm4qi") (RVVM4BF "rvvm2qi") (RVVM2BF "rvvm1qi") (RVVM1BF "rvvmf2qi") (RVVMF2BF "rvvmf4qi") (RVVMF4BF "rvvmf8qi")

  (RVVM8SI "rvvm4bf") (RVVM4SI "rvvm2bf") (RVVM2SI "rvvm1bf") (RVVM1SI "rvvmf2bf") (RVVMF2SI "rvvmf4bf")

  (V1SI "v1bf")
  (V2SI "v2bf")
  (V4SI "v4bf")
  (V8SI "v8bf")
  (V16SI "v16bf")
  (V32SI "v32bf")
  (V64SI "v64bf")
  (V128SI "v128bf")
  (V256SI "v256bf")
  (V512SI "v512bf")
  (V1024SI "v1024bf")

  (V1BF "v1qi")
  (V2BF "v2qi")
  (V4BF "v4qi")
  (V8BF "v8qi")
  (V16BF "v16qi")
  (V32BF "v32qi")
  (V64BF "v64qi")
  (V128BF "v128qi")
  (V256BF "v256qi")
  (V512BF "v512qi")
  (V1024BF "v1024qi")
  (V2048BF "v2048qi")
])

(define_mode_iterator VWCONVERTI_BF [
  (RVVM8SI "TARGET_BF16MS") (RVVM4SI "TARGET_BF16MS") (RVVM2SI "TARGET_BF16MS") (RVVM1SI "TARGET_BF16MS")
  (RVVMF2SI "TARGET_BF16MS")

  (V1SI "riscv_vector::vls_mode_valid_p (V1SImode) && TARGET_BF16MS")
  (V2SI "riscv_vector::vls_mode_valid_p (V2SImode) && TARGET_BF16MS")
  (V4SI "riscv_vector::vls_mode_valid_p (V4SImode) && TARGET_BF16MS")
  (V8SI "riscv_vector::vls_mode_valid_p (V8SImode) && TARGET_BF16MS")
  (V16SI "riscv_vector::vls_mode_valid_p (V16SImode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 64")
  (V32SI "riscv_vector::vls_mode_valid_p (V32SImode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 128")
  (V64SI "riscv_vector::vls_mode_valid_p (V64SImode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 256")
  (V128SI "riscv_vector::vls_mode_valid_p (V128SImode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 512")
  (V256SI "riscv_vector::vls_mode_valid_p (V256SImode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 1024")
  (V512SI "riscv_vector::vls_mode_valid_p (V512SImode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 2048")
  (V1024SI "riscv_vector::vls_mode_valid_p (V1024SImode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 4096")
])

(define_mode_iterator VLSF_BF16 [
  (V1BF "riscv_vector::vls_mode_valid_p (V1BFmode) && TARGET_BF16MS")
  (V2BF "riscv_vector::vls_mode_valid_p (V2BFmode) && TARGET_BF16MS")
  (V4BF "riscv_vector::vls_mode_valid_p (V4BFmode) && TARGET_BF16MS")
  (V8BF "riscv_vector::vls_mode_valid_p (V8BFmode) && TARGET_BF16MS")
  (V16BF "riscv_vector::vls_mode_valid_p (V16BFmode) && TARGET_BF16MS")
  (V32BF "riscv_vector::vls_mode_valid_p (V32BFmode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 64")
  (V64BF "riscv_vector::vls_mode_valid_p (V64BFmode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 128")
  (V128BF "riscv_vector::vls_mode_valid_p (V128BFmode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 256")
  (V256BF "riscv_vector::vls_mode_valid_p (V256BFmode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 512")
  (V512BF "riscv_vector::vls_mode_valid_p (V512BFmode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 1024")
  (V1024BF "riscv_vector::vls_mode_valid_p (V1024BFmode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 2048")
  (V2048BF "riscv_vector::vls_mode_valid_p (V2048BFmode) && TARGET_BF16MS && TARGET_MIN_VLEN >= 4096")
])

(define_mode_iterator V_VLSBF [VBF16 VLSF_BF16])

(define_mode_attr v_double_trunc_bf [
  (RVVM8SF "rvvm4bf") (RVVM4SF "rvvm2bf") (RVVM2SF "rvvm1bf") (RVVM1SF "rvvmf2bf") (RVVMF2SF "rvvmf4bf")])
