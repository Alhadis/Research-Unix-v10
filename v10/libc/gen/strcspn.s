	.file	"strcspn.s"
#	@(#)strcspn.s	1.5
# Fast assembler language version of the following C-program
#			strcspn
# which represents the `standard' for the C-library.

#	Return the number of characters in the maximal leading segment
#	of `string' which consists only of characters NOT in `charset'.

#	int
#	strcspn(string, charset)
#	char	*string;
#	register char	*charset;
#	{
#		register char *p, *q;

#		for(q=string; *q != '\0'; ++q) {
#			for(p=charset; *p != '\0' && *p != *q; ++p)
#				;
#			if(*p != '\0')
#				break;
#		}
#		return(q-string);
#	}
	.globl	_strcspn
	.text
	.align	2
_strcspn:
	.word	0x0000			# pgm uses regs 0,1,2,3,4
	movab	-256(sp),sp		# get table space
	movc5	$0,(ap),$0,$256,(sp)	# and zero it out
	movl	8(ap),r0		# charset ptr in r0
	bneq	L1			# go ahead if arg non-NULL
	movl	4(ap),r1		# string ptr in r1
L0:
	locc	$0,$65535,(r1)		# NULL charset - return length
	beql	L0			# of string arg
	jmp	L5
L1:
	movzbl	(r0)+,r3		# byte from charset for index
	beql	L2			# hop if found ending NULL
	movb	$255,(sp)[r3]		# make entry in scanc table
	jmp	L1			# get next byte from charset
L2:
	movl	4(ap),r1		# string ptr in r1
	beql	L5			# if it's NULL, return 0
L3:
	movl	r1,r4			# preserve a copy of this ptr
	locc	$0,$65535,(r1)		# get length of string
	bneq	L4			# skip if string not > 65535
	scanc	$65535,(r4),(sp),$255	# else scanc the first 65535
	bneq	L5			# hop ahead if we found one
	jmp	L3			# else go back and do next 65535
L4:
	subl3	r4,r1,r0		# length to end of string in r0
	scanc	r0,(r4),(sp),$255	# this is the main function
L5:
	subl3	4(ap),r1,r0		# length of segment returned
	movab	256(sp),sp		# restore the stack pointer
	ret
