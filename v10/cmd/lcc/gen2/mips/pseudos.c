/* C compiler: MIPS code generator */

#include "c.h"

static Symbol f0, ireg[32], d4, d6, f12, f14;

static int flabel;
int gnum = 8;
char *nrflag = "";
int framesize;

dclproto(static void addr,(Symbol, Symbol, int));
dclproto(static void funcsave,(int, int, int, int, unsigned, unsigned, int));
dclproto(static void storearg,(Symbol, Symbol, Symbol));
dclproto(static unsigned a_def,(Symbol));
dclproto(static void a_defaddress,(Symbol));
dclproto(static unsigned a_emit,(unsigned, int, int, int, int));
dclproto(static void a_end,(Symbol));
dclproto(static void a_entry,(Symbol, int, int));
dclproto(static int a_offset,(int));
dclproto(static void a_symbol,(int, Symbol));
dclproto(static void a_word,(unsigned, int));

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
dclproto(static void progbeg,(int, char **));
dclproto(static void segment,(int));
dclproto(static void space,(int));

dclproto(static void stabinit,(char *, int, char *[]));
dclproto(static void stabline,(Coordinate *));
dclproto(static void stabsym,(Symbol));

dclproto(static void initgen,(void));
dclproto(static void final,(Node));

#ifdef GEN4
dclproto(static unsigned emit2,(Node,Node[],short[]));
dclproto(static void target,(Node));
#endif

struct emit put = {
	setseg,
	spacen,
	genascii,
	a_emit,
	addr,
	a_def,
	a_defaddress,
	a_end,
	a_entry,
	a_offset,
	a_symbol,
	a_word
};

Interface mipsInterface = {
	"mips",
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
	0,
	0,
	0,
	stabinit,
	stabline,
	stabsym,
	0,
	{".globl %s\n", 0, 4, {".text", 0, 0, ".rdata", ".rdata" },
	doarg, docall, blkfetch, blkloop, blkstore,
	final, 0, emitspecial,
#ifdef GEN4
	mips_kids, mips_ntname, mips_nts, mips_rule, mips_state, mips_string, mips_cost,
	rules, emit2, target
#endif
	}
}, mipselInterface = {
	"mipsel",
	1, 1, 0,	/* char */
	2, 2, 0,	/* short */
	4, 4, 0,	/* int */
	4, 4, 1,	/* float */
	8, 8, 1,	/* double */
	4, 4, 0,	/* T * */
	0, 1, 0,	/* struct */
	1,		/* left_to_right */
	1,		/* little_endian */
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
	0,
	0,
	0,
	stabinit,
	stabline,
	stabsym,
	0,
	{".globl %s\n", 0, 4, {".text", 0, 0, ".rdata", ".rdata" },
	doarg, docall, blkfetch, blkloop, blkstore,
	final, 0, emitspecial,
#ifdef GEN4
	mips_kids, mips_ntname, mips_nts, mips_rule, mips_state, mips_string, mips_cost,
	rules, emit2, target
#endif
	}
};

#define nop	0,0,0,0,0

static unsigned a_def(s1) Symbol s1; {
	print("%s:\n", s1->x.name);
	return 0;
}

static void a_defaddress(p) Symbol p; {
	if (p->scope == LABELS)
		print(".align 2; ");	/* for switch tables */
	print(".word %s\n", (p)->x.name);
}

