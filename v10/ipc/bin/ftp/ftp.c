#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <varargs.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ipc.h>
#include <sys/inet/in.h>
#include <sys/inet/tcp.h>


#include "ftp.h"
#include "telnet.h"
#include "ftp_var.h"

int	data = -1;
int	abrtflag = 0;
int	ptflag = 0;
int	connected;
int	getuid();

in_addr myaddr;
tcp_port dataport;
char *mynetname;
char *mymachname;

FILE	*cin, *cout;
FILE	*dataconn();

char *
hookup(host)
	char *host;
{
	int s;
	char *fields[4];
	static char myname[128];

	s = ipcopen(ipcpath(host, "tcp", "ftp"), "light");
	if (s < 0) {
		fprintf(stderr, "%s opening connection to %s\n", errstr, host);
		goto bad;
	}
	strcpy(myname, ipcname);
	setfields("!");
	getmfields(myname, fields, 4);
	mynetname = fields[0];
	mymachname = fields[1];
	myaddr = in_aton(mymachname);
	hostname = host;

	cin = fdopen(s, "r");
	cout = fdopen(s, "w");
	if (cin == NULL || cout == NULL) {
		fprintf(stderr, "ftp: fdopen failed.\n");
		if (cin)
			(void) fclose(cin);
		if (cout)
			(void) fclose(cout);
		code = -1;
		goto bad;
	}
	if (verbose)
		printf("Connected to %s.\n", hostname);
	if (getreply(0) > 2) { 	/* read startup message from server */
		if (cin)
			(void) fclose(cin);
		if (cout)
			(void) fclose(cout);
		code = -1;
		goto bad;
	}
	return (hostname);

bad:
	(void) close(s);
	return ((char *)0);
}

login(host)
	char *host;
{
	char tmp[80];
	char *user, *pass, *acct, *getlogin(), *getpass();
	int n, aflag = 0;

	user = pass = acct = 0;
	if (ruserpass(host, &user, &pass, &acct) < 0) {
		code = -1;
		return(0);
	}
	if (user == NULL) {
		char *myname = getlogin();

		if (myname == NULL) {
			struct passwd *pp = getpwuid(getuid());

			if (pp != NULL)
				myname = pp->pw_name;
		}
		printf("Name (%s:%s): ", host, myname);
		(void) fgets(tmp, sizeof(tmp) - 1, stdin);
		tmp[strlen(tmp) - 1] = '\0';
		if (*tmp == '\0')
			user = myname;
		else
			user = tmp;
	}
	n = command("USER %s", user);
	if (n == CONTINUE) {
		if (pass == NULL)
			pass = getpass("Password:");
		n = command("PASS %s", pass);
	}
	if (n == CONTINUE) {
		aflag++;
		acct = getpass("Account:");
		n = command("ACCT %s", acct);
	}
	if (n != COMPLETE) {
		fprintf(stderr, "Login failed.\n");
		return (0);
	}
	if (!aflag && acct != NULL)
		(void) command("ACCT %s", acct);
	if (proxy)
		return(1);
	for (n = 0; n < macnum; ++n) {
		if (!strcmp("init", macros[n].mac_name)) {
			(void) strcpy(line, "$init");
			makeargv();
			domacro(margc, margv);
			break;
		}
	}
	return (1);
}

/*VARARGS1*/
command(fmt, va_alist)
	char *fmt;
	va_dcl
{
	int r;
	va_list ap;

	va_start(ap);
	if (debug) {
		printf("---> ");
		_doprnt(fmt, ap, stdout);
		printf("\n");
		(void) fflush(stdout);
	}
	if (cout == NULL) {
		perror ("No control connection for command");
		code = -1;
		return (0);
	}
	_doprnt(fmt, ap, cout);
	fprintf(cout, "\r\n");
	(void) fflush(cout);
	va_end(ap);
	cpend = 1;
	r = getreply(!strcmp(fmt, "QUIT"));
	return(r);
}

char reply_string[BUFSIZ];

#include <ctype.h>

