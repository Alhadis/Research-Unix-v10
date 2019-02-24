static char ID[] = "@(#) ld01.c: 1.13 8/18/83";
#include "system.h"
#include <stdio.h>
#include <signal.h>
#include "structs.h"
#include "extrns.h"

#if !ONEPROC || TRVEC
#include "tv.h"
#include "ldtv.h"
#endif

#include "list.h"
#include "sgsmacros.h"
#include "instr.h"
#include "ldmacros.h"

#define STRINC 256
/*eject*/
#if ONEPROC
pass2()
#else
main(argc,argv)
int argc;
char *argv[];
#endif
{

/*
 * This is the MAIN procedure for PASS 2 of ld
 *
 * PASS 2 is the execution pass:
 *	1. The control information extracted during PASS 1 is read in
 *	2. Control blocks are constructed, containing the information
 *		extracted during PASS 1
 *	3. Allocation is performed
 *	4. The output file is generated
 */

/*
 * Initialize PASS 2
 */

#if ONEPROC
	initpss2();
#else
	initpass2(argv[1]);
#endif

/*
 * If necessary, make the symbol "_start" undefined
 */

	undefine();


/*
 * Apply, if any, the default SECTIONS directives
 */

	dfltsec();

	psymdef();		/* process symbol (un)definition */
	pmemreg();		/* process memlist and reglist */
	ploadfil();		/* process LDFILE list */
#if TRVEC 
	if (tvflag)		/* look for references to functions */
		tvreloc();	
#endif
#if COMMON
	if (aflag)
		pf77comm();		/* process common data for F77 */
#endif
	pboslist();		/* process BLDOUTSEC list */
	split_scns();		/* break up outscts if they are too big */
	alloc();		/* perform output section allocation */
	if (Bflag)
		add_pad();	/* add padding sections */
#if ILDOPT
	if (ildflag)		/* add extra sections */
		add_extra();
#endif
	updatsms();		/* complete section hdrs & relocate symbols */
	pexlist();		/* process expression list */


/*
 * Output a memory map if requested:
 *	1. The -m flag was set
 *	2. There were no .o or archive files in the ld run
 */

	if( mflag )
		ldmap();

	if (cflag != 3)
		output();	/* write output object file */

#if DEBUG
	if (dflag)
		fprintf( stderr, "total work space allocated: %d\n", allocspc );
#endif

#if !NOSDP
	symfini();
#endif

	instrment();

	exit(0);
}
/*eject*/
instrment()
{
	register FILE *instrfile;
	long times();

/*
 * Complete the collection of the timing statistics
 */

	ttime = times(&ptimes) - ttime;

/*
 * Determine if any statistics are to be output
 */

	if( (instrfile = fopen("instr.data", "r")) == NULL )
		return;
	else
		fclose(instrfile);

	if( (instrfile = fopen("instr.data", "a")) == NULL ) {
		lderror(0,0,NULL,"Can't open 'instr.data'");
		return;
		}
	
/*
 * Output the ld run statistics
 */

	fprintf( instrfile, "%07ld %07ld %07ld %07ld\n",
		SDP_read, SDP_write, LD_lock, LD_unlock);
	fprintf( instrfile, "%07ld %07ld %07ld %07ld %07ld %07ld %07ld\n",
		noutsyms, numldsyms, numldaux, nwalks, nfwalks, maxchain, 
		ncolisns );
	fprintf( instrfile, "%07ld %07ld %07ld %07ld %07ld\n",
		ttime, ptimes );
	fprintf( instrfile, "%07ld %07ld\n",
		allocspc, maxalloc);

	fclose(instrfile);
}
/*eject*/
#if ONEPROC
initpss2()
#else
initpass2(file)
char *file;
#endif
{

	long times();
	int ldexit();

/*
 * Initialize the process time counters
 */

#if !ONEPROC
	ttime = times(&ptimes);
#endif


/*
 * Set the following signals to be caught, unless ld is running in the
 * background. In this case, leave them set at "ignore"
 */

	if( signal(SIGINT, ldexit)  ==  SIG_IGN )
		signal(SIGINT, SIG_IGN);
	if( signal(SIGHUP, ldexit)  ==  SIG_IGN )
		signal(SIGHUP, SIG_IGN);
	if( signal(SIGQUIT, ldexit)  ==  SIG_IGN )
		signal(SIGQUIT, SIG_IGN);

/*
 * Open the "transfer file" built during PASS 1
 *
 * Unless PASS 2 is being run manually, 
 *	THIS FILE IS ALWAYS UNLINKED BY PASS 2
 */

#if !ONEPROC
	if( (trnfdes = fopen(file,"r")) == NULL )
		lderror(2,0,NULL, "Can't open internal file %s",
			file );
#if AR16WR
	trnfbuf = myalloc( BUFSIZ );
	setbuf( trnfdes, trnfbuf );
#endif
#endif

/*
 * Initialize the Software Demand Paging System
 */

#if (!NOSDP) && (!ONEPROC)
	syminit();
#endif


#if !ONEPROC
/*
 * Input control and descriptor information
 */

	fseek( trnfdes, 0L, 0 );

/*
 * Input the sizes of the lists built during PASS 1
 *
 *	1. define symbol directives
 *	2. MEMORY directives
 *	3. REGIONS directives
 *	4. input *.o and archive library files
 *	5. output sections
 *	6. assignment directives
 *	7. tv slot assignment directives
 */

	fread( &symcnt, sizeof(int), 1, trnfdes );
	fread( &memcnt, sizeof(int), 1, trnfdes );
	fread( &regcnt, sizeof(int), 1, trnfdes );
	fread( &ldfcnt, sizeof(int), 1, trnfdes );
	fread( &bldcnt, sizeof(int), 1, trnfdes );
	fread( &expcnt, sizeof(int), 1, trnfdes );
	fread( &tvslotcnt, sizeof(int), 1, trnfdes );

/*
 * Input the ld control flags
 */

	fread( &aflag, sizeof(char), 1, trnfdes );
	fread( &cflag, sizeof(char), 1, trnfdes );
	fread( &dflag, sizeof(char), 1, trnfdes );
	fread( &iflag, sizeof(char), 1, trnfdes );
	fread( &mflag, sizeof(char), 1, trnfdes );
	fread( &rflag, sizeof(char), 1, trnfdes );
	fread( &sflag, sizeof(char), 1, trnfdes );
	fread( &tvflag, sizeof(char), 1, trnfdes );
	fread( &Hflag, sizeof(char), 1, trnfdes );
	fread( &Nflag, sizeof(char), 1, trnfdes );
	fread( &Sflag, sizeof(char), 1, trnfdes );
	fread( &Vflag, sizeof(char), 1, trnfdes );
	fread( &Xflag, sizeof(char), 1, trnfdes );
	fread( &hflag, sizeof(int), 1, trnfdes );
	fread( &pflag, sizeof(int), 1, trnfdes );
	fread( &Bflag, sizeof(int), 1, trnfdes );
	fread( &VSflag, sizeof(short), 1, trnfdes );
#if UNIX || XL
	fread( &xflag, sizeof(char), 1, trnfdes );
#endif
#if COMMON
	fread( &tflag, sizeof(char), 1, trnfdes );
	fread( &Mflag, sizeof(char), 1, trnfdes );
#endif
#if PAGING
	fread( &Fflag, sizeof(char), 1, trnfdes );
#endif
#if PAGING
	fread( &zflag, sizeof(char), 1, trnfdes );
#endif
#if ILDOPT
	fread( &ildflag, sizeof(char), 1, trnfdes );
#endif

/*
 * Input various values extracted from the parse
 */

	fread( &globfill, sizeof(short), 1, trnfdes );
	fread( &magic, sizeof(unsigned short), 1, trnfdes );
	fread( outfilnm, 128, 1, trnfdes );
	fread( epsymbol, 8, 1, trnfdes );
	fread( &tvspec, sizeof(TVINFO), 1, trnfdes );
	fread( &strleng, sizeof(int), 1, trnfdes );

#if DEBUG
	if( dflag > 2 ) {
		fprintf( stderr, "\nList sizes:\n\tSYM: %d   MEM: %d   REG: %d   LDF : %d   BLD: %d   EXP : %d",
			symcnt, memcnt, regcnt, ldfcnt, bldcnt, expcnt );
		fprintf( stderr, "\nFlag values:\n\ta   c   d   i   m   p   r   s   tv   H   S   V   X      h      B" );
		fprintf( stderr, "\n\t%d   %d   %d   %d   %d   %d   %d   %d    %d   %d   %d   %d   %d   %04x   %04x",
			aflag,cflag,dflag,iflag,mflag,pflag,rflag,sflag,tvflag,
			Hflag,Sflag,Vflag,Xflag,hflag,Bflag);
		fprintf( stderr, "\nControl values:\n\tglobal fill: %04x     magic: %04x     a.out: (%.128s)     ep: (%.8s)",
			globfill, magic, outfilnm, epsymbol );
		fprintf( stderr, "\n\ttv: %04x (%8.8s) (%08lx) (%04x) %d %d %08lx",
			tvspec.tvosptr, tvspec.tvfnfill, tvspec.tvfill,
			tvspec.tvinflnm, tvspec.tvinlnno,
			tvspec.tvlength, tvspec.tvbndadr );
		fprintf( stderr, "\nString area length: %d\n", strleng );
		}
#endif

/*
 * Input the strings saved from PASS 1:
 *	The names of input ifiles, input *.o and archive files
 *
 * Allow for a small amount of additional growth, during PASS 2
 */

	strbase = myalloc(strleng + STRINC);
	fread( strbase, 1, strleng, trnfdes );

	strnext = strbase + strleng;
	strlimit = strnext + STRINC;
#endif

#if TRVEC
	if (tvflag)
		tvspecdef();	/* complete definition of tvspec */
#endif

#if !ONEPROC
	if( cflag != 2 )
		unlink( file );
#endif
}
/*eject*/
ldexit()
{

/*
 * Clean up and exit after a fatal error
 */

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_IGN);
	signal(SIGHUP, SIG_DFL);

/*
 * Unlink the a.out file, to cause its removal
 */

	unlink( outfilnm );

#if !NOSDP
	symfini();

	svfini();
#endif

	exit(13);  /* unlucky break */
}
/*eject*/
undefsm(name)
char *name;
{

/* 
 *  Create a symbol table entry for an undefined symbol
 *  ( which may get defined later).
 */

	SYMENT sym;

	zero( (char *) &sym, SYMESZ );

#if FLEXNAMES
	if (strlen(name) > 8) {
		sym.n_zeroes = 0L;
		sym.n_nptr = name;
		}
	else
#endif
		copy( sym.n_name, name, 8);
	sym.n_sclass = C_EXT;

	putsym( makesym(&sym, NULL), 1 );
}
