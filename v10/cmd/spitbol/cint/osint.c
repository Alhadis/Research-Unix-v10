/*
/   OSINT.C - The Main Module of the Macro SPITBOL Compiler
*/

#include "osint.h"
#include "spitblks.h"

extern char *sbrk();

/*
/    Global data areas needed by compiler.
*/

WORD	gblargc;		/*  argc from command line		*/
char	**gblargv;		/*  argv from command line		*/
char	*uarg;			/*  -u argument from command line	*/

/*
/   Compiler flags (see compiler listing for more details):
/
/   ERRORS	send errors to terminal
/   PRTICH	terminal is standard output file
/   NOLIST	suppress compilation listing
/   NOCMPS	suppress compilation statistics
/   NOEXCS	suppress execution statistics
/   LNGLST	generate long listing (WITH page ejects)
/   NOEXEC	suppress program execution
/   TRMNAL	support terminal i/o association
/   STDLST	standard listing (intermediate)
/   NOHDER	suppress spitbol compiler header
/
/   DFLT_FLAGS	reasonable defaults for UN*X environment
*/

#define	ERRORS		1
#define PRTICH		2
#define NOLIST		4
#define NOCMPS		8
#define NOEXCS		16
#define LNGLST		32
#define NOEXEC		64
#define TRMNAL		128
#define STDLST		256
#define NOHEDR		512

#define DFLT_FLAGS	(ERRORS+NOLIST+NOCMPS+NOEXCS+TRMNAL+NOHEDR)

/*
/   Information to be given to compiler
*/

WORD	dfltcase = 1;		/*  case conversion:  0 - no / 1 - yes	*/
WORD	lnsppage = 60;		/*  lines per page for listings		*/
WORD	pagewdth = 120;		/*  width of output line for listings	*/
WORD	spitflag = DFLT_FLAGS;	/*  flags to be given to compiler	*/

/*
/   Memory variables that control compiler's dynamic area and stack.
*/
WORD	meminc = 4096;			/*  # of words in memory chunk to allocate*/
WORD	memincb = 4094*4;		/*  meminc converted to bytes		*/
WORD	datawds = 262144;		/*  max size in words of data area	*/
char	*basemem;		/*  base of dynamic memory		*/
char	*topmem;		/*  current top of dynamic memory	*/
char	*maxmem;		/*  maximum top of dynamic memory	*/
WORD	maxsize = 8192;		/*  maximum size element in dyn. memory	*/

WORD	stacksiz = 2048;		/*  maximum size of stack in bytes	*/
char	*initsp;		/*  initial sp value			*/
char	*lowsp;			/*  lowest legal sp value		*/

/*
/
*/
WORD	inpcnt;			/*  number of input files		*/
char	**inpptr;		/*  pointer to input file in argv array	*/
char	*outptr;		/*  pointer to output listing file	*/

/*
/   lmodstk is set when creating a load module.  On the subsequent
/   execution of a load module, the presence of a non-zero value in
/   lmodstk determines that the execution is indeed of a load module.
*/
char	*lmodstk;

main( argc, argv )

int	argc;
char	*argv[];

