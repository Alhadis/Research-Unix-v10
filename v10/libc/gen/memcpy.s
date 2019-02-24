	.file	"memcpy.s"
#	@(#)memcpy.s	1.5
# Fast assembler language version of the following C-program
#			memcpy
# which represents the `standard' for the C-library.

# Copy s2 to s1, always copy n bytes.
# Return s1

#	char *
#	memcpy(s1, s2, n)
#	register char *s1, *s2;
#	register int n;
#	{
#		register char *os1 = s1;
#	
#		while (--n >= 0)
#			*s1++ = *s2++;
#		return (os1);
#	}
	.globl	_memcpy
	.text
	.align	2
_memcpy:
	.word	0x0040		# uses regs. 0,1,2,3,4,5,6
	movl	12(ap),r6	# length (arg `n')
	bleq	L2		# n <= 0, so return now!
	movq	4(ap),r3	# dest ptr s1
	movl	r4,r1		# src ptr s2
L0:
	cmpl	$65535,r6	# is len VERY large?
	bgeq	L1		# hop if not
	movc3	$65535,(r1),(r3)	# Move n bytes
	subl2	$65535,r6
	jbr	L0		# go move some more string
L1:
	movc3	r6,(r1),(r3)	# Move n bytes
L2:
	movl	4(ap),r0	# we're done - return s1
	ret
