#
# device-independent boot block
# uses the rom device driver
# reads an already-de-headered, small file with predetermined name
# into the beginning of memory
# and executes it
#
# buffering:
# the ROM driver will only write into the bottom 64K of memory
# to make things simple, we always read to a fixed place,
# then copy to the desired place.
# if we would have read to the fixed place,
# we copy to a safe buffer, and copy back when we're all done.
# the same subterfuge is needed to avoid overwriting the in-RAM copy
# of the boot ROMs, where our device driver lives,
# so we use the memory just below the ROM copy for the buffer
#
	.set	BSIZE,512	# size of a disk block, according to rom driver
	.set	BSHIFT,9
	.set	BUFADDR,0xfa00-BSIZE	# where to read stuff
	.set	BUFSIZE,1024+BSIZE	# sizeof(buffer) + sizeof(roms)

	.set	NAMELEN,14	# number of chars in name; one element for now
	.set	HIADDR,0x70000	# high address at which we run

	.set	REGMASK,0xef	# registers to save for second boot
	.set	SVSIZE,7*4
	.set	SADR,0		# r0 -- overwritten with start addr
	.set	SR1,(1*4)	# offsets in save area
	.set	SR3,(3*4)
	.set	SFLGS,(4*4)	# boot flags -- r5
	.set	DRIVER,(5*4)	# where device driver addr gets saved -- r6

	.set	A_MAGIC,0	# offset in exec hdr: magic number
	.set	A_TEXT,4	# text size
	.set	A_DATA,8	# data size
	.set	A_ENTRY,20	# start address

	.set	ISIZE,64	# size of an i-node
	.set	ISHIFT,6	# same as a shift
	.set	INOPB,BSIZE/ISIZE
	.set	ROOTINO,2	# i-number of the root
	.set	ILBLK,2		# first block of i-list
	.set	NDIREC,10	# number of direct blocks
	.set	NINDIR,256	# number of (interesting) indirect blocks
	.set	NINDBLK,NDIREC+NINDIR
	.set	I_ADDR,12	# offset to addresses in inode
	.set	DIRSIZ,14	# chars in filename

	.set	PGSIZE,1024	# ld's idea of page size -- for 0410 a.out
	.set	PGSHIFT,10

	.set	RXCS,32		# console rcv csr
	.set	RXDB,33		# console rcv data buffer
	.set	RRDY_B,7	# bit number of ready bit in RXCS
	.set	TXCS,34		# console xmt csr
	.set	TXDB,35		# console xmt data buffer
	.set	TRDY_B,7	# bit number of ready bit in TXCS

#
# main code
# relocate ourselves to high memory
# read the root directory, and search it for
# the desired file
# read that file in
#
# start must be at addr 0xc
#

beg:
	brb start;brb start	# 0, 2 valid start addresses
hiaddr:	.long	HIADDR
.set didsave,hiaddr+3		# a byte which is 0 when we start
bufaddr: .long BUFADDR		# handy constant reference
start:
	movl	hiaddr,sp	# set up new stack
	pushr	$REGMASK	# save important boot registers
	movl	sp,r11		# remember where they are
	movc3	$end,beg,SVSIZE(sp)	# copy us up
	jmp	strel+SVSIZE(sp)

bootname:
	.byte	'u,'n,'i,'x,0,0,0,0,0,0,0,0,0,0


#
# print a character on the console from r1
#

putc:
	mfpr	$TXCS,r2
	bbc	$TRDY_B,r2,putc
	mtpr	r1,$TXDB
	rsb

#
# jump here after relocation
#

strel:
	blbs	SFLGS(r11),stask	# boot flag 1 == prompt for filename
	movab	bootname,r0
0:	movzbl	(r0)+,r1
	beql	prnl
	bsbb	putc
	brb	0b

stask:
	movl	r11,sp		# in case we loop back

#
# read a string from the console into bootname
# terminated by newline
#

getpr:
	movzbl	$'*,r1		# prompt
	bsbb	putc

getname:
	movab	bootname,r3
	movzbl	$NAMELEN-1,r0
0:	mfpr	$RXCS,r1
	bbc	$RRDY_B,r1,0b
	mfpr	$RXDB,r1
	bicb2	$0200,r1
	bsbb	putc
	cmpb	$015,r1
	beql	8f
	cmpb	$040,r1		# ignore control chars; thanks, nautilus
	bgtr	0b
	decl	r0
	blss	0b
	movb	r1,(r3)+
	brb	0b
8:	clrb	(r3)+
	sobgtr	r0,8b
prnl:	movzbl	$015,r1
	bsbb	putc
	movzbl	$012,r1
	bsbb	putc	# and return
2:
#
# get the root i-node
#
	movl	$ROOTINO,r0
	bsbb	iget
#### check is directory?
	movab	-NINDBLK*4(sp),sp
	movl	sp,r10
	bsbb	addrcpy
#
# search the root directory
#

	clrl	r12			# init block pointer
dirblk:
	clrl	r5			# use beginning of mem as buffer
	bsbw	lread
	bneq	stask			# eof, try another file
	clrl	r9
0:
	movzwl	(r9)+,r0		# empty entry?
	beql	2f			# yes, skip it
	clrl	r1
1:	cmpb	(r9)[r1],bootname[r1]	# MicroVAX II doesn't have cmpc
	bneq	2f
	aoblss	$DIRSIZ,r1,1b
	brb	diryes			# the name we want

2:	acbw	$FSBSIZE-1,$DIRSIZ,r9,0b
	incl	r12			# get next block
	brb	dirblk

berr:	brb	stask