getreply(expecteof)
	int expecteof;
{
	register int c, n;
	register int dig;
	register char *cp;
	int originalcode = 0, continuation = 0;
	int pflag = 0;
	char *pt = pasv;

	cp = reply_string;
	for (;;) {
		dig = n = code = 0;
		while ((c = getc(cin)) != '\n') {
			if (c == IAC) {     /* handle telnet commands */
				switch (c = getc(cin)) {
				case WILL:
				case WONT:
					c = getc(cin);
					fprintf(cout, "%c%c%c",IAC,DONT,c);
					(void) fflush(cout);
					break;
				case DO:
				case DONT:
					c = getc(cin);
					fprintf(cout, "%c%c%c",IAC,WONT,c);
					(void) fflush(cout);
					break;
				default:
					break;
				}
				continue;
			}
			dig++;
			if (c == EOF) {
				if (expecteof) {
					code = 221;
					return (0);
				}
				lostpeer();
				if (verbose) {
					printf("421 Service not available, remote server has closed connection\n");
					(void) fflush(stdout);
				}
				code = 421;
				return(4);
			}
			if (c != '\r' && (verbose > 0 ||
			    (verbose > -1 && n == '5' && dig > 4))) {
				if (proxflag &&
				   (dig == 1 || dig == 5 && verbose == 0))
					printf("%s:",hostname);
				(void) putchar(c);
			}
			if (dig < 4 && isdigit(c))
				code = code * 10 + (c - '0');
			if (!pflag && code == 227)
				pflag = 1;
			if (dig > 4 && pflag == 1 && isdigit(c))
				pflag = 2;
			if (pflag == 2) {
				if (c != '\r' && c != ')')
					*pt++ = c;
				else {
					*pt = '\0';
					pflag = 3;
				}
			}
			if (dig == 4 && c == '-') {
				if (continuation)
					code = 0;
				continuation++;
			}
			if (n == 0)
				n = c;
			*cp++ = c;
		}
		if (verbose > 0 || verbose > -1 && n == '5') {
			(void) putchar(c);
			(void) fflush (stdout);
		}
		if (continuation && code != originalcode) {
			if (originalcode == 0)
				originalcode = code;
			continue;
		}
		*cp = '\0';
		if (n != '1')
			cpend = 0;
		if (code == 421 || originalcode == 421)
			lostpeer();
		return (n - '0');
	}
}

empty(mask, sec)
	struct fd_set *mask;
	int sec;
{
	return(select(32, mask, (struct fd_set *) 0, 1000*sec));
}

#define HASHBYTES 4096

