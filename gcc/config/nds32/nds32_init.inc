/*
 * nds32_init.inc
 *
 * NDS32 architecture startup assembler header file
 *
 */

.macro nds32_init

	! Initialize GP for data access
	la      $gp, _SDA_BASE_

#if defined(__NDS32_EXT_FPU_DP__) || defined(__NDS32_EXT_FPU_SP__)
	! Enable FPU
	mfsr    $r0, $FUCOP_CTL
	ori     $r0, $r0, #0x1
	mtsr    $r0, $FUCOP_CTL
	dsb

	! Enable denormalized flush-to-Zero mode
	fmfcsr  $r0
	ori     $r0,$r0,#0x1000
	fmtcsr  $r0
	dsb
#endif

	! Initialize default stack pointer
	la      $sp, _stack

.endm
