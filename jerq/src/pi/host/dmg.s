	.data	0
	.data	2
	.data	1
	.set	LWM1,0x0
	.data	0
	.align	2
L17:
.byte 0151,0156
.byte 0151,0164
.byte 0151,0141
.byte 0154,040
.byte 0151,040
.byte 075,00
	.align	2
L18:
	.long	9
	.align	2
L19:
	.long	1
	.align	2
L20:
	.long	3
	.align	2
L27:
	.long	4
	.set	LWM2,0xc00
	.data	2
	.data	1
	.align	2
	.lcomm	q.1,64
	.data	0
	.align	2
L35:
.byte 0152,054
.byte 040,0114
.byte 054,040
.byte 0156,054
.byte 040,0163
.byte 054,040
.byte 0164,040
.byte 075,00
	.align	2
L44:
	.long	6
	.align	2
L46:
	.long	7
	.align	2
L48:
.byte 0151,0141
.byte 040,075
.byte 00,00
	.align	2
L53:
.byte 0162,040
.byte 075,040
.byte 00,00
	.align	2
L66:
.byte 0171,040
.byte 075,00
	.align	2
L70:
	.long	2
	.align	2
L71:
.byte 0155,040
.byte 075,040
.byte 056,056
.byte 056,00
	.set	LWM3,0xc00
	.data	2
	.data	1
	.lcomm	v.12,4
	.lcomm	v.14,4
	.data	0
	.align	2
L83:
.byte 0147,0157
.byte 0157,040
.byte 0150,0141
.byte 0163,040
.byte 0153,040
.byte 075,040
.byte 056,056
.byte 056,00
	.set	LWM4,0xc00
	.data	2
	.data	1
	.lcomm	v.18,4
	.lcomm	v.20,4
	.data	0
	.comm	_abc_,16
	.globl	_MAIN__
	.comm	_uvw_,40
	.globl	_s_wsle
	.globl	_do_lio
	.globl	_e_wsle
	.globl	_foo_
	.globl	_goo_
	.align	2
	.data	2
v.10:
	.long	0
	.long	6
	.space	4
	.long	0
v.11:
	.long	0
	.long	6
	.space	4
	.long	0
v.13:
	.long	0
	.long	6
	.space	4
	.long	0
v.15:
	.long	0
	.long	6
	.space	4
	.long	0
v.16:
	.long	0
	.long	6
	.space	4
	.long	0
v.17:
	.long	0
	.long	6
	.space	4
	.long	0
v.19:
	.long	0
	.long	6
	.space	4
	.long	0
v.5:
	.long	23
v.6:
	.long	0
	.long	6
	.space	4
	.long	0
v.9:
	.long	0
	.long	6
	.space	4
	.long	0
q.0:
	.long	6
	.long	7
	.long	8
	.long	9
_abc_:
	.long	101
	.long	102
	.long	103
	.long	104
	.stabs	"dmg.f",0x64,0,0,0
	.stabs	"vaxf77",0xf0,0,22053,0
.text
	.stabd	0x44,0,1
	.stabd	0x44,0,2
	.stabd	0x44,0,3
	.stabd	0x44,0,4
	.stabs	"abc_",0xe2,0,0,_abc_
	.stabs	"x",0x60,0,100,0
	.stabd	0xfc,0,4
	.stabs	"abc_",0xe4,0,0,_abc_
	.stabn	0xc4,0,0,2
	.stabd	0x44,0,5
	.align	1
	.set	LF1,0
	.stabn	0xd4,0,0,2
.text
	.stabd	0x44,0,6
	.globl	_MAIN__
_MAIN__:
	.word	LWM2
	subl2	$LF2,sp
	jmp	L14
	.stabs	"MAIN__",0x24,0,6,_MAIN__
	.stabd	0x44,0,7
	.stabd	0x44,0,8
	.stabd	0x44,0,9
	.stabd	0x44,0,10
	.stabd	0x44,0,11
	.stabs	"uvw_",0xe2,0,0,_uvw_
	.stabs	"r",0x60,0,104,0
	.stabs	"complex",0xfa,0,0,0
	.stabd	0xfc,0,4
	.stabs	"s",0x60,0,8,32
	.stabs	"complex",0xfa,0,0,0
	.stabs	"uvw_",0xe4,0,0,_uvw_
	.stabn	0xe2,0,0,0
	.stabs	"j",0x60,0,4,8
	.stabs	"ia",0x60,0,100,0
	.stabd	0xfc,0,4
	.stabn	0xe8,0,0,q.0
	.stabn	0xe2,0,0,0
	.stabs	"l",0x60,0,4,24
	.stabs	"k",0x60,0,100,0
	.stabd	0xfc,0,16
	.stabn	0xe8,0,0,q.1
	.stabn	0xc4,0,0,2
