L11:	.stabs	"",0x64,0,1,L11
	.stabs	"vaxpcc2",0xf0,0,17665,593440224
	.data
	.text
	.align	2
	.globl	_main
_main:
	.word	L.R1
	subl2	$L.SO1,sp
	.stabs	"main",0x24,0,2,_main
	movl	$4,-4(fp)
	bitl	$5,-4(fp)
	jeql	L14
	.data	1
L16:

	.byte	0x35,0xa,0x0
	.text
	pushl	$L16
	calls	$1,_print
L14:
	jbc	$2,-4(fp),L17
	.data	1
L18:

	.byte	0x34,0xa,0x0
	.text
	pushl	$L18
	calls	$1,_print
L17:
	movl	$3,-8(fp)
	addl3	-8(fp),-4(fp),r0
	jbc	$4,r0,L19
	.data	1
L20:

	.byte	0x31,0x36,0xa,0x0
	.text
	pushl	$L20
	calls	$1,_print
L19:
	addl3	-8(fp),-4(fp),r0
	bitl	$9,r0
	jeql	L21
	.data	1
L22:

	.byte	0x39,0xa,0x0
	.text
	pushl	$L22
	calls	$1,_print
L21:
	addl3	-8(fp),-4(fp),r0
	jbc	$2,r0,L23
	.data	1
L24:

	.byte	0x34,0xa,0x0
	.text
	pushl	$L24
	calls	$1,_print
L23:
	addl3	-8(fp),-4(fp),r0
	jbs	$4,r0,L25
	.data	1
L26:

	.byte	0x21,0x31,0x36,0xa,0x0
	.text
	pushl	$L26
	calls	$1,_print
L25:
	.stabs	"j",0x80,0,4,8
	.stabs	"i",0x80,0,4,4
L13:
	ret
	.set	L.R1,0x0
	.set	L.SO1,0x8
L27:	.data
	.text
L28:	.stabs	"",0x94,0,18,L28
