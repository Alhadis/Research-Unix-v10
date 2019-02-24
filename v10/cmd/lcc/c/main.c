/* C compiler: main program */

#include "version.h"
#include "c.h"

Interface *IR;

int Aflag;			/* >= 0 if -A specified */
int Pflag;			/* != 0 if -P specified */
int glevel;			/* == [0-9] if -g[0-9] specified */
int xref;			/* != 0 for cross-reference data */
Symbol YYnull;			/* symbol for _YYnull if -n specified */

static char *infile;		/* input file */
static char *outfile;		/* output file */
static char *progname;		/* argv[0] */

List loci, tables;		/* current (locus,table) lists */

dclproto(static void compile,(char *));
dclproto(static int doargs,(int, char **));
dclproto(static void emitYYnull,(void));
dclproto(static Type ftype,(Type, Type));
dclproto(static void typestab,(Symbol, Generic));

int main(argc, argv) char *argv[]; {
	Symbol symroot = 0;		/* root of the global symbols */
	static Events z;
	int i, infd = 0, outfd = 1;

	assert(MAXKIDS >= 2);
	assert(MAXSYMS >= 2);
	progname = argv[0];
	level = GLOBAL;
	IR = interfaces[0];
	for (i = 1; i < argc; i++)
		if (strncmp(argv[i], "target=", 7) == 0) {
			int k;
			for (k = 0; interfaces[k]; k++)
				if (strcmp(&argv[i][7], interfaces[k]->target) == 0)
					break;
			if (interfaces[k])
				IR = interfaces[k];
			else
				fprint(2, "%s: unknown target `%s'\n", argv[0], &argv[i][7]);
		}
	assert(IR);
	typeInit();
	assert(inttype->size >= voidptype->size);
	argc = doargs(argc, argv);
	if (infile && *infile != '-') {
		close(infd);
		if ((infd = open(infile, 0)) < 0) {
			fprint(2, "%s: can't read %s\n", argv[0], infile);
			exit(1);
		}
	}
	if (outfile && *outfile != '-') {
		close(outfd);
		if ((outfd = creat(outfile, 0666)) < 0) {
			fprint(2, "%s: can't write %s\n", argv[0], outfile);
			exit(1);
		}
	}
	inputInit(infd);
	outputInit(outfd);
	t = gettok();
	(*IR->progbeg)(argc, argv);
	if (glevel && IR->stabinit)
		(*IR->stabinit)(firstfile, argc, argv);
	program();
	if (events.end)
		apply(events.end, (Generic)0, (Generic)0);
	events = z;
	emitYYnull();
	finalize();
	if (glevel || xref) {
		Coordinate src;
		foreach(types, GLOBAL, typestab, (Generic)&symroot);
		foreach(identifiers, GLOBAL, typestab, (Generic)&symroot);
		src.file = firstfile;
		src.x = 0;
		src.y = lineno;
		if ((glevel > 2 || xref) && IR->stabend)
			(*IR->stabend)(&src, symroot, (Coordinate **)ltoa(append((Generic)0, loci), 0),
				(Symbol *)ltoa(append((Generic)0, tables), 0),
				symbols ? (Symbol *)ltoa(symbols, 0) : (Symbol *)0);
		else if (IR->stabend)
			(*IR->stabend)(&src, 0, 0, 0, 0);
	}
	(*IR->progend)();
	outflush();
	exit(errcnt > 0);
	return 0;
}

/* compile - compile str */
static void compile(str) char *str; {
	inputstring(str);
	t = gettok();
	program();
}

