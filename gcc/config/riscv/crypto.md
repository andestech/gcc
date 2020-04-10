;; Machine description for K extension.
;; Copyright (C) 2022 Free Software Foundation, Inc.
;; Contributed by SiYu Wu (siyu@isrc.iscas.ac.cn) and ShiHua Liao (shihua@iscas.ac.cn).

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

(define_c_enum "unspec" [
    ;;ZBKB unspecs
    UNSPEC_ROR
    UNSPEC_RORW
    UNSPEC_ROL
    UNSPEC_ROLW
    UNSPEC_BREV8
    UNSPEC_BSWAP
    UNSPEC_ZIP
    UNSPEC_UNZIP
    
    ;; Zbkc unspecs
    UNSPEC_CLMUL
    UNSPEC_CLMULH

    ;; Zbkx unspecs
    UNSPEC_XPERM8
    UNSPEC_XPERM4

    ;; Zknd unspecs
    UNSPEC_AES_DSI
    UNSPEC_AES_DSMI
    UNSPEC_AES_DS
    UNSPEC_AES_DSM
    UNSPEC_AES_IM
    UNSPEC_AES_KS1I
    UNSPEC_AES_KS2

    ;; Zkne unspecs
    UNSPEC_AES_ES
    UNSPEC_AES_ESM
    UNSPEC_AES_ESI
    UNSPEC_AES_ESMI

    ;; Zknh unspecs
    UNSPEC_SHA_256_SIG0
    UNSPEC_SHA_256_SIG1
    UNSPEC_SHA_256_SUM0
    UNSPEC_SHA_256_SUM1
    UNSPEC_SHA_512_SIG0
    UNSPEC_SHA_512_SIG0H
    UNSPEC_SHA_512_SIG0L
    UNSPEC_SHA_512_SIG1
    UNSPEC_SHA_512_SIG1H
    UNSPEC_SHA_512_SIG1L
    UNSPEC_SHA_512_SUM0
    UNSPEC_SHA_512_SUM0R
    UNSPEC_SHA_512_SUM1
    UNSPEC_SHA_512_SUM1R

    ;; Zksh
    UNSPEC_SM3_P0
    UNSPEC_SM3_P1

    ;;Zksed
    UNSPEC_SM4_ED
    UNSPEC_SM4_KS
])

(define_insn "riscv_ror_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")]
                  UNSPEC_ROR))]
  "TARGET_ZBKB"
  "ror\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_rorw"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_RORW))]
  "TARGET_ZBKB && TARGET_64BIT"
  "rorw\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_rol_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")]
                  UNSPEC_ROL))]
  "TARGET_ZBKB"
  "rol\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_rolw"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_ROLW))]
  "TARGET_ZBKB && TARGET_64BIT"
  "rolw\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_brev8_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_BREV8))]
  "TARGET_ZBKB"
  "brev8\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_bswap<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_BSWAP))]
  "TARGET_ZBKB"
  "bswap\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_zip"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")]
                  UNSPEC_ZIP))]
  "TARGET_ZBKB && !TARGET_64BIT"
  "zip\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_unzip"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")]
                  UNSPEC_UNZIP))]
  "TARGET_ZBKB && !TARGET_64BIT"
  "unzip\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_clmul_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")]
                  UNSPEC_CLMUL))]
  "TARGET_ZBKC"
  "clmul\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_clmulh_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")]
                  UNSPEC_CLMULH))]
  "TARGET_ZBKC"
  "clmulh\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_xperm8_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")]
                  UNSPEC_XPERM8))]
  "TARGET_ZBKX"
  "xperm8\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_xperm4_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")]
                  UNSPEC_XPERM4))]
  "TARGET_ZBKX"
  "xperm4\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes32dsi"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")
                   (match_operand:SI 3 "bs_operand" "i")]
                   UNSPEC_AES_DSI))]
  "TARGET_ZKND && !TARGET_64BIT"
  "aes32dsi\t%0,%1,%2,%3"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes32dsmi"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")
                   (match_operand:SI 3 "bs_operand" "i")]
                   UNSPEC_AES_DSMI))]
  "TARGET_ZKND && !TARGET_64BIT"
  "aes32dsmi\t%0,%1,%2,%3"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes64ds"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")
                   (match_operand:DI 2 "register_operand" "r")]
                   UNSPEC_AES_DS))]
  "TARGET_ZKND && TARGET_64BIT"
  "aes64ds\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes64dsm"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")
                   (match_operand:DI 2 "register_operand" "r")]
                   UNSPEC_AES_DSM))]
  "TARGET_ZKND && TARGET_64BIT"
  "aes64dsm\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes64im"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")]
                   UNSPEC_AES_IM))]
  "TARGET_ZKND && TARGET_64BIT"
  "aes64im\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes64ks1i"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")
                   (match_operand:SI 2 "rnum_operand" "i")]
                   UNSPEC_AES_KS1I))]
  "(TARGET_ZKND || TARGET_ZKNE) && TARGET_64BIT"
  "aes64ks1i\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes64ks2"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")
                   (match_operand:DI 2 "register_operand" "r")]
                   UNSPEC_AES_KS2))]
  "(TARGET_ZKND || TARGET_ZKNE) && TARGET_64BIT"
  "aes64ks2\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes32esi"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")
                   (match_operand:SI 3 "bs_operand" "i")]
                   UNSPEC_AES_ESI))]
  "TARGET_ZKNE && !TARGET_64BIT"
  "aes32esi\t%0,%1,%2,%3"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes32esmi"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")
                   (match_operand:SI 3 "bs_operand" "i")]
                   UNSPEC_AES_ESMI))]
  "TARGET_ZKNE && !TARGET_64BIT"
  "aes32esmi\t%0,%1,%2,%3"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes64es"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")
                   (match_operand:DI 2 "register_operand" "r")]
                   UNSPEC_AES_ES))]
  "TARGET_ZKNE && TARGET_64BIT"
  "aes64es\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_aes64esm"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")
                   (match_operand:DI 2 "register_operand" "r")]
                   UNSPEC_AES_ESM))]
  "TARGET_ZKNE && TARGET_64BIT"
  "aes64esm\t%0,%1,%2"
  [(set_attr "type" "crypto")])

