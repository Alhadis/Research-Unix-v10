#
# fake boot ROM for nautilus, KDB50
#
# read block 0 of some specified drive into memory,
# and branch there, with registers set to point to
# a device driver that can read other blocks.
# register conventions a la the comet boot ROMs
# we expect to be loaded at address 0xfa00,
# and that we have 1024 bytes to squander there
# (like the four comet ROMs)
#

#
# nautilus-specific defs
#
	.set	NMICSR,0x20080000	# nmi csrs -- device 0
	.set	NMIto2,30		# 0x40000000; add to get device 1
	.set	LOOP,0x10000		# csr0 loopback mode

#
# VAXBI defs
#
	.set	FIRSTBI,0x20000000	# BI 0 node 0
	.set	NSIZE,0x2000		# size of a BI node
	.set	BISIZE,0x2000000	# size of all the registers in a BI

#
# VAXBI and/or KDB50 defs
#
	.set	BITYPE,0
	.set	DB88,0x106		# device type for DB88
	.set	BICSR,0x4
	.set	SST,0x400		# node reset
	.set	BROKE,0x1000		# i'm broken
	.set	BISADR,0x20
	.set	BIEADR,0x24
	.set	IP,0xf2			# word -- ip reg
	.set	SAR,0xf4		# word -- read version of sa reg
	.set	SAW,0xf6		# word -- write version of sa

	.set	STEP0,10		# STEP1 == bit 11

	.set	BPOWN,31		# bit 31 == port owns descriptor
	.set	RTOC,4

#
# mscp defs
# packet offsets have 4 added to account for UQSSP header (mostly length)
#

	.set	m_crf,4			# command ref number
	.set	m_unit,8		# disk unit number
	.set	m_opcd,12		# opcode
	.set	m_sts,14		# status (word)
	.set	RPKSIZE,16		# as much as we want on rcv
	.set	m_bcnt,16		# byte count
	.set	SOSIZE,20		# as much as we send for ONLINE
	.set	m_buff,20		# buffer address
	.set	m_lbn,32		# block number
	.set	SRSIZE,36		# as much as we send to read
	.set	SOSIZE,36		# as much as we send for ONLINE

	.set	OPONL,011		# unit online
	.set	OPRD,041		# read block

#
# boot ROM conventions
#
	.set	WMEM,0x200		# sp == some working memory + WMEM
					# usually working memory == 0
	.set	BSIZE,512		# size of a block


ourstack:
	.byte	'U,'D			# ROM id (though we're not in a ROM)
#
# here to start off, at 0xfa02
# registers:
#	r1	bi node number; 0xf == node, 0xf0 == which BI
#	r2	uninteresting to us; we put register addr there
#	r3	unit number
#	r5	boot flags; ignored but preserved
#	sp	working memory + WMEM
#
# registers to save: r0 r1 r3 r5 r10 r11 ap sp
# init the appropriate NBIA/NBIB
# assume the NBIA is already OK
#
	movl	sp,savesp
	movab	ourstack,sp
	movl	$NMICSR,r2
	extzv	$4,$2,r1,r6	# get BI number
	bbc	$1,r6,0f
	 bbss	$NMIto2,r2,0f	# second NBIA
0:	bisl2	$LOOP,(r2)	# set loopback mode
	mull3	r6,$BISIZE,r7	# first node in this BI
	addl2	$FIRSTBI,r7
	movl	BITYPE(r7),r8
	movw	$DB88,r8
	movl	r8,BITYPE(r7)	# set device type in DB88
	clrl	BISADR(r7)	# start address
	movl	$0x20000000,BIEADR(r7)	# end address: all possible mem
	bisl2	$BROKE,BICSR(r7)	# set BROKE to clear it
	bicl2	$LOOP,(r2)	# and turn off loopback mode

#
# init the KDB50
#

	extzv	$0,$4,r1,r2	# pick out node number
	mull2	$NSIZE,r2
	addl2	r7,r2		# address of KDB50 regs
	bisw2	$SST,BICSR(r2)	# reset: kick into startup