/* doargs - process program arguments, removing top-half arguments from argv */
static int doargs(argc, argv) char *argv[]; {
	char *s;
	int i, j, x;
	Symbol p;

	for (i = j = 1; i < argc; i++)
		if (strcmp(argv[i], "-g") == 0)
			glevel = 2;
		else if (strncmp(argv[i], "-g", 2) == 0
		&& argv[i][2] && argv[i][2] >= '0' && argv[i][2] <= '9')
			glevel = argv[i][2] - '0';
		else if (strcmp(argv[i], "-x") == 0)
			xref++;
		else if (strcmp(argv[i], "-A") == 0)
			Aflag++;
		else if (strcmp(argv[i], "-P") == 0)
			Pflag++;
		else if (strcmp(argv[i], "-w") == 0)
			wflag++;
		else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "-C") == 0
		|| strncmp(argv[i], "-a", 2) == 0)
			bbinit(argv[i]);
		else if (strcmp(argv[i], "-n") == 0) {
			if (!YYnull) {
				YYnull = mksymbol(EXTERN, "_YYnull", ftype(voidtype, inttype));
				YYnull->sclass = STATIC;
			}
		} else if (strncmp(argv[i], "-t", 2) == 0)
			traceinit(&argv[i][2]);
		else if (strcmp(argv[i], "-v") == 0)
			fprint(2, "%s target='%s' version %d.%d\n", progname, IR->target,
				VERSION>>8, VERSION&0xff);
		else if (strncmp(argv[i], "-s", 2) == 0)
			density = strtod(&argv[i][2], (char **)0);
		else if (strncmp(argv[i], "-e", 2) == 0) {
			if ((x = strtol(&argv[i][2], (char **)0, 0)) > 0)
				errlimit = x;
		} else if (strncmp(argv[i], "target=", 7) == 0)
			;
		else if (strcmp(argv[i], "-nodag") == 0)
			IR->no_dag = !IR->no_dag;
		else if (strcmp(argv[i], "-") == 0 || *argv[i] != '-') {
			if (infile == 0)
				infile = argv[i];
			else if (outfile == 0)
				outfile = argv[i];
			else
				argv[j++] = argv[i];
		} else {
			if (strcmp(argv[i], "-XP") == 0)
				argv[i] = "-p";
			else if (strncmp(argv[i], "-X", 2) == 0)
				*++argv[i] = '-';
			argv[j++] = argv[i];
		}
	argv[j] = 0;
	return j;
}

/* emitYYnull - compile definition for _YYnull, if referenced */
static void emitYYnull() {
	if (YYnull && YYnull->ref) {
		Aflag = 0;
		YYnull->defined = 0;
		YYnull = 0;
		compile(stringf("static char *_YYfile = \"%s\";\n", file));
		compile("static void _YYnull(int line,...) {\n\
	char buf[200];\n\
	sprintf(buf, \"null pointer dereferenced @%s:%d\\n\", _YYfile, line);\n\
	write(2, buf, strlen(buf));\n\
	abort();\n\
}\n");
	} else if (YYnull)
		YYnull->ref = 1;
}

/* ftype - return a function type for `rty function (ty,...)' */
static Type ftype(rty, ty) Type rty, ty; {
	List list = append(ty, 0);

	list = append(voidtype, list);
	return func(rty, (Type *)ltoa(list, (Generic *)alloc((length(list) + 1)*sizeof (Type))), 0);
}

/* mkstr - make a string constant */
Symbol mkstr(str) char *str; {
	Value v;
	Symbol p;

	v.p = str;
	p = constant(array(chartype, strlen(v.p) + 1, 0), v);
	if (p->u.c.loc == 0)
		p->u.c.loc = genident(STATIC, p->type, GLOBAL);
	return p;
}

/* mksymbol - make a symbol for name, install in &globals if sclass==EXTERN */
Symbol mksymbol(sclass, name, ty) char *name; Type ty; {
	Symbol p;

	if (sclass == EXTERN)
		p = install(string(name), &globals, 1);
	else {
		p = (Symbol)alloc(sizeof *p);
		BZERO(p, struct symbol);
		p->name = string(name);
		p->scope = GLOBAL;
	}
	p->sclass = sclass;
	p->type = ty;
	(*IR->defsymbol)(p);
	p->defined = 1;
	return p;
}

/* typestab - emit stab entries for p */
static void typestab(p, cl) Symbol p; Generic cl; {
	if (*(Symbol *)cl == 0 && p->sclass && p->sclass != TYPEDEF)
		*(Symbol *)cl = p;
	if ((p->sclass == TYPEDEF || p->sclass == 0) && IR->stabtype)
		(*IR->stabtype)(p);
}

struct callsite {
	char *file, *name;
	union coordinate {
		unsigned int coord;
		struct { unsigned int y:16,x:10,index:6; } le;
		struct { unsigned int index:6,x:10,y:16; } be;
	} u;
};
struct func {
	struct func *link;
	struct caller *callers;
	char *name;
	union coordinate src;
};
struct map {		/* source code map; 200 coordinates/map */
	int size;
	union coordinate u[200];
};
int npoints;		/* # of execution points if -b specified */
int ncalled = -1;	/* #times prof.out says current function was called */
static Symbol YYlink;	/* symbol for file's struct _bbdata */
static Symbol YYcounts;	/* symbol for _YYcounts if -b specified */
static List maplist;	/* list of struct map *'s */
static List filelist;	/* list of file names */
static Symbol funclist;	/* list of struct func *'s */
static Symbol afunc;	/* current function's struct func */

