#define NONE	-1
#define MANY	128
#define	NPROG	32768

#define DEF_CH	   3		/* default number of channels    */

#define PICO	1
#define DUMP	2
#define RUNCODE	3

struct SRC {
	int sx, sy, ex, ey;	/* window into image */
	int ll, nl;		/* length and nr of scanlines in file */
	int ox, oy;		/* wander point, only used in 0 and 1 */
	unsigned char incore;	/* 1: mem allocated, 2: pixels read */
	unsigned char nchan;	/* #channels allocated in mem    */
	unsigned char used;	/* was this picture referenced   */
	unsigned char type;	/* PICO or DUMP file format      */
	char fname[128];	/* unix file name                */
	int fd;			/* file descriptor		 */
	int sop;		/* start of picture in file      */
	unsigned char *pixred;	/* pixel values as 1-dim arrays  */
	unsigned char *pixgrn;
	unsigned char *pixblu;
};

typedef
enum
{
	OASS,	OCOMMA,	ACOMMA,	COMP,	OADD,
	OMUL,	OXOR,	OOR,	OAND,	OBIC,
	OSUB,	OANDAND,	OOROR,	OPOW,
	ONOT,	OMINUS,	ONEG,	ODIV,
	OLT,	OLE,	OEQ,	ONE,
	OGE,	OGT,	OLSH,

	CONST,	CONSTB,	VAR,	REG,
	DEREFB, DEREFS,	DEREFL, CONDI,

	OISUB,	OIDIV,

	LABL,	GOTO,

	OCALL, CCALL,	OARG,	ORETURN,

	DIVV,	MODU,
} Ntype;

typedef
struct	Node
{
	Ntype	type;
	struct	Node	*left, *right;
	union
	{	long		Uarg;
		struct Node *	Uother;
	} u;
} Node;

typedef
struct	SNode
{
	Node *n, *x, *y;
	char i;
} SNode;

struct AR
{	int	sz;		/* dimension  */
	long	*base;		/* array base */
};

struct FU
{	char	usednew;	/* did it access new==tmp? */
	char	usedold;	/* did it access old? */
	char	*fct;		/* compiled function code  */
	char	nrparams;	/* number of formal params */
};

struct BL
{	char	usednew;
	int	(*faddr)();
	char	nrparams;
};

typedef
struct Symbol
{	char	*name;
	short	type;		/* array, function, variable, keyword or name */
	int 	depth;		/* nesting level of declaration */
	union {
		struct AR *ar;	/* array    */
		struct FU *fu;	/* function */
		struct BL *bl;	/* builtin  */
		long val;	/* variable */
	} u;
	struct	Symbol	*next;
} Symbol;

#define	arg	u.Uarg
#define	other	u.Uother
#define NR(n)	new(CONST, Z, Z, n)
#define VR(n)	new(VAR,   Z, Z, n)
#define RL(A,B)	new(DEREFL, VR(A), B, Z)
#define	Z	((Node *)0)

#define Prompt	if (INPUT == stdin)\
		{	fprintf(stderr, "%d: ", linenumber);\
			fflush(stderr);\
		}
#define Old		(&src[CUROLD])
#define Scratch		(&src[CURSCRATCH])
#define oldred		(src[CUROLD].pixred)
#define scratchred	(src[CURSCRATCH].pixred)
#define oldgrn		(src[CUROLD].pixgrn)
#define scratchgrn	(src[CURSCRATCH].pixgrn)
#define oldblu		(src[CUROLD].pixblu)
#define scratchblu	(src[CURSCRATCH].pixblu)

#define snore()		{      if (frameb) fbsnore();   \
			  else if (metheus) om_open(whichdevice); \
			}
#define getscreen(m)	{      if (frameb) fbgetitall(m);   \
			  else if (metheus) metgetitall(m); \
			  setscratch(Old, Scratch); }
#define putscreen(m)	{      if (frameb)  fbputitall(m);  \
			  else if (metheus) metputitall(m); }
#define upit()		{ if (frameb) fbup();   }
#define downit()	{ if (frameb) fbdown(); }

#define udone		(lastguy == '\n')
#define Used		usedold = usednew = 1

#define NLOOP	1	/* execute once */
#define BLOOP	2	/* default loop on x, y, and i=ll*y+x */
#define SLOOP	3	/* loop on 0<=i<=255 */

#define RESIDENT	{ Old->incore = Scratch->incore = 2; }
#define ABSENT		{ Old->incore = Scratch->incore = 1; }

Node	*new(), *Index(), *sumchans(), *cast(), *weird();
Node	*notnew(), *divide(), *modulo(), *splatter();
Node	*newloop(), *newhile(), *newuntil();
Node	*threechans(), *ncopy(), *par();
Node	*mapred(), *mapgrn(), *mapblu(), *mapall();
SNode	*nsup(), *super();
char	*Emalloc(), *strcpy(), *strncpy(), *mktemp(), *strcat();
Symbol	*lookup(), *newsym();

#define min(a, b)	((a<b)?(a):(b))
#define max(a, b)	((a>b)?(a):(b))
