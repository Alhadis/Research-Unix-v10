L11:	.stabs	"btreport.c",0x64,0,1,L11
	.stabs	"vaxpcc2",0xf0,0,17665,526911312
	.data
	.comm	_ndcnt,24
	.comm	_frcnt,4
	.comm	_reccnt,4
	.comm	_reclen,4
	.comm	_bt,4
	.text
	.align	2
	.globl	_main
_main:
	.word	L.R1
	subl2	$L.SO1,sp
	.stabs	"main",0x24,0,13,_main
	.stabs	"main",0x20,0,68,0
	.stabs	"argc",0xa0,0,4,4
	.stabs	"argv",0xa0,0,162,8
	movl	$1,-4(fp)
	jbr	L35
L36:
	movl	-4(fp),r0
	pushl	*8(ap)[r0]
	calls	$1,_doarg
	clrl	-8(fp)
	jbr	L40
L41:
	movl	-8(fp),r0
	clrl	_ndcnt[r0]
	incl	-8(fp)
L40:
	cmpl	-8(fp),$5
	jleq	L41
L39:
	clrl	_reclen
	movl	_reclen,_reccnt
	movl	_reccnt,_frcnt
	addl3	$1,-4(fp),r0
	cmpl	r0,4(ap)
	jgeq	L42
	decl	__iob+16
	jlss	L44
	movb	$10,r0
	movl	__iob+20,r1
	incl	__iob+20
	movb	r0,(r1)
	jbr	L45
L44:
	pushl	$__iob+16
	pushl	$10
	calls	$2,__flsbuf
L45:
L42:
	incl	-4(fp)
L35:
	cmpl	-4(fp),4(ap)
	jlss	L36
L34:
	pushl	$0
	calls	$1,_exit
	.stabs	"j",0x80,0,4,8
	.stabs	"i",0x80,0,4,4
L32:
	ret
	.set	L.R1,0x0
	.set	L.SO1,0x8
L47:	.stabs	"main",0xf4,0,23,L47
	.data
	.text
	.align	2
	.globl	_doarg
_doarg:
	.word	L.R2
	subl2	$L.SO2,sp
	.stabs	"doarg",0x24,0,27,_doarg
	.stabs	"doarg",0x20,0,68,0
	.stabs	"s",0xa0,0,34,4
	pushl	$0
	pushl	4(ap)
	calls	$2,_bopen
	movl	r0,_bt
	tstl	_bt
	jneq	L49
	pushl	4(ap)
	calls	$1,_strlen
	movl	r0,-40(fp)
	subl3	$2,-40(fp),r0
	addl2	4(ap),r0
	cmpb	(r0),$46
	jneq	L51
	subl3	$2,-40(fp),r0
	addl2	4(ap),r0
	clrb	(r0)
	subl3	$1,-40(fp),r0
	addl2	4(ap),r0
	cmpb	(r0),$70
	jneq	L52
	jbr	L48
L52:
	subl3	$1,-40(fp),r0
	addl2	4(ap),r0
	cmpb	(r0),$84
	jneq	L53
	pushl	$0
	pushl	4(ap)
	calls	$2,_bopen
	movl	r0,_bt
L53:
L51:
	tstl	_bt
	jneq	L54
	pushl	4(ap)
	calls	$1,_perror
	jbr	L48
L54:
L49:
	pushal	-32(fp)
	movl	_bt,r0
	pushl	64(r0)
	calls	$2,_fstat
	.data	1
L58:

	.byte	0x25,0x73,0x2e,0x54,0x20,0x25,0x6c,0x64
	.byte	0x20,0x62,0x79,0x74,0x65,0x73,0x0
	.text
	pushl	-16(fp)
	pushl	4(ap)
	pushl	$L58
	calls	$3,_printf
	movl	_bt,r0
	tstl	68(r0)
	jleq	L59
	pushal	-32(fp)
	movl	_bt,r0
	pushl	68(r0)
	calls	$2,_fstat
	tstl	r0
	jneq	L59
L60:
	.data	1
L61:

	.byte	0x2c,0x20,0x25,0x73,0x2e,0x46,0x20,0x25
	.byte	0x6c,0x64,0x20,0x62,0x79,0x74,0x65,0x73
	.byte	0x0
	.text
	pushl	-16(fp)
	pushl	4(ap)
	pushl	$L61
	calls	$3,_printf
L59:
	decl	__iob+16
	jlss	L62
	movb	$10,r0
	movl	__iob+20,r1
	incl	__iob+20
	movb	r0,(r1)
	jbr	L63
L62:
	pushl	$__iob+16
	pushl	$10
	calls	$2,__flsbuf
L63:
	pushl	$0
	calls	$1,_donode
	clrl	-40(fp)
	movl	-40(fp),-36(fp)
	jbr	L67
L68:
	movl	-40(fp),r0
	ashl	$10,_ndcnt[r0],r1
	addl2	r1,-36(fp)
	incl	-40(fp)
