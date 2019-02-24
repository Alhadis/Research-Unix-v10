#
#	Operating System Interface to Berkeley VAX/UNIX
#		              for
#		       Macro SPITBOL V3.5
#	-----------------------------------------------
#
#		Copyright 1982
#		Dewar Information Systems Corporation
#		221 West Lake Street
#		Oak Park, Illinois 60302
#
#	register masks
#
	.set	mr0,1
	.set	mr1,2
	.set	mr2,4
	.set	mr3,8
	.set	mr4,16
	.set	mr5,32
	.set	mr6,64
	.set	mr7,128
	.set	mr8,256
	.set	mr9,512
	.set	mr10,1024
	.set	mr11,2048
#
#	c functions assume that r0 -> r5 are scratch.  however, spitbol
#	uses r2, r3, and r5.  so, need to save these registers across
#	function calls.
#
	.set	mr235,mr2+mr3+mr5
#
#	mask for all registers used by compiler. necessary for dealing
#	with load modules.
#
	.set	cmpreg,mr2+mr3+mr5+mr6+mr7+mr8+mr9+mr10
#
#	miscellaneous equates
#
	.set	ch$lk,107	# upper case letter k
	.set	ch$mn,45	# dash (minus sign)
	.set	k,1024
	.set	kwords,k*4
#
#	internal spitbol blocks
#	-----------------------
#
#	these equates describe compiler control blocks used by osint.
#
#	chain of file control blocks
#
	.set	chtyp,0		# type word
	.set	chlen,4		# block length
	.set	chnxt,8		# -> next chblk
	.set	chfcb,12	# -> fcb
	.set	chsize,16	# size of chblk
#
#	icblk - integer block
#
	.set	ictyp,0		# type word
	.set	icval,4		# integer value
#
#	scblk - string block
#
	.set	sctyp,0		# type word
	.set	sclen,4		# string length
	.set	scstr,8		# start of string
#
#	interface control blocks
#	------------------------
#
#	these control blocks are built by the interface to handle i/o.
#	for files other than standard input, standard output, and the
#	terminal, these control blocks reside in the compiler's 
#	dynamic area. this requires appropriate setting of the type
#	words to allow proper garbage collection.
#
#	blocks with type b$xnt contain non-relocatable addresses - that
#	is, no word may contain a pointer to another block in the
#	dynamic area.
#
#	blocks with type b$xrt contain relocatable addresses - that is
#	words may contain a pointer to another block in the dynamic area.
#
#	the basic structure is:
#
#	for each input or output associated file there is:
#
#		1 ioblk	 containing all global information about
#			 the file:  filename, buffer block pointer,
#			 file descriptor number, flags
#
#		1 bfblk  containing the file buffer
#
#		1 or more fcblks  containing the access mode (line or
#			 raw), the record length, and a pointer to 
#			 the ioblk
#
#	the first INPUT() or OUTPUT() call for a file creates one block
#	of each type. subsequent calls to INPUT() or OUTPUT() for a 
#	previously associated file, may cause the creation of a new fcblk.
#	allowing multiple fcblks provides the program with different
#	ways of accessing the same file. for example, one type of access
#	can be a character at a time and another entire records.
#
#	the compiler keeps track of all active fcblks, and at times
#	like end-of-job provides the inteface with a chain of all fcblks.
#
#
#	bfblk - i/o buffer control block
#
	.set	bftyp,0		# type word - b$xnt
	.set	bflen,4		# block length
	.set	bfsiz,8		# buffer size in bytes
	.set	bfrem,12	# bytes remaining
	.set	bfoff,16	# offset to next remaining byte
	.set	bfsize,20	# end of fixed portion
#
#	fcblk - file control block
#
	.set	fctyp,0		# type word - b$xrt
	.set	fclen,4		# block length
	.set	fcrsz,8		# record size ( >0 line mode / <0 raw mode) 
	.set	fciob,12	# -> ioblk 
	.set	fcsize,16	# size of fcblk
