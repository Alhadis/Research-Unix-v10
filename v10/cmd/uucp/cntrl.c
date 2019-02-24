/*	/sccs/src/cmd/uucp/s.cntrl.c
	cntrl.c	1.9	8/30/84 17:37:09
*/
#include "uucp.h"
VERSION(@(#)cntrl.c	1.9);

struct Proto {
	char P_id;
	int (*P_turnon)();
	int (*P_rdmsg)();
	int (*P_wrmsg)();
	int (*P_rddata)();
	int (*P_wrdata)();
	int (*P_turnoff)();
};

#ifdef	PROTO_G
extern int gturnon(), gturnoff();
extern int errno;
extern int grdmsg(), grddata();
extern int gwrmsg(), gwrdata();
#endif
#ifdef	PROTO_D
extern int dturnon(), dturnoff();
extern int drdmsg(), drddata();
extern int dwrmsg(), dwrdata();
#endif
#ifdef	PROTO_X
extern int xturnon(), xturnoff();
extern int xrdmsg(), xrddata();
extern int xwrmsg(), xwrdata();
#endif
#ifdef	PROTO_E
extern int eturnon(), eturnoff();
extern int erdmsg(), erddata();
extern int ewrmsg(), ewrdata();
#endif

extern int imsg();
extern int omsg();

struct Proto Ptbl[]={
#ifdef	PROTO_G
	'g', gturnon, grdmsg, gwrmsg, grddata, gwrdata, gturnoff,
#endif
#ifdef	PROTO_E
	'e', eturnon, erdmsg, ewrmsg, erddata, ewrdata, eturnoff,
#endif
#ifdef	PROTO_D
	'd', dturnon, drdmsg, dwrmsg, drddata, dwrdata, dturnoff,
#endif
#ifdef	PROTO_X
	'x', xturnon, xrdmsg, xwrmsg, xrddata, xwrdata, xturnoff,
#endif
	'\0'
};

int (*Rdmsg)()=imsg, (*Rddata)();
int (*Wrmsg)()=omsg, (*Wrdata)();
int (*Turnon)(), (*Turnoff)();


#define YES "Y"
#define NO "N"

/*
 * failure messages
 */
#define EM_MAX		7
#define EM_LOCACC	"N1"	/* local access to file denied */
#define EM_RMTACC	"N2"	/* remote access to file/path denied */
#define EM_BADUUCP	"N3"	/* a bad uucp command was generated */
#define EM_NOTMP	"N4"	/* remote error - can't create temp */
#define EM_RMTCP	"N5"	/* can't copy to remote directory - file in public */
#define EM_LOCCP	"N6"	/* can't copy on local system */

char *Em_msg[] = {
	"COPY FAILED (reason not given by remote)",
	"local access to file denied",
	"remote access to path/file denied",
	"system error - bad uucp command generated",
	"remote system can't create temp file",
	"can't copy to file/directory - file left in PUBDIR/user/file",
	"can't copy to file/directory - file left in PUBDIR/user/file",
	"forwarding error"
};


#define XUUCP 'X'	/* execute uucp (string) */
#define SLTPTCL 'P'	/* select protocol  (string)  */
#define USEPTCL 'U'	/* use protocol (character) */
#define RCVFILE 'R'	/* receive file (string) */
#define SNDFILE 'S'	/* send file (string) */
#define RQSTCMPT 'C'	/* request complete (string - yes | no) */
#define HUP     'H'	/* ready to hangup (string - yes | no) */
#define RESET	'X'	/* reset line modes */

#define W_MAX		10	/* maximum number of C. files per line */
#define W_TYPE		wrkvec[0]
#define W_FILE1		wrkvec[1]
#define W_FILE2		wrkvec[2]
#define W_USER		wrkvec[3]
#define W_OPTNS		wrkvec[4]
#define W_DFILE		wrkvec[5]
#define W_MODE		wrkvec[6]
#define W_NUSER		wrkvec[7]
#define W_SFILE		wrkvec[8]
#define W_RFILE		wrkvec[5]
#define W_XFILE		wrkvec[5]
char	*mf;

#define RMESG(m, s) if (rmesg(m, s) != 0) {(*Turnoff)(); return(FAIL);}
#define RAMESG(s) if (rmesg('\0', s) != 0) {(*Turnoff)(); return(FAIL);}
#define WMESG(m, s) if(wmesg(m, s) != 0) {(*Turnoff)(); return(FAIL);}

char Wfile[MAXFULLNAME] = {'\0'};
char Dfile[MAXFULLNAME];

/*
 * execute the conversation between the two machines 
 * after both programs are running.
 * returns:
 *	SUCCESS 	-> ok
 *	FAIL 		-> failed
 */
char	*wrkvec[W_MAX+1];
int	statfopt;
cntrl(role)
register int	role;
{
	FILE * fp;
	struct stat stbuf;
	extern (*Rdmsg)(), (*Wrmsg)();
	int	filemode;
	int	status = 1;
	int	i, narg;
	int	mailopt, ntfyopt;
	int	ret;
	char	rqstr[BUFSIZ];	/* contains the current request message */
	char	msg[BUFSIZ];
	char	filename[MAXFULLNAME], wrktype;
	static int	pnum;

	pnum = getpid();
	Wfile[0] = '\0';
top:
	statfopt = 0;
	*Jobid = '\0';
	DEBUG(4, "*** TOP ***  -  role=%d, ", role);
	setline(RESET);
	if (role == MASTER) {

		/*
		 * get work
		 */
		if ((narg = gtwvec(Wfile, wrkvec, W_MAX)) == 0) {
			WMESG(HUP, "");
			RMESG(HUP, msg);
			goto process;
		}
		DEBUG(7, "Wfile - %s,", Wfile);
		strncpy(Jobid, BASENAME(Wfile, '/')+2, NAMESIZE);
		Jobid[NAMESIZE-1] = '\0';
		DEBUG(7, "Jobid = %s\n", Jobid);
		wrktype = W_TYPE[0];
		mailopt = strchr(W_OPTNS, 'm') != NULL;
#if NOTDEF	/* unsafe */
		statfopt = strchr(W_OPTNS, 'o') != NULL;
#endif
		ntfyopt = strchr(W_OPTNS, 'n') != NULL;

		strcpy(User, W_USER);
		W_USER = Uucp;	/* so what we send is short */
		msg[0] = '\0';
		for (i = 1; i < narg; i++) {
			(void) strcat(msg, " ");
			(void) strcat(msg, wrkvec[i]);
		}

		/*
		 * We used to check for corrupt workfiles here (narg < 5),
		 * but we were doing it wrong, and besides, anlwrk.c is the
		 * appropriate place to do it.
		 */

		if (wrktype == SNDFILE ) {
			(void) sprintf(rqstr, "%s!%s --> %s!%s (%s)", Myname,
			    W_FILE1, Rmtname, W_FILE2, User);
			logent(rqstr, "REQUEST");
			CDEBUG(1, "Request: %s\n", rqstr);
			mf = W_SFILE;
			(void) strcpy(filename, W_FILE1);
			expfile(filename);
			if ( !READSOME(filename) && !READSOME(W_DFILE)) {

				/*
				 * access denied
				 */
				logent("DENIED", "ACCESS");
				unlinkdf(W_DFILE);
				lnotify(User, rqstr, "access denied");
				CDEBUG(1, "Failed: Access Denied\n", 0);
				goto top;
			}

			(void) strcpy(Dfile, W_DFILE);
			fp = NULL;
			fp = fopen(Dfile, "r");
			ASSERT(strlen(Dfile)>0, "Dfile==0", "", role);
			if (fp == NULL && 
			    (fp = fopen(filename, "r")) == NULL) {

				/*  can not read data file */
				unlinkdf(Dfile);
				lnotify(User, rqstr, "can't access");

				(void) sprintf(msg, "CAN'T READ %s %d",
					filename, errno);
				logent(msg, "FAILED");
				CDEBUG(1, "Failed: Can't Read %s\n", filename);
				goto top;
			}
			Seqn++;
			setline(SNDFILE);
		}

		if (wrktype == RCVFILE) {
			(void) sprintf(rqstr, "%s!%s --> %s!%s (%s)", Rmtname,
			    W_FILE1, Myname, W_FILE2, User);
			logent(rqstr, "REQUEST");
			CDEBUG(1, "Request: %s\n", rqstr);
			mf = W_RFILE;
			(void) strcpy(filename, W_FILE2);
			expfile(filename);
			if (chkperm(W_FILE1, filename, strchr(W_OPTNS, 'd'))) {

				/* access denied */
				logent("DENIED", "ACCESS");
				lnotify(User, rqstr, "access denied");
				CDEBUG(1, "Failed: Access Denied--File: $s\n", 
				    filename);
				goto top;
			}
			TMname(Dfile, pnum); /* get TM file name */

			if ( ((fp = fopen(Dfile, "w")) == NULL)
			     || nospace(Dfile)) {

				/* can not create temp */
				logent("CAN'T CREATE TM", "FAILED");
				CDEBUG(1, "Failed: No Space!\n", 0);
				unlinkdf(Dfile);
				assert(Ct_CREATE, Dfile, nospace(Dfile),
				    sccsid, __FILE__, __LINE__);
				cleanup(FAIL);
			}
			Seqn++;
			chmod(Dfile, DFILEMODE);	/* no peeking! */
			setline(RCVFILE);
		}
sendmsg:
		DEBUG(4, "wrktype - %c\n ", wrktype);
		WMESG(wrktype, msg);
		RMESG(wrktype, msg);
		goto process;
	}

	/*
	 * role is slave
	 */
	RAMESG(msg);

process:

	/*
	 * touch all lock files
	 */
	ultouch();
	DEBUG(4, " PROCESS: msg - %s\n", msg);
	switch (msg[0]) {

	case RQSTCMPT:
		DEBUG(4, "%s\n", "RQSTCMPT:");
		if (msg[1] == 'N') {
			i = atoi(&msg[2]);
			if (i < 0 || i > EM_MAX) 
				i = 0;
			logent(Em_msg[i], "REQUESTED");
		}
		if (role == MASTER)
		        notify(mailopt, User, rqstr, Rmtname, &msg[1]);
		goto top;

	case HUP:
		DEBUG(4, "%s\n", "HUP:");
		if (msg[1] == 'Y') {
			WMESG(HUP, YES);
			(*Turnoff)();
			Rdmsg = imsg;
			Wrmsg = omsg;
			return(0);
		}

		if (msg[1] == 'N') {
			ASSERT(role == MASTER, Wr_ROLE, "", role);
			role = SLAVE;
			goto top;
		}

		/*
		 * get work
		 */
		if ( (switchRole() == FALSE) || !iswrk(Wfile) ) {
			DEBUG(5, "SLAVE-switchRole (%s)\n",
			    switchRole() ? "TRUE" : "FALSE");
			WMESG(HUP, YES);
			RMESG(HUP, msg);
			goto process;
		}

		/* Note that Wfile is the first C. to process at top
		 * set above by iswrk() call
		 */
		WMESG(HUP, NO);
		role = MASTER;
		goto top;

	case XUUCP:
		/*
		 * slave part
		 * No longer accepted
		 */

		WMESG(XUUCP, NO);
		goto top;

	case SNDFILE:

		/*
		 * MASTER section of SNDFILE
		 */
		DEBUG(4, "%s\n", "SNDFILE:");
		if (msg[1] == 'N')
		   {
		    i = atoi(&msg[2]);
		    if (i < 0 || i > EM_MAX)
			i = 0;
		    logent(Em_msg[i], "REQUEST");
		    notify(mailopt, User, rqstr, Rmtname, &msg[1]);
		    ASSERT(role == MASTER, Wr_ROLE, "", role);
		    (void) fclose(fp);
		    ASSERT(i != 4, Em_msg[4], Rmtname, i);	/* EM_NOTMP */
		    unlinkdf(W_DFILE);
		    goto top;
		   }

		if (msg[1] == 'Y') {

			/*
			 * send file
			 */
			ASSERT(role == MASTER, Wr_ROLE, "", role);
			if (fstat(fileno(fp), &stbuf)) /* never fail but .. */
			    stbuf.st_size = 0;  /* for time loop calculation */
			ret = (*Wrdata)(fp, Ofn);
			(void) fclose(fp);
			if (ret != 0) {
				(*Turnoff)();
				return(FAIL);
			}

			/* loop depending on the size of the file */
			/* give an extra try for each megabyte */
			for (i = stbuf.st_size >> 10; i >= 0; --i) {
		    	    if ((ret = rmesg(RQSTCMPT, msg)) == 0)
				break;	/* got message */
			}
			if (ret != 0) {
			    (*Turnoff)();
			     return(FAIL);
			}
			unlinkdf(W_DFILE);
			goto process;
		}

		/* 
		 * SLAVE section of SNDFILE
		 */
		ASSERT(role == SLAVE, Wr_ROLE, "", role);

		/*
		 * request to receive file
		 * check permissions
		 */
		i = getargs(msg, wrkvec, W_MAX);

		/* Check for bad request */
		if (i < 7) {
			WMESG(SNDFILE, EM_BADUUCP);
			logent("DENIED", "TOO FEW ARGS IN SLAVE SNDFILE");
			goto top;
		}

		mf = W_SFILE;
		(void) sprintf(rqstr, "%s!%s --> %s!%s (%s)", Rmtname,
			    W_FILE1, Myname, W_FILE2, W_USER);
		logent(rqstr, "REMOTE REQUESTED");
		DEBUG(4, "msg - %s\n", msg);
		CDEBUG(1, "Remote Requested: %s\n", rqstr);
		(void) strcpy(filename, W_FILE2);
		expfile(filename);
		DEBUG(4, "SLAVE - filename: %s\n", filename);
		if (chkpth(filename, CK_WRITE)
		     || chkperm(W_FILE1, filename, strchr(W_OPTNS, 'd'))) {
			WMESG(SNDFILE, EM_RMTACC);
			logent("DENIED", "PERMISSION");
			CDEBUG(1, "Failed: Access Denied\n", 0);
			goto top;
		}
		strcpy(User, W_USER);

		DEBUG(4, "chkpth ok Rmtname - %s\n", Rmtname);
		TMname(Dfile, pnum); /* get TM file name */

		if ( ((fp = fopen(Dfile, "w")) == NULL) || nospace(Dfile) ) {
			WMESG(SNDFILE, EM_NOTMP);
			logent("CAN'T OPEN", "DENIED");
			CDEBUG(1, "Failed: Can't Create Temp File\n", 0);
			unlinkdf(Dfile);
			goto top;
		}
		Seqn++;
		chmod(Dfile, DFILEMODE);	/* no peeking! */
		WMESG(SNDFILE, YES);
		ret = (*Rddata)(Ifn, fp);
		(void) fclose(fp);
		if (ret != 0) {
			(*Turnoff)();
			logent("INPUT FAILURE", "IN SEND/SLAVE MODE");
			return(FAIL);
		}
		/* copy to user directory */
		ntfyopt = strchr(W_OPTNS, 'n') != NULL;
		status = xmv(Dfile, filename);
		WMESG(RQSTCMPT, status ? EM_RMTCP : YES);
		if (status == 0) {
			sscanf(W_MODE, "%o", &filemode);
			if (filemode <= 0)
				filemode = 0666;
			if (PREFIX(RemSpool, filename))
				chmod(filename, 0600);
			else
				chmod(filename, (filemode|0666) & 0777);
			arrived(ntfyopt, filename, W_NUSER, Rmtname, User);
		} else {
			logent("FAILED",  "COPY");
			status = putinpub(filename, Dfile,
			    BASENAME(W_USER, '!'));
			DEBUG(4, "->PUBDIR %d\n", status);
			if (status == 0)
				arrived(ntfyopt, filename, W_NUSER,
				    Rmtname, User);
		}
		goto top;

	case RCVFILE:

		/*
		 * MASTER section of RCVFULE 
		 */
		DEBUG(4, "%s\n", "RCVFILE:");
		if (msg[1] == 'N') {
			i = atoi(&msg[2]);
			if (i < 0 || i > EM_MAX)
				i = 0;
			logent(Em_msg[i], "REQUEST");
		        notify(mailopt, User, rqstr, Rmtname, &msg[1]);
			ASSERT(role == MASTER, Wr_ROLE, "", role);
			(void) fclose(fp);
			unlinkdf(Dfile);
			goto top;
		}

		if (msg[1] == 'Y') {

			/*
			 * receive file
			 */
			ASSERT(role == MASTER, Wr_ROLE, "", role);
			ret = (*Rddata)(Ifn, fp);
			(void) fclose(fp);
			if (ret != 0) {
				(*Turnoff)();
				return(FAIL);
			}

			status = xmv(Dfile, filename);
			WMESG(RQSTCMPT, status ? EM_RMTCP : YES);
			notify(mailopt, User, rqstr, Rmtname,
			    status ? EM_LOCCP : YES);
			if (status == 0) {
				sscanf(&msg[2], "%o", &filemode);
				if (filemode <= 0)
					filemode = 0666;
				if (PREFIX(RemSpool, filename))
					chmod(filename, 0600);
				else
					chmod(filename, (filemode|0666) & 0777);
			} else {
				logent("FAILED", "COPY");
				putinpub(filename, Dfile, W_USER);
			}
			goto top;
		}

		/*
		 * SLAVE section of RCVFILE
		 * (request to send file)
		 */
		ASSERT(role == SLAVE, Wr_ROLE, "", role);

		/* check permissions */
		i = getargs(msg, wrkvec, W_MAX);

		/* Check for bad request */
		if (i < 5) {
			WMESG(RCVFILE, EM_BADUUCP);
			logent("DENIED", "TOO FEW ARGS IN SLAVE RCVFILE");
			goto top;
		}

		(void) sprintf(rqstr, "%s!%s --> %s!%s (%s)", Myname,
			    W_FILE1, Rmtname, W_FILE2, W_USER);
		logent(rqstr, "REMOTE REQUESTED");
		CDEBUG(1, "Remote Requested: %s\n", rqstr);
		mf = W_RFILE;
		DEBUG(4, "msg - %s\n", msg);
		DEBUG(4, "W_FILE1 - %s\n", W_FILE1);
		(void) strcpy(filename, W_FILE1);
		expfile(filename);
		if (DIRECTORY(filename)) {
			(void) strcat(filename, "/");
			(void) strcat(filename, BASENAME(W_FILE2, '/'));
		}
		strcpy(User, W_USER);
		if (requestOK() == FALSE) {
			WMESG(RCVFILE, EM_RMTACC);
			logent("DENIED", "REQUESTING");
			CDEBUG(1, "Failed: Access Denied\n", 0);
			goto top;
		}
		DEBUG(4, "requestOK for Loginuser - %s\n", Loginuser);

		if (chkpth(filename, CK_READ) || !READANY(filename)) {
			WMESG(RCVFILE, EM_RMTACC);
			logent("DENIED", "PERMISSION");
			CDEBUG(1, "Failed: Access Denied\n", 0);
			goto top;
		}
		DEBUG(4, "chkpth ok Loginuser - %s\n", Loginuser);

		if ((fp = fopen(filename, "r")) == NULL) {
			WMESG(RCVFILE, EM_RMTACC);
			logent("CAN'T OPEN", "DENIED");
			CDEBUG(1, "Failed: Can't Open %s\n", filename);
			goto top;
		}

		/*
		 * ok to send file
		 */
		
		ASSERT(fstat(fileno(fp), &stbuf) == 0, Ct_STAT,
		    filename, errno);
		(void) sprintf(msg, "%s %o", YES, stbuf.st_mode & 0777);
		WMESG(RCVFILE, msg);
		Seqn++;
		ret = (*Wrdata)(fp, Ofn);
		(void) fclose(fp);
		if (ret != 0) {
			(*Turnoff)();
			return(FAIL);
		}

		/* loop depending on the size of the file */
		/* give an extra try for each megabyte */
		/* stbuf set in fstat several lines back  */
		for (i = stbuf.st_size >> 10; i >= 0; --i) {
		    if ((ret = rmesg(RQSTCMPT, msg)) == 0)
			break;	/* got message */
		}
		if (ret != 0) {
		    (*Turnoff)();
		     return(FAIL);
		}
		goto process;
	}
	(*Turnoff)();
	return(FAIL);
}



/*
 * read message
 * returns:
 *	0	-> success
 *	FAIL	-> failure
 */
rmesg(c, msg)
char *msg, c;
{
	char str[50];

	DEBUG(4, "rmesg - '%c' ", c);
	if ((*Rdmsg)(msg, Ifn) != 0) {
		DEBUG(4, "got %s\n", "FAIL");
		(void) sprintf(str, "expected '%c' got FAIL", c);
		logent(str, "BAD READ");
		return(FAIL);
	}
	if (c != '\0' && msg[0] != c) {
		DEBUG(4, "got %s\n", msg);
		(void) sprintf(str, "expected '%c' got %s", c, msg);
		logent(str, "BAD READ");
		return(FAIL);
	}
	DEBUG(4, "got %s\n", msg);
	return(0);
}


/*
 * write a message
 * returns:
 *	0	-> ok
 *	FAIL	-> ng
 */
wmesg(m, s)
char *s, m;
{
	CDEBUG(4, "wmesg '%c'", m);
	CDEBUG(4, "%s\n", s);
	return((*Wrmsg)(m, s, Ofn));
}


/*
 * mail results of command
 * return: 
 *	none
 */
notify(mailopt, user, msgin, sys, msgcode)
char *user, *msgin, *sys;
register char *msgcode;
{
	register int i;
	char str[BUFSIZ];
	register char *msg;

	DEBUG(4,"mailopt %d, ", mailopt);
	DEBUG(4,"statfopt %d\n", statfopt);
	if (statfopt == 0 && mailopt == 0 && *msgcode == 'Y')
		return;
	if (*msgcode == 'Y')
		msg = "copy succeeded";
	else {
		i = atoi(msgcode + 1);
		if (i < 1 || i > EM_MAX)
			i = 0;
		msg = Em_msg[i];
	}
	if(statfopt){
		stmesg(msgin, msg);
		return;
	}
	(void) sprintf(str, "REQUEST: %s\n(SYSTEM: %s)  %s\n",
		msgin, sys, msg);
	mailst(user, str, "", "");
	return;
}

/*
 * local notify
 * return:
 *	none
 */
lnotify(user, msgin, mesg)
char *user, *msgin, *mesg;
{
	char mbuf[BUFSIZ];

	if(statfopt){
		stmesg(msgin, mesg);
		return;
	}
	(void) sprintf(mbuf, "REQUEST: %s\n(SYSTEM: %s)  %s\n",
		msgin, Myname, mesg);
	mailst(user, mbuf, "", "");
	return;
}
static
stmesg(f, m)
char	*f, *m;
{
	FILE	*Cf;
	time_t	clock;
	long	td, th, tm, ts;

	ASSERT(1, "stmesg called", "", 0);
#if NOTDEF
	DEBUG(4,"STMES %s\n",mf);
	if((Cf = fopen(mf, "a+")) == NULL){
		chmod(mf, 0666);
		return;
	}
	(void) time(&clock);
	(void) fprintf(Cf, "uucp job: %s (%s) ", Jobid, timeStamp());
	td = clock - Nstat.t_qtime;
	ts = td%60;
	td /= 60;
	tm = td%60;
	td /= 60;
	th = td;
	(void) fprintf(Cf, "(%ld:%ld:%ld)\n%s\n%s\n\n", th, tm, ts, f, m);
	(void) fclose(Cf);
	chmod(mf, 0666);
#endif

}


/*
 * converse with the remote machine, agree upon a 
 * protocol (if possible) and start the protocol.
 * return:
 *	SUCCESS	-> successful protocol selection
 *	FAIL	-> can't find common or open failed
 */
startup(role)
register int role;
{
	extern (*Rdmsg)(), (*Wrmsg)();
	extern imsg(), omsg();
	extern char *blptcl(), fptcl(), *protoString();
	char msg[BUFSIZ], str[BUFSIZ];

	Rdmsg = imsg;
	Wrmsg = omsg;
	if (role == MASTER) {
		RMESG(SLTPTCL, msg);
		if ((str[0] = fptcl(&msg[1])) == NULL) {

			/*
			 * no protocol match
			 */
			WMESG(USEPTCL, NO);
			return(FAIL);
		}
		str[1] = '\0';
		WMESG(USEPTCL, str);
		if (stptcl(str) != 0)
			return(FAIL);
		return(SUCCESS);
	} else {
		WMESG(SLTPTCL, blptcl(str));
		RMESG(USEPTCL, msg);
		if (msg[1] == 'N') {
			return(FAIL);
		}

		if (stptcl(&msg[1]) != 0)
			return(FAIL);
		return(SUCCESS);
	}
}


/*
 * choose a protocol from the input string (str)
 * and return the found letter.
 * Use the MASTER string for order of selection.
 * If a preferred protocol was named, use that if possible.
 * return:
 *	'\0'		-> no acceptable protocol
 *	any character	-> the chosen protocol
 */
char
fptcl(str)
register char *str;
{
	char *l, list[20];	/* assume less than 20 protocols */

	if ((l = protoString()) != NULL)
		for (; *l; l++)
			if (strchr(str, *l))
				return (*l);
	blptcl(list);
	for (l = list; *l != '\0'; l++)
		if (strchr(str, *l) != NULL)
			return(*l);
	return('\0');
}

/*
 * build a string of the letters of the available
 * protocols and return the string (str).
 * return:
 *	a pointer to string (str)
 */
char *
blptcl(str)
register char *str;
{
	register struct Proto *p;
	char *s;

	for (p = Ptbl, s = str; (*s++ = p->P_id) != '\0'; p++)
		;
	return(str);
}

/*
 * set up the six routines (Rdmg. Wrmsg, Rddata
 * Wrdata, Turnon, Turnoff) for the desired protocol.
 * returns:
 *	SUCCESS 	-> ok
 *	FAIL		-> no find or failed to open
 */
stptcl(c)
register char *c;
{
	register struct Proto *p;

	for (p = Ptbl; p->P_id != '\0'; p++) {
		if (*c == p->P_id) {

			/*
			 * found protocol 
			 * set routine
			 */
			Rdmsg = p->P_rdmsg;
			Wrmsg = p->P_wrmsg;
			Rddata = p->P_rddata;
			Wrdata = p->P_wrdata;
			Turnon = p->P_turnon;
			Turnoff = p->P_turnoff;
			if ((*Turnon)() != 0)
				break;
			CDEBUG(4, "Proto started %c\n", *c);
			return(SUCCESS);
		}
	}
	CDEBUG(4, "Proto start-fail %c\n", *c);
	return(FAIL);
}

/*
 * put file in public place
 * if successful, filename is modified
 * returns:
 *	0	-> success
 *	FAIL	-> failure
 */
putinpub(file, tmp, user)
char *file, *user, *tmp;
{
	int status;
	char fullname[MAXFULLNAME];

	(void) sprintf(fullname, "%s/%s/", Pubdir, user);
	if (mkdirs(fullname) != 0) {

		/*
		 * can not make directories
		 */
		return(FAIL);
	}
	(void) strcat(fullname, BASENAME(file, '/'));
	status = xmv(tmp, fullname);
	if (status == 0) {
		(void) strcpy(file, fullname);
		(void) chmod(fullname,0666);
	}
	return(status);
}

/*
 * unlink D. file
 * returns:
 *	none
 */
unlinkdf(file)
register char *file;
{
	if (strlen(file) > 6)
		(void) unlink(file);
	return;
}

/*
 * notify receiver of arrived file
 * returns:
 *	none
 */
arrived(opt, file, nuser, rmtsys, rmtuser)
char *file, *nuser, *rmtsys, *rmtuser;
{
	char mbuf[200];

	if (!opt)
		return;
	(void) sprintf(mbuf, "%s from %s!%s arrived\n", file, rmtsys, rmtuser);
	mailst(nuser, mbuf, "", "");
	return;
}


/*
 * Check to see if there is space for file
 */

#define FREESPACE 50  /* Minimum freespace in blocks to permit transfer */
#define FREENODES 5   /* Minimum number of inodes to permit transfer */

/*ARGSUSED*/
static
nospace(name)
char *name;
#ifdef NOUSTAT
{return(FALSE);}
#else
{
	struct stat statb;
	struct ustat ustatb;

	if( stat(name, &statb) < 0 )
		return(TRUE);
#ifdef	RT
	if( (statb.st_mode|S_IFMT) == S_IFREG ||
	    (statb.st_mode|S_IFMT) == S_IFEXT ||
	    (statb.st_mode&S_IFMT) == S_IF1EXT )
#else
	if( (statb.st_mode&S_IFMT) == S_IFREG )
#endif
	{
		if( ustat(statb.st_dev, &ustatb)<0 )
			return(TRUE);
		if( ustatb.f_tfree < FREESPACE ) {
			logent("FREESPACE IS LOW","REMOTE TRANSFER DENIED - ");
			return(TRUE);
		}
		if( ustatb.f_tinode < FREENODES ) {
			logent("TOO FEW INODES","REMOTE TRANSFER DENIED - ");
			return(TRUE);
		}
	}
	return(FALSE);
}
#endif

#ifdef V7USTAT
ustat(dev, ustat)
int	dev;
struct ustat *ustat;
{
	FILE	*dfp, *popen();
	struct fstab	*fstab = NULL;
	char	*fval, buf[BUFSIZ];

	sprintf(buf, "%s %d %d 2>&1", V7USTAT, major(dev), minor(dev));
	if ((dfp = popen(buf, "r")) == NULL)
		return(-1);
	fval = fgets(buf, sizeof(buf), dfp);
	if (pclose(dfp) != 0
	 || fval == NULL
	 || sscanf(buf, "%d %d", &ustat->f_tfree, &ustat->f_tinode) != 2)
		return(-1);
	return(0);
}
#endif V7USTAT
