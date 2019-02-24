	.globl	Rect
Rect:
	MOVH	2(%ap),0(%r2)
	MOVH	6(%ap),2(%r2)
	MOVH	10(%ap),4(%r2)
	MOVH	14(%ap),6(%r2)
	MOVW	%r2,%r0
	RET
	.globl	Pt
Pt:
	MOVH	2(%ap),0(%r2)
	MOVH	6(%ap),2(%r2)
	MOVW	%r2,%r0
	RET

	.globl	Rpt
Rpt:
	MOVW	0(%ap),0(%r2)
	MOVW	4(%ap),4(%r2)
	MOVW	%r2,%r0
	RET