static unsigned a_emit(op, rs, rt, rd, immed) unsigned op; {
	int off = immed;

	switch (op) {
	case 0:
		if (immed)
			print("sll $%s,$%s,%d\n", ireg[rd]->x.name, ireg[rt]->x.name, immed>>6);
		else
			print("%s", nrflag);
		break;
	case o_bgt:	print("bgtz $%d,L.%d\n",    rs, rd);     break;
	case o_or:	print("or $%d,$%d,$%d\n",   rd, rs, rt); break;
	case o_sltu:	print("sltu $%d,$%d,$%d\n", rd, rs, rt); break;
	case o_addu:
		if (rs == 0)
			print("move $%d,$%d\n", rd, rt);
		else if (rt == 0)
			print("move $%d,$%d\n", rd, rs);
		else
			print("addu $%d,$%d,$%d\n", rd, rs, rt);
		break;
	case o_addui:	print("addu $%s,$%s,%d\n", ireg[rt]->x.name, ireg[rs]->x.name, immed); break;
	case o_srli:	print("srl $%s,$%s,%d\n",  ireg[rd]->x.name, ireg[rt]->x.name, immed>>6); break;
	case o_lbu:	print("lbu $%d,%d($%s)\n",  rt, off, ireg[rs]->x.name); break;
	case o_lhu:	print("lhu $%d,%d($%s)\n",  rt, off, ireg[rs]->x.name); break;
	case o_lw:	print("lw $%d,%d($%s)\n",   rt, off, ireg[rs]->x.name); break;
	case o_lwl:	print("lwl $%d,%d($%s)\n",  rt, off, ireg[rs]->x.name); break;
	case o_lwr:	print("lwr $%d,%d($%s)\n",  rt, off, ireg[rs]->x.name); break;
	case o_sb:	print("sb $%d,%d($%s)\n",   rt, off, ireg[rs]->x.name); break;
	case o_sh:	print("sh $%d,%d($%s)\n",   rt, off, ireg[rs]->x.name); break;
	case o_sw:	print("sw $%d,%d($%s)\n",   rt, off, ireg[rs]->x.name); break;
	case o_swl:	print("swl $%d,%d($%s)\n",  rt, off, ireg[rs]->x.name); break;
	case o_swr:	print("swr $%d,%d($%s)\n",  rt, off, ireg[rs]->x.name); break;
	case o_l_s:	print("l.s $f%d,%d($%s)\n", rt, off, ireg[rs]->x.name); break;
	case o_l_d:	print("l.d $f%d,%d($%s)\n", rt, off, ireg[rs]->x.name); break;
	case o_s_s:	print("s.s $f%d,%d($%s)\n", rt, off, ireg[rs]->x.name); break;
	case o_s_d:	print("s.d $f%d,%d($%s)\n", rt, off, ireg[rs]->x.name); break;
	case o_mtc1_s:	print("mtc1 $%d,$f%d\n",    rt, rd); break;
	case o_mfc1_s:	print("mfc1 $%d,$f%d\n",    rt, rd); break;
	case o_mtc1_d:	print("mtc1.d $%d,$f%d\n",  rt, rd); break;
	case o_mfc1_d:	print("mfc1.d $%d,$f%d\n",  rt, rd); break;
	case o_mov_s:	print("mov.s $f%d,$f%d\n",  immed>>6, rd); break;
	case o_mov_d:	print("mov.d $f%d,$f%d\n",  immed>>6, rd); break;
	default:	assert(0);
	}
	return rt;
}

static void a_end(s1) Symbol s1; {
	print("j $31\n%s.end %s\n", nrflag, s1->x.name);
}

static void a_entry(s1, flabel, framesize) Symbol s1; {
	segment(CODE);
	print(".align 2; .ent %s\n", s1->x.name);
	put.def(s1);
	print("$fp%d=%d\n.frame $sp,%d,$31\n", flabel, framesize, framesize);
}

static int a_offset(i) {
	return i;
}

static void a_symbol(seg, p) Symbol p; {
	if (seg == DATA)
		print(".%s; ", p->type->size > gnum || isarray(p->type) ? "data" : "sdata");
	if (seg == BSS && (p->sclass == STATIC || Aflag >= 2))
		print(".lcomm %s,", p->x.name);
	else if (seg == BSS)
		print(".comm %s,", p->x.name);
	else {
		if (p->type->align > 1)
			print(".align %c; ", "..1.2...3"[p->type->align]);
		print("%s:", p->x.name);
	}
}