#
#	ioblk - i/o control block
#
	.set	iotyp,0		# type word - b$xrt
	.set	iolen,4		# block length
	.set	iofnm,8		# -> filename scblk
	.set	iopid,12	# pid (if one end of pipe)
	.set	iobuf,16	# -> bfblk
	.set	iofdn,20	# file descriptor number
	.set	ioflg,24	# flags
	.set	iosize,28	# size of ioelt
#
#	defines that match "spitio.h" for flags in ioflg
#
	.set	IO_INP,1	# input associated
	.set	IO_OUP,2	# output associated
	.set	IO_APP,4	# output open for append
	.set	IO_OPN,8	# file is open
	.set	IO_EOF,16	# eof on input file
	.set	IO_ERR,32	# i/o error
	.set	IO_SYS,64	# this is an osint file
	.set	IO_WRC,128	# don't do buffering
	.set	IO_PIP,256	# this is one end of a pipe
	.set	IO_DED,512	# other end of pipe died
	.set	IO_ILL,1024	# i/o illegal according to sysfc
#
#-----------
#
#	sysej - end of job
#
	.globl	sysej
sysej:
	movl	r10,_rzfcb	# copy head of fcb chain
	beqlu	1f		# if empty then nothing to close
0:	movl	chfcb(r10),r1	# -> fcb
	pushl	fciob(r1)	# -> ioblk
	calls	$1,_osclose	# call to do close
	movl	chnxt(r10),r10	# -> next on chain
	bnequ	0b		#    and loop back for more
1:	pushl	r7		# return &code
	calls	$1,__exit
	rsb
#
#	sysfc
#
	.globl	sysfc
sysfc:
	movl	(sp)+,(sp)	# remove stacked scblk
	pushr	$mr235
	tstl	sclen(r10)	# if null filearg1 then
	jeqlu	erxit1		#    error
#
#	get length of filename and scan off options.
#
	movl	r9,-(sp)	# -> filename scblk
	calls	$1,_lenfnm	# call to get filename length
	movl	r0,lenfname	# save length for later use
	jlss	erxit1		# length must not be negative
	movl	r9,-(sp)	# -> filename scblk
	movl	$tioblk,-(sp)	# -> temporary ioblk
	movl	r7,-(sp)	# input/output association flag
	calls	$3,_sioarg	# call to scan i/o args
	tstl	r0		# if error in args then
	jlss	erxit1		#    take error return
#
#	check for consistency of calls. cannot have both input
#	and output to same channel. if this happens, though,
#	set flag and let sysio take proper error exit.
#
	tstl	r6		# if no previous fcblk then
	beqlu	0f		#    skip
	movl	fciob(r6),r0	# -> ioblk
	movl	ioflg(r0),r0	# get previous flags
	mcoml	$IO_INP+IO_OUP,r1 # get mask for bicl
	bicl2	r1,r0		# remove all bits but INP&OUP
	bitl	tioblk+ioflg,r0	# if bits are not same then
	bnequ	0f
	bisl2	$IO_ILL,tioblk+ioflg	# then set error flag
0:
#
#	handle null filenames here - must either have a previous
#	fcblk or specify -f arg.
#
	tstl	lenfname	# if non-null filename then
	bgtr	fcfnam		#    go handle below
	bitl	$IO_OPN,tioblk+ioflg
	bnequ	fcfarf		# if -f specified then merge w/non-null
	tstl	r6		# if no previous fcblk then
	jeqlu	erxit1		#    error
	clrl	r6		# assume that no new fcblk needed
	clrl	ioblkptr	# no ioblk ptr to stuff
	tstl	tioblk		# if however i/o args indicate one
	jeqlu	fcxit
	movl	fciob(r6),ioblkptr
	movl	$fcsize,r6	#    allocate one
	jbr	fcxit
