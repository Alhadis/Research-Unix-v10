	.globl	add
add:
	addh3	0(%ap),4(%ap),0(%r2)
	addh3	2(%ap),6(%ap),2(%r2)
	MOVW	%r2,%r0
	RET

	.globl	sub
sub:
	SUBH3	4(%ap),0(%ap),0(%r2)
	SUBH3	6(%ap),2(%ap),2(%r2)
	MOVW	%r2,%r0
	RET

	.globl	inset
inset:
	MOVH	10(%ap),%r0
	ADDH3	0(%ap),%r0,0(%r2)
	ADDH3	2(%ap),%r0,2(%r2)
	SUBH3	%r0,4(%ap),4(%r2)
	SUBH3	%r0,6(%ap),6(%r2)
	MOVW	%r2,%r0
	RET

	.globl	div
div:
	MOVH	6(%ap),%r1
	MOVH	0(%ap),%r0
	DIVH2	%r1,%r0
	MOVH	%r0,0(%r2)
	MOVH	2(%ap),%r0
	DIVH2	%r1,%r0
	MOVH	%r0,2(%r2)
	MOVW	%r2,%r0
	RET

	.globl	mul
mul:
	MOVH	6(%ap),%r1
	MOVH	0(%ap),%r0
	MULH2	%r1,%r0
	MOVH	%r0,0(%r2)
	MOVH	2(%ap),%r0
	MULH2	%r1,%r0
	MOVH	%r0,2(%r2)
	MOVW	%r2,%r0
	RET

	.globl	rsubp
rsubp:
	SUBH3	8(%ap),0(%ap),0(%r2)
	SUBH3	10(%ap),2(%ap),2(%r2)
	SUBH3	8(%ap),4(%ap),4(%r2)
	SUBH3	10(%ap),6(%ap),6(%r2)
	MOVW	%r2,%r0
	RET

	.globl	raddp
raddp:
	ADDH3	0(%ap),8(%ap),0(%r2)
	ADDH3	2(%ap),10(%ap),2(%r2)
	ADDH3	4(%ap),8(%ap),4(%r2)
	ADDH3	6(%ap),10(%ap),6(%r2)
	MOVW	%r2,%r0
	RET

	.globl	eqpt
eqpt:
	CLRW	%r0
	CMPW	0(%ap),4(%ap)
	BNEB	nteqpt
	INCW	%r0
nteqpt:	RET

	.globl	eqrect
eqrect:
	CLRW	%r0
	CMPW	0(%ap),8(%ap)
	BNEB	neqrct
	CMPW	4(%ap),12(%ap)
	BNEB	neqrct
	INCW	%r0
neqrct:	RET
