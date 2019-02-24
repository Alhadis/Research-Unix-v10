/
/ floating point for 11/40 or 11/03 only
/ modified from rhflibp.asm by DEPT 2242 BTL WH
/ 06-JUN-78
/
/ 11/70 floating point removed
/
/ The test:
/
/	mov	sp,-(sp)
/	sub	sp,(sp)
/	tst	(sp)+
/	bne	11/70
/
/ does NOT WORK on the LSI-11 (PDP-11/03)
/
.globl fad,fsb,fmp,fdv,int,float,fltused

.text
fltused: 1	/ used to satisfy c compile reference when floating
		/  point is used
fad:	mov (sp)+,ret
	075006		/ fadd sp on 11/35/40/03
	jmp *ret

fsb:	mov (sp)+,ret
	075016		/ fsub sp on 11/35/40/03
	jmp *ret

fmp:	mov (sp)+,ret
	075026		/ fmul sp on 11/35/40/03
	jmp *ret

fdv:	mov (sp)+,ret
	075036		/ fdiv sp on 11/35/40/03
	jmp *ret
/
/
/
/
int:	mov	r0,-(sp)
	mov	r1,-(sp)
	mov	r2,-(sp)
	clr	-(sp) 		/sign of number
	mov	12.(sp),r1		/ low part
	mov	10.(sp),r0		/ hi part
	bpl	1f			/ ok, positive
	mov	$100000,(sp)	/ set sign -
	bic	(sp),r0		/ set number pos
1:	mov	r0,r2		/ extract exponent
	bic	$100177,r2
	bic	r2,r0
	ash	$-7,r2		/ make it integer
	bis	$200,r0		/ set hidden bit
	sub	$200,r2		/ subtr off excess 200
	ble	2f			/ exponent 0, treat as zero number
	sub	$24.,r2		/ set for shift
	ashc	r2,r0		/ left or right
4:	tst	(sp)+		/ sign
	bpl	3f
	neg	r0
	neg	r1
	sbc	r0
3:	mov	r0,8.(sp)		/ hi part
	mov	r1,10.(sp)		/ low part
	mov	(sp)+,r2
	mov	(sp)+,r1
	mov	(sp)+,r0
	rts	pc

2:	clr	r0			/ exponent was 0, set number to 0
	clr	r1
	br	4b
/
/
/
/
float:	mov	4(sp),-(sp)
	mov	4(sp),-(sp)
	bmi	negflt
	clr	-(sp)
	mov	$[200+24.]*200,-(sp)
1:	bit	$377*200,4(sp)
	beq	2f
	asr	4(sp)
	ror	6(sp)
	add	$200,(sp)
	br	1b
2:	bis	(sp),4(sp)
	jsr	pc,fsb
	mov	(sp)+,4(sp)
	mov	(sp)+,4(sp)
	rts	pc

negflt:	neg	(sp)
	neg	2(sp)
	sbc	(sp)
	bmi	3f		/ overflow on sign flip
	jsr	pc,float
	bis	$100000,(sp)
	mov	(sp)+,4(sp)
	mov	(sp)+,4(sp)
	rts	pc

3:	cmp	(sp)+,(sp)+		/ toss
	mov	$[200+32.]*200+100000,2(sp)		/
	clr	4(sp)
	rts	pc

.data
ret:0
