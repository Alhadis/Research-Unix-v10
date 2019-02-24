#include <sys/types.h>			/* needed for socket.h */
#include <sys/uio.h>			/* needed for socket.h */
#include <sys/socket.h>
#include <sysexits.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/nameser.h>
#include <string.h>

/* imports */
extern char *malloc(), *strcpy(), *inet_ntoa();

extern int ipcdebug;
extern char syserrstr[];

/* private */
#define MAXMXLIST 10
static struct mxitem {
	char *host;
	u_short pref;
	u_char localflag;
} MXlist[MAXMXLIST + 1];
static char *strsave();
static int buildmxlist();
static void mxsave(), mxinsert(), mxlocal();
static struct hostent *getmxhost();

mx_connect(dest, param)
char *dest, *param;
{
	int s, lport, mxfatal;
	char **addr;
	char host[100];
	struct hostent *hp;
	struct sockaddr_in sin;
	struct mxitem *mxp;
	extern int tcp_service();
	int port;
	char *bp;
	int errtype = 0;
	char errstr[256];

	strcpy(host, dest);

	if ((port = tcp_service(host)) < 0) {
		ipcsyserr("unknown service");
		return(-1);
	}

	mxfatal = buildmxlist(host);
	if (MXlist[0].host == 0)
		return(tcp_connect(dest, param));
		/*MXlist[0].host = host;*/

	(void) setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *) 0, 0);

	/* slop in the loop -- i hate the socket dance */
	for (mxp = MXlist; mxp->host; mxp++) {
		int i;

		if ((hp = getmxhost(mxp->host)) == 0) {
			if (mxfatal) {
				ipcsyserr("stub - can't find useful host"); /*EX_NOHOST*/
				return -1;
			}
			continue;
		}
		for (i=0; hp->h_addr_list[i]; i++) {
			s = socket(AF_INET, SOCK_STREAM, 0);
			if (s < 0) {
				ipcsyserr("Can't open socket"); /*EX_OSERR*/
				return -1;
			}
			bzero((char *)&sin, sizeof(sin));
			sin.sin_port = port;
			sin.sin_family = hp->h_addrtype;
			bcopy(hp->h_addr_list[i], (char *) &sin.sin_addr,
				hp->h_length);
			if (ipcdebug)
                                fprintf(stderr, "try %s [%s]\n", mxp->host,
				     inet_ntoa(sin.sin_addr));
			if (connect(s, (struct sockaddr *)&sin, sizeof(sin))>=0) {
				if (ipcdebug)
					fprintf(stderr, " connected\n");
				return s;
			}
			ipcgetsyserr();
			fprintf(stderr, "Couldn't connect to %s [%s]: %s\n",
				mxp->host, inet_ntoa(sin.sin_addr), syserrstr);
			if (errtype == 0 || 
			   (errtype != EX_TEMPFAIL && ipcmaperror()==EX_TEMPFAIL)) {
				errtype = ipcmaperror();
				strcpy(errstr, syserrstr);
			}
			close(s);
		}
	}
	ipcseterror(errtype, errstr, "Can't connect to host");
	return -1;
}