sendrequest(cmd, local, remote)
	char *cmd, *local, *remote;
{
	FILE *fin, *dout = 0, *popen();
	int (*closefunc)(), pclose(), fclose();
	char buf[BUFSIZ], *bufp;
	long bytes = 0, hashbytes = HASHBYTES;
	register int c, d;
	struct stat st;
	long start, stop;
	char *mode;

	if (proxy) {
		proxtrans(cmd, local, remote);
		return;
	}
	closefunc = NULL;
	mode = "w";
	if (strcmp(local, "-") == 0)
		fin = stdin;
	else if (*local == '|') {
		fin = popen(local + 1, "r");
		if (fin == NULL) {
			perror(local + 1);
			code = -1;
			return;
		}
		closefunc = pclose;
	} else {
		fin = fopen(local, "r");
		if (fin == NULL) {
			perror(local);
			code = -1;
			return;
		}
		closefunc = fclose;
		if (fstat(fileno(fin), &st) < 0 ||
		    (st.st_mode&S_IFMT) != S_IFREG) {
			fprintf(stdout, "%s: not a plain file.\n", local);
			fclose(fin);
			code = -1;
			return;
		}
	}
	if (initconn()) {
		code = -1;
		if (closefunc != NULL)
			(*closefunc)(fin);
		return;
	}
	if (remote) {
		if (command("%s %s", cmd, remote) != PRELIM) {
			if (closefunc != NULL)
				(*closefunc)(fin);
			return;
		}
	} else
		if (command("%s", cmd) != PRELIM) {
			if (closefunc != NULL)
				(*closefunc)(fin);
			return;
		}
	dout = dataconn(mode);
	if (dout == NULL)
		goto abort;
	time(&start);
	switch (type) {

	case TYPE_I:
	case TYPE_L:
		errno = d = 0;
		while ((c = read(fileno(fin), buf, sizeof (buf))) > 0) {
			bytes += c;
			for (bufp = buf; c > 0; c -= d, bufp += d)
				if ((d = write(fileno(dout), bufp, c)) <= 0)
					break;
			if (hash) {
				while (bytes >= hashbytes) {
					(void) putchar('#');
					hashbytes += HASHBYTES;
				}
				(void) fflush(stdout);
			}
		}
		if (hash && bytes > 0) {
			if (bytes < HASHBYTES)
				(void) putchar('#');
			(void) putchar('\n');
			(void) fflush(stdout);
		}
		if (c < 0)
			perror(local);
		if (d <= 0) {
			if (d == 0)
				fprintf(stderr, "netout: write returned 0?\n");
			else if (errno != EPIPE) 
				perror("netout");
			bytes = -1;
		}
		break;

	case TYPE_A:
		while ((c = getc(fin)) != EOF) {
			if (c == '\n') {
				while (hash && (bytes >= hashbytes)) {
					(void) putchar('#');
					(void) fflush(stdout);
					hashbytes += HASHBYTES;
				}
				if (ferror(dout))
					break;
				(void) putc('\r', dout);
				bytes++;
			}
			(void) putc(c, dout);
			bytes++;
	/*		if (c == '\r') {			  	*/
	/*		(void)	putc('\0', dout);  /* this violates rfc */
	/*			bytes++;				*/
	/*		}                          			*/	
		}
		if (hash) {
			if (bytes < hashbytes)
				(void) putchar('#');
			(void) putchar('\n');
			(void) fflush(stdout);
		}
		if (ferror(fin))
			perror(local);
		if (ferror(dout)) {
			if (errno != EPIPE)
				perror("netout");
			bytes = -1;
		}
		break;
	}
	time(&stop);
	if (closefunc != NULL)
		(*closefunc)(fin);
	/*
	 * *** hack because linger call in ipc/tcpconnect
	 *     doesn't seem to work:
	 */
	sleep(5);
	(void) fclose(dout);
	(void) getreply(0);
	if (bytes > 0)
		ptransfer("sent", bytes, start, stop, local, remote);
	return;
abort:
	time(&stop);
	if (!cpend) {
		code = -1;
		return;
	}
	if (data >= 0) {
		(void) close(data);
		data = -1;
	}
	if (dout)
		(void) fclose(dout);
	(void) getreply(0);
	code = -1;
	if (closefunc != NULL && fin != NULL)
		(*closefunc)(fin);
	if (bytes > 0)
		ptransfer("sent", bytes, start, stop, local, remote);
}

