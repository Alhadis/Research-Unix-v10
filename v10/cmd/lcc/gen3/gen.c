/* C compiler: VAX subset code generator */

#include "c.h"

#ifdef DEBUG
#define debug(x,y) if (x) y
static void lprint(Node, char *);
static void nprint(Node);
static char *rnames(unsigned);
static int id;
static Node lhead;

#else
#define debug(x,y)
#endif

static int rflag;		/* != 0 to trace register allocation */
static int framesize;		/* size of activation record */
static int offset;		/* current frame offset */
static int argbuildsize;	/* size of argument build area */
static int argoffset;		/* offset from top of stack for next argument */
static int nregs = 12;		/* number of allocatable registers */
static unsigned rmask;		/* rmask&(1<<r) == 0 if register r is free */
static unsigned usedmask;	/* usedmask&(1<<r) == 1 if register r was used */
static int reginfo[] = {	/*      1<<x if op+x is legal; */
	0,			/* 0x1000<<x if op+x needs a register */
#include "reginfo.h"
};

static void genreloads(Node, Node, Symbol);
static Symbol genspill(Node);
static void getreg(Node);
static Node *linearize(Node, Node *, Node);
static int needsreg(Node);
static void putreg(Node);
static void ralloc(Node);
static void restore(unsigned);
static void save(unsigned);
static int spillee(Node, unsigned);
static void spill(int, unsigned, Node);
static unsigned uses(Node);
static int valid(int);

#define typecode(p) (optype(p->op) == U ? I : optype(p->op) == B ? P : optype(p->op))
#define sets(p) ((p)->x.rmask<<(p)->x.reg)

/* address - initialize q for addressing expression p+n */
static void address(Symbol q, Symbol p, int n) {
	if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN)
		q->x.name = stringf("%s%s%d", p->x.name, n >= 0 ? "+" : "", n);
	else {
		q->x.offset = p->x.offset + n;
		q->x.name = stringf("%d(%s)", q->x.offset,
			p->scope == PARAM ? "ap" : "fp");
	}
}

/* blockbeg - begin a compound statement */
static void blockbeg(Env *e) {
	assert(rmask == (((~0)<<nregs)|1));
	e->rmask = rmask;
	e->offset = offset;
}

/* blockend - end a compound statement */
static void blockend(Env *e) {
	if (offset > framesize)
		framesize = offset;
	offset = e->offset;
	rmask = e->rmask;
}

/* defaddress - initialize an address */
static void defaddress(Symbol p) {
	print(".long %s\n", p->x.name);
}

/* defconst - define a constant */
static void defconst(int ty, Value v) {
	switch (ty) {
	case C: print(".byte %d\n",   v.uc); break;
	case S: print(".word %d\n",   v.us); break;
	case I: print(".long %d\n",   v.i ); break;
	case U: print(".long 0x%x\n", v.u ); break;
	case P: print(".long 0x%x\n", v.p ); break;
#ifdef vax
	case F:
		print(".long 0x%x\n", ((unsigned *) &v.f)[0]);
		break;
	case D: 
		print(".long 0x%x,0x%x\n", ((unsigned *) &v.d)[0],
			((unsigned *) &v.d)[1]);
		break;
#else
	case F: {
		char buf[MAXLINE];
		sprintf(buf, ".float 0f%.8e\n", v.f);
		outs(buf);
		break;
		}
	case D: {
		char buf[MAXLINE];
		sprintf(buf, ".double 0d%.18e\n", v.d);
		outs(buf);
		break;
		}
#endif
	default: assert(0);
	}
}

/* defstring - emit a string constant */
static void defstring(int len, char *s) {
	while (len-- > 0)
		print(".byte %d\n", *s++);
}

/* defsymbol - initialize p's Xsymbol fields */
static void defsymbol(Symbol p) {
	if (p->scope == CONSTANTS)
		p->x.name = p->name;
	else if (p->scope >= LOCAL && p->sclass == STATIC)
		p->x.name = stringf("L%d", genlabel(1));
	else if (p->generated)
		p->x.name = stringf("L%s", p->name);
	else
		p->x.name = stringf("_%s", p->name);
}

		  /* " FDCSIUPVB" */
