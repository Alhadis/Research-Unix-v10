/ unsigned divide routine

.globl	udiv, urem

udiv:
	cmp	r1,$1
	ble	9f
	mov	r1,-(sp)
	mov	r0,r1
	clr	r0
	div	(sp)+,r0
	rts	pc
9:
	bne	9f
	tst	r0
	rts	pc
9:
	cmp	r1,r0
	blos	9f
	clr	r0
	rts	pc
9:
	mov	$1,r0
	rts	pc

urem:
	cmp	r1,$1
	ble	9f
	mov	r1,-(sp)
	mov	r0,r1
	clr	r0
	div	(sp)+,r0
	mov	r1,r0
	rts	pc
9:
	bne	9f
	clr	r0
	rts	pc
9:
	cmp	r0,r1
	blo	9f
	sub	r1,r0
9:
	tst	r0
	rts	pc
