#include "sam.h"
#include "parse.h"

Address	addr;
Address	charaddr();
String	lastpat;
int	patset;
File	*matchfile();
File	*menu;

Address
address(ap, a, sign)
	register Addr *ap;
	Address a;
{
	register File *f=a.f;
	Address a1, a2;
	do{
		switch(ap->type){
		case 'l':
		case '#':
			a=(*(ap->type=='#'?charaddr:lineaddr))((Posn)ap->num, a, sign);
			break;
		case '.':
			a=f->dot;
			break;
		case '$':
			a.r.p1=a.r.p2=f->nbytes;
			break;
		case '\'':
			a.r=f->mark;
			break;
		case '?':
			sign= -sign;
			if(sign==0)
				sign=-1;
			/* fall through */
		case '/':
			nextmatch(f, ap->are, sign>=0? a.r.p2 : a.r.p1, sign);
			a.r=sel.p[0];
			break;
		case '"':
			a=matchfile(ap->are)->dot;
			f=a.f;
			if(f->state==Unread)
				load(f);
			break;
		case '*':
			a.r.p1=0, a.r.p2=f->nbytes;
			return a;
		case ',':
		case ';':
			if(ap->aprev)
				a1=address(ap->aprev, a, 0);
			else
				a1.f=a.f, a1.r.p1=a1.r.p2=0;
			if(ap->type==';'){
				f=a1.f;
				f->dot=a=a1;
			}
			if(ap->next)
				a2=address(ap->next, a, 0);
			else
				a2.f=a.f, a2.r.p1=a2.r.p2=f->nbytes;
			if(a1.f!=a2.f)
				error(Eorder);
			a.f=a1.f, a.r.p1=a1.r.p1, a.r.p2=a2.r.p2;
			if(a.r.p2<a.r.p1)
				error(Eorder);
			return a;
		case '+':
		case '-':
			sign=1;
			if(ap->type=='-')
				sign=-1;
			if(ap->next==0 || ap->next->type=='+' || ap->next->type=='-')
				a=lineaddr(1L, a, sign);
			break;
		default:
			panic("address");
			return a;
		}
	}while(ap=ap->next);
	return a;
}
nextmatch(f, r, p, sign)
	register File *f;
	register Regexp *r;
	register Posn p;
{
	compile(r);
	if(sign>=0){
		if(!execute(f, p, INFINITY))
			error(Esearch);
		if(sel.p[0].p1==sel.p[0].p2 && sel.p[0].p1==p){
			if(++p>f->nbytes)
				p=0;
			if(!execute(f, p, INFINITY))
				panic("address");
		}
	}else{
		if(!bexecute(f, p))
			error(Esearch);
		if(sel.p[0].p1==sel.p[0].p2 && sel.p[0].p2==p){
			if(--p<0)
				p=f->nbytes;
			if(!bexecute(f, p))
				panic("address");
		}
	}
}
File *
matchfile(r)
	Regexp *r;
{
	register File *f;
	register File *match=0;
	register i;
	for(i=0; i<file.nused; i++){
		f=file.ptr[i];
		if(f==cmd)
			continue;
		if(filematch(f, r)){
			if(match)
				error(Emanyfiles);
			match=f;
		}
	}
	if(!match)
		error(Efsearch);
	return match;
}
filematch(f, r)
	register File *f;
	register Regexp *r;
{
	sprint((char *)genbuf, "%c%c%c %s\n", " '"[f->state==Dirty],
		"-+"[f->rasp!=0], " ."[f==curfile], f->name.s);
	strdup(&genstr, genbuf);
	/* A little dirty... */
	if(menu==0)
		(menu=Fopen())->state=Clean;
	Bdelete(menu->buf, (Posn)0, menu->buf->nbytes);
	Binsert(menu->buf, &genstr, (Posn)0);
	menu->nbytes=menu->buf->nbytes;
	compile(r);
	return execute(menu, (Posn)0, menu->nbytes);
}
Address
charaddr(l, addr, sign)
	register Posn l;
	Address addr;
{
	if(sign==0)
		addr.r.p1=addr.r.p2=l;
	else if(sign<0)
		addr.r.p2=addr.r.p1-=l;
	else if(sign>0)
		addr.r.p1=addr.r.p2+=l;
	if(addr.r.p1<0 || addr.r.p2>addr.f->nbytes)
		error(Erange);
	return addr;
}
Address
lineaddr(l, addr, sign)
	register Posn l;
	Address addr;
{
	register n;
	register c;
	register File *f=addr.f;
	Address a;
	a.f=f;
	if(sign>=0){
		if(l==0){
			if(sign==0 || addr.r.p2==0){
				a.r.p1=a.r.p2=0;
				return a;
			}
			a.r.p1=addr.r.p2;
			Fgetcset(f, addr.r.p2-1);
		}else{
			if(sign==0 || addr.r.p2==0){
				Fgetcset(f, (Posn)0);
				n=1;
			}else{
				Fgetcset(f, addr.r.p2-1);
				n=Fgetc(f)=='\n';
			}
			for(; n<l; ){
				c=Fgetc(f);
				if(c==-1)
					error(Erange);
				else if(c=='\n')
					n++;
			}
			a.r.p1=f->getcp;
		}
		do; while((c=Fgetc(f))!='\n' && c!=-1);
		a.r.p2=f->getcp;
	}else{
		Fbgetcset(f, addr.r.p1);
		if(l==0)
			a.r.p2=addr.r.p1;
		else{
			for(n=0; n<l; ){	/* always runs once */
				c=Fbgetc(f);
				if(c=='\n')
					n++;
				else if(c==-1){
					if(++n!=l)
						error(Erange);
				}
			}
			a.r.p2=f->getcp;
			if(c=='\n')
				a.r.p2++;	/* lines start after a newline */
		}
		do; while((c=Fbgetc(f))!='\n' && c!=-1);
		a.r.p1=f->getcp;
		if(c=='\n')
			a.r.p1++;	/* lines start after a newline */
	}
	return a;
}
