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
#	this operating system interface, commonly referred to as osint,
#	provides the Macro SPITBOL compiler with a means to do all i/o
#	and other OS dependent activities. 
#
#	osint is composed of one medium sized assembly module, with
#	multiple entry points, and a collection of c functions. the
#	entire interface is not in c due to the compiler's use of
#	registers to pass parameters and other technicalities. all
#	entries, except for sbchk, have five letter names starting
#	with sys.
#
#	when spitbol first starts execution, control passes to function
#	main, just like in a normal c program. main processes any options
#	and does other initialization chores. after completing
#	initialization, main jumps to label sec04 to start
#	the compiler itself. from that point, the compiler makes calls
#	to sysxx routines to get service.
#
#	the c functions are called from this module to do most of the
#	hard and/or unix dependent work. an effort has been made to
#	do all the system calls from c functions. this goal has not
#	been entirely achieved.
#	
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
#	osint( argc,argv,environ ) is called just like the main
#	function of a c program.
#
	.globl	_main
_main:	.word	0
#
#	normal start - process all command arguments.
#
	movl	4(ap),r4	# get number of options
	movl	r4,argc		#    and save
	movl	8(ap),r6	# -> option pointers
	movl	r6,argv		#    and save
#
#	if this is a restart from an load module, go handle.
#
	tstl	lmodstk		# if load module stack != 0 then
	jnequ	rstart		#    go handle
#
	tstl	(r6)+		# program name not interesting
	brb	gtcont		# process other args
gtarg:	movl	(r6)+,r0	# -> option text
	cmpb	$ch$mn,(r0)	# if no leading - then
	bnequ	gtinp		#    treat as input filename
	incl	r0		# bump over -
1:	movb	(r0)+,curopt+1	# set option character in table
	movl	$opttbl,r1	# -> option table
2:	cmpl	curopt,(r1)	# if we have found it then
	beqlu	3f		#    process it
	addl2	$optsiz,r1	# -> next table entry
	brb	2b		# loop until found
3:	movl	optrtn(r1),r11	# -> option routine
	jsb	(r11)		# call option routine
4:	tstb	(r0)		# if char is binary zeros then
	beqlu	gtcont		#    done with this arg
	cmpb	$040,(r0)	# if next char is blank then
	bnequ	5f
	incl	r0		#    ignore it
	brb	4b		#    and check next char
5:	cmpb	$ch$mn,(r0)	# if next char is not - then
	bnequ	1b		#    treat as option character
	incl	r0		# else skip over -
	brb	1b		#     and treat next char as option
gtinp:	jsb	optinp		# process input filename(s)
gtcont:	sobgtr	r4,gtarg	# loop thru all options
#
#	switch to proper input file
#
	pushl	inpptr
	pushl	inpcnt
	calls	$2,_swcinp
#
#	call to do initial switch of output files.
#
	pushl	oupptr
	calls	$1,_swcoup
#
#	see if standard output is tty or not.
#
	pushl	$1		# file descriptor 1
	calls	$1,_testty	# call to check i/o chcaracteristics
	tstl	r0		# if r0 not 0 then
	bnequ	0f		#    not a tty
	clrl	lnsppg		# reset # lines per page
	bisl2	$prtich,sptflg	# else tell compiler
0:
#
#	set signals for execution
#
	.globl	_setsigs
	calls	$0,_setsigs	# trap overflow signals
#
#	allocate initial dynamic memory
#
	mull3	meminc,$4,meminb# convert meminc to bytes
	pushl	meminb		# memory request increment
	calls	$1,_sbrk	# call to system call
	movl	r0,basmem	# save base of memory
	addl3	r0,meminb,topmem# computer top of memory
	mull3	datwds,$4,r1	# convert max data words to bytes
	addl3	r0,r1,maxmem	# computer top of memory
0:	clrl	(r0)+		# clear initial allocation
	cmpl	r0,topmem	# loop until all cleared
	blssu	0b
#
#	set up lowest legal sp value, so that stack overflow can be detected,
#
	mull3	$4,stksiz,r0	# convert words to bytes
	subl3	r0,sp,lowsp	#    and compute lowest sp
	movl	sp,initsp	# save initial sp
