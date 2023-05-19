//	riscv_crypto_scalar.h
//	2021-11-08	Markku-Juhani O. Saarinen <mjos@pqshield.com>
//	Copyright (c) 2021, PQShield Ltd. All rights reserved.

//	=== Scalar crypto: General mapping from intrinsics to compiler builtins,
//		inline assembler, or to an (insecure) porting / emulation layer.

/*
 *	_rv_*(...)
 *	  RV32/64 intrinsics that return the "long" data type
 *
 *	_rv32_*(...)
 *	  RV32/64 intrinsics that return the "int32_t" data type
 *
 *	_rv64_*(...)
 *	  RV64-only intrinsics that return the "int64_t" data type
 *
 */

#ifndef _RISCV_CRYPTO_SCALAR_H
#define _RISCV_CRYPTO_SCALAR_H

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__riscv_xlen) && !defined(RVKINTRIN_EMULATE)
#warning "Target is not RISC-V. Enabling insecure emulation."
#define RVKINTRIN_EMULATE 1
#endif

#if defined(RVKINTRIN_EMULATE)

//	intrinsics via emulation (insecure -- porting / debug option)
#include "rvk_emu_intrin.h"
#define _RVK_INTRIN_IMPL(s) _rvk_emu_##s

#elif defined(RVKINTRIN_ASSEMBLER)

//	intrinsics via inline assembler (builtins not available)
#include "rvk_asm_intrin.h"
#define _RVK_INTRIN_IMPL(s) _rvk_asm_##s
#else

//	intrinsics via compiler builtins
#include <stdint.h>
#define _RVK_INTRIN_IMPL(s) __builtin_riscv_##s

#endif

//	set type if not already set
#if !defined(RVKINTRIN_RV32) && !defined(RVKINTRIN_RV64)
#if __riscv_xlen == 32
#define RVKINTRIN_RV32
#elif __riscv_xlen == 64
#define RVKINTRIN_RV64
#else
#error "__riscv_xlen not valid."
#endif
#endif

//	Mappings to implementation

//	=== (mapping)	Zbkb:	Bitmanipulation instructions for Cryptography

static inline int32_t _rv32_ror(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(ror_32)(rs1, rs2); }			//	ROR[W] ROR[W]I

static inline int32_t _rv32_rol(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(rol_32)(rs1, rs2); }			//	ROL[W] ROR[W]I

#ifdef RVKINTRIN_RV64
static inline int64_t _rv64_ror(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(ror_64)(rs1, rs2); }			//	ROR or RORI

static inline int64_t _rv64_rol(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(rol_64)(rs1, rs2); }			//	ROL or RORI
#endif

#ifdef RVKINTRIN_RV32
static inline int32_t _rv32_brev8(int32_t rs1)
	{ return _RVK_INTRIN_IMPL(brev8_32)(rs1); }				//	BREV8 (GREVI)
#endif

#ifdef RVKINTRIN_RV64
static inline int64_t _rv64_brev8(int64_t rs1)
	{ return _RVK_INTRIN_IMPL(brev8_64)(rs1); }				//	BREV8 (GREVI)
#endif

#ifdef RVKINTRIN_RV32
static inline int32_t _rv32_zip(int32_t rs1)
	{ return _RVK_INTRIN_IMPL(zip_32)(rs1); }				//	ZIP (SHFLI)

static inline int32_t _rv32_unzip(int32_t rs1)
	{ return _RVK_INTRIN_IMPL(unzip_32)(rs1); }				//	UNZIP (UNSHFLI)
#endif

//	=== (mapping)	Zbkc:	Carry-less multiply instructions

#ifdef RVKINTRIN_RV32
static inline int32_t _rv32_clmul(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(clmul_32)(rs1, rs2); }		//	CLMUL

static inline int32_t _rv32_clmulh(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(clmulh_32)(rs1, rs2); }		//	CLMULH
#endif

#ifdef RVKINTRIN_RV64
static inline int64_t _rv64_clmul(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(clmul_64)(rs1, rs2); }		//	CLMUL

static inline int64_t _rv64_clmulh(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(clmulh_64)(rs1, rs2); }		//	CLMULH
#endif

//	=== (mapping)	Zbkx:	Crossbar permutation instructions

#ifdef RVKINTRIN_RV32
static inline int32_t _rv32_xperm8(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(xperm8_32)(rs1, rs2); }		//	XPERM8

static inline int32_t _rv32_xperm4(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(xperm4_32)(rs1, rs2); }		//	XPERM4
#endif

#ifdef RVKINTRIN_RV64
static inline int64_t _rv64_xperm8(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(xperm8_64)(rs1, rs2); }		//	XPERM8

static inline int64_t _rv64_xperm4(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(xperm4_64)(rs1, rs2); }		//	XPERM4
#endif

//	=== (mapping)	Zknd:	NIST Suite: AES Decryption

#ifdef RVKINTRIN_RV32
static inline int32_t _rv32_aes32dsi(int32_t rs1, int32_t rs2, int bs)
	{ return _RVK_INTRIN_IMPL(aes32dsi)(rs1, rs2, bs); }	//	AES32DSI

static inline int32_t _rv32_aes32dsmi(int32_t rs1, int32_t rs2, int bs)
	{ return _RVK_INTRIN_IMPL(aes32dsmi)(rs1, rs2, bs); }	//	AES32DSMI
