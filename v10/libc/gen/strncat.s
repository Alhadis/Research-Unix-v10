	.file	"strncat.s"
#	@(#)strncat.s	1.6
#	3.0 SID #	1.2
# Fast assembler language version of the following C-program
#			strncat
# which represents the `standard' for the C-library.

# Concatenate s2 on the end of s1.  S1's space must be large enough.
# At most n characters are moved.  Return (null terminated) s1.

#	char *
#	strncat(s1, s2, n)
#	register char *s1, *s2;
#	register n;
#	{
#		register char *os1;
#	
#		os1 = s1;
#		while(*s1++)
#			;
#		--s1;
#		while(*s1++ = *s2++)
#			if(--n < 0) {
#				*--s1 = '\0';
#				break;
#			}
#		return(os1);
#	}
	.globl	_strncat
	.text
	.align	2
_strncat:
	.word	0x03c0		# uses regs. 0,1,2,3,4,5,6,7,8,9
	movq	4(ap),r6	# dest in r6, src in r7
	movl	12(ap),r8	# max src length (arg `n')
	bleq	L6		# done if <= 0
	movl	r6,r1		# working copy of dest
L0:
	locc	$0,$65535,(r1)	# gotta find the end of dest
	beql	L0		# BIG string, go around again
	movl	r1,r3		# found the null to cat onto
L1:
	subl3	$65535,r8,r9	# may have to take it in chunks
	blss	L2		# skip if `n' < 65535
	movl	$65535,r8	# else use 65535 as `chunk' length
L2:
	locc	$0,r8,(r7)	# see if there's a null in src
	bneq	L3		# found null, go move string & quit
	movc3	r8,(r7),(r3)	# else move a `chunk' of data
	tstl	r9		# ask if `n' is bigger than 65535
	bleq	L5		# skip to finish if `n' was small
	movl	r9,r8		# else reduce `n' by 65535,
	movl	r1,r7		# update source ptr,
	jmp	L1		# and go again
L3:
	subl3	r7,r1,r5	# calculate length of piece to move
	cmpl	r8,r5		# is `n' <= length of src?
	bleq	L4		# YES, then hop onward and use `n'
	movl	r5,r8		# NO, then use length
L4:
	movc3	r8,(r7),(r3)	# and then move it
L5:
	movb	$0,(r3)		# stick in the terminating null
L6:
	movl	r6,r0		# return pointer to dest
	ret