#
#	clear registers, set dynamic area pointers, and jump off
#	to compiler
#
	clrl	r2
	clrl	r3
	clrl	r4
	clrl	r5
	clrl	r6
	clrl	r7
	clrl	r8
	movl	basmem,r9
	subl3	$4,topmem,r10
	jmp	sec04
#
#	here to restart program after EXIT() call. this means we are now
#	executing from an a.out file created by the interface.
#
rstart:
#
#	before restoring stack, set up values for proper checking of
#	stack overflow. (initial sp here will most probably differ
#	from initial sp when compile was done.)
#
	mull3	$4,stksiz,r0	# convert words to bytes
	subl3	r0,sp,lowsp	#    and compute lowest sp
	movl	sp,initsp	# save initial sp
	jsb	streloc		# relocate pointers into stack
#
#	restore stack from tscblk.
#
	movl	lmodstk,r0	# -> bottom word of stack
	movab	tscblk+scstr,r1	# -> top word of stack
0:	movl	-(r0),-(sp)	# relocate word of stack
	cmpl	r0,r1		# if not at end of relocation then
	bgtru	0b		#    loop back
#
#	if restarting, we can always access the command line arguments
#
	movl	$1,cmdcnt
#
#	the system break will not be what it should, so reset it
#
	pushl	topmem
	calls	$1,_brk
#
#	reset signals to what they should be.
#
	calls	$0,_setsigs
#
#	forget about files open during compilation -
#
	clrl	inpptr		# no input files
	clrl	inpcnt		#    so count is 0 too
	clrl	oupptr		# no output file
	clrl	errfdn		# no error file
#
#	reset standard input buffer
#
	clrl	inpbf+bfrem	# no remaining chars
	clrl	inpbf+bfoff	# offset to next char
#
#	restore compiler's registers and off we go.
#
	popr	$cmpreg		# restore compiler's registers
	addl2	$8,(sp)
	rsb
#
#	sbchk is called by the compiler to check for stack overflow.
#
	.globl	sbchk
sbchk:	cmpl	sp,lowsp	# if sp is ok then
	blssu	0f
	rsb			#    return
0:	tstl	(sp)+		# else pop stack
	jmp	sec05		#    and go to stack overflow section
#
#	unsupported entries that simply return.
#
	.globl	sysdc
sysdc:
	.globl	sysdm
sysdm:
	.globl	systt
systt:
	.globl	sysul
sysul:
	rsb
#
#	unsupported that take error returns
#
	.globl	sysex
sysex:
	.globl	sysld
sysld:
	pushr	$mr235		# save ye registers
	jbr	erxit1
#
#	sysax - after execution call
#
#	sysax is called immediately after execution, so that the interface
#	can do any cleanup. here, the standard output file is switched, if
#	necessary back to the listing file.
#
	.globl	sysax
sysax:
	pushr	$mr235
	pushl	oupptr
	calls	$1,_swcoup
	popr	$mr235
	rsb
#
#	sysbx - before execution call
#
#	sysbx is called after compilation and before execution begins.
#	sysbx allows the inteface to do any cleanup. here, the standard
#	listing file is switched to the standard output file.
#
	.globl	sysbx
sysbx:
	pushr	$mr235
	pushl	oupptr
	calls	$1,_swcoup
	popr	$mr235
	rsb
#
#	sysdt - get current date
#
#	return both date and time
#
#	jsb	sysdt			# call to get date
#	(r10)				# -> scblk
#
	.globl	sysdt
sysdt:
	pushr	$mr235
	movl	$tscblk,r10
	movl	$17,sclen(r10)		# dd/mm/yy hh.mm.ss
	pushl	$tscblk+scstr
	calls	$1,_getdate
	popr	$mr235
	rsb
#
#	sysef - eject file
#
	.globl	sysef
sysef:
	pushr	$mr235
	pushl	fciob(r6)	# -> ioblk
	calls	$1,_osopen	# call to do open
	tstl	r0		# if open error then
	jnequ	erxit1		#    take error exit
	pushl	$ffscb		# -> ff scblk
	pushl	fciob(r6)	# -> ioblk
	pushl	ffscb+sclen	# record length
	mnegl	fcrsz(r6),-(sp)	# i/o mode - raw or line
	calls	$4,_oswrite	# call to do write
	tstl	r0		# if output error then
	jneq	erxit3		#    signal failure
	popr	$mr235
	addl2	$12,(sp)
	rsb
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
#
#	sysem - get error message text
#
	.globl	sysem