recvrequest(cmd, local, remote, mode)
	char *cmd, *local, *remote, *mode;
{
	FILE *fout, *din = 0, *popen();
	int (*closefunc)(), pclose(), fclose();
	int oldverbose, oldtype = 0, is_retr, tcrflag, nfnd;
	char *bufp, *gunique(), msg;
	static char *buf;
	static int bufsize;
	long bytes = 0, hashbytes = HASHBYTES;
	struct fd_set mask;
	register int c, d;
	long start, stop;
	struct stat st;
	extern char *malloc();

	is_retr = strcmp(cmd, "RETR") == 0;
	if (proxy && is_retr) {
		proxtrans(cmd, local, remote);
		return;
	}
	closefunc = NULL;
	tcrflag = !crflag && is_retr;
	if (strcmp(local, "-") && *local != '|') {
		if (access(local, 2) < 0) {
			char *dir = strrchr(local, '/');

			if (errno != ENOENT && errno != EACCES) {
				perror(local);
				code = -1;
				return;
			}
			if (dir != NULL)
				*dir = 0;
			d = access(dir ? local : ".", 2);
			if (dir != NULL)
				*dir = '/';
			if (d < 0) {
				perror(local);
				code = -1;
				return;
			}
			if (!runique && errno == EACCES &&
			    chmod(local, 0600) < 0) {
				perror(local);
				code = -1;
				return;
			}
			if (runique && errno == EACCES &&
			   (local = gunique(local)) == NULL) {
				code = -1;
				return;
			}
		}
		else if (runique && (local = gunique(local)) == NULL) {
			code = -1;
			return;
		}
	}
	if (initconn()) {
		code = -1;
		return;
	}
	if (!is_retr) {
		if (type != TYPE_A) {
			oldtype = type;
			oldverbose = verbose;
			if (!debug)
				verbose = 0;
			setascii();
			verbose = oldverbose;
		}
	}
	if (remote) {
		if (command("%s %s", cmd, remote) != PRELIM) {
			if (oldtype) {
				if (!debug)
					verbose = 0;
				switch (oldtype) {
					case TYPE_I:
						setbinary();
						break;
					case TYPE_E:
						setebcdic();
						break;
					case TYPE_L:
						settenex();
						break;
				}
				verbose = oldverbose;
			}
			return;
		}
	} else {
		if (command("%s", cmd) != PRELIM) {
			if (oldtype) {
				if (!debug)
					verbose = 0;
				switch (oldtype) {
					case TYPE_I:
						setbinary();
						break;
					case TYPE_E:
						setebcdic();
						break;
					case TYPE_L:
						settenex();
						break;
				}
				verbose = oldverbose;
			}
			return;
		}
	}
	din = dataconn("r");
	if (din == NULL)
		goto abt;
	if (strcmp(local, "-") == 0)
		fout = stdout;
	else if (*local == '|') {
		fout = popen(local + 1, "w");
		if (fout == NULL) {
			perror(local+1);
			goto abt;
		}
		closefunc = pclose;
	} else {
		fout = fopen(local, mode);
		if (fout == NULL) {
			perror(local);
			goto abt;
		}
		closefunc = fclose;
	}
	if (BUFSIZ > bufsize) {
		if (buf)
			(void) free(buf);
		buf = malloc(BUFSIZ);
		if (buf == NULL) {
			perror("malloc");
			fprintf(stderr, "recvrequest: malloc failed...tell ches\n");
			fflush(stderr);
			abort();
			bufsize = 0;
			goto abt;
		}
		bufsize = BUFSIZ;
	}
	time(&start);
	switch (type) {

	case TYPE_I:
	case TYPE_L:
		errno = d = 0;
		while ((c = read(fileno(din), buf, bufsize)) > 0) {
			if ((d = write(fileno(fout), buf, c)) != c)
				break;
			bytes += c;
			if (hash) {
				while (bytes >= hashbytes) {
					(void) putchar('#');
					hashbytes += HASHBYTES;
				}
				(void) fflush(stdout);
			}
		}
		if (hash && bytes > 0) {
			if (bytes < HASHBYTES)
				(void) putchar('#');
			(void) putchar('\n');
			(void) fflush(stdout);
		}
		if (c < 0) {
			if (errno != EPIPE)
				perror("netin1");
			bytes = -1;
		}
		if (d < c) {
			if (d < 0)
				perror(local);
			else
				fprintf(stderr, "%s: short write\n", local);
		}
		break;

	case TYPE_A:
		while ((c = getc(din)) != EOF) {
			while (c == '\r') {
				while (hash && (bytes >= hashbytes)) {
					(void) putchar('#');
					(void) fflush(stdout);
					hashbytes += HASHBYTES;
				}
				bytes++;
				if ((c = getc(din)) != '\n' || tcrflag) {
					if (ferror(fout))
						goto break2;
					(void) putc('\r', fout);
					if (c == '\0') {
						bytes++;
						goto contin2;
					}
					if (c == EOF)
						goto contin2;
				}
			}
			(void) putc(c, fout);
			bytes++;
	contin2:	;
		}
break2:
		if (hash) {
			if (bytes < hashbytes)
				(void) putchar('#');
			(void) putchar('\n');
			(void) fflush(stdout);
		}
		if (ferror(din)) {
			if (errno != EPIPE)
				perror("netin2");
			bytes = -1;
		}
		if (ferror(fout))
			perror(local);
		break;
	}
	if (closefunc != NULL)
		(*closefunc)(fout);
	time(&stop);
	(void) fclose(din);
	(void) getreply(0);
	if (bytes > 0 && is_retr)
		ptransfer("received", bytes, start, stop, local, remote);
	if (oldtype) {
		if (!debug)
			verbose = 0;
		switch (oldtype) {
			case TYPE_I:
				setbinary();
				break;
			case TYPE_E:
				setebcdic();
				break;
			case TYPE_L:
				settenex();
				break;
		}
		verbose = oldverbose;
	}
	return;
abt:

/* abt using RFC959 recommended IP,SYNC sequence  */

	time(&stop);
	if (oldtype) {
		if (!debug)
			verbose = 0;
		switch (oldtype) {
			case TYPE_I:
				setbinary();
				break;
			case TYPE_E:
				setebcdic();
				break;
			case TYPE_L:
				settenex();
				break;
		}
		verbose = oldverbose;
	}
	if (!cpend) {
		code = -1;
		return;
	}

	fprintf(cout,"%c%c",IAC,IP);
	(void) fflush(cout); 
	msg = IAC;
#ifdef STUB
/* send IAC in urgent mode instead of DM because UNIX places oob mark */
/* after urgent byte rather than before as now is protocol            */
	if (sendurgent(fileno(cout),&msg,1) != 1) {
		perror("abt");
	}
	fprintf(cout,"%cABOR\r\n",DM);
#endif
	fprintf(cout,"ABOR\r\n");
	(void) fflush(cout);
	FD_ZERO(mask);
	FD_SET(fileno(cin), mask);
	if (din) { 
		FD_SET(fileno(din), mask);
	}
	if ((nfnd = empty(&mask,10)) <= 0) {
		if (nfnd < 0) {
			perror("abt");
		}
		code = -1;
		lostpeer();
	}
	if (din && FD_ISSET(fileno(din), mask)) {
		while ((c = read(fileno(din), buf, bufsize)) > 0)
			;
	}
	if ((c = getreply(0)) == ERROR && code == 552) { /* needed for nic style abt */
		if (data >= 0) {
			(void) close(data);
			data = -1;
		}
		(void) getreply(0);
	}
	(void) getreply(0);
	code = -1;
	if (data >= 0) {
		(void) close(data);
		data = -1;
	}
	if (closefunc != NULL && fout != NULL)
		(*closefunc)(fout);
	if (din)
		(void) fclose(din);
	if (bytes > 0)
		ptransfer("received", bytes, start, stop, local, remote);
}

