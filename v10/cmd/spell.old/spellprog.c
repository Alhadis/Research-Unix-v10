/*	@(#)spellprog.c	1.1	*/
#include <stdio.h>
#include <ctype.h>
#include "code.h"
#define Tolower(c) (isupper(c)?(c)-'A'+'a':(c))
#define pair(a,b) (((a)<<8)|(b))
#define DLEV 2

char	*strcat();
int	strip();
int	cstrip();
int	subst();
char	*skipv();
int	an();
int	s(), *s_tst(), *s_apos();
int	es();
int	ily();
int	CCe();
int	VCe();
int	bility();
int	tion();
int	ize();
int	y_to_e();
int	i_to_y();
int	nop();
unsigned hashlook();
int	trysuff(), trypref(), tryword(), dict();

struct suftab {
	char	*suf;
	int	(*p1)();
	int	n1;
	char	*d1;
	char	*a1;
	int	flag;
	int	affixable;
	int	(*p2)();
	int	n2;
	char	*d2;
	char	*a2;
};
struct suftab stabc[] = {
	{"citsi",strip,2,"","+ic",N_AFFIX, ADJ | N_AFFIX | NOUN},
	{"citi",ize,1,"-e+ic","",N_AFFIX, ADJ },
	{"cihparg",i_to_y,1,"-y+ic","",NOUN, ADJ|NOUN },
	{"cipocs",ize,1,"-e+ic","",NOUN, ADJ },
	{"cirtem",i_to_y,1,"-y+ic","",NOUN, ADJ },
	{"cigol",i_to_y,1,"-y+ic","",NOUN, ADJ },
	{"cimono",i_to_y,1,"-y+ic","",NOUN, ADJ },
	0
};
struct suftab stabd[] = {
	{"de",strip,1,"","+d",ED,ADJ | N_AFFIX|COMP,i_to_y,2,"-y+ied","+ed"},
	{"dooh",ily,4,"-y+ihood","+hood",NOUN | ADV, NOUN},
	0
};
struct suftab stabe[] = {
		/*  V_affix for comment ->commence->commentment?? */
	{"ecn",subst,1,"-t+ce","",ADJ,N_AFFIX|_Y|NOUN|VERB|ACTOR|V_AFFIX},
	{"elbaif",i_to_y,4,"-y+iable","",V_IRREG,ADJ},
	{"elba",CCe,4,"-e+able","+able",V_AFFIX,ADJ},
	{"evi",subst,0,"-ion+ive","",N_AFFIX | V_AFFIX,NOUN | N_AFFIX| ADJ},
	{"ezi",CCe,3,"-e+ize","+ize",N_AFFIX|ADJ ,V_AFFIX | VERB |ION | COMP},
	{"ekil",strip,4,"","+like",N_AFFIX ,ADJ},
	{"ekam",strip,4,"","+make",NOUN,V_IRREG|ACTOR},
	0
};
struct suftab stabg[] = {
	{"gni",CCe,3,"-e+ing","+ing",V_IRREG ,ADJ|ED|NOUN},
	0
};
struct suftab stabl[] = {
	{"laci",strip,2,"","+al",NOUN |ADJ,ADJ |NOUN|N_AFFIX},
	{"latnem",strip,2,"","+al",N_AFFIX,ADJ},
	{"lanoi",strip,2,"","+al",N_AFFIX,ADJ|NOUN},
	{"luf",ily,3,"-y+iful","+ful",N_AFFIX,ADJ | NOUN},
	0
};
struct suftab stabm[] = {
		/* congregational + ism */
	{"msi",CCe,3,"-e+ism","ism",N_AFFIX|ADJ,NOUN},
	{"margo",subst,-1,"-ph+m","",NOUN,NOUN},
	0
};
struct suftab stabn[] = {
	{"noitacifi",i_to_y,6,"-y+ication","",ION,NOUN | N_AFFIX},
	{"noitazi",ize,4,"-e+ation","",ION,NOUN| N_AFFIX},
	{"noit",tion,3,"-e+ion","+ion",ION,NOUN| N_AFFIX | V_AFFIX |VERB|ACTOR},
	{"naino",an,3,"","+ian",NOUN|PROP_COLLECT,NOUN| N_AFFIX},
	{"namow",strip,5,"","+woman",MAN,PROP_COLLECT|N_AFFIX},
	{"nam",strip,3,"","+man",MAN,PROP_COLLECT | N_AFFIX | VERB},
	{"na",an,1,"","+n",NOUN|PROP_COLLECT,NOUN | N_AFFIX},
	{"nemow",strip,5,"","+women",MAN,PROP_COLLECT},
	{"nem",strip,3,"","+man",MAN,PROP_COLLECT},
	0
};
struct suftab stabp[] = {
	{"pihs",strip,4,"","+ship",NOUN|PROP_COLLECT,NOUN| N_AFFIX},
	0
};
struct suftab stabr[] = {
	{"rehparg",subst,1,"-y+er","",ACTOR,NOUN,strip,2,"","+er"},
	{"reyhparg",nop,0,"","",0,NOUN},
	{"re",strip,1,"","+r",ACTOR,NOUN | N_AFFIX|VERB|ADJ,	i_to_y,2,"-y+ier","+er"},
	{"rota",tion,2,"-e+or","",ION,NOUN| N_AFFIX|_Y},
	{"rotc",tion,2,"","+or",ION,NOUN| N_AFFIX},
	{"rotp",tion,2,"","+or",ION,NOUN| N_AFFIX},
	0
};
struct suftab stabs[] = {
	{"ssen",ily,4,"-y+iness","+ness",ADJ|ADV,NOUN| N_AFFIX},
	{"ssel",ily,4,"-y+i+less","+less",NOUN | PROP_COLLECT,ADJ },
	{"se",s,1,"","+s",NOUN | V_IRREG,DONT_TOUCH ,	es,2,"-y+ies","+es"},
	{"s'",s,2,"","+'s",PROP_COLLECT | NOUN,DONT_TOUCH },
	{"s",s,1,"","+s",NOUN | V_IRREG,DONT_TOUCH  },
	0
};
struct suftab stabt[] = {
	{"tnem",strip,4,"","+ment",V_AFFIX,NOUN | N_AFFIX | ADJ|VERB},
	{"tse",strip,2,"","+st",EST,DONT_TOUCH,	i_to_y,3,"-y+iest","+est" },
	{"tsigol",i_to_y,2,"-y+ist","",N_AFFIX,NOUN | N_AFFIX},
	{"tsi",CCe,3,"-e+ist","+ist",N_AFFIX|ADJ,NOUN | N_AFFIX|COMP},
	0
};
struct suftab staby[] = {
	{"ytilb",nop,0,"","",0,NOUN},
	{"ycn",subst,1,"-t+cy","",ADJ | N_AFFIX,NOUN | N_AFFIX},
	{"ytilib",bility,5,"-le+ility","",ADJ | V_AFFIX,NOUN | N_AFFIX},
	{"ytisuo",nop,0,"","",NOUN},
	{"yti",CCe,3,"-e+ity","+ity",ADJ ,NOUN | N_AFFIX },
	{"ylb",y_to_e,1,"-e+y","",ADJ,ADV},
	{"ylc",nop,0,"","",0},
	{"yl",ily,2,"-y+ily","+ly",ADJ,ADV|COMP},
	{"yrtem",subst,0,"-er+ry","",NOUN,NOUN | N_AFFIX},
	{"y",CCe,1,"-e+y","+y",_Y,ADJ|COMP},
	0
};
struct suftab stabz[] = {
	0
};
struct suftab *suftab[] = {
	stabz,
	stabz,
	stabc,
	stabd,
	stabe,
	stabz,
	stabg,
	stabz,
	stabz,
	stabz,
	stabz,
	stabl,
	stabm,
	stabn,
	stabz,
	stabp,
	stabz,
	stabr,
	stabs,
	stabt,
	stabz,
	stabz,
	stabz,
	stabz,
	staby,
	stabz,
};