sysem:
	pushr	$mr235
	movl	$tscblk,r9	# -> temp scblk
	clrl	sclen(r9)	# default error text is null string
	tstl	errfdn		# if error fd exists then
	bnequ	0f		#    skip open
	calls	$0,_openerr	# else open error text file
	movl	r0,errfdn	#    and save fd
0:	tstl	errfdn		# if no error text file then
	blss	emxit		#    return null string
	decl	r6		# errors start at 1
	blss	emxit		#    but don't have error 0
	mull2	$49,r6		# compute byte offset of error text
	pushl	$0		# whence
	pushl	r6		# offset
	pushl	errfdn		# file descriptor number
	calls	$3,_lseek	# call to do seek
	tstl	r0		# if lseek fails then
	blss	emxit		#    return null string
	pushl	$48		# error text length w/out nl
	pushl	$tscblk+8	# buffer address
	pushl	errfdn		# file descriptor number
	calls	$3,_read	# call to do read
	tstl	r0		# if read failed then
	blss	emxit		#    return null string
1:	cmpb	$' ,tscblk+7(r0)# if last character is
	bneq	2f		#   nonblank, return length
	sobgeq	r0,1b		# otherwise discard the blank, try again
2:	movl	r0,sclen(r9)	# set actual length
emxit:
	popr	$mr235
	rsb			# return
#
#	sysen - endfile
#
	.globl	sysen
sysen:
	pushr	$mr235
	movl	r$fcb,_rzfcb	# copy fcb chain head
	pushl	fciob(r6)	# -> ioblk
	calls	$1,_osclose	# call to do close
	popr	$mr235
	addl2	$12,(sp)
	rsb
#
#	sysej - eject standard output
#
	.globl	sysep
sysep:
	pushr	$mr235
	pushl	$1		# 1 character
	pushl	$ffstr		# -> ff
	pushl	$1		# fd 1
	calls	$3,_write	# call to do write
	popr	$mr235
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
#	syshs
#
	.globl	syshs
syshs:
	pushr	$mr235
	cmpl	$b$icl,(r6)	# if arg1 not integer then
	jnequ	9f		#    return host string
	tstl	icval(r6)	# if integer 0 then
	beqlu	1f		#    return -u argument
	cmpl	$1,icval(r6)	# if integer 1 then
	beqlu	2f		#    do system call
	cmpl	$2,icval(r6)	# if integer 2 then
	jeqlu	3f		#    return command arg
	cmpl	$3,icval(r6)	# if integer 3 then
	jeqlu	4f		#    return number of 1st #! arg
	cmpl	$4,icval(r6)	# if integer 4 then
	jeqlu	5f		#    return value of environment variable
	jbr	erxit1		# else arg error
#
1:	tstl	uarg		# if uarg is 0 then
	jeqlu	erxit4		#    return null string
	pushl	tscblk		# push scblk string length
	pushl	$tscblk		# -> temp scblk
	clrl	-(sp)		# ending character is 0
	pushl	uarg		# -> -u argument
	calls	$4,_cpys2sc	# copy string to scblk
	movab	tscblk,r10	# -> temp scblk
	jbr	erxit3		# return
#
2:	cmpl	$b$scl,(r10)	# if 2nd arg not string then
	jnequ	erxit1		#    return error
	tstl	sclen(r10)	# if null string then
	jeqlu	erxit4		#    return null string
	pushl	r10		# -> command string
	calls	$1,_dosys	# call to do system call
	jbr	erxit4		# return null string
#
3:	cmpl	$b$icl,(r10)	# if 2nd arg not integer then
	jnequ	erxit1		#    return error
	movl	tscblk,tscblk+sclen	# set max length of scblk
	pushab	tscblk		# push -> tscblk
	pushl	argv		# push -> pointers
	pushl	argc		# push number of args
	pushl	icval(r10)	# arg requested
	calls	$4,_arg2scb	# call to do real move
	tstl	r0		# if out of range then
	jlss	erxit6		#    fail
	jeqlu	erxit4		#    (if 0) return null string
	movab	tscblk,r10	# -> tscblk
	jbr	erxit3		# return