dclproto(static void bbcall,(Symbol, Coordinate *, Tree *));
dclproto(static void bbentry,(Symbol, Symbol));
dclproto(static void bbexit,(Symbol, Symbol, Tree));
dclproto(static int bbfile,(char *));
dclproto(static void bbfunc,(Symbol, Symbol));
dclproto(static void bbincr,(Symbol, Coordinate *, Tree *));
dclproto(static void bbvars,(Symbol));

/* bbcall - build tree to set _callsite at call site *cp, emit call site data */
static void bbcall(yycounts, cp, e) Symbol yycounts; Coordinate *cp; Tree *e; {
	static Symbol caller;
	Value v;
	union coordinate u;
	Symbol p = genident(STATIC, array(voidptype, 0, 0), GLOBAL);

	defglobal(p, LIT);
	defpointer(cp->file ? mkstr(cp->file)->u.c.loc : (Symbol)0);
	defpointer(mkstr(cfunc->name)->u.c.loc);
	if (IR->little_endian) {
		u.le.x = cp->x;
		u.le.y = cp->y;
	} else {
		u.be.x = cp->x;
		u.be.y = cp->y;
	}
	(*IR->defconst)(U, (v.u = u.coord, v));
	if (caller == 0)
		caller = mksymbol(EXTERN, "_caller", ptr(voidptype));
	*e = right(asgn(caller, idnode(p)), *e);
}

/* bbentry - return tree for `_prologue(&afunc, &YYlink)' */
static void bbentry(yylink, f) Symbol yylink, f; {
	static Symbol p;
	
	afunc = genident(STATIC, array(voidptype, 4, 0), GLOBAL);
	if (p == 0)
		p = mksymbol(EXTERN, "_prologue", ftype(inttype, voidptype));
	walk(callnode(pointer(idnode(p)), freturn(p->type),
		tree(ARG+P, ptr(unsignedtype), idnode(yylink),
		tree(ARG+P, ptr(unsignedtype), idnode(afunc), 0))), 0, 0);
}

/* bbexit - return tree for `_epilogue(&afunc)' */
static void bbexit(yylink, f, e) Symbol yylink, f; Tree e; {
	static Symbol p;
	
	if (p == 0)
		p = mksymbol(EXTERN, "_epilogue", ftype(inttype, voidptype));
	walk(callnode(pointer(idnode(p)), freturn(p->type),
		tree(ARG+P, ptr(unsignedtype), idnode(afunc), 0)), 0, 0);
}

/* bbfile - add file to list of file names, return its index */
static int bbfile(file) char *file; {
	if (file) {
		List lp;
		int i = 1;
		if (lp = filelist)
			do {
				lp = lp->link;
				if (((Symbol)lp->x)->u.c.v.p == file)
					return i;
				i++;
			} while (lp != filelist);
		filelist = append(mkstr(file), filelist);
		return i;
	}
	return 0;
}

/* bbfunc - emit function name and src coordinates */
static void bbfunc(yylink, f) Symbol yylink, f; {
	Value v;
	union coordinate u;

	defglobal(afunc, DATA);
	defpointer(funclist);
	defpointer(0);
	defpointer(mkstr(f->name)->u.c.loc);
	if (IR->little_endian) {
		u.le.x = f->u.f.pt[2].x;
		u.le.y = f->u.f.pt[2].y;
		u.le.index = bbfile(f->u.f.pt[2].file);
	} else {
		u.be.x = f->u.f.pt[2].x;
		u.be.y = f->u.f.pt[2].y;
		u.be.index = bbfile(f->u.f.pt[2].file);
	}
	(*IR->defconst)(U, (v.u = u.coord, v));
	funclist = afunc;
}

/* bbincr - build tree to increment execution point at *cp */
static void bbincr(yycounts, cp, e) Symbol yycounts; Coordinate *cp; Tree *e; {
	struct map *mp = (struct map *)maplist->x;

	/* append *cp to source map */
	if (mp->size >= sizeof mp->u/sizeof mp->u[0]) {
		mp = (struct map *)alloc(sizeof *mp);
		mp->size = 0;
		maplist = append((Generic *)mp, maplist);
	}
	if (IR->little_endian) {
		mp->u[mp->size].le.x = cp->x;
		mp->u[mp->size].le.y = cp->y;
		mp->u[mp->size++].le.index = bbfile(cp->file);
	} else {
		mp->u[mp->size].be.x = cp->x;
		mp->u[mp->size].be.y = cp->y;
		mp->u[mp->size++].be.index = bbfile(cp->file);
	}
	*e = right(incr('+', rvalue((*opnode['+'])(ADD, pointer(idnode(yycounts)),
		constnode(npoints++, inttype))), constnode(1, inttype)), *e);
}

