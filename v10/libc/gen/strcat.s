	.file	"strcat.s"
#	@(#)strcat.s	1.5
# Fast assembler language version of the following C-program
#			strcat
# which represents the `standard' for the C-library.

# Concatenate s2 on the end of s1.  S1's space must be large enough.
# Return s1.

#	char *
#	strcat(s1, s2)
#	register char *s1, *s2;
#	{
#		register char *os1;
#	
#		os1 = s1;
#		while (*s1++)
#			;
#		--s1;
#		while (*s1++ = *s2++)
#			;
#		return(os1);
#	}
	.globl	_strcat
	.text
	.align	2
_strcat:
	.word	0x01c0		# uses regs. 0,1,2,3,4,5,6,7,8
	movl	$65535,r8	# constant used several times
	movq	4(ap),r6	# dest. in r6, src. in r7
	movl	r6,r1		# working copy of dest.
L0:
	locc	$0,r8,(r1)	# gotta find null in dest, so we
	beql	L0		#   know where to start `copy-to'.
	movl	r1,r3		# set-up r3 for copy destination.
L1:
	locc	$0,r8,(r7)	# try to find null in src.
	bneq	L2		# skip if null found -
	movc3	r8,(r7),(r3)	# else move a BLOCK of 65535 chars
	movl	r1,r7		#   and adjust pointers to
	jmp	L1		#   go back and try again
L2:
	subl2	r7,r1		# length (but 1) of src.
	incl	r1		# adjust length to cover null
	movc3	r1,(r7),(r3)	# move string including null
	movl	r6,r0		# return destination ptr.
	ret
