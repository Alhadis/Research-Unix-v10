#include "sam.h"
#include "parse.h"

int	nl_cmd(), a_cmd(), b_cmd(), c_cmd(), cd_cmd(), d_cmd(), D_cmd(), e_cmd();
int	f_cmd(), g_cmd(), i_cmd(), k_cmd(), m_cmd(), n_cmd(), p_cmd(), q_cmd();
int	s_cmd(), u_cmd(), w_cmd(), x_cmd(), X_cmd(), unix_cmd(), eq_cmd();

static char	linex[]="\n";
static char	wordx[]=" \t\n";
struct cmdtab cmdtab[]={
/*	cmdc	text	regexp	addr	defcmd	defaddr	count	token	 fn	*/
	'\n',	0,	0,	0,	0,	aDot,	0,	0,	nl_cmd,
	'a',	1,	0,	0,	0,	aDot,	0,	0,	a_cmd,
	'b',	0,	0,	0,	0,	aNo,	0,	linex,	b_cmd,
	'B',	0,	0,	0,	0,	aNo,	0,	linex,	b_cmd,
	'c',	1,	0,	0,	0,	aDot,	0,	0,	c_cmd,
	'd',	0,	0,	0,	0,	aDot,	0,	0,	d_cmd,
	'D',	0,	0,	0,	0,	aNo,	0,	linex,	D_cmd,
	'e',	0,	0,	0,	0,	aNo,	0,	wordx,	e_cmd,
	'f',	0,	0,	0,	0,	aNo,	0,	wordx,	f_cmd,
	'g',	0,	1,	0,	'p',	aDot,	0,	0,	g_cmd,
	'i',	1,	0,	0,	0,	aDot,	0,	0,	i_cmd,
	'k',	0,	0,	0,	0,	aDot,	0,	0,	k_cmd,
	'm',	0,	0,	1,	0,	aDot,	0,	0,	m_cmd,
	'n',	0,	0,	0,	0,	aNo,	0,	0,	n_cmd,
	'p',	0,	0,	0,	0,	aDot,	0,	0,	p_cmd,
	'q',	0,	0,	0,	0,	aNo,	0,	0,	q_cmd,
	'r',	0,	0,	0,	0,	aDot,	0,	wordx,	e_cmd,
	's',	0,	1,	0,	0,	aDot,	1,	0,	s_cmd,
	't',	0,	0,	1,	0,	aDot,	0,	0,	m_cmd,
	'u',	0,	0,	0,	0,	aNo,	1,	0,	u_cmd,
	'v',	0,	1,	0,	'p',	aDot,	0,	0,	g_cmd,
	'w',	0,	0,	0,	0,	aAll,	0,	wordx,	w_cmd,
	'x',	0,	1,	0,	'p',	aDot,	0,	0,	x_cmd,
	'y',	0,	1,	0,	'p',	aDot,	0,	0,	x_cmd,
	'X',	0,	1,	0,	'f',	aNo,	0,	0,	X_cmd,
	'Y',	0,	1,	0,	'f',	aNo,	0,	0,	X_cmd,
	'!',	0,	0,	0,	0,	aNo,	0,	linex,	unix_cmd,
	'>',	0,	0,	0,	0,	aDot,	0,	linex,	unix_cmd,
	'<',	0,	0,	0,	0,	aDot,	0,	linex,	unix_cmd,
	'|',	0,	0,	0,	0,	aDot,	0,	linex,	unix_cmd,
	'=',	0,	0,	0,	0,	aDot,	0,	linex,	eq_cmd,
	'c'|0x100,0,	0,	0,	0,	aNo,	0,	wordx,	cd_cmd,
	0,	0,	0,	0,	0,	0,	0,	0,
};
Cmd	*parsecmd();
Addr	*compoundaddr();
Addr	*simpleaddr();
uchar	line[BLOCKSIZE];
uchar	termline[BLOCKSIZE];
uchar	*linep=line;
uchar	*terminp=termline;
uchar	*termoutp=termline;
List	cmdlist;
List	addrlist;
List	relist;
List	stringlist;
int	eof;

