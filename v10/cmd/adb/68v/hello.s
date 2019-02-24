	data	1
	even
	global	x
x:
	short	1
	text
	global	main
main:
	link	%fp,&F%1
	movm.l	&M%1,S%1(%fp)
	mov.l	&0,%d2
L%16:
	cmp.w	%d2,&10
	bge	L%15
	add.w	&1,x
L%14:
	add.w	&1,%d2
	br	L%16
L%15:
L%13:
	movm.l	S%1(%fp),&M%1
	unlk	%fp
	rts
	set	S%1,-4
	set	T%1,-4
	set	F%1,-8
	set	M%1,0x0004
	data	1