#
4:	tstl	cmdcnt		# if not invoked by #! then
	jeqlu	erxit6		#    fail
	movab	temp1,r9	# -> temp icblk
	movl	$b$icl,(r9)	# set integer block
	movl	cmdcnt,icval(r9)# set value
	jbr	erxit5		# return result
#
5:	cmpl	$b$scl,(r10)	# if 2nd arg not string then
	jnequ	erxit1		#    return error
	tstl	sclen(r10)	# if null string then
	jeqlu	erxit1		#    return error
	movl	tscblk,tscblk+sclen	# set max length of scblk
	pushab	tscblk		# push -> tscblk
	pushl	r10		# -> environment variable requested
	calls	$2,_rdenv	# fetch the environment variable
	tstl	r0		# if it couldn't be found
	jlss	erxit6		#    fail
	movab	tscblk,r10	# else return tscblk
	jbr	erxit3
#
9:	pushl	hststr		# push length of host string
	pushab	hststr		# push -> host string scblk
	calls	$2,_gethost	# call to get host string
	tstl	hststr+sclen	# if null host string then
	jeqlu	erxit4		#    return null string
	movl	$hststr,r10	# -> host string
	jbr	erxit3		# return
#
#	sysid - return system id
#
	.globl	sysid
sysid:
	movl	$id1,r9
	movl	$id2,r10
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
#	try to open the file
#
	movl	fcblkptr,r0
	pushl	fciob(r0)
	calls	$1,_osopen
	tstl	r0
	jneq	erxit2		# if open failed, indicate error
#
iodon:	movl	fcblkptr,r10
	clrl	r8
	popr	$mr235
	addl2	$8,(sp)
	rsb
#
#	sysmm - get more memory
#
	.globl	sysmm
sysmm:
	cmpl	topmem,maxmem	# if already at top of memory then
	blssu	0f
	clrl	r9		#    no more to be had
	rsb
0:				# else {alloc some more}
	pushr	$mr235
	pushl	meminb		# size in bytes of memory request
	calls	$1,_sbrk	# call to get memory
	popr	$mr235
	tstl	r0		# if memory obtained then
	blss	1f
	addl2	meminb,topmem	#    adjust current top
	movl	meminc,r9	#    set number of words in block
	rsb			#    return
1:	clrl	r9		# else nothing to get
	rsb
#
#	sysmx - get maximum size of created objects
#
	.globl	sysmx
sysmx:
	mull3	$4,maxsiz,r6
	rsb
#
#	sysou - output record
#
	.globl	sysou
sysou:
	pushr	$mr235
	pushl	fciob(r6)	# -> ioblk
	calls	$1,_osopen	# call to do open
	tstl	r0		# if open error then
	jnequ	erxit1		#    take error exit
	pushl	r9		# -> scblk
	pushl	fciob(r6)	# -> ioblk
	pushl	sclen(r9)	# record length
	pushl	fcrsz(r6)	# i/o mode - raw or line
	calls	$4,_oswrite	# call to do write
	tstl	r0		# if output error,
	jneq	erxit2		#    take error exit
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
#	syspp - return print parameters
#
	.globl	syspp
syspp:
	movl	pagwid,r6
	movl	lnsppg,r7
	movl	sptflg,r8
	movl	defcas,kvcas
	rsb
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
	tstl	inpptr		# if not file from command line then
	beqlu	rdskp		#    skip
	cmpb	$'#,scstr(r9)	# if 1st char not # then
	bnequ	rdskp		#    skip
	cmpb	$'!,scstr+1(r9)	# if 2nd char not ! then
	bnequ	rdskp		#    skip
	subl3	inpcnt,argc,cmdcnt
	incl	cmdcnt		# compute # args after filename
	movl	$1,inpcnt	# reset input count
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
	pushl	inpptr		# push -> array of pointers
	pushl	inpcnt		# push size of areray
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
#	sysrw - rewind file
#
	.globl	sysrw
