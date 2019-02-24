/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"
#include <signal.h>
#include "regs.h"
#include "space.h"

static	char	BADEQ[] = "unexpected `='";

BOOL	executing;
extern	char	*lp;

extern	char	lastc, peekc;
static	char	eqformat[ARB] = "z";
char	stformat[ARB] = "X\"= \"^i";

ADDR	ditto;

ADDR	dot;
WORD	dotinc;
WORD	adrval, cntval, loopcnt;
int	adrflg, cntflg;
int	adrsp, dotsp, ditsp;

/* command decoding */

command(buf,defcom)
register char	*buf;
char	defcom;
{
	int	modifier, regptr;
	char	savc;
	char	*savlp=lp;
	char	savlc = lastc;
	char	savpc = peekc;
	static char lastcom = '=';

	if (defcom == 0)
		defcom = lastcom;
	if (buf) {
		if (*buf==EOR)
			return(FALSE);
		clrinp();
		lp=buf;
	}
	do {
		if (adrflg=expr(0)) {
			dot=ditto=expv;
			dotsp=ditsp=expsp;
		}
		adrval=dot;
		adrsp=dotsp;
		if (rdc()==',' && expr(0)) {
			cntflg=TRUE;
			cntval=expv;
		} else {
			cntflg=FALSE;
			cntval=1;
			reread();
		}
		if (!eol(rdc()))
			lastcom=lastc;
		else {
			if (adrflg==0)
				dot=inkdot(dotinc);
			reread();
			lastcom=defcom;
		}
		switch(lastcom&STRIP) {
		case '/':
		case '=':
		case '?':
			acommand(lastcom & STRIP);
			break;

		case '>':
			lastcom=0; 
			savc=rdc();
			if ((regptr=getreg(savc)) != BADREG)
				rput(regptr, wtor(dot));
			else if ((modifier=varchk(savc)) != -1)	
				var[modifier]=dot;
			else	
				error("bad variable");
			break;

		case '!':
			lastcom=0;
			shell(); 
			break;

		case '$':
			lastcom=0;
			printtrace(nextchar()); 
			break;

		case ':':
			if (!executing) { 
				executing=TRUE;
				subpcs(nextchar());
				executing=FALSE;
				lastcom=0;
			}
			break;

		case 0:
			prints(DBNAME);
			break;

		default: 
			error("bad command");
		}
		flushbuf();
	} while (rdc()==';');
	if (buf == NULL)
		reread();
	else {
		clrinp();
		lp=savlp;
		lastc = savlc;
		peekc = savpc;
	} 
	return(adrflg && dot!=0);
}

/*
 * [/?][wml]
 */

acommand(pc)
char pc;
{
	register int itype;
	int eqcom;
	int star;
	ADDR savdot;
	
	switch (pc) {
	case '/':
		itype = CORF | DATASP; 
		break;

	case '=':
		itype = NOSP; 
		break;

	case '?':
		itype = SYMF | INSTSP; 
		break;
	}
	eqcom = FALSE;
	star = FALSE;
	if (pc == '=')
		eqcom = TRUE;
	else {
		if (rdc()=='*')
			star = TRUE; 
		else
			reread(); 
		if (star) {
			if (itype & SYMF)
				itype = SYMF | DATASP;
			else
				itype = CORF | INSTSP;
		}
		if ((adrsp & SPTYPE) == REGSP) {
			itype &=~ SPTYPE;
			itype |= REGSP;
		}
	}
	switch (rdc()) {
	case 'm':
		if (eqcom)
			error(BADEQ); 
		cmdmap(itype, star);
		break;

	case 'L':
	case 'l':
		if (eqcom)
			error(BADEQ); 
		cmdsrc(lastc, itype, itype);
		break;

	case 'W':
	case 'w':
		if (eqcom)
			error(BADEQ); 
		cmdwrite(lastc, itype);
		break;

	default:
		reread();
		getformat(eqcom ? eqformat : stformat);
		if (!eqcom)
			psymoff((WORD)dot, itype, itype & SYMF ?"?%16t":"/%16t");
		scanform(cntval,(eqcom?eqformat:stformat),itype,itype);
	}
}

cmdsrc(c, itype, ptype)
char c;
{
	register WORD w;
	register WORD locval, locmsk;
	ADDR savdot;

	if (c == 'L')
		dotinc = SZLONG;
	else
		dotinc = SZSHORT;
	savdot=dot;
	expr(1); 
	locval=expv;
	if (expr(0))
		locmsk=expv; 
	else
		locmsk = ~0;
	if (c == 'L') {
		for (;;) {
			w = ltow(lget(dot, itype));
			if (errflg || mkfault
			||  (w & locmsk) == locval)
				break;
			dot = inkdot(dotinc);
		}
	}
	else {
		for (;;) {
			w = stow(sget(dot, itype));
			if (errflg || mkfault
			||  (w & locmsk) == locval)
				break;
			dot = inkdot(dotinc);
		}
	}
	if (errflg) { 
		dot=savdot; 
		errflg="cannot locate value";
	}
	psymoff((WORD)dot,ptype,"");
}

cmdwrite(wcom, itype)
char wcom;
int itype;
{
	ADDR savdot;
	char format[2];

	format[0] = wcom == 'w' ? 'r' : 'R';
	format[1] = 0;
	expr(1);
	do {  
		savdot=dot;
		psymoff((WORD)dot, itype, itype & SYMF ?"?%16t":"/%16t"); 
		exform(1,format,itype,itype);
		errflg=0; 
		dot=savdot;
		if (wcom == 'W')
			lput(dot,itype,wtol(expv));
		else
			sput(dot,itype,wtos(expv));
		savdot=dot;
		printf("=%8t"); 
		exform(1,format,itype,itype);
		newline();
	} while (expr(0) && errflg==0);
	dot=savdot;
	chkerr();
}

/*
 * collect a register name; return register offset
 * this is not what i'd call a good division of labour
 */

int
getreg(regnam)
{
	char	buf[LINSIZ];
	register char *p;
	register int c;

	p = buf;
	*p++ = regnam;
	while (isalnum(c = readchar()))
		*p++ = c;
	*p = 0;
	reread();
	return (rname(buf));
}

/*
 * shell escape
 */

extern (*sigint)(), (*sigqit)();

shell()
{
	int	rc, status, unixpid;
	char *argp = lp;
	int (*isig)();

	while (lastc!=EOR)
		rdc();
	if ((unixpid=fork())==0) {
		signal(SIGINT, sigint);
		signal(SIGQUIT, sigqit);
		*lp=0;
		execl("/bin/sh", "sh", "-c", argp, 0);
		_exit(16);
	} else if (unixpid == -1) {
		error("cannot fork");
	} else {
		isig = signal(SIGINT, SIG_IGN);
		while ((rc = wait(&status)) != unixpid && rc != -1)
			;
		signal(SIGINT, isig);
		prints("!"); 
		reread();
	}
}