static void a_word(w, sz) unsigned w; {
	static char *fmt[] = { 0, ".byte %d\n", ".half %d\n", 0, ".word 0x%x\n" };

	assert(sz >= 1 && sz < sizeof fmt/sizeof fmt[0] && fmt[sz]);
	print(fmt[sz], w);
}

/* addr - initialize q for addressing expression p+n */
static void addr(q, p, n) Symbol q, p; {
	q->x.offset = p->x.offset + n;
	if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN)
		q->x.name = stringf("%s%s%d", p->x.name, n >= 0 ? "+" : "", n);
	else
		q->x.name = stringf("$fp%d%s%d", flabel,
			q->x.offset >= 0 ? "+" : "", q->x.offset);
}

static void address(p, q, n) Symbol p, q; {
	put.addr(p, q, n);
}

/* argreg - which register, if any, holds arg at given offset and of given type? */
Symbol argreg(offset, ty, ty0, varargs) {
	assert((offset&3) == 0);
	if (offset > 12)
		return 0;
	else if (!varargs && offset == 0 && (ty == F || ty == D))
		return f12;
	else if (!varargs && offset == 4 && ty0 == F && ty == F)
		return f14;
	else if (!varargs && offset == 8 && ty0 == D && (ty == F || ty == D))
		return f14;
	else if (varargs && offset == 0 && ty == D)
		return d4;
	else if (offset == 8 && ty == D)
		return d6;
	else
		return ireg[(offset/4) + 4];
}

#define BIG (!IR->little_endian)

/* blkfetch - emit code to load k bytes from reg+off into tmp */
static void blkfetch(k, off, reg, tmp) {
	static unsigned ops[] = {0, o_lbu, o_lhu, 0, o_lw};

	assert(k == 1 || k == 2 || k == 4);
	if (salign >= k) {
		put.emit(ops[k], reg, tmp, 0, put.offset(off));
		put.emit(nop);
	} else if (k == 2) {
		put.emit(o_lbu, reg, tmp, 0, put.offset(off+!BIG));
		if (!bflag)
			print(".set noat\n");
		put.emit(o_lbu, reg, 1,   0, put.offset(off+ BIG));
		put.emit(o_slli, 0, tmp, tmp, 8<<6);
		put.emit(o_or, 1, tmp, tmp, 0);
		if (!bflag)
			print(".set at\n");
	} else {
		put.emit(o_lwl, reg, tmp, 0, put.offset(off+(BIG?0:3)));
		put.emit(o_lwr, reg, tmp, 0, put.offset(off+(BIG?3:0)));
		put.emit(nop);
	}
}

/* blkloop - emit loop to copy size bytes from sreg+soff to dreg+doff using tmp regs */
static void blkloop(dreg, doff, sreg, soff, size, tmp) int tmp[]; {
	struct symbol lab;
	unsigned pc1;

	BZERO(&lab, struct symbol);
	lab.u.l.label = genlabel(1);
	lab.name = stringd(lab.u.l.label);
	lab.scope = LABELS;
	lab.generated = 1;
	defsymbol(&lab);
	put.emit(o_addui, sreg, sreg,   0, size&~7);
	put.emit(o_addui, dreg, tmp[2], 0, size&~7);
	blkcopy(tmp[2], doff, sreg, soff, size&7, tmp);
	put.def(&lab);
	pc1 = symeaddr(&lab);
	put.emit(o_addui, sreg,   sreg,   0, put.offset(-8));
	put.emit(o_addui, tmp[2], tmp[2], 0, put.offset(-8));
	blkcopy(tmp[2], doff, sreg, soff, 8, tmp);
	if (!bflag)
		print(".set noat\n");
	put.emit(o_sltu, dreg, tmp[2], 1, 0);
	if (bflag) {
		symeaddr(&lab) = 0;
		put.def(&lab);
	}
	put.emit(o_bgt, 1, 0, bflag ? 0 : lab.u.l.label,
		put.offset((int)(pc1 - (symeaddr(&lab) + 4))>>2));
	put.emit(nop);
	if (!bflag)
		print(".set at\n");
}

