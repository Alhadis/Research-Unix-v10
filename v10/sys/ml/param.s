#
# parameters for assembly-language stuff
#

	.set	SAVREG,0x3f	# mask of registers to save on traps
	.set	NRS,6		# number of registers saved
	.set	NISP,3		# number of pages of interrupt stack

	.set	KA,0x80000000	# base of system address space

	.set	MAXPHMEM,1024*1024*1024	# largest memory we can cope with

#
# hardware definitions
# keep these as few as possible;
# write in C instead.
#

#
# the hardware part of the PCB
#
	.set	PCB_KSP,0
	.set	PCB_ESP,4
	.set	PCB_SSP,8
	.set	PCB_USP,12
			# r0-r13
	.set	PCB_PC,72
	.set	PCB_PSL,76
	.set	PCB_P0BR,80
	.set	PCB_P0LR,84
	.set	PCB_P1BR,88
	.set	PCB_P1LR,92
#
# one software extension word; a botch to be removed
#
	.set	PCB_SSWAP,96

#
# psl
#

	.set	PSL_PRVMOD,0x00c00000	# previous mode; all on == user
	.set	PSL_CURMOD,0x03000000	# current mode; ditto
	.set	PSL_FPD,0x08000000	# first part done before trap
	.set	PSLB_IS,26		# bit number: on interrupt stack

#
# processor registers
#
	.set	USP,0x3		# user stack pointer
	.set	P0BR,0x8	# page table base, length
	.set	P0LR,0x9
	.set	P1BR,0xa
	.set	P1LR,0xb
	.set	SBR,0xc
	.set	SLR,0xd
	.set	PCBB,0x10	# current process pcb
	.set	SCBB,0x11	# system control block
	.set	IPL,0x12	# interrupt priority level
	.set	ICCS,0x18	# interval counter control
	.set	NICR,0x19	# next interval count
	.set	RXCS,0x20	# console rcv status, data
	.set	RXDB,0x21
	.set	TXCS,0x22	# console xmt status, data
	.set	TXDB,0x23
	.set	MAPEN,0x38	# enable memory mapping
	.set	TBIA,0x39	# invalidate translation buffer cache
	.set	TBIS,0x3a
	.set	CPUSID,0x3e	# system ID register

#
# hardware memory management
#
	.set	NBPG,512	# bytes per hardware page
	.set	PGSHIFT,9	# a/NBPG == a>>PGSHIFT
	.set	PG_V,0x80000000	# page table entry: valid bit
	.set	PG_KW,0x10000000 # page prot: kernel write
	.set	PG_URKW,0x70000000 # page prot: kernel write user read

#
# miscellany
#
	.set	HIGH,0x1f	# highest interrupt level
	.set	DUMPMAGIC,0x79646e61

#
# software parameters duplicated from C code
# their presence is a bug; fix later
#

	.set	UPAGES,10
	.set	P_LINK,0
	.set	P_RLINK,4
	.set	P_ADDR,8
	.set	P_PRI,13
	.set	P_STAT,15
	.set	P_FLAG,32
	.set	SRUN,3
	.set	SPROCIO,16777216
