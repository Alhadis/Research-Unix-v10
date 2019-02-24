/* C compiler: machine-independent back-end support routines */

#include "c.h"

unsigned freemask[3];
int savebase[3];
unsigned savemask[3];
unsigned tempmask[3];
unsigned usedmask[3];
unsigned eemask[3];
unsigned tmask[3];
unsigned vmask[3];

char **asmstr[MAXINTS];
int nregsets;
Node last;
int maxoffset;
int ncalls;
int offset;
int argoffset;
int argbuildsize;
int bflag, dflag, kflag, pflag, rflag;
char **opcodes;
Symbol regs;
Symbol regvars;
Symbol rmap[8];
int dalign, salign;
int genx = 2;

static int cseg;
static Node curhead;

dclproto(static Node *findxnext,(Node));
dclproto(static void freereg,(Symbol));
dclproto(static void genreload,(Node*, Symbol, int, Symbol));
dclproto(static void genreloads,(Node, Symbol, int, Symbol));
dclproto(static Symbol genspill,(Symbol, int, Node *));
dclproto(static Symbol getreg,(Symbol, Node, unsigned, int));
dclproto(static void linearize,(Node, Node *, Node));
dclproto(static Node *lin1,(Node, Node *));
dclproto(static Symbol mkreg,(char *, int, int, int, int, Symbol));
dclproto(static void mvregvar,(Symbol, unsigned));
dclproto(static int ntob,(Symbol));
dclproto(static int overlaps,(Symbol, Symbol));
dclproto(static void ralloc,(Node));
dclproto(static int rmember,(Regnode, Symbol));
dclproto(static Symbol spillee,(Symbol, Node, unsigned));
dclproto(static void spillr,(Symbol, Node));
dclproto(static int uses,(Node, Symbol));

/* aset - record that p and any right ADD+P descendants are address calculations */
int aset(p, n) Node p; {
	p = p->kids[n];
	p->x.loadaddr = 1;
	while (p->op == ADD+P) {
		p = p->kids[1];
		p->x.loadaddr = 1;
	}
	return 1;
}

/* bitcount - count number of 1 bits in mask */
int bitcount(mask) unsigned mask; {
	unsigned i, n = 0;

	for (i = 1; i; i <<= 1)
		if (mask&i)
			n++;
	return n;
}

/* blkcopy - emit code to copy size bytes from sreg+soff to dreg+doff using tmp regs */
void blkcopy(dreg, doff, sreg, soff, size, tmp) int tmp[]; {
	if (size <= 0)
		return;
	else if (size <= 1) {
		blkunroll(1, dreg, doff, sreg, soff, size, tmp);
	} else if (size <= 3) {
		blkunroll(2, dreg, doff, sreg, soff, 2, tmp);
		blkcopy(dreg, doff+(size&~(2-1)), sreg, soff+(size&~(2-1)), size&(2-1), tmp);
	} else if (size <= 16) {
		blkunroll(4, dreg, doff, sreg, soff, size&~(4-1), tmp);
		blkcopy(dreg, doff+(size&~(4-1)), sreg, soff+(size&~(4-1)), size&(4-1), tmp);
	} else
		(*IR->x.blkloop)(dreg, doff, sreg, soff, size, tmp);
}

/* blkunroll - blkcopy as an unrolled loop using k-byte chunks */
void blkunroll(k, dreg, doff, sreg, soff, size, tmp) int tmp[]; {
	int i;

	assert(IR->x.maxunalignedload);
	if ((salign < k || dalign < k) && k > IR->x.maxunalignedload)
		k = IR->x.maxunalignedload;
	(*IR->x.blkfetch)(k, soff, sreg, tmp[0]);
	for (i = k; i <= size - k; i += k) {
		(*IR->x.blkfetch)(k, i+soff,   sreg, tmp[(i&k)==k]);
		(*IR->x.blkstore)(k, i+doff-k, dreg, tmp[(i&k)!=k]); 
	}
	(*IR->x.blkstore)(k, i+doff-k, dreg, tmp[((i-k)&k)==k]);
}

