#
#define ERROR	0
#define GOOD 1

/*
 * fdemon tiu txt msiz [trace]
 * file store demon to handle commands from (tiu).
 * the txt arg is not used.
 * The maximum message size sent to the user is (msiz).
 * If (trace) is present the demon will trace on the specified file.
 *	the current output is used if '-' is specified.
 */
#include <stdio.h>
#include "udemon.h"


char unixid[128];
char	*uidp	&unixid;
char nbuf[NSIZ] {
};
char *mkdir "/bin/mkdir";
char *rmdir "/bin/rmdir";
char *minishell "/bin/sh";
char	*netpwd	"/usr/lib/spidernet";
int zero[2] {
	0,0};




int	*tp;
int	parent;
int	pid;
char	*uniqs	"prXXXXXX";
int	kk[50];
#define CHILD	0
#define INPUT	0
#define OUTPUT	1
#define RPIPE	0
#define WPIPE	1
main(argc,argv) 
int	argc;
char *argv[];
{
	int code;		/*status to/from tiu*/
	int fil;		/*file code for active file*/
	int	mode;
	int rpfil[2], wpfil[2];	/*pipes for shell execution*/
	int op;			/*op code*/
	char *cp, *ap;
	int  k, n, s, t;
	register i;
	int	rlength,rsum;
	int	nchar;
	char *rbp;
	char	*p;


	signal(1,1); 
	signal(2,1); 
	signal(3,1);
	if (argc == 4) 
		trace = 0;
	else
	 if (argc == 5){
		trace = 1;
		if (*argv[4] != '-'){
			close(OUTPUT); 
			close(INPUT);
			close(2);
			if (creat(argv[4],0666) < 0) {
				mesg("debug creat failed");
				exit(EGOOD);
			}
			dup(INPUT);
			dup(INPUT);
		}
	}else {
		mesg("Incorrect number of arguments");
		exit(EGOOD);
	}
	msiz = decimal(argv[3]);
	prt("msiz = %d\n",msiz);
	parent = 1;
rstart:
	prt("got to res");
	panic = 0;
	if ((fn = open(tiu = argv[1],2)) < 0) {
		mesg("Can't open tiu channel");
		exit(EGOOD);
	}
	snstat(fn,&chan,2); 
	utmp.u_tty = chan + 'A';
	for (i=0; i<OPENS; i++) 
		openf[i] = 0;
	rp = resp;
	logout();
	prt("logout done");
trouble:
	snstat(fn,&code,3);
	prt("check trob code");
	if (code) 
		prt("trouble %d\n",code);
	closeall();
	setexit();
nextmes:
	prt("nextmes");
	if (panic){
		prt("nextmes panic\n");
		closeall();
		close(fn);
		if(!parent)
			exit(EBAD);
		goto rstart;

	}
	prt("start recv");
	if ((n=recv(mbuf,&code)) < 0) 
		goto trouble;
	prt("mesg received");
	if (code != 3) 
		error(ESIG,code);
	if ((login == 0) && (filstat("/tmp/ulock",&statbuf) >= 0)) 
		error(EUNAV,1);
	mlast = &mbuf[n];
	lp = mlink;
	mp = mbuf;
	sp = stack;
	rp = resp;
	ig = 0;
	(sp++)->s_type = NULL;
	prt("sp = %o %o\n",stack,sp);
	if ((*mp <= '~') && (*mp >= '!')) 
		goto escape;

nextcmd:
	prt("next sp %o %o\n",sp,sp->s_type);
	if ((lp == mlink) && (mp >= mlast)){
		if (mp == mlast) 
			goto stop;
		error(EEOM,1);
	}
	if (*mp > 0) 
		goto data;

	if ((op = *mp++ & 0377) == 0) 
		goto nextcmd;
	prt("op = %o\n",op);

	if (ig){
		if (op == EOM) 
			error(EEOM,2);
		if ((op == FI) & (ig < 0)) ig = 0;
		goto nextcmd;

	}
	op =& 0177;
	if (op >= OPNUM) 
		error(EOP,1);
	if ((login == 0) && (op != (LOGIN-128)) && (argspec[op][0] != 0))
		error(ELOG,1);
	asp = sp;
	prt("for sp = %o %o\n",sp,sp->s_type);
	for (i=0; i<3; i++){
		s = argspec[op][i];
		if (s==0) 
			break;
la:
		t = asp->s_type;
		prt("la %o %o %o %o\n",s,t,asp,sp);
		if (t==s) 
			goto cont;

		if ((t==NTYPE) || (t==WTYPE)){
			deref(asp);
			prt("deref %o\n",asp); 
			goto la;

		}
		if (s < 0) 
			goto cont;

		prt("swit s = %o\n",s);
		switch (s){
		case XTYPE:
			if (t == IITYPE) 
				goto cont;

		case IITYPE:
			tp = zero; 
			break;
		case RTYPE:
			if (wstore[1].s_type != RTYPE) 
				error(EARGT,2);
			tp = &wstore[1].s_val; 
			break;
		case DTYPE:
			if (wstore[0].s_type != DTYPE) 
				error(EARGT,8);
			tp = &wstore[0].s_val; 
			break;
		default:
			error(EARGT,1);
		}
		argpush(&asp);
		prt("argpush %o %o\n",asp,sp);
		asp->s_type = s;
		asp->s_val = *tp++;
		asp->s_val1 = *tp;
prt("arg %o %o %o %o %o\n",asp->s_val,asp->s_val1,tp,zero);
cont:
		asp--;
	}
	switch(op+128){
	case UNLESS:
		if (dstat(ncopy(sp),&entry) >= 0) 
			ig = -1;
	case FI:
		prt("FI");
		goto nextcmd;

	case IF:
		prt("IF %o %o\n",sp,sp->s_type);
		if (dstat(ncopy(sp),&entry) < 0){ 
			ig = -1;
			prt("IF ig = -1\n");
		}
		prt("end IF %o\n",sp->s_type);
		goto nextcmd;

	case SKIP:
		ig = ((sp--)->s_val1&0377) | 0400;
		mp = lp->l_ms;
		goto nextcmd;

	case EOM:
stop:
		if (respond(OK)) {
			prt("respond = 1\n");
			if(!parent){
				exit(EBAD);
			}
			goto trouble;
		}
		goto nextmes;

	case ASSIGN:
		t = (sp--)->s_val1;
		if ((t<0) || (t>=VARNO)) 
			error(EWADDR,1);
assign:
		asp = &wstore[t];
		asp->s_type = sp->s_type;
		asp->s_val = sp->s_val;
		asp->s_val1 = sp->s_val1;
		goto nextcmd;

	case FCREATE:
		cp = ncopy(sp--);
		asp = sp--;
		if (asp->s_type == XTYPE) {
			asp->s_val1 = 0666;
prt("made it 0666\n");
		}
prt("creat mode %o\n",asp->s_val1);
		fil = creat(cp, asp->s_val1);
		goto opn;

	case DCREATE:
		cp = ncopy(sp--);
		asp = sp--;
		tp = mkdir;
		if (dstat(cp,&entry) >= 0) 
			error(EEXIST,1);
execute:
	callsys(cp);
		goto nextcmd;

	case REDEFINE:
		t = 0; 
		goto df;

	case DEFINE:
		t = 1;
df:
		if (nodef[sp->s_type]==0) 
			error(EARGT,3);
		asp = sp--;
		if (sp->s_type != NTYPE) 
			error(EARGT,4);
		ncopy(asp);
		cp = ncopy(sp);
		if (asp->s_type != FTYPE)
			unixid[1].d_type = asp->s_type | 0200;
		if (dstat(cp, &entry) >= 0){
			if (t) 
				error(EEXIST,2);
			unlink(cp);
		}
		if (mklink(unixid,cp) < 0) 
			error(0,1);
		goto nextcmd;

	case DELETE:
		i = dcheck(sp);
		prt("delete dcheck finished\n");
		if (i == DTYPE) {
			tp = rmdir; 
			cp = ncopy(sp);
			goto execute;
		}
		if (i == ATYPE) 
			error(EISACC,1);
		prt("delete copy %s\n",ncopy(sp));
		cp = ncopy(sp--);
		if (unlink(cp) < 0) 
			error(EUNDEF,1);
		prt("delete unlink\n");
		goto nextcmd;

	case OPEN:
		prt("OP %o %o \n",*mp,READ);
		if (sp->s_type == XTYPE) sp->s_val1 = 02;
		asp = sp--;
prt("OPEN %o \n",asp->s_val1);
		if((*mp&0377) == READ){
			asp->s_val1 = 0;
		}else
		if((*mp&0377) == WRITE){
			asp->s_val1 = 1;
		}else
			asp->s_val1 = 2;
		fil = open(ncopy(sp),asp->s_val1);
opn:
		if (fil < 0) 
			error(0,2);
		sp++;
		sp->s_type = RTYPE;
		sp->s_val = 0;
		sp->s_val1 = fil;
		openf[fil] = 1;
		t = 1; 
		goto assign;

	case CLOSE:
		t = (sp--)->s_val1;
		if (openf[t] == 0) 
			error(EFREF,1);
		close(t);
		goto nextcmd;

/*
	case SEEK:
		asp = sp--;
		t = sp->s_val1;
		if (openf[t] == 0) 
error(EFREF,2);
		seek(t,asp->s_val1,0);
		seek(t,asp->s_val,7);
		goto nextcmd;

	case APPEND:
		t = sp->s_val1;
		if (openf[t] == 0) 
error(EFREF,7);
		seek(t,0,2);
		goto nextcmd;

	case TRUNCATE:
		asp = sp--;
		t = sp->s_val1;
		if (openf[t] == 0) 
error(EFREF,3);
		if (asp->s_type != XTYPE){
			seek(t,asp->s_val1,0);
			seek(t,asp->s_val,7);
			}
		trunc(t);
		goto nextcmd;

*/
	case READ:
		asp = sp--;
		fil = sp->s_val1;
		if (openf[fil] == 0) 
			error(EFREF,4);
		k = (asp->s_type ==XTYPE) ? 0 : 1;
		cp = asp->s_val1;
		if (rcopy(fil,k,cp)) 
			goto trouble;
		goto nextcmd;

	case WRITE:
		asp = sp--;
		fil = sp->s_val1;
		if (openf[fil] == 0) 
			error(EFREF,5);
		if (respond(WRITE)) 
			goto trouble;

		k = (asp->s_type == XTYPE) ? 0 : 1;
		cp = asp->s_val1;
		length = 0;
		sum = 0;
		i = 0;
		for (;;){
			if ((n=recv(buf,&code)) < 0) 
				goto trouble;

			length =+ n;
			sum =+ addup(buf,n);
			if (code==3) 
				error(ESIG,code);
			if (code > 4) 
				error(ESIG,code);
			if ((k) && (n>cp)) n=cp;
			if ((n!=0) && (i == 0))
				if (write(fil,buf,n) < 0)
					i = 1;
			if (code > 1) 
				break;
			cp =- n;
		}
		if((nchar = recv(buf,&code)) < 0)
			prt("bad read\n");
		if(code != 3)
			prt("Bad cheksum message\n");
		prt("snstat after %o\n",code);
		prt("code %o %o %o %o\n",sum,length,code,buf[0]);
		rbp = buf;
		rbp++;
		rsum.hibyte = *rbp++;
		rsum.lobyte = *rbp++;
		rlength.hibyte = *rbp++;
		rlength.lobyte = *rbp++;
		prt("received %o %o %o %o\n",buf[1],buf[2],buf[3],buf[4]);
		prt("%o %o\n",rsum,rlength);
		if(sum != rsum){
			prt("Checksum error\n");
		}
		if(length != rlength){
			prt("Byte count error\n");
		}
		if (i) 
			error(0,3);
		goto nextcmd;

	case XMT:
		goto nextcmd;

	case CMODE:
		k = (sp--)->s_val1;
		t = (sp--)->s_val1;
		cp = ncopy(sp);
		if (filstat(cp,&statbuf) < 0) 
			error(EUNDEF,4);
		k = (k & t) | (statbuf->mode & (~t));
		if (chmod(cp,k) < 0) 
			error(0,10);
		goto nextcmd;

	case STATUS:
		prt("STAT %o %d\n",sp,sp->s_type);
		switch(sp->s_type){
		case RTYPE:
			fil = sp->s_val1;
			if (openf[fil] == 0) 
				error(EFREF,6);
			if (desstat(fil,&statbuf) < 0) 
				error(0,7);
			goto stdun;

		case NTYPE:
		case FTYPE:
		case DTYPE:
		case ATYPE:
			if (filstat(ncopy(sp),&statbuf) < 0) 
				error(EUNDEF,3);
stdun:
			*rp++ = STYPE;
			*rp++ = sp->s_type;
prt("stat rp = %o\n",rp);
			rp =+ copy(rp,&statbuf,sizeof(us));
prt("rp +34. = %o\n",rp);
			prt("STATUS to be sent %d\n",sp->s_type);
			goto nextcmd;

		default:
			error(EARGT,7);
		}
	case LIST:
		asp = sp--;
		prt("list  %o %o\n",asp->s_type,unixid);
		prt("list %s %s %o\n",asp->s_val1,ncopy(asp),ncopy(asp));
		if ((fil = open(ncopy(asp),0)) < 0) 
			error(0,8);
		n = rcopy(fil,2,16);
		close(fil);
		if (n) 
			goto trouble;

		goto nextcmd;

	case POP:
		sp--;
		goto nextcmd;

	case CDIR:
		if (chdir(ncopy(sp)) < 0) 
			error(0,4);
		wstore[0].s_type = sp->s_type;
		wstore[0].s_val = sp->s_val;
		wstore[0].s_val1 = (sp--)->s_val1;
		goto nextcmd;

	case LOGIN:
		closeall();
		if (getpwentry(sp->s_val1,buf)) 
			error(EPWD,1);
		login = 1;
		if(parent){
			parent = 0;
			if((pid = fork())!= -1){
				if(pid != CHILD){
					parent = 1;
					while(pid != wait(&status));
					if(((status>>8)&0377) == EBAD)
						prt("Daemon error\n");
					logout();
					goto nextmes;
				}else{
					setuid(uid);
				}
	
			}else{
				prt("Can't fork\n");
			}
		}
		goto stop;

	case QUIT:
		closeall();
		if(respond(OK))
			if(!parent){
				prt("going to exit\n");
				exit(EBAD);
			}
		if(!parent){
			prt("going to exit - good\n");
			exit(EGOOD);
		}

		logout();
		goto nextmes;
	case UNIQUE:
prt("asp = %o\n",asp);
/*
*/
prt("asp = %o %o\n",asp,sp);
		up = uniqb;
		cp = ncopy(sp--);
		asp = sp--;
		if(asp->s_type == XTYPE){
			asp->s_val1 = 0666;
		}
prt("saved %s\n",cp);
		up =+ copy(uniqb,cp,size(cp));
prt("copied %s\n",uniqb);
		*up++ = '/';
		copy(up,uniqs,size(uniqs));
		prt("unique name %s\n",uniqb);
		up = mktemp(uniqb);
	prt("mktmp name %s\n",up);
		*rp++ = UNIQUE;
		rp =+ copy(rp,uniqb,size(uniqb));
		*rp++ = '\0';
prt("resp name %s\n",resp);
		close(creat(up,asp->s_val1));
		fil = open(up,2);
		goto opn;
	default:
		error(EOP,2);
	}
data:
	prt("data %d\n",ig);
	if (ig){
		switch (*mp++){
		case LTYPE:
			if (((*mp++ & 0377) + 0400) == ig) 
				ig = 0;
			goto nextcmd;

		case NTYPE:
			while (*mp++);
			goto nextcmd;

		default:
			mp =+ 2;
		case ITYPE:
		case WTYPE:
		case RTYPE:
			mp =+ 2;
			goto nextcmd;

		}
	}
	sp++;
	sp->s_type = *mp++;
	prt("data2 %o %o %o\n",sp,sp->s_type,*mp);
	switch (sp->s_type){
	case NTYPE:
		sp->s_val1 = mp;
		if (*mp == 0) 
			error(EARGT,9);
		while (*mp++);
		prt("NTYPE %o\n",sp->s_type);
		goto nextcmd;

	case ITYPE:
		sp->s_type = IITYPE;
	case WTYPE:
prt("mp = %o\n",mp);
		mp =+ copy(&sp->s_val1,mp,2);
prt("mp = %o\n",mp);
		sp->s_val = (sp->s_val1 < 0) ? -1 : 0;
		goto nextcmd;

	case RTYPE:
		error(EARGT,5);
	case LTYPE:
		sp--;
		mp++;
		goto nextcmd;

	case ETYPE:
		sp--;
		mp =+ 2;
		error(*mp++, *mp++);
	}
	mp =+ copy(&sp->s_val,mp,4);
	goto nextcmd;

escape:
	if (login == 0) 
		error(ELOG, 2);
	if ((pipe(rpfil) < 0) || (pipe(wpfil) < 0))
		error(0,11);
	mbuf[n++] = '\n';
	write(rpfil[WPIPE],mbuf,n);
	close(rpfil[WPIPE]);
	while((i = fork()) == -1) 
		sleep(1);
	if (i == CHILD){
		if (rpfil[RPIPE] != 0) {
			close(INPUT); 
			dup(rpfil[RPIPE]);
		}
		if (wpfil[WPIPE] != 1) {
			close(OUTPUT); 
			dup(wpfil[WPIPE]);
		}
		for (i=2; i<15; i++) 
			close(i);
		dup(OUTPUT);
		execl(minishell,minishell,"-t",0);
		mesg("Can't execute shell");
		exit(EBAD);
	}
	close(wpfil[WPIPE]); 
	close(rpfil[RPIPE]);
/*
	if(desstat(wpfil[RPIPE],kk) < 0){
		prt("stat failed of pipe file\n");
	}else{
		prt("stat %o %o %o\n",kk[0],kk[1],kk[5]);
	}
*/
	if (n = rcopy(wpfil[RPIPE],-1,0)){
		kill(i,9);
	}
	while(wait(&status) != i);
	close(wpfil[RPIPE]);
	if (n) 
		goto trouble; 
	else
		 goto stop;

}
mklink(p,q)
char	*p,*q;
{
	return(link(p,q));
}
callsys(cp)
char	*cp;
{
	register i;

		while ((i=fork()) == -1) 
			sleep(1);
		if (i==CHILD){
			for (i=0; i<15; i++) close(i);
			execl(tp,tp,cp,0);
			execl(&tp[4],&tp[4],cp,0);
			mesg("Can't exec");
			mesg(cp);
			exit(EBAD);
		}
		while (i != wait(&pstat));
		if (pstat.w_stat == EBAD) 
			error(EFAIL,1);
}
