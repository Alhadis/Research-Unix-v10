/* C compiler: UNIX code generator
 *
 * Outputs `stab' entries for the UNIX dbx debugger,
 * if -g is specified.
 * -DV9 selects 9th edition stab format.
 */
#include "c.h"

#ifdef vax
#include </usr/include/stab.h>
#ifndef V9
#include <stabbsd.c>
#else
int glevel = 1;
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
void stabblock(brace, lev, p) Symbol *p; {
	if (brace == '}')
		while (*p)
			stabsym(*p++);
	print(".stabd 0x%x,0,%d\n", brace == '{' ? N_LBRAC : N_RBRAC, lev);
}

/* stabend - finalize stab output */
void stabend(cp, p, cpp, sp, syms) Coordinate *cp; Symbol p, *sp, *syms; Coordinate **cpp; {
	&p; &cpp; &sp; &syms;
	if (glevel == 0)
		return;
	if (cp->file && *cp->file) {
		segment(CODE);
		print("Lend:.stabs \"%s\",0x%x,0,%d,Lend\n", cp->file, N_ESO, cp->y);
	}
}

/* stabfend - end of function p */
void stabfend(p, line) Symbol p; {
	int lab;

	if (glevel <= 1)
		return;
	lab = genlabel(1);
	print(".stabn 0x%x,0,%d,L%d\n", N_RFUN, line, p->u.f.label);
	print("L%d:.stabs \"%s\",0x%x,0,%d,L%d\n", lab,
		p->name, N_EFUN, line, lab);
}

/* stabinit - initialize stab output */
void stabinit(file, argc, argv) char *file, *argv[]; {
	if (glevel == 0)
		return;
	if (file && *file) {
		segment(CODE);
		print("Ltext:.stabs \"%s\",0x%x,0,0,Ltext\n", file, N_SO);
		print(".stabs \"rcc\",0x%x,0,17665,%d\n", N_VER, time(0));
		currentfile = file;
	}
}

/* stabline - emit stab entry for source coordinate *cp */
void stabline(cp) Coordinate *cp; {
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
void stabsym(p) Symbol p; {
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
		print("0x%x,0,", isregvar(p) ? N_RSYM : N_PSYM);
	else if (p->scope >= LOCAL) {
		print("0x%x,0,", isregvar(p) ? N_RSYM : N_LSYM);
		if (!isregvar(p))
			adr = p->x.name + 1;	/* skip - sign */
	} else
		assert(0);
	emittype(0, 5, p->type, adr);
}

/* stabtype - output a stab entry for type name, ty */
void stabtype(p) Symbol p; {
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
			if (q->to)
				print("0x%x,0,0x%x,%d\n", N_SFLD,
					fieldsize(q)<<5|(q->type == inttype ? 4 : 14),
					8*q->offset + q->from);
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
#else
#include "stabstub.c"
#endif
