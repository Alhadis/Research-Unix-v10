	.file	"pcbs.c"
	.data
	.def	istack;	.val	istack;	.scl	2;	.type	064;	.dim	64;	.size	256;	.endef
	.comm	istack,256
	.def	ststk;	.val	ststk;	.scl	2;	.type	064;	.dim	256;	.size	1024;	.endef
	.comm	ststk,1024
	.def	procstk;	.val	procstk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	procstk,400
	.def	stackstk;	.val	stackstk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	stackstk,400
	.def	hoststk;	.val	hoststk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	hoststk,400
	.def	outstk;	.val	outstk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	outstk,400
	.def	piohstk;	.val	piohstk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	piohstk,400
	.def	keystk;	.val	keystk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	keystk,400
	.def	msvidstk;	.val	msvidstk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	msvidstk,400
	.def	piolstk;	.val	piolstk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	piolstk,400
	.def	strstk;	.val	strstk;	.scl	2;	.type	064;	.dim	100;	.size	400;	.endef
	.comm	strstk,400
	.def	mainstk;	.val	mainstk;	.scl	2;	.type	064;	.dim	1024;	.size	4096;	.endef
	.comm	mainstk,4096
	.align	4
	.def	proc_pcb;	.val	proc_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	proc_pcb
proc_pcb:
	.word	42066304
	.word	excep_proc
	.word	procstk
	.word	42066176
	.word	excep_proc
	.word	procstk
	.word	procstk
	.word	procstk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	stack_pcb;	.val	stack_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	stack_pcb
stack_pcb:
	.word	42066304
	.word	excep_stack
	.word	stackstk
	.word	42066176
	.word	excep_stack
	.word	stackstk
	.word	stackstk
	.word	stackstk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	host_pcb;	.val	host_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	host_pcb
host_pcb:
	.word	42066304
	.word	host_int
	.word	hoststk
	.word	42066176
	.word	host_int
	.word	hoststk
	.word	hoststk
	.word	hoststk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	out_pcb;	.val	out_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	out_pcb
out_pcb:
	.word	42066304
	.word	out_int
	.word	outstk
	.word	42066176
	.word	out_int
	.word	outstk
	.word	outstk
	.word	outstk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	pioh_pcb;	.val	pioh_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	pioh_pcb
pioh_pcb:
	.word	42066304
	.word	pioh_int
	.word	piohstk
	.word	42066176
	.word	pioh_int
	.word	piohstk
	.word	piohstk
	.word	piohstk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	key_pcb;	.val	key_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	key_pcb
key_pcb:
	.word	42066304
	.word	key_int
	.word	keystk
	.word	42066176
	.word	key_int
	.word	keystk
	.word	keystk
	.word	keystk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	msvid_pcb;	.val	msvid_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	msvid_pcb
msvid_pcb:
	.word	42058112
	.word	msvid_int
	.word	msvidstk
	.word	42057984
	.word	msvid_int
	.word	msvidstk
	.word	msvidstk
	.word	msvidstk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	piol_pcb;	.val	piol_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	piol_pcb
piol_pcb:
	.word	42066304
	.word	piol_int
	.word	piolstk
	.word	42066176
	.word	piol_int
	.word	piolstk
	.word	piolstk
	.word	piolstk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	int_pcb;	.val	int_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	int_pcb
int_pcb:
	.word	42066304
	.word	excep_int
	.word	strstk
	.word	42066176
	.word	excep_int
	.word	strstk
	.word	strstk
	.word	strstk+396
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	4
	.def	excep_tab;	.val	excep_tab;	.scl	2;	.type	064;	.dim	32;	.size	128;	.endef
	.globl	excep_tab
excep_tab:
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.word	42066176
	.word	excep_norm
	.align	4
	.def	main_pcb;	.val	main_pcb;	.scl	2;	.type	064;	.dim	20;	.size	80;	.endef
	.globl	main_pcb
main_pcb:
	.word	42066304
	.word	_start
	.word	mainstk
	.word	42066176
	.word	_start
	.word	mainstk
	.word	mainstk
	.word	mainstk+4092
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
