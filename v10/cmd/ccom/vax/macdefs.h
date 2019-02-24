/*	VAX 	*/

# define ARGINIT 32 
# define AUTOINIT 0 
# define SZCHAR 8
# define SZINT 32
# define SZFLOAT 32
# define SZDOUBLE 64
# define SZSHORT 16
# define SZPOINT 32
# define ALCHAR 8
# define ALINT 32
# define ALFLOAT 32
# define ALDOUBLE 32
# define ALSHORT 16
# define ALPOINT 32
# define ALSTRUCT 8
# define ALSTACK 32 
# define ALINIT 32
# define NOLONG		/* map longs to ints */

	/* format for labels */
# define LABFMT "L%d"

/* automatics and temporaries are on a negative growing stack */
# define BACKAUTO
# define BACKTEMP
# define BACKTEMP

	/* bytes run from right to left */
# define RTOLBYTES
	/* characters are signed */
# define CHSIGN
	/* structures are returned in a static location */
# define STATSRET
#define SRETNAME sretname

# define szty(t) (((t)==TDOUBLE)?2:1)

	/* number of scratch registers */
# define NRGS 6
	/* use clocal */
# define CLOCAL
	/* turn on pi */
# define DEBUGSYM
	/* turn on ? */
# define ASSYCOMMENT

/* asm markers */

#define	ASM_COMMENT	"#ASM"
#define	ASM_END		"#ASMEND"

	/* decide what ops can be shortened */
# define OPBIGSZ

/* yes, we can use .comm and .lcomm */

#define ALLCOMM

/* try to make float constants single precision */

#define MYFCON(p) myfcon(p)

/* debugging hooks */

#define GDEBUG