#
#	handle real filenames and null filenames with -f arg here.
#	note that they're mutually exclusive.
#
fcfnam:	bitl	$IO_OPN,tioblk+ioflg
	jnequ	erxit1		# can't have -f arg too
fcfarf:	tstl	r6		# if previous fcblk passed then
	jnequ	erxit1		#    error
	clrl	ioblkptr
	addl3	$bfsize+3,tioblk+iopid,r6
	bicl2	$3,r6
	movl	r6,bfblksiz
	addl2	$fcsize+iosize,r6
	movl	$1,tioblk	# set newfcb flag
#
fcxit:	clrl	r10		# no private fcblk
	clrl	r8		# xrblk please
	popr	$mr235
	addl2	$4,(sp)
	rsb
#
#	sysil - get input record length
#
	.globl	sysil
sysil:
	movl	fcrsz(r6),r6
	bgtr	0f
	mnegl	r6,r6
0:
	rsb
#
#	sysin - read input record
#
	.globl	sysin
sysin:
	pushr	$mr235
	pushl	fciob(r6)	# -> ioblk
	calls	$1,_osopen	# call to open file
	tstl	r0		# if open unsuccessful then
	jnequ	erxit3		#    take error exit
	pushl	r9		# -> scblk
	pushl	fciob(r6)	# -> ioblk
	pushl	fcrsz(r6)	# push record length
	bgtr	0f
	mnegl	(sp),(sp)	# if negative then make it positive
0:	pushl	fcrsz(r6)	# i/o mode - raw or line
	calls	$4,_osread	# call to do read
	cmpl	r0,$-1		# check for eof or input error
	jeql	erxit1		#   take eof exit
	jlss	erxit2		#   take error exit
	movl	r0,sclen(r9)	# set record length
	popr	$mr235
	addl2	$12,(sp)
	rsb
#
#	sysio
#
	.globl	sysio
sysio:
	pushr	$mr235
	bitl	$IO_ILL,tioblk+ioflg
	jnequ	erxit2		# if illegal then take error exit
	movl	r6,fcblkptr	# copy fcblk pointer for exit
#
#	fill in fcblk.
#
	tstl	tioblk+iotyp	# if no new fcb to build then
	jeqlu	iodon		#    done
	movl	$fcsize,fclen(r6)
	movl	tioblk+iolen,fcrsz(r6)
	movl	ioblkptr,fciob(r6)
	jnequ	iodon
	movab	fcsize(r6),fciob(r6)
#
#	fill in ioblk.
#
	movab	fcsize(r6),r6	# -> ioblk
	movl	$b$xrt,(r6)
	movl	$iosize,iolen(r6)
	movl	r9,iofnm(r6)
	clrl	iopid(r6)
	movab	iosize(r6),iobuf(r6)
	movl	tioblk+iofdn,iofdn(r6)
	movl	tioblk+ioflg,ioflg(r6)
#
#	if -f0 or -f1 specified then
#
#		for -f0 ensure that buffer is same as osint's
#
#		for -f1 no buffering should be done
#
	bitl	$IO_SYS,ioflg(r6)
	beqlu	9f
	cmpl	$1,iofdn(r6)
	blssu	9f
	beqlu	1f
	movl	$inpbf,iobuf(r6)
	jbr	9f
1:	clrl	iobuf(r6)
	bisl2	$IO_WRC,ioflg(r6)
9:
#
#	fill in bfblk
#
	movab	iosize(r6),r6	# -> bfblk
	movl	$b$xnt,(r6)
	movl	bfblksiz,bflen(r6)
	movl	tioblk+iopid,bfsiz(r6)
	clrl	bfrem(r6)
	clrl	bfoff(r6)
#
iodon:	movl	fcblkptr,r10
	clrl	r8
	popr	$mr235
	addl2	$8,(sp)
	rsb
#
#	syspi - print on interactive channel (terminal)
#
	.globl	syspi
syspi:
	movl	$ttyiob,r11
	jbr	piprt
#
#	syspr - print on standard output
#
	.globl	syspr
