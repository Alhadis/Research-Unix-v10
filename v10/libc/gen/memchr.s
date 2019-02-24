	.file	"memchr.s"
#	@(#)memchr.s	1.5
# Fast assembler language version of the following C-program
#			memchr
# which represents the `standard' for the C-library.

# Return the ptr in sp at which the character c appears;
#   NULL if not found in n chars; don't stop at \0.
#	
#	char *
#	memchr(sp, c, n)
#	register char *sp, c;
#	register int n;
#	{
#		while (--n >= 0)
#			if (*sp++ == c)
#				return (--sp);
#		return (0);
#	}
	.globl	_memchr
	.text
	.align	2
_memchr:
	.word	0x0000		# pgm uses regs 0,1,2,3
	movl	12(ap),r3	# n in r3
	bleq	L4		#	should be > 0
	movq	4(ap),r1	# s1 in r1; `c' in r2
L0:
	cmpl	$65535,r3	# is len VERY large?
	bgeq	L1		# hop if not
	locc	r2,$65535,(r1)	# look for `c' in this BLOCK
	bneq	L2		# hop if found (r1 points to it)
	subl2	$65535,r3
	jbr	L0
L1:
	locc	r2,r3,(r1)	# look for `c' in this BLOCK
	beql	L3		# fall through if found (r1 points to it)
L2:
	movl	r1,r0		# found one, return pointer to user
L3:
	ret			# return 0 if not found (r0 is 0 already)
L4:
	clrl	r0		# n <= 0, return 0 (not found)
	ret
