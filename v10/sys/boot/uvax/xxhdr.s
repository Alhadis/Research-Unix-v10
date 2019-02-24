#
# first half of boot block for MicroVAX:
# VMB header, boot driver
#
	.set	IDAREA,8
	.word	0x0			# junk
	.byte	IDAREA/2		# word offset to beginning of id area
	.byte	0x1			# must be one
	.long	0x00000000		# lbn of secondary image, PDP-11 format long
#
# id area: byte 8
#
	.byte	0x18			# VAX instruction set
	.byte	0x0			# unknown controller type
	.byte	0x0			# unknown file structure
	.byte	~0x18			# ones complement of the last three bytes
	.byte	0x0			# must be zero
	.byte	0x1			# format version 1, single sided
	.word	0x0			# junk
	.long	0x1			# size in blocks of secondary image
	.long	0x0			# load offset from default load address
	.long	begin			# offset into image to start execution
	.long	0x1+0x0+begin		# sum of previous three longwords

#
# prologue, before we call
# the code that thinks it's a rom
# set up registers in the standard ROM-like way
# copy us to the normal ROM address
#
# on entry, r11 == extended rpb
#

	.set	RROM,0xfa00		# where rom should be in memory
	.set	RRSIZE,512		# max size expected for rom:
					# no bigger than this disk sector!

#
# offsets in VMB's `extended RPB'
#
	.set	RPBFLAG,0x30		# restart flags
	.set	RPBCSR,0x54		# boot device csr
	.set	RPBUNIT,0x64		# boot device unit number

begin:
	movc3	$RRSIZE,rombegin,*$RROM
	movl	$0772150,r2
	movzbl	$2,r3
	movl	$0x200,sp		# `working memory'
	jmp	*$RROM
	.align	2

rombegin:
