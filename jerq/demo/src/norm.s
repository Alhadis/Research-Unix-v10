	data	1
	text
	global	norm
	global	sqrtryz
sqrtryz:
	link	%fp,&F%1
	mov.w	&M%1,-2(%fp)
	movm.l	&M%1,S%1(%fp)

	mov.w	8(%fp),%d0
	muls.w	%d0,%d0
	mov.w	10(%fp),%d1
	muls.w	%d1,%d1
	sub.l	%d1,%d0
	mov.w	12(%fp),%d1
	muls.w	%d1,%d1
	sub.l	%d1,%d0
	br	common
norm:
	link	%fp,&F%1	# save registers for sqrt
	mov.w	&M%1,-2(%fp)
	movm.l	&M%1,S%1(%fp)

	mov.w	12(%fp),%d0
	muls.w	%d0,%d0
	mov.w	10(%fp),%d1
	muls.w	%d1,%d1
	add.l	%d1,%d0
	mov.w	8(%fp),%d1
	muls.w	%d1,%d1
	add.l	%d1,%d0		# done with vector sum

common:
	mov.l	%d0,%d2		# d2 is x, d3 is s, d4 is olds
	tst.l	%d2
	bgt	L%13
	clr.l	%d0
	br	L%12
L%13:
	cmp.l	%d2,&1
	bne	L%14
	mov.l	&1,%d0
	br	L%12
L%14:
	mov.l	%d2,%d4
	asr.l	&1,%d4
	mov.l	%d4,%d3
	add.l	&2,%d4
L%16:
	mov.l	%d3,%d0
	sub.l	%d4,%d0
	bge	skip
	neg.l	%d0
skip:
	cmp.l	%d0,&1
	ble	L%17
	mov.l	%d3,%d4
	mov.l	%d2,%d0
	divs.w	%d3,%d0
	ext.l	%d0
	add.l	%d3,%d0
	asr.l	&1,%d0
	mov.l	%d0,%d3
	br	L%16
L%17:
	mov.l	%d3,%d0
	br	L%12
L%12:
	movm.l	S%1(%fp),&M%1
	unlk	%fp
	rts
	set	S%1,-14
	set	T%1,-14
	set	F%1,-18
	set	M%1,034
	data	1
