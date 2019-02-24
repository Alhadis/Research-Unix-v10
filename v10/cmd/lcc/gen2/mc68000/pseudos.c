/* C compiler: MC68020 code generator */

#include "c.h"

Symbol a0, a1, d0, d1, d1234, fp0;
char *segnames[] = {"text", 0, "data", "text", "data 2"};
unsigned eemask[] = {0x3c,0xfc,0xff};
unsigned tmask[]  = {0x07,0x1f,0x0f};
unsigned vmask[]  = {0x38,0xe0,0xf0};

/* address - initialize q for addressing expression p+n */
void address(q, p, n) Symbol q, p; int n; {
	q->x.offset = p->x.offset + n;
	if (p->scope == GLOBAL || p->class == STATIC || p->class == EXTERN)
		q->x.name = stringf("%s%s%d", p->x.name, n >= 0 ? "+" : "", n);
	else
		q->x.name = stringd(q->x.offset);
}

/* asmname - print assembler code for symbol p (FIX) */
void asmname(p) Symbol p; {
	if (p->class == REGISTER)
		switch (ttob(p->type)) {
		case P:				print("a%s",  p->x.name); break;
		case F:	case D:			print("fp%s", p->x.name); break;
		case I:	case U:	case C:	case S:	print("d%s",  p->x.name); break;
		default:			assert(0);
		}
	else if (p->scope >= PARAM && p->class == AUTO)
		print("%s(a6)", p->x.name);
	else
		print("_%s", p->x.name);
}

/* defconst - define a constant */
void defconst(ty, v) Value v; {
	switch (ty) {
	case C: print("byte %d\n",   v.uc); break;
	case S: print("word %d\n",   v.ss); break;
	case I: print("long %d\n",   v.i ); break;
	case U: print("long 0x%x\n", v.u ); break;
	case P: print("long 0x%x\n", v.p ); break;
	case F:
		if (v.f == 0.0)
			print("long 0x0\n");
		else {
			struct real r;
			r = decode(F, v.f);
			r.exp = (r.exp + 127)&0xff;
			print("long 0x%x\n", (r.sign<<31) | (r.exp<<23) | (r.msb>>9));
		}
		break;
	case D:
		if (v.d == 0.0)
			print("long 0x0,0x0\n");
		else {
			struct real r;
			r = decode(D, v.d);
			r.exp = (r.exp + 1023)&0x7ff;
			print("long 0x%x,0x%x\n", (r.sign<<31) | (r.exp<<20) | (r.msb>>12),
				(r.msb<<20) | (r.lsb>>12));
		}
		break;
	default:
		assert(0);
	}
}

/* doarg - assign offset for next ARG node */
void doarg(p) Node p; {
	assert(p);
	assert(p->syms[0]);
	assert(p->syms[1]);
	p->syms[2] = intconst(mkactual(p->syms[1]->u.c.v.i, p->syms[0]->u.c.v.i, 4));
}

/* docall - finalize a call */
void docall(p) Node p; {
	if (argoffset > argbuildsize)
		argbuildsize = argoffset;
	p->syms[0] = intconst(argoffset);
	argoffset = 0;
}

/* function - generate code for a function */
void function(p, caller, callee, n) Symbol p, callee[], caller[]; {
	int i;
	unsigned fmask, imask;

	initfunc(n, 8, 0);
	for (i = 0; callee[i]; i++) {
		assert(caller[i]);
		callee[i]->x.offset = caller[i]->x.offset = offset;
		callee[i]->x.name = caller[i]->x.name = stringd(offset);
		offset = roundup(offset + caller[i]->type->size, 4);
		getregvar(callee[i], rmap[ttob(callee[i]->type)]);
	}
	assert(caller[i] == 0);
	offset = 0;
	gencode(caller, callee);
	mvregvars();
	fmask = 0;
	if (usedmask[FREG])
		for (i = 0; i < 8; i++)
			if (usedmask[FREG]&(1<<i))
				fmask |= 0x80>>i;
	imask = (((usedmask[AREG]&eemask[AREG])<<8) | (usedmask[DREG]&eemask[DREG]));
	maxoffset += 12*bitcount(fmask) + 4*bitcount(imask);
	print("even\n_%s:link $a6,&-%d\n", p->x.name, maxoffset + argbuildsize);
	if (fmask)
		print("fmovemx &0x%x,%d($a6)\n", fmask, -maxoffset + 4*bitcount(imask));
	if (imask)
		print("movm.l &0x%x,%d($a6)\n", imask, -maxoffset);
	if (isstruct(freturn(p->type)))
		print("mov.l a1,-4($a6)\n");
	if (pflag) {
		int lab = genlabel(1);
		print("mov.l &L%d,$a0\njsr mcount\n", lab);
		print("%s\n.even\nL%d: .skip 4\n.text\n", segnames[BSS-1], lab);
	}
	emitcode();
	if (ttob(p->type->type) == F)
		print("fmoves fp0,d0\n");
	else if (ttob(p->type->type) == D)
		print("fmoved fp0,-($sp); movl ($sp)+,d0; movl ($sp)+,d1\n");
	if (imask)
		print("movm.l %d($a6),&0x%x\n", -maxoffset, imask);
	if (fmask)
		print("fmovemx %d($a6),&0x%x\n", -maxoffset + 4*bitcount(imask), fmask);
	print("unlk $a6\nrts\n");
}

/* global - global id */
void global(p) Symbol p; {
	assert(p->u.seg);
	if (p->u.seg == BSS && (p->class == STATIC || Aflag >= 2))
		print("lcomm _%s,", p->x.name);
	else if (p->u.seg == BSS)
		print("comm _%s,", p->x.name);
	else {
		if (p->type->align > 1)
			print("even; ");
		print("_%s:", p->x.name);
	}
}

/* local - local */
void local(p) Symbol p; {
	if (getregvar(p, rmap[ttob(p->type)]) == 0)
		p->x.name = stringd(mkauto(p, 4));
}

/* progbeg - beginning of program */
void progbeg(argc, argv) char *argv[]; {
	parseflags(argc, argv);
	if (kflag)
		segnames[BSS-1] = "data";
	initgen();
	a0	= mkregs("%d", 0, 0, 1, 1, AREG, P, 0);
	a1	= mkregs("%d", 1, 1, 1, 1, AREG, P, 0);
	d0	= mkregs("%d", 0, 0, 1, 1, DREG, I, 0);
	d1234	= mkregs("%d", 1, 4, 1, 1, DREG, I, 0);
	fp0	= mkregs("%d", 0, 0, 1, 1, FREG, D, 0);
	rmap[P] = mkregs("%d", 0, 5, 1, 1, AREG, P, 0);
	rmap[I] = mkregs("%d", 0, 7, 1, 1, DREG, I, 0);
	rmap[D] = mkregs("%d", 0, 7, 1, 1, FREG, D, 0);
	rmap[F] = rmap[D]; rmap[C] = rmap[S] = rmap[U] = rmap[I];
}
