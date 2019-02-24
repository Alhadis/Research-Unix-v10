# C library -- select

# select(nfd, rfdset, wfdset, time);
#

	.set	select,38
.globl	_select
.globl  cerror

_select:
	.word	0x0000
	chmk	$select
	bcc 	noerror
	jmp 	cerror
noerror:
	ret
