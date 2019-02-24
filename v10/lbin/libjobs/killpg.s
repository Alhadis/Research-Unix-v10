# C library -- killpg

	.set	kill,37
.globl	_killpg
.globl cerror

_killpg:
	.word	0x0000
	mnegl	4(ap),4(ap)		# kill with - pid is killpg
	chmk	$kill
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
