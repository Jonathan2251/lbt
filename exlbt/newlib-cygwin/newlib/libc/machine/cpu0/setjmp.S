# setjmp/longjmp for FR30.  The jmpbuf looks like this:
#	
# Register	jmpbuf offset
# R8		0x00
# R9		0x04
# R10		0x08
# R11		0x0c
# R12		0x10
# R13		0x14
# R14 (FP)	0x18
# R15 (SP)	0x1c
# RP            0x20
	
.macro save reg
	st	\reg,@r4
	add	#4,r4
.endm
	
.macro restore reg
	ld	@r4,\reg
	add	#4,r4
.endm


	.text
	.global	setjmp
setjmp:
	st $9,  0($a0)
	st $10, 4($a0)
	st $11, 8($a0)
	st $12, 12($a0)
	st $13, 16($a0)
	st $14, 20($a0)
	st $15, 24($a0)
# Return 0 to caller.
	addiu $lr, $zero, 0x0
	ret $lr

	.global	longjmp
longjmp:
	ld $9,  0($a0)
	ld $10, 4($a0)
	ld $11, 8($a0)
	ld $12, 12($a0)
	ld $13, 16($a0)
	ld $14, 20($a0)
	ld $15, 24($a0)

# If caller attempted to return 0, return 1 instead.
        bne     $5,$0,1f
        addiu   $5,$0,1
1:
        addu    $2,$0,$5
        ret	$lr
