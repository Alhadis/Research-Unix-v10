	.file	"memccpy.s"
#	@(#)memccpy.s	1.5
# Fast assembler language version of the following C-program
#			memccpy
# which represents the `standard' for the C-library.

# Copy s2 to s1, stopping if character c is copied. Copy no more than n bytes.
# Return a pointer to the byte after character c in the copy,
# or NULL if c is not found in the first n bytes.

#	char *
#	memccpy(s1, s2, c, n)
#	register char *s1, *s2;
#	register int c, n;
#	{
#		while (--n >= 0)
#			if ((*s1++ = *s2++) == c)
#				return (s1);
#		return (0);
#	}
	.globl	_memccpy
	.text
	.align	2
_memccpy:
	.word	0x00c0		# uses regs. 0,1,2,3,4,5,6,7
	movq	12(ap),r6	# escape char in r6, len in r7
	tstl	r7		# source length (arg `n')
	bleq	L2		# n <= 0, so return now!
	movq	4(ap),r3	# dest ptr in r3, src ptr in r4
	movl	r4,r1		# src ptr in r1 also
L0:
	cmpl	$65535,r7	# is max len VERY large?
	bgeq	L1		# hop if not
	locc	r6,$65535,(r1)	# look for escape char in src.
	bneq	L3		# found it
	movc3	$65535,(r4),(r3)	# copy all the chars
	movl	r1,r4		# updated src ptr
	subl2	$65535,r7
	jbr	L0		# go look at some more chars
L1:
	locc	r6,r7,(r1)	# look for escape char in src.
	bneq	L4		# found it
	movc3	r7,(r4),(r3)	# copy all the chars
	ret			# done (r0 is now 0)
L2:
	clrl	r0		# char not found, return zero
	ret
L3:
	movl	$65535,r7	# len of BLOCK when len VERY large
L4:
	subl2	r0,r7		# number of bytes to escape char
	incl	r7		# move the escape char also
	movc3	r7,(r4),(r3)	# copy the chars
	movl	r3,r0		# we're done - return ptr to next dest char
	ret
