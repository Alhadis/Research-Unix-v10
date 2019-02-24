#
#	address of the vector table in firmware
#
	.set	Firm,	0x200
	.globl	Firm

#
#	dummy entry for defont
#
	.set	defont,	0
	.globl	defont

#
#	hooks into firmware
#
		.text
	.globl	checkbram
checkbram:
	jmp	*(Firm+(4*18))

	.globl	setbram
setbram:
	jmp	*(Firm+(4*101))

	.globl	exit
	.globl	reboot
exit:
reboot:
	jmp	*(Firm+(4*87))

	.globl	ringbell
ringbell:
	jmp	*(Firm+(4*92))

	.globl	spl0
spl0:
	jmp	*(Firm+(4*103))

	.globl	spl1
spl1:
	jmp	*(Firm+(4*104))

	.globl	spl2
spl2:
	jmp	*(Firm+(4*105))

	.globl	spl3
spl3:
	jmp	*(Firm+(4*106))

	.globl	spl4
spl4:
	jmp	*(Firm+(4*107))

	.globl	spl5
spl5:
	jmp	*(Firm+(4*108))

	.globl	spl6
spl6:
	jmp	*(Firm+(4*109))

	.globl	spl7
spl7:
	jmp	*(Firm+(4*110))

	.globl	splx
splx:
	jmp	*(Firm+(4*111))
