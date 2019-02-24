#
#	VAXINTER.S
#
#	This file contains the assembly language routines that interface
#	the Macro SPITBOL compiler written in VAX assembly language to its
#	operating system interface functions written in C.
#
#	Contents:
#
#	o Overview
#	o Global variables accessed by OSINT functions
#	o Interface routines between compiler and OSINT functions
#	o C callable function get_sp
#	o C callable function startup
#	o Compiler callable function sbchk
#
#-----------
#
#	Overview
#
#	The Macro SPITBOL compiler relies on a set of operating system
#	interface functions to provide all interaction with the host
#	operating system.  These functions are referred to as OSINT
#	functions.  A typical call to one of these OSINT functions takes
#	the following form in the VAX version of the compiler:
#
#		...code to put arguments in registers...
#		jsb	sysxx		; call osint function
#		.long	exit_1		; address of exit point 1
#		.long	exit_2		; address of exit point 2
#		...	...		; ...
#		.long	exit_n		; address of exit point n
#		...instruction following call...
#
#	The OSINT function 'sysxx' can then return in one of n+1 ways:
#	to one of the n exit points or to the instruction following the
#	last exit.  This is not really very complicated - the jsb places
#	the return address on the stack, so all the interface function has
#	to do is add the appropriate offset to the return address and then
#	pick up the exit address and jump to it OR do a normal return via
#	an rsb instruction.
#
#	Unfortunately, a C function cannot handle this scheme.  So, an
#	intermediary set of routines have been established to allow the
#	interfacing of C functions.  The mechanism is as follows:
#
#	(1) The compiler calls OSINT functions as described above.
#
#	(2) A set of assembly language interface routines is established,
#	    one per OSINT function, named accordingly.  Each interface
#	    routine ...
#
#	    (a) saves all compiler registers in global variables
#	        accessible by C functions
#	    (b) calls the OSINT function written in C
#	    (c) restores all compiler registers from the global variables
#	    (d) inspects the OSINT function's return value to determine
#	        which of the n+1 returns should be taken and does so
#
#	(3) A set of C language OSINT functions is established, one per
#	    OSINT function, named differently than the interface routines.
#	    Each OSINT function can access compiler registers via global
#	    variables.  NO arguments are passed via the call.
#
#	    When an OSINT function returns, it must return a value indicating
#	    which of the n+1 exits should be taken.  These return values are
#	    defined in header file 'inter.h'.
#
#	Note:  in the actual implementation below, the saving and restoring
#	of registers is actually done in one common routine accessed by all
#	interface routines.
#
#	Other notes:
#
#	The Un*x VAX cC ompilers transform "internal" global names to
#	"external" global names by adding a leading underscore at the front
#	of the internal name.  Thus, the function name 'osopen' becomes
#	'_osopen'.
#
#	Acknowledgement:
#
#	This interfacing scheme is based on an idea put forth by Andy Koenig.
#
#-----------
#
#	Global Variables
#
#	The compiler uses 8 registers as described below.
#
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
_reg_ra:	.space	4	# Register RA (R2)
_reg_cp:	.space	4	# Register CP (R3)
_reg_ia:	.space	4	# Register IA (R5)
_reg_wa:	.space	4	# Register WA (R6)
_reg_wb:	.space	4	# Register WB (R7)
_reg_wc:	.space	4	# Register WC (R8)
_reg_xr:	.space	4	# Register XR (R9)
_reg_xl:	.space	4	# Register XL (R10)
#
	.globl	_rsboff
_rsboff:	.space	4	# Normal rsb return offset
#
#	The following pointers address those cells in the compiler
#	that point into the stack when a load module might be written,
#	and which must therefore be relocated.
	.globl	_strellst
_strellst:
	.long	flptr
	.long	stbas
	.long	gtcef
	.long	0		# end of list marker
#
#	Setup a number of internal addresses in the compiler that cannot
#	be directly accessed from within C because of naming difficulties.
#
	.globl	_ic_type
_ic_type:	.long	b$icl
	.globl	_sc_type
_sc_type:	.long	b$scl
	.globl	_xn_type
_xn_type:	.long	b$xnt
	.globl	_xr_type
_xr_type:	.long	b$xrt
#
	.globl	_id1
_id1:	.long	0,id1l
	.ascii	"(2.0)"
id1e:
	.set	id1l,id1e-_id1-8
	.align	2
#
	.globl	_id2
_id2:	.long	0,id2l
	.ascii	"VAX/UNIX Version"
id2e:
	.set	id2l,id2e-_id2-8
	.align	2