/* bbinit - initialize basic block counting options */
void bbinit(opt) char *opt; {
	if (strncmp(opt, "-a", 2) == 0) {
		if (ncalled == -1 && process(opt[2] ? &opt[2] : "prof.out") > 0)
			ncalled = 0;
	} else if ((strcmp(opt, "-b") == 0 || strcmp(opt, "-C") == 0) && YYlink == 0) {
		YYlink = genident(STATIC, array(unsignedtype, 0, 0), GLOBAL);
		attach((Apply)bbentry, (Generic)YYlink, &events.entry);
		attach((Apply)bbexit,  (Generic)YYlink, &events.returns);
		attach((Apply)bbfunc,  (Generic)YYlink, &events.exit);
		attach((Apply)bbvars,  (Generic)YYlink, &events.end);
		if (strcmp(opt, "-b") == 0) {
			YYcounts = genident(STATIC, array(unsignedtype, 0, 0), GLOBAL);
			maplist = append((Generic)alloc(sizeof(struct map)), maplist);
			((struct map *)maplist->x)->size = 0;
			attach((Apply)bbcall, (Generic)YYcounts, &events.calls);
			attach((Apply)bbincr, (Generic)YYcounts, &events.points);
		}
	}
}

/* bbvars - emit definition for basic block counting data */
static void bbvars(yylink) Symbol yylink; {
	int i, j, n = npoints;
	Value v;
	struct map **mp;
	Symbol coords, files, *p;

	if (!YYcounts && !yylink)
		return;
	if (YYcounts) {
		if (n <= 0)
			n = 1;
		defglobal(YYcounts, BSS);
		(*IR->space)(n*YYcounts->type->type->size);
	}
	files = genident(STATIC, array(ptr(chartype), 1, 0), GLOBAL);
	defglobal(files, LIT);
	for (p = (Symbol *)ltoa(filelist, 0); *p; p++)
		defpointer((*p)->u.c.loc);
	defpointer(0);
	coords = genident(STATIC, array(unsignedtype, n, 0), GLOBAL);
	defglobal(coords, LIT);
	for (i = n, mp = (struct map **)ltoa(maplist, 0); *mp; i -= (*mp)->size, mp++)
		for (j = 0; j < (*mp)->size; j++)
			(*IR->defconst)(U, (v.u = (*mp)->u[j].coord, v));
	if (i > 0)
		(*IR->space)(i*coords->type->type->size);
	defpointer(0);
	defglobal(yylink, DATA);
	defpointer(0);
	(*IR->defconst)(U, (v.u = n, v));
	defpointer(YYcounts);
	defpointer(coords);
	defpointer(files);
	defpointer(funclist);
}

static char *fmt, *fp, *fmtend;	/* format string, current & limit pointer */
static Tree args;		/* printf arguments */
static Symbol frameno;		/* local holding frame number */

dclproto(static void appendstr,(char *));
dclproto(static void tracecall,(Symbol, Symbol));
dclproto(static void tracefinis,(Symbol));
dclproto(static void tracereturn,(Symbol, Symbol, Tree));
dclproto(static void tracevalue,(Tree, int));

/* appendstr - append str to the evolving format string, expanding it if necessary */
static void appendstr(str) char *str; {
	do
		if (fp == fmtend)
			if (fp) {
				char *s = (char *)talloc(2*(fmtend - fmt));
				strncpy(s, fmt, fmtend - fmt);
				fp = s + (fmtend - fmt);
				fmtend = s + 2*(fmtend - fmt);
				fmt = s;
			} else {
				fp = fmt = (char *)talloc(80);
				fmtend = fmt + 80;
			}
	while (*fp++ = *str++);
	fp--;
}

/* tracecall - generate code to trace entry to f */
static void tracecall(printer, f) Symbol printer, f; {
	int i;
	Symbol counter = genident(STATIC, inttype, GLOBAL);

	defglobal(counter, BSS);
	(*IR->space)(counter->type->size);
	frameno = genident(AUTO, inttype, level);
	addlocal(frameno);
	appendstr(f->name); appendstr("#");
	tracevalue(asgn(frameno, incr(INCR, idnode(counter), constnode(1, inttype))), 0);
	appendstr("(");
	for (i = 0; f->u.f.callee[i]; i++) {
		if (i)
			appendstr(",");
		appendstr(f->u.f.callee[i]->name); appendstr("=");
		tracevalue(idnode(f->u.f.callee[i]), 0);
	}
	if (variadic(f->type))
		appendstr(",...");
	appendstr(") called\n");
	tracefinis(printer);
}