syspr:
	movl	$oupiob,r11
#
#	handle both syspi and syspr here.
#
piprt:
	pushr	$mr235
	bisl2	$IO_WRC,ioflg(r11) # briefly set no buffering
	pushl	r9		# -> scblk
	pushl	r11		# -> ioblk
	pushl	r6		# number characters
	pushl	$1		# line mode
	calls	$4,_oswrite	# call to do write
	bicl2	$IO_WRC,ioflg(r11) # back to buffering
	tstl	r0		# if output error then
	jneq	erxit1		#    indicate error return
	popr	$mr235
	addl2	$4,(sp)
	rsb
#
#	sysrd - read from standard input
#
	.globl	sysrd
sysrd:
	pushr	$mr235
	movl	$inpiob,ioblkptr
#
#	handle both sysrd and sysri here.
#
rdmrg:
	pushl	r9		# -> scblk
	pushl	ioblkptr	# -> ioblk
	pushl	r8		# read length
	pushl	r8		# line mode
	calls	$4,_osread	# call to do read
	cmpl	r0,$-1		# check for eof or input error
	jeql	rdeof		#    take eof exit
	jlss	erxit1		#    take error exit
	movl	r0,sclen(r9)	# set read length
#
#	check for 1st record of standard input coming from a file specified
#	on the command line. if all of these conditions are true, allow
#	the program to access any arguments following the file name.
#
	tstl	rdrec1		# if already ready record 1 then
	bnequ	rdskp		#    skip
	cmpl	$inpiob,ioblkptr
	bnequ	rdskp		# if sysri entry then skip
	incl	rdrec1		# indicate read 1st record from std input
	tstl	_inpptr		# if not file from command line then
	beqlu	rdskp		#    skip
	cmpb	$'#,scstr(r9)	# if 1st char not # then
	bnequ	rdskp		#    skip
	cmpb	$'!,scstr+1(r9)	# if 2nd char not ! then
	bnequ	rdskp		#    skip
	subl3	_inpcnt,_gblargc,_cmdcnt
	incl	_cmdcnt		# compute # args after filename
	movl	$1,_inpcnt	# reset input count
	brb	rdmrg		# ignore 1st record and try again
rdskp:
	popr	$mr235
	addl2	$4,(sp)
	rsb
#
#	come here to handle eof for both sysrd and sysri. if eof
#	is for sysrd, standard input, switch to next input file
#	if one exists.
#
rdeof:	movl	ioblkptr,r4	# -> ioblk
	tstl	iofdn(r4)	# if not file descriptor 0 then
	jnequ	erxit1		#    real eof
	pushl	_inpptr		# push -> array of pointers
	pushl	_inpcnt		# push size of areray
	calls	$2,_swcinp	# call to switch input files
	tstl	r0		# if more to read then
	jeqlu	rdmrg		#    read it
	jmp	erxit1		# else signal eof
#
#	sysri - read from interactive channel (terminal)
#
	.globl	sysri
sysri:
	pushr	$mr235
	movl	$ttyiob,ioblkptr
	jbr	rdmrg
#
#	sysst - set file pointer
#
	.globl	sysst
sysst:
	pushr	$mr235
	pushl	fciob(r6)	# -> ioblk
	calls	$1,_osopen	# call to do open
	tstl	r0		# if file open error then
	jnequ	erxit3		#    return error
#
	movl	fciob(r6),r1	# -> ioblk
	bitl	$IO_PIP,ioflg(r1)
	jnequ	erxit4		# if pipe then set not allowed
	cmpl	iofdn(r1),$2	# if fd < 2 then
	jlssu	erxit4		#    set not allowed
#
	cmpl	$b$icl,(r7)	# if already integer then
	bnequ	0f
	movl	icval(r7),temp1	#    grab value
	brb	1f