0:	movw	SAR(r2),r7
	blss	oops
	bbc	$STEP0+1,r7,0b	# wait for step1
	movw	$0x8000,SAW(r2)	# no vector, ring size (2^0,2^0)
0:	movw	SAR(r2),r7
	blss	oops
	bbc	$STEP0+2,r7,0b	# wait for step2
	movab	rsp,r6		# ring address
	movw	r6,SAW(r2)
0:	movw	SAR(r2),r7
	blss	oops
	bbc	$STEP0+3,r7,0b	# wait for step3
	ashl	$-16,r6,r6
	movw	r6,SAW(r2)
0:	movw	SAR(r2),r7
	blss	oops
	bbc	$STEP0+4,r7,0b	# wait for step4
	movw	$1,SAW(r2)	# GO

#
# read block 0 into bottom of memory
#

	pushl	r0		# rblk eats it
	movab	rblk,r6		# set up device driver address
	subl3	$WMEM,savesp,-(sp)	# physical addr
# trick: we know driver doesn't use r5, so we needn't preserve boot flags
	clrl	r8		# block 0
	jsb	(r6)
	blbc	r0,oops
	tstl	(sp)+
	movl	(sp)+,r0
#
# start boot block
# registers now:
#	r0	device type (unchanged)
#	r1	BI node number (unchanged)
#	r2	device register addr
#	r3	device unit number (unchanged)
#	r5	boot flags (unchanged)
#	r6	address of our device driver
#	sp	`working memory + 0x200' (unchanged)
#
	movl	savesp,sp
	jmp	0xc-WMEM(sp)

#
# here when something is broken
#
oops:	halt

#
# device driver; called above and by boot block and friends
#
#	r2	device registers
#	r3	device unit number
#	r5	buffer address, relative to `working memory' (ignored)
#	4(sp)	physical buffer address; we use that
#	r8	block number
#
# do not destroy r1-r6 r10 r11 ap
# on return, low bit of r0 set if we read it, clear if we failed
#
# always bring disk online, even if already is
#

rblk:
	movq	r10,-(sp)
	clrl	r0			# assume failure
	movab	cp,r10
	movab	rp,r11
	movab	rsp,r9
	movl	r3,m_unit(r10)		# make ONLINE packet
	movzbl	$OPONL,m_opcd(r10)
	movzbl	$SOSIZE,(r10)
	clrl	m_bcnt(r10)		# ugh
	clrl	m_buff(r10)		# ugh
	clrl	m_lbn(r10)		# ugh
	bsbb	sendrcv
	bneq	rfail
	movzbl	$OPRD,m_opcd(r10)	# now make READ packet
	movl	4+8(sp),m_buff(r10)	# unmapped address
	movzwl	$BSIZE,m_bcnt(r10)
	movl	r8,m_lbn(r10)
	movzbl	$SRSIZE,(r10)
	bsbb	sendrcv
	bneq	rfail
	incl	r0			# success
rfail:	movq	(sp)+,r10
	rsb

sendrcv:
	movzbl	$RPKSIZE,(r11)
	movab	m_crf(r10),RTOC(r9)
	movab	m_crf(r11),(r9)
	bbss	$BPOWN,(r9),0f		# make response buffer available
0:	bbss	$BPOWN,RTOC(r9),0f	# send the ONLINE packet
0:	movw	IP(r2),r7		# kick to make it go
0:	bbs	$BPOWN,(r9),0b		# wait for answer
	tstb	m_sts(r11)		# tstb: ignore `already online' flag
	rsb

#
# KDB50 communication area
#
	.align	2
savesp:	.space	4		# working mem + WMEM
	.space	8		# crap at the top
rsp:	.space	4		# response descriptor
	.space	4		# command descriptor

#
# a pair of packets
# +4 for length
#

cp:	.space	SRSIZE+4	# command packet
rp:	.space	RPKSIZE+4	# response packet
