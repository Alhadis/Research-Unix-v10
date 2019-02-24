#include "sam.h"
#include "parse.h"

#ifdef	lint
#define	UNUSED(c)	NONEXISTENT((char *)c)
#else
#define	UNUSED(c)
#endif

int	Glooping;
int	nest;

resetxec(){
	Glooping=nest=0;
}
cmdexec(f, cp)
	register File *f;
	register Cmd *cp;
{
	register i;
	register Addr *ap;
	Address a;
	if(f && f->state==Unread)
		load(f);
	if(f==0 && (cp->addr==0 || cp->addr->type!='"') &&
	    !strchr("bBnqUXY!{", cp->cmdc) &&
	    cp->cmdc!=('c'|0x100) && !(cp->cmdc=='D' && cp->ctext))
		error(Enofile);
	i=lookup(cp->cmdc);
	if(cmdtab[i].defaddr!=aNo){
		if((ap=cp->addr)==0 && cp->cmdc!='\n'){
			cp->addr=ap=newaddr();
			ap->type='.';
			if(cmdtab[i].defaddr==aAll)
				ap->type='*';
		}else if(ap && ap->type=='"' && ap->next==0 && cp->cmdc!='\n'){
			ap->next=newaddr();
			ap->next->type='.';
			if(cmdtab[i].defaddr==aAll)
				ap->next->type='*';
		}
		if(cp->addr){	/* may be false for '\n' (only) */
			addr=address(ap, f->dot, 0);
			f=addr.f;
		}
	}
	current(f);
	switch(cp->cmdc){
	case '{':
		a=cp->addr? address(cp->addr, f->dot, 0): f->dot;
		for(cp=cp->ccmd; cp; cp=cp->next){
			a.f->dot=a;
			cmdexec(a.f, cp);
		}
		break;
	default:
		i=(*cmdtab[i].fn)(f, cp);
		return i;
	}
	return 1;
}
a_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	return append(f, cp, addr.r.p2);
}
b_cmd(f, cp)
	register File *f;
	Cmd *cp;
{
	extern File *tofile(), *getfile();
	UNUSED(f);
	f=cp->cmdc=='b'? tofile(cp->ctext) : getfile(cp->ctext);
	if(f->state==Unread)
		load(f);
	else if(nest==0)
		filename(f);
	return TRUE;
}
c_cmd(f, cp)
	register File *f;
	Cmd *cp;
{
	Fdelete(f, addr.r.p1, addr.r.p2);
	f->dot.r.p1=f->dot.r.p2=addr.r.p2;
	return append(f, cp, addr.r.p2);
}
d_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	UNUSED(cp);
	Fdelete(f, addr.r.p1, addr.r.p2);
	f->dot.r.p1=f->dot.r.p2=addr.r.p1;
	return TRUE;
}
D_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	closefiles(f, cp->ctext);
	return TRUE;
}
e_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	if(getname(f, cp->ctext, cp->cmdc=='e')==0)
		error(Enoname);
	edit(f, cp->cmdc);
	return TRUE;
}
f_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	getname(f, cp->ctext, TRUE);
	filename(f);
	return TRUE;
}
g_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	if(f!=addr.f)panic("g_cmd f!=addr.f");
	compile(cp->re);
	if(execute(f, addr.r.p1, addr.r.p2) ^ cp->cmdc=='v'){
		f->dot=addr;
		return cmdexec(f, cp->ccmd);
	}
	return TRUE;
}
i_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	return append(f, cp, addr.r.p1);
}
k_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	UNUSED(cp);
	f->mark=addr.r;
	return TRUE;
}
m_cmd(f, cp)
	register File *f;
	register Cmd *cp;
{
	Address addr2;
	addr2=address(cp->caddr, f->dot, 0);
	if(cp->cmdc=='m')
		move(f, addr2);
	else
		copy(f, addr2);
	current(addr2.f);
	return TRUE;
}
n_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	register i;
	UNUSED(f);
	UNUSED(cp);
	for(i=0; i<file.nused; i++){
		if(file.ptr[i]==cmd)
			continue;
		strdupstr(&genstr, &file.ptr[i]->name);
		filename(file.ptr[i]);
	}
	return TRUE;
}
p_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	UNUSED(cp);
	return display(f);
}
q_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	UNUSED(cp);
	UNUSED(f);
	trytoquit();
	if(downloaded){
		outT0(Hexit);
		return TRUE;
	}
	return FALSE;
}
s_cmd(f, cp)
	register File *f;
	register Cmd *cp;
{
	register i, j, c, n;
	register Posn p1, op, didsub=0, delta=0;
	n=cp->num;
	op=-1;
	compile(cp->re);
	for(p1=addr.r.p1; p1<=addr.r.p2 && execute(f, p1, addr.r.p2); ){
		if(sel.p[0].p1==sel.p[0].p2){	/* empty match? */
			if(sel.p[0].p1==op){
				p1++;
				continue;
			}
			p1=sel.p[0].p2+1;
		}else
			p1=sel.p[0].p2;
		op=sel.p[0].p2;
		if(--n>0)
			continue;
		strzero(&genstr);
		for(i=0; i<cp->ctext->n; i++)
			if((c=cp->ctext->s[i])=='\\' && i<cp->ctext->n-1){
				c=cp->ctext->s[++i];
				if('1'<=c && c<='9') {
					j=c-'0';
					if(sel.p[j].p2-sel.p[j].p1>BLOCKSIZE)
						error(Elongtag);
					Fchars(f, genbuf, sel.p[j].p1, sel.p[j].p2);
					strinsert(&genstr, tempstr(genbuf, (int)(sel.p[j].p2-sel.p[j].p1)), (ulong)genstr.n);
				}else
				 	straddc(&genstr, c);
			}else if(c!='&')
				straddc(&genstr, c);
			else{
				if(sel.p[0].p2-sel.p[0].p1>BLOCKSIZE)
					error(Elongrhs);
				Fchars(f, genbuf, sel.p[0].p1, sel.p[0].p2);
				strinsert(&genstr,
					tempstr(genbuf, (int)(sel.p[0].p2-sel.p[0].p1)),
					(ulong)genstr.n);
			}
		if(sel.p[0].p1!=sel.p[0].p2){
			Fdelete(f, sel.p[0].p1, sel.p[0].p2);
			delta-=sel.p[0].p2-sel.p[0].p1;
		}
		if(genstr.n){
			Finsert(f, &genstr, sel.p[0].p2);
			delta+=genstr.n;
		}
		didsub=1;
		if(!cp->flag)
			break;
	}
	if(!didsub && nest==0)
		error(Enosub);
	f->dot.r.p1=addr.r.p1, f->dot.r.p2=addr.r.p2+delta;
	return TRUE;
}
u_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	register n;
	UNUSED(f);
	UNUSED(cp);
	n=cp->num;
	while(n--)
		undo();
	return TRUE;
}
w_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	if(getname(f, cp->ctext, FALSE)==0)
		error(Enoname);
	writef(f);
	return TRUE;
}
x_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	if(cp->re)
		looper(f, cp, cp->cmdc=='x');
	else
		linelooper(f, cp);
	return TRUE;
}
X_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	UNUSED(f);
	filelooper(cp, cp->cmdc=='X');
	return TRUE;
}
unix_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	Unix(f, cp->cmdc, cp->ctext, nest);
	return TRUE;
}
eq_cmd(f, cp)
	File *f;
	Cmd *cp;
{
	register charsonly;
	switch(cp->ctext->n){
	case 1:
		charsonly=FALSE;
		break;
	case 2:
		if(cp->ctext->s[0]=='#'){
			charsonly=TRUE;
			break;
		}
	default:
		error(Enewline);
	}
	printposn(f, charsonly);
	return TRUE;
}
nl_cmd(f, cp)
	register File *f;
	register Cmd *cp;
{
	if(cp->addr==0){
		/* First put it on newline boundaries */
		addr=lineaddr((Posn)0, f->dot, -1);
		addr.r.p2=lineaddr((Posn)0, f->dot, 1).r.p2;
		if(addr.r.p1==f->dot.r.p1 && addr.r.p2==f->dot.r.p2)
			addr=lineaddr((Posn)1, f->dot, 1);
		display(f);
	}else if(downloaded)
		moveto(f, addr.r);
	else
		display(f);
	return TRUE;
}
cd_cmd(f, cp)
	register File *f;
	register Cmd *cp;
{
	UNUSED(f);
	cd(cp->ctext);
	return TRUE;
}
append(f, cp, p)
	register File *f;
	register Cmd *cp;
	register Posn p;
{
	if(cp->ctext->n>0 && cp->ctext->s[cp->ctext->n-1]==0)
		--cp->ctext->n;
	if(cp->ctext->n>0)
		Finsert(f, cp->ctext, p);
	f->dot.r.p1=p;
	f->dot.r.p2=p+cp->ctext->n;
	return TRUE;
}
display(f)
	register File *f;
{
	register Posn p1=addr.r.p1, p2=addr.r.p2;
	register np, n;
	if(f!=addr.f)panic("display f!=addr.f");
	while(p1<p2){
		np=p2-p1;
		if(np>BLOCKSIZE)
			np=BLOCKSIZE;
		n=Fchars(f, genbuf, p1, p1+np);
		if(n<=0)
			panic("display");
		if(downloaded)
			termwrite(genbuf, n);
		else
			Write(1, genbuf, n);
		p1+=n;
	}
	f->dot=addr;
	return TRUE;
}
looper(f, cp, xy)
	register File *f;
	register Cmd *cp;
{
	register Posn p, op;
	Range r;
	r=addr.r;
	op= xy? -1 : r.p1;
	nest++;
	compile(cp->re);
	for(p=r.p1; p<=r.p2; ){
		if(!execute(f, p, r.p2)){ /* no match, but y should still run */
			if(xy || op>r.p2)
				break;
			f->dot.r.p1=op, f->dot.r.p2=r.p2;
			p=r.p2+1;	/* exit next loop */
		}else{
			if(sel.p[0].p1==sel.p[0].p2){	/* empty match? */
				if(sel.p[0].p1==op){
					p++;
					continue;
				}
				p=sel.p[0].p2+1;
			}else
				p=sel.p[0].p2;
			if(xy)
				f->dot.r=sel.p[0];
			else
				f->dot.r.p1=op, f->dot.r.p2=sel.p[0].p1;
		}
		op=sel.p[0].p2;
		cmdexec(f, cp->ccmd);
		compile(cp->re);
	}
	--nest;
}
linelooper(f, cp)
	register File *f;
	register Cmd *cp;
{
	register Posn p;
	Range r, linesel;
	Address a3;
	nest++;
	r=addr.r;
	a3.f=f;
	a3.r.p1=a3.r.p2=r.p1;
	for(p=r.p1; p<r.p2; p=a3.r.p2){
		a3.r.p1=a3.r.p2;
/*pjw		if(p!=r.p1 || (linesel=lineaddr((Posn)0, a3, 1)).r.p2==p)*/
		if(p!=r.p1 || ((linesel=lineaddr((Posn)0, a3, 1).r), linesel.p2==p))
			linesel=lineaddr((Posn)1, a3, 1).r;
		if(linesel.p1>=r.p2)
			break;
		if(linesel.p2>=r.p2)
			linesel.p2=r.p2;
		if(linesel.p2>linesel.p1)
			if(linesel.p1>=a3.r.p2 && linesel.p2>a3.r.p2){
				f->dot.r=linesel;
				cmdexec(f, cp->ccmd);
				a3.r=linesel;
				continue;
			}
		break;
	}
	--nest;
}
filelooper(cp, XY)
	register Cmd *cp;
{
	register File *f, *cur;
	register i;
	if(Glooping++)
		error(EnestXY);
	nest++;
	settempfile();
	cur=curfile;
	for(i=0; i<tempfile.nused; i++){
		f=tempfile.ptr[i];
		if(f==cmd)
			continue;
		if(cp->re==0 || filematch(f, cp->re)==XY)
			cmdexec(f, cp->ccmd);
	}
	if(cur && whichmenu(cur)>=0)	/* check that cur is still a file */
		current(cur);
	--Glooping;
	--nest;
}
