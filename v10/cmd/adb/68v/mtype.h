/*
 * machine dependent stuff, mostly type structure
 *
 * some of these types belong to adb; some belong to the machine
 * being debugged.  (imagine adb running on one type of cpu, examining
 * a core image from another.)
 *
 * WORD and ADDR belong to adb.
 * WORD holds expression values.  it must hold an ADDR.
 * ADDR is an address in a file.  it should probably be unsigned,
 * unlike off_t.  (think of VAX kernel addresses)
 *
 * other types belong to the target machine:
 * TCHAR is a byte
 * TSHORT is a short integer
 * TLONG is a long integer
 * TADDR is a core address (probably char *)
 * TREG is whatever fits in a register
 * all of these must fit in a WORD, or things won't work right
 *
 * TFLOAT and TDOUBLE are float and double on the target;
 * they needn't fit in a WORD, though.
 *
 * the T* types are, in general, magic cookies, and shouldn't be
 * examined directly, save perhaps to compare them.  the conversions
 * wtol &c should be used first.
 *
 * printf (adb's own) prints adb integers, not target ones.  it prints
 * target floats, though.  this is probably a botch.
 */

/*
 * definitions for a VAX, debugging a 68000
 * everything (including the a.out header) is in 68000 order
 */

typedef long WORD;
typedef unsigned long ADDR;

#define	HUGE	0x7fffffff	/* enormous WORD */

typedef unsigned char TCHAR;
typedef unsigned short TSHORT;
typedef long TLONG;
typedef long TADDR;
typedef short TREG;
typedef float TFLOAT;		/* 4-byte fp11 */
typedef double TDOUBLE;		/* 8-byte fp11 */

#define	SZCHAR	sizeof(TCHAR)
#define	SZSHORT	sizeof(TSHORT)
#define	SZLONG	sizeof(TLONG)
#define	SZADDR	sizeof(TADDR)
#define	SZREG	sizeof(TREG)
#define	SZFLOAT	sizeof(TFLOAT)
#define	SZDOUBLE sizeof(TDOUBLE)

/*
 * conversions: WORD to target type
 */

#define	wtoc(x)	((TCHAR)(x))
#define	wtos(x)	((TSHORT)swab(x))
#define	wtol(x)	((TLONG)swal(x))
#define	wtoa(x)	((TADDR)swal(x))
#define	wtor(x)	((TREG)swal(x))

/*
 * conversions: target type to WORD
 */

#define	ctow(x)	((WORD)(x))
#define	stow(x)	((WORD)swab(x))
#define	ltow(x)	((WORD)swal(x))
#define	atow(x)	((WORD)swal(x))
#define	rtow(x)	((WORD)swal(x))

TCHAR cget();
TSHORT sget();
TLONG lget();
TADDR aget();
TREG rget();
long swal();
unsigned short swab();

/*
 * is this number seriously negative?
 * hack to make numbers prettier
 */

#define	REALLYNEG(n)	(((n) & 0xFFFF0000) == 0xFFFF0000)

/*
 * miscellany
 */

#define	DEFRADIX	16
#define	MAXOFF	255
#define	INCDIR	"/usr/gnot/lib/2adb"