L15:
	.stabd	0x44,0,12
	pushl	$v.6
	calls	$1,_s_wsle
	pushl	$11
	pushl	$L17
	pushl	$L19
	pushl	$L18
	calls	$4,_do_lio
	pushl	$4
	pushl	$v.5
	pushl	$L19
	pushl	$L20
	calls	$4,_do_lio
	calls	$0,_e_wsle
	.stabd	0x44,0,13
	movl	$1,r11
L24:
	movl	r11,v.5
	.stabd	0x44,0,14
	addl3	$1000,r11,r0
	addl3	$-1,r11,r1
	ashl	$2,r1,r1
	addl2	$-16,r1
	addl3	r1,fp,r1
	movl	r0,(r1)
	.stabd	0x44,0,15
	addl3	$-1,r11,r0
	ashl	$3,r0,r0
	movl	r0,-20(fp)
	addl3	$2000,r11,r0
	cvtld	r0,r0
	movd	r0,-28(fp)
	addl3	-20(fp),$_uvw_,r0
	cvtdf	-28(fp),(r0)
	addl3	-20(fp),$_uvw_+4,r0
	clrf	(r0)
	.stabd	0x44,0,16
	movl	$1,r10
L26:
	movl	r10,q.0+8
	.stabd	0x44,0,17
L22:
	mull3	$10,r11,r0
	addl2	r10,r0
	ashl	$2,r10,r1
	addl2	r11,r1
	movl	r0,q.1+-20[r1]
	incl	r10
	cmpl	r10,$4
	jleq	L26
L25:
	movl	r10,q.0+8
	incl	r11
	cmpl	r11,$4
	jleq	L24
L23:
	movl	r11,v.5
	.stabd	0x44,0,18
	addl3	$2000,v.5,r0
	cvtld	r0,r0
	movd	r0,-28(fp)
	cvtdf	-28(fp),_uvw_+32
	clrf	_uvw_+36
	.stabd	0x44,0,19
	addl3	$-32,fp,r0
	pushl	r0
	addl3	$-16,fp,r0
	pushl	r0
	pushl	$q.1+24
	pushl	$q.1
	pushl	$q.0
	pushl	$q.0+8
	calls	$6,_foo_
	.stabd	0x44,0,20
	pushl	$q.1
	pushl	$L27
	calls	$2,_goo_
	.stabd	0x44,0,21
L16:
	.stabd	0xb4,0,21
	ret
	.align	1
L14:
	jmp	L15
	.set	LF2,32
	.stabs	"i",0x26,0,4,v.5
	.stabs	"k",0x28,0,100,q.1
	.stabd	0xfc,0,16
	.stabs	"m",0x80,0,100,16
	.stabd	0xfc,0,4
	.stabs	"n",0x80,0,4,32
	.stabs	"ia",0x26,0,100,q.0
	.stabd	0xfc,0,4
	.stabs	"_0",0x26,0,8,q.0
	.stabs	"_0",0xfa,0,0,0
	.stabs	"_0",0x5c,0,8,0
	.stabs	"j",0x60,0,4,8
	.stabs	"ia",0x60,0,100,0
	.stabd	0xfc,0,4
	.stabs	"_0",0x5e,0,8,16
	.stabs	"_1",0x28,0,8,q.1
	.stabs	"_1",0xfa,0,0,0
	.stabs	"_1",0x5c,0,8,0
	.stabs	"l",0x60,0,4,24
	.stabs	"k",0x60,0,100,0
	.stabd	0xfc,0,16
	.stabs	"_1",0x5e,0,8,64
	.stabn	0xd4,0,0,2
L28:
	.stabs	"MAIN_",0xf4,0,21,L28
.text
	.stabd	0x44,0,22
	.globl	_foo_
