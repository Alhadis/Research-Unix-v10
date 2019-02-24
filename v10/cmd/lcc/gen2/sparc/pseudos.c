/* C compiler: SPARC code generator */

#include "c.h"

#ifdef sparc
#include <stab.h>
#include "stabbsd.c"
#endif

dclproto(static void address,(Symbol, Symbol, int));
dclproto(static void blkfetch,(int, int, int, int));
dclproto(static void blkloop,(int, int, int, int, int, int[]));
dclproto(static void blkstore,(int, int, int, int));
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
dclproto(static Symbol nextreg,(Symbol));
dclproto(static void progbeg,(int, char **));
dclproto(static void segment,(int));
dclproto(static void space,(int));

dclproto(static void initgen,(void));
dclproto(static void final,(Node));

#ifdef GEN4
dclproto(static unsigned emit2,(Node,Node[],short[]));
dclproto(static void target,(Node));
#endif

Interface sparcInterface = {
	"sparc",
	1, 1, 0,	/* char */
	2, 2, 0,	/* short */
	4, 4, 0,	/* int */
	4, 4, 1,	/* float */
	8, 8, 1,	/* double */
	4, 4, 0,	/* T * */
	0, 1, 0,	/* struct */
	1,		/* left_to_right */
	0,		/* little_endian */
	1,		/* jump_on_return */
	1,		/* mulops_are_calls */
	0,		/* compl_band */
	1,		/* no_argb */
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
#ifdef sparc
	stabblock, 0, 0, stabinit, stabline, stabsym, stabtype,
#else
	0, 0, 0, 0, 0, 0, 0,
#endif
	{".global %s\n", 0, 1, {".seg \"text\"", ".seg \"bss\"", ".seg \"data\"", ".seg \"text\""},
	doarg, docall, blkfetch, blkloop, blkstore,
	final, 0, emitspecial,
#ifdef GEN4
	sparc_kids, sparc_ntname, sparc_nts, sparc_rule, sparc_state, sparc_string, sparc_cost,
	rules, emit2, target
#endif
	}
};

static Symbol curfunc;
static int nlocals;
static Symbol d0, f0, g0, i0, freg[32], ireg[6], oreg[6];
dclproto(static void rename,(int));

/* address - initialize q for addressing expression p+n */
static void address(q, p, n) Symbol q, p; int n; {
	q->x.offset = p->x.offset + n;
	if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN)
		q->x.name = stringf("%s%s%d", p->x.name, n >= 0 ? "+" : "", n);
	else
		q->x.name = stringd(q->x.offset);
}

/* blkfetch - emit code to load k bytes from reg+off into tmp */
static void blkfetch(k, off, reg, tmp) {
	assert(k == 1 || k == 2 || k == 4);
	assert(salign >= k);
	print("ld%s [%%r%d+%d],%%r%d\n", &"ub\0uh\0..\0\0"[3*(k-1)], reg, off, tmp);
}

/* blkloop - emit loop to copy size bytes from sreg+soff to dreg+doff using tmp regs */
static void blkloop(dreg, doff, sreg, soff, size, tmp) int tmp[]; {
	if ((size&~7) > 4096) {
		print("set %d,%%%s\n", size&~7, tmp[2]);
		print("add %%r%d,%%r%d,%%r%d\n", sreg, tmp[2], sreg);
		print("add %%r%d,%%r%d,%%r%d\n", dreg, tmp[2], tmp[2]);
	} else {
		print("add %%r%d,%d,%%r%d\n", sreg, size&~7, sreg);
		print("add %%r%d,%d,%%r%d\n", dreg, size&~7, tmp[2]);
	}
	blkcopy(tmp[2], doff, sreg, soff, size&7, tmp);
	print("1: dec 8,%%r%d\n", tmp[2]);
	blkcopy(tmp[2], doff, sreg, soff - 8, 8, tmp);
	print("cmp %%r%d,%%r%d; bgt 1b; dec 8,%%r%d\n", tmp[2], dreg, sreg);
}

