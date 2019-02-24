static char ID[] = "@(#) display.c: 1.1 1/9/82";

#include <stdio.h>
#include "structs.h"
#include "extrns.h"
#include "tv.h"
#include "ldtv.h"
#include "y.tab.h"

char *DS[] = {
	"** BAD 0 **",
	"** BAD 1 **",
	"** BAD 2 **",
	"AILDFILE",
	"AILDLBRY",	
	"AIEVEXPR",	
	"AIDFNSCN",	
	"AIADDSCN",	
	"AIADFILE",	
	"AIBOND",	
	"AIDFOWNR",
	"AIOWNATR",	
	"AIDFNGRP",	
	"AIDFNSYM"	
	};
char *OP[] = {
	"** BAD OP 0 **",
	"** BAD OP 1 **",
	"NAME",
	"LONGINT",
	"INT",
	"ALIGN",
	"DOT",
	"LEN",
	"MEMORY",
	"ORG",
	"REGIONS",
	"SECTIONS",
	"PHY",
	"AND",
	"ANDAND",
	"BNOT",
	"COLON",
	"COMMA",
	"DIV",
	"EQ",
	"EQEQ",
	"GE",
	"** BAD OP 22 **",
	"GT",
	"LBRACE",
	"** BAD OP 25 **",
	"LE",
	"LPAREN",
	"LSHIFT",
	"LT",
	"MINUS",
	"MULT",
	"NE",
	"NOT",
	"OR",
	"OROR",
	"PC",
	"PLUS",
	"RBRACE",
	"RPAREN",
	"RSHIFT",
	"SEMICOL",
	"DIVEQ",
	"MINUSEQ",
	"MULTEQ",
	"PLUSEQ",
	"FILENAME",
	"TV",
	"SPARE",
	"DSECT",
	"NOLOAD",
	"BLOCK",
	"UMINUS",
	"GROUP"
	};