_foo_:
	.word	LWM3
	subl2	$LF3,sp
	jmp	L30
	.stabs	"foo_",0x24,0,22,_foo_
	.stabs	"foo_",0x20,0,68,0
	.stabd	0x44,0,23
	.stabd	0x44,0,24
	.stabd	0x44,0,25
	.stabd	0x44,0,26
	.stabd	0x44,0,27
	.stabd	0x44,0,28
	.stabd	0x44,0,29
	.stabd	0x44,0,31
	.stabd	0x44,0,32
	.stabd	0x44,0,33
	.stabs	"abc_",0xe2,0,0,_abc_
	.stabs	"x",0x60,0,100,0
	.stabd	0xfc,0,4
	.stabs	"abc_",0xe4,0,0,_abc_
	.stabs	"uvw_",0xe2,0,0,_uvw_
	.stabs	"r",0x60,0,104,0
	.stabs	"complex",0xfa,0,0,0
	.stabd	0xfc,0,4
	.stabs	"s",0x60,0,8,32
	.stabs	"complex",0xfa,0,0,0
	.stabs	"uvw_",0xe4,0,0,_uvw_
	.stabs	"abc_",0xe2,0,0,_abc_
	.stabs	"y",0x60,0,100,4
	.stabd	0xfc,0,2
	.stabs	"abc_",0xe4,0,0,_abc_
	.stabs	"uvw_",0xe2,0,0,_uvw_
	.stabs	"t",0x60,0,8,16
	.stabs	"dcomplex",0xfa,0,0,0
	.stabs	"uvw_",0xe4,0,0,_uvw_
	.stabs	"j",0xa0,0,36,4
	.stabs	"ia",0xa0,0,36,8
	.stabs	"k",0xa0,0,36,12
	.stabs	"l",0xa0,0,36,16
	.stabs	"m",0xa0,0,36,20
	.stabs	"n",0xa0,0,36,24
	.stabn	0xc4,0,0,2
L33:
	pushl	$v.9
	calls	$1,_s_wsle
	pushl	$15
	pushl	$L35
	pushl	$L19
	pushl	$L18
	calls	$4,_do_lio
	pushl	$4
	pushl	4(ap)
	pushl	$L19
	pushl	$L20
	calls	$4,_do_lio
	pushl	$4
	pushl	16(ap)
	pushl	$L19
	pushl	$L20
	calls	$4,_do_lio
	pushl	$4
	pushl	24(ap)
	pushl	$L19
	pushl	$L20
	calls	$4,_do_lio
	pushl	$8
	pushl	$_uvw_+32
	pushl	$L19
	pushl	$L44
	calls	$4,_do_lio
	pushl	$16
	pushl	$_uvw_+16
	pushl	$L19
	pushl	$L46
	calls	$4,_do_lio
	calls	$0,_e_wsle
	.stabd	0x44,0,34
	pushl	$v.10
	calls	$1,_s_wsle
	pushl	$4
	pushl	$L48
	pushl	$L19
	pushl	$L18
	calls	$4,_do_lio
	pushl	$4
	addl3	$4,8(ap),r0
	pushl	r0
	pushl	$L27
	pushl	$L20
	calls	$4,_do_lio
	calls	$0,_e_wsle
	.stabd	0x44,0,35
	pushl	$v.11
	calls	$1,_s_wsle
	pushl	$4
	pushl	$L53
	pushl	$L19
	pushl	$L18
	calls	$4,_do_lio
	pushl	$8
	pushl	$_uvw_
	pushl	$L27
	pushl	$L44
	calls	$4,_do_lio
	calls	$0,_e_wsle
	.stabd	0x44,0,36
	movl	$1,r11
L60:
	movl	r11,v.12
	.stabd	0x44,0,37
L58:
	pushl	$v.13
	calls	$1,_s_wsle
	movl	$1,r10
L63:
	movl	r10,v.14
	pushl	$4
	ashl	$2,r10,r0
	addl2	r11,r0
	ashl	$2,r0,r0
	addl2	12(ap),r0
	pushl	r0
	pushl	$L19
	pushl	$L20
	calls	$4,_do_lio
	incl	r10
	cmpl	r10,$4
	jleq	L63
L62:
	movl	r10,v.14
	calls	$0,_e_wsle
	incl	r11
	cmpl	r11,$4
	jleq	L60
L59:
	movl	r11,v.12
	.stabd	0x44,0,38
	pushl	$v.15
	calls	$1,_s_wsle
	pushl	$3
	pushl	$L66
	pushl	$L19
	pushl	$L18
	calls	$4,_do_lio
	pushl	$4
	pushl	$_abc_+4
	pushl	$L70
	pushl	$L20
	calls	$4,_do_lio
	calls	$0,_e_wsle
	.stabd	0x44,0,39
	pushl	$v.16
	calls	$1,_s_wsle
	pushl	$7
	pushl	$L71
	pushl	$L19
	pushl	$L18
	calls	$4,_do_lio
	pushl	$4
	addl3	$4,20(ap),r0
	pushl	r0
	pushl	$L27
	pushl	$L20
	calls	$4,_do_lio
	calls	$0,_e_wsle
	.stabd	0x44,0,40