/* blockbeg - begin a compound statement */
void blockbeg(e) Env *e; {
	int i;

	e->off = offset;
	for (i = 0; i < nregsets; i++)
		e->freemask[i] = freemask[i];
}

/* blockend - end a compound statement */
void blockend(e) Env *e; {
	int i;

	if (offset > maxoffset)
		maxoffset = offset;
	offset = e->off;
	for (i = 0; i < nregsets; i++)
		freemask[i] = e->freemask[i];
}

/* docall - finalize a call */
void docall(p) Node p; {
	if (argoffset > argbuildsize)
		argbuildsize = argoffset;
	p->syms[0] = intconst(argoffset);
	argoffset = 0;
}

/* dumpregs - trace register allocation */
void dumpregs(msg, a, b) char *a, *b, *msg; {
	int i;

	fprint(2, msg, a, b);
	for (i = 0; i < nregsets; i++)
		fprint(2, "(free[%d]=%x)\n", i, freemask[i]);
}

/* dumptree1 - emit debugging image of dag p, indented n, with kid# k */
void dumptree1(p, op, k, n) Node p; {
	int i;
	char *s;
	extern char **opcodes;

	if (dflag == 0 || p == 0)
		return;
	for (i = 0; i < n; i++)
		fprint(2, " ");
	if (k < 0)
		fprint(2, "(root=%x ", p);
	else
		fprint(2, "(kid%d=%x ", k, p);
	s = opname(op);
	if (*s >= '0' && *s <= '9')
		s = opcodes[op];
	fprint(2, "op=%s ", s);
	for (i = 0; i < MAXINTS; i++)
		if (p->x.ints[i])
			fprint(2, "I%d=%s ", i, asmstr[i][p->x.ints[i]]);
	for (i = 0; i < MAXSYMS; i++)
		if (p->syms[i])
			fprint(2, "S%d=%s ", i, p->syms[i]->x.name);
	fprint(2, "count=%d)\n", p->count);
	if (p->x.registered == 0)
		for (i = 0; i < MAXKIDS; i++)
			if (p->kids[i])
				dumptree1(p->kids[i], p->kids[i]->op, i, n+1);
	if (k == -1)
		fprint(2, "\n");
}

void export(p) Symbol p; {
	if (!bflag)
		print(IR->x.exportfmt, p->x.name);
}

/* findxnext - return the address of the pointer to p in the linearized chain */
static Node *findxnext(p) Node p; {
	Node *q;

	for (q = &curhead; *q; q = &(*q)->x.next)
		if (*q == p)
			return q;
	assert(0);
	return 0;
}

/* freereg - free register p */
static void freereg(p) Symbol p; {
	assert(p->x.regnode);
	p->x.regnode->count = 0;
	freemask[p->x.regnode->regset] |= p->x.regnode->mask;
	debug(rflag, dumpregs("(freeing %s)\n", p->x.name, 0));
}

/* freeregvars - free register variables */
void freeregvars() {
	Symbol p;

	for (p = regvars; p; p = p->x.nextregvar) {
		Regnode r = p->x.regvar->x.regnode;
		usedmask[r->regset] |= r->mask;
		freereg(p->x.regvar);
	}
}

