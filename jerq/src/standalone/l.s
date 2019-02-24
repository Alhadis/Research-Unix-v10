# Stand-Alone Startup routine
# Linked with most stand-alone programs
#
# Modified to use ROM vector table by MJK 2/84
#
# WARNING: This file contains ABSOLUTE REFERENCES
#	   to jump table addresses (as in	CALL *522),
#	   so it fixes these entries in the jump table.  Changing
#	   these will obsolete all compiled stand-alone programs.
#
#
	.text
	.def	_start;	.val	_start;	.scl	2;	.type	044;	.endef
	.globl	_start
_start:
					# First, patch up things with ROM-dependent
					# stuff.
	MOVW	640, proc_pcb+4		# Fill in process exception PCB 
	MOVW	644, stack_pcb+4	# and stack exception PCB (reset is fixed) 
	MOVAW	excep_tab, %r1		# r1=base of exception table (+4)
	MOVB	&16, %r2		# 16 entries in normal excep tab
	MOVW	636, %r3		# filled in with excep_norm
back:	MOVW	%r3, 0(%r1)		# copy, copy, copy...
	ADDW2	&8, %r1			# point to next entry
	DECB	%r2			# done?
	BNEB	back
#	call	&0,jinit
	call	&0,main
	RETPS
	.def	_start;	.val	.;	.scl	-1;	.endef
	.globl	ticks0
	.bss	ticks0,4,4
	.text


	.def	key_int;	.val	key_int;	.scl	2;	.type	044;	.endef
	.globl	key_int
key_int:
	call	&0,*548			# auto2
	RETPS
	.def	key_int;	.val	.;	.scl	-1;	.endef

	.def	msvid_int;	.val	msvid_int;	.scl	2;	.type	044;	.endef
	.globl	msvid_int
msvid_int:
	MOVB	0x200013,%r5
	BITB	&0x40,%r5
	BEB	mous_chk
	BITB	&0x04,%r5		# update at 60 hz.
	BNEB	mous_chk	# if fall through, have	a video	interrupt
	call	&0,*544		# (auto1) calls	cursor update in cursor.c
	call	&0,introutine	# calls	user provided introutine
mous_chk:
	BITB	&0xb0,%r5
	BEB	msvid_exit
				# if fall through, have	a mouse	interrupt
	call	&0,*552		# (auto4) call buttons handler in buttons.c
msvid_exit:
	RETPS

	.def	msvid_int;	.val	.;	.scl	-1;	.endef

	.def	host_int;	.val	host_int;	.scl	2;	.type	044;	.endef
	.globl	host_int
host_int:
	pushw	$1096		# pointer to queues
	andw3	0x20000c,&255,%r0
	pushw	%r0
	call	&2,*832		# qputc
	RETPS
	.def	host_int;	.val	.;	.scl	-1;	.endef

	.def	out_int;	.val	out_int;	.scl	2;	.type	044;	.endef
	.globl	out_int
out_int:
	call	&0,*524		# aciatrin
	RETPS
	.def	out_int;	.val	.;	.scl	-1;	.endef
	.def	excep_int;	.val	excep_int;	.scl	2;	.type	044;	.endef
excep_int:
	call	&0,*632		# excep_int
	.def	excep_int;	.val	.;	.scl	-1;	.endef


	.def	realtime;	.val	realtime;	.scl	2;	.type	044;	.endef
	.globl	realtime
realtime:
	movw	ticks0,%r0
	RET
	.def	realtime;	.val	.;	.scl	-1;	.endef