#
	.globl  _tscblk
	.globl  _ticblk
_ticblk: .space 8
_tscblk: .space 108
#
#-----------
#
#	Interface routines
#
#	Each interface routine takes the following form:
#
#		sysxx:	jsb	ccaller		# call common interface
#			.long	_sysxx		# address of C OSINT function
#			.long	n		# offset to instruction after
#						#   last procedure exit
#
#	We take advantage of the "internal" to "external" transformation
#	of names by the C compiler.  Specifically, the assembly language
#	interface routine names match those directly called by the compiler:
#	'sysxx'.  The corresponding C OSINT functions are also called 'sysxx',
#	but the C compiler nicely transforms these names into '_sysxx'.
#	
	.text
	.globl	sysax
sysax:	jsb	ccaller
	.long	_sysax
	.long	0
#
	.globl	sysbx
sysbx:	jsb	ccaller
	.long	_sysbx
	.long	0
#
	.globl	sysdc
sysdc:	jsb	ccaller
	.long	_sysdc
	.long	0
#
	.globl	sysdm
sysdm:	jsb	ccaller
	.long	_sysdm
	.long	0
#
	.globl	sysdt
sysdt:	jsb	ccaller
	.long	_sysdt
	.long	0
#
	.globl	sysef
sysef:	jsb	ccaller
	.long	_sysef
	.long	12
#
#	.globl	sysej
#sysej:	jsb	ccaller
#	.long	_sysej
#	.long	0
#
	.globl	sysem
sysem:	jsb	ccaller
	.long	_sysem
	.long	0
#
	.globl	sysen
sysen:	jsb	ccaller
	.long	_sysen
	.long	12
#
	.globl	sysep
sysep:	jsb	ccaller
	.long	_sysep
	.long	0
#
	.globl	sysex
sysex:	jsb	ccaller
	.long	_sysex
	.long	4
#
#	.globl	sysfc
#sysfc:	jsb	ccaller
#	.long	_sysfc
#	.long	4
#
	.globl	syshs
syshs:	jsb	ccaller
	.long	_syshs
	.long	24
#
	.globl	sysid
sysid:	jsb	ccaller
	.long	_sysid
	.long	0
#
#	.globl	sysil
#sysil:	jsb	ccaller
#	.long	_sysil
#	.long	0
#
#	.globl	sysin
#sysin:	jsb	ccaller
#	.long	_sysin
#	.long	12
#
#	.globl	sysio
#sysio:	jsb	ccaller
#	.long	_sysio
#
	.globl	sysld
sysld:	jsb	ccaller
	.long	_sysld
	.long	8
#
	.globl	sysmm
sysmm:	jsb	ccaller
	.long	_sysmm
	.long	0
#
	.globl	sysmx
sysmx:	jsb	ccaller
	.long	_sysmx
	.long	0
#
	.globl	sysou
sysou:	jsb	ccaller
	.long	_sysou
	.long	8
#
#	.globl	syspi
#syspi:	jsb	ccaller
#	.long	_syspi
#	.long	0
#
	.globl	syspp
syspp:	jsb	ccaller
	.long	_syspp
	.long	0
#
#	.globl	syspr
#syspr:	jsb	ccaller
#	.long	_syspr
#	.long	4
#
#	.globl	sysrd
#sysrd:	jsb	ccaller
#	.long	_sysrd
#	.long	4
#
#	.globl	sysri
#sysri:	jsb	ccaller
#	.long	_sysri
#	.long	4
#
	.globl	sysrw
sysrw:	jsb	ccaller
	.long	_sysrw
	.long	12
#
#	.globl	sysst
#sysst:	jsb	ccaller
#	.long	_sysst
#	.long	20
#
#	.globl	systm
#systm:	jsb	ccaller
#	.long	_systm
#	.long	0
#
	.globl	systt
systt:	jsb	ccaller
	.long	_systt
	.long	0
#
	.globl	sysul
sysul:	jsb	ccaller
	.long	_sysul
	.long	0