/* gen - drive nodelist from front end through all back-end phases */
Node gen(nodelist) Node nodelist; {
	int i;
	Node p, q = 0;

	curhead = nodelist;
	last = 0;
	for (p = nodelist; p; p = p->link) {
		switch (generic(p->op)) {
		case CALL:
			p->x.sideeffect = 1;
			p->x.iscall = 1;
			p->x.args = q;
			q = 0;
			(*IR->x.docall)(p);
			break;
		case ARG:
			p->x.isarg = 1;
			p->x.args = q;
			q = p;
			(*IR->x.doarg)(p);
			/* fall through */
		case ASGN: case JUMP: case LABEL: case RET:
		case EQ: case GE: case GT: case LE: case LT: case NE:
			assert(p->count == 0);
			p->x.sideeffect = 1;
			break;
		case INDIR:
			p->x.unavailable = 1;
			assert(p->count);
			break;
		case DIV: case MOD: case MUL:
			if (IR->mulops_are_calls && (optype(p->op) == I || optype(p->op) == U))
				break;
			/* else fall through */
		default:
			assert(0);
		}
		(*IR->x.rewrite)(p);
		last = p;
		debug(dflag, fprint(2, "(last=%x)\n", last));
	}
	last = 0;
	linearize(curhead, &curhead, 0);
	for (p = curhead; p; p = p->x.next) {
		assert(p->x.sideeffect || p->count);
		if (p->x.registered == 0) {
			if (p->x.iscall)
				for (q = p->x.args; q; q = q->x.args)
					if (q->syms[RX] && q->syms[RX]->x.regnode)
						freereg(q->syms[RX]);
			ralloc(p);
			if (p->x.isarg && p->syms[RX] && p->syms[RX]->x.regnode)
				p->syms[RX]->x.regnode->count = -1;
			else if (p->x.listed && p->count == 0 && p->syms[RX] && p->syms[RX]->x.regnode && p->syms[RX]->x.regnode->count >= 0)
				freereg(p->syms[RX]);
			last = p;
		} else
			last = 0;
	}
	for (i = 0; i < nregsets; i++) {
		freemask[i] |= tempmask[i];
		tempmask[i] = 0;
	}
	return curhead;
}

/* genascii - emit ascii string str */
void genascii(ascii, byte, len, str) char *ascii, *byte, *str; {
	char *s = str;
	int m, n;

	for (n = 5, m = 9; s < str + len; s++) {
		if (*s == '"' || *s == '\\')
			m += 2;
		else
			m += *s < ' ' || *s >= 0177 ? 4 : 1;
		n += *s < 10 ? 2 : *s < 100 ? 3 : 4;
	}
	s = str;
	if (ascii == 0 || n < m) {
		print("%s %d", byte, (*s++)&0xff);
		for (n = 0; s < str + len; n++)
			if (n > 10) {
				n = 0;
				print("\n%s %d", byte, (*s++)&0xff);
			} else
				print(",%d", (*s++)&0xff);
		outs("\n");
	} else {
		n = 0;
		print("%s \"", ascii);
		for (s = str; s < str + len; s++) {
			if (n >= 64) {
				print("\"\n%s \"", ascii);
				n = 0;
			}
			if (*s == '"' || *s == '\\') {
				print("\\%c", *s);
				n += 2;
			} else if (*s >= ' ' && *s < 0177) {
				*bp++ = *s;
				n += 1;
			} else {
				print("\\%d%d%d", (*s>>6)&3, (*s>>3)&7, *s&7);
				n += 4;
			}
		}
		outs("\"\n");
	}
}

/* genreload - make *pp use reloads instead of r */
static void genreload(pp, temp, ty, r) Node *pp; Symbol r, temp; {
	int i;
	Node p = *pp;

	for (i = 0; i < MAXKIDS; i++)
		if (p->kids[i] && p->kids[i]->syms[RX] == r && p->kids[i]->x.registered) {
			Symbol target = p->kids[i]->x.rtarget;
			p->kids[i] = newnode(INDIR + ty, newnode(ADDRL+P, 0, 0, temp), 0, 0);
			p->kids[i]->count = 1;
			(*IR->x.rewrite)(p->kids[i]);
			if (target)
				rtarget(p, i, target);
			(*IR->x.rewrite)(p);
			linearize(p->kids[i], pp, *pp);
			if (p->kids[i])
				pp = &p->kids[i]->x.next;
		}
}

/* genreloads - make the nodes after dot use reloads instead of r */
static void genreloads(dot, temp, ty, r) Node dot; Symbol r, temp; {
	Node p, *pp;

	if (IR->x.twoop && dot->x.zap0) {
		pp = findxnext(dot);
		genreload(pp, temp, ty, r);
		for (p = *pp; p != dot; p = p->x.next)
			ralloc(*pp);
	}
	for (pp = &dot->x.next; dot = *pp; pp = &dot->x.next)
		genreload(pp, temp, ty, r);
}