/*
 * Need to start a listen on the data channel
 * before we send the command, otherwise the
 * server's connect may fail.
 */

initconn()
{
	int result;
	char *a, *p;
	char *fields[4];
	char myname[128];
	in_addr hostorderaddr;
	tcp_port hostorderport;

	/*
	 *  network = mynetname
	 *  host name = mymachname
	 *  port = *
	 */
	a = ipcpath(mymachname, mynetname, "*");
	data = ipccreat(a, "heavy");
	if(data<0) {
		fprintf(stderr, "ftp: %s creating data connection\n", errstr);
		return(1);
	}
	strcpy(myname, ipcname);
	setfields("!");
	getmfields(myname, fields, 4);
	dataport = atoi(fields[2]);
	hostorderport = htons(dataport);
	hostorderaddr = htonl(myaddr);
	p = (char *)&hostorderport;
	a = (char *)&hostorderaddr;
#define	UC(b)	(((int)b)&0xff)
	result =
	    command("PORT %d,%d,%d,%d,%d,%d",
	      UC(a[0]), UC(a[1]), UC(a[2]), UC(a[3]),
	      UC(p[0]), UC(p[1]));
	return (result != COMPLETE);
}


FILE *
dataconn(mode)
	char *mode;
{
	int s;
	ipcinfo *ip;

	ip = ipclisten(data);
	if (ip == NULL) {
		perror("ftp: listen");
		(void) close(data), data = -1;
		return (NULL);
	}
	s = ipcaccept(ip);
	(void) close(data);
	data = s;
	return (fdopen(data, mode));
}