/* blkstore - emit code to store k bytes from tmp to reg+off */
static void blkstore(k, off, reg, tmp) {
	static unsigned ops[] = {0, o_sb, o_sh, 0, o_sw};

	assert(k == 1 || k == 2 || k == 4);
	if (dalign >= k)
		put.emit(ops[k], reg, tmp, 0, put.offset(off));
	else if (k == 2) {
		if (!bflag)
			print(".set noat\n");
		put.emit(o_srli, 0, tmp, 1, 8<<6);
		put.emit(o_sb, reg, 1,   0, put.offset(off+!BIG));
		if (!bflag)
			print(".set at\n");
		put.emit(o_sb, reg, tmp, 0, put.offset(off+ BIG));
	} else {
		put.emit(o_swl, reg, tmp, 0, put.offset(off+(BIG?0:3)));
		put.emit(o_swr, reg, tmp, 0, put.offset(off+(BIG?3:0)));
	}
}

static void defaddress(p) Symbol p; {
	put.defaddr(p);
}

/* defconst - define a constant */
static void defconst(ty, v) Value v; {
	struct real r;

	switch (ty) {
	case C: put.word(v.uc, 1); break;
	case S: put.word(v.ss, 2); break;
	case I: put.word(v.i,  4); break;
	case U: put.word(v.u,  4); break;
	case P: put.word((unsigned int)v.p, 4); break;
	case F:
		r = decode(F, v.f);
		if (v.f)
			r.exp = (r.exp + 127)&0xff;
		put.word((r.sign<<31) | (r.exp<<23) | (r.msb>>9), 4);
		break;
	case D:
		r = decode(D, v.d);
		if (v.d)
			r.exp = (r.exp + 1023)&0x7ff;
		if (IR->little_endian) {
			put.word(( r.msb<<20) | (r.lsb>>12), 4);
			put.word((r.sign<<31) | (r.exp<<20) | (r.msb>>12), 4);
		} else {
			put.word((r.sign<<31) | (r.exp<<20) | (r.msb>>12), 4);
			put.word(( r.msb<<20) | (r.lsb>>12), 4);
		}
		break;
	default:
		assert(0);
	}
}

static void defstring(n, s) char *s; {
	put.string(".ascii", ".byte", n, s);
}

/* defsymbol - compute and store p's back-end name */
static void defsymbol(p) Symbol p; {
	if (p->scope >= LOCAL && p->sclass == STATIC)
		p->x.name = stringf("L.%d", genlabel(1));
	else if (p->generated)
		p->x.name = stringf("L.%s", p->name);
	else
		p->x.name = p->name;
}

dclproto(static Node iscall,(Node));

static Node iscall(q) Node q; {
	if (generic(q->op) == CALL)
		return q;
	else if (IR->no_dag && generic(q->op) == ASGN && generic(q->kids[1]->op) == CALL)
		return q->kids[1];
	else
		return 0;
}

/* doarg - assign offset for next ARG node */
static void doarg(p) Node p; {
	Symbol tmp;

	if (argoffset == 0) {
		Node call, q = p->link;
		while ((call = iscall(q)) == 0)
			q = q->link;
		assert(generic(call->op) == CALL && call->syms[0]);
		if (variadic(call->syms[0]->type))
			for (q = p; !iscall(q); q = q->link)
				if (generic(q->op) == ARG)
					q->x.isvararg = 1;
		if (optype(call->op) == B)
			argoffset = 4;
	}
	tmp = p->syms[0];
	p->syms[0] = intconst(mkactual(p->syms[1]->u.c.v.i, p->syms[0]->u.c.v.i, 4));
	p->syms[2] = p->syms[1];
	p->syms[1] = tmp;	/* xc=S0 */
}

#ifdef GEN4
char **opcodes;	/* dreg to get what's below to compile */
#endif

static void emitspecial(p) Node p; {

	if (genx == 4 ? (p->op == ARGB) : !strcmp(opcodes[p->op], "# argb"))
		emitargb(p);
	else if (genx == 4 ? (p->op == ASGNB) : !strcmp(opcodes[p->op], "# asgnb"))
		emitasgnb(p);
}