/* tracefinis - complete & generate the trace call to print */
static void tracefinis(printer) Symbol printer; {
	Tree *ap;
	Symbol p;

	*fp = 0;
	p = mkstr(string(fmt));
	for (ap = &args; *ap; ap = &(*ap)->kids[1])
		;
	*ap = tree(ARG+P, ptr(chartype), pointer(idnode(p->u.c.loc)), 0);
	walk(callnode(pointer(idnode(printer)), freturn(printer->type), args), 0, 0);
	args = 0;
	fp = fmtend = 0;
}

/* traceinit - initialize for tracing */
void traceinit(print) char *print; {
	static Symbol printer;

	if (!printer) {
		printer = mksymbol(STATIC, print && *print ? print : "printf",
			ftype(voidtype, ptr(chartype)));
		printer->sclass = EXTERN;
		attach((Apply)tracecall,   (Generic)printer, &events.entry);
		attach((Apply)tracereturn, (Generic)printer, &events.returns);
	}
}

/* tracereturn - generate code to trace return e */
static void tracereturn(printer, f, e) Symbol printer, f; Tree e; {
	appendstr(f->name); appendstr("#");
	tracevalue(idnode(frameno), 0);
	appendstr(" returned");
	if (freturn(f->type) != voidtype && e) {
		appendstr(" ");
		tracevalue(e, 0);
	}
	appendstr("\n");
	tracefinis(printer);
}

/* tracevalue - append format and argument to print the value of e */
static void tracevalue(e, lev) Tree e; {
	Type ty = unqual(e->type);

	switch (ty->op) {
	case CHAR:
		appendstr("'\\x%2x'");
		break;
	case SHORT:
		if (ty == unsignedshort)
			appendstr("0x%x");
		else /* fall thru */
	case INT:
			appendstr("%d");
		break;
	case UNSIGNED:
		appendstr("0x%x");
		break;
	case FLOAT: case DOUBLE:
		appendstr("%g");
		break;
	case POINTER:
		if (unqual(ty->type) == chartype) {
			static Symbol null;
			if (null == 0)
				null = mkstr("(null)");
			tracevalue(constnode(0, unsignedtype), lev + 1);
			appendstr(" \"%s\"");
			e = condnode(e, e, pointer(idnode(null->u.c.loc)));
		} else {
			appendstr("("); appendstr(typestring(ty, "")); appendstr(")0x%x");
		}
		break;
	case STRUCT: {
		Field q;
		appendstr("("); appendstr(typestring(ty, "")); appendstr("){");
		for (q = ty->u.sym->u.s.flist; q; q = q->link) {
			appendstr(q->name); appendstr("=");
			tracevalue(field(addrof(e), q->name), lev + 1);
			if (q->link)
				appendstr(",");
		}
		appendstr("}");
		return;
		}
	case UNION:
		appendstr("("); appendstr(typestring(ty, "")); appendstr("){...}");
		return;
	case ARRAY:
		if (lev && ty->type->size > 0) {
			int i;
			e = pointer(e);
			appendstr("{");
			for (i = 0; i < ty->size/ty->type->size; i++) {
				Tree p = (*opnode['+'])(ADD, e, constnode(i, inttype));
				if (isptr(p->type) && isarray(p->type->type))
					p = retype(p, p->type->type);
				else
					p = rvalue(p);
				if (i)
					appendstr(",");
				tracevalue(p, lev + 1);
			}
			appendstr("}");
		} else
			appendstr(typestring(ty, ""));
		return;
	default:
		assert(0);
	}
	if (ty == floattype)
		e = cast(e, doubletype);
	else if ((isint(ty) || isenum(ty)) && ty->size != inttype->size)
		e = cast(e, promote(ty));
	args = tree(ARG + widen(e->type), e->type, e, args);
}
Events events;

struct entry {
	Apply func;
	Generic cl;
};

void attach(func, cl, list) Apply func; Generic cl; List *list; {
	struct entry *p = (struct entry *)alloc(sizeof *p);

	p->func = func;
	p->cl = cl;
	*list = append((Generic)p, *list);
}
void apply(list, arg1, arg2) List list; Generic arg1, arg2; {
	if (list) {
		List lp = list;
		do {
			struct entry *p = (struct entry *)lp->x;
			(*p->func)(p->cl, arg1, arg2);
			lp = lp->link;
		} while (lp != list);
	}
}
