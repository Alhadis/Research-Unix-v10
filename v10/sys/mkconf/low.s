	.text
	.align 9
	.set INTSTACK,1
	.globl	scb
scb:
	.long straycatch+0x0+INTSTACK
	.long Xmachcheck+INTSTACK
	.long Xkspnotval+INTSTACK
	.long straycatch+0xc+INTSTACK
	.long Xprivinflt
	.long Xxfcflt
	.long Xresopflt
	.long Xresadflt
	.long Xprotflt
	.long Xtransflt
	.long Xtracep
	.long Xbptflt
	.long Xcompatflt
	.long Xarithtrap
	.long straycatch+0x38+INTSTACK
	.long straycatch+0x3c+INTSTACK
	.long Xsyscall
	.long Xchmx+INTSTACK
	.long Xchmx+INTSTACK
	.long Xchmx
	.long straycatch+0x50+INTSTACK
	.long Xcmrd+INTSTACK
	.long straycatch+0x58+INTSTACK
	.long straycatch+0x5c+INTSTACK
	.long Xwtime+INTSTACK
	.long straycatch+0x64+INTSTACK
	.long straycatch+0x68+INTSTACK
	.long straycatch+0x6c+INTSTACK
	.long straycatch+0x70+INTSTACK
	.long straycatch+0x74+INTSTACK
	.long straycatch+0x78+INTSTACK
	.long straycatch+0x7c+INTSTACK
	.long straycatch+0x80+INTSTACK
	.long Xshsched
	.long Xastflt
	.long Xqsched
	.long straycatch+0x90+INTSTACK
	.long straycatch+0x94+INTSTACK
	.long straycatch+0x98+INTSTACK
	.long straycatch+0x9c+INTSTACK
	.long Xsoftclock+INTSTACK
	.long straycatch+0xa4+INTSTACK
	.long straycatch+0xa8+INTSTACK
	.long straycatch+0xac+INTSTACK
	.long straycatch+0xb0+INTSTACK
	.long straycatch+0xb4+INTSTACK
	.long straycatch+0xb8+INTSTACK
	.long straycatch+0xbc+INTSTACK
	.long Xhardclock+INTSTACK
	.long straycatch+0xc4+INTSTACK
	.long straycatch+0xc8+INTSTACK
	.long straycatch+0xcc+INTSTACK
	.long straycatch+0xd0+INTSTACK
	.long straycatch+0xd4+INTSTACK
	.long straycatch+0xd8+INTSTACK
	.long straycatch+0xdc+INTSTACK
	.long straycatch+0xe0+INTSTACK
	.long straycatch+0xe4+INTSTACK
	.long straycatch+0xe8+INTSTACK
	.long straycatch+0xec+INTSTACK
	.long Xctu00+INTSTACK
	.long Xctu01+INTSTACK
	.long Xcnrint+INTSTACK
	.long Xcnxint+INTSTACK
	.long straycatch+0x100+INTSTACK
	.long straycatch+0x104+INTSTACK
	.long straycatch+0x108+INTSTACK
	.long Xmba10+INTSTACK
	.long straycatch+0x110+INTSTACK
	.long straycatch+0x114+INTSTACK
	.long straycatch+0x118+INTSTACK
	.long straycatch+0x11c+INTSTACK
	.long Xuba00+INTSTACK
	.long straycatch+0x124+INTSTACK
	.long straycatch+0x128+INTSTACK
	.long straycatch+0x12c+INTSTACK
	.long straycatch+0x130+INTSTACK
	.long straycatch+0x134+INTSTACK
	.long straycatch+0x138+INTSTACK
	.long straycatch+0x13c+INTSTACK
	.long straycatch+0x140+INTSTACK
	.long straycatch+0x144+INTSTACK
	.long straycatch+0x148+INTSTACK
	.long Xmba10+INTSTACK
	.long straycatch+0x150+INTSTACK
	.long straycatch+0x154+INTSTACK
	.long straycatch+0x158+INTSTACK
	.long straycatch+0x15c+INTSTACK
	.long Xuba00+INTSTACK
	.long straycatch+0x164+INTSTACK
	.long straycatch+0x168+INTSTACK
	.long straycatch+0x16c+INTSTACK
	.long straycatch+0x170+INTSTACK
	.long straycatch+0x174+INTSTACK
	.long straycatch+0x178+INTSTACK
	.long straycatch+0x17c+INTSTACK
	.long straycatch+0x180+INTSTACK
	.long straycatch+0x184+INTSTACK
	.long straycatch+0x188+INTSTACK
	.long Xmba10+INTSTACK
	.long straycatch+0x190+INTSTACK
	.long straycatch+0x194+INTSTACK
	.long straycatch+0x198+INTSTACK
	.long straycatch+0x19c+INTSTACK
	.long Xuba00+INTSTACK
	.long straycatch+0x1a4+INTSTACK
	.long straycatch+0x1a8+INTSTACK
	.long straycatch+0x1ac+INTSTACK
	.long straycatch+0x1b0+INTSTACK
	.long straycatch+0x1b4+INTSTACK
	.long straycatch+0x1b8+INTSTACK
	.long straycatch+0x1bc+INTSTACK
	.long straycatch+0x1c0+INTSTACK
	.long straycatch+0x1c4+INTSTACK
	.long straycatch+0x1c8+INTSTACK
	.long Xmba10+INTSTACK
	.long straycatch+0x1d0+INTSTACK
	.long straycatch+0x1d4+INTSTACK
	.long straycatch+0x1d8+INTSTACK
	.long straycatch+0x1dc+INTSTACK
	.long Xuba00+INTSTACK
	.long straycatch+0x1e4+INTSTACK
	.long straycatch+0x1e8+INTSTACK
	.long straycatch+0x1ec+INTSTACK
	.long straycatch+0x1f0+INTSTACK
	.long straycatch+0x1f4+INTSTACK
	.long straycatch+0x1f8+INTSTACK
	.long straycatch+0x1fc+INTSTACK
	.long straycatch+0x200+INTSTACK
	.long straycatch+0x204+INTSTACK
	.long straycatch+0x208+INTSTACK
	.long straycatch+0x20c+INTSTACK
	.long straycatch+0x210+INTSTACK
	.long straycatch+0x214+INTSTACK
	.long straycatch+0x218+INTSTACK
	.long straycatch+0x21c+INTSTACK
	.long straycatch+0x220+INTSTACK
	.long straycatch+0x224+INTSTACK
	.long straycatch+0x228+INTSTACK
	.long straycatch+0x22c+INTSTACK
	.long straycatch+0x230+INTSTACK
	.long straycatch+0x234+INTSTACK
	.long straycatch+0x238+INTSTACK
	.long straycatch+0x23c+INTSTACK
	.long straycatch+0x240+INTSTACK
	.long straycatch+0x244+INTSTACK
	.long straycatch+0x248+INTSTACK
	.long straycatch+0x24c+INTSTACK
	.long straycatch+0x250+INTSTACK
	.long straycatch+0x254+INTSTACK
	.long straycatch+0x258+INTSTACK
	.long straycatch+0x25c+INTSTACK
	.long straycatch+0x260+INTSTACK
	.long straycatch+0x264+INTSTACK
	.long straycatch+0x268+INTSTACK
	.long straycatch+0x26c+INTSTACK
	.long straycatch+0x270+INTSTACK
	.long straycatch+0x274+INTSTACK
	.long straycatch+0x278+INTSTACK
	.long straycatch+0x27c+INTSTACK
	.long straycatch+0x280+INTSTACK
	.long straycatch+0x284+INTSTACK
	.long straycatch+0x288+INTSTACK
	.long straycatch+0x28c+INTSTACK
	.long straycatch+0x290+INTSTACK
	.long straycatch+0x294+INTSTACK
	.long straycatch+0x298+INTSTACK
	.long straycatch+0x29c+INTSTACK
	.long straycatch+0x2a0+INTSTACK
	.long straycatch+0x2a4+INTSTACK
	.long straycatch+0x2a8+INTSTACK
	.long straycatch+0x2ac+INTSTACK
	.long straycatch+0x2b0+INTSTACK
	.long straycatch+0x2b4+INTSTACK
	.long straycatch+0x2b8+INTSTACK
	.long straycatch+0x2bc+INTSTACK
	.long Xdz00+INTSTACK
	.long Xdz01+INTSTACK
	.long Xdz10+INTSTACK
	.long Xdz11+INTSTACK
	.long straycatch+0x2d0+INTSTACK
	.long straycatch+0x2d4+INTSTACK
	.long straycatch+0x2d8+INTSTACK
	.long straycatch+0x2dc+INTSTACK
	.long straycatch+0x2e0+INTSTACK
	.long straycatch+0x2e4+INTSTACK
	.long straycatch+0x2e8+INTSTACK
	.long straycatch+0x2ec+INTSTACK
	.long straycatch+0x2f0+INTSTACK
	.long straycatch+0x2f4+INTSTACK
	.long straycatch+0x2f8+INTSTACK
	.long straycatch+0x2fc+INTSTACK
	.long straycatch+0x300+INTSTACK
	.long straycatch+0x304+INTSTACK
	.long straycatch+0x308+INTSTACK
	.long straycatch+0x30c+INTSTACK
	.long straycatch+0x310+INTSTACK
	.long straycatch+0x314+INTSTACK
	.long straycatch+0x318+INTSTACK
	.long straycatch+0x31c+INTSTACK
	.long straycatch+0x320+INTSTACK
	.long straycatch+0x324+INTSTACK
	.long Xud00+INTSTACK
	.long straycatch+0x32c+INTSTACK
	.long straycatch+0x330+INTSTACK
	.long straycatch+0x334+INTSTACK
	.long straycatch+0x338+INTSTACK
	.long straycatch+0x33c+INTSTACK
	.long straycatch+0x340+INTSTACK
	.long straycatch+0x344+INTSTACK
	.long straycatch+0x348+INTSTACK
	.long straycatch+0x34c+INTSTACK
	.long straycatch+0x350+INTSTACK
	.long straycatch+0x354+INTSTACK
	.long straycatch+0x358+INTSTACK
	.long straycatch+0x35c+INTSTACK
	.long straycatch+0x360+INTSTACK
	.long straycatch+0x364+INTSTACK
	.long straycatch+0x368+INTSTACK
	.long straycatch+0x36c+INTSTACK
	.long straycatch+0x370+INTSTACK
	.long straycatch+0x374+INTSTACK
	.long straycatch+0x378+INTSTACK
	.long straycatch+0x37c+INTSTACK
	.long straycatch+0x380+INTSTACK
	.long straycatch+0x384+INTSTACK
	.long straycatch+0x388+INTSTACK
	.long straycatch+0x38c+INTSTACK
	.long straycatch+0x390+INTSTACK
	.long straycatch+0x394+INTSTACK
	.long straycatch+0x398+INTSTACK
	.long straycatch+0x39c+INTSTACK
	.long straycatch+0x3a0+INTSTACK
	.long straycatch+0x3a4+INTSTACK
	.long straycatch+0x3a8+INTSTACK
	.long straycatch+0x3ac+INTSTACK
	.long straycatch+0x3b0+INTSTACK
	.long straycatch+0x3b4+INTSTACK
	.long straycatch+0x3b8+INTSTACK
	.long straycatch+0x3bc+INTSTACK
	.long straycatch+0x3c0+INTSTACK
	.long straycatch+0x3c4+INTSTACK
	.long straycatch+0x3c8+INTSTACK
	.long straycatch+0x3cc+INTSTACK
	.long straycatch+0x3d0+INTSTACK
	.long straycatch+0x3d4+INTSTACK
	.long straycatch+0x3d8+INTSTACK
	.long straycatch+0x3dc+INTSTACK
	.long straycatch+0x3e0+INTSTACK
	.long straycatch+0x3e4+INTSTACK
	.long straycatch+0x3e8+INTSTACK
	.long straycatch+0x3ec+INTSTACK
	.long straycatch+0x3f0+INTSTACK
	.long straycatch+0x3f4+INTSTACK
	.long straycatch+0x3f8+INTSTACK
	.long straycatch+0x3fc+INTSTACK
	.globl	straycatch
