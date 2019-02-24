	.text
	.align	2
	.globl	_vecadd
_vecadd:
	.word	L.R1
	subl2	$L.SO1,sp
	.stabs	"vecadd",0x24,0,6,_vecadd
	.stabs	"vecadd",0x20,0,77,0
	.stabs	"w",0xa0,0,45,4
	.stabs	"n",0xa0,0,4,8
	.stabs	"s",0xa0,0,13,12
	movl	4(ap),r11
	movl	8(ap),r10
	movl	12(ap),r9
L15:
# while n >= 128
	subl2	$128,r10
	jlss	L14
#  add in 128 bytes
	addl2	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	$0,r9
	jbr	L15
L14:
	addl2	$128,r10
L17:
# while n >= 32
	subl2	$32,r10
	jlss	L16
#  add in 32 bytes
	addl2	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	(r11)+,r9
	adwc	$0,r9
	jbr	L17
L16:
	addl2	$32,r10
# fold the long
	ashl	$-16,r9,r0
	movzwl	r0,r0
	movzwl	r9,r9
	addl2	r0,r9
L19:
# while n >= 2
	subl2	$2,r10
	jlss	L18
#  add in 2 bytes
	movzwl	(r11)+,r0
	addl2	r0,r9
	jbr	L19
L18:
	cmpl	r10,$-1
	jneq	L20
	movzbl	(r11),r0
	addl2	r0,r9
L20:
# fold the long
	ashl	$-16,r9,r0
	movzwl	r9,r9
	addl2	r0,r9
	ashl	$-16,r9,r0
	movzwl	r9,r9
	addw2	r9,r0
	jbr	L13
	.stabs	"s",0x40,0,13,9
	.stabs	"n",0x40,0,4,10
	.stabs	"w",0x40,0,45,11
L13:
	ret
	.set	L.R1,0xe00
	.set	L.SO1,0x0
