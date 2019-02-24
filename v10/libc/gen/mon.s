L11:	.stabs	"mon.c",0x64,0,1,L11
	.stabs	"vaxpcc2",0xf0,0,17665,516607924
	.data
	.text
	.align	2
	.globl	_monitor
_monitor:
	.word	L.R1
	subl2	$L.SO1,sp
	.stabs	"monitor",0x24,0,5,_monitor
	.stabs	"monitor",0x20,0,68,0
	.stabs	"lowpc",0xa0,0,34,4
	.stabs	"highpc",0xa0,0,34,8
	.stabs	"buf",0xa0,0,36,12
	.stabs	"bufsiz",0xa0,0,4,16
	.stabs	"cntsiz",0xa0,0,4,20
	.lcomm	L14,4
	.lcomm	L15,4
	tstl	4(ap)
	jneq	L16
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0
	calls	$4,_profil
	.data	1
L19:

	.byte	0x6d,0x6f,0x6e,0x2e,0x6f,0x75,0x74,0x0
	.text
	pushl	$438
	pushl	$L19
	calls	$2,_creat
	movl	r0,r11
	pushl	L15
	pushl	L14
	pushl	r11
	calls	$3,_write
	pushl	r11
	calls	$1,_close
	jbr	L13
L16:
	movl	12(ap),L14
	movl	16(ap),L15
	movl	4(ap),*12(ap)
	movl	12(ap),r0
	movl	8(ap),4(r0)
	movl	12(ap),r0
	movl	20(ap),8(r0)
	movl	12(ap),r11
#ASM
.comm	countbase,4
#ASMEND
#ASM
.text
#ASMEND
#ASM
addl3	$12,r11,countabse
#ASMEND
	addl3	20(ap),20(ap),r0
	addl2	r0,r0
	addl2	r0,r0
	addl3	$12,r0,r11
	addl2	r11,12(ap)
	subl2	r11,16(ap)
	tstl	16(ap)
	jgtr	L22
	jbr	L13
L22:
	subl3	4(ap),8(ap),r0
	extzv	$1,$31,r0,r11
	cmpl	16(ap),r11
	jgeq	L23
	.data
	.align	2
L24:
	.long	0x4800,0x0	# %.20e
	.text
	cvtld	16(ap),r0
	cvtld	r11,r2
	divd2	r2,r0
	muld2	L24,r0
	cvtdl	r0,r11
	jbr	L25
L23:
	movl	$65535,r11
L25:
	pushl	r11
	pushl	4(ap)
	pushl	16(ap)
	pushl	12(ap)
	calls	$4,_profil
	.stabs	"ssiz",0x28,0,4,L15
	.stabs	"sbuf",0x28,0,36,L14
	.stabs	"o",0x40,0,4,11
L13:
	ret
	.set	L.R1,0x800
	.set	L.SO1,0x0
L26:	.stabs	"monitor",0xf4,0,45,L26
	.data
	.text
L27:	.stabs	"mon.c",0x94,0,46,L27