ptransfer(direction, bytes, t0, t1, local, remote)
	char *direction, *local, *remote;
	long bytes;
	long t0, t1;
{
	long td;
	float s;
	int bs;

	if (verbose) {
		td = t1 - t0;
#define	nz(x)	((x) == 0 ? 1 : (x))
		bs = bytes / nz(td);
		printf("%ld bytes %s in %d seconds (%d bytes/s)\n",
		    bytes, direction, td, bs);
	} else {
		if (local && *local != '-')
			printf("local: %s ", local);
		if (remote)
			printf("remote: %s\n", remote);
	}
}

psabort()
{
	extern int abrtflag;

	abrtflag++;
}

pswitch(flag)
	int flag;
{
	extern int proxy, abrtflag;
	static struct comvars {
		int connect;
		char name[MAXHOSTNAMELEN];
		FILE *in;
		FILE *out;
		int tpe;
		int cpnd;
		int sunqe;
		int runqe;
		int mcse;
		int ntflg;
		char nti[17];
		char nto[17];
		int mapflg;
		char mi[MAXPATHLEN];
		char mo[MAXPATHLEN];
		} proxstruct, tmpstruct;
	struct comvars *ip, *op;

	abrtflag = 0;
	if (flag) {
		if (proxy)
			return;
		ip = &tmpstruct;
		op = &proxstruct;
		proxy++;
	}
	else {
		if (!proxy)
			return;
		ip = &proxstruct;
		op = &tmpstruct;
		proxy = 0;
	}
	ip->connect = connected;
	connected = op->connect;
	if (hostname) {
		(void) strncpy(ip->name, hostname, sizeof(ip->name) - 1);
		ip->name[strlen(ip->name)] = '\0';
	} else
		ip->name[0] = 0;
	hostname = op->name;
#ifdef REMOVE
	ip->hctl = hisctladdr;
	hisctladdr = op->hctl;
	ip->mctl = myctladdr;
	myctladdr = op->mctl;
#endif
	ip->in = cin;
	cin = op->in;
	ip->out = cout;
	cout = op->out;
	ip->tpe = type;
	type = op->tpe;
	if (!type)
		type = 1;
	ip->cpnd = cpend;
	cpend = op->cpnd;
	ip->sunqe = sunique;
	sunique = op->sunqe;
	ip->runqe = runique;
	runique = op->runqe;
	ip->mcse = mcase;
	mcase = op->mcse;
	ip->ntflg = ntflag;
	ntflag = op->ntflg;
	(void) strncpy(ip->nti, ntin, 16);
	(ip->nti)[strlen(ip->nti)] = '\0';
	(void) strcpy(ntin, op->nti);
	(void) strncpy(ip->nto, ntout, 16);
	(ip->nto)[strlen(ip->nto)] = '\0';
	(void) strcpy(ntout, op->nto);
	ip->mapflg = mapflag;
	mapflag = op->mapflg;
	(void) strncpy(ip->mi, mapin, MAXPATHLEN - 1);
	(ip->mi)[strlen(ip->mi)] = '\0';
	(void) strcpy(mapin, op->mi);
	(void) strncpy(ip->mo, mapout, MAXPATHLEN - 1);
	(ip->mo)[strlen(ip->mo)] = '\0';
	(void) strcpy(mapout, op->mo);
}

int ptabflg;

