/* C compiler: MC68020 code generator */

#include "c.h"

#define	AREG	0	/* an address register */
#define	DREG	1	/* a data register */
#define FREG	2	/* a floating point register */

#if mc68020 || mc68030 || NeXT
#include <stab.h>
#include "stabbsd.c"
#endif

dclproto(static void address,(Symbol, Symbol, int));
dclproto(static void defaddress,(Symbol));
dclproto(static void defconst,(int, Value));
dclproto(static void defstring,(int, char *));
dclproto(static void defsymbol,(Symbol));
dclproto(static void doarg,(Node));
dclproto(static void emitspecial,(Node));
dclproto(static void function,(Symbol, Symbol [], Symbol [], int));
dclproto(static void global,(Symbol));
dclproto(static void import,(Symbol));
dclproto(static void local,(Symbol));
dclproto(static void progbeg,(int, char **));
dclproto(static void segment,(int));
dclproto(static void space,(int));

dclproto(static void initgen,(void));
dclproto(static void final,(Node));

Interface mcInterface = {
	"mc",
	1, 1, 0,	/* char */
	2, 2, 0,	/* short */
	4, 2, 0,	/* int */
	4, 2, 1,	/* float */
	8, 2, 1,	/* double */
	4, 2, 0,	/* T * */
	0, 2, 0,	/* struct */
	1,		/* left_to_right */
	0,		/* little_endian */
	1,		/* jump_on_return */
	0,		/* mulops_are_calls */
	0,		/* compl_band */
	0,		/* no_argb */
	NODAG,		/* no_dag */
	address,
	blockbeg,
	blockend,
	defaddress,
	defconst,
	defstring,
	defsymbol,
	0,
	export,
	function,
	gen,
	global,
	import,
	local,
	progbeg,
	progend,
	segment,
	space,
#if mc68020 || mc68030 || NeXT
	stabblock, 0, 0, stabinit, stabline, stabsym, stabtype,
#else
	0, 0, 0, 0, 0, 0, 0,
#endif
	{".globl %s\n", 1, 0, {".text", ".bss", ".data", ".text", ".data2"},
	doarg, docall, 0, 0, 0,
	final, 0, emitspecial }
}, nextInterface = {
	"next",
	1, 1, 0,	/* char */
	2, 2, 0,	/* short */
	4, 2, 0,	/* int */
	4, 2, 1,	/* float */
	8, 2, 1,	/* double */
	4, 2, 0,	/* T * */
	0, 2, 0,	/* struct */
	1,		/* left_to_right */
	0,		/* little_endian */
	1,		/* jump_on_return */
	0,		/* mulops_are_calls */
	0,		/* compl_band */
	0,		/* no_argb */
	NODAG,		/* no_dag */
	address,
	blockbeg,
	blockend,
	defaddress,
	defconst,
	defstring,
	defsymbol,
	0,
	export,
	function,
	gen,
	global,
	import,
	local,
	progbeg,
	progend,
	segment,
	space,
#if mc68020 || mc68030 || NeXT
	stabblock, 0, 0, stabinit, stabline, stabsym, stabtype,
#else
	0, 0, 0, 0, 0, 0, 0,
#endif
	{".globl %s\n", 1, 0, {".text", ".bss", ".data", ".text", ".data2"},
	doarg, docall, 0, 0, 0,
	final, 0, emitspecial }

};

static Symbol a0, a1, d0, d1, d1234, fp0;

/* address - initialize q for addressing expression p+n */
static void address(q, p, n) Symbol q, p; {
	q->x.offset = p->x.offset + n;
	if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN)
		q->x.name = stringf("%s%s%d", p->x.name, n >= 0 ? "+" : "", n);
	else
		q->x.name = stringd(q->x.offset);
}

static void defaddress(p) Symbol p; {
	if (p->scope == LABELS)
		print(".align 2; ");	/* for switch tables */
	print(".long %s\n", p->x.name);
}

/* defconst - define a constant */
static void defconst(ty, v) Value v; {
	struct real r;

	switch (ty) {
	case C: print(".byte %d\n",   v.uc); break;
	case S: print(".word %d\n",   v.ss); break;
	case I: print(".long %d\n",   v.i ); break;
	case U: print(".long 0x%x\n", v.u ); break;
	case P: print(".long 0x%x\n", v.p ); break;
	case F:
		r = decode(F, v.f);
		if (v.f)
			r.exp = (r.exp + 127)&0xff;
		print(".long 0x%x\n", (r.sign<<31) | (r.exp<<23) | (r.msb>>9));
		break;
	case D:
		r = decode(D, v.d);
		if (v.d)
			r.exp = (r.exp + 1023)&0x7ff;
		print(".long 0x%x,0x%x\n",
			(r.sign<<31) | (r.exp<<20) | (r.msb>>12),
			( r.msb<<20) | (r.lsb>>12));
		break;
	default:
		assert(0);
	}
}

