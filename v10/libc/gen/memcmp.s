	.file	"memcmp.s"
#	@(#)memcmp.s	1.5
# Fast assembler language version of the following C-program for
#			memcmp
# which represents the `standard' for the C-library.

# Compare n bytes:  s1>s2: >0  s1==s2: 0  s1<s2: <0

#	int
#	memcmp(s1, s2, n)
#	register char *s1, *s2;
#	register int n;
#	{
#		int diff;
#
#		if (s1 != s2)
#			while (--n >= 0)
#				if (diff = *s1++ - *s2++)
#					return (diff);
#		return (0);
#	}
	.globl	_memcmp
	.text
	.align	2
_memcmp:
	.word	0x0000		# uses regs. 0,1,2,3,4
	movl	12(ap),r4	# arg `n'
	bleq	L4		#      should be > 0
	movq	4(ap),r1	# s1 in r1; s2 in r2
	cmpl	r1,r2		# if pointers are eql, strings are also
	beql	L4		# return a zero
	movl	r2,r3		# s2
L0:
	cmpl	$65535,r4	# check for whether `n' <= 65535
	bgeq	L1		# skip if r4 is not too big
	cmpc3	$65535,(r1),(r3)	# Compare s1 and s2
	bneq	L2		# skip if strings are !=
	subl2	$65535,r4
	jbr	L0		# more compare is needed, go back
L1:
	cmpc3	r4,(r1),(r3)	# Compare s1 and s2
	beql	L3		# skip if strings are ==
L2:
	subb3	(r3),(r1),r0	# difference is returned
	cvtbl	r0,r0		# gotta make it an int
L3:
	ret			# else return a zero (r0 = 0 already)
L4:
	clrl	r0		# n <= 0; return a zero
	ret
