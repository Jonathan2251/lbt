# Cpu0 startup code
# This function has done in elf2hex for Cpu0, so useless at this point.

	.section .text
	.global	_start
_start:
	
	;; Initialise the stack pointer
	li	$sp, __stack
	move    $fp, $sp

	;; Zero the data space
	li	$7,	_edata
	li	$8,	_end
	li	$t9,	0
	;; $t9, $7 and $8 are caller saved registers
.L0:	
	st	$t9, 0($7)
	addiu	4, $7
	cmp	$sw, $8, $7
	jlt	$sw, .L0
        nop

	;; Call global and static constructors
	jsub	_init
        nop
	
	;;  Setup destrcutors to be called from exit.
	;;  (Just in case main never returns....)
	li	$t9,	atexit
	li	$7,	_fini
	jsub	atexit
        nop
	
	;;  Initialise argc, argv and envp to empty
	li	r4, 0
	li	r5, 0
	l	r6, 0

	;; Call main
	jsub	 main
        nop

	;; Jump to exit
	jsub	exit
        nop

	;; Should never reach here
        nop