#define suffix(p)    ".fdbwllll."[optype((p)->op)]
#define binary(inst) print("%s%c3 r%d,r%d,r%d\n", inst, suffix(p), \
                        b->x.reg, a->x.reg, r)
#define unary(inst)  print("%s%c r%d,r%d\n", inst, suffix(p), a->x.reg, r)
#define compare(cp)  print("cmp%c r%d,r%d; j%s %s\n", suffix(p), \
                        a->x.reg, b->x.reg, cp, p->syms[0]->x.name)

/* emit - emit the dags on list p */
static void emit(Node p) {
	for (; p; p = p->x.next) {
		Node a = p->kids[0], b = p->kids[1];
		int r = p->x.reg;
		switch (p->op) {
		case BANDU:				binary("bic");   break;
		case BORU:				binary("bis");   break;
		case BXORU:				binary("xor");   break;
		case ADDD:  case ADDF:			binary("add");   break;
		case ADDI:  case ADDP:  case ADDU:	binary("add");   break;
		case SUBD:  case SUBF:			binary("sub");   break;
		case SUBI:  case SUBP:  case SUBU:	binary("sub");   break;
		case MULD:  case MULF:			binary("mul");   break;
		case MULI:  case MULU:			binary("mul");   break;
		case DIVD:  case DIVF:  case DIVI:	binary("div");   break;
		case DIVU:
			save(p->x.busy&0x3e);
			print("pushl r%d; pushl r%d; calls $2,udiv; movl r0,r%d\n",
				b->x.reg, a->x.reg, r);
			restore(p->x.busy&0x3e);
			break;
		case MODI:
			print("divl3 r%d,r%d,r0; mull2 r%d,r0; subl3 r0,r%d,r%d\n",
				b->x.reg, a->x.reg, b->x.reg, a->x.reg, r);
			break;
		case MODU:
			save(p->x.busy&0x3e);
			print("pushl r%d; pushl r%d; calls $2,urem; movl r0,r%d\n",
				b->x.reg, a->x.reg, r);
			restore(p->x.busy&0x3e);
			break;
		case RSHU:
			print("subl3 r%d,$32,r0; extzv r%d,r0,r%d,r%d\n",
				b->x.reg, b->x.reg, a->x.reg, r);
			break;
		case RSHI: case LSHI: case LSHU:
			print("ashl r%d,r%d,r%d\n", b->x.reg, a->x.reg, r);
			break;
		case INDIRB:
			print("moval (r%d),r%d\n", a->x.reg, r);
			break;
		case INDIRC: case INDIRD: case INDIRF: case INDIRI:
		case INDIRP: case INDIRS:
			print("mov%c (r%d),r%d\n", suffix(p), a->x.reg, r);
			break;
		case BCOMU:				unary("mcom" );  break;
		case NEGD:  case NEGF:  case NEGI:	unary("mneg" );  break;
		case CVCI:				unary("cvtb" );  break;
		case CVCU:				unary("movzb");  break;
		case CVSI:				unary("cvtw" );  break;
		case CVSU:				unary("movzw");  break;
		case CVDF:  case CVDI:			unary("cvtd" );  break;
		case CVFD:				unary("cvtf" );  break;
		case CVUC:  case CVUS:			unary("cvtl" );  break;
		case CVIC:  case CVIS:  case CVID:	unary("cvtl" );  break;
		case CVIU:  case CVUI: 			unary("mov"  );  break;
		case CVPU:  case CVUP: 			unary("mov"  );  break;
		case RETD: case RETF: case RETI:
			print("mov%c r%d,r0; ret\n", suffix(p), a->x.reg);
			break;
		case RETV:
			print("ret\n");
			break;
		case ADDRGP: case ADDRFP: case ADDRLP:
			print("moval %s,r%d\n", p->syms[0]->x.name, r);
			break;
		case CNSTC: case CNSTI: case CNSTP:
		case CNSTS: case CNSTU:
			print("movl $%s,r%d\n", p->syms[0]->x.name, r);
			break;
		case JUMPV:
			print("jmp (r%d)\n", a->x.reg);
			break;
		case ASGNB:
			save(p->x.busy&0x3f);
			print("movc3 $%s,(r%d),(r%d)\n", p->syms[0]->x.name,
				b->x.reg, a->x.reg);
			restore(p->x.busy&0x3f);
			break;
		case ASGNC: case ASGND: case ASGNF: case ASGNI: case ASGNP: case ASGNS:
			print("mov%c r%d,(r%d)\n", suffix(p), b->x.reg, a->x.reg);
			break;
		case ARGB:
			save(p->x.busy&0x3f);
			print("movc3 $%s,(r%d),%d(sp)\n", p->syms[0]->x.name,
				a->x.reg, p->x.argoffset);
			restore(p->x.busy&0x3f);
			break;
		case ARGD: case ARGF: case ARGI: case ARGP:
			print("mov%c r%d,%d(sp)\n", suffix(p),
				a->x.reg, p->x.argoffset);
			break;
		case CALLB:
			save(p->x.busy&0x3e);
			if (a->x.reg == 1) {
				print("movl r1,r0\n");
				a->x.reg = 0;
			}
			if (b->x.reg != 1)
				print("movl r%d,r1\n", b->x.reg);
			print("calls $0,(r%d)\n", a->x.reg);
			restore(p->x.busy&0x3e);
			break;
		case CALLD: case CALLF: case CALLI: case CALLV:
			save(p->x.busy&0x3e);
			print("calls $0,(r%d)\n", a->x.reg);
			if (p->op != CALLV)
				print("mov%c r0,r%d\n", suffix(p), r);
			restore(p->x.busy&0x3e);
			break;
		case EQD:   case EQF:   case EQI:	compare("eql" ); break;
		case GED:   case GEF:   case GEI:	compare("geq" ); break;
		case GEU:				compare("gequ"); break;
		case GTD:   case GTF:   case GTI:	compare("gtr" ); break;
		case GTU:				compare("gtru"); break;
		case LED:   case LEF:   case LEI:	compare("leq" ); break;
		case LEU:				compare("lequ"); break;
		case LTD:   case LTF:   case LTI:	compare("lss" ); break;
		case LTU:				compare("lssu"); break;
		case NED:   case NEF:   case NEI:	compare("neq" ); break;
		case LABELV:
			print("%s:", p->syms[0]->x.name);
			break;
		default: assert(0);
		}
	}
}

