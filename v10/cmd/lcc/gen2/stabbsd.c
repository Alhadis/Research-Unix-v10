/* C compiler: UNIX code generator
 *
 * Outputs `stab' entries for the UNIX dbx debugger,
 * if -g is specified.
 */

static char *currentfile;	/* current file name */
static int ntypes;

dclproto(static void stabblock,(int, int, Symbol*));
dclproto(static void stabfend,(Symbol, int));
dclproto(static void stabinit,(char *, int, char *[]));
dclproto(static void stabline,(Coordinate *));
dclproto(static void stabsym,(Symbol));
dclproto(static void stabtype,(Symbol));
	
dclproto(static void asgncode,(Type, int));
dclproto(static void dbxout,(Type));
dclproto(static int dbxtype,(Type));
dclproto(static int emittype,(Type, int, int));

/* asgncode - assign type code to ty */
static void asgncode(ty, lev) Type ty; {
	if (ty->x.marked || ty->x.typeno)
		return;
/*	fprint(2, "asgncode(`%t',%d) ty->x.typeno=0x%x\n", ty, lev, ty->x.typeno); */
	ty->x.marked = 1;
	switch (ty->op) {
	case VOLATILE: case CONST: case VOLATILE+CONST:
		asgncode(ty->type, lev);
		ty->x.typeno = ty->type->x.typeno;
		break;
	case POINTER: case FUNCTION: case ARRAY:
		asgncode(ty->type, lev + 1);
		/* fall thru */
	case VOID: case CHAR: case SHORT: case INT: case UNSIGNED:
	case FLOAT: case DOUBLE:
		break;
	case STRUCT: case UNION: {
		Field p;
		for (p = fieldlist(ty); p; p = p->link)
			asgncode(p->type, lev + 1);
		/* fall thru */
	case ENUM:
		if (ty->x.typeno == 0) {
			ty->x.typeno = ++ntypes;
/*			fprint(2,"`%t'=%d in asgncode\n", ty, ntypes); */
		}
		if (lev > 0 && (*ty->u.sym->name < '0' || *ty->u.sym->name > '9'))
			dbxout(ty);
		break;
		}
	default:
		assert(0);
	}
}

/* dbxout - output .stabs entry for type ty */
static void dbxout(ty) Type ty; {
	ty = unqual(ty);
	if (!ty->x.printed) {
		int col = 0;
		print(".stabs \""), col += 8;
		if (ty->u.sym && !(isfunc(ty) || isarray(ty) || isptr(ty)))
			print("%s", ty->u.sym->name), col += strlen(ty->u.sym->name);
		print(":%c", isstruct(ty) || isenum(ty) ? 'T' : 't'), col += 2;
		emittype(ty, 0, col);
		print("\",%d,0,0,0\n", N_LSYM);
	}
}

/* dbxtype - emit a stabs entry for type ty, return type code */
static int dbxtype(ty) Type ty; {
	asgncode(ty, 0);
	dbxout(ty);
	return ty->x.typeno;
}

/*
 * emittype - emit ty's type number, emitting its definition if necessary.
 * Returns the output column number after emission; col is the approximate
 * output column before emission and is used to emit continuation lines for long
 * struct, union, and enum types. Continuations are not emitted for other types,
 * even if the definition is long. lev is the depth of calls to emittype.
 */