/* genspill - generate code to spill r and return the temp used */
static Symbol genspill(r, ty, last) Symbol r; Node *last; {
	Node p, q;
	Symbol s, tmp;
	static struct symbol z;
	unsigned i, savemask[(sizeof freemask)/(sizeof freemask[0])];

	for (i = 0; i < nregsets; i++) {
		savemask[i] = vmask[i];
		vmask[i] &= ~usedmask[i];
	}
	tmp = newtemp(REGISTER, ty);
	for (i = 0; i < nregsets; i++)
		vmask[i] = savemask[i];
	debug(rflag, fprint(2, "(spilling %s to symbol %s=%s)\n", r->x.name, tmp->name, tmp->x.name));
	s = (Symbol) talloc(sizeof *s);
	*s = z;
	s->sclass = REGISTER;
	s->x.name = r->x.name;
	symeaddr(s) = symeaddr(r);
	s->x.regvar = r;
	q = newnode(ADDRL+P, 0, 0, s);
	q = newnode(INDIR + ty, q, 0, 0);
	p = newnode(ADDRL+P, 0, 0, tmp);
	p = newnode(ASGN + ty, p, q, 0);
	p->x.sideeffect = 1;
	(*IR->x.rewrite)(p);
	q = *last;
	linearize(p, last, q);
	for (p = *last; p != q; p = p->x.next)
		ralloc(p);
	return tmp;
}

/* getint - return value of signed integer constant */
int getint(p) Symbol p; {
	switch (ttob(p->type)) {
	case C:		return p->u.c.v.sc;
	case S:		return p->u.c.v.ss;
	default:	return p->u.c.v.i;
	}
}

/* getreg - assign a register from set rs to node p */
static Symbol getreg(rs, p, rmask, count) Symbol rs; Node p; unsigned rmask; {
	Regnode rn;
	Symbol r;

	if (rs->x.regnode->link == 0)
		rmask = -1;
	for (r = rs; r; r = rn->link) {
		rn = r->x.regnode;
		assert(rn);
		if ((rn->mask&~(freemask[rn->regset]&rmask)) == 0) {
			freemask[rn->regset] &= ~rn->mask;
			r->x.regnode->node = p;
			r->x.regnode->count = count;
			return r;
		}
	}
	if (p == 0)  /* register variable */
		return 0;
	r = spillee(rs, p, rmask);
	spill(r->x.regnode->mask, r->x.regnode->regset, p);
	r = getreg(rs, p, rmask, count);
	assert(r);
	return r;
}

/* getregnode - return regnode for register variable or codegen temp */
Regnode getregnode(p) Symbol p; {
	assert(p);
	if (p->x.regvar) {
		assert(!p->x.regnode);
		assert(p->x.regvar->x.regnode);
		return p->x.regvar->x.regnode;
	} else {
		assert(p->x.regnode);
		return p->x.regnode;
	}
}

/* getregset - return index of the register set to which p's been allocated */
int getregset(p) Symbol p; {
	if (p->sclass == REGISTER) {
		assert(p->x.regvar);
		return getregnode(p)->regset;
	} else
		return -1;
}

/* getregvar - assign variable p to a register if possible */
int getregvar(p, reglist) Symbol p, reglist; {
	Symbol reg;

	assert(p && p->type);
	if (reglist && isscalar(p->type) && isregvar(p) && (reg = getreg(reglist, 0, vmask[reglist->x.regnode->regset], -1))) {
		p->x.regvar = reg;
		p->x.name = reg->x.name;
		symeaddr(p) = symeaddr(reg);
		p->x.nextregvar = regvars;
		regvars = p;
		debug(rflag, dumpregs("(allocating %s to symbol %s)\n", p->x.name, p->name));
		if (p->temporary)
			tempmask[reg->x.regnode->regset] |= reg->x.regnode->mask;
		return 1;
	} else {
		p->sclass = AUTO;
		return 0;
	}
}