/* export - announce an exported symbol */
static void export(Symbol p) {
	print(".globl %s\n", p->x.name);
}

/* function - generate code for a function */
static void function(Symbol f, Symbol caller[],
	Symbol callee[], int ncalls) {
	int i;

	offset = 4;
	for (i = 0; caller[i] && callee[i]; i++) {
		offset = roundup(offset, caller[i]->type->align);
		callee[i]->x.offset = caller[i]->x.offset = offset;
		callee[i]->x.name = caller[i]->x.name = stringf("%d(ap)", offset);
		offset += caller[i]->type->size;
		callee[i]->sclass = AUTO;
	}
	usedmask = argbuildsize = framesize = offset = 0;
	gencode(caller, callee);
	print("%s:.word 0x%x\n", f->x.name, usedmask&~0x3f);
	framesize += 4*nregs + argbuildsize;
	print("subl2 $%d,sp\n", framesize);
	if (isstruct(freturn(f->type)))
		print("movl r1,-4(fp)\n");
	emitcode();
	if (glevel > 1)
		print("ret\n");
}

/* gen - generate code for the dags on list p */
static Node gen(Node p) {
	Node head, *last;

	debug(1,id = 0);
	for (last = &head; p; p = p->link)
		last = linearize(p, last, 0);
	debug(rflag,(lhead = head, lprint(head," before ralloc")));
	for (p = head; p; p = p->x.next) {
		ralloc(p);
		if (p->count == 0 && sets(p))
			putreg(p);
	}
	debug(rflag,lprint(lhead," after ralloc"));
	return head;
}