#endif

#ifdef RVKINTRIN_RV64
static inline int64_t _rv64_aes64ds(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(aes64ds)(rs1, rs2); }			//	AES64DS

static inline int64_t _rv64_aes64dsm(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(aes64dsm)(rs1, rs2); }		//	AES64DSM

static inline int64_t _rv64_aes64im(int64_t rs1)
	{ return _RVK_INTRIN_IMPL(aes64im)(rs1); }				//	AES64IM

static inline int64_t _rv64_aes64ks1i(int64_t rs1, int rnum)
	{ return _RVK_INTRIN_IMPL(aes64ks1i)(rs1, rnum); }		//	AES64KS1I

static inline int64_t _rv64_aes64ks2(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(aes64ks2)(rs1, rs2); }		//	AES64KS2
#endif

//	=== (mapping)	Zkne:	NIST Suite: AES Encryption

#ifdef RVKINTRIN_RV32
static inline int32_t _rv32_aes32esi(int32_t rs1, int32_t rs2, int bs)
	{ return _RVK_INTRIN_IMPL(aes32esi)(rs1, rs2, bs); }	//	AES32ESI

static inline int32_t _rv32_aes32esmi(int32_t rs1, int32_t rs2, int bs)
	{ return _RVK_INTRIN_IMPL(aes32esmi)(rs1, rs2, bs); }	//	AES32ESMI
#endif

#ifdef RVKINTRIN_RV64
static inline int64_t _rv64_aes64es(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(aes64es)(rs1, rs2); }			//	AES64ES

static inline int64_t _rv64_aes64esm(int64_t rs1, int64_t rs2)
	{ return _RVK_INTRIN_IMPL(aes64esm)(rs1, rs2); }		//	AES64ESM
#endif

//	=== (mapping)	Zknh:	NIST Suite: Hash Function Instructions

static inline long _rv_sha256sig0(long rs1)
	{ return _RVK_INTRIN_IMPL(sha256sig0)(rs1); }			//	SHA256SIG0

static inline long _rv_sha256sig1(long rs1)
	{ return _RVK_INTRIN_IMPL(sha256sig1)(rs1); }			//	SHA256SIG1

static inline long _rv_sha256sum0(long rs1)
	{ return _RVK_INTRIN_IMPL(sha256sum0)(rs1); }			//	SHA256SUM0

static inline long _rv_sha256sum1(long rs1)
	{ return _RVK_INTRIN_IMPL(sha256sum1)(rs1); }			//	SHA256SUM1

#ifdef RVKINTRIN_RV32
static inline int32_t _rv32_sha512sig0h(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(sha512sig0h)(rs1, rs2); }		//	SHA512SIG0H

static inline int32_t _rv32_sha512sig0l(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(sha512sig0l)(rs1, rs2); }		//	SHA512SIG0L

static inline int32_t _rv32_sha512sig1h(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(sha512sig1h)(rs1, rs2); }		//	SHA512SIG1H

static inline int32_t _rv32_sha512sig1l(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(sha512sig1l)(rs1, rs2); }		//	SHA512SIG1L

static inline int32_t _rv32_sha512sum0r(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(sha512sum0r)(rs1, rs2); }		//	SHA512SUM0R

static inline int32_t _rv32_sha512sum1r(int32_t rs1, int32_t rs2)
	{ return _RVK_INTRIN_IMPL(sha512sum1r)(rs1, rs2); }		//	SHA512SUM1R
#endif

#ifdef RVKINTRIN_RV64
static inline int64_t _rv64_sha512sig0(int64_t rs1)
	{ return _RVK_INTRIN_IMPL(sha512sig0)(rs1); }			//	SHA512SIG0

static inline int64_t _rv64_sha512sig1(int64_t rs1)
	{ return _RVK_INTRIN_IMPL(sha512sig1)(rs1); }			//	SHA512SIG1

static inline int64_t _rv64_sha512sum0(int64_t rs1)
	{ return _RVK_INTRIN_IMPL(sha512sum0)(rs1); }			//	SHA512SUM0

static inline int64_t _rv64_sha512sum1(int64_t rs1)
	{ return _RVK_INTRIN_IMPL(sha512sum1)(rs1); }			//	SHA512SUM1
#endif

//	=== (mapping)	Zksed:	ShangMi Suite: SM4 Block Cipher Instructions

static inline long _rv_sm4ks(int32_t rs1, int32_t rs2, int bs)
	{ return _RVK_INTRIN_IMPL(sm4ks)(rs1, rs2, bs); }		//	SM4KS

static inline long _rv_sm4ed(int32_t rs1, int32_t rs2, int bs)
	{ return _RVK_INTRIN_IMPL(sm4ed)(rs1, rs2, bs); }		//	SM4ED

//	=== (mapping)	Zksh:	ShangMi Suite: SM3 Hash Function Instructions

static inline long _rv_sm3p0(long rs1)
	{ return _RVK_INTRIN_IMPL(sm3p0)(rs1); }				//	SM3P0

static inline long _rv_sm3p1(long rs1)
	{ return _RVK_INTRIN_IMPL(sm3p1)(rs1); }				//	SM3P1

#ifdef __cplusplus
}
#endif

#endif	//	_RISCV_CRYPTO_SCALAR_H