/*eject*/
main(argc,argv)
int argc;
char *argv[];
{

/*
 * List a ld "intermediate file"
 */

/*
 * Open the "transfer file"
 */

	if( (trnfdes = fopen( argv[1], "r" )) == NULL ) {
		printf( "\nFailure in opening the transfer file %s\n",
			argv[1] );
		exit( 1 );
		}
	printf( "\nStart of ld lister program for file %s\n", argv[1] );

/*
 * List the file
 */

	lst_tf_hdr();
	lst_tf();

	printf( "\nEnd of ld lister program\n" );
}
/*eject*/
lst_tf_hdr()
{

/*
 * Read and print control and descriptor information
 */

	fseek( trnfdes, 0L, 0 );

/*
 * Print the sizes of the lists built during PASS 1
 *
 *	1. define symbol directives
 *	2. MEMORY directives
 *	3. REGIONS directives
 *	4. input *.o and archive library files
 *	5. output sections
 *	6. assignment directives
 */

	fread( &symcnt, sizeof(int), 1, trnfdes );
	fread( &memcnt, sizeof(int), 1, trnfdes );
	fread( &regcnt, sizeof(int), 1, trnfdes );
	fread( &ldfcnt, sizeof(int), 1, trnfdes );
	fread( &bldcnt, sizeof(int), 1, trnfdes );
	fread( &expcnt, sizeof(int), 1, trnfdes );
	fread( &tvslotcnt, sizeof(int), 1, trnfdes );

	printf( "\nList sizes:\n\tSYM: %d   MEM: %d   REG: %d   LDF : %d   BLD: %d   EXP : %d   TV : %d\n",
		symcnt, memcnt, regcnt, ldfcnt, bldcnt, expcnt, tvslotcnt );

/*
 * Print the ld control flags
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
	fread( &Sflag, sizeof(char), 1, trnfdes );
	fread( &Vflag, sizeof(char), 1, trnfdes );
	fread( &Xflag, sizeof(char), 1, trnfdes );
	fread( &hflag, sizeof(int), 1, trnfdes );
	fread( &pflag, sizeof(int), 1, trnfdes );
	fread( &Bflag, sizeof(int), 1, trnfdes );
#if U3B
	fread( &xflag, sizeof(char), 1, trnfdes );

	printf( "\nFlag values:\n\ta   c   d   i   m   p   r   s   x   tv   H   O   S   V      h      B" );
	printf( "\n\t%d   %d   %d   %d   %d   %d   %d   %d   %d    %d   %d   %d   %d   %d   %04x   %04x\n",
		aflag,cflag,dflag,iflag,mflag,pflag,rflag,sflag,xflag,tvflag,Hflag,
		Xflag,Sflag,Vflag,hflag,Bflag);
#else
	printf( "\nFlag values:\n\ta   c   d   i   m   p   r   s   tv   H   S   V   X      h      B" );
	printf( "\n\t%d   %d   %d   %d   %d   %d   %d   %d    %d   %d   %d   %d   %d   %04x   %04x\n",
		aflag,cflag,dflag,iflag,mflag,pflag,rflag,sflag,tvflag,Hflag,
		Sflag,Vflag,Xflag,hflag,Bflag);
#endif

/*
 * Print various values extracted from the parse
 */

	fread( &globfill, sizeof(short), 1, trnfdes );
	fread( &magic, sizeof(unsigned short), 1, trnfdes );
	fread( outfilnm, 128, 1, trnfdes );
	fread( epsymbol, 8, 1, trnfdes );
	fread( &tvspec, sizeof(TVINFO), 1, trnfdes );

	printf( "\nControl values:\n\tglobal fill: %04x     magic: %04x     a.out: (%.128s)     ep: (%.8s)",
		globfill, magic, outfilnm, epsymbol );
#if B16 || X86
	printf( "\n\ttv: %04x (%8.8s) (%04x,%04x) (%04x) %d %d %08lx (%04x,%04x)\n",
		tvspec.tvosptr, tvspec.tvfnfill, tvspec.tvfill.tv_ip,
		tvspec.tvfill.tv_cs, tvspec.tvinflnm, tvspec.tvinlnno,
		tvspec.tvlength, tvspec.tvbndadr, tvspec.tvrange[0],
		tvspec.tvrange[1] );
#else
	printf( "\n\ttv: %04x (%8.8s) (%08lx) (%04x) %d %d %08lx (%04x,%04x)\n",
		tvspec.tvosptr, tvspec.tvfnfill, tvspec.tvfill.tv_addr,
		tvspec.tvinflnm, tvspec.tvinlnno,
		tvspec.tvlength, tvspec.tvbndadr, tvspec.tvrange[0],
		tvspec.tvrange[1] );
#endif


/*
 * Print the siza of the saved strings
 */

	fread( &strleng, sizeof(int), 1, trnfdes );

	printf( "\nString area length: %d\n", strleng );
}
/*eject*/
lst_tf()
{

/*
 * Print the bulk of the information extracted from PASS 1:
 *
 *	1. The names of input ifiles, input *.o and archive files, and
 *		output sections
 *	2. The data structures describing the ld directives
 */

	extern char *calloc();
/*
 * 1. Read and print saved strings
 */

	strbase = calloc(1,strleng);
	fread( strbase, 1, strleng, trnfdes );

	strnext = strbase;
	strlimit = strbase + strleng;
	printf( "\nSaved strings:" );
	while( strnext < strlimit ) {
		printf( "\n\t%5d : (%s)", (strnext-strbase), strnext );
		while( *strnext++ )
			;
		}
	printf( "\n" );

/*
 * 2a. Read and print DFNSYM data structures
 */

	printf( "\nDFNSYM data structures:" );
	while( symcnt-- )
		prtds1();
	printf( "\n" );

/*
 * 2b. Read and print MEMTYPE data structures
 */

	printf( "\nMEMTYPE data structures:" );
	while( memcnt-- )
		prtds2();
	printf( "\n" );

/*
 * 2c. Read and print REGION data structures
 */

	printf( "\nREGION data structures:" );
	while( regcnt-- )
		prtds3();
	printf( "\n" );

/*
 * 2d. Read and print information on input *.o and archive library files
 */

	printf( "\nLDFILE data structures:" );
	while( ldfcnt-- )
		prtds4();
	printf( "\n" );
	

/*
 * 2e. Read and print information on output sections
 */

	printf( "\nBLDOSCN data structures:" );
	while( bldcnt-- )
		prtds5();

	printf( "\n" );

/*
 * 2e. Read and print information on assignments
 */

	printf( "\nEVEXPR data structures:" );
	while( expcnt-- )
		prtds10();
	printf( "\n" );

/*
 * 2f. Read and print information on tv slot assignments
 */

	printf( "\nTVASSIGN data structures:" );
	while( tvslotcnt-- )
		prtds11();
	printf( "\n" );
}
/*eject*/
prtds1()
{
/*
 * Read and print one DFNSYM data structure
 */

	ACTITEM a;

	fread( &a, 1, sizeof(ACTITEM), trnfdes );

	printf( "\n\t%04x %s (%s) %d (%s) %s %08lx",
		a.dfnsym.ainext, DS[a.dfnsym.aitype], (strbase+a.dfnsym.aiinflnm),
		a.dfnsym.aiinlnno, (strbase+a.dfnsym.aisymbol),
		(a.dfnsym.aideflag ? "DEF" : "UNDEF"), a.dfnsym.aidefval );
}
prtds2()
{
/*
 * Read and print one MEMTYPE data structure
 */

	MEMTYPE m;

	fread( &m, 1, sizeof(MEMTYPE), trnfdes );

	printf( "\n\t%04x (%8.8s) %08lx %08lx %d %04x %04x",
		m.mtnext, m.mtname, m.mtorig, m.mtlength, m.mtattr,
		m.mtaddrhd, m.mtaddrtl );
}
prtds3()
{
/*
 * Read and print one REGION data structure
 */

	REGION r;

	fread( &r, 1, sizeof(REGION), trnfdes );

	printf( "\n\t%04x (%8.8s) %08lx %08lx %08lx %04x %04x",
		r.rgnext, r.rgname, r.rgorig, r.rglength, r.rgvaddr,
		r.rgaddrhd, r.rgaddrtl );
}
prtds4()
{
/*
 * Read and print one AILDFILE or AILDLBRY data structure
 */

	ACTITEM a;

	fread( &a, 1, sizeof(ACTITEM), trnfdes );

	printf( "\n\t%04x %s (%s) %d (%s)",
		a.ldlbry.ainext, DS[a.ldlbry.aitype], (strbase+a.ldlbry.aiinflnm),
		a.ldlbry.aiinlnno, (strbase+a.ldlbry.aifilnam) );
}
prtds5()
{
/*
 * Read and print one AIDFNSCN or AIDFNGRP data structure
 */

	ACTITEM a, b;

	fread( &a, 1, sizeof(ACTITEM), trnfdes );

	printf( "\n\t%s%04x %s (%s) %d (%s) %04x (%s)\n\t\t\t%08lx %d %08lx %08lx %d %d (%04x,%04x)",
		(a.dfnscn.aitype == AIDFNSCN) ? "    " : "",
		a.dfnscn.ainext, DS[a.dfnscn.aitype], (strbase+a.dfnscn.aiinflnm),
                a.dfnscn.aiinlnno, a.dfnscn.ainame, a.dfnscn.aifill,
		a.dfnscn.aiowname, a.dfnscn.aibndadr, a.dfnscn.aiattown,
		a.dfnscn.aialign, a.dfnscn.aiblock, (unsigned) a.dfnscn.aifillfg,
		(unsigned) a.dfnscn.aisctype, a.dfnscn.sectspec.head, a.dfnscn.sectspec.tail );

	switch( a.dfnscn.aitype ) {
	case AIDFNSCN:
		if( ! a.dfnscn.sectspec.head )
			return;
		do {
			fread( &b, 1, sizeof(ACTITEM), trnfdes );
			switch( b.adfile.aitype ) {
			case AIADDSCN:
				prtds6(&b);
				if( b.addscn.ainext )
					break;
				return;
			case AIADFILE:
				prtds7(&b);
				if( b.adfile.ainext  ||  b.adfile.ainadscs )
					break;
				return;
			case AIEVEXPR:
				prtds8(&b, 1);
				if( b.evexpr.ainext )
					break;
				return;
			default:
				printf( "\n***** failure listing an AIDFNSCN data structure\n");
				return;
			}
			}
		while( 1 );
		break;
	case AIDFNGRP:
		do {
			fread( &b, 1, sizeof(ACTITEM), trnfdes );
			switch( b.dfnscn.aitype ) {
			case AIDFNSCN:
				fseek( trnfdes, (long) -sizeof(ACTITEM), 1 );
				prtds5();
				if( b.dfnscn.ainext )
					break;
				return;
			default:
				printf( "\n***** failure listing an AIDFNGRP data structure\n");
				return;
			}
			}
		while( 1 );
		break;
	default:
		printf( "\n***** failure listing an AIDFNxxx data structure\n");
	}

}
/*eject*/
prtds6(a)
ACTITEM *a;
{
/*
 * Print one AIADDSCN data structure
 */

	printf( "\n\t\t    %04x %s (%s) %d (%s) %04x (%s)",
		a->addscn.ainext, DS[a->addscn.aitype], (strbase+a->addscn.aiinflnm),
		a->addscn.aiinlnno, a->addscn.ainame, a->addscn.aifill,
		(strbase+a->addscn.aiscfile) );
}
prtds7(a)
ACTITEM *a;
{
/*
 * Print one AIADFILE data structure
 */

	printf( "\n\t\t%04x %s (%s) %d (%s) %d %d %d",
		a->adfile.ainext, DS[a->adfile.aitype], (strbase+a->adfile.aiinflnm),
		a->adfile.aiinlnno, (strbase+a->adfile.aifilnam), a->adfile.ainadscs,
		a->adfile.aifill2, (unsigned) a->adfile.aifilflg );
}
prtds8(a, type)
ACTITEM *a;
int type;
{
/*
 * Print one AIEVEXPR data structure
 */
	ENODE e;

	printf( "\n\t%s%04x %s (%s) %d %04x\n\n\t\tROOT:",
		(type == 1) ? "        " : "",
		a->evexpr.ainext, DS[a->evexpr.aitype], (strbase+a->evexpr.aiinflnm),
		a->evexpr.aiinlnno, a->evexpr.aiexptr );
	
	fread( &e, 1, sizeof(ENODE), trnfdes );

	prtds9(&e);
	printf( "\n" );
}
prtds9(e)
ENODE *e;
{
/*
 * Print one ENODE data structure
 */
	ENODE f;

	switch( (unsigned) e->gnode.exop ) {
	case INT:
		printf( "\n\t\t\tINT %08lx", e->vnode.exvalue );
		break;
	case DOT:
		printf( "\n\t\t\tDOT ()" );
		break;
	case NAME:
		printf( "\n\t\t\tNAME (%s)", e->pnnode.symbol );
		break;
	default:
		printf( "\n\t\t\t%s %04x %04x", OP[(unsigned) e->gnode.exop],
			e->gnode.exleft, e->gnode.exright );
		if( e->gnode.exleft ) {
			fread( &f, 1, sizeof(ENODE), trnfdes );
			printf( "\n\t\tLEFT DESENT:" );
			prtds9(&f);
			}
		if( e->gnode.exright ) {
			fread( &f, 1, sizeof(ENODE), trnfdes );
			printf( "\n\t\tRIGHT DESENT:" );
			prtds9(&f);
			}
	}

}
/*eject*/
prtds10()
{
/*
 * Read and print one EVEXPR data structure
 */

	ACTITEM a;

	fread( &a, 1, sizeof(ACTITEM), trnfdes );

	prtds8(&a, 0);
}
prtds11()
{
/*
 * Read and print one TVASSIGN data structure
 */

	TVASSIGN tv;

	fread( &tv, 1, sizeof(TVASSIGN), trnfdes );

	printf( "\n\t%04x %8.8s %d",
		tv.nxtslot, tv.funname, tv.slot );
}
