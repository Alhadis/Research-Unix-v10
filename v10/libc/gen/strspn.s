	.file	"strcpn.s"
#	@(#)strspn.s	1.6
# Fast assembler language version of the following C-program
#			strspn
# which represents the `standard' for the C-library.

#	Return the number of characters in the maximum leading segment
#	of string which consists solely of characters from charset.

#	int
#	strspn(string, charset)
#	char	*string;
#	register char	*charset;
#	{
#		register char *p, *q;
#	
#		for(q=string; *q != '\0'; ++q) {
#			for(p=charset; *p != '\0' && *p != *q; ++p)
#				;
#			if(*p == '\0')
#				break;
#		}
#		return(q-string);
#	}
	.globl	_strspn
	.text
	.align	2
_strspn:
	.word	0x0000			# pgm uses regs 0,1,2,3,4
	movab	-256(sp),sp		# get table space
	movc5	$0,(ap),$0,$256,(sp)	# and zero it out
	movl	8(ap),r0		# charset ptr in r0
	beql	L5			# return 0 if arg was NULL
L0:
	movzbl	(r0)+,r3		# byte from charset for index
	beql	L1			# hop if found ending NULL
	movb	$255,(sp)[r3]		# make entry in spanc table
	jmp	L0			# get next byte from charset
L1:
	movl	4(ap),r1		# string ptr in r1
	beql	L4			# if it's NULL, return 0
L2:
	movl	r1,r4			# preserve a copy of this ptr
	locc	$0,$65535,(r1)		# get length of string
	bneq	L3			# skip if string not > 65535
	spanc	$65535,(r4),(sp),$255	# else spanc the first 65535
	bneq	L4			# hop ahead if we ended the span
	jmp	L2			# else go back and do next 65535
L3:
	subl3	r4,r1,r0		# length to end of string in r0
	spanc	r0,(r4),(sp),$255	# this is the main function
L4:
	subl3	4(ap),r1,r0		# length of the span
L5:
	movab	256(sp),sp		# restore the stack pointer
	ret
