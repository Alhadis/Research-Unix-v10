	.file	"strchr.s"
#	@(#)strchr.s	1.5
#	3.0 SID #	1.2
# Fast assembler language version of the following C-program
#			strchr
# which represents the `standard' for the C-library.

# Return the ptr in sp at which the character c appears;
#   NULL if not found
#	
#	char *
#	strchr(sp, c)
#	register char *sp, c;
#	{
#		do {
#			if (*sp == c)
#				return(sp);
#		} while (*sp++);
#		return(0);
#	}
	.globl	_strchr
	.text
	.align	2
_strchr:
	.word	0x0000		# pgm uses regs 0,1,2,3,4
	movq	4(ap),r2	# s1 in r2; `c' in r3
L0:
	locc	$0,$65535,(r2)	# search for a terminating null
	bneq	L1		# hop onward if we found one
	locc	r3,$65535,(r2)	# else look for `c' in this BLOCK
	bneq	L2		# hop if found (r1 points to it)
	movl	r1,r2		# no luck at all in this BLOCK
	jmp	L0		# go back to try next BLOCK
L1:
	subl3	r2,r1,r4	# length of (this segment of) s1 in r4
	incl	r4		# gotta include terminal null
	locc	r3,r4,(r2)	# scan for `c' in this (tail) segment
	beql	L3		# skip if not found! (r0 is already 0)
L2:
	movl	r1,r0		# else found one, return pointer to user
L3:
	ret
