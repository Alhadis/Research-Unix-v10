#

#include <stdio.h>
#include "udemon.h"

char argspec[OPNUM][3]		/* required arg types (reverse order) */
{			/* negative entry = any non-name type */
	0, 0, 0,		/*OK*/
	-1, NTYPE, 0,		/*DEFINE*/
	-1, NTYPE, 0,		/*REDEFINE*/
	NTYPE, 0, 0,		/*DELETE*/
	0, 0, 0,		/*spare*/
	0, 0, 0,		/*EOM*/
	0, 0, 0,		/*unu	gd*/
	0, 0, 0,		/*XMT*/
	0, 0, 0,		/*spare*/
	0, 0, 0,		/*POP*/
	DTYPE, 0, 0,		/*CDIR*/
	IITYPE, IITYPE, FTYPE,	/*CMODE*/
	XTYPE, FTYPE, 0,	/*OPEN*/
	RTYPE, 0, 0,		/*CLOSE*/
	IITYPE, RTYPE, 0,	/*SEEK*/
	XTYPE, RTYPE, 0,	/*READ*/
	XTYPE, RTYPE, 0,	/*WRITE*/
	XTYPE, RTYPE, 0,	/*TRUNCATE*/
	NTYPE, 0, 0,		/*LOGIN*/
	0, 0, 0,		/*QUIT*/
	DTYPE, 0, 0,		/*LIST*/
	-1, 0, 0,		/*STATUS*/
	0, 0, 0,		/*TIME*/
	NTYPE, XTYPE, 0,		/*UNIQUE*/
	RTYPE, 0, 0,		/*APPEND*/
	0, 0, 0,		/*spare*/
	WTYPE, -1, 0,		/*ASSIGN*/
	NTYPE, XTYPE, 0,	/*FCREATE*/
	NTYPE, 0, 0,		/*IF*/
	0, 0, 0,		/*FI*/
	IITYPE, 0, 0,		/*SKIP*/
	NTYPE, XTYPE, 0,	/*DCREATE*/
	0, 0, 0,		/*spare*/
	NTYPE, 0, 0		/*UNLESS*/
};

char nodef[MAXTYPE] {		/*non-zero if type can be used in definition*/
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0};



deref(xsp)
struct sentry *xsp;
{
	register i, r;
	switch (xsp->s_type){
	case NTYPE:
		if (dstat(ncopy(xsp),&entry) < 0) 
			error(EUNDEF,2);
		xsp->s_type = dcheck(xsp);
		prt("deref ret %o\n",xsp->s_type);
		break;
	case WTYPE:
		i = xsp->s_val1;
		if ((i < 0) || (i >= VARNO)) 
			error(EWADDR,2);
		xsp->s_type = wstore[i].s_type;
		xsp->s_val = wstore[i].s_val;
		xsp->s_val1 = wstore[i].s_val1;
		break;
	case NULL:
		error(EARGT,6);
	}
}


argpush(xsp)
struct sentry **xsp;
{
	register struct sentry *pp, *ss;

	ss = sp++;
	pp = sp;
	while (ss != *xsp){
		pp->s_type = ss->s_type;
		pp->s_val = ss->s_val;
		(pp--)->s_val1 = (ss--)->s_val1;
	}
	*xsp = pp;
}



respond(t)
char t;
{
	if (t) *rp++ = t;
	if (send(resp,rp-resp,3)) 
		return(1);
	rp = resp;
	return(0);
}



addup(b,n)
int n; 
char *b;
{
	register k,s; 
	register char *p;

	s=0;
	p = b;
	if (n>0){
		for (k=0; k<n; k++)
			 s =+ p[k];
	}
	return(s);
}


send(p,n,c)
char *p; 
int n,c;
{
	extern timeout();
	register r;

	if (c==3) {
		prt("send "); 
		tprint(p,n);
	}else
	    prt("send data %d,%d\n",n,c);
	signal(14,timeout); 
	alarm(30);
	snstat(fn,&c,0);
	r = write(fn,p,n);
	alarm(0);
	if (r < 0 || panic) 
		return(1);
	return(0);
}


timeout()
{
	prt("timeout\n");
	panic = 1;
}