/* return 1 for fatal MX error (authoritative NXDOMAIN), 0 o.w. */
static int
buildmxlist(host)
	char *host;
{	register HEADER *hp;
	register char *cp;
	register int n;
	char q[PACKETSZ], a[PACKETSZ];	/* query, answer */
	char *eom, *bp;
	int buflen, ancount, qdcount;
	char hostbuf[BUFSIZ+1];
	u_short preference, reclen;
	int niter = 0;
	char nhostbuf[BUFSIZ+1];

again:
	if ((n = res_mkquery(QUERY, host, C_IN, T_MX, (char *) 0,
	    0, (struct rrec *) 0, q, sizeof(q))) < 0)
		return 0;
	n = res_send(q, n, a, sizeof(a));
	if (n < 0)
		return 0;
	if (ipcdebug)
		fprintf(stderr, "buildmxlist got %d\n", n);
	eom = a + n;
	hp = (HEADER *) a;
	ancount = ntohs(hp->ancount);
	qdcount = ntohs(hp->qdcount);
	if (ipcdebug)
		fprintf(stderr, "buildmx rcode %d ancount %d qdcount %d aa %d\n",
		 hp->rcode, ancount, qdcount, hp->aa);
	if (hp->rcode != NOERROR || ancount == 0) {
		if (hp->aa == 0)
			return 0;	/* non-authoritative in any event */
		return hp->rcode == NOERROR || hp->rcode == NXDOMAIN;
	}
	bp = hostbuf;
	buflen = sizeof(hostbuf);
	cp = a + sizeof(HEADER);
	if (ipcdebug>1) {
		int i;
		fprintf(stderr, "buildmx got:");
		for (i=0; i<n; i++)
			if (a[i]>=' '&& a[i]<0177)
				fprintf(stderr, "%c", a[i]);
			else fprintf(stderr, "\\%.3o", a[i]&0377);
		fprintf(stderr, "\n");
	}
	while (--qdcount >= 0)
		cp += dn_skipname(cp,eom) + QFIXEDSZ;
	/* TODO: if type is CNAME, reissue query */
	while (--ancount >= 0 && cp < eom) {
		int type;
		cp += dn_skipname(cp,eom);	/* name */
		type = _getshort(cp);	
		cp += sizeof(u_short)	/* type */
		    + sizeof(u_short)	/* class */
		    + sizeof(u_long);	/* ttl (see rfc973) */
		reclen = _getshort(cp);
		cp += sizeof(u_short);
		if (type==T_CNAME) {	/* canonical name */
			if (dn_expand(a, eom, cp, nhostbuf, BUFSIZ) < 0)
				if (ipcdebug)
					fprintf(stderr, "CNAME? in mxexpand\n");
			host = nhostbuf;
			if (++niter>10)
				return 0;
			if (ipcdebug)
				fprintf(stderr, "CNAME, try with %s\n", host);
			goto again;
		}
		preference = _getshort(cp);
		if ((n = dn_expand(a, eom, cp + sizeof(u_short), bp, buflen)) < 0)
			break;
		mxsave(bp, preference);
		cp += reclen;
	}
	mxlocal();
	return 0;
}

/* NOT TODO: issue WKS query.  (just try to connect.) */

static void
mxsave(host, pref)
	char *host;
	u_short pref;
{	struct mxitem *mxp;
	int localflag;
	static char thishost[64];

	if (*thishost == 0)
		gethostname(thishost, sizeof(thishost));
	if (ipcdebug)
		fprintf(stderr, "MXsave %s\n", host);

	if (MXlist[MAXMXLIST].host)
		return;				/* full */

	localflag = (strcmp(thishost, host) == 0);

	/* insertion sort */
	for (mxp = MXlist; mxp < MXlist + MAXMXLIST; mxp++) {
		if (mxp->host == 0) {
			mxinsert(mxp, host, pref, localflag);
			return;
		}
		if (pref < mxp->pref) {
			mxinsert(mxp, host, pref, localflag);
			return;
		}
		if (pref == mxp->pref) {
			if (mxp->localflag)
				return;
			if (localflag) {
				mxp->host = strsave(host);
				mxp->pref = pref;
				mxp->localflag = localflag;
				(++mxp)->host = 0;
				return;
			}
			mxinsert(mxp, host, pref, localflag);
			return;
		}
	}
}

static void
mxinsert(mxlistp, host, pref, localflag)
	struct mxitem *mxlistp;
	char *host;
	u_short pref;
{	register struct mxitem *mxp;

	for (mxp = MXlist + MAXMXLIST - 1; mxp > mxlistp; --mxp)
		*mxp = mxp[-1];
	mxp->host = strsave(host);
	mxp->pref = pref;
	mxp->localflag = localflag;
}

static char *
strsave(str)
	register char *str;
{	register char *rval;

	if ((rval = malloc(strlen(str) + 1)) == 0) {
		ipcsyserr("mx connect malloc");
		exit(1);
	}
	strcpy(rval, str);
	return rval;
}

static void
mxlocal()
{	register struct mxitem *mxp;

	if (MXlist[0].host == 0)
		return;

	for (mxp = MXlist; mxp->host; mxp++) {
		if (mxp->localflag) {
			mxp->host = 0;
			break;
		}
	}
}

static struct hostent *
getmxhost(host)
	char *host;
{	struct hostent *hp, *gethostbyname();
	long inet_addr();
	int err;
	char errbuf[256];
	static struct in_addr ia;
	static char *hlist[2] = {0, 0};
	static struct hostent he =
		{0, 0, AF_INET, sizeof(struct in_addr), hlist};

	if (!host)
		return 0;

	if ((hp = gethostbyname(host)) != 0)
		return hp;
	ipcgetsyserr();

	if ((ia.s_addr = inet_addr(host)) != -1) {
		he.h_addr = (char *)&ia;
		return &he;
	}
	sprintf(errbuf, "Error looking up MX Host '%s'.\n", host);
	ipcsyserr(errbuf);
	return 0;
}