proxtrans(cmd, local, remote)
	char *cmd, *local, *remote;
{
	int tmptype, oldtype = 0, secndflag = 0, nfnd;
	char *cmd2;
	struct fd_set mask;

	if (strcmp(cmd, "RETR"))
		cmd2 = "RETR";
	else
		cmd2 = runique ? "STOU" : "STOR";
	if (command("PASV") != COMPLETE) {
		printf("proxy server does not support third part transfers.\n");
		return;
	}
	tmptype = type;
	pswitch(0);
	if (!connected) {
		printf("No primary connection\n");
		pswitch(1);
		code = -1;
		return;
	}
	if (type != tmptype) {
		oldtype = type;
		switch (tmptype) {
			case TYPE_A:
				setascii();
				break;
			case TYPE_I:
				setbinary();
				break;
			case TYPE_E:
				setebcdic();
				break;
			case TYPE_L:
				settenex();
				break;
		}
	}
	if (command("PORT %s", pasv) != COMPLETE) {
		switch (oldtype) {
			case 0:
				break;
			case TYPE_A:
				setascii();
				break;
			case TYPE_I:
				setbinary();
				break;
			case TYPE_E:
				setebcdic();
				break;
			case TYPE_L:
				settenex();
				break;
		}
		pswitch(1);
		return;
	}
	if (command("%s %s", cmd, remote) != PRELIM) {
		switch (oldtype) {
			case 0:
				break;
			case TYPE_A:
				setascii();
				break;
			case TYPE_I:
				setbinary();
				break;
			case TYPE_E:
				setebcdic();
				break;
			case TYPE_L:
				settenex();
				break;
		}
		pswitch(1);
		return;
	}
	sleep(2);
	pswitch(1);
	secndflag++;
	if (command("%s %s", cmd2, local) != PRELIM)
		goto abort;
	ptflag++;
	(void) getreply(0);
	pswitch(0);
	(void) getreply(0);
	switch (oldtype) {
		case 0:
			break;
		case TYPE_A:
			setascii();
			break;
		case TYPE_I:
			setbinary();
			break;
		case TYPE_E:
			setebcdic();
			break;
		case TYPE_L:
			settenex();
			break;
	}
	pswitch(1);
	ptflag = 0;
	printf("local: %s remote: %s\n", local, remote);
	return;
abort:
	ptflag = 0;
	if (strcmp(cmd, "RETR") && !proxy)
		pswitch(1);
	else if (!strcmp(cmd, "RETR") && proxy)
		pswitch(0);
	if (!cpend && !secndflag) {  /* only here if cmd = "STOR" (proxy=1) */
		if (command("%s %s", cmd2, local) != PRELIM) {
			pswitch(0);
			switch (oldtype) {
				case 0:
					break;
				case TYPE_A:
					setascii();
					break;
				case TYPE_I:
					setbinary();
					break;
				case TYPE_E:
					setebcdic();
					break;
				case TYPE_L:
					settenex();
					break;
			}
			if (cpend) {
				char msg[2];

				fprintf(cout,"%c%c",IAC,IP);
				(void) fflush(cout); 
				*msg = IAC;
				*(msg+1) = DM;
#ifdef STUB
#ifdef OLD
				if (send(fileno(cout),msg,2,MSG_OOB) != 2)
					perror("abort");
#else
				if (sendurgent(fileno(cout),msg,2) != 2)
					perror("abort");
#endif
#endif
				fprintf(cout,"ABOR\r\n");
				(void) fflush(cout);
				FD_ZERO(mask);
				FD_SET(fileno(cin), mask);
				if ((nfnd = empty(&mask,10)) <= 0) {
					if (nfnd < 0) {
						perror("abort");
					}
					if (ptabflg)
						code = -1;
					lostpeer();
				}
				(void) getreply(0);
				(void) getreply(0);
			}
		}
		pswitch(1);
		if (ptabflg)
			code = -1;
		return;
	}
	if (cpend) {
		char msg[2];

		fprintf(cout,"%c%c",IAC,IP);
		(void) fflush(cout); 
#ifdef STUB
		*msg = IAC;
		*(msg+1) = DM;
		if (sendurgent(fileno(cout),msg,2) != 2)
			perror("abort");
#endif
		fprintf(cout,"ABOR\r\n");
		(void) fflush(cout);
		FD_ZERO(mask);
		FD_SET(fileno(cin), mask);
		if ((nfnd = empty(&mask,10)) <= 0) {
			if (nfnd < 0) {
				perror("abort");
			}
			if (ptabflg)
				code = -1;
			lostpeer();
		}
		(void) getreply(0);
		(void) getreply(0);
	}
	pswitch(!proxy);
	if (!cpend && !secndflag) {  /* only if cmd = "RETR" (proxy=1) */
		if (command("%s %s", cmd2, local) != PRELIM) {
			pswitch(0);
			switch (oldtype) {
				case 0:
					break;
				case TYPE_A:
					setascii();
					break;
				case TYPE_I:
					setbinary();
					break;
				case TYPE_E:
					setebcdic();
					break;
				case TYPE_L:
					settenex();
					break;
			}
			if (cpend) {
				char msg[2];

				fprintf(cout,"%c%c",IAC,IP);
				(void) fflush(cout); 
#ifdef STUB
				*msg = IAC;
				*(msg+1) = DM;
				if (sendurgent(fileno(cout),msg,2) != 2)
					perror("abort");
#endif
				fprintf(cout,"ABOR\r\n");
				(void) fflush(cout);
				FD_ZERO(mask);
				FD_SET(fileno(cin), mask);
				if ((nfnd = empty(&mask,10)) <= 0) {
					if (nfnd < 0) {
						perror("abort");
					}
					if (ptabflg)
						code = -1;
					lostpeer();
				}
				(void) getreply(0);
				(void) getreply(0);
			}
			pswitch(1);
			if (ptabflg)
				code = -1;
			return;
		}
	}
	if (cpend) {
		char msg[2];

		fprintf(cout,"%c%c",IAC,IP);
		(void) fflush(cout); 
#ifdef STUB
		*msg = IAC;
		*(msg+1) = DM;
		if (sendurgent(fileno(cout),msg,2) != 2)
			perror("abort");
#endif
		fprintf(cout,"ABOR\r\n");
		(void) fflush(cout);
		FD_ZERO(mask);
		FD_SET(fileno(cin), mask);
		if ((nfnd = empty(&mask,10)) <= 0) {
			if (nfnd < 0) {
				perror("abort");
			}
			if (ptabflg)
				code = -1;
			lostpeer();
		}
		(void) getreply(0);
		(void) getreply(0);
	}
	pswitch(!proxy);
	if (cpend) {
		FD_ZERO(mask);
		FD_SET(fileno(cin), mask);
		if ((nfnd = empty(&mask,10)) <= 0) {
			if (nfnd < 0) {
				perror("abort");
			}
			if (ptabflg)
				code = -1;
			lostpeer();
		}
		(void) getreply(0);
		(void) getreply(0);
	}
	if (proxy)
		pswitch(0);
	switch (oldtype) {
		case 0:
			break;
		case TYPE_A:
			setascii();
			break;
		case TYPE_I:
			setbinary();
			break;
		case TYPE_E:
			setebcdic();
			break;
		case TYPE_L:
			settenex();
			break;
	}
	pswitch(1);
	if (ptabflg)
		code = -1;
}

