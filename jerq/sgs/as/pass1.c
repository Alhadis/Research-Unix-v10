static char *ID_pass1 = "@(#) pass1.c: 1.9 12/10/83";

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <paths.h>
#include "systems.h"
#include "symbols.h"
#include "gendefs.h"

/*
 *
 *	"pass1.c" is a file containing the main routine for the first
 *	pass of the assembler.  It is invoked with the command:
 *
 *		as1 [flags] ifile ofile t1 t2 t3 t4 t5 t6 t7
 *
 *	where {flags] are optional flags passed from pass 0,
 *	"ifile" is the name of the assembly language input file,
 *	"t1" through "t7" are the names of temporary files to be used
 *	by the assembler, and "ofile" is the file where the object code
 *	is to be written.  Pass 1 of the assembler reads "ifile" and
 *	writes the temporary text section to "t1", the temporary data
 *	section to "t2", and the symbol table to "t3".
 *
 *	The following things are done by this function:
 *
 *	1. Initialization. This consists of calling signal to catch
 *	   interrupt signals for hang-up, break, and terminate. Then
 *	   the argument list is processed by "getargs" followed by the
 *	   initialization of the symbol table with mnemonics for
 *	   instructions and pseudos-ops.
 *
 *	2. "yyparse" is called to do the actual first pass processing.
 *	   This is followed by a call to "cgsect". Normally this
 *	   function is used to change the section into which code
 *	   is generated. In this case, it is only called to make
 *	   sure that "dottxt", "dotdat", and "dotbss" contain the
 *	   proper values for the program counters for the respective
 *	   sections. The following symbols are then defined:
 *
 *		.text	This has a type of text and a value of zero.
 *			It is used to label the beginning of the text
 *			section, and later as a reference for relocation
 *			entries that are relative to the text section.
 *
 *		.data	This has a type of data and a value of zero.
 *			It is used to label the beginning of the data
 *			section, and later as a reference for relocation
 *			entries that are relative to the data section.
 *
 *		.bss	This has a type of bss and a value of zero.  It
 *			is used to label the beginning of the bss
 *			section, and later as a reference for relocation
 *			entries that are relative to the bss section.
 *
 *		(text)	This is a totally internal symbol used to
 *			remember the size of the text section.  It has
 *			characters in it that cannot legally be used in
 *			a symbol, and hence cannot be referenced or
 *			redefined by a user.
 *
 *		(data)	This is an internal symbol used to remember the
 *			size of the data section.
 *
 *		(bss)	This is an internal symbol used to remember the
 *			size of the bss section.
 *
 *		(sdicnt) This is the internal symbol used to remember
 *			the number of span dependent instructions
 *			on which optimizations were performed.
 *
 *	     3. The function "dmpstb" is called to dump the symbol
 *		table out to a temporary file to be used by pass 2 of
 *		the assembler.
 *
 *	4. The temporary files are closed and the next pass (if any)
 *	   is called.
 *
 */

#if ONEPROC
extern short passnbr;
#endif

extern char file[];

extern char *filenames[];

extern unsigned short
	line,
	sdicnt;


#if DEBUG
extern unsigned
	numcalls,
	numids,
	numcoll;
#endif

extern short
	anyerrs;

extern int
	aerror(),
	delexit(),
#if !ONEPROC
	dmpstb(),
#endif
	fixsyms(),
	flags(),
	flushbuf(),
	onintr();

extern FILE
	*fderr;

#if !ONEPROC
extern FILE
	*fdstab;
#endif

extern upsymins
	*lookup();

extern long
#if MULTSECT
	dottxt[4],
	dotdat[4],
#else
	dottxt,
	dotdat,
#endif
	dotbss;

#if !ONEPROC
char	*xargp[15];
#endif

short	opt = YES,
	workaround = YES,
	Oflag = NO;

#if M4ON
extern short rflag;
#endif

#if M32RSTFIX
short rstflag = YES;
#endif	/* M32RSTFIX */
#if ONEPROC
extern short
	transvec;