/* getunsigned - return value of unsigned integer constant */
unsigned getunsigned(p) Symbol p; {
	switch (ttob(p->type)) {
	case C:		return p->u.c.v.uc;
	case S:		return p->u.c.v.us;
	default:	return p->u.c.v.u;
	}
}

/* initfunc - common function initialization */
void initfunc(a, b, c) {
	int i;

	offset = b;
	maxoffset = 0;
	argbuildsize = c;
	ncalls = a;
	regvars = 0;
	for (i = 0; i < nregsets; i++) {
		usedmask[i] = 0;
		freemask[i] = -1;
	}
}

/* linearize - linearize nodelist p */
static void linearize(p, last, next) Node *last, next, p; {
	for (; p; p = p->link)
		if (p->x.sideeffect || p->count > 0) {
			last = lin1(p, last);
			p->x.listed = 1;
		}
	*last = next;
}

/* lin1 - linearize dag p */
static Node *lin1(p, last) Node p, *last; {
	int i;

	if (p->x.linearized)
		return last;
	for (i = 0; i < MAXKIDS; i++)
		if (p->kids[i])
			last = lin1(p->kids[i], last);
	p->x.linearized = 1;
	*last = p;
	debug(rflag, fprint(2, "(listing %x)\n", p));
	return &p->x.next;
}

/* lop - p->syms[i] &= mask */
int lop(p, i, size) Node p; {
	unsigned before = getunsigned(p->syms[i]);
	unsigned after = before&((1<<size) - 1);

	if (before == after)
		return 0;
	else {
		Value v;
		v.u = after;
		p->syms[i] = constant(p->syms[i]->type, v);
		return 1;
	}
}

/* mkactual - make next frame offset */
int mkactual(prealign, size, postalign) {
	int n = roundup(argoffset, prealign);
	argoffset = roundup(n + size, postalign);
	return n;
}

/* mkauto - make next local */
int mkauto(p, align) Symbol p; {
	offset = roundup(offset + p->type->size, align);
	p->x.offset = -offset;
	return -offset;
}

/* mkreg - create register s with given mask, regset, and link */
static Symbol mkreg(s, n, mask, regset, regtype, link) char *s; Symbol link; {
	static struct symbol zs;
	static struct regnode zr;
	Symbol q = (Symbol)alloc(sizeof *q);

	*q = zs;
	q->x.name = string(s);
	symeaddr(q) = atoi(s);
	q->x.regnode = (Regnode)alloc(sizeof *q->x.regnode);
	*q->x.regnode = zr;
	q->x.regnode->regnum = n;
	q->x.regnode->mask = mask;
	q->x.regnode->link = link;
	q->x.regnode->regset = regset;
	q->x.regnode->regtype = regtype;
	q->x.regnode->next = regs;
	regs = q;
	return q;
}

/* mkregs - create registers lo to hi by inc with given mask and type */
Symbol mkregs(fmt, lo, hi, inc, mask, regset, regtype, p) char *fmt; Symbol p; {
	for (; lo*inc <= hi*inc; lo += inc)
		p = mkreg(stringf(fmt, lo), lo, mask << lo, regset, regtype, p);
	return p;
}

/* mvregvar - move register variable */
static void mvregvar(p, mask) Symbol p; unsigned mask; {
	Symbol reg;

	if (reg = getreg(rmap[ttob(p->type)], 0, ~mask, -1)) {
		freereg(p->x.regvar);
		p->x.regvar = reg;
		p->x.name = reg->x.name;
		symeaddr(p) = symeaddr(reg);
		debug(rflag, dumpregs("(moving register variable %s to %s)\n", p->name, p->x.name));
	}
}

