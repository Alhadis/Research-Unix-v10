/* a4 -- pdp-11 assembler pass1 */
#include "as1.h"
extern char end[];
char *brkend = end;
extern struct symtab *hshtab[HSHSIZ];
extern struct symtab symtab[];
extern int noputw;

struct expr *
rname(t)
{
	char *sbrk();
	register int i;
	register char *cp1,*cp2;
	struct symtab *sp,**hp;
	int around;
	int h=0,flags=0;

	for (cp1=symbol+NCPS,i=NCPS; --i>=0; ) *--cp1=0;
	i=NCPS;
	if (t=='~') {ch=0; flags=1;}	/* symbol not for hash table */
	for (;;) {
		if (chartab[t=rch()]<=0) break;
		h+=t; h=((h&0377)<<8)|((h>>8)&0377);
		if (--i>=0) *cp1++=t;
	}
	ch=t;
	if (flags) {sp=symend++; goto f4;}
	hp=hshtab+((long)(unsigned short)h)%HSHSIZ;
	h=((long)(unsigned short)h)/HSHSIZ;
	around=0;
	for (;;) {
		if ((hp-=h)<=hshtab) {
			hp+=HSHSIZ;
			if (around) {fprintf(stderr,"as: symbol table overflow\n"); aexit();}
			around++;
		}
		cp1=symbol;
		if ((sp= *--hp)!=0) {
			cp2=sp->name;
			if (*cp1++!=*cp2++ || *cp1++!=*cp2++ || *cp1++!=*cp2++ || *cp1++!=*cp2++ ||
			    *cp1++!=*cp2++ || *cp1++!=*cp2++ || *cp1++!=*cp2++ || *cp1++!=*cp2++)
				continue;
			break;
		}
		*hp=sp=symend++;
	f4:
		cp2=sp->name; cp1=symbol;
		if (cp2>=brkend) {
			register char *s;
			if (-1==(int)(s=sbrk(512))) {error('M'); aexit();}
			brkend=s+512-sizeof(*sp);
		}
		for (i=NCPS; --i>=0; ) *cp2++= *cp1++;
		sp->sval.typ=0; sp->sval.val=0;
		break;
	}
	if (sp>=usymtab)
		putw(USYM+(sp-usymtab));
	else
		putw(BSYM+(sp-symtab));
	noputw=1;
	return(&sp->sval);
}

int fbflag;

number()
{
	register int t;
	register short dec=0,oct=0;

	fbflag=0;
	for (;;) {
		t=rch();
		if (t<'0' || t>'9') break;
		dec=(dec*10)+t-'0'; oct=(oct<<3)+t-'0';
	}
	if (t=='b' || t=='f') {
		register int i;
		fbflag=1;
		i=fbcheck(dec)+'a';
		if (t=='f') i+=10;
		return(i);
	}
	if (t=='.')
		{ch=0; return(dec);}
	else
		{ch=t; return(oct);}
}

rch()
{
	register int t;
	register char *cp;

	for (;;) {
		if (t=ch) {ch=0; return(t);}
		if ((t=getchar())!=EOF) return(t);
		fclose(stdin);
		if (--nargs<=0) return('\004');
		if (ifflg) {error('i'); aexit();}
		if (NULL==freopen(*++curarg,"r",stdin)) {filerr(*curarg,"?\n"); aexit();}
		line=1;
		putw(5); cp= *curarg; while (t= *cp++) putw(t); putw(-1);
	}
}