#else
short
	transvec = NO,
	argindex = 1;
#endif

#if ONEPROC
extern long newdot;
extern symbol *dot;
#else
long	newdot;
symbol	*dot;
#endif

FILE	*fdin,
#if !ONEPROC
	*fdstring,
	*fdlong,
#endif
	*fdtext,
#if MULTSECT
	*fdtxt1,
	*fdtxt2,
	*fdtxt3,
	*fddat1,
	*fddat2,
	*fddat3,
#endif
#if DEBUG
	*perfile,	/* performance data file descriptor */
#endif
	*fddata,
	*fdcomment;

#if MULTSECT
add1text(ptr)
symbol	*ptr;
{
	addsect(ptr,TXT,1);
}	/* add1text()	*/


add2text(ptr)
symbol	*ptr;
{
	addsect(ptr,TXT,2);
}	/* add2text()	*/


add3text(ptr)
symbol	*ptr;
{
	addsect(ptr,TXT,3);
}	/* add3text()	*/


add1data(ptr)
symbol	*ptr;
{
	addsect(ptr,DAT,1);
}	/* add1data()	*/


add2data(ptr)
symbol	*ptr;
{
	addsect(ptr,DAT,2);
}	/* add2data()	*/


add3data(ptr)
symbol	*ptr;
{
	addsect(ptr,DAT,3);
}	/* add3data()	*/

addsect(ptr,sectclass,sectnum)
symbol	*ptr;
short	sectclass,
	sectnum;
{
	if (((ptr->styp & TYPE) == sectclass) && (ptr->sectnum == sectnum))
		ptr->value += (sectclass == TXT) ? dottxt[0] : dotdat[0];
}	/* addsect()	*/

#endif

#if !ONEPROC
static char
	nextpass[80];

static char
	teststr[4] = {'-','t','\0'};
#endif

short tstlookup = NO;

static short
#if !ONEPROC
	filecnt = 0,
#endif
	testas = TESTVAL;

#if DEBUG
/*
 *	Performance data structure
 */
	long	ttime;
	struct	tbuffer {
		long	proc_user_time;
		long	proc_system_time;
		long	child_user_time;
		long	child_system_time;
		} ptimes;
	extern	long	times();

#endif
#if !ONEPROC
/*
 *
 *	"getargs" is a general purpose argument parsing routine.
 *	It locates flags (identified by a preceding minus sign ('-'))
 *	and initializes any associated flags for the assembler.
 *	If there are any file names in the argument list, then a
 *	pointer to the name is stored in the array "filenames" for
 *	later use.
 *
 */

getargs(xargc,xargv)
	register int xargc;
	register char **xargv;
{
	register char ch;
	
	while (xargc-- > 0) {
		if (**xargv == '-') {
			while ((ch = *++*xargv) != '\0')
				switch (ch) {
					case 'n':
						if( *++*xargv == 'f' ) {
							/* -nf option; disable work arounds */
							workaround = NO;
#if M32RSTFIX
							rstflag = NO;
#endif	/* M32RSTFIX */
						} else { /* -n option */
							opt = NO;
							*--*xargv;
						}
						break;
#if DEBUG
					case 'O':
						Oflag = YES;
						break;
#endif
#if M4ON
					case 'R':
						rflag = YES;
						xargp[argindex++] = "-R";
						break;
#endif
					case 'd':
						if (*++*xargv == 'l')
						      xargp[argindex++] = "-dl";
						break;
					case 't': {
						++*xargv;
#if TRANVEC
						if (**xargv == 'v'){
							transvec = YES;
							xargp[argindex++]="-tv";
							break;
						}
#endif
						if (isdigit(**xargv)) {
							testas = **xargv - '0' -1;
							if (testas > TESTVAL + 1) {
								teststr[2] = (char)(testas + '0');
							}
						}
						else {
							--*xargv;
							testas += 2;
						}
						xargp[argindex++] = teststr;
						break;
					}
#if DEBUG
					case 'T': {
						switch (*++*xargv) {
							case 'L': {
								tstlookup = YES;
								break;
							}
						}
						break;
					}
#endif
#if M32RSTFIX
					case 'r':
						rstflag = NO;
						break;
#endif	/* M32RSTFIX */
					default: {
						/* installation dependent flag? */
						flags(ch);
						break;
					}
				}
			xargv++;
		}
		else {
			filenames[filecnt++] = *xargv++;
		}
	}
}