/* getreg - allocate 1 or 2 registers for node p */
static void getreg(Node p) {
	int r, m = optype(p->op) == D ? 3 : 1;

	for (r = 0; r < nregs; r++)
		if ((rmask&(m<<r)) == 0) {
			p->x.rmask = m;
			p->x.reg = r;
			rmask |= sets(p);
			usedmask |= sets(p);
			debug(rflag,fprint(2,"allocating %s to node #%d\n", rnames(sets(p)), p->x.id));
			return;
		}
	debug(rflag,lprint(lhead, " before spillee"));
	r = spillee(p, m);
	spill(r, m, p);
	debug(rflag,lprint(lhead, " after spill"));
	assert((rmask&(m<<r)) == 0);
	getreg(p);
}

/* genreloads - make the nodes after dot use reloads of temp instead of p's register */
static void genreloads(Node dot, Node p, Symbol temp) {
	int i;
	Node last;

	for (last = dot; dot = dot->x.next; last = dot)
		for (i = 0; i < MAXKIDS; i++)
			if (dot->kids[i] == p) {
				dot->kids[i] = newnode(INDIR + typecode(p),
					newnode(ADDRL+P, 0, 0, temp), 0, 0);
				dot->kids[i]->count = 1;
				p->count--;
				linearize(dot->kids[i], &last->x.next, last->x.next);
				last = dot->kids[i];
			}
	assert(p->count == 0);
}

/* genspill - generate code to spill p's register and return the temporary used */
static Symbol genspill(Node p) {
	Symbol temp = newtemp(AUTO, typecode(p));
	Node q = p->x.next;

	linearize(newnode(ASGN + typecode(p),
		newnode(ADDRLP, 0, 0, temp), p, 0),
		&p->x.next, p->x.next);
	rmask &= ~1;
	for (p = p->x.next; p != q; p = p->x.next)
		ralloc(p);
	rmask |= 1;
	return temp;
}

/* global - global id */
static void global(Symbol p) {
	switch (p->type->align) {
	case 2: print(".align 1; "); break;
	case 4: print(".align 2; "); break;
	case 8: print(".align 3; "); break;
	}
	print("%s:", p->x.name);
}

/* import - announce an imported symbol */
static void import(Symbol p) {
}

/* linearize - linearize node list p */
static Node *linearize(Node p, Node *last, Node next) {
	if (p && !p->x.visited) {
		last = linearize(p->kids[0], last, 0);
		last = linearize(p->kids[1], last, 0);
		p->x.visited = 1;
		*last = p;
		last = &p->x.next;
		debug(1,if (p->x.id == 0) p->x.id = ++id);		
		debug(rflag,{fprint(2,"listing node "); nprint(p);})
	}
	*last = next;
	return last;
}

/* local - local variable */
static void local(Symbol p) {
	offset = roundup(offset + p->type->size, p->type->align);
	offset = roundup(offset, 4);
	p->x.offset = -offset;
	p->x.name = stringf("%d(fp)", -offset);
	p->sclass = AUTO;
}

/* needsreg - does p need a register? */
static int needsreg(p) Node p; {
	assert(opindex(p->op) > 0 && opindex(p->op) < sizeof reginfo/sizeof reginfo[0]);
	return reginfo[opindex(p->op)]&(0x1000<<optype(p->op));
}

/* progbeg - beginning of program */
static void progbeg(int argc, char *argv[]) {
	extern int atoi(char *);		/* (omit) */
	while (--argc > 0)
		if (**++argv == '-' && argv[0][1] >= '0' && argv[0][1] <= '9')
			nregs = atoi(*argv + 1);
		else if (strcmp(*argv, "-r") == 0)	/* (omit) */
			rflag++;			/* (omit) */
	rmask = ((~0)<<nregs)|1;
}

/* progend - end of program */
static void progend(void) {
}

