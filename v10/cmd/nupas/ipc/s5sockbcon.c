#include	"stdio.h"
#include	"tiuser.h"
#include	"sys/fcntl.h"
#include	"sys/stropts.h"

/*
 *	s5sockbcon():	System V socket-bind-connect sequence
 *
 *	Alan M Buckwalter	3-90
 *
 *	Mimic the BSD socket(), bind() and connect() sequence using
 *	System V TLI calls.
 *
 *	In:	IP Address		(char *)
 *		Port Number		(int)
 *		UNIX Device		(char *)
 *	Out:	Open File Descriptor or -1 on Failure
 *
 *	Warning, there is a bug in the standard UTS Phase I inet_addr()
 *		library call that corrupts the passed string.  Make sure
 *		this fix is installed on your system to avoid any nasty
 *		side affects from this routine.
 */

#define	MAXRETRY	10

s5sockbcon(add,port,dev)
char *add,*dev;
int port;
{
	int			netfd,i;
	char			addrs[20];
	struct	t_call		*callp;
	struct	netbuf		addr;
	static	int		stoa();
	static	char		*atos();
	extern int		t_errno;

	sprintf(addrs,"\\x%08x%04x0000\0",inet_addr(add),port);
	stoa(addrs,&addr);

#ifdef	DEBUG
	fprintf(stderr,"%s - %d\n",atos(&addr),addr.len);
#endif

	/* open transport endpoint */
	if((netfd=t_open(dev,O_RDWR,NULL))<0) {
		t_error("t_open");
		return(-1);
	}

	/* bind default address */
	if(t_bind(netfd,NULL,NULL)<0) {
		t_error("t_bind");
		return(-1);
	}

	/* allocate call structure */
	if((callp=(struct t_call *)t_alloc(netfd,T_CALL,0))==NULL) {
		t_error("t_alloc");
		return(-1);
	}

	callp->addr.buf=addr.buf;
	callp->addr.len=addr.len;

	/* connect to the listener */
	for (i=0;i<MAXRETRY;i++) {
		if(t_connect(netfd,callp,NULL)==0)
			break;
		if((t_errno==TLOOK)&&(t_look(netfd)==T_DISCONNECT)) {
			t_rcvdis(netfd,NULL);
		} else {
			t_error("t_connect");
			return(-1);
		}
	}
	if(i>2) {
		t_error("t_connect");
		return(-1);
	}

	/* push the read/write streams modules */
	if(ioctl(netfd,I_PUSH,"tirdwr")<0) {
		t_error("ioctl");
		return(-1);
	}
	return(netfd);
}

static int
stoa(s, np)
register char	*s;
struct netbuf	*np;
{
	static char	buf[128];
	register char	*bp = buf;
	int	b;

	if (s[0] == '\\' && tolower(s[1]) == 'x') {
		while (*(s += 2)) {
			sscanf(s, "%2x", &b);
			*bp++ = b;
		}
		np->len = bp - buf;
	} else
		np->len = strlen(strcpy(buf, s));
	np->buf = buf;
	return 1;
}

static char *
atos(np)
register struct netbuf	*np;
{
	static char	buf[128];
	register char	*bp = buf;
	register int	i;

	bp += sprintf(bp, "\\x");
	for (i = 0; i < np->len; i++)
		bp += sprintf(bp, "%.2x", np->buf[i]);
	return buf;
}