/* mvregvars - move register variables to reduce callee saves */
void mvregvars() {
	Symbol p;

	if (ncalls == 0) {
		unsigned i, mask[(sizeof usedmask)/(sizeof usedmask[0])];
		for (i = 0; i < nregsets; i++)
			mask[i] = usedmask[i] | eemask[i];
		for (p = regvars; p; p = p->x.nextregvar) {
			Regnode r = p->x.regvar->x.regnode;
			mask[r->regset] |= r->mask;
		}
		for (p = regvars; p; p = p->x.nextregvar) {
			Regnode r = p->x.regvar->x.regnode;
			if (r->mask&eemask[r->regset])
				mvregvar(p, mask[r->regset]);
		}
	}
	freeregvars();
}

/* ntob - infer type suffix from register set for register p */
static int ntob(p) Symbol p; {
	Regnode r = getregnode(p);
	assert(r->regtype);
	return r->regtype;
}

/* overlaps - do registers x and y overlap? */
static int overlaps(x, y) Symbol x, y; {
	Regnode rx = getregnode(x), ry = getregnode(y);
	return rx->regset == ry->regset && rx->mask&ry->mask;
}

/* parseflags - parse back end flags */
void parseflags(argc, argv) char *argv[]; {
	for (++argv; --argc > 0; ++argv)
		if (strcmp(*argv, "-d") == 0)
			dflag++;
		else if (strcmp(*argv, "-p") == 0 || strcmp(*argv, "-pg") == 0)
			pflag++;
		else if (strcmp(*argv, "-r") == 0)
			rflag++;
		else if (strcmp(*argv, "-k") == 0)
			kflag++;
}

void progend() {}

/* ralloc - assign registers for p */
static void ralloc(p) Node p; {
	Symbol s;
	int i, n;

	assert(p && p->x.registered == 0);
	n = IR->x.twoop && p->x.zap0 ? 1 : MAXKIDS;
	for (i = 0; i < n; i++)
		if (p->kids[i]) {
			assert(p->kids[i]->x.registered);
			s = p->kids[i]->syms[RX];
			if (s == 0) {
				debug(rflag, fprint(2, "(%x->kids[%d]=%x has no register allocated)\n", p, i, p->kids[i]));
				assert(s);
			}
			if (s->x.regnode && s->x.regnode->count >= 0 && --s->x.regnode->count == 0)
				freereg(s);
		}
	s = p->syms[RX];
	if (s && s->x.regnode) {
		p->syms[RX] = getreg(s, p, tmask[s->x.regnode->regset], p->count);
		usedmask[p->syms[RX]->x.regnode->regset] |= p->syms[RX]->x.regnode->mask;
		debug(rflag, dumpregs("(allocating %s to node %x)\n", p->syms[RX]->x.name, (char *) p));
	}
	for (i = n; i < MAXKIDS; i++)
		if (p->kids[i]) {
			assert(p->kids[i]->x.registered);
			s = p->kids[i]->syms[RX];
			assert(s);
			if (s->x.regnode && s->x.regnode->count >= 0 && --s->x.regnode->count == 0)
				freereg(s);
		}
	p->x.registered = 1;
	(*IR->x.final)(p);
}

/* range - does signed k fit in n bits? */
int range(k, n) {
	return k >= -(1<<(n - 1)) && k < (1<<(n - 1));
}

/* regchain - does this binary instruction overwrite its left operand? */
regchain(a) Node a; {
	return a->kids[0] == 0 || a->kids[0]->syms[RX] == a->syms[RX];
}

/* regloc - return "id" for p's register */
unsigned regloc(p) Symbol p; {
	assert(p && p->sclass == REGISTER && p->x.regvar && p->x.regvar->x.regnode);
	return p->x.regvar->x.regnode->regset<<8 | p->x.regvar->x.regnode->regnum;
}
	
/* rmcopy - remove the register-to-register copy in node a? */
int rmcopy(a) Node a; {
	return !isregvar(a->syms[RX]) && !a->x.unavailable;
}