struct ptab {
	char *s;
	int flag;
};
struct ptab ptaba[] = {
	"anti", 0,
	"auto", 0,
	0
};
struct ptab ptabb[] = {
	"bio", 0,
	0
};
struct ptab ptabc[] = {
	"counter", 0,
	0
};
struct ptab ptabd[] = {
	"dis", 0,
	0
};
struct ptab ptabe[] = {
	"electro", 0,
	"en", 0,
	0
};
struct ptab ptabf[] = {
	"femto", 0,
	0
};
struct ptab ptabg[] = {
	"geo", 0,
	"giga", 0,
	0
};
struct ptab ptabh[] = {
	"hyper", 0,
	0
};
struct ptab ptabi[] = {
	"im", IN,
	"intra", 0,
	"inter", 0,
	"in", IN,
	"ir", IN,
	"iso", 0,
	0
};
struct ptab ptabj[] = {
	0
};
struct ptab ptabk[] = {
	"kilo", 0,
	0
};
struct ptab ptabl[] = {
	0
};
struct ptab ptabm[] = {
	"magneto", 0,
	"mega", 0,
	"meta", 0,
	"micro", 0,
	"mid", 0,
	"milli", 0,
	"mis", 0,
	"mono", 0,
	"multi", 0,
	0
};
struct ptab ptabn[] = {
	"nano", 0,
	"non", 0,
	0
};
struct ptab ptabo[] = {
	"out", 0,
	"over", 0,
	0
};
struct ptab ptabp[] = {
	"para", 0,
	"photo", 0,
	"pico", 0,
	"poly", 0,
	"pre", 0,
	"pseudo", 0,
	"psycho", 0,
	0
};
struct ptab ptabq[] = {
	"quasi", 0,
	0
};
struct ptab ptabr[] = {
	"re", 0,
	0
};
struct ptab ptabs[] = {
	"semi", 0,
	"stereo", 0,
	"sub", 0,
	"super", 0,
	0
};
struct ptab ptabt[] = {
	"tele", 0,
	"thermo", 0,
	0
};
struct ptab ptabu[] = {
	"ultra", 0,
	"under", 0,	/*must precede un*/
	"un", IN,
	0
};
struct ptab ptabv[] = {
	0
};
struct ptab ptabw[] = {
	0
};
struct ptab ptabx[] = {
	0
};
struct ptab ptaby[] = {
	0
};
struct ptab ptabz[] = {
	0
};

