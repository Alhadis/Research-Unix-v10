/*
 *	static	char	dis_main_ID[] = "@(#)dis_main.c	1.15 12/15/83";
 */
#include	<stdio.h>
#include	<ar.h>
#include	"dis.h"
#include	"filehdr.h"
#include	"ldfcn.h"
#include	"sgs.h"
#include	"scnhdr.h"
#include	"paths.h"
#include	"syms.h"
#include	"linenum.h"
#include	"structs.h"
#ifdef OLIST
#include	"olist_defs.h"
#else
#define TOOL	"dis"
#endif

/* this header is to contain the archive header information */
static	ARCHDR	archhdr;

/*
 *	To process the '-l string' option, the library name will be
 *	built by concatenating  LIBDIR  from 'paths.h' (where the library is),
 *	LIB from '_main.c' (/lib), the user supplied string
 *	(will probably be 'a' for liba.a) and '.a', thus building
 *	the whole name.  Code for this is in the 'command_line' function.
 */

#define		LIB	"/lib"

/*
 *	the number of files that may be disassembled.
 */

#define		FILES	20
#define		FILNMLEN	14


/*
 * filename	contains the names of the files to be disassembled
 * files	contains the number of entries in the above array
 */

static	char	*filename[FILES];
static	int	files = -1;

static	int	status;		/* for check after return from I/O routines	*/


/*
 *	main (argc, argv)
 *
 *	this routine deals with the disassembly only on a per-file basis.
 *	Upon determining that a file is valid (by a magic number) it then
 *	gives control to a subordinate routine (disassembly) to proceed
 *	with the processing.
 */

main(argc, argv)
int	argc;
char	*argv[];
{
	extern	LDFILE	*f_ptr;		/* in _extn.c */
	extern	char	*fname;		/* in _extn.c */
	extern	int	lflag;		/* in _extn.c */
	extern	int	Lflag;		/* in _extn.c */
	extern int	save_Lflag;	/* in _extn.c */
	extern int	nosyms;		/* in _extn.c */
#ifdef OLIST
	extern	int	isarch;
	extern	char	membernm[];
	extern	char	tmpfil[];
#endif	

	int	c;
	unsigned short	type;
	static char	buffer[BUFSIZ];
#ifdef OLIST
	char	buf[BUFSIZ/2];
#endif

	if (argc == 1) {
		fprintf(stderr,"%s%s: Usage is %s%s [flags] file1 ...\n",SGS,TOOL,SGS,TOOL);
		exit(1);
	}

	setbuf(stdout, buffer);

	/* process the command line	*/

	command_line(argc, argv);

#ifdef OLIST
	/* make temporary files */
	sprintf(tmpfil, "%s/listpXXXXXX", TMPDIR);
	mktemp(tmpfil);
#else
	printf("\t\t****  %s DISASSEMBLER  ****\n\n",SGS);
#endif

	/* verify each named file as it is opened for disassembly	*/
	for (c = 0; c <= files; c++) {	/* for each named file */
		fname = filename[c];
#ifdef OLIST
		printf("\n\n\t\t\t3B20D OBJECT LISTING\n\t\t\t\t%s\n\n", fname);
#else
		printf("\ndisassembly for %s\n",fname);
#endif

		/*
		 * The do while loop is the construct used to process both
		 * archive and regular files.  The library routine 'ldclose'
		 * will recognize the fact that an archive is being processed
		 * and will return a true value only when asked to close
		 * what was the last member of an archive file.
		 */
		do {
#ifdef OLIST
			isarch = FALSE;
#endif
			f_ptr = ldopen(fname, f_ptr);
			type = TYPE(f_ptr);
			fflush(stdout);
			if (f_ptr == NULL)
				fprintf(stderr,"\n%s%s: %s: cannot open\n",SGS,TOOL,fname);
			else if ((!ISARCHIVE(type)) && (!ISMAGIC(type))) {
				fprintf(stderr,"\n%s%s: %s: bad magic %o\n",SGS,TOOL,fname,type);
				ldclose(f_ptr);
				break;	/* to next file	*/
			}

			else  {	/* successful open so process */
				/* if the file was an archive, read	*/
				/* and verify its header		*/
				if (type == USH_ARTYPE) {
					status = ldahread(f_ptr, &archhdr);
#if OLIST
					isarch = TRUE;
					sprintf(membernm, "%s", archhdr.ar_name);
					membernm[FILNMLEN] = NULL;
#else
					printf("\narchive member\t\t%s\n",
						archhdr.ar_name);
#endif

					if (status == FAIL) {
						printf("\n%s%s: %s:  %s:",SGS,TOOL,
							fname,archhdr.ar_name);
						fprintf(stderr," bad archive header\n");
						continue;

					}
				}

				disassembly();
			}
		}
		while (!ldclose(f_ptr));
		if (Lflag == -1)
			Lflag = save_Lflag;
	}

	if (lflag > 0)
		lib_free();	/* free space allocated for library names */
#ifdef OLIST
	sprintf(buf, "/bin/rm -f %s\n", tmpfil);
	system(buf);	/* remove temp files */
#endif

	/* this statement is to assure that the return code will be 0 */
	exit(0);
}
/*
 *	disassembly ()
 *
 *	For each file that is disassembled, disassembly opens the
 *	necessary file pointers, builds the list of section
 *	headers, and if necessary, lists of functions and labels.
 * 	It then calls text_sections or dis_data to disassemble
 *	the sections.
 */

