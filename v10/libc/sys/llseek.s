# C library -- llseek

# (hi,lo) = llseek(file, offset, ptr);
#  (-1,-1) in case of error

	.set	llseek,11
.globl	_llseek
.globl  cerror
.globl	_errno
.lcomm	rv,8

_llseek:
	.word	0x0000
	chmk	$llseek
	bcc 	noerror
	movl	r0,_errno
	movl	$-1,r0
	movl	$-1,r1
noerror:
	movl	r0,rv
	movl	r1,rv+4
	movab	rv,r0
	ret