0:	cmpl	$b$scl,(r7)	# else if not a string then
	jnequ	erxit1		#    error
	clrl	temp3		# clear scnint character count
	pushl	$temp3		# -> temp3
	pushl	sclen(r7)	# string length
	pushab	scstr(r7)	# -> string
	calls	$3,_scnint	# call to scan integer
	movl	r0,temp1	# and save
1:
#
	cmpl	$b$icl,(r8)	# if already integer then
	bnequ	0f
	movl	icval(r8),temp2	#    grab value
	brb	1f
0:	cmpl	$b$scl,(r8)	# else if not a string then
	jnequ	erxit1		#    error
	clrl	temp3		# clear scnint character count
	pushl	$temp3		# -> temp3
	pushl	sclen(r8)	# string length
	pushab	scstr(r8)	# -> string
	calls	$3,_scnint	# call to scan integer
	movl	r0,temp2	# and save
1:
#
	pushl	temp2		# whence
	pushl	temp1		# offset
	pushl	fciob(r6)	# -> ioblk
	calls	$3,_doset	# call to do set
	popr	$mr235
	addl2	$20,(sp)
	rsb
#
#	systm - get execution time so far
#
	.globl	systm
systm:
	pushr	$mr2+mr3
	movl	$tscblk+8,-(sp)	# -> times buffer
	calls	$1,_times	# call to do times
	movl	tscblk+8,r5	# get user time in 60ths
	mull2	$100,r5		#    mulitply by 100 to get 6000ths
	divl2	$6,r5		#    divide by 6 to get 1000ths
	popr	$mr2+mr3
	rsb
#
#	sysxi - exit from executing program
#
	.globl	sysxi
sysxi:
	tstl	r10		# if 0 instead of scblk then
	jeqlu	xilmod		#    try to write load module 
	pushr	$mr235
	cmpl	$b$scl,(r10)	# if not scblk then
	jnequ	erxit1		#    error
	pushl	r10		# push scblk pointer
	calls	$1,_doexec	# go do exit
	jmp	erxit2		# should never return
#
#	write load module
#
xilmod:	tstl	r5		# if r5 <= 0 then
	bgtr	0f
	pushr	$mr235		#    save regs for error exits
	jbr	erxit1		#    and take error exit
0:	pushr	$cmpreg		# else save all compiler regs
#
#	need to save stack contents, so that when load module is
#	invoked, stack can be recreated.
#
	subl3	sp,_initsp,r0	# compute depth of stack
	cmpl	r0,tscblk	# if stack won't fit in tscblk then
	jgtru	xi2big		#    big trouble
	movl	sp,r0		# -> into real stack
	movab	tscblk+scstr,r1	# -> save stack area
1:	movl	(r0)+,(r1)+	# copy word of stack ...
	cmpl	r0,_initsp	#    until hit top word
	blssu	1b
	movl	r1,lmodstk	# set top of saved stack
#
#	before creating the load module, we must relativize the
#	compiler cells that point into the stack.  We do this by
#	temporarily negating _initsp, calling streloc, and then
#	restoring _initsp.  After the load module has been written,
#	another call to streloc will restore the stack pointers.
#
	mnegl	_initsp,_initsp	# negate _initsp so streloc will subtract
	jsb	streloc		# relativize the compiler cells
	mnegl	_initsp,_initsp	# restore _initsp to its previous value
#
#	create a.out header in hststr scblk.
#
	addl3	$1023,dnamp,r1	# round current memory in use
	bicl3	$0x3ff,r1,-(sp)	#   to a multiple of the page size
	movab	hststr+scstr,r0	# -> a.out header block
	pushl	r0		#   which will be the other argument
	movl	$0413,(r0)+	# set magic number
	bicl3	$0x3ff,$_etext,r1 # get text size, rounded down
	movl	r1,(r0)+	#   and place it in a.out header
	subl3	r1,4(sp),(r0)+	# data size = total - text size
	clrl	(r0)+		# we will use no bss
	clrl	(r0)+
	clrl	(r0)+		# set starting address
	clrl	(r0)+
	clrl	(r0)+
