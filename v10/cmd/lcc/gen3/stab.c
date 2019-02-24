/* C compiler: VAX subset code generator */

#include "../c/c.h"

/* stabblock - output a stab entry for '{' or '}' at level lev */
void stabblock(brace, lev, p) Symbol p; { &brace; &lev; &p; }

/* stabend - finalize stab output */
void stabend(file, line) char *file; { &file; &line; }

/* stabfend - end of function p */
void stabfend(p, line) Symbol p; { &p; &line; }

/* stabinit - initialize stab output */
void stabinit(file) char *file; { &file; }

/* stabline - emit stab entry for line `line' in file */
void stabline(file, line) char *file; { &file; &line; }

/* stabsym - output a stab entry for symbol p */
void stabsym(p) Symbol p; { &p; }

/* stabtype - output a stab entry for type *p */
void stabtype(p) Symbol p; { &p; }