static int emittype(ty, lev, col) Type ty; {
	int tc = ty->x.typeno;

	if (isconst(ty) || isvolatile(ty)) {
		col = emittype(ty->type, lev, col);
		ty->x.typeno = ty->type->x.typeno;
		ty->x.printed = 1;
		return col;
	}
	if (tc == 0) {
		ty->x.typeno = tc = ++ntypes;
/*		fprint(2,"`%t'=%d\n", ty, tc); */
	}
	print("%d", tc), col += 3;
	if (ty->x.printed)
		return col;
	ty->x.printed = 1;
	switch (ty->op) {
	case VOID:	/* void is defined as itself */
		print("=%d", tc), col += 1+3;
		break;
	case CHAR:	/* unsigned char is a subrange of int */
		if (ty == unsignedchar)
			print("=r1;0;255;"), col += 10;
		else	/* following pcc, char is a subrange of itself */
			print("=r%d;-128;127;", tc), col += 2+3+10;
		break;
	case SHORT:	/* short is a subrange of int */
		if (ty == unsignedshort)
			print("=r1;0;65535;"), col += 12;
		else	/* signed */
			print("=r1;-32768;32767;"), col += 17;
		break;
	case INT:	/* int is a subrange of itself */
		print("=r1;%d;%d;", INT_MIN, INT_MAX), col += 4+11+1+10+1;
		break;
	case UNSIGNED:	/* unsigned is a subrange of int */
		print("=r1;0;-1;"), col += 9;
		break;
	case FLOAT: case DOUBLE:	/* float, double get sizes instead of ranges */
		print("=r1;%d;0;", ty->size), col += 4+1+3;
		break;
	case POINTER:
		print("=*"), col += 2;
		col = emittype(ty->type, lev + 1, col);
		break;
	case FUNCTION:
		print("=f"), col += 2;
		col = emittype(ty->type, lev + 1, col);
		break;
	case ARRAY:	/* array includes subscript as an int range */
		if (ty->size && ty->type->size)
			print("=ar1;0;%d;", ty->size/ty->type->size - 1), col += 7+3+1;
		else
			print("=ar1;0;-1;"), col += 10;
		col = emittype(ty->type, lev + 1, col);
		break;
	case STRUCT: case UNION: {
		Field p;
#ifdef sun
		if (!ty->u.sym->defined) {
			print("=x%c%s:", ty->op == STRUCT ? 's' : 'u', ty->u.sym->name);
			col += 2+1+strlen(ty->u.sym->name)+1;
			break;
		}
#endif
		if (lev > 0 && (*ty->u.sym->name < '0' || *ty->u.sym->name > '9')) {
			ty->x.printed = 0;
			break;
		}
		print("=%c%d", ty->op == STRUCT ? 's' : 'u', ty->size), col += 1+1+3;
		for (p = fieldlist(ty); p; p = p->link) {
			if (p->name)
				print("%s:", p->name), col += strlen(p->name)+1;
			else
				print(":"), col += 1;
			col = emittype(p->type, lev + 1, col);
			if (p->lsb)
				print(",%d,%d;", 8*p->offset +
					(IR->little_endian ? fieldright(p) : fieldleft(p)), 
					fieldsize(p));
			else
				print(",%d,%d;", 8*p->offset, 8*p->type->size);
			col += 1+3+1+3+1;	/* accounts for ,%d,%d; */
#if mc68020 || mc68030 || sparc
			if (col >= 80 && p->link) {
				print("\\\\\",%d,0,0,0\n.stabs \"", N_LSYM);
				col = 8;
			}
#endif
		}
		print(";"), col += 1;
		break;
		}
	case ENUM: {
		Symbol *p;
		if (lev > 0 && (*ty->u.sym->name < '0' || *ty->u.sym->name > '9')) {
			ty->x.printed = 0;
			break;
		}
		print("=e"), col += 2;
		for (p = ty->u.sym->u.idlist; *p; p++) {
			print("%s:%d,", (*p)->name, (*p)->u.value), col += strlen((*p)->name)+3;
#if mc68020 || mc68030 || sparc
			if (col >= 80 && p[1]) {
				print("\\\\\",%d,0,0,0\n.stabs \"", N_LSYM);
				col = 8;
			}
#endif
		}
		print(";"), col += 1;
		break;
		}
	default:
		assert(0);
	}
	return col;
}