L67:
	cmpl	-40(fp),$5
	jleq	L68
L66:
	.data	1
L69:

	.byte	0x25,0x6c,0x64,0x20,0x62,0x79,0x74,0x65
	.byte	0x73,0x20,0x75,0x73,0x65,0x64,0x20,0x69
	.byte	0x6e,0x20,0x74,0x72,0x65,0x65,0xa,0x0
	.text
	pushl	-36(fp)
	pushl	$L69
	calls	$2,_printf
	clrl	-40(fp)
	jbr	L72
L73:
	.data	1
L74:

	.byte	0x20,0x25,0x6c,0x64,0x20,0x6e,0x6f,0x64
	.byte	0x65,0x73,0x20,0x61,0x74,0x20,0x6c,0x65
	.byte	0x76,0x65,0x6c,0x20,0x25,0x64,0x0
	.text
	pushl	-40(fp)
	movl	-40(fp),r0
	pushl	_ndcnt[r0]
	pushl	$L74
	calls	$3,_printf
	incl	-40(fp)
L72:
	movl	_bt,r0
	cvtbl	28(r0),r0
	cmpl	-40(fp),r0
	jleq	L73
L71:
	.data	1
L75:

	.byte	0xa,0x25,0x6c,0x64,0x20,0x62,0x79,0x74
	.byte	0x65,0x73,0x20,0x66,0x72,0x65,0x65,0xa
	.byte	0x0
	.text
	pushl	_frcnt
	pushl	$L75
	calls	$2,_printf
	.data	1
L76:

	.byte	0x25,0x6c,0x64,0x20,0x72,0x65,0x63,0x6f
	.byte	0x72,0x64,0x73,0x20,0x74,0x6f,0x74,0x61
	.byte	0x6c,0x6c,0x69,0x6e,0x67,0x20,0x25,0x6c
	.byte	0x64,0x20,0x62,0x79,0x74,0x65,0x73,0xa
	.byte	0x0
	.text
	pushl	_reclen
	pushl	_reccnt
	pushl	$L76
	calls	$3,_printf
	pushl	_bt
	calls	$1,_bclose
	.stabs	"i",0x80,0,4,40
	.stabs	"x",0x80,0,4,36
	.stabs	"statbuf",0x80,0,8,32
	.stabs	"stat",0xfa,0,0,0
L48:
	ret
	.set	L.R2,0x0
	.set	L.SO2,0x28
L78:	.stabs	"doarg",0xf4,0,59,L78
	.data
	.text
	.align	2
	.globl	_donode
_donode:
	.word	L.R3
	subl2	$L.SO3,sp
	.stabs	"donode",0x24,0,64,_donode
	.stabs	"donode",0x20,0,68,0
	.stabs	"n",0xa0,0,4,4
	moval	-1024(fp),-1028(fp)
	pushl	$0
	ashl	$10,4(ap),-(sp)
	movl	_bt,r0
	pushl	64(r0)
	calls	$3,_lseek
	pushl	$1024
	pushal	-1024(fp)
	movl	_bt,r0
	pushl	64(r0)
	calls	$3,_read
	movl	r0,-1032(fp)
	cmpl	-1032(fp),$1024
	jeql	L82
	.data	1
L83:

	.byte	0x62,0x74,0x72,0x65,0x70,0x6f,0x72,0x74
	.byte	0x3a,0x20,0x61,0x74,0x74,0x65,0x6d,0x70
	.byte	0x74,0x20,0x74,0x6f,0x20,0x72,0x65,0x61
	.byte	0x64,0x20,0x6e,0x6f,0x64,0x65,0x20,0x25
	.byte	0x64,0x20,0x66,0x61,0x69,0x6c,0x65,0x64
	.byte	0xa,0x0
	.text
	pushl	4(ap)
	pushl	$L83
	calls	$2,_printf
	.data	1
L84:

	.byte	0x62,0x72,0x65,0x70,0x6f,0x72,0x74,0x0
	.text
	pushl	$L84
	calls	$1,_perror
	pushl	$1
	calls	$1,_exit
L82:
	movl	-1028(fp),r0
	cvtbl	7(r0),r0
	incl	_ndcnt[r0]
	movl	-1028(fp),r0
	cvtwl	1022(r0),r0
	addl2	r0,_frcnt
	movl	-1028(fp),r0
	tstb	7(r0)
	jeql	L85
	clrl	-1032(fp)
	jbr	L88
L89:
	addl3	$1022,-1028(fp),r0
	addl3	-1032(fp),-1032(fp),r1
	addl2	r1,r1
	subl2	r1,r0
	pushl	-4(r0)
	calls	$1,_donode
	incl	-1032(fp)
L88:
	movl	-1028(fp),r0
	cvtwl	4(r0),r0
	cmpl	-1032(fp),r0
	jleq	L89
L87:
	jbr	L90
L85:
	clrl	-1032(fp)
	jbr	L93