main(argc,argv)
	int argc;
	char **argv;
#else

aspass1()

#endif
{
	register short i;
	register symbol *ptr;
	
#if ONEPROC
	passnbr = 1;
#endif
	if (signal(SIGHUP,SIG_IGN) == SIG_DFL)
		signal(SIGHUP,onintr);
	if (signal(SIGINT,SIG_IGN) == SIG_DFL)
		signal(SIGINT,onintr);
	if (signal(SIGTERM,SIG_IGN) == SIG_DFL)
		signal(SIGTERM,onintr);
	fderr = stderr;

#if DEBUG
/*	Performance data collected	*/
	ttime = times(&ptimes);
#endif

#if !ONEPROC
	strcpy(nextpass,argv[0]);
	argv++;
	argc--;
	getargs(argc,argv);
	if (filecnt < NFILES)
		aerror("Illegal number of temporary files");
	strcpy(file,filenames[0]);
#endif
	if ((fdin = fopen(file, "r")) == NULL)
		aerror("Unable to open input file");
	if ((fdtext = fopen(filenames[2], "w")) == NULL)
		aerror("Unable to open temporary (text) file");
#if MULTSECT
	if ((fdtxt1 = fopen(filenames[9],"w")) == NULL)
		aerror("Unable to open temporary (text 1) file");
	if ((fdtxt2 = fopen(filenames[10],"w")) == NULL)
		aerror("Unable to open temporary (text 2) file");
	if ((fdtxt3 = fopen(filenames[11],"w")) == NULL)
		aerror("Unable to open temporary (text 3) file");
	if ((fddat1 = fopen(filenames[12],"w")) == NULL)
		aerror("Unable to open temporary (data 1) file");
	if ((fddat2 = fopen(filenames[13],"w")) == NULL)
		aerror("Unable to open temporary (data 2) file");
	if ((fddat3 = fopen(filenames[14],"w")) == NULL)
		aerror("Unable to open temporary (data 3) file");
#endif
	if ((fddata = fopen(filenames[3], "w")) == NULL)
		aerror("Unable to open temporary (data) file");
	if ((fdcomment = fopen(filenames[8], "w")) == NULL)
		aerror("Unable to open temporary (comment) file");
#if !ONEPROC
	if ((fdstring = fopen(filenames[6], "w")) == NULL)
		aerror("Unable to open temporary (string) file");
#endif
#if FLEXNAMES
	strtabinit();
#endif
	creasyms();
	dot = (*lookup(".",INSTALL, USRNAME)).stp;
	dot->styp = TXT;
	dot->value = newdot = 0L;
#if MULTSECT
	dot->sectnum = 0;
#endif
	yyparse();	/* pass 1 */
	fclose(fdin);
#if !ONEPROC
	fflush(fdstring);
	if (ferror(fdstring))
		aerror("trouble writing; probably out of temp-file space");
	fclose(fdstring);
#endif
#if MULTSECT
	cgsect(TXT,0);
#else
	cgsect(TXT);
#endif
	flushbuf();
#if MULTSECT
	fclose(fdtxt1);
	fclose(fdtxt2);
	fclose(fdtxt3);
	fclose(fddat1);
	fclose(fddat2);
	fclose(fddat3);

	traverse(add1text);
	dottxt[0] += dottxt[1];
	traverse(add2text);
	dottxt[0] += dottxt[2];
	traverse(add3text);
	dottxt[0] += dottxt[3];

	traverse(add1data);
	dotdat[0] += dotdat[1];
	traverse(add2data);
	dotdat[0] += dotdat[2];
	traverse(add3data);
	dotdat[0] += dotdat[3];
#endif
	fflush(fdtext);
	if (ferror(fdtext))
		aerror("trouble writing; probably out of temp-file space");
	fclose(fdtext);
	fflush(fddata);
	if (ferror(fddata))
		aerror("trouble writing; probably out of temp-file space");
	fclose(fddata);
	fflush(fdcomment);
	if (ferror(fdcomment))
		aerror("trouble writing; probably out of temp-file space");
	fclose(fdcomment);

#if !ONEPROC
	if ((fdlong = fopen(filenames[5], "w")) == NULL)
		aerror("Unable to open temporary (sdi) file");
#endif
	fixsyms();
#if MULTSECT
	cgsect(TXT,0);
#else
	cgsect(TXT); /* update "dottxt" */
#endif
#if !ONEPROC
	fflush(fdlong);
	if (ferror(fdlong))
		aerror("trouble writing; probably out of temp-file space");
	fclose(fdlong);
#endif

	ptr = (*lookup(".text", INSTALL, USRNAME)).stp;
	ptr->styp = TXT;
	ptr->value = 0L;
	ptr = (*lookup("(text)", INSTALL, USRNAME)).stp;
	ptr->styp = TXT;
#if MULTSECT
	ptr->value = dottxt[0];
#else
	ptr->value = dottxt;
#endif
	ptr = (*lookup(".data", INSTALL, USRNAME)).stp;
	ptr->styp = DAT;
	ptr->value = 0L;
	ptr = (*lookup("(data)", INSTALL, USRNAME)).stp;
	ptr->styp = DAT;
#if MULTSECT
	ptr->value = dotdat[0];
#else
	ptr->value = dotdat;
#endif
	ptr = (*lookup(".bss", INSTALL, USRNAME)).stp;
	ptr->styp = BSS;
	ptr->value = 0L;
	ptr = (*lookup("(bss)", INSTALL, USRNAME)).stp;
	ptr->styp = BSS;
	ptr->value = dotbss;

#if !ONEPROC
	ptr = (*lookup("(sdicnt)",INSTALL,USRNAME)).stp;
	ptr->value = (long)sdicnt; /* has to be set after fixsyms is called */
	ptr->styp = ABS;

	if ((fdstab = fopen(filenames[4], "w")) == NULL)
		aerror("Unable to open temporary (symtab) file");
	dmpstb();	/* dump the symbol table for the next pass */
	fflush(fdstab);
	if (ferror(fdstab))
		aerror("trouble writing; probably out of temp-file space");
	fclose(fdstab);
#endif

#if DEBUG
	if (tstlookup) {
		printf("Number of calls to lookup: %u\n",numcalls);
		printf("Number of identifiers: %u\n",numids);
		printf("Number of identifier collisions: %u\n",numcoll);
		fflush(stdout);
	}
/*
 *	Performance data collected and written out here
 */

	ttime = times(&ptimes) - ttime;
	if ((perfile = fopen("as.info", "r")) != NULL ) {
		fclose(perfile);
		if ((perfile = fopen("as.info", "a")) != NULL ) {
			fprintf(perfile,
			   "as1\t%07ld\t%07ld\t%07ld\t%07ld\t%07ld\tpass 1\n",
			    ttime, ptimes);
			fclose(perfile);
		}
	}

#endif

	if (!anyerrs) {
#if ONEPROC
		return(aspass2());
#else
		nextpass[strlen(nextpass) - 1] = '2';
		xargp[0] = nextpass;
		for (i=0; i < filecnt; ++i)
			xargp[argindex++] = filenames[i];
		if (testas != TESTVAL + 1) {
			if (testas > TESTVAL + 1){
				execv(NAS2,xargp);
			}
			else
				execv(AS2,xargp);
			aerror("Unable to exec pass 2");
		}
#endif
	}
	else {
		delexit();
	}
}
