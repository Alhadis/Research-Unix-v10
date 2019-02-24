#
# init code, executed by process 1
# this should be pc-relative
#

	.text

	.globl	_icode
	.globl	_szicode

_icode:
	pushab	4f	# environment
	pushab	2f	# arg list
	pushab	1f	# program
	pushl	$3	# arg count
	movl	bootflags,r11	# old init expects flags here
	movab	ireg,r0
	bbs	$1,r11,0f	# bit 1 (sic) means single user
	 movab	iauto,r0
0:	movl	r0,2f+4	# init first arg
	movl	sp,ap	# args
	chmk	$59	# exece
0:	brb	0b	# loop on error

1:
	.asciz	"/etc/init"
2:
	.long	3f-_icode
	.long	0
4:	.long	0
3:
	.asciz	"init"
iauto:	.asciz	"a"
ireg:	.asciz	"s"
bootflags:
	.long	0

_szicode:
	.long	_szicode-_icode


#
# signal trampoline code
# its length is (stupidly) well-known
# to be 12 bytes
#

	.globl	_sigcode
	.globl	_szsigcode
	.align	2
_sigcode:
	calls	$3,1(pc)
	rei
	.word	0x7f
	callg	(ap),*12(ap)
	ret

_szsigcode:
	.long	_szsigcode-_sigcode