;; Zknh - SHA256

(define_insn "riscv_sha256sig0_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_SHA_256_SIG0))]
  "TARGET_ZKNH"
  "sha256sig0\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha256sig1_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_SHA_256_SIG1))]
  "TARGET_ZKNH"
  "sha256sig1\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha256sum0_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_SHA_256_SUM0))]
  "TARGET_ZKNH"
  "sha256sum0\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha256sum1_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_SHA_256_SUM1))]
  "TARGET_ZKNH"
  "sha256sum1\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sig0h"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_SHA_512_SIG0H))]
  "TARGET_ZKNH && !TARGET_64BIT"
  "sha512sig0h\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sig0l"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_SHA_512_SIG0L))]
  "TARGET_ZKNH && !TARGET_64BIT"
  "sha512sig0l\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sig1h"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_SHA_512_SIG1H))]
  "TARGET_ZKNH && !TARGET_64BIT"
  "sha512sig1h\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sig1l"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_SHA_512_SIG1L))]
  "TARGET_ZKNH && !TARGET_64BIT"
  "sha512sig1l\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sum0r"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_SHA_512_SUM0R))]
  "TARGET_ZKNH && !TARGET_64BIT"
  "sha512sum0r\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sum1r"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")
                   (match_operand:SI 2 "register_operand" "r")]
                   UNSPEC_SHA_512_SUM1R))]
  "TARGET_ZKNH && !TARGET_64BIT"
  "sha512sum1r\t%0,%1,%2"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sig0"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")]
                   UNSPEC_SHA_512_SIG0))]
  "TARGET_ZKNH && TARGET_64BIT"
  "sha512sig0\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sig1"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")]
                   UNSPEC_SHA_512_SIG1))]
  "TARGET_ZKNH && TARGET_64BIT"
  "sha512sig1\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sum0"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")]
                   UNSPEC_SHA_512_SUM0))]
  "TARGET_ZKNH && TARGET_64BIT"
  "sha512sum0\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sha512sum1"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec:DI [(match_operand:DI 1 "register_operand" "r")]
                   UNSPEC_SHA_512_SUM1))]
  "TARGET_ZKNH && TARGET_64BIT"
  "sha512sum1\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sm3p0_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_SM3_P0))]
  "TARGET_ZKSH"
  "sm3p0\t%0,%1"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sm3p1_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")]
                  UNSPEC_SM3_P1))]
  "TARGET_ZKSH"
  "sm3p1\t%0,%1"
  [(set_attr "type" "crypto")])

;; Zksed

(define_insn "riscv_sm4ed_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")
                  (match_operand:SI 3 "bs_operand" "i")]
                  UNSPEC_SM4_ED))]
  "TARGET_ZKSED"
  "sm4ed\t%0,%1,%2,%3"
  [(set_attr "type" "crypto")])

(define_insn "riscv_sm4ks_<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (unspec:X [(match_operand:X 1 "register_operand" "r")
                  (match_operand:X 2 "register_operand" "r")
                  (match_operand:SI 3 "bs_operand" "i")]
                  UNSPEC_SM4_KS))]
  "TARGET_ZKSED"
  "sm4ks\t%0,%1,%2,%3"
  [(set_attr "type" "crypto")])