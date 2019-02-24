/* C compiler: UNIX code generator
 *
 * Outputs `stab' entries for the UNIX dbx debugger,
 * if -g is specified.
 */

#include "c.h"

static char *currentfile;

/* stabblock - output a stab entry for '{' or '}' at level lev */
void stabblock(brace, lev, p) Symbol *p; { &brace; &lev; &p; }

/* stabend - finalize stab output */
void stabend(cp, p, cpp, sp, syms) Coordinate *cp; Symbol p, *sp, *syms; Coordinate **cpp; { &cp; &p; &cpp; &sp; &syms; }

/* stabfend - end of function p */
void stabfend(p, line) Symbol p; { &p; &line; }

/* stabinit - initialize stab output */
void stabinit(file, argc, argv) char *file, *argv[]; {
	if (file) {
		print(".file 2,\"%s\"\n", file);
		currentfile = file;
	}
}

/* stabline - emit stab entry for source coordinate *cp */
void stabline(cp) Coordinate *cp; {
	if (cp->file && cp->file != currentfile) {
		print(".file 2,\"%s\"\n", cp->file);
		currentfile = cp->file;
	}
	print(".loc 2,%d\n", cp->y);
}

/* stabsym - output a stab entry for symbol p */
void stabsym(p) Symbol p; {
	if (p == cfunc)
		stabline(&p->src);
}

/* stabtype - output a stab entry for type *p */
void stabtype(p) Symbol p; { &p; }
