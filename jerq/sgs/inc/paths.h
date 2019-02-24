/*
 *	static char *ID_pathsh = "@(#) paths.h: 1.7 4/24/83";
 *
 *
 *	Pathnames for m32
 */

/*
 *	Directory containing libraries and executable files
 *	(e.g. assembler pass 1)
 */
#define LIBDIR	"/lib"
#define LLIBDIR1 "/lib/../usr/lib"
#define NDELDIRS 2

/*
 *	Directory containing executable ("bin") files
 */
#define BINDIR	"/bin"

/*
 *	Directory containing include ("header") files for building tools
 */
#define INCDIR	"/tmp"

/*
 *	Directory for "temp"  files
 */
#define TMPDIR	"/tmp"

/*
 *	Default name of output object file
 */
#define A_OUT	"a.out"

/*
 *	The following pathnames will be used by the "cc" command
 *
 *	m32 cross compiler
 */
#define CPP	"/lib/cpp"
/*
 *	Directory containing include ("header") files for users' use
 */
#define INCLDIR	"-I/tmp"
#define COMP	"/lib/comp"
#define C0	"/lib/front"
#define C1	"/lib/back"
#define OPTIM	"/lib/optim"
/*
 *	m32 cross assembler
 */
#define AS	"/bin/as"
#define AS1	"/lib/as1"	/* assembler pass 1 */
#define AS2	"/lib/as2"	/* assembler pass 2 */
#define M4	"/usr/bin/m4"			/* macro preprocessor */
#define CM4DEFS	"/lib/cm4defs"	/* C interface macros */
#define CM4TVDEFS "/lib/cm4tvdefs"	/* C macros with 'tv' call */
/*
 *	m32 link editor
 */
#define LD	"/bin/ld"
#define LD2	"/lib/ld2"	/* link editor pass 2 */
#define CRT0	"/lib/crt0.o"	/* C run time startoff */
#define MCRT0	"/lib/mcrt0.o"	/* Profile run time startoff */
#define FCRT0	"/lib/fcrt0.o"	/* Float Pt. run time startoff */
#define FMCRT0	"/lib/fmcrt0.o"	/* Profiled Float Pt. run time startoff */
#define TVCRT0	"/lib/tvcrt0.o"	/* C run time startoff for tv */

/*
 *	Test versions of comp, optim, as, ld - for developers only
 */
#define	NCOMP	""
#define NOPTIM	""
#define NAS	""
#define NAS1	""
#define NAS2	""
#define NLD	""
#define NLD2	""

/*
 *	Backup versions of comp, optim, as, ld - for developers only
 */
#define OCOMP	"/lib/comp.back"
#define OC0	"/lib/front.back"
#define OC1	"/lib/back.back"
#define OOPTIM	"/lib/optim.back"
#define OAS	"/bin/as.back"
#define OAS1	"/lib/as1.back"
#define OAS2	"/lib/as2.back"
#define OLD	"/bin/ld.back"
#define OLD2	"/bin/ld2.back"