L94:
	incl	_reccnt
	movl	-1028(fp),r0
	bitb	$2,6(r0)
	jneq	L95
	addl3	$1022,-1028(fp),r0
	addl3	-1032(fp),-1032(fp),r1
	addl2	r1,r1
	addl2	r1,r1
	subl2	r1,r0
	movzwl	-4(r0),r0
	addl2	r0,_reclen
L95:
	incl	-1032(fp)
L93:
	movl	-1028(fp),r0
	cvtwl	4(r0),r0
	cmpl	-1032(fp),r0
	jlss	L94
L92:
L90:
	.stabs	"i",0x80,0,4,1032
	.stabs	"b",0x80,0,40,1028
	.stabs	"hdr",0xfa,0,0,0
	.stabs	"buf",0x80,0,98,1024
	.stabn	0xfc,0,0,1024
L79:
	ret
	.set	L.R3,0x0
	.set	L.SO3,0x408
L96:	.stabs	"donode",0xf4,0,85,L96
	.data
	.align	2
_VER:
	.long	_VER
	.data	2
L98:

	.byte	0xa,0x38,0x31,0x2f,0x34,0x2f,0x32,0x36
	.byte	0x3a,0x62,0x74,0x72,0x65,0x70,0x6f,0x72
	.byte	0x74,0x2e,0x63,0xa,0x0
	.data
	.long	L98
	.stabs	"cbt.h$73",0x5c,0,8,0
	.stabs	"lloc",0x60,0,4,0
	.stabs	"llen",0x60,0,13,4
	.stabs	"cbt.h$73",0x5e,0,8,8
	.stabs	"cbt.h$61",0x5c,0,8,0
	.stabs	"tfree",0x60,0,3,0
	.stabs	"cbt.h$61",0x5e,0,8,2
	.stabs	"hdr",0x5c,0,8,0
	.stabs	"hstamp",0x60,0,4,0
	.stabs	"kcnt",0x60,0,3,4
	.stabs	"htype",0x60,0,2,6
	.stabs	"hlev",0x60,0,2,7
	.stabs	"hdr",0x5e,0,8,8
	.stabs	"bfile",0x5c,0,8,0
	.stabs	"next",0x60,0,40,0
	.stabs	"bfile",0xfa,0,0,0
	.stabs	"path",0x60,0,232,4
	.stabs	"hdr",0xfa,0,0,0
	.stabn	0xfc,0,0,6
	.stabs	"height",0x60,0,2,28
	.stabs	"advnc",0x60,0,2,29
	.stabs	"rdwrt",0x60,0,2,30
	.stabs	"flag",0x60,0,98,31
	.stabn	0xfc,0,0,6
	.stabs	"loc",0x60,0,100,40
	.stabn	0xfc,0,0,6
	.stabs	"tfd",0x60,0,4,64
	.stabs	"dfd",0x60,0,4,68
	.stabs	"fname",0x60,0,34,72
	.stabs	"altname",0x60,0,34,76
	.stabs	"rdptr",0x60,0,8,80
	.stabs	"rdptr",0xfa,0,0,0
	.stabs	"fatal",0x60,0,2,336
	.stabs	"bfile",0x5e,0,8,340
	.stabs	"_iob",0x20,0,104,0
	.stabs	"_iobuf",0xfa,0,0,0
	.stabn	0xfc,0,0,120
	.stabs	"_iobuf",0x5c,0,8,0
	.stabs	"_cnt",0x60,0,4,0
	.stabs	"_ptr",0x60,0,44,4
	.stabs	"_base",0x60,0,44,8
	.stabs	"_flag",0x60,0,3,12
	.stabs	"_file",0x60,0,2,14
	.stabs	"_iobuf",0x5e,0,8,16
	.stabs	"VER",0x26,0,8,_VER
	.stabs	"D",0xfa,0,0,0
	.stabs	"bt",0x20,0,40,0
	.stabs	"bfile",0xfa,0,0,0
	.stabs	"reclen",0x20,0,4,0
	.stabs	"reccnt",0x20,0,4,0
	.stabs	"frcnt",0x20,0,4,0
	.stabs	"ndcnt",0x20,0,100,0
	.stabn	0xfc,0,0,6
	.stabs	"stat",0x5c,0,8,0
	.stabs	"st_dev",0x60,0,13,0
	.stabs	"st_ino",0x60,0,13,2
	.stabs	"st_mode",0x60,0,13,4
	.stabs	"st_nlink",0x60,0,3,6
	.stabs	"st_uid",0x60,0,3,8
	.stabs	"st_gid",0x60,0,3,10
	.stabs	"st_rdev",0x60,0,13,12
	.stabs	"st_size",0x60,0,4,16
	.stabs	"st_atime",0x60,0,4,20
	.stabs	"st_mtime",0x60,0,4,24
	.stabs	"st_ctime",0x60,0,4,28
	.stabs	"stat",0x5e,0,8,32
	.text
L99:	.stabs	"btreport.c",0x94,0,87,L99
