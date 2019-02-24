# count subroutine called during profiling

.globl	mcount
.globl	__mcbase

mcount:
	movl	(r0),r1
	beql	init
incr:
	incl	(r1)
return:
	rsb
init:
	movl	__mcbase,r1
	beql	return
	addl2	$8,__mcbase
	movl	(sp),(r1)+
	movl	r1,(r0)
	brb 	incr
