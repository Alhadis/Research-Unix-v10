	.file	"strncmp.s"
#	@(#)strncmp.s	1.4
# Fast assembler language version of the following C-program
#			strncmp
# which represents the `standard' for the C-library.

# Compare strings (at most n bytes): return *s1-*s2 for the last
# characters in s1 and s2 which were compared.

#	strncmp(s1, s2, n)
#	register char *s1, *s2;
#	register n;
#	{
#		if(s1 == s2)
#			return(0);
#		while(--n >= 0 && *s1 == *s2++)
#			if(*s1++ == '\0')
#				return(0);
#		return((n < 0)? 0: *s1 - *--s2);
#	}
	.globl	_strncmp
	.text
	.align	2
_strncmp:
	.word	0x0000		# uses regs. 0,1,2,3,4,5
	movl	12(ap),r4	# arg `n'
	bgtr	L1		#      should be > 0
L0:
	clrl	r0		# else return a zero
	ret
L1:
	movl	4(ap),r1	# working copy of s1
	movl	8(ap),r3	# working copy of s2
	cmpl	r1,r3		# if pointers are eql, strings are also
	beql	L0		# return a zero
L2:
	subl3	$65535,r4,r5	# memory for whether `n' <= 65535
	blss	L3		# skip if r4 is not too big
	movl	$65535,r4	# else use big number (remdr in r5)
L3:
	movl	r1,r2		# hide r1 while searching for end
	locc	$0,r4,(r1)	# search for terminal null in s1
	beql	L4		# hop, null not found
	clrl	r5		# found, we sure won't need to go again
	subl3	r2,r1,r4	# length of s1
	incl	r4		# add 1 to account for the null
L4:
	movl	r2,r1		# restore copy of s1 for the cmpc3
	cmpc3	r4,(r1),(r3)	# Compare s1 and s2
	beql	L5		# skip if strings are ==, (so far)
	subb3	(r3),(r1),r0	# else difference is returned
	cvtbl	r0,r0		# gotta make it an int
	ret
L5:
	movl	r5,r4
	bgtr	L2		# if more compare is needed, go back
	ret			# note that r0 is already 0 in this case
