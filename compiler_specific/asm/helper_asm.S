@
@
@


	@.import		__aeabi_idiv
	@.import		__aeabi_uidiv

	.section	.text
	.thumb

@-------------------------------------------------------------------------------
	.global		__aeabi_uidivmod
	@ uidiv_return __aeabi_uidivmod( unsigned numerator, unsigned denominator )
	.align	2
	.thumb_func

__aeabi_uidivmod:
@
@ r0: quotient
@ r1: dividend (HIGH)
@ r2: divisor
@ r3: dividend (LOW)
@ r4: counter

	push	{ r4, lr }			@ save registers

	mov		r2, r1				@ the divisor

	mov		r3, r0				@ the dividend
	mov		r1, #0				@

	mov		r0, #0				@ the quotient

	mov		r4, #32				@ loop count

uidiv_loop01:
	add		r0, r0				@ shift the quotient toward left

	add		r3, r3				@ shift the dividend toward left
	adc		r1, r1				@

	cmp		r1, r2				@ can subtract the divisor from thre dividend  ?
	bcc		uidiv_skip01		@ no, skip

	add		r0, #1				@ add 1 into a quotient
	sub		r1, r2				@ div0

uidiv_skip01:
	sub		r4, #1				@ count down
	bne		uidiv_loop01		@ loop until all columns subtracted

	pop		{ r4, pc }			@ restore registers and return from this

@-------------------------------------------------------------------------------
	.global		__aeabi_idivmod
	@ idiv_return __aeabi_idivmod( int numerator, int denominator )
	.align	2
	.thumb_func

__aeabi_idivmod:
@ r0: dividend -> quotient
@ r1: divisor  -> remainder (ignore)
@ r4: sign (bit0: the quotient / bit1: the remainder )

	push	{ r4, lr }			@ save registers

.if 1
	cmp		r0, #0				@ separate sign and value
	bpl		idiv_skip01

	mov		r4, #3				@ minus
	rsbs	r0, r0, #0			@ negate
	b		idiv_skip02

idiv_skip01:
	mov		r4, #0				@ plus
.else
	cmp		r0, #0				@ separate sign and value

	ite		hi
	movs	r0, #1
	movs	r0, #1

	itte	pl
	mov		r4, #3				@ minus
	rsb		r0, r0, #0			@ negate
	mov		r4, #0				@ plus
.endif

idiv_skip02:
	cmp		r1, #0				@ separate sign and value
	bpl		idiv_skip03

	mov		r2, #1
	eor		r4, r2				@ inverse
	rsbs	r1, r1, #0			@ negate

idiv_skip03:
	blx		__aeabi_uidivmod	@ divide r0 by r1

	mov		r3, #1				@ is the quatient minus?
	tst		r4, r3				@
	beq		idiv_skip04			@ no, skip

	rsbs	r0, r0, #0			@ negate

idiv_skip04:
	mov		r3, #2				@ is the remainder minus?
	tst		r4, r3				@
	beq		idiv_skip05			@ no, skip

	rsbs	r1, r1, #0			@ negate

idiv_skip05:
	pop	{ r4, pc }				@ restore register and return from this


@-------------------------------------------------------------------------------
	.global		__aeabi_idiv
	@ int __aeabi_idiv( int numerator, int denominator )
	.align	2
	.thumb_func

__aeabi_idiv:
@ r0: dividend
@ r1: divisor

	b		__aeabi_idivmod		@ same as idivmod (ignore remainder)


@-------------------------------------------------------------------------------
	.global		__aeabi_uidiv
	@ unsigned __aeabi_uidiv( unsigned numerator, unsigned denominator )
	.align	2
	.thumb_func

__aeabi_uidiv:
@ r0: dividend
@ r1: divisor

	b		__aeabi_uidivmod	@ same as uidivmod (ignore remainder)

@-------------------------------------------------------------------------------
	.end
