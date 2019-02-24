/*	@(#)conn.c	1.16
*/
#include "uucp.h"
VERSION(@(#)conn.c	1.16);

static char _ProtoStr[20] = "";	/* protocol string from Systems file entry */
extern jmp_buf Sjbuf;

int alarmtr();
static void getProto();
/* static getto(), finds();   PUT this back after altconn is included */
extern getto();
static finds();

static notin(), ifdate(), classmatch();

extern struct caller caller[];

/* Needed for cu for force which line will be used */
#ifdef STANDALONE
extern char *Myline;
#endif STANDALONE

/*
 * conn - place a telephone call to system and login, etc.
 *
 * return codes:
 *	FAIL - connection failed
 *	>0  - file no.  -  connect ok
 * When a failure occurs, Uerror is set.
 */

conn(system)
char *system;
{
	int nf, fn = FAIL;
	char *flds[F_MAX+1];
	FILE *fsys;
	int (*oalrm)();
#ifdef MANYSYS
#define	SYSCLOSE	pclose
	FILE *sysopen();
#else
#define	SYSCLOSE	fclose
#endif

	CDEBUG(4, "conn(%s)\n", system);
#ifndef MANYSYS
	fsys = fopen(SYSFILE, "r");
#else
	fsys = sysopen(system);
#endif
	ASSERT(fsys != NULL, Ct_OPEN, SYSFILE, errno);
	Uerror = 0;
	while ((nf = finds(fsys, system, flds, F_MAX)) > 0) {
		fn = getto(flds);
		CDEBUG(4, "getto ret %d\n", fn);
		if (fn < 0)
		    continue;
#ifdef STANDALONE
		SYSCLOSE(fsys);
		return(fn);
#else
		if (chat(nf - F_LOGIN, flds + F_LOGIN, fn,"","") == SUCCESS) {
			SYSCLOSE(fsys);
			return(fn); /* successful return */
		}

		/* login failed */
		DEBUG(6, "close caller (%d)\n", fn);
		if (setjmp(Sjbuf) == 0) {
			oalrm = signal(SIGALRM, alarmtr);
			alarm(30);
			close(fn);
		}
		alarm(0);
		signal(SIGALRM, oalrm);
		if (Dc[0] != NULLCHAR) {
			DEBUG(6, "delock(%s)\n", Dc);
			delock(Dc);
		}
#endif
	}

	/* finds or getto failed */
	SYSCLOSE(fsys);
	CDEBUG(1, "Call Failed: %s\n", UERRORTEXT);
	return(FAIL);
}

/*
 * getto - connect to remote machine
 *
 * return codes:
 *	>0  -  file number - ok
 *	FAIL  -  failed
 */

getto(flds)
char *flds[];
{
	FILE *dfp;
	char *dev[D_MAX+2], devbuf[BUFSIZ];
	register int status;
	register int dcf = -1;
	int reread = 0;
	int tries = 0;	/* count of call attempts - for limit purposes */

	CDEBUG(1, "Device Type %s wanted\n", flds[F_TYPE]);
	dfp = fopen(DEVFILE, "r");
	ASSERT(dfp != NULL, Ct_OPEN, DEVFILE, errno);
	Uerror = 0;
	while (tries < TRYCALLS) {
		if ((status=rddev(dfp, flds[F_TYPE], dev, devbuf, D_MAX)) == FAIL) {
			if (tries == 0 || ++reread >= TRYCALLS)
				break;
			rewind(dfp);
			continue;
		}
		/* check class, check (and possibly set) speed */
		if (classmatch(flds, dev) != SUCCESS)
			continue;
		if ((dcf = processdev(flds, dev)) >= 0)
			break;

		switch(Uerror) {
		case SS_CANT_ACCESS_DEVICE:
		case SS_DEVICE_FAILED:
		case SS_LOCKED_DEVICE:
			break;
		default:
			tries++;
			break;
		}
	}
	(void) fclose(dfp);
	if (status == FAIL && !Uerror) {
		CDEBUG(1, "Requested Device Type Not Found\n", 0);
		Uerror = SS_NO_DEVICE;
	}
	return(dcf);
}

/*
 * classmatch - process 'Any' in Devices and Systems and
 * 	determine the correct speed, or match for ==
 */

static int
classmatch(flds, dev)
char *flds[], *dev[];
{
	/* check class, check (and possibly set) speed */
	if (EQUALS(flds[F_CLASS], "Any")
	   && EQUALS(dev[D_CLASS], "Any")) {
		dev[D_CLASS] = flds[F_CLASS] = DEFAULT_BAUDRATE;
		return(SUCCESS);
	} else if (EQUALS(dev[F_CLASS], "Any")) {
		dev[D_CLASS] = flds[F_CLASS];
		return(SUCCESS);
	} else if (EQUALS(flds[F_CLASS], "Any")) {
		flds[D_CLASS] = dev[F_CLASS];
		return(SUCCESS);
	} else if (EQUALS(flds[F_CLASS], dev[D_CLASS]))
		return(SUCCESS);
	else
		return(FAIL);
}


/***
 *	rddev - find and unpack a line from device file for this caller type 
 *	lines starting with whitespace of '#' are comments
 *
 *	return codes:
 *		>0  -  number of arguments in vector - succeeded
 *		FAIL - EOF
 ***/

rddev(fp, type, dev, buf, devcount)
FILE *fp;
char *type;
char *dev[];
char *buf;
{
	int na;

	while (fgets(buf, BUFSIZ, fp) != NULL) {
		if (buf[0] == ' ' || buf[0] == '\t'
		    ||  buf[0] == '\n' || buf[0] == '\0' || buf[0] == '#')
			continue;
		na = getargs(buf, dev, devcount);
		ASSERT(na >= D_CALLER, "BAD LINE", buf, na);

/* For cu -- to force the requested line to be used */
#ifdef STANDALONE
		if ((Myline != NULL) && (!EQUALS(Myline, dev[D_LINE])) )
		    continue;
#endif STANDALONE

		bsfix(dev);	/* replace \X fields */
		if (EQUALS(dev[D_TYPE], type))
			return(na);
	}
	return(FAIL);
}


/*
 * finds	- set system attribute vector
 *
 * input:
 *	fsys - open Systems file descriptor
 *	sysnam - system name to find
 * output:
 *	flds - attibute vector from Systems file
 *	fldcount - number of fields in flds
 * return codes:
 *	>0  -  number of arguments in vector - succeeded
 *	FAIL - failed
 * Uerror set:
 *	0 - found a line in Systems file
 *	SS_BADSYSTEM - no line found in Systems file
 *	SS_TIME_WRONG - wrong time to call
 */

static
finds(fsys, sysnam, flds, fldcount)
char *sysnam, *flds[];
FILE *fsys;
{
	static char info[BUFSIZ];
	int na;

	/* format of fields
	 *	0 name;
	 *	1 time
	 *	2 acu/hardwired
	 *	3 speed
	 *	etc
	 */
	while (fgets(info, BUFSIZ, fsys) != NULL) {
		if (info[0] == '#')
			continue;
		na = getargs(info, flds, fldcount);
		bsfix(flds);	/* replace \X fields */
		if ( !EQUALSN(sysnam, flds[F_NAME], SYSNSIZE))
			continue;
#ifndef STANDALONE
		if (ifdate(flds[F_TIME])) {
			/*  found a good entry  */
			getProto(flds[F_TYPE]);
			Uerror = 0;
			return(na);	/* FOUND OK LINE */
		}
		CDEBUG(1, "Wrong Time To Call: %s\n", flds[F_TIME]);
		Uerror = SS_TIME_WRONG;
#else
			Uerror = 0;
			return(na);	/* FOUND OK LINE */
#endif
	}
	if (!Uerror)
		Uerror = SS_BADSYSTEM;
	return(FAIL);
}

/*
 * getProto - get the protocol letters from the input string.
 * input:
 *	str - string from Systems file (flds[F_TYPE])--the ,
 *		delimits the protocol string
 *		e.g. ACU,g or DK,d
 * output:
 *	str - the , (if present) will be replaced with NULLCHAR
 *	global ProtoStr will be modified
 * return:  none
 */

static
void
getProto(str)
char *str;
{
	register char *p;
	if ( (p=strchr(str, ',')) != NULL) {
		*p = NULLCHAR;
		(void) strcpy(_ProtoStr, p+1);
		DEBUG(7, "ProtoStr = %s\n", _ProtoStr);
	}
	else
		*_ProtoStr = NULLCHAR;
}

/*
 * check for a specified protocol selection string
 * return:
 *	protocol string pointer
 *	NULL if none specified for LOGNAME
 */
char *
protoString()
{
	return(_ProtoStr[0] == NULLCHAR ? NULL : _ProtoStr);
}

static int _Echoflag;
static int _Slowflag;
/*
 * chat -	do conversation
 * input:
 *	flds - fields from Systems file
 *	nf - number of fields in flds array
 *	dcr - write file number
 *	phstr1 - phone number to replace \D
 *	phstr2 - phone number to replace \T
 *
 *	return codes:  0  |  FAIL
 */

chat(nf, flds, fn, phstr1, phstr2)
char *flds[], *phstr1, *phstr2;
int nf, fn;
{
	char *want, *altern;
	extern char *strchr();
	int k, ok;

	_Echoflag = 0;
	_Slowflag = 0;
	for (k = 0; k < nf; k += 2) {
		want = flds[k];
		ok = FAIL;
		while (ok != 0) {
			altern = strchr(want, '-');
			if (altern != NULL)
				*altern++ = NULLCHAR;
			ok = expect(want, fn);
			if (ok == 0)
				break;
			if (altern == NULL) {
				Uerror = SS_LOGIN_FAILED;
				logent(UERRORTEXT, "FAILED");
				return(FAIL);
			}
			want = strchr(altern, '-');
			if (want != NULL)
				*want++ = NULLCHAR;
			sendthem(altern, fn, phstr1, phstr2);
		}
		sleep(2);
		if (flds[k+1])
		    sendthem(flds[k+1], fn, phstr1, phstr2);
	}
	return(0);
}

/***
 *	expect(str, fn)	look for expected string
 *	char *str;
 *
 *	return codes:
 *		0  -  found
 *		FAIL  -  lost line or too many characters read
 *		some character  -  timed out
 *
 * There are many delicacies here.
 * -- keep the system calls in the inner loop to a minimum;
 * else a garrulous system might time out while we're reading
 * its chatter.
 * -- because systems may be garrulous but correct, the alarm
 * must be reset whenever a character arrives; time out only
 * if nothing has come for a while.  But putting the alarm inside
 * the inner loop slows it down too much; so put it outside,
 * and don't time out if anything arrives.  But that might let
 * the far side time out in some pathological cases, so wait
 * a shorter time if some character have already arrived.
 * -- reading several characters at a time might be a mistake;
 * we might consume something beyond the string we expect,
 * discard it here, but need it later.  Too bad for now.
 */

#define MR 2000		/* max chars before we give up */
#define	NNEXT 4		/* chars to read at a time */

expect(str, fn)
char *str;
int fn;
{
	static char rdvec[MR];
	register char *rp = rdvec;
	register int kr, c;
	char nextch[NNEXT];
	char dbuf[NNEXT*2 + 1];
	register char *dp, *cp;
	int nread;
	extern	errno;

	CDEBUG(4, "expect: (", 0);
	for (c=0; kr=str[c]; c++)
		if (kr < 040) {
			CDEBUG(4, "^%c", kr | 0100);
		} else {
			CDEBUG(4, "%c", kr);
		}
	CDEBUG(4, ")\n", 0);

	if (EQUALS(str, "\"\"")) {
		CDEBUG(4, "got it\n", 0);
		return(0);
	}
	nread = 0;
	if (setjmp(Sjbuf) && nread == 0) {
		return(FAIL);
	}
	(void) signal(SIGALRM, alarmtr);
	if (nread)
		alarm(MAXEXPECTTIME/2);		/* cheap hack */
	else
		alarm(MAXEXPECTTIME);
	nread = 0;
	do {
		errno = 0;
		kr = read(fn, nextch, NNEXT);
		if (kr <= 0) {
			alarm(0);
			CDEBUG(4, "lost line errno - %d\n", errno);
			logent("LOGIN", "LOST LINE");
			return(FAIL);
		}
		if (rp + kr >= rdvec + MR) {
			CDEBUG(4, "\ntoo much junk\n", 0);
			alarm(0);
			return(FAIL);
		}
		nread += kr;
		for (dp = dbuf, cp = nextch; --kr >= 0; ) {
			if ((c = *cp++ & 0177) == 0)
				continue;
			*rp++ = c;
			if (c >= 040)
				*dp++ = c;
			else {
				*dp++ = '^';
				*dp++ = c+0100;
			}
		}
		*dp = 0;
		CDEBUG(4, "%s", dbuf);
		*rp = NULLCHAR;
	} while (notin(str, rdvec));
	alarm(0);
	CDEBUG(4, "got it\n", 0);
	return(0);
}


/***
 *	alarmtr()  -  catch alarm routine for "expect".
 */

alarmtr()
{
	CDEBUG(6, "timed out\n", 0);
	longjmp(Sjbuf, 1);
}


/***
 *	sendthem(str, fn, phstr1, phstr2)	send line of chat sequence
 *	char *str, *phstr;
 *
 *	return codes:  none
 */

sendthem(str, fn, phstr1, phstr2)
char *str, *phstr1, *phstr2;
int fn;
{
	int sendcr = 1;
	register char	*sptr, *pptr;

	if (PREFIX("BREAK", str)) {
		/* send break */
		CDEBUG(5, "BREAK\n", 0);
		genbrk(fn);
		return;
	}

	if (EQUALS(str, "EOT")) {
		CDEBUG(5, "EOT\n", 0);
		(void) write(fn, EOTMSG, strlen(EOTMSG));
		return;
	}

	if (EQUALS(str, "\"\"")) {
		CDEBUG(5, "\"\"\n", 0);
		str += 2;
	}

	CDEBUG(5, "sendthem (", "");
	if (setjmp(Sjbuf))	/* Timer so echo check doesn't last forever */
		goto err;
	(void) signal(SIGALRM, alarmtr);
	alarm(MAXEXPECTTIME);
	for (sptr = str; *sptr; sptr++) {
		if (*sptr == '\\')
			switch(*++sptr) {
			case 'D':
				for (pptr=phstr1; *pptr; pptr++)
					if (wrchar(fn, pptr))
						goto err;
				continue;
			case 'T':
				for (pptr=phstr2; *pptr; pptr++)
					if (wrchar(fn, pptr))
						goto err;
				continue;
			case 'N':
				*sptr = 0;
				break;
			case 'd':
				CDEBUG(5, "DELAY\n", 0);
				sleep(2);
				continue;
			case 'c':
				if (*(sptr+1) == NULLCHAR) {
				CDEBUG(5, "<NO CR>", 0);
					sendcr = 0;
					continue;
				}
				CDEBUG(5, "<NO CR - MIDDLE IGNORED>\n", 0);
				continue;
			case 's':
				*sptr = ' ';
				break;
			case 'p':
				CDEBUG(5, "PAUSE\n", 0);
				nap(HZ/4);	/* approximately 1/4 second */
				continue;
			case 'E':
				CDEBUG(5, "ECHO CHECK ON\n", 0);
				_Echoflag = 1;
				continue;
			case 'e':
				CDEBUG(5, "ECHO CHECK OFF\n", 0);
				_Echoflag = 0;
				continue;
			case 'K':
				CDEBUG(5, "BREAK\n", 0);
				genbrk(fn);
				continue;
			case 'W':
				_Slowflag = 1;
				continue;
			case 'w':
				_Slowflag = 0;
				continue;
			case '\\':
				/* backslash escapes itself */
				break;
			default:
				/* send the backslash */
				--sptr;
				break;
			}
		if (wrchar(fn, sptr))
			goto err;
	}

	if (sendcr) {
		CDEBUG(5, "^M", 0);
		(void) write(fn, "\r", 1);
	}
err:
	alarm(0);
	CDEBUG(5, ")\n", 0);
	return;
}

wrchar(fn, sptr)
register int fn;
register char *sptr;
{
	if (GRPCHK(getgid()))	/* protect Systems file */
	{
		CDEBUG(5, "%s", *sptr < 040 ? "^" : "");
		CDEBUG(5, "%c", *sptr < 040 ? *sptr | 0100 : *sptr);
	}
	if ((write(fn, sptr, 1)) != 1)
		return(FAIL);
	if (_Echoflag) {
		char chr;
		while(1) {	/* Should set a timer here */
			if (read(fn, &chr, 1) != 1)
				return(FAIL);
			chr &= 0177;
			CDEBUG(4, "%s", chr < 040 ? "^" : "");
			CDEBUG(4, "%c", chr < 040 ? chr | 0100 : chr);
			if (chr == ((*sptr) & 0177))
				break;
		}

	}
	if (_Slowflag)
		sleep(1);
	return(SUCCESS);
}


/***
 *	notin(sh, lg)	check for occurrence of substring "sh"
 *	char *sh, *lg;
 *
 *	return codes:
 *		0  -  found the string
 *		1  -  not in the string
 */

static
notin(sh, lg)
register char *sh, *lg;
{
	while (*lg != NULLCHAR) {
		if (PREFIX(sh, lg))
			return(0);
		else
			lg++;
	}
	return(1);
}


/*******
 *	ifdate(s)
 *	char *s;
 *
 *	ifdate  -  this routine will check a string (s)
 *	like "MoTu0800-1730" to see if the present
 *	time is within the given limits.
 *	SIDE EFFECT - Retrytime is set to number following ";"
 *
 *	String alternatives:
 *		Wk - Mo thru Fr
 *		zero or one time means all day
 *		Any - any day
 *
 *	return codes:
 *		0  -  not within limits
 *		1  -  within limits
 */

static
ifdate(s)
char *s;
{
	static char *days[] = {
		"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa", 0
	};
	time_t	clock;
	int	t__now;
	char	*p, *strrchr(), *strchr();
	struct tm	*tp;

	time(&clock);
	tp = localtime(&clock);
	t__now = tp->tm_hour * 100 + tp->tm_min;	/* "navy" time */

	/*
	 *	pick up retry time for failures
	 *	global variable Retrytime is set here
	 */
	if ((p = strrchr(s, ';')) != NULL)
	    if (isdigit(p[1])) {
		if (sscanf(p+1, "%d", &Retrytime) < 1)
			Retrytime = 5;	/* 5 minutes is error default */
		Retrytime  *= 60;
		*p = NULLCHAR;
	    }

	while (*s) {
		int	i, dayok;

		for (dayok = 0; (!dayok) && isalpha(*s); s++) {
			if (PREFIX("Any", s))
				dayok = 1;
			else if (PREFIX("Wk", s)) {
				if (tp->tm_wday >= 1 && tp->tm_wday <= 5)
					dayok = 1;
			} else
				for (i = 0; days[i]; i++)
					if (PREFIX(days[i], s))
						if (tp->tm_wday == i)
							dayok = 1;
		}

		if (dayok) {
			int	t__low, t__high;

			while (isalpha(*s))	/* flush remaining day stuff */
				s++;

			if ((sscanf(s, "%d-%d", &t__low, &t__high) < 2)
			 || (t__low == t__high))
				return(1);

			/* 0000 crossover? */
			if (t__low < t__high) {
				if (t__low <= t__now && t__now <= t__high)
					return(1);
			} else if (t__low <= t__now || t__now <= t__high)
				return(1);

			/* aim at next time slot */
			if ((s = strchr(s, ',')) == NULL)
				break;
		}
		s++;
	}
	return(0);
}

/***
 *	char *
 *	fdig(cp)	find first digit in string
 *
 *	return - pointer to first digit in string or end of string
 */

char *
fdig(cp)
char *cp;
{
	char *c;

	for (c = cp; *c; c++)
		if (*c >= '0' && *c <= '9')
			break;
	return(c);
}


#ifdef FASTTIMER
/*	Sleep in increments of 60ths of second.	*/
nap (time)
register int time;
{
	static int fd;

	if (fd == 0)
		fd = open (FASTTIMER, 0);

	read (fd, 0, time);
}

#endif FASTTIMER

#ifdef BSD4_2

	/* nap(n) -- sleep for 'n' ticks of 1/60th sec each. */
	/* This version, by Mark Horton, uses the select system call */


nap(n)
unsigned n;
{
	struct timeval tv;
	int rc;

	if (n==0)
		return;
	tv.tv_sec = n/60;
	tv.tv_usec = ((n%60)*1000000L)/60;
	rc = select(32, 0, 0, 0, &tv);
}

#endif

#ifdef NONAP

/*	nap(n) where n is ticks
 *
 *	loop using n/HZ part of a second
 *	if n represents more than 1 second, then
 *	use sleep(time) where time is the equivalent
 *	seconds rounded off to full seconds
 *	NOTE - this is a rough approximation and chews up
 *	processor resource!
 */

nap(n)
unsigned n;
{
	struct tms	tbuf;
	long endtime;
	int i;

	if (n > HZ) {
		/* > second, use sleep, rounding time */
		sleep( (int) (((n)+HZ/2)/HZ) );
		return;
	}

	/* use timing loop for < 1 second */
	endtime = times(&tbuf) + 3*n/4;	/* use 3/4 because of scheduler! */
	while (times(&tbuf) < endtime) {
	    for (i=0; i<1000; i++, i*i)
		;
	}
	return;
}


#endif
