/*	@(#) macdefs.h: 1.6 3/4/84	*/
/*	macdefs - machine dependent macros and parameters
 *
 *		Bellmac32 w/Mao/Berenbaum stack frame
 *
 *			R. J. Mascitti
 */

/* initial values for stack offsets in bits */

#define ARGINIT 0
#define AUTOINIT 0

/* sizes of types in bits */

#define SZCHAR 8
#define SZSHORT 16
#define SZINT 32
#define SZPOINT 32

#define SZFLOAT 32
#define SZDOUBLE 64

/* alignments of types in bits */

#define ALCHAR 8
#define ALSHORT 16
#define ALINT 32
#define ALPOINT 32
#define ALSTACK 32
#define ALSTRUCT 32
#define ALINIT 32
#define ALPROC	64

#define ALFLOAT 32
#define ALDOUBLE 32

#define NOLONG			/* map longs to ints */

/* format for labels (if anybody cares) */

#define LABFMT ".L%d"

/* type size in number of regs/temps */

#define szty(t) (((t) == TDOUBLE) ? 2 : 1)

/* number of scratch registers - fp r0->r2 (watch out for pairs!) */

#define NRGS 3

/* total number of registers (scratch and variables)	*/

#define TOTREGS	9

/* Define symbol so we can allocate odd register pairs */

#define ODDPAIR	1

/* params addressed with positive offsets from ap, args with negative from sp */

#undef BACKPARAM
#define BACKARGS

/* temps and autos addressed with positive offsets from fp */

#undef BACKTEMP
#undef BACKAUTO

/* bytes run left to right */

#define LTORBYTES
#undef RTOLBYTES

/* function calls and arguments */
/* don't nest function calls (where do the args go?) */

/* 	#define NONEST			  */

/* evaluate args left to right (3b style) */

#define LTORARGS

/* chars are unsigned */

#undef CHSIGN

/* structures returned in temporary location -- define magic string */

#define TMPSRET	"	movaw	T,%r2\n"

/* optimizer throws away unless %r2 */

#define AUXREG 2


/* comment indicator */

#define COMMENTSTR "#"

/* asm markers */

#define	ASM_COMMENT	"#ASM"
#define	ASM_END		"#ASMEND"

/* reflect my high costing in default load/store cost */

#define CLOAD(x) 55
#define CSTORE(x) 55
#define CTEST(x)  55

#define MYLOCCTR

#define EXIT(x) myexit(x)

/* Register number for debugging */

#define	OUTREGNO(p) (p->offset)

/* Turn on debug information */

#define	SDB

/* Enable assembly language comments */

#define	ASSYCOMMENT

/* user-level fix-up at symbol definition time */

#define FIXDEF(p)	fixdef(p)

/* support for structure debug info */

#define FIXSTRUCT(p,q)	strend(p)

/* We want bitfields all to act like integers because we want to
** use the extract/insert instructions, which must deal with
** word-aligned data.
*/

#define	FLDINTADR

#ifdef M32B
#	define IMPSWREG
#	define IMPREGAL
#endif

/* Commutable operators are symetric anyway */
#define NOCOMMUTE 1
