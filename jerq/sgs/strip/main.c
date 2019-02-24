# define BUF 256
/* UNIX HEADER */
#include	<stdio.h>
#include 	<ar.h>

/* COMMON SGS HEADERS */
#include	"filehdr.h"
#include	"ldfcn.h"



/* SGS SPECIFIC HEADERS */
#include	"paths.h"
#include	"sgs.h"

/* EXTERNAL VARIABLES DEFINED */
LDFILE		*ldptr;
ARCHDR		arhead;
long		xsize = 0;
int 		aflag;
long 		xmore;
int 		mag;
char		tmpnam1[sizeof (TMPDIR) + 16],
		tmpnam2[sizeof (TMPDIR) + 16],
		tmpnam3[sizeof (TMPDIR) + 16],
		tmpnam4[sizeof (TMPDIR) + 16];
#if FLEXNAMES
char		tmpnam5[sizeof (TMPDIR) + 16];	/* string table temp file */
#endif

    /*  The SGS/UNIX Strip Command operates on one or more object files
     *
     *  main(argc, argv)
     *
     *  parses the command line (setflags( ))
     *  prepares to field interrupt (catchsig( ))
     *  opens, processes and closes each command line object file argument
     *  cleans up after itself 
     *
     *  defines:
     *      - ldptr = ldopen(*argv, NULL) for each command line file argument
     *      - tmpnam1 = TMPDIR/SGSstrp1?????  to be used for building the 
     *			stripped file
     *
     *	    - tmpnam2 = TMPDIR/SGSstrp2????? to be used for building a
     *			temporary partially stripped symbol table
     *  calls:
     *      - setflags(--argc, ++argv) to set flags
     *      - catchsig( ) to set up interrupt handling
     *      - process(*argv) to direct the stripping of the object file *argv
     *
     *  prints:
     *      - a usage message if there are no command line object file arguments
     *      - an error message if it can't open a file or if it isn't MAGIC
     *
     *  exits successfully always
     */


main(argc, argv)

int	argc;
char	**argv;

{
    /* UNIX FUNCTIONS CALLED */
    extern		fprintf( ),
			sprintf( ),
			sputl( ),
			unlink( );
    extern int		getpid( );
#ifdef PORT5AR
    extern long		time( );
#endif

    /* OBJECT FILE ACCESS ROUTINES CALLED */
    extern LDFILE	*ldopen( );
    extern int		ldaclose( );

    /* STRIP FUNCTIONS CALLED */
    extern int		setflags( ),
			process( );
    extern		catchsig( ),
			copyarh( ),
			resetsig( );
    extern  FILE 	*stripout;
	    FILE	*tmp;
    extern  long	xmore;
    short		buf[BUF];
    int			nitems,
			retval = 0;		/* exit return value */
#ifdef PORT5AR
    struct  ar_hdr	arhbuf;
#endif



    if ((argc = setflags(--argc, ++argv)) == 0) {
#if UNIX
	fprintf(stderr,"usage:  %sstrip [-l] [-x] [-r] file ...\n", SGS);
#else
	fprintf( stderr, "usage: %sstrip [-l] [-x] [-r] [-s] [-f] file ...\n", SGS);
#endif
	exit(0);
    }

	sprintf(tmpnam1, "%s/%sstrp1%d", TMPDIR, SGS, getpid( ));
	sprintf(tmpnam2, "%s/%sstrp2%d", TMPDIR, SGS, getpid( ));
	sprintf(tmpnam3, "%s/%sstrp3%d", TMPDIR, SGS, getpid( ));
	sprintf(tmpnam4, "%s/%sstrp4%d", TMPDIR, SGS, getpid( ));
#if FLEXNAMES
	sprintf( tmpnam5, "%s/%sstrp5%d", TMPDIR, SGS, getpid( ));
#endif
	catchsig( );

	for (	; argc > 0; --argc, ++argv) {
		if ((ldptr = ldopen(*argv, NULL)) == NULL) {
			fprintf( stderr, "%sstrip:	%s:	cannot open\n", SGS, *argv );
			continue;
		}

		mag = TYPE(ldptr);
		if (!ISMAGIC (TYPE(ldptr)) && (TYPE(ldptr) != ARTYPE)) {
			fprintf(stderr, "%sstrip:  %s:  bad magic\n", SGS, *argv);
			ldaclose(ldptr);

		} else if (TYPE(ldptr) == ARTYPE) {
			aflag = 1;
			do {
			    if (ldahread(ldptr, &arhead) != SUCCESS) {
				fprintf(stderr,"error in archive hdr read %s    %s\n", *argv,arhead.ar_name);
				ldaclose(ldptr);
				exit(1);
				}

			    if (process(*argv) != SUCCESS) {
				/* abandon processing archive, bail out */
				ldaclose(ldptr);
				retval = 1;	/* return an error code */
				goto getout;
				}
			} while (xmore != SUCCESS);

			if ((stripout = fopen(tmpnam4, "r")) == NULL) {
				fprintf(stderr,"cannot open temporary file %s for reading  \n", tmpnam4);
				exit(1);
			}
			if ((tmp  = fopen(*argv, "w")) == NULL) {
				fprintf(stderr,"cannot open temporary file %s for writing\n", *argv);
				exit(1);
			}

#if defined(PORTAR) || defined(PORT5AR)
			/* rewrite the archive header without a symbol dir */
			fprintf(stderr,"%sstrip: Warning - symbol directory deleted from archive %s\n",
				SGS, *argv);
			fprintf(stderr,"	execute  `ar ts %s` to restore symbol directory.\n",
				*argv);
#endif

#ifdef PORT5AR
			strncpy(arhbuf.ar_magic,ARMAG,SARMAG);
			strncpy(arhbuf.ar_name,*argv,sizeof(arhbuf.ar_name));
			sputl(time(NULL),arhbuf.ar_date);
			sputl(0L,arhbuf.ar_syms);
			if (fwrite(&arhbuf,sizeof(struct ar_hdr),1,tmp) != 1) {
#else
#ifdef PORTAR
			if (fwrite( ARMAG, SARMAG, 1, tmp ) != 1) {
#else
			if (fwrite( ARMAG, sizeof(ARMAG), 1, tmp ) != 1) {
#endif
#endif
				error( *argv, "can't recreate file", 5 );
				resetsig( );
				exit(1);
			}
			while((nitems = fread(buf,sizeof(short), BUF, stripout)) != 0 ) {
				if (fwrite(buf,sizeof(short), nitems,tmp) != nitems) {
					error(*argv,"can't recreate file ",5);
					resetsig( );
					exit(1);
				}
			}

		} else { /* not an archive */
			(void) process(*argv);
		}
	} /* for */

getout:
	unlink(tmpnam1);
	unlink(tmpnam2);
	unlink(tmpnam3);
	unlink(tmpnam4);
#if FLEXNAMES
	unlink(tmpnam5);
#endif
	exit(retval);
}
/*
 *	@(#) main.c: 1.5 11/17/83
 */
