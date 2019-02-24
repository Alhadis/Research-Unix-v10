# C library -- reboot

# error = reboot(how);

	.set	reboot,55
.globl	_reboot

_reboot:
	.word	0x0000
	chmk	$reboot
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