recv(b,c)
int *c; 
char *b;
{
	register n;
	register i;
	if ((n = read(fn,b,512)) < 0) 
		return(-1);
	snstat(fn,c,1);
	if (*c==3) {
		prt("received "); 
		tprint(b,n);
	}else
	    prt("received data %d,%d\n",n,*c);
	if (trace){
/*
		for (i=0; i<n; i++){
			printf("%3.1o ",b[i]&0377);
			if ((i&7) == 7 || i == (n-1))
				printf("\n");
		}
*/
	}
	return(n);
}



closeall(){
	register i;

	for (i=0; i<OPENS; i++)
		if (openf[i]) {
			close(i); 
			openf[i] = 0;
		}
	for (i=0; i<VARNO; i++)
		wstore[i].s_type = 0;
}


logout()
{
	register g,t;
	static struct utmp z;
	long	l;

	login = 0;
	utmp.u_name[0] = 0;
#ifdef ACCT
	if ((g = open("/tmp/utmp",1)) >= 0){
		t = utmp.u_tty;
		if(t >= 'a')
			t =- 'a' - (10+'0');
		if((t >= 'A')&&(t<= 'Z'))
			t = 10 + 'z' - 'a' + 1 + '0' + (t - 'A');
		l = (t-'0')*16;
		seek(g, l, 0);
		if(write(g,&utmp,16)<0){
			prt("write in utmp failed\n");
		}
		close(g);
	}
	if((g = open("/usr/adm/wtmp",1)) >= 0){
		z.u_tty = utmp.u_tty;
		time(&z.u_time[0]);
		seek(g,0L,2);
		write(g,&z,16);
		close(g);
	}
#endif
}


/*
 *  data from (fil) , block size (cp).
 * k = 0 if (cp) not set, = 1 normal, = 2 if directory entry
 */

rcopy(fil,k,cp) 
{
	register n, code;
	long	tell();
	register char *bp;

	if (respond(READ)) return(1);
	length = 0;
	sum = 0;
	for (;;){
		n = ((k>0)&&(cp<msiz)) ? cp : msiz;
		if(trace)
			printf("com %d %d %d %d\n",n,k,cp,msiz);
		if(trace)
			printf("posit %d %O\n",fil,tell(fil));
		n = read(fil,buf,n);
		if(trace)
			printf("read %o %o %o\n",fil,buf,n);
		if(trace&&((n == 0) || (n<0))){
			perror("read");
			printf("n = %d %d %d\n",fil,n,k);
		}
		if (n < 0) 
			error(0,6);
		if (k == 2){
			if (n != cp) {
				n=0; 
				code = 4;
			}else
			if (buf[0] == 0) 
				continue;
			else{
				code = 1;
			}
		}else
			if (k>0){
				cp =- n;
				code = (cp==0) ? 2 : ((n==msiz) ? 1 : 4);
			}else
				if (k<0) code = (n==0) ? 4 : 1;
				else
					code = (n==msiz) ? 1 : 4;
		length =+ n;
		sum =+ addup(buf,n);
		prt("part sum %o %o\n",sum , n);
		if (send(buf,n,code)) 
			return(1);
		if (code > 1) 
			break;
	}
	bp = buf;
	*bp++ = 'K';
	*bp++ = (sum >> 8); *bp++ = (sum &0377);
	*bp++ = (length >> 8); *bp++ = (length&0377);
	prt("send checksum %o %o\n",sum, length);
	if(send(buf,5,3) < 0)
		return(1);
	prt("checksum ok\n");
	return(0);
}

ncopy(p)
struct sentry *p;
{
	register i;
	register char *np;

	prt("ncopy %o\n",p->s_type);
	if (p->s_type == NTYPE){
		np = p->s_val1;
		prt("Ntype %s\n",np);
		for (i=0; i<(NSIZ-1); i++){
			if (*np == 0) break;
			nbuf[i] = *np++;
		}
		nbuf[i] = 0;
		return(nbuf);
	}
	prt("ncopy ret %o\n",p->s_val1);
	return(p->s_val1);
}


