	.file	"jerq.h"
	.data
	.comm	proctab,8872
	.text
	.align	8
	.def	main;	.val	main;	.scl	2;	.type	044;	.endef
	.globl	main
main:
	save	&.R1
	addw2	&.F1,%sp
	subw3	&proctab,&proctab+4516,%r0
	movw	%r0,0(%fp)
.L49:
	.def	.ef;	.val	.;	.scl	101;	.line	3;	.endef
	.ln	3
	.set	.F1,4
	.set	.R1,0
	ret	&.R1
	.def	main;	.val	.;	.scl	-1;	.endef
	.data
