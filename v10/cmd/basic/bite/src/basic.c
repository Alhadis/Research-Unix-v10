/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	****** MAIN PROGRAM FOR BITE ******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 14:38:50 1979 *****
 *	***********************************
 *	***********************************
 *
 * mod,J.P.Hawkins,17-SEP-79, encode "else" and "endif"

 * MOD,J.P.Hawkins to read environment for BITEPATH
 */
/*   "@(#) basic.c:  V 1.3  6/17/81" */

/*
 * THIS IS THE CALL INTERFACE TO BASIC
 *
 * the cmd format is
 * bite [-] prog1 prog2 ......
 *
 * where - causes the programs to load without executing
 *
 */
#include "bas.h"
extern	int	endfcnt;
extern	char	bpath[];
static int nquit;
quit()
{
	signal(2,quit);
	nquit = 1;
	printf("\n?\n");
	return(0);
}
int rem;
int atad;
int tv;
int tel;
int nxt;
int _sub;
int _fi;
int _togo;
int tospgo;
int subog;
int tsil;
int no;
int daol;
int evas;
int dlo;
int bat;
int esle;	/* else code */
int fidne;	/* endif code */
int rof;	/* for code */
int mr;
int sl;
/*
 *
 *
 */
char txtbuf[NMPAG*PGSIZ];
main(argc,argv,envp)
int argc;
char *argv[];
char *envp[];
{
	int	seed = 1;
	int	i;
	int	loadgo, j;
	endfcnt = 0;	/* initialize structured "if" nest count */
	loadgo = 0;	/* init load and go flag */
	/*
	 * Look for BITEPATH in environment and
	 * append to end of internal search path.
	 * Users can define their own searchpath for BITE
	 * by adding BITEPATH=:path1:path2: etc. in their profile
	 */
	for(i=0; envp[i] != 0; i++)
	{
		if(strncmp("BITEPATH",envp[i],8) == 0)
		{
			strcat(bpath,&envp[i][9]);
			break;
		}
	}
	linptr=lbdptr=curptr=txtbuf;
	signal(2,quit);
	rem=encode("rem");
	tv=encode("n");
	tospgo=encode("go to");
	tsil=encode("list");
	atad=encode("data");
	daol = encode("load");
	evas = encode("save");
	dlo = encode("old");
	bat = encode("!");
	nxt=encode("next");
	_sub=encode("s");
	_fi=encode("if");
	subog=encode("gosub");
	no = encode("on");
	_togo=encode("goto");
	tel=encode("\010");
	esle=encode("else");
	fidne=encode("endif");
	rof=encode("for");
	sl=encode("ls");
	mr=encode("mr");
	initvar();
	srand(seed);	/* init random number gen */
#ifdef TEST
	ibinit(); /* IEEE bus init */
	bdvminit(); /* digital voltmeter init */
#endif
#ifdef LSX
	iinit(); /* keyboard interrupt enable */
#endif
	if(argc > 1)
	{
		j = 1;			/* init arg index to 1 */
		if(argc > 2 && *argv[1] == '-') /* run flag? */
		{
			j = 2;		/* load starting at arg 2 */
			--argc;		/* decrement arg count */
		}
		else
		{
			loadgo = 1;	/* set run flag */
		}
		for(i=j;--argc;i++)
		{
			expr = argv[i];
			load();
		}
		if(loadgo)	/* run? */
		{
			expr = linbuf;
			*expr = '\0';
			run();
		}
	}
	while(1)
	{
		nquit = 0;
		if(bed(stdin) < 0 && nquit != 1) break;
	}
}
nocode(code)
int	code;
{
	int i;
	static int *list[] =
		{ &daol,
		&evas,
		&dlo,
		&rem,
		&bat,
		&_sub,
		&mr,
		&sl,
		0 };
	for(i=0;list[i] !=0;i++)
	{
		if(code == *list[i])
		return(1);
	}
		return(0);
}
