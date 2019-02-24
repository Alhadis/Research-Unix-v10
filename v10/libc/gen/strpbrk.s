	.file	"strpbrk.s"
#	@(#)strpbrk.s	1.5
# Fast assembler language version of the following C-program
#			strpbrk
# which represents the `standard' for the C-library.

#  Return ptr to first occurance of any character from `brkset'
#    in the character string `string'; NULL if none exists.

#	char *
#	strpbrk(string, brkset)
#	register char *string, *brkset;
#	{
#		register char *p;
#	
#		if(!string || !brkset)
#			return(0);
#		do {
#			for(p=brkset; *p != '\0' && *p != *string; ++p)
#				;
#			if(*p != '\0')
#				return(string);
#		}
#		while(*string++);
#		return(0);
#	}
	.globl	_strpbrk
	.text
	.align	2
_strpbrk:
	.word	0x0000			# pgm uses regs 0,1,2,3,4
	movab	-256(sp),sp		# get table space
	movc5	$0,(ap),$0,$256,(sp)	# and zero it out
	movl	8(ap),r0		# charset ptr in r0
	beql	L5			# return 0 if arg was NULL
L0:
	movzbl	(r0)+,r3		# byte from charset for index
	beql	L1			# hop if found ending NULL
	movb	$255,(sp)[r3]		# make entry in scanc table
	jmp	L0			# get next byte from charset
L1:
	movl	4(ap),r1		# string ptr in r1
	beql	L4			# if it's NULL, return 0
L2:
	movl	r1,r4			# preserve a copy of this ptr
	locc	$0,$65535,(r1)		# get length of string
	bneq	L3			# skip if string not > 65535
	scanc	$65535,(r4),(sp),$255	# else scanc the first 65535
	bneq	L4			# hop ahead if we found one
	jmp	L2			# else go back and do next 65535
L3:
	subl3	r4,r1,r0		# length to end of string in r0
	scanc	r0,(r4),(sp),$255	# this is the main function
	beql	L5			# skip if none (r0 already 0)
L4:
	movl	r1,r0			# return ptr to brk char
L5:
	movab	256(sp),sp		# restore the stack pointer
	ret
