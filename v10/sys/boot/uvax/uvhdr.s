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
# temporary cheap hack:
# copy to very high memory, not ordinary ROM addr
# fix up Q-bus map
# this to (hopefully) keep certain UQSSP controllers
# from overwriting the wrong data
#
# on entry, r11 == extended rpb
#

#	.set	RROM,0xfa00		# where rom should be in memory
	.set	RROM,0x72000
	.set	RRSIZE,512		# max size expected for rom:
					# no bigger than this disk sector!

	.set	QMAP,0x20088000		# Q-bus map registers
	.set	QV,0x80000000		# valid map bit

#
# offsets in VMB's `extended RPB'
#
	.set	RPBFLAG,0x30		# restart flags
	.set	RPBCSR,0x54		# boot device csr
	.set	RPBUNIT,0x64		# boot device unit number

begin:
	movc3	$RRSIZE,rombegin,*$RROM
	movl	$QV|(RROM/512),*$QMAP+(RROM/128)	# make map valid, for UQSSP stuff
	movl	RPBCSR(r11),r2
	movzbl	RPBUNIT(r11),r3
	movl	RPBFLAG(r11),r5
	movl	$0x200,sp		# `working memory'
	jmp	*$RROM
	.align	2

rombegin:
