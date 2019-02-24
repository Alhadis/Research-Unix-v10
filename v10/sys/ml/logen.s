#
# things that must be at known, fixed addresses
#

	.text
#
# 0x0
# restart parameter block
# must be page aligned
#
	.globl	_rpb
_rpb:
	.long	0	# my physical address
	.long	restart
	.long	0	# eventually a checksum
	.long	0	# set to 1; clear it again

#
# 0x10
# enter here to cause a crash dump
#
	jmp	doadump
	.align	4
#
# 0x20
# crash dump constants:
# - magic number
# - when it crashed
# - how big
#
	.globl	dumpmagic
dumpmagic: .long 0
	.globl	dumptime
dumptime: .long 0
	.globl	dumplen
dumplen: .long 0
