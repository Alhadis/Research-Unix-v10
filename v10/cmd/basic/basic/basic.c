/*	"@(#) basic.c: V 2.40 11/10/83"	*/

/*
 * THIS IS THE CALL INTERFACE TO BASIC
 *
 * the cmd format is
 * basic [-n] prog1 prog2 ......
 *
 * where -n causes the programs to load without executing
 *
 */

#include <stdio.h>
#include <values.h>
#include "bas.h"

extern	UCHAR	autflg,
		stpflg;
extern	char	*malloc(),
		_string[],
		bpath[];
extern	struct sym	*htab[];
extern	char	*optarg;
extern	int	optind;

struct usrrec {
	int	cod;
	long	addr;
} usrlist[10];

extern struct conrec	constack[],
			*contop;

static UCHAR 	nquit;

quit()
{
	signal(2,quit);
	nquit = 1;
	stpflg = 1;
	if (!autflg)
	{
		preprint();
		printf("?\n");
	}
	autflg = 0;
	return(0);
}

int	CHAINcmd,
	REMcmd,
	DATAcmd,
	LETcmd,
	NEXTcmd,
	SUBcmd,
	IFcmd,
	RESUMEcmd,
	GOTOcmd,
	GOSUBcmd,
	LLISTcmd,
	ONcmd,
	RUNcmd,
	BANGcmd,
	ELSEcmd,
	ENDIFcmd,
	FORcmd,
	GLOBcmd,
	RESTOREcmd,
	WHILEcmd,
	WENDcmd,
	QUOcmd,
	COMMONcmd,
	INPUTPcmd,
	PRINTPcmd,
	WRITEPcmd,
	RSETcmd,
	MIDcmd,
	ERLcmd,
	USRcmd;

float	maxfloat = MAXFLOAT;

#define ONE_K	1024

struct pages	*index;

char	*txtbuf;		/* pointer to beginning of user workspace */

long	NMPAG = 2 * ((int) (DFSIZ + ONE_K - 1) / (int) ONE_K);

FILE	*tmpfile();

FILE 	*tmpfp;

#if	u3b5	||	u3b2
#include	<sys/sys3b.h>
#include	<sys/param.h>

extern long	dmmoffset;
extern int	dmmfault;
extern		etext;

#define	DATASTART	0x80880000
#define LONGDATASTART	0x80900000
#endif


main(argc,argv,envp)
int	argc;
char	*argv[],
	*envp[];
{
	int	seed = 1,
		ch,				/* holds command line flags */
		i,
		files = 0,
		loadgo;

	char	buf[LINMAX+1];			/* holds command line files */

#if	u3b5	||	u3b2
	long	dmmstart,
		datastart;
#endif

	setsig();
#if	u3b5	||	u3b2
	if ((dmmstart = sys3b(S3BDMM, 1)) == -1)
	{
		printf("WARNING: System problem.  Cannot use BLOAD\n");
		dmmfault = 1;
	}
	else
	{
		if ((int)(&etext) >= DATASTART)
			datastart = LONGDATASTART;
		else
			datastart = DATASTART;
		dmmoffset = dmmstart - datastart;
	}
#endif
/*v8	setbuf(stdout, 0);		/* no output buffering */
	tmpfp = tmpfile();		/* make temporary file name for UNDO */
	loadgo = 1;		/* init load and go flag */
	contop = constack;

	/*
	 * Look for BPATH in environment and
	 * append to end of internal search path.
	 * Users can define their own searchpath for BASIC
	 * by adding BPATH=:path1:path2: etc. in their profile
	 */

	for (i = 0; envp[i] != 0; i++)
	{
		if (strncmp("BPATH",envp[i],5) == 0)
		{
			strcat(bpath,&envp[i][6]);
			break;
		}
	}
	signal(2,quit);
	CHAINcmd = encode("chain");
	RSETcmd = encode("rset");
	INPUTPcmd = encode("input#");
	PRINTPcmd = encode("print#");
	WRITEPcmd = encode("write#");
	COMMONcmd = encode("common");
	REMcmd = encode("rem");
	QUOcmd = encode("'");
	LLISTcmd = encode("llist");
	DATAcmd = encode("data");
	RUNcmd = encode("run");
	BANGcmd = encode("!");
	NEXTcmd = encode("next");
	RESUMEcmd = encode("resume");
	SUBcmd = encode("sub");
	GLOBcmd = encode("global");
	IFcmd = encode("if");
	GOSUBcmd = encode("gosub");
	ONcmd = encode("on");
	GOTOcmd = encode("goto");
	LETcmd = encode("\010");
	ELSEcmd = encode("else");
	ENDIFcmd = encode("endif");
	FORcmd = encode("for");
	RESTOREcmd = encode("restore");
	WHILEcmd = encode("while");
	WENDcmd = encode("wend");
	MIDcmd = funcsearch("mid$");
	ERLcmd = funcsearch("erl");
	USRcmd = funcsearch("usr");
	usrlist[0].cod = funcsearch("usr0");
	usrlist[1].cod = funcsearch("usr1");
	usrlist[2].cod = funcsearch("usr2");
	usrlist[3].cod = funcsearch("usr3");
	usrlist[4].cod = funcsearch("usr4");
	usrlist[5].cod = funcsearch("usr5");
	usrlist[6].cod = funcsearch("usr6");
	usrlist[7].cod = funcsearch("usr7");
	usrlist[8].cod = funcsearch("usr8");
	usrlist[9].cod = funcsearch("usr9");

	srand(seed);	/* init random number gen */
	if (argc > 1)
	{
		while ((ch = getopt(argc,argv,"s:n")) != EOF)
		{
			switch (ch)
			{
			case 's':
				NMPAG = 2 * ((int) (atoi(optarg) +
				       ONE_K - 1) / (int) ONE_K);
				break;
			case 'n':
				loadgo = 0;
				break;
			}
		}
	}

	if ((txtbuf = malloc((unsigned) NMPAG * PGSIZ + ENCLINMAX)) == NULL ||
		(index = (struct pages *) calloc((unsigned) NMPAG,
			(unsigned) sizeof(struct pages))) == NULL)
	{
		printf("ERROR  %d Bytes of Workspace Not Available\n",
					NMPAG * PGSIZ);
		goaway();				/* exit */
	}
	endofprog = linptr = lbdptr = curptr = txtbuf;
	clear_pages();
	for (i=0; i<HTSIZE; i++)
		htab[i] = NULL;
	initvar();
	initfiles();
	buf[0] = '"';
	while (argv[optind] != NULL)	/* pick up file name(s) */
	{
		strcpy(buf+1,argv[optind]);
		strcat(buf, "\"");
		expr = buf;
		merge();
		++optind;
		++files;
	}
	if (loadgo && files)	/* run? */
	{
		strcpy(_string, "run");
		bed(0);
	}
	while (1)
	{
		nquit = 0;
		if (bed(stdin) < 0 && nquit != 1)
		{
			break;
		}
		contop = constack;
	}
}



nocode(code)
UCHAR	code;
{
	int i;
	static int *list[] = {
		&DATAcmd,
		&REMcmd,
		&QUOcmd,
		&BANGcmd,
		&SUBcmd,
		&GLOBcmd,
		0 };

	for (i = 0; list[i] != 0; i++)
	{
		if (code == *list[i])
			return(1);
	}
		return(0);
}
