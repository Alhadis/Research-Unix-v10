L11:	.stabs	"x.c",0x64,0,0,L11
	.stabs	"vaxpcc2",0xf0,0,17665,563928025
	.data
	.text
	.align	2
	.globl	_main
_main:
	.word	L.R1
	subl2	$L.SO1,sp
	.stabs	"main",0x24,0,3,_main
	.stabs	"main",0x20,0,68,0
	.stabs	"argc",0xa0,0,4,4
	.stabs	"argv",0xa0,0,162,8
L16:
	.data	1
L18:

	.byte	0x25,0x73,0xa,0x0
	.text
	.data	1
L19:

	.byte	0x79,0x0
	.text
	cmpl	4(ap),$1
	jleq	L20
	movl	8(ap),r0
	movl	4(r0),-(sp)
	jbr	L21
L20:
	movl	$L19,-(sp)
L21:
	pushl	$L18
	calls	$2,_printf
	jbr	L16
L15:
L13:
	ret
	.set	L.R1,0x0
	.set	L.SO1,0x0
L22:	.stabs	"main",0xf4,0,6,L22
	.data
	.text
L23:	.stabs	"x.c",0x94,0,7,L23