straycatch:
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
	.align 2; bsbw stray
stray:	jmp Xstray
	.align 2
.globl _uba0int
Xuba00:
	pushr	$0x3f
	clrl	-(sp)
	calls	$1,_uba0int
	popr	$0x3f
	rei
	.align 2
.globl _ud0int
Xud00:
	pushr	$0x3f
	clrl	-(sp)
	calls	$1,_ud0int
	popr	$0x3f
	rei
	.align 2
.globl _dz0int
Xdz00:
	pushr	$0x3f
	clrl	-(sp)
	calls	$1,_dz0int
	popr	$0x3f
	rei
	.align 2
.globl _dz1int
Xdz01:
	pushr	$0x3f
	clrl	-(sp)
	calls	$1,_dz1int
	popr	$0x3f
	rei
	.align 2
.globl _dz0int
Xdz10:
	pushr	$0x3f
	pushl	$1
	calls	$1,_dz0int
	popr	$0x3f
	rei
	.align 2
.globl _dz1int
Xdz11:
	pushr	$0x3f
	pushl	$1
	calls	$1,_dz1int
	popr	$0x3f
	rei
	.align 2
.globl _mba0int
Xmba10:
	pushr	$0x3f
	pushl	$1
	calls	$1,_mba0int
	popr	$0x3f
	rei
	.align 2
.globl _ctu0int
Xctu00:
	pushr	$0x3f
	clrl	-(sp)
	calls	$1,_ctu0int
	popr	$0x3f
	rei
	.align 2
.globl _ctu1int
Xctu01:
	pushr	$0x3f
	clrl	-(sp)
	calls	$1,_ctu1int
	popr	$0x3f
	rei
