	data	1
	text
	global	main
main:
	mov.l	&2000, %a2
	clr.l	%d5
	clr.w	393248
	mov.l	&500, 393240
	clr.l	%d4
L%15:
	cmp.l	%d4,&51200
	bge	L%14
	mov.l	%d4,%d0
	lsl.l	&1,%d0
	clr.w	0(%a2,%d0.l)
L%13:
	add.l	&1,%d4
	br	L%15
L%14:
L%18:
	clr.w	%d2
L%21:
	cmp.w	%d2,&800
	bhs	L%20
	clr.l	%d0
	mov.w	%d2,%d0
	eor.l	%d5,%d0
	and.l	&1023,%d0
	mov.w	%d0,%d3
	cmp.w	%d3,&1024
	bhs	L%22
	mov.w	%d2,%d0
	lsr.w	&4,%d0
	mov.w	%d3,%d1
	mulu.w	&50,%d1
	add.w	%d0,%d1
	mov.w	%d1,%d6
	mov.w	%d2,%d0
	and.w	&15,%d0
	mov.l	&0x8000,%d1
	lsr.l	%d0,%d1
	clr.l	%d0
	mov.w	%d6,%d0
	lsl.l	&1,%d0
	eor.w	%d1,0(%a2,%d0.l)
L%22:
L%19:
	add.w	&1,%d2
	br	L%21
L%20:
	add.l	&13,%d5
L%16:
	br	L%18
L%17:
L%12:
	rts
