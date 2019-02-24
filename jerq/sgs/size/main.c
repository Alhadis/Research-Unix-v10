/* UNIX HEADERS */
#include	<stdio.h>
#include	<ar.h>

/* COMMON SGS HEADERS */
#include	"filehdr.h"
#include	"ldfcn.h"

/* SGS SPECIFIC HEADER */
#include	"sgs.h"

/* SIZE HEADER */
#include	"defs.h"

/* EXTERNAL VARIABLES DEFINED */
#ifdef UNIX
int		numbase = DECIMAL;
#else
int		numbase = HEX;
#endif
LDFILE		*ldptr;
char		outbuf[BUFSIZ];

    /*
     *  main(argc, argv)
     *
     *  parses the command line
     *  opens, processes and closes each object file command line argument
     *
     *  defines:
     *      - int	numbase = HEX if the -x flag is in the command line
     *				= OCTAL if the -o flag is in the command line
     *				= DECIMAL if the -d flag is in the command line
     *      - LDFILE	*ldptr = ldopen(*filev, ldptr) for each obj file arg
     *
     *  calls:
     *      - process(filename) to print the size information in the object file
     *        filename
     *
     *  prints:
     *      - an error message if any unknown options appear on the command line
     *      - a usage message if no object file args appear on the command line
     *      - an error message if it can't open an object file
     *	      or if the object file has the wrong magic number
     *
     *  exits successfully always
     */


int
main(argc, argv)

int	argc;
char	**argv;

{
    /* UNIX FUNCTIONS CALLED */
    extern 		fprintf( ),
			sprintf( ),
			fflush( ),
    			exit( );

    /* OBJECT FILE ACCESS ROUTINES CALLED */
    extern LDFILE	*ldopen( );
    extern int		ldclose( ),
			ldaclose( ),
			ldahread( );

    /* SIZE FUNCTIONS CALLED */
    extern		process( );

    /* EXTERNAL VARIABLES USED */
    extern int		numbase;
    extern LDFILE	*ldptr;
    extern char		outbuf[ ];

    int		filec;
    char	**filev;

    --argc;
    filev = ++argv;
    filec = 0;

    {
	int		flagc;
	char		**flagv;

	for (flagc = argc, flagv = argv; flagc > 0; --flagc, ++flagv) {

	    if (**flagv == '-') {
		while(*++*flagv != '\0') {
		    switch (**flagv) {
			case 'o':
			    numbase = OCTAL;
			    break;

			case 'd':
			    numbase = DECIMAL;
			    break;

			case 'x':
				numbase = HEX;
				break;
			case 'V':
			    fprintf(stderr,"%s: size-%s\n",SGSNAME,RELEASE);
			    break;


			default:
			    fprintf(stderr,
				"%ssize:  unknown option \"%c\" ignored\n",
				SGS, **flagv);
			    break;
		    }
		}
	    } else {
		*filev++ = *flagv;
		++filec;
	    }
	}
    }

    if (filec == 0) {
	fprintf(stderr, "usage:  %ssize [-Voxd] file ...\n", SGS);
        exit(0);
    }

    /*  buffer standard output, they say it's better to do so
     *  but remember to flush stdout before writing to stderr
     */
    setbuf(stdout, outbuf);

    {
	/* BLOCK LOCAL VARIABLES */
	ARCHDR	arhead;
	char	filename[MAXLEN];
	int	prtst;

	prtst = filec > 1;
	for (filev = argv; filec > 0; --filec, ++filev) {
	    ldptr = NULL;
	    do {
		if ((ldptr = ldopen(*filev, ldptr)) != NULL) {
		    if (ISMAGIC(HEADER(ldptr).f_magic)) {
		        if (ISARCHIVE(TYPE(ldptr))) {
			    if (ldahread(ldptr, &arhead) == SUCCESS) {
#ifdef PORTAR
				sprintf(filename, "%s[%.16s]",
#else
				sprintf(filename, "%s[%.14s]",
#endif
				    *filev, arhead.ar_name);
#ifdef UNIX
				if (prtst == 1) {
					printf("%s: ", filename);
				}
#endif
				process(filename);
				fflush(stdout);
			    } else {
				fprintf(stderr,
				  "%ssize:  %s:  cannot read archive header\n",
				  SGS, *filev);
			    }
			} else {
			    if ( prtst == 1) {
					printf("%s: ", *filev);
			    }
			    process(*filev);
			}
		    } else {
			fprintf(stderr,"%ssize:  %s:  bad magic\n",SGS,*filev);
			ldaclose(ldptr);
			ldptr = NULL;
		    }
		} else {
		    fprintf(stderr, "%ssize:  %s:  cannot open\n", SGS, *filev);
		}
	    } while (ldclose(ldptr) == FAILURE);
	}
    }
    exit(0);
}

/*
 *	static char ID[] = "@(#) main.c: 1.4 3/5/83";
 */