static void defstring(n, s) char *s; {
	genascii(".ascii", ".byte", n, s);
}

/* defsymbol - compute and store p's back-end name */
static void defsymbol(p) Symbol p; {
	if (p->scope == CONSTANTS)
		p->x.name = p->name;
	else if (p->scope >= LOCAL && p->sclass == STATIC)
		p->x.name = stringf("L%d", genlabel(1));
	else if (p->generated)
		p->x.name = stringf("L%s", p->name);
	else
		p->x.name = stringf("_%s", p->name);
}

/* doarg - assign offset for next ARG node */
static void doarg(p) Node p; {
	assert(p);
	assert(p->syms[0]);
	assert(p->syms[1]);
	p->syms[2] = intconst(mkactual(p->syms[1]->u.c.v.i, p->syms[0]->u.c.v.i, 4));
}

static void emitspecial(p) Node p; {}

/* function - generate code for a function */
static void function(p, caller, callee, n) Symbol p, callee[], caller[]; {
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
	print(".align 2\n%s:link a6,#-%d\n", p->x.name, maxoffset + argbuildsize);
	savemask[DREG] = usedmask[DREG]&eemask[DREG];
	savemask[AREG] = usedmask[AREG]&eemask[AREG];
	savemask[FREG] = usedmask[FREG]&eemask[FREG];
	savebase[DREG] = -maxoffset;
	savebase[AREG] = savebase[DREG] + 4*bitcount(savemask[DREG]);
	savebase[FREG] = savebase[AREG] + 4*bitcount(savemask[AREG]);
	if (fmask)
		print("fmovemx #0x%x,a6@(%d)\n", fmask, -maxoffset + 4*bitcount(imask));
	if (imask)
		print("moveml #0x%x,a6@(%d)\n", imask, -maxoffset);
	if (isstruct(freturn(p->type)))
		print("movl a1,a6@(-4)\n");
	if (pflag) {
		int lab = genlabel(1);
		print("movl #L%d,a0\njsr mcount\n", lab);
		print("%s\n.even\nL%d: .skip 4\n.text\n", IR->x.segnames[BSS-1], lab);
	}
	emitcode();
	if (ttob(p->type->type) == F)
		print("fmoves fp0,d0\n");
	else if (ttob(p->type->type) == D)
		print("fmoved fp0,sp@-; movl sp@+,d0; movl sp@+,d1\n");
	if (imask)
		print("moveml a6@(%d),#0x%x\n", -maxoffset, imask);
	if (fmask)
		print("fmovemx a6@(%d),#0x%x\n", -maxoffset + 4*bitcount(imask), fmask);
	print("unlk a6\nrts\n");
}

/* global - global id */
static void global(p) Symbol p; {
	assert(p->u.seg);
	if (IR == &nextInterface && p->u.seg == BSS && Aflag >= 2) {
		if (p->type->align > 1)
			print(".align %d; ", p->type->align);
		print("%s:.skip ", p->x.name);
	} else if (p->u.seg == BSS && (p->sclass == STATIC || Aflag >= 2))
		print(".lcomm %s,", p->x.name);
	else if (p->u.seg == BSS)
		print(".comm %s,", p->x.name);
	else {
		if (p->type->align > 1)
			print(".align %d; ", p->type->align);
		print("%s:", p->x.name);
	}
}

static void import(p) Symbol p; {}

/* local - local */
static void local(p) Symbol p; {
	if (getregvar(p, rmap[ttob(p->type)]) == 0)
		p->x.name = stringd(mkauto(p, 4));
}

/* progbeg - beginning of program */
static void progbeg(argc, argv) char *argv[]; {
	nregsets = 3;
	eemask[AREG] = 0x3c; eemask[DREG] = 0xfc; eemask[FREG] = 0xff;
	tmask[AREG]  = 0x07; tmask[DREG] = 0x1f; tmask[FREG] = 0x0f;
	vmask[AREG]  = 0x38; vmask[DREG] = 0xe0; vmask[FREG] = 0xf0;
	parseflags(argc, argv);
	kflag = IR == &nextInterface;
	if (kflag)
		IR->x.segnames[BSS-1] = ".data";
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

static void segment(n) {
	setseg(n);
}

static void space(n) {
	spacen(n, ".skip");
}