disassembly()
{
	extern	LDFILE	*ldaopen( );
	extern	SCNLIST	*build_sections();

	extern	LDFILE	*f_ptr;
	extern	LDFILE	*symb;
	extern	LDFILE	*symb_ptr;
	extern	LDFILE	*l_ptr;
	extern	LDFILE	*t_ptr;
	extern	int	Lflag;
	extern	char	*fname;
	extern	char	*sname;
	extern	FILHDR	filhdr;
	extern	FUNCLIST	*next_function;
	extern	SCNHDR	scnhdr;
#if OLIST
	extern	int	minmal;
	extern	struct	files	*curfile;
	extern	SCNLIST	*curr_section;
	extern	int	nosyms;
#else
	extern	int	fflag;
#endif

	SCNLIST	*sclist;
	SCNLIST		*sectp;
	int		i;

	filhdr = HEADER( f_ptr );
	symb = ldaopen( fname, f_ptr );
	if (ldtbseek( symb ) == FAILURE)
		nosyms = TRUE;
	else
		nosyms = FALSE;
	symb_ptr = ldaopen( fname, f_ptr );
	ldtbseek( symb_ptr );
	l_ptr = ldaopen( fname, f_ptr );
	t_ptr = ldaopen(fname, f_ptr);

	/* if labels are requested, make a linked list of the labels */
	if ( Lflag > 0 )
		build_labels();

#if OLIST
	minmal = (filhdr.f_flags & F_MINMAL);
#else
	/* disassemble only the names functions */
	if (fflag)
	{
		search_table();
		dis_funcs();

		if (Lflag > 0)
			label_free();
		ldaclose( symb );
		ldaclose( symb_ptr );
		ldaclose( l_ptr );
		ldaclose( t_ptr );
		return;
	}
#endif

	/* make a list of the sections to be disassembled */
	sclist = build_sections();

	/* make lists of the files and functions from the symbol table */
#if OLIST
	bld_files();
	if (minmal)
		build_funcs( sclist );
#else
#if !AR16WR
	build_funcs( sclist );
#endif
#endif

	for ( sectp = sclist; sectp; sectp = sectp->snext )
		if (sectp->stype == TEXT)
		{
			FSEEK( l_ptr, sectp->shdr.s_lnnoptr, BEGINNING );
			FSEEK( t_ptr, sectp->shdr.s_scnptr, BEGINNING );
			sprintf( sname, "%.8s", sectp->shdr.s_name );
#if OLIST
			curr_section = sectp;
#else
			printf("\nsection\t%s\n", sname );
#endif
			next_function = sectp->funcs;
			scnhdr = sectp->shdr;
			dis_text();
		}
		else
			dis_data( sectp );

	/* Now free up all the storage that was allocated for	*/
	/* the lookup of labels, files, functions, and sections */
#if OLIST
	file_free();
#endif
	if (Lflag > 0)
		label_free();
	section_free( sclist );
	ldaclose( symb );
	ldaclose( symb_ptr );
	ldaclose( l_ptr );
	ldaclose( t_ptr );

#if OLIST
	putchar( '\014' );
	curfile = NULL;
#endif
}

/*
 *	command_line (argc, argv)
 *
 *	This routine processes the command line received by the disassembler.
 *	Flags and filenames may be in any order.
 */