/* putreg - decrement register usage */
static void putreg(Node p) {
	if (p && --p->count <= 0)
		{ assert(p->x.rmask);
		rmask &= ~sets(p);
		debug(rflag,fprint(2,"deallocating %s from node #%d\n", rnames(sets(p)), p->x.id)); }
}

/* ralloc - assign a register for p */
static void ralloc(Node p) {
	int i;

	assert(p);
	assert(p->x.rmask == 0);
	switch (generic(p->op)) {
	case ARG:
		argoffset = roundup(argoffset, p->syms[1]->u.c.v.i);
		p->x.argoffset = argoffset;
		argoffset += p->syms[0]->u.c.v.i;
		if (argoffset > argbuildsize)
			argbuildsize = roundup(argoffset, 4);
		break;
	case CALL:
		argoffset = 0;
		break;
	default:assert(valid(p->op));
	}
	for (i = 0; i < MAXKIDS; i++)
		putreg(p->kids[i]);
	p->x.busy = rmask;
	if (needsreg(p))
		getreg(p);
}

/* restore - restore registers in mask */
static void restore(unsigned mask) {
	int i;

	for (i = 1; i < nregs; i++)
		if (mask&(1<<i))
			print("movl %d(fp),r%d\n",
				4*i - framesize + argbuildsize, i);
}

/* save - save registers in mask */
static void save(unsigned mask) {
	int i;

	for (i = 1; i < nregs; i++)
		if (mask&(1<<i))
			print("movl r%d,%d(fp)\n", i,
				4*i - framesize + argbuildsize);
}

/* segment - switch to logical segment s */
static void segment(int s) {
	switch (s) {
	case CODE: print(".text\n");   break;
	case  LIT: print(".text 1\n"); break;
	case DATA:
	case  BSS: print(".data\n");   break;
	default: assert(0);
	}
}

/* space - initialize n bytes of space */
static void space(int n) {
	print(".space %d\n", n);
}

/* spill - spill all registers that overlap (r,m) */
static void spill(int r, unsigned m, Node dot) {
	int i;
	Node p = dot;

	while (p = p->x.next)
		for (i = 0; i < MAXKIDS; i++)
			if (p->kids[i] && sets(p->kids[i])&(m<<r)) {
				Symbol temp = genspill(p->kids[i]);
				rmask &= ~sets(p->kids[i]);
				genreloads(dot, p->kids[i], temp);
			}
}

/* spillee - identify the most-distantly-used register */
static int spillee(Node dot, unsigned m) {
	int bestdist = -1, bestreg = 0, dist, r;
	Node q;

	debug(rflag,fprint(2,"spillee: dot is node #%d\n", dot->x.id));
	for (r = 1; r < nregs - (m>>1); r++) {
		dist = 0;
		for (q = dot->x.next; q && !(uses(q)&(m<<r)); q = q->x.next)
			dist++;
		assert(q);	/* (omit) */
		debug(rflag,fprint(2,"r%d used in node #%d at distance %d\n", r, q->x.id, dist));
		if (dist > bestdist) {
			bestdist = dist;
			bestreg = r;
		}
	}
	debug(rflag,fprint(2,"spilling %s\n",rnames(m<<bestreg)));
	assert(bestreg);	/* (omit) */
	return bestreg;
}

/* uses - return mask of registers used by node p */
static unsigned uses(Node p) {
	int i;
	unsigned m = 0;

	for (i = 0; i < MAXKIDS; i++)
		if (p->kids[i])
			m |= sets(p->kids[i]);
	return m;
}

/* valid - is operator op a valid operator ? */
static int valid(op) {
	return opindex(op) > 0 && opindex(op) < sizeof reginfo/sizeof reginfo[0] ?
		reginfo[opindex(op)]&(1<<optype(op)) : 0;
}

