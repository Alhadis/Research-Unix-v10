	.file	"strcmp.s"
#	@(#)strcmp.s	1.4
# Fast assembler language version of the following C-program for
#			strcmp
# which represents the `standard' for the C-library.

#	strcmp(s1, s2)
#	register char *s1, *s2;
#	{
#		if(s1 == s2)
#			return(0);
#		while(*s1 == *s2++)
#			if(*s1++ == '\0')
#				return(0);
#		return(*s1 - *--s2);
#	}
	.globl	_strcmp
	.text
	.align	2
_strcmp:
	.word	0x0000		# uses regs. 0,1,2,3,4
	movq	4(ap),r3	# s1 in r3; s2 in r4
	cmpl	r3,r4		# if pointers are eql, don't bother
	bneq	L0
	clrl	r0
	ret
L0:
	locc	$0,$65535,(r4)	# search for a terminating null in s2
	bneq	L1		# found a null, go start comparison
	cmpc3	$65535,(r4),(r3)
	bneq	L2		# no null in s2, but strings differ
	movl	r1,r4		# hey, first BLOCK of chars are same.
	jmp	L0		# let's go continue this loop.
L1:
	subl2	r4,r1		# length of (final segment) of s2 in r1
	incl	r1		# gotta include null in comparison
	cmpc3	r1,(r4),(r3)	# strings compared.
	beql	L3		# complete equality - r0 is already 0
L2:
	cvtbl	(r3),r0
	cvtbl	(r1),r1
	subl2	r1,r0
L3:
	ret