reset()
{
	struct fd_set mask;
	int nfnd = 1;

	FD_ZERO(mask);
	while (nfnd > 0) {
		FD_SET(fileno(cin), mask);
		if ((nfnd = empty(&mask,0)) < 0) {
			perror("reset");
			code = -1;
			lostpeer();
		}
		else if (nfnd) {
			(void) getreply(0);
		}
	}
}

char *
gunique(local)
	char *local;
{
	static char new[MAXPATHLEN];
	char *cp = strrchr(local, '/');
	int d, count=0;
	char ext = '1';

	if (cp)
		*cp = '\0';
	d = access(cp ? local : ".", 2);
	if (cp)
		*cp = '/';
	if (d < 0) {
		perror(local);
		return((char *) 0);
	}
	(void) strcpy(new, local);
	cp = new + strlen(new);
	*cp++ = '.';
	while (!d) {
		if (++count == 100) {
			printf("runique: can't find unique file name.\n");
			return((char *) 0);
		}
		*cp++ = ext;
		*cp = '\0';
		if (ext == '9')
			ext = '0';
		else
			ext++;
		if ((d = access(new, 0)) < 0)
			break;
		if (ext != '0')
			cp--;
		else if (*(cp - 2) == '.')
			*(cp - 1) = '1';
		else {
			*(cp - 2) = *(cp - 2) + 1;
			cp--;
		}
	}
	return(new);
}