struct ptab *preftab[] = {
	ptaba,
	ptabb,
	ptabc,
	ptabd,
	ptabe,
	ptabf,
	ptabg,
	ptabh,
	ptabi,
	ptabj,
	ptabk,
	ptabl,
	ptabm,
	ptabn,
	ptabo,
	ptabp,
	ptabq,
	ptabr,
	ptabs,
	ptabt,
	ptabu,
	ptabv,
	ptabw,
	ptabx,
	ptaby,
	ptabz,
};

char voweltab[] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,
	0,1,0,0,0,0,0,1,
	0,0,0,0,0,1,0,0,
	0,1,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,
	0,1,0,0,0,0,0,1,
	0,0,0,0,0,1,0,0,
	0,1,0,0,0,0,0,0
};
#define isvowel(c) voweltab[c]

int cflag;
int vflag;
int xflag;
char word[100];
char original[100];
char *deriv[40];
char affix[40];
char errmsg[] = "spell: cannot initialize hash table\n";
FILE *found;
/*	deriv is stack of pointers to notes like +micro +ed
 *	affix is concatenated string of notes
 *	the buffer size 141 stems from the sizes of original and affix.
*/

/*
 *	in an attempt to defray future maintenance misunderstandings, here is an attempt to
 *	describe the input/output expectations of the spell program.
 *
 *	spellprog is intended to be called from the shell file spell.
 *	because of this, there is little error checking (this is historical, not
 *	necessarily advisable).
 *
 *	spellprog hashed-list filename options
 *
 *	the hashed-list is a list of the form made by spellin.
 *	It is treated as the regular dictionary list,with an
 *	"affix" code for each entry.  THis code determines the
 *	validity of adding a prefix or suffix.
 *	 Found words are written to  filename, in
 *	the normal case the filename = /dev/null. However, if the v option is
 *	specified, the derivations are written to this file.
 *	the spellprog looks up words in the hashed-list; if a word is found, it is
 *	printed to the stdout. if the hashed-list code was "STOP", the words found
 *	are presumed to be misspellings. Correct words 
 *	may be ditched. (unless the -v option was used - see the manual page).

 *
 *	spellprog hlista /dev/null 
 *
 *	for a complete scenario, see the shell file: spell.
 *
 *
  */

