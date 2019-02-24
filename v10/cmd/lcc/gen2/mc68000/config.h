/* C compiler: MC68020 configuration parameters */

#undef V9

#define MC68020
#define MAXINTS 	5		/* codes/node */
#define MAXKIDS 	6		/* children/node */
#define MAXSYMS 	3		/* symbols/node */
#define	NREGSETS 	3		/* number of register sets */
#define RX		2		/* symbol holding target register */

#define	AREG	0	/* an address register */
#define	DREG	1	/* a data register */
#define FREG	2	/* a floating point register */
#define ZAP0	1

#include "gen.h"

/* type metrics: size,alignment,constants */
#define CHAR_METRICS		1,1,0
#define SHORT_METRICS		2,2,0
#define INT_METRICS		4,2,0
#define FLOAT_METRICS		4,2,1
#define DOUBLE_METRICS		8,2,1
#define POINTER_METRICS		4,2,0
#define	STRUCT_ALIGN		2

#define LEFT_TO_RIGHT		/* define for left-to-right argument evaluation */
/* #define LITTLE_ENDIAN	/* define for right-to-left bit fields */
#define JUMP_ON_RETURN   1

extern Symbol a0, a1, d0, d1234, fp0;
dclproto(extern void doarg,(Node))
dclproto(extern void docall,(Node))

#define defaddress(p) print("long %c%s\n", (p)->scope == LABELS ? 'L' : '_', (p)->x.name)
#define defstring(n,s) genascii(0, "byte", n, s)
#define emitspecial(p)
#define export(p) print("global _%s\n", (p)->x.name)
#define import(p)
#define defsymbol(p) ((p)->x.name = (p)->name)
#define space(n) spacen((n), "skip")
