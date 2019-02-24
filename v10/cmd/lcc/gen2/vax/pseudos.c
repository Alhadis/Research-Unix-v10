/* C compiler: VAX code generator */

#include "c.h"

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

#ifdef vax
#ifdef V9
dclproto(static void stabblock,(int, int, Symbol*));
dclproto(static void stabend,(Coordinate *, Symbol, Coordinate **, Symbol *, Symbol *));
dclproto(static void stabfend,(Symbol, int));
dclproto(static void stabinit,(char *, int, char *[]));
dclproto(static void stabline,(Coordinate *));
dclproto(static void stabsym,(Symbol));
dclproto(static void stabtype,(Symbol));
#else
#include <stab.h>
#include "stabbsd.c"
#define stabend 0
#endif
#endif
	
dclproto(static void initgen,(void));
dclproto(static void final,(Node));

#ifdef GEN4
dclproto(static unsigned emit2,(Node,Node[],short[]));
dclproto(static void target,(Node));
#endif

Interface vaxInterface = {
	"vax",
	1, 1, 0,	/* char */
	2, 2, 0,	/* short */
	4, 4, 0,	/* int */
	4, 4, 1,	/* float */
	8, 4, 1,	/* double */
	4, 4, 0,	/* T * */
	0, 1, 0,	/* struct */
	0,		/* left_to_right */
	1,		/* little_endian */
	0,		/* jump_on_return */
	0,		/* mulops_are_calls */
	1,		/* compl_band */
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
#ifdef vax
	stabblock, stabend, stabfend, stabinit, stabline, stabsym, stabtype,
#else
	0, 0, 0, 0, 0, 0, 0,
#endif
	{".globl %s\n", 0, 0, {".text", ".data", ".data", ".text 1", ".data 2"},
	doarg, docall, 0, 0, 0,
	final, 0, emitspecial,
#ifdef GEN4
	vax_kids, vax_ntname, vax_nts, vax_rule, vax_state, vax_string, vax_cost,
	rules, emit2, target
#endif
	}
};

static Symbol d0, f0, r0, r1;

/* address - initialize q for addressing expression p+n */
static void address(q, p, n) Symbol q, p; {
	q->x.offset = p->x.offset + n;
	if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN)
		q->x.name = stringf("%s%s%d", p->x.name, n >= 0 ? "+" : "", n);
	else
		q->x.name = stringd(q->x.offset);
}

static void defaddress(p) Symbol p; {
	print(".long %s\n", p->x.name);
}