main(argc,argv)
char **argv;
{
	register char *ep, *cp;
	register char *dp;
	register j, h;
	int low = 0;

	if(!prime(argc,argv)) {
		fwrite(errmsg, sizeof(*errmsg), sizeof(errmsg), stderr);
		exit(1);
	}
	found = fopen(argv[2],"w");
	for(argc-=3,argv+=3; argc>0 && argv[0][0]=='-'; argc--,argv++)
		switch(argv[0][1]) {
		case 'c':	/* + or - for each word (use: OCR) */
			cflag++;
			setbuf(stdout, (char*)0); /* so pipes work */
			break;			  /*word-by-word */
		case 'b':	/* british */
			ise();
			break;
		case 'v':	/* verbose - print derivations */
			vflag++;
			break;
		case 'x':	/* print every lookup try */
			xflag++;
			break;
		}
	for(;;) {
		affix[0] = 0;
		dp = original;
		low = 0;
		for(ep=word;(*dp= *ep=j=getchar())!='\n';ep++,dp++){
			if(j == EOF)
				exit(0);
			if(islower(j))
				low++;
			if(ep >= word+sizeof(word)-1)
				break;
		}
		if(isdigit(word[0]) && ordinal())
			continue;
		*dp = 0;

		h = 0;
		if(!low && !(h = trypref(ep,".",0,ALL|STOP|DONT_TOUCH)))
			for(cp=original+1,dp=word+1;dp<ep;dp++,cp++)
				*dp = Tolower(*cp);
		if(!h) for(;;) {	/* at most twice */
			if(h = trypref(ep,".",0,ALL|STOP|DONT_TOUCH))
				break;
			if(h = trysuff(ep,0,ALL|STOP|DONT_TOUCH))
				break;
			if(!isupper(word[0]))
				break;
			cp=original,dp=word; 
			while(*dp = *cp++) { 
					if(!low)*dp = Tolower(*dp);
				dp++;
			}
			word[0] = Tolower(word[0]);
		}

		if(cflag) {
			putchar(!h||(h&STOP)? '-': '+');
			continue;
		}
		if(!h || (h&STOP))
			printf("%s\n", original);
		else if(affix[0]!=0 && affix[0]!='.')
			fprintf(found, "%s\t%s\n", affix, original);
	}
}

/*	strip exactly one suffix and do
 *	indicated routine(s), which may recursively
 *	strip suffixes
*/
int 
trysuff(ep,lev,flag)
char *ep;
{
	register struct suftab *t;
	register char *cp, *sp;
	register unsigned h = 0;
	int initchar = ep[-1];

	flag &= ~MONO;
	lev += DLEV;
	deriv[lev] = deriv[lev-1] = 0;
	if(!islower(initchar))
		return h;
	for(t=suftab[initchar-'a'];sp=t->suf;t++) {
		cp = ep;
		while(*sp)
			if(*--cp!=*sp++)
				goto next;
		for(sp=cp; --sp>=word&&!isvowel(*sp); ) ;
		if(sp<word)
			return 0;
		if(!(t->affixable&flag))
			return 0;
		h = (*t->p1)(ep-t->n1,t->d1,t->a1,lev+1,t->flag|STOP);
		if(!h && t->p2!=0) {
			deriv[lev] = deriv[lev+1] = 0;
			h = (*t->p2)(ep-t->n2,t->d2,t->a2,lev,t->flag|STOP);
		}
		break;
next:		;
	}
	return h;
}
/*ARGSUSED*/
int 
nop(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	return 0;
}
int 
cstrip(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register temp = ep[0];
	if(isvowel(temp)&&isvowel(ep[-1])) {
		switch(pair(ep[-1],ep[0])) {
		case pair('a', 'a'):
		case pair('a', 'e'):
		case pair('a', 'i'):
		case pair('e', 'a'):
		case pair('e', 'e'):
		case pair('e', 'i'):
		case pair('i', 'i'):
		case pair('o', 'a'):
			return 0;
		}
	} else if(temp==ep[-1]&&temp==ep[-2])
		return 0;
	return strip(ep,d,a,lev,flag);
}
/*ARGSUSED*/
int 
strip(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register h = trypref(ep,a,lev,flag);
	if((h&MONO) && isvowel(*ep) && isvowel(ep[-2]))
		h = 0;
	if(h) return h;
	if(isvowel(*ep) && ep[-1]==ep[-2]) {
		h = trypref(ep-1,a,lev,flag|MONO);
		if(h) return h;
	}
	return trysuff(ep,lev,flag);
}
int 
s(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	if(lev>DLEV+1)
		return 0;
	if(*ep=='s') {
		switch(ep[-1]) {
		case 'y':
			if(isvowel(ep[-2])||isupper(*word))
				break;	/*says Kennedys*/
		case 'x':
		case 'z':
		case 's':
			return 0;
		case 'h':
			switch(ep[-2]) {
			case 'c':
			case 's':
				return 0;
			}
		}
	}
	return strip(ep,d,a,lev,flag);
}
/*ARGSUSED*/
int 
an(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	if(!isupper(*word))	/*must be proper name*/
		return 0;
	return trypref(ep,a,lev,flag);
}
/*ARGSUSED*/
int 
ize(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register temp = ep[-1];
	register h;
	ep[-1] = 'e';
	h = strip(ep,"",d,lev,flag);
	ep[-1] = temp;
	return h;
}
/*ARGSUSED*/
int 
y_to_e(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register h;
	register  temp;
	switch(ep[-1]) {
	case 'a':
	case 'e':
	case 'i':
		return 0;
	}
	temp = *ep;
	*ep++ = 'e';
	h = strip(ep,"",d,lev,flag);
	*--ep = temp;
	return h;
}
int 
ily(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register temp = ep[0];
	char *cp = ep;
	if(temp==ep[-1]&&temp==ep[-2])		/* sillly */
		return 0;
	if(*--cp=='y' && !isvowel(*--cp))		/* happyly */
		while(cp>word)
			if(isvowel(*--cp))	/* shyness */
				return 0;
	if(ep[-1]=='i')
		return i_to_y(ep,d,a,lev,flag);
	else
		return cstrip(ep,d,a,lev,flag);
}
int 
bility(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	*ep++ = 'l';
	return y_to_e(ep,d,a,lev,flag);
}
int 
i_to_y(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register h;
	register temp;
	if(isupper(*word))
		return 0;
	if((temp=ep[-1])=='i'&&!isvowel(ep[-2])) {
		ep[-1] = 'y';
		a = d;
	}
	h = cstrip(ep,"",a,lev,flag);
	ep[-1] = temp;
	return h;
}

