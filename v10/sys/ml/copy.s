	.text

	.set	MOVCMAX,0xffff		# largest single movc
#
# copyin and copyout
#
_Copyin:	.globl	_Copyin		# <<<massaged for jsb by asm.sed>>>
	movl	12(sp),r0		# copy length
	blss	ersb
	movl	4(sp),r1		# copy user address
	cmpl	$NBPG,r0		# probing one page or less ?
	blss	ciloop			# no
cishort:
	prober	$3,r0,(r1)		# bytes accessible ?
	beql	ersb			# no
docp:
	pushl	_nofault
	movab	ersbp,_nofault
cpagain:
	cmpl	16(sp),$MOVCMAX
	bgtr	cpbig
	movc3	16(sp),*8(sp),*12(sp)
	movl	(sp)+,_nofault	
#	clrl	r0		# movc leaves 0 in r0
	rsb
ersbp:
	movl	(sp)+,_nofault	
ersb:
	mnegl	$1,r0
	rsb

cpbig:
	movc3	$MOVCMAX,*8(sp),*12(sp)
	subl2	$MOVCMAX,16(sp)	# new len
	movl	r1,8(sp)	# new source, from movc
	movl	r3,12(sp)	# new dest, from movc
	brb	cpagain
ciloop:
	prober	$3,$NBPG,(r1)		# bytes accessible ?
	beql	ersb			# no
	addl2	$NBPG,r1		# incr user address ptr
	acbl	$NBPG+1,$-NBPG,r0,ciloop	# reduce count and loop
	brb	cishort

_Copyout: 	.globl	_Copyout	# <<<massaged for jsb by asm.sed >>>
	movl	12(sp),r0		# get count
	blss	ersb
	movl	8(sp),r1		# get user address
	cmpl	$NBPG,r0		# can do in one probew?
	blss	coloop			# yes
coshort:
	probew	$3,r0,(r1)		# bytes accessible?
	beql	ersb			# no
	brw	docp
coloop:
	probew	$3,$NBPG,(r1)		# bytes accessible?
	beql	ersb			# no 
	addl2	$NBPG,r1		# increment user address
	acbl	$NBPG+1,$-NBPG,r0,coloop	# reduce count and loop
	brb	coshort

/*
 * {fu,su},{byte,word}, all massaged by asm.sed to jsb's
 */
	.globl	_Fuword
_Fuword:
	prober	$3,$4,(r0)
	beql	fserr
	movl	(r0),r0
	rsb
fserr:
	mnegl	$1,r0
	rsb

	.globl	_Fubyte
_Fubyte:
	prober	$3,$1,(r0)
	beql	fserr
	movzbl	(r0),r0
	rsb

	.globl	_Suword
_Suword:
	probew	$3,$4,(r0)
	beql	fserr
	movl	r1,(r0)
	clrl	r0
	rsb

	.globl	_Subyte
_Subyte:
	probew	$3,$1,(r0)
	beql	fserr
	movb	r1,(r0)
	clrl	r0
	rsb

/*
 * find length of a NUL-terminated string in user space;
 * check access along the way
 * returns -1 if access is bad,
 * otherwise length of string including NUL
 * NB not the same number strlen returns
 */

	.globl	_fustrlen
_fustrlen:			# fustrlen(ustr)
	.word	0x0000
	movl	4(ap),r2
	bsbb	Fustrlen
	ret

/*
 * the real work, split off so can jsb here for speed
 * address of string in r2
 */

Fustrlen:
	movl	r2,r1
	bicl3	$~(NBPG-1),r1,r0	# bytes within page
	subl3	r0,$NBPG,r0	# bytes remaining in page
0:
	prober	$3,r0,(r1)
	beql	fsfault		# sic - if error, ret from whoever called us
	locc	$0,r0,(r1)
	bneq	1f
	movl	$NBPG,r0	# next page
	brb	0b
1:				# r1 contains address of NUL
	subl3	r2,r1,r0
	incl	r0		# byte count
	rsb

/*
 * strncpy, but from user space; returns -1 if error
 * it is an error if the string is too long
 *	fustrncpy(to, from, tolen)
 */
	.globl	_fustrncpy
_fustrncpy:
	.word	0
	movl	8(ap),r2
	bsbb	Fustrlen	# check access and find length
#	tstl	r0		# Fustrlen does ret if error
#	blss	fsfault
	cmpl	r0,12(ap)
	bgtr	fsfault
0:	cmpl	r0,$MOVCMAX
	bgtr	1f		# hard case: too big for one movc
	movc3	r0,*8(ap),*4(ap)
#	clrl	r0		# movc leaves 0 in r0
	ret

1:
	subl3	$MOVCMAX,r0,-(sp)
	movc3	$MOVCMAX,*8(ap),*4(ap)
	movl	r1,8(ap)	# movc leaves new source here
	movl	r3,4(ap)	# movc leaves new dest here
	movl	(sp)+,r0
	jbr	0b

fsfault:
	mnegl	$1,r0
	ret

/*
 * Copy 1 relocation unit (NBPG bytes)
 * from user virtual address to physical address
 */
_copyseg: 	.globl	_copyseg
	.word	0x0
	bisl3	$PG_V|PG_KW,8(ap),*_CMAP2
	mtpr	_CADDR2,$TBIS	# invalidate entry for copy 
	movc3	$NBPG,*4(ap),*_CADDR2
	ret

/*
 * zero out physical memory
 * specified in relocation units (NBPG bytes)
 */
_clearseg: 	.globl	_clearseg
	.word	0x0
	bisl3	$PG_V|PG_KW,4(ap),*_CMAP1
	mtpr	_CADDR1,$TBIS
	movc5	$0,(sp),$0,$NBPG,*_CADDR1
	ret

	.comm	_CMAP1,4
	.comm	_CADDR1,4
	.comm	_CMAP2,4
	.comm	_CADDR2,4

#
# get/put one byte in physical memory
# the original excuse is I/O ECC correction
#
#	phgetc(addr)
#	phputc(addr, byte)
#	both return -1 for error
#
	.comm	_CMAP3,4
	.comm	_CADDR3,4
_phgetc:	.globl	_phgetc
	.word	0
	bsbb	phcomm
	movzbl	(r1),r0
	brb	2f

_phputc:	.globl	_phputc
	.word	0
	bsbb	phcomm
	clrl	r0		# return 0 if ok
	cvtlb	8(ap),(r1)
	brb	2f
1:	 mnegl	$1,r0		# error
2:	movl	r3,_nofault
	mtpr	r2,$IPL
	ret

# r2 -> old IPL, new IPL high
# r3 -> old nofault, new nofault ours
# *CMAP3 -> pte
# r1 -> virtual address of the interesting byte

phcomm:
	mfpr	$IPL,r2
	mtpr	$HIGH,$IPL
	divl3	$NBPG,4(ap),r0	# page number
	bisl3	$PG_V|PG_KW,r0,*_CMAP3
	bicl3	$~(NBPG-1),4(ap),r1	# offset within page
	addl2	_CADDR3,r1
	mtpr	r1,$TBIS
	movl	_nofault,r3
	movab	1b,_nofault
	rsb
