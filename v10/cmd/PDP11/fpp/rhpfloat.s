.globl pfloat,pscien,_ftoa
.globl _pfloat
.text

_pfloat:rts pc
pfloat:	mov $'f,-(sp)
	br 1f
pscien:	mov $'e,-(sp)
1:
	mov r0,-(sp)
	tst r2
	bne 3f
	mov $6,(sp)
3:
	mov r3,-(sp)
	mov 2(r4),-(sp)
	mov (r4)+,-(sp)
	tst (r4)+
	jsr pc,_ftoa
2:	tstb (r3)+
	bne 2b
	dec r3
	add $10.,sp
	rts pc
