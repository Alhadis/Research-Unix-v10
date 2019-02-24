	.file	"memset.s"
#	@(#)memset.s	1.5
# Fast assembler language version of the following C-program
#			memset
# which represents the `standard' for the C-library.

# Set an array of n chars starting at sp to the character c.
#	
#	char *
#	memset(sp, c, n)
#	register char *sp, c;
#	register int n;
#	{
#		register char *sp0 = sp;
#
#		while (--n >= 0)
#			*sp++ = c;
#		return (sp0);
#	}
	.globl	_memset
	.text
	.align	2
_memset:
	.word	0x00c0		# pgm uses regs 0,1,2,3,4,5,6,7
	movl	12(ap),r6	# n in r6
	bleq	L2		#	should be > 0
	movq	4(ap),r3	# s1 in r3; `c' in r4
	movl	r4,r7		# r0 to r5 are clobbered by movc5
L0:
	cmpl	$65535,r6	# is len VERY large?
	bgeq	L1		# hop if not
	movc5	$0,(ap),r7,$65535,(r3)	# copy `c' into this BLOCK
	subl2	$65535,r6	# compute remaining len
	jbr	L0		# go back to fill some more
L1:
	movc5	$0,(ap),r7,r6,(r3)	# copy `c' into this BLOCK
L2:
	movl	4(ap),r0	# we're done - return s1
	ret
