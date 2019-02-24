#include <jerq.h>
#include <layer.h>
#include <queue.h>
#include "jerqproc.h"
#include "frame.h"

frselect(f, pt)
	register Frame *f;
	Point pt;
{
	register fix, var, oldvar;
	if(f->s1 != f->s2){
		selectf(f, F_XOR);
		f->nullsel=0;	/* no double clicks this time */
	}
	fix=charofpt(f, pt);	/* fixed point */
	oldvar=fix;		/* moving point tracks mouse */
	var=fix;
	while(button1()){
		if(var!=oldvar){
			f->s1=oldvar;
			f->s2=var;
			order(f);
			selectf(f, F_XOR);
			oldvar=var;
		}
		var=charofpt(f, mouse.xy);
	}
	f->s1=fix;
	f->s2=oldvar;
	order(f);
	if (f->nullsel && f->s2==f->s1 && f->lastch==f->s1){
		f->nullsel=0;
		matchit(f);
		selectf(f, F_XOR);
	}
	else {
		f->nullsel=f->s2==f->s1;
		f->lastch=f->s1;
	}
}
order(f)
	register Frame *f;
{
	register a;
	if(f->s1 > f->s2){
		a=f->s1;
		f->s1=f->s2;
		f->s2=a;
	}
}
static char ltbrack[]="([{<";		/* sorry, only one match per char */
static char rtbrack[]=")]}>";
static char same[]="\n'`\"";		/* match in either direction */
extern char *index();
matchit(f)
	register Frame *f;
{
	register i,n=0;
	register c;
	register char *s = f->str.s;
	int ch,me;
	if (f->s1>0 && (i=index(ltbrack, me=s[f->s1-1])-ltbrack)>=0) {
		ch = rtbrack[i];
		for (i = f->s1; i<f->str.n && !((c=s[i])==ch && n==0); i++)
			if (c==me)
				n++;
			else if (c==ch)
				n--;
		if (c==ch && n==0)
			f->s2=i;
	}
	else if (f->s2<f->str.n && (i=index(rtbrack, me=s[f->s2])-rtbrack)>=0) {
		ch = ltbrack[i];
		for (i = f->s2-1; i>0 && !((c=s[i])==ch && n==0); i--)
			if (c==me)
				n++;
			else if (c==ch)
				n--;
		if (c==ch && n==0)
			f->s1=i+1;
	}
	else if (f->s1>0 && (i=index(same, me=s[f->s1-1])-same)>=0) {
		for (i = f->s1; i<f->str.n && (c=s[i])!=me; i++)
			;
		if (c==me)
			f->s2=i;
	}
	else if (f->s2<f->str.n && (i=index(same, me=s[f->s2])-same)>=0) {
		for (i = f->s2-1; i>0 && (c=s[i])!=me; i--)
			;
		if (c==me)
			f->s1=i+1;
	}
	else {
		for (i=f->s1; i>0 && isalnum(s[i-1]); i--)
			;
		f->s1=i;
		for (i=f->s2; i<f->str.n && isalnum(s[i]); i++)
			;
		f->s2=i;
	}
}

isalnum(c)
register char c;
{
	return (c>='0' && c<='9') ||
		(c>='a' && c<='z') ||
		(c>='A' && c<='Z') ||
		c=='_';
}