sysrw:
	pushr	$mr235
	pushl	fciob(r6)	# -> ioblk
	calls	$1,_osopen	# call to do open
	tstl	r0		# if open error then
	jnequ	erxit1		#    take error exit
	movl	fciob(r6),r1	# -> ioblk
	bitl	$IO_PIP,ioflg(r1)
	jnequ	erxit2		# if pipe then rewind not allowed
	cmpl	iofdn(r1),$2	# if fd < 2 then
	jlssu	erxit2		#    rewind not allowed
	pushl	$0		# whence
	pushl	$0		# offset
	pushl	fciob(r6)	# -> ioblk
	calls	$3,_doset	# call to do set
	popr	$mr235
	addl2	$12,(sp)
	rsb
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
	subl3	sp,initsp,r0	# compute depth of stack
	cmpl	r0,tscblk	# if stack won't fit in tscblk then
	jgtru	xi2big		#    big trouble
	movl	sp,r0		# -> into real stack
	movab	tscblk+scstr,r1	# -> save stack area
1:	movl	(r0)+,(r1)+	# copy word of stack ...
	cmpl	r0,initsp	#    until hit top word
	blssu	1b
	movl	r1,lmodstk	# set top of saved stack
#
#	before creating the load module, we must relativize the
#	compiler cells that point into the stack.  We do this by
#	temporarily negating initsp, calling streloc, and then
#	restoring initsp.  After the load module has been written,
#	another call to streloc will restore the stack pointers.
#
	mnegl	initsp,initsp	# negate initsp so streloc will subtract
	jsb	streloc		# relativize the compiler cells
	mnegl	initsp,initsp	# restore initsp to its previous value
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
#	initsp to every cell whose address appears in strellst.
#
streloc:
	pushr	$mr0+mr1
	moval	strellst,r1	# start of list of thing to relocate
	jbr	strel1		# jump into the loop
strel0:	addl2	initsp,(r0)	# relocate a pointer
strel1:	movl	(r1)+,r0	# fetch a pointer to a cell
	jneq	strel0		# if zero, we're done
	popr	$mr0+mr1
	rsb	
#
#	option routines
#
#	optclr	clears a flag
#	opterr	signals an error
#	optfld	sets defcas to 0 for no folding
#	optnum	get numeric value
#	optset	set option value
#
#	optclr
#
optclr:	bicl2	optflg(r1),sptflg
	rsb
#
#	opterr
#
opterr:	pushr	$mr0+mr1+mr2+mr3+mr4+mr5
	pushl	$6
	pushl	$curopt
	pushl	$2
	calls	$3,_write
	popr	$mr0+mr1+mr2+mr3+mr4+mr5
	rsb
#
#	optinp
#
optinp:	tstl	inpptr		# if already processed input filenames then
	bnequ	opterr		#    error
	subl3	$4,r6,inpptr	# -> first input filename
	movl	r4,inpcnt	# set number of filenames
	movl	$1,r4		# done scanning options
	rsb			# return
#
#	optfld
#
optfld:	clrl	defcas
	rsb
#
#	optnum
#
optnum:	pushl	r0		# -> number
	jsb	getnum		# get number
	movl	(sp)+,r0	# -> byte past last digit
	movzbl	(r0),r2		# get byte past last digit
	bisb2	$040,r2		# fold to lower case
	cmpb	$ch$lk,r2	# if number followed by k then
	bnequ	0f
	mull2	$1024,r5	#    mulitply by 1024
	incl	r0		#    skip over k
0:	tstl	r5		# if number zero or negative
	bleq	opterr		#    treat as error
	movl	r5,*optflg(r1)	# store option
	rsb			# return
#
#	optoup
#
optoup:	cmpl	$2,r4		# if no option after -o then
	bgtru	opterr		#    error
	movl	(r6),r1		# -> output filename
	cmpb	$ch$mn,(r1)	# if filename starts with - then
	beqlu	opterr		#    error
	movl	(r6)+,oupptr	# save pointer to output filename
	decl	r4		# one less option to process
	rsb			# return
#
#	optset
#
optset:	bisl2	optflg(r1),sptflg
	rsb
#
#	optusr
#
optusr:	cmpl	$2,r4		# if fewer than 2 options then
	jgtru	opterr		#    can't have argument
	movl	(r6)+,uarg	# save -> argument
	decl	r4		# dec number of remaining options
	rsb
#
#	getnum
#
#	(sp)			-> string to convert
#	jsb	getnum
#	(sp)			-> char after last digit
#	(r5)			converted number
#
getnum:
	movl	4(sp),r7		# -> string
	clrl	r5		# clear accumulator