command_line(argc, argv)
int	argc;
char	*argv[];
{
	/* the following external variables are all from _extn.c */
	extern	int	oflag;
	extern	int	trace;
#if !OLIST
	extern	int	fflag;
	extern	NFUNC	ffunction[];
#endif
	extern	int	Lflag;
	extern	int	lflag;
	extern	char	*libs[];
	extern	short	aflag;
#ifdef OLIST
	extern	char	Sfile[];
#endif
	extern	char	*namedsec[];
	extern	int	namedtype[];
	extern	int	nsecs;

	char	*calloc();
	char	z;
	int	str_1, str_2, str_len;

	char	*p;
	int c, i;
	for (c = 1; c < argc; c++){	/* begin processing command line */
		p = argv[c];
		if (*p == '-'){		/* have one or more flags together */
			for (i = 1; p[i]; i++){ /* more than one flag possible*/
				switch(p[i]){
				case 'T':
					trace++;
					continue;
				case 'o':
					oflag++;
					continue;
				case 'L':
					Lflag++;
					continue;
				case 'l':
					lflag++;
					c++;
					str_1 = sizeof(LIBDIR) - 1 + strlen(LIB);
					str_2 = strlen(argv[c]);
					/* add 2 for '.a'	*/
					str_len = str_1 + str_2 +2;
					if ((libs[lflag] = calloc(str_len+1,sizeof(z))) == NULL) {
						fprintf(stderr, "memory allocation failure on call to calloc");
						exit(0);
					}
					sprintf(libs[lflag],"%s%s%s.a", LIBDIR,LIB,argv[c]);
					if (trace > 0)
						printf("\nlib is {%s}\n", libs[lflag]);
					files++;
					filename[files] = libs[lflag];
					break;
				case 'd':	/* disassemble as a data section flag */
					if (p[i+1] == 'a') {
					/* print addresses of data rather than offsets */
						i++;
						aflag++;
					}
					nsecs++;
					c++;
					namedsec[nsecs] = argv[c];
					namedtype[nsecs] = DATA;
					break;
				case 't':
					nsecs++;
					c++;
					namedsec[nsecs] = argv[c];
					namedtype[nsecs] = TEXT;
					break;
				case 'V':	/* version flag */
					fprintf(stderr,"%s%s: %s\n",SGS,TOOL,
						RELEASE);
					break;
#if !OLIST
				case 'F':
					if (argc < (c + 3)) {
						fprintf(stderr,"Usage is: %sdis ",SGS);
						fprintf(stderr,"-F function file\n");
						exit(2);
					}
					if (fflag == FFUNC)
					{
						fprintf( stderr, "too many -F otpions\n");
						break;
					}
					fflag++;
					c++;
					ffunction[fflag - 1].funcnm = argv[c];
					break;
#endif
				default:
					fprintf(stderr,"\n%s%s: bad flag %c\n",SGS,TOOL,p[i]);
					break;
				}	/* end of switch */
			}	/* end of multi flag processing */
		}
		else {		/* must be a file name */
#ifdef OLIST
			if ( argv[c][strlen( argv[c] ) -2] == '.' &&
					argv[c][strlen( argv[c] ) - 1] == 'S' ) 
				strcpy( Sfile, argv[c] );
			else
#endif
			{
				files++;
				filename[files] = argv[c];
			}
		}	/* end of processing for this parameter */
	}	/* the entire command line is now processed */
}

#if !OLIST
/*
 * search the symbol table for the named functions and fill in the
 * information in the ffunction array
 */

search_table()
{
	extern	char	*ldgetname();

	extern	LDFILE	*symb;
	extern	FILHDR	filhdr;
	extern	int	fflag;
	extern	NFUNC	ffunction[];

	long	i;
	int	j;
	SYMENT	symbol;
	AUXENT	auxent;
	char	*name;

	for ( i = 0; i < filhdr.f_nsyms; i++ )
	{
		if (ldtbread( symb, i, &symbol ) == FAILURE)
			fatal( "cannot read symbol table" );
		if ((name = ldgetname( symb, &symbol )) == NULL)
			fatal( "ldgetname returned NULL" );

		for ( j = 0; j < fflag; j++ )
			if (strcmp( name, ffunction[j].funcnm ) == 0)
				break;

		if ((j != fflag) && ISFCN(symbol.n_type) && (symbol.n_numaux > 0))
		{
			ffunction[j].faddr = symbol.n_value;
			ffunction[j].fcnindex = i;
			ffunction[j].found = 1;
			ffunction[j].fscnum = symbol.n_scnum;

			if (ldtbread( symb, i + 1, &auxent ) == FAILURE)
				fatal( "cannot read symbol table" );
			ffunction[j].fsize = auxent.x_sym.x_misc.x_fsize;
		}

		i += symbol.n_numaux;
	}
}

/*
 * disassemble the functions in the ffunction array
 */
dis_funcs()
{
	extern	NFUNC	ffunction[];
	extern	int	fflag;
	extern	char	*fname;
	extern	LDFILE	*f_ptr;
	extern	LDFILE	*t_ptr;
	extern	SCNHDR	scnhdr;
	extern	char	*sname;
	extern	FUNCLIST	*next_function;

	int		i;
	FUNCLIST	func;

	for ( i = 0; i < fflag; i++ )
	{
		if (!ffunction[i].found)
		{
			fprintf( stderr, "%sdis: %s: function %s not found\n",
				SGS, fname, ffunction[i].funcnm );
			continue;
		}

		/* find the right section header */
		if (ldshread( f_ptr, ffunction[i].fscnum, &scnhdr ) == FAILURE)
			fatal( "cannot read section headers" );
		sprintf( sname, "%.8s", scnhdr.s_name );
		printf( "\nsection\t%s\n", sname );

		/*
		 * seek to the start of the function, and change the section
		 * header to fake out dis_text (make it think that the section
		 * contains just that one function
		 */
		FSEEK( t_ptr, scnhdr.s_scnptr + (ffunction[i].faddr - scnhdr.s_paddr), BEGINNING );
		scnhdr.s_paddr = ffunction[i].faddr;
		scnhdr.s_size = ffunction[i].fsize;
		if ((ldlinit(f_ptr,ffunction[i].fcnindex)) == FAILURE) {
			scnhdr.s_lnnoptr = NULL;
			scnhdr.s_nlnno = 0;
		}

		func.funcnm = ffunction[i].funcnm;
		func.faddr = ffunction[i].faddr;
		func.fcnindex = ffunction[i].fcnindex;
		func.nextfunc = NULL;
		next_function = &func;
		dis_text();
	}
}
#endif