#
#	call a workhorse c routine to actually write a.out file.
#	the amount of memory to write has already been pushed.
#
	calls	$2,_wrtaout	# call to write a.out
#
#	restore compiler cells to their previous values
#
	jsb	streloc		# unrelativize stack pointers
#
	tstl	r0		# if error creating a.out then
	blss	xi2big		#    return error
#
#	pop registers and set up call to sysej
#
	popr	$cmpreg		# restore all registers
	movl	r7,r10		# -> chain of fcbs
	clrl	r7		# set &CODE = 0
	jsb	sysej		# call to end run
#
#	if stack too big
#
xi2big:	popr	$cmpreg		# restore all regs
	pushr	$mr235		# push correct regs
	jbr	erxit2		# take error exit
#
#	error/ppm exits - pick up n-th word following jsb and return
#	to address contained in that word.
#
erxit1:
	popr	$mr235
	movl	(sp)+,r11
	jmp	*(r11)+
#
erxit2:
	popr	$mr235
	addl3	$4,(sp)+,r11
	jmp	*(r11)+
#
erxit3:
	popr	$mr235
	addl3	$8,(sp)+,r11
	jmp	*(r11)+
#
erxit4:
	popr	$mr235
	addl3	$12,(sp)+,r11
	jmp	*(r11)+
#
erxit5:
	popr	$mr235
	addl3	$16,(sp)+,r11
	jmp	*(r11)+
#
erxit6:
	popr	$mr235
	addl3	$20,(sp)+,r11
	jmp	*(r11)+
#
#	streloc - relocate stack pointers.  this routine adds
#	_initsp to every cell whose address appears in strellst.
#
streloc:
	pushr	$mr0+mr1
	moval	_strellst,r1	# start of list of thing to relocate
	jbr	strel1		# jump into the loop
strel0:	addl2	_initsp,(r0)	# relocate a pointer
strel1:	movl	(r1)+,r0	# fetch a pointer to a cell
	jneq	strel0		# if zero, we're done
	popr	$mr0+mr1
	rsb	
#
#	interface data area
#	-------------------
#
	.data	1
#
#	#! data areas
#
	.globl	_cmdcnt
_cmdcnt:	.long	0		# number of  command args
rdrec1:	.long	0		# read record 1 from std in flag
#
#	standard ioblks
#
inpiob:	.space	iobuf
	.long	inpbf		# -> input bfblk
	.long	0		# file descriptor
	.long	IO_INP|IO_OPN|IO_SYS
#
inpbf:	.space	bfsiz
	.long	1024		# buffer size
	.long	0		# remaining chars to read
	.long	0		# offset to next character to read
	.space	1024		# buffer
#
#
oupiob:	.space	iobuf
	.long	0		# no buffer
	.long	1		# file descriptor number
	.long	IO_OUP|IO_OPN|IO_SYS
#
#
ttyiob:	.space	iobuf
	.long	ttybf		# -> tty buffer input
	.long	2		# file descriptor number
	.long	IO_INP|IO_OUP|IO_OPN|IO_SYS
#
ttybf:	.space	bfsiz
	.long	258		# buffer size
	.long	0		# remaining chars to read
	.long	0		# offset to next char to read
	.space	258		# buffer
	.align	2
#
	.globl	_rzfcb
_rzfcb:	.long	0
#
fildes:	.long	0
pr_len:	.long	0
rd_len:	.long	0
lenfname: .long	0
ioblkptr: .long 0
bfblkptr: .long	0
bfblksiz: .long	0
fcblkptr: .long	0
tioblk:	.space	iosize
#
#	flag that indicates that this is a load module. also, serves
#	the dual purpose of indicating size of saved stack.
#
lmodstk: .long	0
#
temp1:	.long	0
temp2:	.long	0
temp3:	.long	0
#
nulstr:	.long	0,0
#
tscblk:	.long	512,0
	.space	512
#
hststr:	.long	128,0
	.space	128
