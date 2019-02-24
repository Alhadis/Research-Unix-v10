	.file	"strcpy.s"
#	@(#)strcpy.s	1.5
# Fast assembler language version of the following C-program
#			strcpy
# which represents the `standard' for the C-library.

# Copy string s2 to s1.  s1 must be large enough. Return s1.

#	char *
#	strcpy(s1, s2)
#	register char *s1, *s2;
#	{
#		register char *os1;
#	
#		os1 = s1;
#		while (*s1++ = *s2++)
#			;
#		return(os1);
#	}
	.globl	_strcpy
	.text
	.align	2
_strcpy:
	.word	0x00c0			# use regs. 0,1,2,3,4,5,6,7
	movq	4(ap),r6		# dest. in r6, src. in r7
	movl	r6,r3			# working copy of dest. in r3
L0:
	locc	$0,$65535,(r7)		# try to find null in src.
	bneq	L1			# skip if null found -
	movc3	$65535,(r7),(r3)	# else move BLOCK of chars
	movl	r1,r7			#   and adjust pointers to
	jmp	L0			#   go back and try again
L1:
	subl2	r7,r1			# length (but 1) of src.
	incl	r1			# adjust length to cover null
	movc3	r1,(r7),(r3)		# move string including null
	movl	r6,r0			# return dest.
	ret
