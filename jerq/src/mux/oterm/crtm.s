	.data
	.set	Sysval,0x0071e700
	.text
	.def	Point;	.scl	10;	.type	010;	.size	4;	.endef
	.def	x;	.val	0;	.scl	8;	.type	03;	.endef
	.def	y;	.val	2;	.scl	8;	.type	03;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	Point;	.size	4;	.endef
	.data
	.def	Point;	.scl	13;	.type	010;	.tag	Point;	.size	4;	.endef
	.text
	.def	Rectangl;	.scl	10;	.type	010;	.size	8;	.endef
	.def	origin;	.val	0;	.scl	8;	.type	010;	.tag	Point;	.size	4;	.endef
	.def	corner;	.val	4;	.scl	8;	.type	010;	.tag	Point;	.size	4;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	Rectangl;	.size	8;	.endef
	.data
	.def	Rectangl;	.scl	13;	.type	010;	.tag	Rectangl;	.size	8;	.endef
	.text
	.def	Bitmap;	.scl	10;	.type	010;	.size	20;	.endef
	.def	base;	.val	0;	.scl	8;	.type	024;	.endef
	.def	width;	.val	4;	.scl	8;	.type	016;	.endef
	.def	rect;	.val	8;	.scl	8;	.type	010;	.tag	Rectangl;	.size	8;	.endef
	.def	_null;	.val	16;	.scl	8;	.type	022;	.endef
	.def	.eos;	.val	20;	.scl	102;	.tag	Bitmap;	.size	20;	.endef
	.data
	.def	Bitmap;	.scl	13;	.type	010;	.tag	Bitmap;	.size	20;	.endef
	.text
	.def	Mouse;	.scl	10;	.type	010;	.size	8;	.endef
	.def	xy;	.val	0;	.scl	8;	.type	010;	.tag	Point;	.size	4;	.endef
	.def	buttons;	.val	8;	.scl	8;	.type	03;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	Mouse;	.size	12;	.endef
	.data
	.def	mouse;	.val	mouse;	.scl	2;	.type	010;	.tag	Mouse;	.size	12;	.endef
	.def	Drect;	.val	Drect;	.scl	2;	.type	010;	.tag	Rectangl;	.size	8;	.endef
	.def	Jdisplayp;	.val	Jdisplayp;	.scl	2;	.type	030;	.tag	Bitmap;	.size	20;	.endef

	.globl	Drect
Drect:	.word	16:0, 16:0			# Drect.origin
	.word	16:0, 16:0			# Drect.corner

	.globl	Jdisplayp
Jdisplayp:	.word 0

argc:	.word 0					# not global

argv:	.word 0					# not global

	.globl	mouse
mouse:	.word	16:0, 16:0			# mouse.jxy
	.word	16:0, 16:0			# mouse.xy
	.half	0				# mouse.buttons

	.text
	.globl	_start
_start:	SAVE	%r3			# easy way to init save	area on	stack
	movaw	Sysval,%r8			# base of the routine pointers
	PUSHW	argc			# push args to main
	PUSHW	argv			#
	call	&2,main			# main(argc,argv);
	call	&0,*(17*4)(%r8)		# Uexit(); execs windowstart
#	RESTORE	%r3			# only if joff needs it
#	RET				# same here