int 
es(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	if(lev>DLEV)
		return 0;
	switch(ep[-1]) {
	default:
		return 0;
	case 'i':
		return i_to_y(ep,d,a,lev,flag);
	case 'h':
		switch(ep[-2]) {
		default:
			return 0;
		case 'c':
		case 's':
			break;
		}
	case 's':
	case 'z':
	case 'x':
		return strip(ep,d,a,lev,flag);
	}
}
/*ARGSUSED*/
int 
subst(ep,d,a,lev,flag)
char *ep, *d, *a;
{
	register char *u,*t;
	register h;
	if(skipv(skipv(ep-1))<word)
		return 0;
	for(t=d;*t!='+';t++)
		continue;
	for(u=ep;*--t!='-'; )
		*--u = *t;
	h = strip(ep,"",d,lev,flag);
	while(*++t!='+')
		continue;
	while(*++t)
		*u++ = *t;
	return h;
}

int 
tion(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	switch(ep[-2]) {
	case 's':
	case 'c':
	case 'r':
	case 'p':
		return trypref(ep,a,lev,flag);
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return y_to_e(ep,d,a,lev,flag);
	}
	return 0;
}

/*	possible consonant-consonant-e ending*/
int 
CCe(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register h;
	switch(ep[-1]) {
	case 'l':
		if(isvowel(ep[-2]))
			break;
		switch(ep[-2]) {
		case 'l':
		case 'r':
		case 'w':
			break;
		default:
			return y_to_e(ep,d,a,lev,flag);
		}
		break;
	case 'c':
	case 'g':
		if(*ep=='a')	/* prevent -able for -eable */
			return 0;
	case 's':
	case 'v':
	case 'z':
		if(ep[-2]==ep[-1])
			break;
		if(isvowel(ep[-2]))
			break;
	case 'u':
		if(h = y_to_e(ep,d,a,lev,flag))
			return h;
		if(!(ep[-2]=='n'&&ep[-1]=='g'))
			return 0;
	}
	return VCe(ep,d,a,lev,flag);
}

