/* C compiler: UNIX code generator
 *
 * Dummy `stab' entries if -g is specified.
 */

/* stabblock - output a stab entry for '{' or '}' at level lev */
void stabblock(brace, lev, p) Symbol *p; { &brace; &lev; &p; }

/* stabend - finalize stab output */
void stabend(cp, p, cpp, sp, syms) Coordinate *cp; Symbol p, *sp, *syms; Coordinate **cpp; { &cp; &p; &cpp; &sp; &syms; }

/* stabfend - end of function p */
void stabfend(p, line) Symbol p; { &p; &line; }

/* stabinit - initialize stab output */
void stabinit(file, argc, argv) char *file, *argv[]; { &file; &argc; &argv; }

/* stabline - emit stab entry for source coordinate *cp */
void stabline(cp) Coordinate *cp; { &cp; }

/* stabsym - output a stab entry for symbol p */
void stabsym(p) Symbol p; { &p; }

/* stabtype - output a stab entry for type *p */
void stabtype(p) Symbol p; { &p; }