/* blkstore - emit code to store k bytes from tmp to reg+off */
static void blkstore(k, off, reg, tmp) {
	assert(k == 1 || k == 2 || k == 4);
	assert(dalign >= k);
	print("st%s %%r%d,[%%r%d+%d]\n", &"b\0h\0.\0\0"[2*(k-1)], tmp, reg, off);
}


static void defaddress(p) Symbol p; {
	if (p->scope == LABELS)
		print(".align 4; ");	/* for switch tables */
	print(".word %s\n", p->x.name);
}

/* defconst - define a constant */
static void defconst(ty, v) Value v; {
	struct real r;

	switch (ty) {
	case C: print(".byte %d\n",   v.uc); break;
	case S: print(".half %d\n",   v.ss); break;
	case I: print(".word %d\n",   v.i ); break;
	case U: print(".word 0x%x\n", v.u ); break;
	case P: print(".word 0x%x\n", v.p ); break;
	case F:
		r = decode(F, v.f);
		if (v.f)
			r.exp = (r.exp + 127)&0xff;
		print(".word 0x%x\n", (r.sign<<31) | (r.exp<<23) | (r.msb>>9));
		break;
	case D:
		r = decode(D, v.d);
		if (v.d)
			r.exp = (r.exp + 1023)&0x7ff;
		print(".word 0x%x,0x%x\n",
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
	if (p->scope >= LOCAL && p->sclass == STATIC)
		p->x.name = stringf("%d", genlabel(1));
	else
		p->x.name = p->name;
	if (p->scope >= LABELS)
		p->x.name = stringf(p->generated ? "L%s" : "_%s", p->x.name);
}

/* doarg - assign offset for next ARG node */
static void doarg(p) Node p; {
	static int reg;
	
	if (argoffset == 0)
		reg = 0;
	assert(p);
	assert(p->syms[0]);
	assert(p->syms[1]);
	p->syms[2] = intconst(mkactual((p->syms[1]->u.c.v.i == 8 ? 4 : p->syms[1]->u.c.v.i), p->syms[0]->u.c.v.i, 4)/4);
	p->syms[1] = intconst(reg/4);
	assert(p->op != ARG+B);
	reg += roundup(p->syms[0]->u.c.v.i, 4);
}

#ifdef GEN4
char **opcodes;
#endif

/* emitspecial - emit hard double-precision operators */
static void emitspecial(p) Node p; {
	if (genx == 4 ? (p->op == ASGN+B) : !strcmp(opcodes[p->op], "# asgnb")) {
		static int tmp[] = { 1, 2, 3 };
		dalign = salign = p->syms[1]->u.c.v.i;
		blkcopy(getregnode(p->kids[0]->syms[RX])->regnum, 0,
			getregnode(p->kids[1]->syms[RX])->regnum, 0, p->syms[0]->u.c.v.i, tmp);
	} else if (genx == 4 ? (p->op == ARG+D) : !strcmp(opcodes[p->op], "# argd")) {
		int dstoff = p->syms[2]->u.c.v.i;
		int dstreg = p->syms[1]->u.c.v.i;
		int src = getregnode(p->kids[0]->syms[RX])->regnum;
		print("st %%f%d,[%%sp+4*%d+68]\n", src, dstoff);
		print("st %%f%d,[%%sp+4*%d+68]\n", src+1, dstoff+1);
		if (dstreg <= 5)
			print("ld [%%sp+4*%d+68],%%o%d\n", dstoff, dstreg);
		if (dstreg <= 4)
			print("ld [%%sp+4*%d+68],%%o%d\n", dstoff+1, dstreg+1);
	} else if (genx == 4 ? (p->op == LOAD+D) : !strcmp(opcodes[p->op], "# movd")) {
		int dst = getregnode(p->syms[RX])->regnum;
		int src = getregnode(p->kids[0]->syms[RX])->regnum;
		print("fmovs %%f%d,%%f%d; fmovs %%f%d,%%f%d\n", src, dst, src+1, dst+1);
	} else if (genx == 4 ? (p->op == NEG+D) : !strcmp(opcodes[p->op], "# negd")) {
		int dst = getregnode(p->syms[RX])->regnum;
		int src = getregnode(p->kids[0]->syms[RX])->regnum;
		print("fnegs %%f%d,%%f%d; fmovs %%f%d,%%f%d\n", src, dst, src+1, dst+1);
	}
}

/* function - generate code for a function */
static void function(f, caller, callee, n) Symbol f, callee[], caller[]; {
	int fpused = 0, framesize, i, reg = 0, varargs;
	Symbol p, q;

	initfunc(n + glevel + pflag, 4*(16 + 1), 6*4);
	for (i = 0; callee[i]; i++)
		;
	varargs = variadic(f->type) || i > 0 && strcmp(callee[i-1]->name, "__builtin_va_alist") == 0;
	for (i = 0; p = callee[i]; i++) {
		q = caller[i];
		assert(q);
		if (isfloat(p->type) || isstruct(p->type) || reg >= 6) {
			p->x.offset = offset;
			p->x.name = q->x.name = stringd(p->x.offset);
			p->sclass = q->sclass = AUTO;
			fpused++;
		} else if (p->addressed || varargs || glevel && glevel != 3) {
			p->x.offset = offset;
			p->x.name = stringd(p->x.offset);
			p->sclass = AUTO;
			q->sclass = REGISTER;
			if (!getregvar(q, ireg[reg]))
				assert(0);
			fpused++;
		} else {
			p->sclass = q->sclass = REGISTER;
			if (!getregvar(p, ireg[reg]))
				assert(0);
			q->x.name = p->x.name;
		}
		reg += isstruct(p->type) ? 1 : roundup(q->type->size, 4)/4;
		offset += roundup(q->type->size, 4);
	}
	assert(caller[i] == 0);
	curfunc = f;
	nlocals = offset = 0;
	gencode(caller, callee);
	freeregvars();
	if (isstruct(freturn(f->type)))
		fpused++;
	if (maxoffset > 0 || (usedmask[IREG]&0x00ffff00) || (usedmask[FREG]&~3) || varargs)
		ncalls++;
	framesize = roundup(4*(16 + 1) + argbuildsize + maxoffset, 8);
	savebase[IREG] = framesize;
	print(".align 4\n.proc 4\n%s:", f->x.name);
	savemask[IREG] = 0xffff0000;
	if (ncalls == 0) {
		Symbol p;
		rename(-16);
		for (p = regvars; p; p = p->x.nextregvar)
			p->x.name = p->x.regvar->x.name;
		print(fpused ? "mov %%fp,%%g7; mov %%sp,%%fp\n" : "\n");
		savemask[IREG] = 0;
	} else if (framesize <= 4095)
		print("save %%sp,%d,%%sp\n", -framesize);
	else
		print("set %d,%%g1; save %%sp,%%g1,%%sp\n", -framesize);
	if (varargs)
		for (; reg < 6; reg++)
			print("st %%i%d,[%%fp+%d]\n", reg, 4*reg + 68);
	else
		for (offset = 4*(16 + 1), reg = i = 0; p = caller[i]; i++) {
			if (isdouble(p->type) && reg <= 4) {
				print("st %%%s,[%%fp+%d]\n", ireg[reg++]->x.name, offset);
				print("st %%%s,[%%fp+%d]\n", ireg[reg++]->x.name, offset + 4);
			} else if (isfloat(p->type) && reg <= 5)
				print("st %%%s,[%%fp+%d]\n", ireg[reg++]->x.name, offset);
			else
				reg++;
			offset += roundup(p->type->size, 4);
		}
	if (pflag) {
		int lab = genlabel(1);
		print("set L%d,%%o0; call mcount; nop\n", lab);
		print(".seg \"data\"\n.align 4; L%d:.word 0\n.seg \"text\"\n", lab);
	}
	emitcode();
	if (ncalls == 0) {
		rename(16);
		if (fpused)
			print("retl; mov %%g7,%%fp\n");
		else
			print("retl; nop\n");
	} else
		print("ret; restore\n");
}

/* global - global id */
static void global(p) Symbol p; {
	if (p->type->align > 1)
		print(".align %d; ", p->type->align);
	assert(p->u.seg);
	if (p->u.seg == BSS && (p->sclass == STATIC || Aflag >= 2))
		print(".reserve %s,", p->x.name);
	else if (p->u.seg == BSS)
		print(".common %s,", p->x.name);
	else
		print("%s:", p->x.name);
}

static void import(p) Symbol p; {
	p->x.name = stringf("_%s", p->name);
}

/* local - local */
static void local(p) Symbol p; {
	if (nlocals++ == 0 && isstruct(freturn(curfunc->type))) {
		p->x.name = stringd(4*16);
		return;
	}
	if (!p->addressed && isscalar(p->type) && !isfloat(p->type))
		p->sclass = REGISTER;
	if (glevel && glevel != 3)
		p->sclass = AUTO;
	if (getregvar(p, rmap[ttob(p->type)]) == 0)
		p->x.name = stringd(mkauto(p, p->type->align));
}

static Symbol nextreg(p) Symbol p; {
	return freg[getregnode(p)->regnum+1];
}

/* progbeg - beginning of program */
static void progbeg(argc, argv) char *argv[]; {
	int i;

	nregsets = 2;
	eemask[IREG] = 0xffff0000; eemask[FREG] = 0;
	tmask[IREG]  = 0x3fffff00; tmask[FREG]  = -1;
	vmask[IREG]  = 0x3ff00000; vmask[FREG]  = 0;
	parseflags(argc, argv);
	initgen();
	d0 = mkregs(  "0",  0,  0, 2, 3, FREG, D, 0);
	f0 = mkregs(  "0",  0,  0, 1, 1, FREG, F, 0);
	g0 = mkregs( "r0",  0,  0, 1, 1, IREG, I, 0);
	i0 = mkregs("r24", 24, 24, 1, 0, IREG, I, 0);  /* zero mask; use only for RETI. */
	for (i = 0; i < 6; i++) {
		oreg[i]  = mkregs("r%d", i +  8, i +  8, 1, 1, IREG, I, 0);
		ireg[i]  = mkregs("r%d", i + 24, i + 24, 1, 1, IREG, I, 0);
	}
	for (i = 0; i < 32; i++)
		freg[i] = mkregs("%d", i, i, 1, 1, FREG, F, 0);
	rmap[I] = mkregs("r%d",  8, 13, 1, 1, IREG, I, rmap[I]);
	rmap[I] = mkregs("r%d", 16, 23, 1, 1, IREG, I, rmap[I]);
	rmap[I] = mkregs("r%d", 24, 29, 1, 1, IREG, I, rmap[I]);
	rmap[F] = mkregs("%d", 0, 31, 1, 1, FREG, F, rmap[F]);
	rmap[D] = mkregs("%d", 0, 31, 2, 3, FREG, D, rmap[D]);
	rmap[C] = rmap[S] = rmap[P] = rmap[U] = rmap[I];
}

static void rename(inc) {
	Symbol p;

	for (p = regs; p; p = p->x.regnode->next)
		if (p == i0 || p->x.regnode->regset == IREG && p->x.regnode->mask&0x3f000000)
			p->x.name = stringf("r%d", p->x.regnode->regnum += inc);
}

static void segment(n) {
	setseg(n);
}

static void space(n) {
	spacen((n), ".skip");
}