{
    int		ch, i;
    char	*cp, *optnum();


/*
/   If this is a restart of this program from a load module, skip the
/   option processing and handle things.
*/
/*    if ( lmodstk )
	rstart();
*/

/*
/   Here we are for a normal startup of the compiler.  Process all 
/   command line options.  Notice that certain options modify the value
/   of the loop control variable i.
*/
    for( i = 1 ; i < argc ; i++ )
    {
	cp = argv[i];
	if ( *cp != '-' )
	{
	    if ( inpptr )
	    {
		write( 2, "Hmmm\n", 5 );
		exit();
	    }
	    inpptr = argv + i;
	    inpcnt = argc - i;
	    break;
	}

	++cp;
	while( *cp )
	    switch( ch = *cp++ )
	    {
	    /*
	    /   -a	turn on listing options except header
	    */
	    case 'a':
		spitflag &= ~(NOLIST | NOCMPS | NOEXCS);
		break;

	    /*
	    /   -c	turn on compilation statistics
	    */
	    case 'c':
		spitflag &= ~NOCMPS;
		break;

	    /*
	    /   -dnnn	set maximum size of dynamic area in words
	    */
	    case 'd':
		cp = optnum( cp, &datawds );
		break;

	    /*
	    /   -e	don't send errors to terminal
	    */
	    case 'e':
		spitflag &= ~ERRORS;
		break;

	    /*
	    /   -f	don't fold lower case to upper case
	    */
	    case 'f':
		dfltcase = 0;
		break;

	    /*
	    /   -h	print header listing
	    */
	    case 'h':
		spitflag &= ~NOHEDR;
		break;

	    /*
	    /   -iddd	set memory expansion increment
	    */
	    case 'i':
		cp = optnum( cp, &meminc );
		memincb = meminc * sizeof( WORD );
		break;

	    /*
	    /   -l	turn on compilation listing
	    */
	    case 'l':
		spitflag &= ~NOLIST;
		break;

	    /*
	    /   -mddd	set maximum size of object in dynamic area
	    */
	    case 'm':
		cp = optnum( cp, &maxsize );
		break;

	    /*
	    /   -n	suppress program execution
	    */
	    case 'n':
		spitflag |= NOEXEC;
		break;

	    /*
	    /   -o fff	set output file to fff
	    */
	    case 'o':
		outptr = argv[++i];
		if ( i == argc   ||  *outptr == '-' )
		    abort();
		break;

	    /*
	    /   -p	turn on long listing format
	    */
	    case 'p':
		spitflag |= LNGLST;
		break;

	    /*
	    /   -s	set stack size in bytes
	    */
	    case 's':
		cp = optnum( cp, &stacksiz );
		stacksiz *= sizeof( WORD );
		break;

	    /*
	    /   -u aaa	set user argument accessible via host()
	    */
	    case 'u':
		uarg = argv[++i];
		if ( i == argc )
		    abort();
		break;

	    /*
	    /   -x	print execution statistics
	    */
	    case 'x':
		spitflag &= ~NOEXCS;
		break;

	    /*
	    /   -z	turn on standard listing options
	    */
	    case 'z':
		spitflag |= STDLST;
		break;

	    /*
	    /   anything else is an error
	    */
	    default:
		write( 2, "Illegal option -", 17 );
		write( 2, cp - 1, 1 );
		write( 2, "?\n", 2 );
	    }
    }

/*
/   Switch to proper input file.
*/
    swcinp( inpptr, inpcnt );

/*
/   Switch to proper output file.
*/
    swcoup( outptr );

/*
/   Determine if standard output is a tty or not.
*/
    if ( testty() )
    {
	lnsppage = 0;
	spitflag |= PRTICH;
    }

/*
/   Set signals for execution.
*/
    setsigs();

/*
/   Allocate dynamic memory.
*/
    basemem = sbrk( memincb );
    topmem = basemem + memincb;
    maxmem = basemem + (datawds * sizeof( WORD ) );
    for( cp = basemem ; cp < topmem ; cp += sizeof( WORD ) )
	*( (WORD *) cp ) = 0;

/*
/   All compiler registers are initially zero, except for XL and XR.
*/
    SET_RA( 0 );
    SET_CP( 0 );
    SET_IA( 0 );
    SET_WA( 0 );
    SET_WB( 0 );
    SET_WC( 0 );
    SET_XR( basemem );
    SET_XL( topmem - 4 );

/*
/   Startup compiler.
*/
    startup();
}


/*
/    getnum() converts an ASCII string to an integer AND returns a pointer
/    to the character following the last valid digit.
/
/    Parameters:
/	cp	pointer to character string
/	ip	pointer to WORD receiving converted result
/    Returns:
/	Pointer to character following last valid digit in input string
/    Side Effects:
/	Modifies contents of integer pointed to by ip.
*/

char	*getnum( cp, ip )

char	*cp;
WORD	*ip;

{
    WORD	result = 0;

    while( *cp >= '0'  &&  *cp <= '9' )
	result = result * 10 + *cp++ - '0';

    *ip = result;
    return  cp;
}


/*
/   optnum() converts an ASCII string to an integer AND returns a pointer
/   to the character following the last valid digit.  optnum() is similar
/   to getnum() excpet that optnum accepts a trailing 'k' to indicate that
/   the value should be scaled in units of 1024.
/
/   Parameters:
/	cp	pointer to character string
/	ip	pointer to WORD receiving converted result
/   Returns:
/	Pointer to character following last valid digit in input string,
/	including a trailing k.
/   Side Effects:
/	Modifies contents of integer pointed to by ip.
*/

char	*optnum( cp, ip )

char	*cp;
WORD	*ip;

{
    cp = getnum( cp, ip );

    if ( *cp == 'k' )
    {
	++cp;
	*ip <<= 10;
    }

    return  cp;
}