#
# subroutines placed here, out of sequence, to save space
#
# copy/convert block numbers
# out of an i-node
# r10 points to where we want them
# r0 points to the i-node
# for now, only the direct blocks
#
# r0 is destroyed
#

addrcpy:
	movl	r10,r1			# make a volatile copy
	addl2	$I_ADDR,r0		# point to addresses
	clrl	r2
0:
	movw	(r0)+,(r1)+
	movzbw	(r0)+,(r1)+
	aoblss	$NDIREC+1,r2,0b
	rsb

#
# fetch an i-node
# r0 has the i-number
# on exit, r0 points to the i-node in core
# address 0 is used as a buffer
#

iget:
	decl	r0			# i-numbers are 1 based
	clrl	r1			# make a quadword
	ediv	$INOPB,r0,r8,-(sp)	# r8 == block; (sp) == offset
	addl2	$ILBLK*FSBSIZE/BSIZE,r8
	clrl	r5			# into address 0
	bsbb	bread
	ashl	$ISHIFT,(sp)+,r0	# point to i-node
	rsb

#
# read a BSIZE block from the disk
# using the rom subroutine
# block number in r8;
# buffer address in r5
# no return if it failed
#
# hack: if we would read atop the boot roms, don't;
# read into a safe place instead, and remember
# for the purposes of the hack, it's assumed that
# reads are aligned in memory conveniently
#

bread:
	subl3	bufaddr,r5,r0
	blss	0f
	cmpl	r0,$BUFSIZE
	bgeq	0f
	 movab	savedata,r5
	 addl2	r0,r5
	 movb	$1,didsave
0:
	pushl	r5		# save real addr
	movl	bufaddr,r5	# get buffer loc
	movq	SR1(r11),r1	# device address stuff
	movl	SR3(r11),r3	# unit number
	pushl	r5		# duplicate address for driver routine
	jsb	*DRIVER(r11)	# and call driver
	blbc	r0,berr		# disk error, start over
	tstl	(sp)+		# fixup stack
	movc3	$BSIZE,*bufaddr,*(sp)+	# copy data to final resting place
	rsb

#
# back to the main path:
# found the file
# fetch its i-node and read it in
# i-number left in r0 for us
#

diryes:
	bsbb	iget
##### check i-node?
	bsbb	addrcpy
#
# indirect blocks
# only the first 1024 bytes of the first indirect block examined
#
	mull3	NDIREC*4(r10),$FSBSIZE/BSIZE,r12 # pick up one indirect block
	beql	noind
	movl	r12,r8
	moval	NDIREC*4(r10),r5
	bsbb	bread
	addl3	r12,$1,r8		# 2 sectors == 256 indirections
	moval	NDIREC*4+BSIZE(r10),r5
	bsbb	bread
noind:
	clrl	r5
	clrl	r12
rdloop:
	ashl	$FSSHIFT,r12,r5
	bsbb	lread
	bneq	eof
	incl	r12
	brb	rdloop
#
# whole file read in
# check for a.out header and relocate
#
eof:
	blbc	didsave,1f
	 movc3	$BUFSIZE,savedata,*bufaddr
1:	clrl	r12
	movq	A_TEXT(r12),r7	# r7 = text size; r8 = data size
	movl	A_ENTRY(r12),SADR(r11)	# starting address
2:	casel	A_MAGIC(r12),$0407,$1
0:	.word	m0407-0b
	.word	m0410-0b
	clrl	SADR(r11)	# no a.out, start at the top
	brb	done
m0407:
	addl2	r8,r7		# text = text + data; no data
	clrl	r8
#
# do copies by longword: simpler and smaller than movc3,
# which would need to loop anyway
# longword not quadword because ld rounds sizes to long
#
# really should beql after the second div, else when there's no data
# we copy one extra word to a nonsense place.
# too bad; we can't spare the code space
#
m0410:
	clrl	r0
	movzbl	$32,r1
	addl3	r7,r8,r2
	divl2	$4,r2
1:	movl	(r1)+,(r0)+	# copy down over a.out header
	sobgtr	r2,1b
	divl3	$4,r8,r2
	addl3	r8,r7,r1	# from t + d
	extzv	$0,$PGSHIFT,r7,r0
	beql	0f		# don't overround
	 subw3	r0,$PGSIZE,r0
0:	addl2	r1,r0		# to t + d + round
2:	movl	-(r1),-(r0)	# copy data to start at page boundary
	sobgtr	r2,2b
#
# reset registers and start
#

done:
	movl	r11,sp
	popr	$REGMASK
	movl	r5,r11		# put boot flags in stupid bky place
	jmp	2(r0)		# skip register mask & start it

#
# read a (filesystem) block from a file
# (filesystem) block number in r12
# buffer address in r5
# r10 points to the addresses
# returns status in psw:
# zero flag set if read a block
# clear if error or block doesn't exist
#

lread:
	cmpw	r12,$NINDBLK
	blss	0f
lerr:
	bicpsw	$4		# clear zero flag
	rsb
0:
	movl	(r10)[r12],r8
	beql	lerr
	pushl	$FSBSIZE/BSIZE	# count of disk blocks within fs block
	mull3	$FSBSIZE/BSIZE,r8,-(sp)	# fs block to disk block
1:
	movl	(sp),r8
	movl	r5,r6		# safe place
	bsbw	bread
	movab	BSIZE(r6),r5
	incl	(sp)
	sobgtr	4(sp),1b
	clrq	(sp)+		# addl2 $8,sp; set zero bit
	rsb
#
# last address that needs saving
#
	.globl end
end:

#
# put uninitialized (not 0) data here
#
	.align	2
savedata:
#	.space	ROMSIZE
