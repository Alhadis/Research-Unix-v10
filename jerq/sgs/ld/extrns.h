/*
  static char ID[] = "@(#) extrns.h: 1.11 8/18/83";
 */

#ifndef PAGING
#	define PAGING	0
#endif

extern char **argptr;		/* pointer to main arg list and arg */
extern int argcnt;		/*	count (argv,argc values	    */

extern int lineno;		/* line number of ifile being parsed */
extern char *curfilnm;		/* ptr to current input file name */
extern char aflag, dflag, cflag, iflag, mflag, rflag, sflag,
#if UNIX || XL
					xflag,
#endif
#if COMMON
					tflag, Mflag,
#endif
#if PAGING
					Fflag, 
#endif
#if PAGING
					zflag,
#endif
#if ILDOPT
	ildflag,
#endif
	    tvflag, Hflag, Nflag, Sflag, Vflag, Xflag;
extern int hflag, pflag, Bflag;
extern unsigned short VSflag;
extern short globfill;		/* default fill value */
extern short usrdeftv;		/* .tv input sections or SECTION */
extern short errlev;		/* error level */
#if PAGING
extern short bond_t_d;		/* text and data must be bound */
#endif
extern unsigned short magic;	/* magic number to be matched. */

extern struct listown ldfilist;	/* head of the LDFILE List	*/
extern struct listown bldoutsc;	/* head of the BLDOUTSEC List	*/
extern struct listown bondlist;	/* head of the BONDLIST		*/
extern struct listown ownlist;	/* head of the OWNLIST		*/
extern struct listown atownlst;	/* head of the ATTOWN List	*/
extern struct listown memlist;	/* head of the MEMLIST		*/
extern struct listown reglist;	/* head of the REGLIST		*/
extern struct listown infilist;	/* head of the INFILE List	*/
extern struct listown outsclst;	/* head of the OUTSEC List	*/
extern struct listown explist;	/* head of the expression List	*/
extern struct listown symlist;	/* head of the symbol list	*/
extern struct listown avlist;	/* head of the avail space list */
extern struct listown dsectlst;	/* head of the dsect list	*/

#if !ONEPROC
extern int symcnt, memcnt, regcnt, ldfcnt, bldcnt, expcnt;

extern char trnname[];		/* name of the PASS 1 -> PASS 2 */
extern FILE *trnfdes;		/* transfer file		*/
#if AR16WR
extern char *trnfbuf;
#endif
#endif

extern char *strbase, *strnext, *strlimit;
extern int strleng;

extern char outfilnm[];		/* name of output object file */
extern char epsymbol[];		/* a.out header entry point symbol */
extern int numoutsc;		/* number of output sections */
extern long noutsyms;		/* number of symbol entries written */

#if PORTAR
extern long ar_size;		/* size of archive .o */
#endif
#if FLEXNAMES
extern long tnumsyms;		/* total number of symbols in output file */
extern long stroffset;		/* offset of a string in string table */
#endif

extern long cur_dot;		/* current value of "dot" symbol */
extern ACTITEM *curexp;		/* current expression being evaluated */
extern long maxlcsms;		/* max no. of local symbols for an input file */
extern long symtborg;		/* file address of symbol table in output file */
extern long	allocspc,	/* amount of space allocated by myalloc;*/
		maxalloc;	/* used for statistics only */
extern int yydebug;
#if DMERT || XL
extern int dmertplib;
#endif
extern ADDRESS	memorg;





extern long	 alignment();
extern ITEMID	 allocate();
extern ACTITEM	*bldadfil();
extern ENODE	*buildtree();
extern ENODE	*cnstnode();
extern ACTITEM	*dfnscngrp();
extern long	 eval();
extern ANODE	*findnode();
extern OUTSECT	*findoscn();
extern ANODE	*findsanode();
extern AUXTAB	*findaux();
extern SYMTAB	*findsym();
extern INFILE	*fndinfil();
extern OUTSECT	*fndoutsec();
extern AUXTAB	*getaux();
extern SYMTAB	*getsym();
#if !ONEPROC
extern ACTITEM	*ldexp();
#endif
extern AUXTAB	*loopaux();
extern SYMTAB	*loopsym();
extern AUXTAB	*makeaux();
extern SYMTAB	*makesym();
extern char	*myalloc();
extern ANODE	*newnode();
extern REGION	*newregion();
extern long	 plocsyms();
extern OUTSECT	*proc_dfn_ai();
extern long	 psymtab();
extern char	*savefn();
extern INSECT	*sectnum();
extern char	*sname();
extern ANODE	*splitnode();
extern long	 swapb4();
extern ENODE	*symnode();
extern char	*zero();
extern char	*strncpy();
extern char	*strtabread( );