/*	possible consonant-vowel-consonant-e ending*/
int 
VCe(ep,d,a,lev,flag)
char *ep,*d,*a;
{
	register c, h;

	c = ep[-1];
	if(c=='e')
		return 0;
	if(!isvowel(c) && isvowel(ep[-2])) {
		c = *ep;
		*ep++ = 'e';
		h = (h = trypref(ep,d,lev,flag))?h:trysuff(ep,lev,flag);
		if(h)return(h);
		ep--;
		*ep = c;
	}
	return cstrip(ep,d,a,lev,flag);
}
struct ptab *lookuppref(wp,ep)
char **wp;
char *ep;
{
	register struct ptab *sp;
	register char *bp,*cp;
	int initchar = Tolower(**wp);
	if(!isalpha(initchar))
		return 0;
	for(sp=preftab[initchar-'a'];sp->s;sp++) {
		bp = *wp;
		for(cp= sp->s;*cp; )
			if(*bp++!=*cp++)
				goto next;
		for(cp=bp;cp<ep;cp++) 
			if(isvowel(*cp)) {
				*wp = bp;
				return sp;
			}
next:	;
	}
	return 0;
}

/*	while word is not in dictionary try stripping
 *	prefixes. Fail if no more prefixes.
*/
int 
trypref(ep,a,lev,flag)
char *ep,*a;
{
	register struct ptab *tp;
	char *bp, *cp;
	register char *pp;
	int h;
	char space[20];
	deriv[lev] = a;
	if(h = tryword(word,ep,lev,flag))
		if((h&flag&~MONO) && (flag&MONO)<=(h&MONO))
			return h;
		else
			h = 0;
	bp = word;
	pp = space;
	deriv[lev+1] = pp;
	while(tp=lookuppref(&bp,ep)) {
		*pp++ = '+';
		cp = tp->s;
		while(*pp = *cp++)
			pp++;
		h = tryword(bp,ep,lev+1,flag);
		if((h&NOPREF) ||
		   ((tp->flag&IN) && inun(bp-2,h)==0)) {
			h = 0;
			break;
		}	
		if((h&flag&~MONO) && (flag&MONO)<=(h&MONO))
			break;
		h = 0;
	}
	deriv[lev+1] = deriv[lev+2] = 0;
	return h;
}
int
tryword(bp,ep,lev,flag)
char *bp,*ep;
{
	register  j;
	register h = 0 ;
	char duple[3];
	if(ep-bp<=1)
		return h;
	if(flag&MONO) {
		deriv[++lev] = duple;
		duple[0] = '+';
		duple[1] = *ep;
		duple[2] = 0;
	}
	h = dict(bp,ep);
	if(vflag==0||h==0 )
		return h;
	/*	when derivations are wanted, collect them
	 *	for printing
	*/
	j = lev;
	do {
		if(deriv[j])
			strcat(affix,deriv[j]);
	} while(--j>0);
	return h;
}
inun(bp,h)
char *bp;
{
	if(*bp=='u')
		return (h&IN) == 0;
	/* *bp == 'i' */
	if((h&IN)==0)
		return 0;
	switch(bp[2]) {
	case 'r':
		return bp[1] == 'r';
	case 'm':
	case 'p':
		return bp[1] == 'm';
	}
	return 1;
}
char *
skipv(s)
char *s;
{
	if(s>=word&&isvowel(*s))
		s--;
	while(s>=word&&!isvowel(*s))
		s--;
	return s;
}

/* crummy way to Britishise */
ise()
{
	register struct suftab *p;
	register i;
	for(i=0; i<26; i++) {
		for(p = suftab[i];p->suf;p++) {
			ztos(p->suf);
			ztos(p->d1);
			ztos(p->a1);
		}
	}
}
ztos(s)
char *s;
{
	for(;*s;s++)
		if(*s=='z')
			*s = 's';
}
int 
dict(bp,ep)
char *bp, *ep;
{
	register temp, h;
	temp = *ep;
	*ep = 0;
	h = hashlook(bp);
	if(xflag) {
		fprintf(stderr, "=%s ", bp);
		typeprint(stderr,h);
		fprintf(stderr, "\n");
	}
	*ep = temp;
	return h;
}

ordinal()
{
	char *cp = word;
	static char sp[4];
	while(isdigit(*cp))
		cp++;
	sp[0] = Tolower(cp[0]);
	sp[1] = Tolower(cp[1]);
	sp[2] = cp[2]=='\n'? 0: cp[2];
	return strncmp(sp,
		cp[-2]=='1'? "th":
		*--cp=='1'? "st":
		*cp=='2'? "nd":
		*cp=='3'? "rd":
		"th", 3) == 0;
}
