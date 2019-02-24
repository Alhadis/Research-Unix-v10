#
# pseudo-boot-rom for MASSBUS SMD disks on the star
#

#
# massbus adapter stuff
#

	.set	MBASR, 8		# mba status
	.set	MBAVAD, 12		# mba virtual address
	.set	MBABCR, 16		# mba byte count
	.set	MBAREG, 0x400		# start of external registers
	.set	MBAMAP, 0x800		# start of mapping regs

	.set	REGSIZ, 32*4		# size of register space per unit

	.set	V_B, 31			# bit number of `valid' in map reg

	.set	MBBUSY_B, 31		# `busy' bit in mbasr
	.set	MBERR, 0x1000		# error bits in mbasr

#
# rm controller stuff
#

#
# register offsets
#
	.set	HPCS1, 0		# control/status 1
	.set	HPDS, 4			# driver status
	.set	HPDA, 20		# sector & track
	.set	HPDT, 24		# driver type
	.set	HPSN, 32		# serial number
	.set	HPOF, 36		# offset
	.set	HPDC, 40		# cylinder

#
# useful bits in various registers
#
	.set	PRESET, 021		# read in preset
	.set	PACKACK, 023		# pack acknowledge
	.set	READ, 071		# read

	.set	FMT22, 010000		# pdp11 format

#
# vax stuff
#

	.set	PGSIZ, 512		# size of a page/disk sector

#
# bootstrap stuff
#

	.set	BOOT, 0xc		# start address in boot block
	.set	TYPE, 0			# device type; 0 == massbus
	.set	SSIZE, 0x20		# stack space needed

#
# initial bootstrap
# read boot block,
# set up regs for it
# and jump there
#
# start:
#	r1 = mba address
#	r2 = rubbish
#	r3 = device unit number, assumed == mba controller number
# assume sp == 0x200, ie first 64K of memory are good
#
# finish:
#	r3, r5, r10, r11, ap, sp as when started
#	r0 == type of boot device
#	r1 == mba address still
#	r2 == controller number
#	r3 == unit number
#	r5 == software flags
#	r6 == address of our driver
#	sp == 0x200 again
#

	.byte	'P, 'H			# device type for rom
boot:
	addl2	$SSIZE,sp	# move stack away from boot block
	movab	driver,r6
	movl	r3,r2		# unit == controller (??)
	mull3	$REGSIZ,r2,r7
	addl2	$MBAREG,r7
	addl2	r1,r7		# r7 = address of device registers
	movzbl	$PRESET,HPCS1(r7)	# pack acknowledge
	movzwl	$FMT22,HPOF(r7)		# pdp-11 format

	clrl	r8		# read block 0
	clrl	-(sp)		# onto physadr 0
	jsb	(r6)		# call our driver
bbad:	blbc	r0,bbad		# hang if error
	movl	$TYPE,r0	# ok, get device type
	subl2	$SSIZE-4,sp	# fix up sp
	jmp	*$BOOT		# and run boot block

#
# table of device types:
# each a longword of
# (byte) type, (byte) sectors per track, (word) sectors per cylinder
# table is reversed for hackish reasons
#
	.long	0
	.byte	024, 32; .word 32*5	# RM03
	.byte	027, 32; .word 32*19	# RM05
	.byte	022, 22; .word 22*19	# RP06
	.byte	021, 22; .word 22*19	# RP05
	.byte	042, 50; .word 50*32	# RP07
typtab:
# fuji/emulex drives left out on purpose for now;
# not worth the extra code till someone has one

#
# driver subroutine
# read one block into a specified place
# args:
#	r1 == mba address
#	r2 == controller number within mba
#	r3 == unit number (usually == r2)
#	r5 == offset from beginning of memory; ignored
#	r6 == our address
#	r8 == desired block number
#	4(sp) == physical address to read to
#
# preserve r1-r6 r10 r11 ap sp
#

driver:
	mull3	$REGSIZ,r2,r7		# compute device addr
	addl2	$MBAREG,r7
	addl2	r1,r7
	movzbl	$PACKACK,HPCS1(r7)	# just in case
	moval	typtab,r9
0:	tstl	-(r9)
	bneq	1f
	 rsb
1:	movl	HPDT(r7),r0
	cmpb	r0,(r9)			# compare types
	bneq	0b
	extzv	$8,$8,(r9),-(sp)	# sec per track
	extzv	$16,$16,(r9),-(sp)	# sec per cyl
	clrl	r9			# make r8+r9 == quadword block number
	ediv	(sp)+,r8,HPDC(r7),r8	# r8 == offset in cyl; da == cyl
	ediv	(sp)+,r8,r9,r8		# r8 == sect; r9 == track
	insv	r9,$8,$8,r8		# high part of r8 == track
	movl	r8,HPDA(r7)
	divl3	$PGSIZ,4(sp),r9		# get phys page number
	bbss	$V_B,r9,0f		# make map reg contents
0:	movl	r9,MBAMAP(r1)		# set up map
	addl3	$1,r9,MBAMAP+4(r1)	# in case page unaligned
	bicl3	$~(PGSIZ-1),4(sp),MBAVAD(r1)	# offset within page
	movzwl	$-PGSIZ,MBABCR(r1)	# byte count
	mnegl	$1,MBASR(r1)		# clear error bits
	movzbl	$READ,HPCS1(r7)		# start xfer
1:
	movl	MBASR(r1),r9
	bbs	$MBBUSY_B,r9,1b		# spin until not busy
	clrl	r0			# assume bad
	bitl	$MBERR,r9		# any error bits?
	bneq	2f			# yep
	 incl	r0			# no, remember ok
2:	rsb