/* rmember - is regnode r in register class s? */
static int rmember(r, s) Regnode r; Symbol s; {
	assert(r && s->x.regnode);
	for (; s; s = s->x.regnode->link)
		if (r->regset == s->x.regnode->regset && r->mask == s->x.regnode->mask)
			return 1;
	return 0;
}

/* rtarget - evaluate p->kids[n] into register r */
void rtarget(p, n, r) Node p; Symbol r; {
	Node q;
	Regnode regnode;
	Symbol s;

	assert(p && r);
	q = p->kids[n];
	assert(q && r);
	regnode = getregnode(r);
	assert(r->x.regvar || regnode->link == 0);
	s = q->syms[RX];
	assert(s);
	if (!isregvar(s) && rmember(regnode, s)) {
		debug(rflag, fprint(2, "(targeting %x to %s)\n", q, r->x.name));
		q->syms[RX] = q->x.rtarget = r;
	} else {
		debug(rflag, fprint(2, "(moving %x to %s)\n", q, r->x.name));
		q->count--;
		p->kids[n] = q = newnode(LOAD + ntob(q->syms[RX]), q, 0, q->syms[0]?q->syms[0]:p->syms[0]);
		q->count++;
		q->syms[RX] = q->x.rtarget = r;
		(*IR->x.rewrite)(q);
	}
}

/* setseg - switch to logical segment n (1..5) */
void setseg(n) {
	assert(n >= 0 && n < sizeof IR->x.segnames/sizeof IR->x.segnames[0]);
	cseg = n;  
	if (IR->x.segnames[n-1])
		print("%s\n", IR->x.segnames[n-1]); 
}

/* spacen - allocate n bytes of space in the current segment */
void spacen(n, prefix) char *prefix; {
	if (cseg != BSS)
		print("%s ", prefix);
	print("%d\n", n);
}

/* spill - spill all busy registers in regset n that overlap mask */
void spill(mask, n, dot) unsigned mask; Node dot; {
	Symbol r;

	if (mask&~freemask[n])
		for (r = regs; r; r = r->x.regnode->next)
			if (n == r->x.regnode->regset && mask&r->x.regnode->mask && r->x.regnode->count)
				spillr(r, dot);
	usedmask[n] |= mask;
}

/* spillee - identify most-distantly-used register in register class r */
static Symbol spillee(r, dot, rmask) Node dot; Symbol r; unsigned rmask; {
	int bestdist = -1;
	Symbol bestreg = 0;

	for (; r; r = r->x.regnode->link)
		if ((r->x.regnode->mask&~rmask) == 0 && r->x.regnode->count >= 0) {
			Node q;
			int dist = 0;
			for (q = dot; q && !uses(q, r); q = q->x.next)
				dist++;
			if (q && dist > bestdist) {  /* no local use => reg vbl */
				bestdist = dist;
				bestreg = r;
			}
		}
	assert(bestreg);
	return bestreg;
}

/* spillr - spill register r */
static void spillr(r, dot) Symbol r; Node dot; {
	int ty = ntob(r);
	if (r->x.regnode->count < 0) {
		Node p = newnode(LOAD+ty,
			newnode(INDIR+ty,
			newnode(ADDRL+P, 0, 0,
			genspill(r, ty, findxnext(dot))), 0, 0), 0, 0);
		p->syms[RX] = r;
		p->x.sideeffect = 1;
		(*IR->x.rewrite)(p);
		linearize(p, &dot->x.next, dot->x.next);
		p->x.registered = 1;
	} else if (dot->x.registered && r == dot->syms[RX]) {
		/* dot can't have set it yet, so no spill is needed */
	} else {
		genreloads(dot, genspill(r, ty, &r->x.regnode->node->x.next), ty, r);
		freereg(r);
	}
}

/* uses - does node p read register r? */
static int uses(p, r) Node p; Symbol r; {
	int i;

	for (i = 0; i < MAXKIDS; i++)
		if (
			p->kids[i] &&
			p->kids[i]->x.registered && 
			overlaps(r, p->kids[i]->syms[RX])
		)
			return 1;
	return 0;
}