#ifdef DEBUG
/* lprint - print the nodelist beginning at p */
static void lprint(Node p, char *s) {
	fprint(2, "node list%s:\n", s);
	if (p) {
		char buf[100];
		sprintf(buf, "%-4s%-8s%-8s%-8s%-7s%-13s%s",
			" #", "op", "kids", "syms", "count", "uses", "sets");
		fprint(2, "%s\n", buf);
	}
	for ( ; p; p = p->x.next)
		nprint(p);
}

/* nprint - print a line describing node p */
static void nprint(Node p) {
	int i;
	char *kids = "", *syms = "", buf[200];

	if (p->kids[0]) {
		static char buf[100];
		buf[0] = 0;
		for (i = 0; i < MAXKIDS && p->kids[i]; i++)
			sprintf(buf + strlen(buf), "%3d", p->kids[i]->x.id);
		kids = &buf[1];
	}
	if (p->syms[0] && p->syms[0]->x.name) {
		static char buf[100];
		buf[0] = 0;
		for (i = 0; i < MAXSYMS && p->syms[i]; i++) {
			if (p->syms[i]->x.name)
				sprintf(buf + strlen(buf), " %s", p->syms[i]->x.name);
			if (p->syms[i]->u.c.loc)
				sprintf(buf + strlen(buf), "=%s", p->syms[i]->u.c.loc->name);
		}
		syms = &buf[1];
	}
	sprintf(buf, "%2d. %-8s%-8s%-8s %2d    %-13s",
		p->x.id, opname(p->op), kids, syms, p->count, rnames(uses(p)));
	sprintf(buf + strlen(buf), "%s", rnames(sets(p)));
	fprint(2, "%s\n", buf);
}

/* rnames - return names of registers given by mask m */
static char *rnames(unsigned m) {
	static char buf[100];
	int r;

	buf[0] = buf[1] = 0;
	for (r = 0; r < nregs; r++)
		if (m&(1<<r))
			sprintf(buf + strlen(buf), " r%d", r);
	return &buf[1];
}
#endif

#ifndef V9
#include <errno.h>
#ifndef errno
extern int errno;
#endif

/* strtol - interpret str as a base b number; if ptr!=0, *ptr gets updated str */
long strtol(str, ptr, b) char *str, **ptr; {
	long n = 0;
	char *s, sign = '+';
	int d, overflow = 0;

	if (ptr)
		*ptr = str;
	if (b < 0 || b == 1 || b > 36)
		return 0;
	while (*str==' '||*str=='\f'||*str=='\n'||*str=='\r'||*str=='\t'||*str=='\v')
		str++;
	if (*str == '-' || *str == '+')
		sign = *str++;
	if (b == 0)
		if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
			b = 16;
			str += 2;
		} else if (str[0] == '0')
			b = 8;
		else
			b = 10;
	for (s = str; *str; str++) {
		if (*str >= '0' && *str <= '9')
			d = *str - '0';
		else if (*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z')
			d = (*str&~040) - 'A' + 10;
		else
			break;
		if (d >= b)
			break;
		if (n < (LONG_MIN + d)/b)
			overflow = 1;
		n = b*n - d;
	}
	if (s == str)
		return 0;
	if (ptr)
		*ptr = str;
	if (overflow || (sign == '+' && n == LONG_MIN)) {
		errno = ERANGE;
		return sign == '+' ? LONG_MAX : LONG_MIN;
	}
	return sign == '+' ? -n : n;
}
#endif

static Interface naiveVAX = {
	"naiveVAX",
	1, 1, 0,	/* char */
	2, 2, 0,	/* short */
	4, 4, 0,	/* int */
	4, 4, 1,	/* float */
	8, 4, 1,	/* double */
	4, 4, 0,	/* T * */
	0, 1, 0,	/* struct */
	1,		/* left_to_right */
	1,		/* little_endian */
	0,		/* jump_on_return */
	0,		/* mulops_are_calls */ 
	1,		/* compl_band */
	0,		/* no_argb */
	0,		/* no_dag */
	address,
	blockbeg,
	blockend,
	defaddress,
	defconst,
	defstring,
	defsymbol,
	emit,
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
};
Interface *interfaces[] = { &naiveVAX, 0 };