void emitargb(p) Node p; {
	static int tmp[] = { 2, 3, 8 };

	int dst, n = p->syms[0]->u.c.v.i + p->syms[1]->u.c.v.i;
	dalign = 4;
	salign = p->syms[2]->u.c.v.i;
	blkcopy(29, p->syms[0]->u.c.v.i,
		getregnode(p->kids[0]->syms[RX])->regnum, 0, p->syms[1]->u.c.v.i, tmp);
	for (dst = p->syms[0]->u.c.v.i; dst <= 12 && dst < n; dst += 4)
		put.emit(o_lw, r_sp, dst/4+4, 0, put.offset(dst));
	if (p->syms[0]->u.c.v.i <= 12)
		put.emit(nop);
}

void emitasgnb(p) Node p; {
	static int tmp[] = { 2, 3, 8 };

	dalign = salign = p->syms[1]->u.c.v.i;
	blkcopy(getregnode(p->kids[0]->syms[RX])->regnum, 0,
		getregnode(p->kids[1]->syms[RX])->regnum, 0, p->syms[0]->u.c.v.i, tmp);
}

/* function - generate code for a function */
static void function(f, caller, callee, n) Symbol f, callee[], caller[]; {
	int i, lbase, varargs;
	Symbol p, q, r, argireg[4];
	int ty0 = caller[0] ? ttob(caller[0]->type) : I;

	initfunc(n, 0, 0);
	for (i = 0; callee[i]; i++)
		;
	varargs = variadic(f->type) || i > 0 && strcmp(callee[i-1]->name, "va_alist") == 0;
	flabel++;
	if (isstruct(freturn(f->type))) {
		offset = 4;
		ty0 = I;
	}
	for (i = 0; p = callee[i]; i++) {
		q = caller[i];
		assert(q);
		offset = roundup(offset, q->type->align);
		p->x.offset = q->x.offset = offset;
		p->x.name = q->x.name = stringf("$fp%d+%d", flabel, offset);
		r = argreg(offset, ttob(q->type), ty0, varargs);
		if (i < 4)
			argireg[i] = r;
		offset = roundup(offset + q->type->size, 4);
		if (varargs)
			p->sclass = AUTO;
		else if (
			r &&
			ncalls == 0 &&
			!isstruct(q->type) &&
			!p->addressed &&
			(!isfloat(q->type) || r->x.regnode->regset == FREG)
		) {
			p->x.regvar = q->x.regvar = r;
			p->x.name = q->x.name = r->x.name;
			p->sclass = q->sclass = REGISTER;
			if (!isfloat(p->type))
				q->type = p->type;
		} else if (
			getregvar(p, rmap[ttob(p->type)]) &&
			r &&
			(isint(p->type) || p->type == q->type)
		) {
			assert(p->x.regvar != r);
			q->sclass = REGISTER;
			q->type = p->type;
		}
	}
	assert(caller[i] == 0);
	offset = 0;
	gencode(caller, callee);
	mvregvars();
	if (ncalls)
		usedmask[IREG] |= 1<<31;
	usedmask[IREG] &= eemask[IREG];
	usedmask[FREG] &= eemask[FREG];
	if (argbuildsize && argbuildsize < 16)
		argbuildsize = 16;
	savemask[FREG] = usedmask[FREG]; savemask[IREG] = usedmask[IREG];
	savebase[FREG] = argbuildsize; savebase[IREG] = savebase[FREG] + 4*bitcount(usedmask[FREG]);
	lbase = savebase[IREG] + 4*bitcount(usedmask[IREG]);
	framesize = roundup(lbase + maxoffset, 8);
	put.entry(f, flabel, framesize);
      	for (i = 0; p = callee[i]; i++) {
        	if (p->sclass == REGISTER) {
			assert(p->x.regvar && p->x.regvar->x.regnode);
 			symeaddr(p) = p->x.regvar->x.regnode->regnum;
               	}
       	}
	if (framesize > 0)
		put.emit(o_addui, r_sp, r_sp, 0, put.offset(-framesize));
	if (usedmask[FREG]) {
		if (!bflag)
			print(".fmask 0x%x,%d\n", usedmask[FREG], savebase[IREG] - framesize - 8);
		funcsave(o_s_d, 20, 30, 2, usedmask[FREG], 3, savebase[FREG]);
	}
	if (usedmask[IREG]) {
		if (!bflag)
			print(".mask 0x%x,%d\n", usedmask[IREG], lbase - framesize - 4);
		funcsave(o_sw, 16, 31, 1, usedmask[IREG], 1, savebase[IREG]);
	}
	if (isstruct(freturn(f->type)))
		put.emit(o_sw, r_sp, 4, 0, put.offset(framesize - 4));
	for (i = 0; i < 4 && callee[i]; i++)
		if ((r = argireg[i]) && (varargs || r != callee[i]->x.regvar))
			storearg(callee[i], caller[i], r);
	if (varargs && r)
		for (i = r->x.regnode->regnum + (r == d4 || r == d6); i <= 6; i++)
			put.emit(o_sw, r_sp, i + 1, 0, put.offset(framesize + 4*(i - 3)));
	emitcode();
	if (usedmask[FREG]) {
		funcsave(o_l_d, 20, 30, 2, usedmask[FREG], 3, savebase[FREG]);
		put.emit(nop);
	}
	if (usedmask[IREG]) {
		funcsave(o_lw, 16, 31, 1, usedmask[IREG], 1, savebase[IREG]);
		put.emit(nop);
	}
	if (framesize > 0)
		put.emit(o_addui, r_sp, r_sp, 0, put.offset(framesize));
	put.end(f);
}

