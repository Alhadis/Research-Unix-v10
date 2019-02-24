# enter(&prog,arg)
# will execute prog(arg) and return zero
# if a "leave(str)" is executed, an immediate return
# from the caller to the lowest level enter will be
# executed, and will return(str) to the caller of enter.

	.globl	_write
	.globl	_exit

	.text

	.globl	_enter
_enter:
	.word	0
	pushl	savesp		# save previous stack pointer
	pushl	fp		# save the current frame pointer
	movl	sp,savesp	# the saved stack pointer
	pushl	8(ap)		# fetch the argument
	calls	$1,*4(ap)	# call the function
	clrl	r0		# normal return
rtn:
	movl	savesp,sp	# get old stack
	movl	(sp)+,fp	# and old fp
	movl	(sp)+,savesp	# and restore old exit
	ret

	.globl	_leave

_leave:
	.word	0
	movl	4(ap),r0	# fetch return value
	tstl	savesp
	bneq	rtn		# and exit
	pushl	$18
	pushl	$msg
	pushl	$2
	calls	$3,_write
	clrl	-(sp)
	calls	$1,_exit

	.data
msg:
	.ascii	"bad call to leave\12"

	.text
	.lcomm	savesp,4