L32:
L31:
	.stabd	0xb4,0,40
	ret
	.align	1
L30:
	subl2	$4,8(ap)
	subl2	$20,12(ap)
	subl2	$4,20(ap)
	jmp	L33
	.set	LF3,0
	.stabs	"i",0x28,0,4,v.12
	.stabs	"i2",0x28,0,4,v.14
	.stabn	0xd4,0,0,2
L76:
	.stabs	"foo",0xf4,0,40,L76
.text
	.stabd	0x44,0,41
	.globl	_goo_
_goo_:
	.word	LWM4
	subl2	$LF4,sp
	jmp	L78
	.stabs	"goo_",0x24,0,41,_goo_
	.stabs	"goo_",0x20,0,68,0
	.stabd	0x44,0,42
	.stabd	0x44,0,43
	.stabd	0x44,0,44
	.stabs	"n",0xa0,0,36,4
	.stabs	"k",0xa0,0,36,8
	.stabn	0xc4,0,0,2
L81:
	pushl	$v.17
	calls	$1,_s_wsle
	pushl	$15
	pushl	$L83
	pushl	$L19
	pushl	$L18
	calls	$4,_do_lio
	calls	$0,_e_wsle
	.stabd	0x44,0,45
	movl	$1,r11
	movl	*4(ap),-16(fp)
	cmpl	r11,-16(fp)
	jgtr	L87
L88:
	movl	r11,v.18
	.stabd	0x44,0,46
L86:
	pushl	$v.19
	calls	$1,_s_wsle
	movl	$1,r10
	movl	*4(ap),-20(fp)
	cmpl	r10,-20(fp)
	jgtr	L90
L91:
	movl	r10,v.20
	pushl	$4
	mull3	-4(fp),r10,r0
	addl2	r11,r0
	ashl	$2,r0,r0
	addl2	8(ap),r0
	pushl	r0
	pushl	$L19
	pushl	$L20
	calls	$4,_do_lio
	incl	r10
	cmpl	r10,-20(fp)
	jleq	L91
L90:
	movl	r10,v.20
	calls	$0,_e_wsle
	incl	r11
	cmpl	r11,-16(fp)
	jleq	L88
L87:
	movl	r11,v.18
	.stabd	0x44,0,47
L80:
L79:
	.stabd	0xb4,0,47
	ret
	.align	1
L78:
	movl	*4(ap),-4(fp)
	movl	*4(ap),-8(fp)
	addl3	$1,-4(fp),r0
	movl	r0,-12(fp)
	ashl	$2,-12(fp),r0
	subl2	r0,8(ap)
	jmp	L81
	.set	LF4,20
	.stabs	"i",0x28,0,4,v.18
	.stabs	"j",0x28,0,4,v.20
	.stabn	0xd4,0,0,2
L94:
	.stabs	"goo",0xf4,0,47,L94
.text
	.data
	.stabs	"abc_",0x20,0,8,0
	.stabs	"abc_COMMON_",0xfa,0,0,0
	.stabs	"abc_COMMON_",0x5c,0,8,0
	.stabs	"x",0x60,0,100,0
	.stabd	0xfc,0,4
	.stabs	"y",0x60,0,100,4
	.stabd	0xfc,0,2
	.stabs	"abc_COMMON_",0x5e,0,8,16
	.stabs	"uvw_",0x20,0,8,0
	.stabs	"uvw_COMMON_",0xfa,0,0,0
	.stabs	"uvw_COMMON_",0x5c,0,8,0
	.stabs	"r",0x60,0,104,0
	.stabs	"complex",0xfa,0,0,0
	.stabd	0xfc,0,4
	.stabs	"s",0x60,0,8,32
	.stabs	"complex",0xfa,0,0,0
	.stabs	"t",0x60,0,8,16
	.stabs	"dcomplex",0xfa,0,0,0
	.stabs	"uvw_COMMON_",0x5e,0,8,40
	.stabs	"complex",0x5c,0,8,0
	.stabs	"real",0x60,0,6,0
	.stabs	"imag",0x60,0,6,4
	.stabs	"complex",0x5e,0,8,8
	.stabs	"dcomplex",0x5c,0,8,0
	.stabs	"real",0x60,0,7,0
	.stabs	"imag",0x60,0,7,8
	.stabs	"dcomplex",0x5e,0,8,16
.text
L95:
	.stabs	"dmg.f",0x94,0,47,L95