/* funcsave - generate register save/restore sequences */
static void funcsave(op, lo, hi, inc, used, mask, base) unsigned used, mask; {
	for (; lo <= hi; lo += inc)
		if ((used>>lo)&mask) {
			put.emit(op, r_sp, lo, 0, put.offset(base));
			base += 4*inc;
		}
}

static void global(p) Symbol p; {
	assert(p->u.seg);
	put.symbol(p->u.seg, p);
}

/* import - import a symbol */
static void import(p) Symbol p; {
	if (!isfunc(p->type) && !bflag)
		print(".extern %s %d\n", p->name,
			isarray(p->type) ? 0 : p->type->size);
}

/* local - local */
static void local(p) Symbol p; {
	if (getregvar(p, rmap[ttob(p->type)]) == 0) {
		int offset = mkauto(p, p->type->align);
		p->x.name = stringf("$fp%d%d", flabel, offset);
	}
}

/* progbeg - beginning of program */
static void progbeg(argc, argv) char *argv[]; {
	int i;

	nregsets = 2;
	eemask[IREG] = 0xc0ff0000; eemask[FREG] = 0xfff00000;
	tmask[IREG]  = 0x0300ffff; tmask[FREG]  = 0x000f0fff;
	vmask[IREG]  = 0x40ff0000; vmask[FREG]  = 0xfff00000;
	parseflags(argc, argv);
	for (i = 0; i < argc; i++)
		if (strncmp(argv[i], "-G", 2) == 0)
			gnum = atoi(argv[i] + 2);
#if BINARY
		else if (strcmp(argv[i], "-binary") == 0) {
			dclproto(extern unsigned (*emitter),(Node, int));
			dclproto(extern unsigned emitbin,(Node, int));
			extern struct emit bput;
			bflag = 1;
			put = bput;
			emitter = emitbin;
		}
#endif
		else if (strcmp(argv[i], "-noreorder") == 0) {
			nrflag = "nop\n";
#ifdef GEN4
			addnops();
#endif
		}
	initgen();
	for (i = 0; i < 32; i++)
		ireg[i] = mkregs("%d",  i,  i, 1, 1, IREG, I, 0);
	ireg[29]->x.name = "sp";
	d4	= mkregs("%d",  4,  4, 1, 3, IREG, V, 0);
	d6	= mkregs("%d",  6,  6, 1, 3, IREG, V, 0);
	f0	= mkregs("%d",  0,  0, 1, 3, FREG, D, 0);
	f12	= mkregs("%d", 12, 12, 1, 3, FREG, D, 0);
	f14	= mkregs("%d", 14, 14, 1, 3, FREG, D, 0);
	rmap[I] = mkregs("%d",  2,  2, 1, 1, IREG, I, rmap[I]);
	rmap[I] = mkregs("%d",  8, 25, 1, 1, IREG, I, rmap[I]);
	rmap[I] = mkregs("%d", 30, 30, 1, 1, IREG, I, rmap[I]);
	rmap[D] = mkregs("%d",  4, 10, 2, 3, FREG, D, rmap[D]);
	rmap[D] = mkregs("%d", 16, 30, 2, 3, FREG, D, rmap[D]);
	rmap[F] = rmap[D]; rmap[C] = rmap[S] = rmap[P] = rmap[U] = rmap[I];
	rmap[F] = rmap[D]; rmap[C] = rmap[S] = rmap[P] = rmap[U] = rmap[I];
	if (!bflag)
		print(".set %sreorder\n", *nrflag ? "no" : "");
}