#
#	.globl	sysxi
#sysxi:	jsb	ccaller
#	.long	_sysxi
#	.long	8
#
#-----------
#
#	CCALLER is called by the OS interface routines to call the
#	real C OS interface function.
#
#	General calling sequence is:
#
#		jsb	ccaller
#		.long	address_of_C_function
#		.long	4*number_of_exit_points
#
#	Control IS NEVER returned to a interface routine.  Instead, control
#	is returned to the compiler (THE caller of the interface routine).
#
#	The C function that is called MUST ALWAYS return an integer
#	indicating the proecedure exit to take or that a normal return
#	is to be performed.
#
#		C function      Interpretation
#		return value
#		------------	-------------------------------------------
#		     <0		Do normal return to instruction past
#				last procedure exit (distance passed
#				in by dummy routine and saved in _rsboff)
#		      0		Take procedure exit 1
#		      4		Take procedure exit 2
#		      8		Take procedure exit 3
#		     ...	...
#	
ccaller:
#
#	(1) Save registers in global variables
#
	movl	r2,_reg_ra	# save RA (R2)
	movl	r3,_reg_cp	# save CP (R3)
	movl	r5,_reg_ia	# save IA (R5)
	movl	r6,_reg_wa	# save WA (R6)
	movl	r7,_reg_wb	# save WB (R7)
	movl	r8,_reg_wc	# save WC (R8)
	movl	r9,_reg_xr	# save XR (R9)
	movl	r10,_reg_xl	# save XL (R10)
#
#	(2) Fetch address of C function, fetch offset to 1st instruction
#	    past last procedure exit, and call C function.
#
	movl	(sp)+,r0	# point to arg list
	movl	(r0)+,r1	# point to C function entry point
	movl	(r0),_rsboff	# get adjustment for normal exit
	calls	$0,(r1)		# call C interface function
#
#	(3) Restore registers after C function returns.
#
	movl	_reg_ra,r2	# restore RA (R2)
	movl	_reg_cp,r3	# restore CP (R3)
	movl	_reg_ia,r5	# restore IA (R5)
	movl	_reg_wa,r6	# restore WA (R6)
	movl	_reg_wb,r7	# restore WB (R7)
	movl	_reg_wc,r8	# restore WC (R8)
	movl	_reg_xr,r9	# restore XR (R9)
	movl	_reg_xl,r10	# restore XL (R10)
#
#	(4) Based on returned value from C function either do a normal
#	    return or take a procedure exit.
#
	tstl	r0		# if normal return ...
	bgeq	erexit
	addl2	_rsboff,(sp)	#   point to instruction following exits
	rsb			#   return
#				# else (take procedure exit n)
erexit:	addl3	r0,(sp)+,r11	#   point to address of exit
	jmp	*(r11)+		#   take procedure exit
#
#-----------
#
#	_get_sp	- get C caller's SP
#
#	_get_sp() returns the current value of the stack pointer from
#	the point of reference of a C function.
#
#	From C this procedure is called by
#
#		our_sp = get_sp();
#
#	The generated code will look like
#
#		calls	#0,_get_sp
#		movl	r0,our_sp
#
#	On entry to _get_sp, the stack looks like
#
#		+---------------+  (<-- SP before 'calls' executed)
#		|  arg cnt (0)	|
#		|---------------|  <-- AP
#		|    old PC	|
#		|---------------|
#		|    old FP	|
#		|---------------|
#		|    old AP	|
#		|---------------|
#		|   mask/PSW	|
#		|---------------|
#		|      0	|
#		+---------------+  <-- SP  <-- FP
#
#	On exit, all these words will be removed from the stack.  Therefore,
#	the current value of the SP from the caller's perspective is AP+4.
#
	.text
	.globl	_get_sp
_get_sp:
	.word	0		# no registers to save
	addl3	$4,ap,r0	# compute caller's SP
	ret			# done
#
#-----------
#
#	_startup - startup compiler
#
#	An OSINT C function calls _startup to transfer control
#	to the compiler.
#
	.text
	.globl	_startup
_startup:
	.word	0
	movl	_reg_ra,r2	# initialize RA (R2)
	movl	_reg_cp,r3	# initialize CP (R3)
	movl	_reg_ia,r5	# initialize IA (R5)
	movl	_reg_wa,r6	# initialize WA (R6)
	movl	_reg_wb,r7	# initialize WB (R7)
	movl	_reg_wc,r8	# initialize WC (R8)
	movl	_reg_xr,r9	# initialize XR (R9)
	movl	_reg_xl,r10	# initialize XL (R10)
#
	movl	sp,_initsp	# save initial sp
	subl3	_stacksiz,sp,_lowsp
				# set lowest legal sp for sbchk
#
	movl	_dfltcase,kvcas	# set case flag IN compiler
#
	jmp	sec04		# transfer control to compiler
#
#-----------
#
#	sbchk - check for stack overflow
#
	.text
	.globl	sbchk
sbchk:	cmpl	sp,_lowsp	# if sp is ok then
	blssu	sb100
	rsb			#    return
sb100:	tstl	(sp)+		# else pop stack
	jmp	sec05		#    and go to stack overflow section