resetcmd(){
	linep=line;
	*linep=0;
	terminp=termoutp=termline;
	freecmd();
}
getc(){
	if(eof)
		return -1;
	if(*linep==0 && inputline()<0){
		eof=TRUE;
		return -1;
	}
	return *linep++;
}
nextc(){
	if(*linep==0)
		return -1;
	return *linep;
}
ungetc(){
	if(--linep<line)
		panic("ungetc");
}
inputline(){
	register i, c;
	linep=line;
	i=0;
	do{
		if((c=inputc())<=0)
			return -1;
		if(i==(sizeof line)-1)
			error(Etoolong);
	}while((line[i++]=c)!='\n');
	line[i]=0;
	return 1;
}
termcommand()
{
	register Posn p;
	dounlock=TRUE;
	Fgetcset(cmd, cmdpt);
	for(p=cmdpt; p<cmd->nbytes; p++)
		*terminp++=Fgetc(cmd);
	cmdpt=cmd->nbytes;
}
inputc()
{
	register c, n;
	uchar cc;
   Again:
	if(downloaded){
		while(termoutp==terminp){
			cmdupdate();
			if(dounlock){
				outT0(Hunlock);
				dounlock=FALSE;
			}
			if(patset)
				tellpat();
			if(rcv()==0)
				return -1;
		}
		c=*termoutp++;
		if(termoutp==terminp)
			terminp=termoutp=termline;
		n=1;
	}else{
		n=read(0, (char *)&cc, 1);
		c=cc;
	}
	if(n<=0)
		return -1;
	if(c==0 || (c&HIGHBIT)){
		warn(Wnonascii);
		goto Again;
	}
	return c;
}
cmdloop(){
	Cmd *cmdp;
	File *ocurfile;
	int loaded;
	for(;;){
		if(!downloaded && curfile && curfile->state==Unread)
			load(curfile);
		if((cmdp=parsecmd(0))==0)
			break;
		ocurfile=curfile;
		loaded=curfile && curfile->state!=Unread;
		if(cmdexec(curfile, cmdp)==0)
			break;
		freecmd();
		cmdupdate();
		update();
		if(downloaded && curfile &&
		    (ocurfile!=curfile || (!loaded && curfile->state!=Unread)))
			outTs(Hcurrent, curfile->tag);
	}
}
Cmd *
newcmd(){
	register Cmd *p;
	p=new(Cmd, 1);
	inslist(&cmdlist, cmdlist.nused, (long)p);
	return p;
}
Addr *
newaddr(){
	register Addr *p;
	p=new(Addr, 1);
	inslist(&addrlist, addrlist.nused, (long)p);
	return p;
}
Regexp *
newre(){
	register Regexp *p;
	p=new(Regexp, 1);
	inslist(&relist, relist.nused, (long)p);
	strinit(&p->text);
	return p;
}
String *
newstring(){
	register String *p;
	p=new(String, 1);
	inslist(&stringlist, stringlist.nused, (long)p);
	strinit(p);
	return p;
}
freecmd()
{
	register i;
	while(cmdlist.nused>0)
		free((uchar *)cmdlist.ptr[--cmdlist.nused]);
	while(addrlist.nused>0)
		free((uchar *)addrlist.ptr[--addrlist.nused]);
	while(relist.nused>0){
		i=--relist.nused;
		strclose(&((Regexp *)relist.ptr[i])->text);
		free((uchar *)relist.ptr[i]);	/* WARNING!! */
	}
	while(stringlist.nused>0){
		i=--stringlist.nused;
		strclose((String *)stringlist.ptr[i]);
		free((uchar *)stringlist.ptr[i]);
	}
}
lookup(c)
	register c;
{
	register i;
	for(i=0; cmdtab[i].cmdc; i++)
		if(cmdtab[i].cmdc==c)
			return i;
	return -1;
}
okdelim(c){
	if(c=='\\' || ('a'<=c && c<='z') || ('A'<=c && c<='Z') || ('0'<=c && c<='9'))
		error_c(Edelim, c);
}
atnl(){
	skipbl();
	if(getc()!='\n')
		error(Enewline);
}
getrhs(s, delim, cmd)
	register String *s;
	register delim;
{
	register c;
	while((c=getc())>0 && c!=delim && c!='\n'){
		if(c=='\\'){
			if((c=getc())<=0)
				error(Ebadrhs);
			if(c=='\n'){
				ungetc();
				c='\\';
			}else if(c=='n')
				c='\n';
			else if(c!=delim && (cmd=='s' || c!='\\'))	/* s does its own */
				straddc(s, '\\');
		}
		straddc(s, c);
	}
	ungetc();	/* let client read whether delimeter, '\n' or whatever */
}
String *
collecttoken(end)
	register char *end;
{
	register String *s=newstring();
	register c;
	while((c=nextc())==' ' || c=='\t')
		straddc(s, getc()); /* blanks significant for getname() */
	while((c=getc())>0 && strchr(end, c)==0)
		straddc(s, c);
	straddc(s, 0);
	if(c!='\n')
		atnl();
	return s;
}
String *
collecttext(){
	register String *s=newstring();
	register begline, i;
	register c, delim;
	if(skipbl()=='\n'){
		getc();
		i=0;
		do{
			begline=i;
			while((c=getc())>0 && c!='\n')
				i++, straddc(s, c);
			i++, straddc(s, '\n');
			if(c<0)
				goto Return;
		}while(s->s[begline]!='.' || s->s[begline+1]!='\n');
		strdelete(s, (long)s->n-2, (long)s->n);
	}else{
		okdelim(delim=getc());
		getrhs(s, delim, 'a');
		if(nextc()==delim)
			getc();
		atnl();
	}
    Return:
	straddc(s, 0);		/* JUST FOR CMDPRINT() */
	return s;
}
Cmd *
parsecmd(nest)
{
	register i, c;
	register struct cmdtab *ct;
	register Cmd *cp, *ncp;
	Cmd cmd;
	cmd.next=cmd.ccmd=0;
	cmd.re=0;
	cmd.flag=cmd.num=0;
	cmd.addr=compoundaddr();
	if(skipbl()==-1)
		return 0;
	if((c=getc())==-1)
		return 0;
	cmd.cmdc=c;
	if(cmd.cmdc=='c' && nextc()=='d'){	/* sleazy two-character case */
		getc();		/* the 'd' */
		cmd.cmdc='c'|0x100;
	}
	i=lookup(cmd.cmdc);
	if(i>=0){
		if(cmd.cmdc=='\n')
			goto Return;	/* let nl_cmd work it all out */
		ct=&cmdtab[i];
		if(ct->defaddr==aNo && cmd.addr)
			error(Enoaddr);
		if(ct->count)
			cmd.num=getnum();
		if(ct->regexp){
			/* x without pattern -> .*\n, indicated by cmd.re==0 */
			/* X without pattern is all files */
			if((ct->cmdc!='x' && ct->cmdc!='X') ||
			   ((c=nextc())!=' ' && c!='\t' && c!='\n')){
				skipbl();
				if((c=getc())=='\n' || c<0)
					error(Enopattern);
				okdelim(c);
				cmd.re=getregexp(c);
				if(ct->cmdc=='s'){
					cmd.ctext=newstring();
					getrhs(cmd.ctext, c, 's');
					if(nextc()==c){
						getc();
						if(nextc()=='g')
							cmd.flag=getc();
					}
			
				}
			}
		}
		if(ct->addr && (cmd.caddr=simpleaddr())==0)
			error(Eaddress);
		if(ct->defcmd){
			if(skipbl()=='\n'){
				getc();
				cmd.ccmd=newcmd();
				cmd.ccmd->cmdc=ct->defcmd;
			}else if((cmd.ccmd=parsecmd(nest))==0)
				panic("defcmd");
		}else if(ct->text)
			cmd.ctext=collecttext();
		else if(ct->token)
			cmd.ctext=collecttoken(ct->token);
		else
			atnl();
	}else
		switch(cmd.cmdc){
		case '{':
			cp=0;
			do{
				if(skipbl()=='\n')
					getc();
				ncp=parsecmd(nest+1);
				if(cp)
					cp->next=ncp;
				else
					cmd.ccmd=ncp;
			}while(cp=ncp);
			break;
		case '}':
			atnl();
			if(nest==0)
				error(Enolbrace);
			return 0;
		default:
			error_c(Eunk, cmd.cmdc);
		}
    Return:
	cp=newcmd();
	*cp=cmd;
	return cp;
}
Regexp *				/* BUGGERED */
getregexp(delim)
	register delim;
{
	register Regexp *r=newre();
	register c;
	for(strzero(&genstr); ; straddc(&genstr, c))
		if((c=getc())=='\\'){
			if(nextc()==delim)
				c=getc();
			else if(nextc()=='\\'){
				straddc(&genstr, c);
				c=getc();
			}
		}else if(c==delim || c=='\n')
			break;
	if(c!=delim && c)
		ungetc();
	if(genstr.n>0){
		patset=TRUE;
		strdupstr(&lastpat, &genstr);
		straddc(&lastpat, '\0');
	}
	if(lastpat.n<=1)
		error(Epattern);
	strdupstr(&r->text, &lastpat);
	return r;
}
Addr *
compoundaddr(){
	Addr addr;
	register Addr *ap, *next;
	addr.aprev=simpleaddr();
	if((addr.type=skipbl())!=',' && addr.type!=';')
		return addr.aprev;
	getc();
	next=addr.next=compoundaddr();
	if(next && (next->type==',' || next->type==';') && next->aprev==0)
		error(Eaddress);
	ap=newaddr();
	*ap=addr;
	return ap;
}
Addr *
simpleaddr()
{
	Addr addr;
	register Addr *ap, *nap;
	addr.next=0;
	addr.aprev=0;
	switch(skipbl()){
	case '#':
		addr.type=getc();
		addr.num=getnum();
		break;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9': 
		addr.num=getnum();
		addr.type='l';
		break;
	case '/': case '?': case '"':
		addr.are=getregexp(addr.type=getc());
		break;
	case '.':
	case '$':
	case '+':
	case '-':
	case '\'':
		addr.type=getc();
		break;
	default:
		return 0;
	}
	if(addr.next=simpleaddr())
		switch(addr.next->type){
		case '.':
		case '$':
		case '\'':
			if(addr.type!='"')
		case '"':
				error(Eaddress);
			break;
		case 'l':
		case '#':
			if(addr.type=='"')
				break;
			/* fall through */
		case '/':
		case '?':
			if(addr.type!='+' && addr.type!='-'){
				/* insert the missing '+' */
				nap=newaddr();
				nap->type='+';
				nap->next=addr.next;
				addr.next=nap;
			}
			break;
		case '+':
		case '-':
			break;
		default:
			panic("simpleaddr");
		}
	ap=newaddr();
	*ap=addr;
	return ap;
}