static void segment(n) {
	put.seg(n);
}

static void space(n) {
	put.spacen(n, ".space");
}

/* storearg - emit a store for an argument in register r. p is callee, q caller */
static void storearg(p, q, r) Symbol p, q, r; {
	int pn, rn;

	assert(r && r->x.regnode);
	rn = r->x.regnode->regnum;
	if (p->sclass == REGISTER && (isint(p->type) || p->type == q->type)) {
		assert(p && p->x.regvar && p->x.regvar->x.regnode);
		pn = p->x.regvar->x.regnode->regnum;
		if (!isfloat(q->type))
			put.emit(o_move, rn, 0, pn, 0);
		else if (r->x.regnode->regset == FREG) {
			put.emit(ttob(q->type) == D ? o_mov_d : o_mov_s, 0, 0, rn, pn<<6);
			put.emit(nop);
		} else
			put.emit(ttob(q->type) == D ? o_mtc1_d : o_mtc1_s, 0, rn, pn, 0);
	} else {
		if (!isfloat(q->type)) {
			int n = (q->type->size - 1)/4;
			if (n > '7' - *r->x.name)
				n = '7' - *r->x.name;
			for ( ; n >= 0; n--)
				put.emit(o_sw, r_sp, rn+n, 0, put.offset(q->x.offset + framesize + n*4));
			put.emit(nop);
		} else if (r->x.regnode->regset == FREG) {
			put.emit(ttob(q->type) == D ? o_s_d : o_s_s, r_sp, rn, 0,
				put.offset(q->x.offset + framesize));
			put.emit(nop);
		} else {
			put.emit(o_sw, r_sp, rn, 0, put.offset(q->x.offset + framesize));
			if (ttob(q->type) == D)
				put.emit(o_sw, r_sp, rn + 1, 0, put.offset(q->x.offset + framesize + 4));
			put.emit(nop);
		}
	}
}

static char *currentfile;

/* stabinit - initialize stab output */
static void stabinit(file, argc, argv) char *file, *argv[]; {
	if (file) {
		print(".file 2,\"%s\"\n", file);
		currentfile = file;
	}
}

/* stabline - emit stab entry for source coordinate *cp */
static void stabline(cp) Coordinate *cp; {
	if (cp->file && cp->file != currentfile) {
		print(".file 2,\"%s\"\n", cp->file);
		currentfile = cp->file;
	}
	print(".loc 2,%d\n", cp->y);
}

/* stabsym - output a stab entry for symbol p */
static void stabsym(p) Symbol p; {
	if (p == cfunc && IR->stabline)
		(*IR->stabline)(&p->src);
}
