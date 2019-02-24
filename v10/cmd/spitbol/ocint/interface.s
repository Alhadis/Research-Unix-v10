	.globl	sysax
	.globl	sysbx
	.globl	sysdc
	.globl	sysdm
	.globl	sysdt
#	.globl	sysef
#	.globl	sysej
	.globl	sysem
#	.globl	sysen
	.globl	sysep
	.globl	sysex
#	.globl	sysfc
#	.globl	syshs
#	.globl	sysid
#	.globl	sysil
#	.globl	sysin
#	.globl	sysio
	.globl	sysld
#	.globl	sysmm
#	.globl	sysmx
	.globl	sysou
#	.globl	syspi
#	.globl	syspp
#	.globl	syspr
#	.globl	sysrd
#	.globl	sysri
	.globl	sysrw
#	.globl	sysst
#	.globl	systm
	.globl	systt
	.globl	sysul
#	.globl	sysxi

	.data
	.globl	_reg_ra
	.globl	_reg_cp
	.globl	_reg_ia
	.globl	_reg_wa
	.globl	_reg_wb
	.globl	_reg_wc
	.globl	_reg_xr
	.globl	_reg_xl

	.align	2
_reg_ra:	.space	4
_reg_cp:	.space	4
_reg_ia:	.space	4
_reg_wa:	.space	4
_reg_wb:	.space	4
_reg_wc:	.space	4
_reg_xr:	.space	4
_reg_xl:	.space	4

	.text

sysax:	jsb	ccall
	.long	_sysax

sysbx:	jsb	ccall
	.long	_sysbx

sysdc:	jsb	ccall
	.long	_sysdc

sysdm:	jsb	ccall
	.long	_sysdm

sysdt:	jsb	ccall
	.long	_sysdt

#sysef:	jsb	ccall
#	.long	_sysef

#sysej:	jsb	ccall
#	.long	_sysej

sysem:	jsb	ccall
	.long	_sysem

#sysen:	jsb	ccall
#	.long	_sysen

sysep:	jsb	ccall
	.long	_sysep

sysex:	jsb	ccall
	.long	_sysex

#sysfc:	jsb	ccall
#	.long	_sysfc

#syshs:	jsb	ccall
#	.long	_syshs

#sysid:	jsb	ccall
#	.long	_sysid

#sysil:	jsb	ccall
#	.long	_sysil

#sysin:	jsb	ccall
#	.long	_sysin

#sysio:	jsb	ccall
#	.long	_sysio

sysld:	jsb	ccall
	.long	_sysld

#sysmm:	jsb	ccall
#	.long	_sysmm

#sysmx:	jsb	ccall
#	.long	_sysmx

sysou:	jsb	ccall
	.long	_sysou

#syspi:	jsb	ccall
#	.long	_syspi

#syspp:	jsb	ccall
#	.long	_syspp

#syspr:	jsb	ccall
#	.long	_syspr

#sysrd:	jsb	ccall
#	.long	_sysrd

#sysri:	jsb	ccall
#	.long	_sysri

sysrw:	jsb	ccall
	.long	_sysrw

#sysst:	jsb	ccall
#	.long	_sysst

#systm:	jsb	ccall
#	.long	_systm

systt:	jsb	ccall
	.long	_systt

sysul:	jsb	ccall
	.long	_sysul

#sysxi:	jsb	ccall
#	.long	_sysxi

ccall:	movl	r2,_reg_ra
	movl	r3,_reg_cp
	movl	r5,_reg_ia
	movl	r6,_reg_wa
	movl	r7,_reg_wb
	movl	r8,_reg_wc
	movl	r9,_reg_xr
	movl	r10,_reg_xl
	movl	(sp)+,r0
	movl	(r0),r0
	calls	$0,(r0)
	ashl	$2,r0,r0
	addl2	r0,(sp)
	movl	_reg_ra,r2
	movl	_reg_cp,r3
	movl	_reg_ia,r5
	movl	_reg_wa,r6
	movl	_reg_wb,r7
	movl	_reg_wc,r8
	movl	_reg_xr,r9
	movl	_reg_xl,r10
	rsb