decimal(v)
char *v;
{
	register n, negative;
	char c;

	n=0; 
	negative=0;
	while (c = *v++){
		if ((n==0) && (c=='-')) {
			negative=1; 
			continue;
		}
		if ((c < '0') || (c > '9')) {
			mesg("Bad buffer size");
			exit(EBAD);
		}
		n = (n*10) + (c-'0');
	}
	if (negative) 
		n = -n;
	return(n);
}


dcheck(p){
	register i;

	prt("dcheck on %s\n",ncopy(p));
	if (filstat(ncopy(p),&statbuf) < 0) 
		return(NULL);
	if (statbuf->type == FFILE) 
		return(FTYPE);
	if (statbuf->type == DIR) 
		return(DTYPE);
	return(NULL);
}
char	pbuf[128];
getpwentry(uname)
char *uname;
{
	char	*np,*namep;
	extern char *copy();
	int	t,f;
	long	l;
	int	i;

	utmp.u_tty = 'A' + chan;
	if(getent(uname,pbuf))
		return(1);
	np = colon(pbuf);
#ifdef ROOT
	if(*np != ':')
		return(1);
#endif
	for(i=0;i<8;i++)
		utmp.u_name[i] = 0;
	copy(utmp.u_name,pbuf,t = (size(pbuf)>8)?8:size(pbuf));
	np = colon(np);
	uid = 0;
	while (*np != ':')
		uid = uid*10 + *np++ - '0';
#ifndef ROOT
	if(uid == ROOTUSR)		/* disallow root */
		return(1);	/* */
#endif
	np++;
	np = colon(np);		/* skip group id */
	np = colon(np);
	namep = np;
	np = colon(np);
	prt("dir = %s\n",namep);
	copy(unixid,namep,  t = (size(namep)>8)?8:size(namep));
	if (chdir(namep)<0) {
		error(0,9);
	}
	time(&utmp.u_time[0]);
#ifdef ACCT
	if ((f = open("/tmp/utmp", 1)) >= 0) {
		t = utmp.u_tty;
		if (t>='a')
			t =- 'a' - (10+'0');
		if((t >= 'A')&&(t<= 'Z'))
			t = 10 + 'z' - 'a' + 1 + '0' + (t - 'A');
		l = (t-'0')*16;
		seek(f, l, 0);
		write(f, &utmp, 16);
		close(f);
	}
	if ((f = open("/usr/adm/wtmp", 1)) >= 0) {
		seek(f, 0L, 2);
		write(f, &utmp, 16);
		close(f);
	}
#endif
	return(0);
}
char	ab[BUFSIZ];
getent(name, rbuf)
char *name, *rbuf;
{
	int fi, r, c;
	FILE *ffin;
	register char *gnp, *rnp;

	r = 1;
	if((ffin = fopen(netpwd, "r")) == NULL){
		prt("Can't open passwd");
		goto ret;

	}
	setbuf(ffin,&ab);
loop:
	gnp = name;
	rnp = rbuf;
	while((c=getc(ffin)) != '\n') {
		if(c == EOF)
			goto ret;

		*rnp++ = c;
	}
	*rnp++ = '\0';
	rnp = rbuf;
	while (*gnp++ == *rnp++);
	if ((*--gnp!='\0')  || *--rnp!=':')
		goto loop;

	r = 0;
ret:
	fclose(ffin);
	return(r);
}

colon(p)
register char *p;
{

	while (*p != ':') {
		if (*p++ == '\0') {
			error(EPWD,2);
		}
	}
	*p++ = '\0';
	return(p);
}
dstat(p,bp)
struct dentry	*bp;
char	*p;
{
	int	f;

	prt("dstat on %s\n",p);
	bp->d_type = NULL;
	if((f = open(p,0)) < 0)
		return(-1);
	close(f);
	bp->d_type = NTYPE;
	return(1);
}
/*
 * Copy c characters of q into p
 */
char *copy(p,q,c)
register char	*p,*q;
int	c;
{
	register i;

	for(i=0;i<c;i++){
		*p++ = *q++;
	}
	return(c);
}
/*
 * Length of string
 */
size(s)
register char *s;
{
	register i;

	i = 0;
	while(*s++ != '\0')
		i++;
	return(i);
}
