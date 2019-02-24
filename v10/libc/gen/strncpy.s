	.file	"strncpy.s"
#	@(#)strncpy.s	1.6
# Fast assembler language version of the following C-program
#			strncpy
# which represents the `standard' for the C-library.

# Copy s2 to s1, truncating or null-padding to always copy n bytes.
# Return s1

#	char *
#	strncpy(s1, s2, n)
#	register char *s1, *s2;
#	register int n;
#	{
#		register char *os1 = s1;
#	
#		while (--n >= 0)
#			if ((*s1++ = *s2++) == '\0')
#				while (--n >= 0)
#					*s1++ = '\0';
#		return (os1);
#	}
	.globl	_strncpy
	.text
	.align	2
_strncpy:
	.word	0x03c0		# uses regs. 0,1,2,3,4,5,6,7,8,9
	movl	12(ap),r5	# source length (arg `n')
	bleq	L4		# n <= 0, so return now!
	movq	4(ap),r3	# dest ptr
	movl	r4,r6		# working copy of src ptr
	movl	r5,r2		# copy of `n' for dest. length
	movl	$65535,r9	# max length for vax string instructions
L0:
	subl3	r9,r5,r8	# is max src.len VERY large?
	bleq	L1		# hop if not
	movl	r9,r5		# wow, src.limit > 65535, (and r8 > 0)
L1:
	locc	$0,r5,(r6)	# look for terminal null in src.
	beql	L2		# none? then r5 is right, so hop
	subl3	r6,r1,r5	# else src.len < `n'. (put in r5).
	clrl	r8		# indicate no more (src) iterations
L2:
	subl3	r9,r2,r7	# is dest.len VERY large?
	bleq	L3		# hop if not
	movl	r9,r2		# dest.limit was > 65535 (and r7 > 0)
L3:
	movc5	r5,(r6),$0,r2,(r3)	# Move n bytes, pad with nulls
	movl	r7,r2		# was dst.ln > 65535?
	bleq	L4		# NO, we're done
	movl	r8,r5		# YES, was src.ln also > 65535?
	bgtr	L0		# YES, then go move some more
	clrl	r5		# NO, set up to pad nulls
	jbr	L2		#   to dst.ln `n'
L4:
	movl	4(ap),r0	# we're done - return s1
	ret
