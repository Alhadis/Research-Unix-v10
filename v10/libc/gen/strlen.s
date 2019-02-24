	.file	"strlen.s"
#	@(#)strlen.s	1.5
# Fast assembler language version of the following C-program
#			strlen
# which represents the `standard' for the C-library.

# Given string s, return length (not including the terminating null).

#	strlen(s)
#	register char *s;
#	{
#		register n;
#	
#		n = 0;
#		while (*s++)
#			n++;
#		return(n);
#	}
	.globl	_strlen
	.text
	.align	2
_strlen:
	.word	0x0000		# pgm uses regs 0,1,2
	movl	4(ap),r2	# ptr to s1
	movl	r2,r1		# workin copy in r1
L0:
	locc	$0,$65535,(r1)	# search for a terminating null
	beql	L0		# go back for more if string is BIG!
	subl3	r2,r1,r0	# calculate string length in r0
	ret