0:	cmpb	$060,(r7)	# if not a decimal digit then
	bgtru	1f		#    done with conversion
	cmpb	$071,(r7)	# 
	blssu	1f
	movzbl	(r7)+,r8	# load digit
	subl2	$060,r8		# remove  unnecessary bits
	mull2	$10,r5		# accum * 10
	addl2	r8,r5		# add in this digit
	brb	0b
1:	movl	r7,4(sp)		# return address of next byte
	rsb			# return
#
#	interface data area
#	-------------------
#
	.data	1
#
#	flags for compiler
#
	.set	errors,1	# send errors to terminal
	.set	prtich,2	# standard printer is terminal
	.set	nolist,4	# suppress compilation listing
	.set	nocmps,8	# suppress compilation statistics
	.set	noexcs,16	# suppress execution statistics
	.set	lnglst,32	# generate page ejects
	.set	noexec,64	# suppress program execution
	.set	trmnal,128	# terminal i/o association
	.set	stdlst,256	# standard listing (intermediate)
	.set	nohedr,512	# suppress sysid header
#
	.set	deflag,errors+nolist+nocmps+noexcs+trmnal+nohedr
#
#	option table
#
	.set	opttxt,0	# option characters
	.set	optflg,4	# option flag - flags or address
	.set	optrtn,8	# -> option processing routine
	.set	optsiz,12	# size in bytes of entry
#
opttbl:
	.ascii	"-f  "
	.long	0,optfld
#
	.ascii	"-e  "
	.long	errors,optclr
#
	.ascii	"-l  "
	.long	nolist,optclr
#
	.ascii	"-c  "
	.long	nocmps,optclr
#
	.ascii	"-x  "
	.long	noexcs,optclr
#
	.ascii	"-a  "
	.long	nolist+nocmps+noexcs,optclr
#
	.ascii	"-p  "
	.long	lnglst,optset
#
	.ascii	"-z  "
	.long	stdlst,optset
#
	.ascii	"-h  "
	.long	nohedr,optclr
#
	.ascii	"-n  "
	.long	noexec,optset
#
	.ascii	"-m  "
	.long	maxsiz,optnum
#
	.ascii	"-s  "
	.long	stksiz,optnum
#
	.ascii	"-d  "
	.long	datwds,optnum
#
	.ascii	"-i  "
	.long	meminc,optnum
#
	.ascii	"-o  "
	.long	0,optoup
#
	.ascii	"-u  "
	.long	0,optusr
#
curopt:	.ascii	"-   "
	.ascii	"?\n  "
	.long	opterr
	.align	2
#
#	standard input/output pointers
#
inpcnt:	.long	0
inpptr:	.long	0
oupptr:	.long	0
#
#	pointer to -u arg
#
uarg:	.long	0
#
#	save argc and argv from initial call
#
argc:	.long	0
argv:	.long	0
#
#	#! data areas
#
cmdcnt:	.long	0		# number of  command args
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
#	memory allocation variables
#
meminc:	.long	4*k		# increment in words for sbrk
meminb:	.long	0		# meminc * 4 (to get bytes)
datwds:	.long	256*k		# max size in words of dynamic area
basmem:	.long	0		# base of dynamic memory
topmem:	.long	0		# current top of dynamic memory
maxmem:	.long	0		# maximum top of dynamic memory
maxsiz:	.long	8*k		# maximum object size in words
stksiz:	.long	2*k		# stack size in words
initsp:	.long	0		# initial value of sp on entry to sec04
lowsp:	.long	0		# lowest legal sp value
#
#	default value for &case
#
defcas:	.long	1
#
#	values given to syspp for print parameters
#
lnsppg:	.long	60		# lines per page
pagwid:	.long	120		# page width
sptflg:	.long	deflag		# flags
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
#
id1:	.long	0,id1l
	.ascii	"(0.0)"
id1e:
	.set	id1l,id1e-id1-8
	.align	2
#
id2:	.long	0,id2l
	.ascii	"VAX/UNIX Version"
id2e:
	.set	id2l,id2e-id2-8
	.align	2
#
ffscb:	.long	0,1
ffstr:	.byte	12
#
nlstr:	.ascii	"\n"
	.align	2
#
errfdn:	.long	0
#
#	The following pointers address those cells in the compiler
#	that point into the stack when a load module might be written,
#	and which must therefore be relocated.
strellst:
	.long	flptr
	.long	stbas
	.long	gtcef
	.long	0		# end of list marker
