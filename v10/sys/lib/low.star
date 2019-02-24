#
# template for system control block for star (11/780)
#
0x4	i	Xmachcheck	# machine check interrupt
0x8	i	Xkspnotval	# kernel stack not valid
# 0xc	i	Xpowfail	# power fail
0x10	t	Xprivinflt	# privileged instruction
0x14	t	Xxfcflt		# xfc instruction
0x18	t	Xresopflt	# reserved operand
0x1c	t	Xresadflt	# reserved addressing
0x20	t	Xprotflt	# protection and pt length violation
0x24	t	Xtransflt	# address translation not valid fault
0x28	t	Xtracep		# trace pending
0x2c	t	Xbptflt		# bpt instruction
0x30	t	Xcompatflt	# compatibility mode fault
0x34	t	Xarithtrap	# arithmetic trap
0x40	t	Xsyscall	# chmk
0x44	i	Xchmx		# chme
0x48	i	Xchmx		# chms
0x4c	t	Xchmx		# chmu
# 0x50				# sbi silo compare; VMS ignores it too
0x54	i	Xmemerr		# memory CRD or RDS
0x58	i	Xmemerr		# SBI alert; assume memory controller
0x5c	i	Xsbiflt		# SBI fault
0x60	i	Xwtime		# async write timeout
0x88	t	Xastflt		# software 2: reschedule nudge, profile
0x8c	t	Xqsched		# software 3: stream queue scheduling
0xa0	i	Xsoftclock	# software 8: soft clock	
0xc0	i	Xhardclock	# clock
0xf8	i	Xcnrint		# console receiver
0xfc	i	Xcnxint		# console transmitter