/* stabblock - output a stab entry for '{' or '}' at level lev */
static void stabblock(brace, lev, p) Symbol *p; {
	if (brace == '{')
		while (*p)
			stabsym(*p++);
	print(".stabd 0x%x,0,%d\n", brace == '{' ? N_LBRAC : N_RBRAC, lev);
}

/* stabfend - end of function p */
static void stabfend(p, line) Symbol p; {}

/* stabinit - initialize stab output */
static void stabinit(file, argc, argv) char *file, *argv[]; {
	dclproto(typedef void (*Closure),(Symbol, Generic));

	if (file && *file) {
		(*IR->segment)(CODE);
		print("Ltext:.stabs \"%s\",0x%x,0,0,Ltext\n", file, N_SO);
		currentfile = file;
	}
	stabtype(inttype->u.sym);
	stabtype(chartype->u.sym);
	foreach(types, 0, (Closure)stabtype, (Generic)0);
	foreach(types, GLOBAL, (Closure)stabtype, (Generic)0);
}

/* stabline - emit stab entry for source coordinate *cp */
static void stabline(cp) Coordinate *cp; {
	if (cp->file && cp->file != currentfile) {
		int lab = genlabel(1);
		print("L%d: .stabs \"%s\",0x%x,0,0,L%d\n", lab,
				cp->file, N_SOL, lab);
		currentfile = cp->file;
	}
	print(".stabd 0x%x,0,%d\n", N_SLINE, cp->y);
}

/* stabsym - output a stab entry for symbol p */
static void stabsym(p) Symbol p; {
	int code, tc, sz = p->type->size;

	if (p->generated || p->computed)
		return;
	if (isfunc(p->type)) {
		print(".stabs \"%s:%c%d\",%d,0,0,%s\n", p->name,
			p->sclass == STATIC ? 'f' : 'F', dbxtype(freturn(p->type)),
			N_FUN, p->x.name);
		return;
	}
	if (IR->no_argb && p->scope == PARAM && p->structarg) {
		assert(isptr(p->type) && isstruct(p->type->type));
		tc = dbxtype(p->type->type);
		sz = p->type->type->size;
	} else
		tc = dbxtype(p->type);
	if (p->sclass == AUTO && p->scope == GLOBAL || p->sclass == EXTERN) {
		print(".stabs \"%s:G", p->name);
		code = N_GSYM;
	} else if (p->sclass == STATIC) {
		print(".stabs \"%s:%c%d\",%d,0,0,%s\n", p->name, p->scope == GLOBAL ? 'S' : 'V',
			tc, p->u.seg == BSS ? N_LCSYM : N_STSYM, p->x.name);
		return;
	} else if (isregvar(p)) {
		print(".stabs \"%s:r%d\",%d,0,", p->name, tc, N_RSYM);
#ifdef vax
		print("0,%s\n", p->x.name);
#endif
#if mc68020 || mc68030 || NeXT
		print("0,%s%s\n", isptr(p->type) ? "8+" : "", p->x.name);
#endif
#ifdef sparc
		{ int r = p->x.regvar->x.regnode->regnum;
		switch (p->x.name[0]) {
		case 'r': break;
		case 'f': r += 32; break;
		default: assert(0);
		}
		print("%d,%d\n", sz, r);
		}
#endif
		return;
	} else if (p->scope == PARAM) {
		print(".stabs \"%s:p", p->name);
		code = N_PSYM;
	} else if (p->scope >= LOCAL) {
		print(".stabs \"%s:", p->name);
		code = N_LSYM;
	} else
		assert(0);
	print("%d\",%d,0,0,%s\n", tc, code, p->scope >= PARAM && p->sclass != EXTERN ? p->x.name : "0");
}

/* stabtype - output a stab entry for type *p */
static void stabtype(p) Symbol p; {
	if (p->type) {
		if (p->sclass == 0)
			dbxtype(p->type);
		else if (p->sclass == TYPEDEF)
			print(".stabs \"%s:t%d\",%d,0,0,0\n", p->name, dbxtype(p->type), N_LSYM);
	}
}
