/*	@(#)prof.h	1.5	*/
#ifndef MARK
#define MARK(L)	{}
#else
#undef MARK
#ifdef vax
#define MARK(L)	{\
		asm("	.data");\
		asm("	.align	4");\
		asm(".L.:");\
		asm("	.long	0");\
		asm("	.text");\
		asm("M.L:");\
		asm("	nop;nop");\
		asm("	movab	.L.,r0");\
		asm("	jsb	mcount");\
		}
#endif
#if u3b || u3b5 || u3b2
#define MARK(L)	{\
		asm("	.data");\
		asm("	.align	4");\
		asm(".L.:");\
		asm("	.word	0");\
		asm("	.text");\
		asm("M.L:");\
		asm("	movw	&.L.,%r0");\
		asm("	jsb	_mcount");\
		}
#endif
#ifdef pdp11
#define MARK(L)	{\
		asm("	.bss");\
		asm(".L.:");\
		asm("	.=.+2");\
		asm("	.text");\
		asm("M.L:");\
		asm("	mov	$.L.,r0");\
		asm("	jsr	pc,mcount");\
		}
#endif
#endif