/* defconst - define a constant */
static void defconst(ty, v) Value v; {
	switch (ty) {
	case C: print(".byte %d\n",   v.uc); break;
	case S: print(".word %d\n",   v.ss); break;
	case I: print(".long %d\n",   v.i ); break;
	case U: print(".long 0x%x\n", v.u ); break;
	case P: print(".long 0x%x\n", v.p ); break;
	case F:
		if (v.f == 0.0)
			print(".long 0x0\n");
		else {
			struct real r;
			r = decode(F, v.f);
			r.exp = (r.exp + 129)&0xff;
			r.msb &= ~0x1ff;
			print(".long 0x%x\n", (r.msb<<7) | (r.sign<<15) | (r.exp<<7) | (r.msb>>25));
		}
		break;
	case D:
		if (v.d == 0.0)
			print(".long 0x0,0x0\n");
		else {
			unsigned u;
			struct real r;
			r = decode(D, v.d);
			r.exp = (r.exp + 129)&0xff;
			u = r.msb&~0x1ff;
			print(".long 0x%x,", (u<<7) | (r.sign<<15) | (r.exp<<7) | (u>>25));
			u = (r.msb<<23) | (r.lsb>>9);
			print("0x%x\n", (u<<16) | (u>>16));
		}
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

	initfunc(n, 4, 0);
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
	savemask[0] = usedmask[0]&eemask[0];
	savebase[0] = 20;
	print(".align 1\n%s:.word 0x%x\n", p->x.name, savemask[0]);
	if (maxoffset > 0)
		print("subl2 $%d,sp\n", maxoffset);
	if (isstruct(freturn(p->type)))
		print("movl r1,-4(fp)\n");
	if (pflag) {
		int lab = genlabel(1);
		print("movab L%d,r0\njsb mcount\n", lab);
		print(".data\n.align 2\nL%d: .long 0\n.text\n", lab);
	}
	emitcode();
	if (glevel > 1)
		print("ret\n");
}

/* global - global id */
static void global(p) Symbol p; {
	assert(p->u.seg);
	if (p->u.seg == BSS && (p->sclass == STATIC || Aflag >= 2))
		print(".lcomm %s,", p->x.name);
	else if (p->u.seg == BSS)
		print(".comm %s,", p->x.name);
	else {
		if (p->type->align > 1)
			print(".align %c; ", "..1.2...3"[p->type->align]);
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
	nregsets = 1;
	eemask[0] = 0xfc0;
	tmask[0] = 0x03f;
	vmask[0] = 0xfc0;
	parseflags(argc, argv);
	initgen();
	d0	= mkregs("%d", 0,  0, 2, 3, 0, D, 0);
	r0	= mkregs("%d", 0,  0, 1, 1, 0, I, 0);
	f0	= mkregs("%d", 0,  0, 1, 1, 0, F, 0);
	r1	= mkregs("%d", 1,  1, 1, 1, 0, I, 0);
	rmap[D] = mkregs("%d", 0, 10, 2, 3, 0, D, 0);
	rmap[F] = mkregs("%d", 0, 11, 1, 1, 0, F, 0);
	rmap[I]	= mkregs("%d", 0, 11, 1, 1, 0, I, 0);
	rmap[C] = rmap[S] = rmap[P] = rmap[U] = rmap[I];
}

static void segment(n) {
	setseg(n);
}

static void space(n) {
	spacen(n, ".space");
}

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

#ifdef vax
#ifdef V9
#include <stab.h>
static char *currentfile;	/* current file name */

dclproto(static void emittype,(int desc, int s, Type ty, char *str));
dclproto(extern long time,(long *));

/* emittype - emit p's type */
static void emittype(desc, s, ty, str) Type ty; char *str; {
	ty = unqual(ty);
	switch (ty->op) {
	case CHAR: desc |= (ty == unsignedchar ? 12 : 2); break;
	case SHORT: desc |= (ty == unsignedshort ? 13 : 3); break;
	case INT: desc |= 4; break;
	case UNSIGNED: desc |= 14; break;
	case FLOAT: desc |= 6; break;
	case DOUBLE: desc |= 7; break;
	case STRUCT: case UNION: case ENUM:
		if (ty->op == STRUCT)
			desc |= 8;
		else if (ty->op == UNION)
			desc |= 9;
		else
			desc |= 10;
		print("0x%x,%s\n", desc, str);
		if (glevel > 1)
			if (*ty->u.sym->name >= '0' && *ty->u.sym->name <= '9')
				print(".stabs \"%s$%d.%s\",0x%x,0,0,0\n", ty->u.sym->src.file,
					ty->u.sym->src.y, ty->u.sym->name, N_TYID);
			else
				print(".stabs \"%s\",0x%x,0,0,0\n", ty->u.sym->name, N_TYID);
		return;
	case ARRAY:
		emittype(desc | (3<<s), s + 2, ty->type, str);
		if (glevel > 1)
			print(".stabn 0x%x,0,0,%d\n", N_DIM, ty->size/ty->type->size);
		return;
	case POINTER:
		emittype(desc | (1<<s), s + 2, ty->type, str);
		return;
	case FUNCTION:
		emittype(desc | (2<<s), s + 2, ty->type, str);
		return;
	case VOID: desc |= 16; break;
	default: break;
	}
	print("0x%x,%s\n", desc, str);
}

/* stabblock - output a stab entry for '{' or '}' at level lev */
static void stabblock(brace, lev, p) Symbol *p; {
	if (brace == '}')
		while (*p)
			stabsym(*p++);
	print(".stabd 0x%x,0,%d\n", brace == '{' ? N_LBRAC : N_RBRAC, lev);
}

/* stabend - finalize stab output */
static void stabend(cp, p, cpp, sp, syms) Coordinate *cp; Symbol p, *sp, *syms; Coordinate **cpp; {
	if (glevel && cp->file && *cp->file) {
		segment(CODE);
		print("Lend:.stabs \"%s\",0x%x,0,%d,Lend\n", cp->file, N_ESO, cp->y);
	}
}

/* stabfend - end of function p */
static void stabfend(p, line) Symbol p; {
	int lab;

	if (glevel <= 1)
		return;
	lab = genlabel(1);
	print(".stabn 0x%x,0,%d,L%d\n", N_RFUN, line, p->u.f.label);
	print("L%d:.stabs \"%s\",0x%x,0,%d,L%d\n", lab,
		p->name, N_EFUN, line, lab);
}

/* stabinit - initialize stab output */
static void stabinit(file, argc, argv) char *file, *argv[]; {
	if (file && *file) {
		segment(CODE);
		print("Ltext:.stabs \"%s\",0x%x,0,0,Ltext\n", file, N_SO);
		print(".stabs \"rcc\",0x%x,0,17665,%d\n", N_VER, time(0));
		currentfile = file;
	}
}

/* stabline - emit stab entry for source coordinate *cp */
static void stabline(cp) Coordinate *cp; {
	if (glevel <= 1)
		return;
	if (cp->file && cp->file != currentfile) {
		int lab = genlabel(1);
		print("L%d: .stabs \"%s\",0x%x,0,%d,L%d\n", lab,
				cp->file, N_SOL, line, lab);
		currentfile = cp->file;
	}
	print(".stabd 0x%x,0,%d\n", N_SLINE, cp->y);
}

/* stabsym - output a stab entry for symbol p */
static void stabsym(p) Symbol p; {
	char buf[100], *adr = p->x.name;

	if (p->generated || p->computed)
		return;
	print(".stabs \"%s\",", p->name);
	if (isfunc(p->type)) {
		print("0x%x,0,0x%x,_%s\n", N_FUN, p->u.f.pt[0].y, p->name);
		print(".stabs \"%s\",0x%x,0,", p->name,
			p->sclass == STATIC ? N_STFUN : N_GSYM);
		adr = "0";
	} else if (p->sclass == AUTO && p->scope == GLOBAL || p->sclass == EXTERN) {
		print("0x%x,0,", N_GSYM);
		adr = "0";
	} else if (p->sclass == STATIC)
		print("0x%x,0,", p->u.seg == BSS ? N_LCSYM : N_STSYM);
	else if (p->scope == PARAM)
		print("0x%x,0,", p->sclass == REGISTER ? N_RSYM : N_PSYM);
	else if (p->scope >= LOCAL) {
		print("0x%x,0,", p->sclass == REGISTER ? N_RSYM : N_LSYM);
		if (p->sclass != REGISTER)
			adr = p->x.name + 1;	/* skip - sign */
	} else
		assert(0);
	emittype(0, 5, p->type, adr);
}

/* stabtype - output a stab entry for type name, ty */
static void stabtype(p) Symbol p; {
	char *name = p->name;

	if (p->sclass || p->type == 0 || glevel <= 1)
		return;
	if (*p->name >= '0' && *p->name <= '9')
		name = stringf("%s$%d.%s", p->src.file, p->src.y, p->name);
	switch (p->type->op) {
	case STRUCT: case UNION: {
		Field q;
		print(".stabs \"%s\",0x%x,0,%d,0\n", name, N_BSTR,
			p->type->op == STRUCT ? 8 : 9);
		for (q = fieldlist(p->type); q; q = q->link) {
			print(".stabs \"%s\",", q->name ? q->name : "");
			if (q->lsb)
				print("0x%x,0,0x%x,%d\n", N_SFLD,
					fieldsize(q)<<5|(q->type == inttype ? 4 : 14),
					8*q->offset + fieldright(q));
			else {
				print("0x%x,0,", N_SSYM);
				emittype(0, 5, q->type, stringd(q->offset));
			}
		}
		print(".stabs \"%s\",0x%x,0,%d,%d\n", name, N_ESTR,
			p->type->op == STRUCT ? 8 : 9, p->type->size);
		break;
		}
	case ENUM: {
		Symbol *q;
		print(".stabs \"%s\",0x%x,0,10,0\n", name, N_BSTR);
		for (q = p->u.idlist; *q; q++)
			print(".stabs \"%s\",0x%x,0,11,%d\n", (*q)->name,
				N_SSYM, (*q)->u.value);
		print(".stabs \"%s\",0x%x,0,10,%d\n", name, N_ESTR,
			p->type->size);
		break;
		}
	}
}
#endif
#endif
