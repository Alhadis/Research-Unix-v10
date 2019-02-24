
#include "code.h"

#ifndef _POSIX_SOURCE

#include <stdio.h>
#include <ctype.h>
#include <libc.h>

#else

#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#endif

#define isvowel(c)	voweltab[c]
#define Tolower(c)	(isupper(c)? (c)-'A'+'a': (c))
#define pair(a,b)	(((a)<<8) | (b))
#define DLEV		2
#define DSIZ		40

typedef	long	Bits;
typedef unsigned char uchar;
#define	Set(h, f)	((long)(h) & (f))

Bits 	nop(char*, char*, char*, int, int);
Bits 	strip(char*, char*, char*, int, int);
Bits 	ize(char*, char*, char*, int, int);
Bits 	i_to_y(char*, char*, char*, int, int);
Bits 	ily(char*, char*, char*, int, int);
Bits 	subst(char*, char*, char*, int, int);
Bits 	CCe(char*, char*, char*, int, int);
Bits 	tion(char*, char*, char*, int, int);
Bits 	an(char*, char*, char*, int, int);
Bits 	s(char*, char*, char*, int, int);
Bits 	es(char*, char*, char*, int, int);
Bits 	bility(char*, char*, char*, int, int);
Bits 	y_to_e(char*, char*, char*, int, int);
Bits 	VCe(char*, char*, char*, int, int);

Bits 	trypref(char*, char*, int, int);
Bits	tryword(char*, char*, int, int);
Bits 	trysuff(char*, int, int);
Bits	dict(char*, char*);
void	typeprint(Bits);
void	pcomma(char*);

void	runout(char*);
void	ise(void);
int	ordinal(void);
char*	skipv(char*);
int	inun(char*, Bits);
char*	ztos(char*);
void	readdict(char*);

char	*strdupl(char *);

typedef	struct	Ptab	Ptab;
struct	Ptab
{
	char*	s;
	int	flag;
};

typedef	struct	Suftab	Suftab;
struct	Suftab
{
	char	*suf;
	Bits	(*p1)(char*, char*, char*, int, int);
	int	n1;
	char	*d1;
	char	*a1;
	int	flag;
	int	affixable;
	Bits	(*p2)(char*, char*, char*, int, int);
	int	n2;
	char	*d2;
	char	*a2;
};

Suftab	staba[] = {
	{"aibohp",subst,1,"-e+ia","",NOUN, NOUN},
	0
};

Suftab	stabc[] =
{
	{"cai",strip,1,"","+c",N_AFFIX, ADJ|NOUN},
	{"citsi",strip,2,"","+ic",N_AFFIX, ADJ | N_AFFIX | NOUN},
	{"citi",ize,1,"-e+ic","",N_AFFIX, ADJ },
	{"cihparg",i_to_y,1,"-y+ic","",NOUN, ADJ|NOUN },
	{"cipocs",ize,1,"-e+ic","",NOUN, ADJ },
	{"cirtem",i_to_y,1,"-y+ic","",NOUN, ADJ },
	{"cigol",i_to_y,1,"-y+ic","",NOUN, ADJ },
	{"cimono",i_to_y,1,"-y+ic","",NOUN, ADJ },
	{"cibohp",subst,1,"-e+ic","",NOUN, ADJ },
	0
};
Suftab	stabd[] =
{
	{"de",strip,1,"","+d",ED,ADJ |COMP,i_to_y,2,"-y+ied","+ed"},
	{"dooh",ily,4,"-y+ihood","+hood",NOUN | ADV, NOUN},
	0
};
Suftab	stabe[] =
{
	/*
	 * V_affix for comment ->commence->commentment??
	 */
	{"ecn",subst,1,"-t+ce","",ADJ,N_AFFIX|_Y|NOUN|VERB|ACTOR|V_AFFIX},
	{"elbaif",i_to_y,4,"-y+iable","",V_IRREG,ADJ},
	{"elba",CCe,4,"-e+able","+able",V_AFFIX,ADJ},
	{"evi",subst,0,"-ion+ive","",N_AFFIX | V_AFFIX,NOUN | N_AFFIX| ADJ},
	{"ezi",CCe,3,"-e+ize","+ize",N_AFFIX|ADJ ,V_AFFIX | VERB |ION | COMP},
	{"ekil",strip,4,"","+like",N_AFFIX ,ADJ},
	0
};
Suftab	stabg[] =
{
	{"gniee",strip,3,"","+ing",V_IRREG ,ADJ|NOUN},
	{"gnikam",strip,6,"","+making",NOUN,NOUN},
	{"gnipeek",strip,7,"","+keeping",NOUN,NOUN},
	{"gni",CCe,3,"-e+ing","+ing",V_IRREG ,ADJ|ED|NOUN},
	0
};
Suftab	stabl[] =
{
	{"ladio",strip,2,"","+al",NOUN |ADJ,ADJ},
	{"laci",strip,2,"","+al",NOUN |ADJ,ADJ |NOUN|N_AFFIX},
	{"latnem",strip,2,"","+al",N_AFFIX,ADJ},
	{"lanoi",strip,2,"","+al",N_AFFIX,ADJ|NOUN},
	{"luf",ily,3,"-y+iful","+ful",N_AFFIX,ADJ | NOUN},
	0
};
Suftab	stabm[] =
{
		/* congregational + ism */
	{"msi",CCe,3,"-e+ism","ism",N_AFFIX|ADJ,NOUN},
	{"margo",subst,-1,"-ph+m","",NOUN,NOUN},
	0
};
Suftab	stabn[] =
{
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
Suftab	stabp[] =
{
	{"pihs",strip,4,"","+ship",NOUN|PROP_COLLECT,NOUN| N_AFFIX},
	0
};
Suftab	stabr[] =
{
	{"rehparg",subst,1,"-y+er","",ACTOR,NOUN,strip,2,"","+er"},
	{"reyhparg",nop,0,"","",0,NOUN},
	{"rekam",strip,5,"","+maker",NOUN,NOUN},
	{"repeek",strip,6,"","+keeper",NOUN,NOUN},
	{"re",strip,1,"","+r",ACTOR,NOUN | N_AFFIX|VERB|ADJ,	i_to_y,2,"-y+ier","+er"},
	{"rota",tion,2,"-e+or","",ION,NOUN| N_AFFIX|_Y},
	{"rotc",tion,2,"","+or",ION,NOUN| N_AFFIX},
	{"rotp",tion,2,"","+or",ION,NOUN| N_AFFIX},
	0
};
Suftab	stabs[] =
{
	{"ssen",ily,4,"-y+iness","+ness",ADJ|ADV,NOUN| N_AFFIX},
	{"ssel",ily,4,"-y+iless","+less",NOUN | PROP_COLLECT,ADJ },
	{"se",s,1,"","+s",NOUN | V_IRREG,DONT_TOUCH ,	es,2,"-y+ies","+es"},
	{"s'",s,2,"","+'s",PROP_COLLECT | NOUN,DONT_TOUCH },
	{"s",s,1,"","+s",NOUN | V_IRREG,DONT_TOUCH  },
	0
};
Suftab	stabt[] =
{
	{"tnem",strip,4,"","+ment",V_AFFIX,NOUN | N_AFFIX | ADJ|VERB},
	{"tse",strip,2,"","+st",EST,DONT_TOUCH,	i_to_y,3,"-y+iest","+est" },
	{"tsigol",i_to_y,2,"-y+ist","",N_AFFIX,NOUN | N_AFFIX},
	{"tsi",CCe,3,"-e+ist","+ist",N_AFFIX|ADJ,NOUN | N_AFFIX|COMP},
	0
};
Suftab	staby[] =
{
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
Suftab	stabz[] =
{
	0
};
Suftab*	suftab[] =
{
	staba,
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

Ptab	ptaba[] =
{
	"anti", 0,
	"auto", 0,
	0
};
Ptab	ptabb[] =
{
	"bio", 0,
	0
};
Ptab	ptabc[] =
{
	"counter", 0,
	0
};
Ptab	ptabd[] =
{
	"dis", 0,
	0
};
Ptab	ptabe[] =
{
	"electro", 0,
	0
};
Ptab	ptabf[] =
{
	"femto", 0,
	0
};
Ptab	ptabg[] =
{
	"geo", 0,
	"giga", 0,
	0
};
Ptab	ptabh[] =
{
	"hyper", 0,
	0
};
Ptab	ptabi[] =
{
	"immuno", 0,
	"im", IN,
	"intra", 0,
	"inter", 0,
	"in", IN,
	"ir", IN,
	"iso", 0,
	0
};
Ptab	ptabj[] =
{
	0
};
Ptab	ptabk[] =
{
	"kilo", 0,
	0
};
Ptab	ptabl[] =
{
	0
};
Ptab	ptabm[] =
{
	"magneto", 0,
	"mega", 0,
	"meta", 0,
	"micro", 0,
	"mid", 0,
	"milli", 0,
	"mini", 0,
	"mis", 0,
	"mono", 0,
	"multi", 0,
	0
};
Ptab	ptabn[] =
{
	"nano", 0,
	"neuro", 0,
	"non", 0,
	0
};
Ptab	ptabo[] =
{
	"out", 0,
	"over", 0,
	0
};
Ptab	ptabp[] =
{
	"para", 0,
	"photo", 0,
	"pico", 0,
	"poly", 0,
	"pre", 0,
	"pseudo", 0,
	"psycho", 0,
	0
};
Ptab	ptabq[] =
{
	"quasi", 0,
	0
};
Ptab	ptabr[] =
{
	"radio", 0,
	"re", 0,
	0
};
Ptab	ptabs[] =
{
	"semi", 0,
	"stereo", 0,
	"sub", 0,
	"super", 0,
	0
};
Ptab	ptabt[] =
{
	"tele", 0,
	"tera", 0,
	"thermo", 0,
	0
};
Ptab	ptabu[] =
{
	"ultra", 0,
	"under", 0,	/*must precede un*/
	"un", IN,
	0
};
Ptab	ptabv[] =
{
	0
};
Ptab	ptabw[] =
{
	0
};
Ptab	ptabx[] =
{
	0
};
Ptab	ptaby[] =
{
	0
};
Ptab	ptabz[] =
{
	0
};

Ptab*	preftab[] =
{
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

typedef struct {
	char *mesg;
	enum { NONE, SUFF, PREF} type;
} Deriv;

int	cflag;
int	fflag;
int	vflag;
int	xflag;
char	word[500];
char	original[500];
Deriv	deriv[DSIZ+3];
Deriv	emptyderiv;
char	affix[DSIZ*10];	/* 10 is longest affix message */
int	prefcount;
int 	suffcount;
char	space[300000];	/* must be as large as "words"+"space" in pcode run */
Bits	encode[2048];	/* must be as long as "codes" in pcode run */
int	nencode;
char	voweltab[128];
char*	spacep[128*128+1];	/* pointer to words starting with 'xx' */

char*	codefile = "/usr/lib/spell/amspell";
char*	brfile = "/usr/lib/spell/brspell";

main(int argc, char *argv[])
{
	char *ep, *cp;
	char *dp;
	int j, i, c;
	int low = 0;
	Bits h;

	for(i=0; c = "aeiouyAEIOUY"[i]; i++)
		voweltab[c] = 1;
	while(argc > 1) {
		if(argv[1][0] != '-')
			break;
		for(i=1; c = argv[1][i]; i++)
		switch(c) {
		default:
			fprintf(stderr, "usage: spell [-bcvx] [-f file]\n");
			exit(1);

		case 'b':
			ise();
			if(!fflag)
				codefile = brfile;
			continue;

		case 'C':
			vflag++;
		case 'c':
			setbuf(stdout,0);
			cflag++;
			continue;

		case 'v':
			vflag++;
			continue;

		case 'x':
			xflag++;
			continue;

		case 'f':
			if(argc <= 2) {
				fprintf(stderr, "spell: -f requires another argument\n");
				exit(1);
			}
			argv++;
			argc--;
			fflag++;
			codefile = argv[1];
			goto brk;
		}
	brk:
		argv++;
		argc--;
	}
	readdict(codefile);
	if(argc > 1) {
		fprintf(stderr, "usage: spell [-bcvx] [-f file]\n");
		exit(1);
	}
	for(;;) {
	loop:
		affix[0] = 0;
		for(ep=original; ; ep++) {
			if(ep >= original + sizeof(original) - 1) {
				*ep = 0;
				runout(original);
				goto loop;
			}
			j = getchar();
			if(j == EOF)
				exit(0);
			if(j != '\n')
				*ep = j;
			else {
				*ep = 0;
				break;
			}
		}
		low = 0;
		for(ep=word,dp=original; j = *dp; ep++,dp++) {
			if(islower(j))
				low++;
			if(ep >= word+sizeof(word)-1)
				break;
			*ep = j;
		}
		*ep = 0;

		h = ~STOP;
		if(word[1] == 0 && isalnum(word[0]) ||
		   isdigit(word[0]) && ordinal())
			goto check;

		h = 0;
		if(!low && !(h = trypref(ep,".",0,ALL|STOP|DONT_TOUCH)))
			for(cp=original+1,dp=word+1; dp<ep; dp++,cp++)
				*dp = Tolower(*cp);
		if(!h)
		for(;;) {	/* at most twice */
			if(h = trypref(ep,".",0,ALL|STOP|DONT_TOUCH))
				break;
			if(h = trysuff(ep,0,ALL|STOP|DONT_TOUCH))
				break;
			if(!isupper(word[0]))
				break;
			cp = original;
			dp = word;
			while(*dp = *cp++) {
					if(!low)
						*dp = Tolower(*dp);
				dp++;
			}
			word[0] = Tolower(word[0]);
		}
	check:
		if(cflag) {
			if(!h || Set(h,STOP))
				putchar('-');
			else if(!vflag)
				putchar('+');
			else 
				putchar('0' + (suffcount>0) +
				   (prefcount>4? 8: 2*prefcount));
		} else if(!h || Set(h,STOP))
			printf("%s\n", original);
		else if(affix[0] != 0 && affix[0] != '.')
			printf("%s\t%s\n", affix, original);
	}
}

/*	strip exactly one suffix and do
 *	indicated routine(s), which may recursively
 *	strip suffixes
 */
Bits
trysuff(char* ep, int lev, int flag)
{
	Suftab *t;
	char *cp, *sp;
	Bits h = 0;
	int initchar = ep[-1];

	flag &= ~MONO;
	lev += DLEV;
	if(lev<DSIZ)
		deriv[lev] = deriv[lev-1] = emptyderiv;
	if(!islower(initchar))
		return h;
	for(t=suftab[initchar-'a']; sp=t->suf; t++) {
		cp = ep;
		while(*sp)
			if(*--cp != *sp++)
				goto next;
		for(sp=ep-t->n1; --sp >= word && !isvowel(*sp);)
			;
		if(sp < word)
			continue;
		if(!(t->affixable & flag))
			return 0;
		h = (*t->p1)(ep-t->n1, t->d1, t->a1, lev+1, t->flag|STOP);
		if(!h && t->p2!=0) {
			if(lev<DSIZ)
				deriv[lev] = deriv[lev+1] = emptyderiv;
			h = (*t->p2)(ep-t->n2, t->d2, t->a2, lev, t->flag|STOP);
		}
		break;
	next:;
	}
	return h;
}

Bits
nop(char* ep, char* d, char* a, int lev, int flag)
{
#pragma ref ep
#pragma ref d
#pragma ref a
#pragma ref lev
#pragma ref flag
	return 0;
}

Bits
cstrip(char* ep, char* d, char* a, int lev, int flag)
{
	int temp = ep[0];

	if(isvowel(temp) && isvowel(ep[-1])) {
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
	} else
	if(temp==ep[-1]&&temp==ep[-2])
		return 0;
	return strip(ep,d,a,lev,flag);
}

Bits
strip(char* ep, char* d, char* a, int lev, int flag)
{
#pragma ref d
	Bits h = trypref(ep, a, lev, flag);

	if(Set(h,MONO) && isvowel(*ep) && isvowel(ep[-2]))
		h = 0;
	if(h)
		return h;
	if(isvowel(*ep) && !isvowel(ep[-1]) && ep[-1]==ep[-2]) {
		h = trypref(ep-1,a,lev,flag|MONO);
		if(h)
			return h;
	}
	return trysuff(ep,lev,flag);
}

Bits
s(char* ep, char* d, char* a, int lev, int flag)
{
	if(lev > DLEV+1)
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

Bits
an(char* ep, char* d, char* a, int lev, int flag)
{
#pragma ref d
	if(!isupper(*word))	/*must be proper name*/
		return 0;
	return trypref(ep,a,lev,flag);
}

Bits
ize(char* ep, char* d, char* a, int lev, int flag)
{
#pragma ref a
	int temp = ep[-1];
	Bits h;

	ep[-1] = 'e';
	h = strip(ep,"",d,lev,flag);
	ep[-1] = temp;
	return h;
}

Bits
y_to_e(char* ep, char* d, char* a, int lev, int flag)
{
#pragma ref a
	Bits h;
	int  temp;

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

Bits
ily(char* ep, char* d, char* a, int lev, int flag)
{
	int temp = ep[0];
	char *cp = ep;

	if(temp==ep[-1]&&temp==ep[-2])		/* sillly */
		return 0;
	if(*--cp=='y' && !isvowel(*--cp))	/* happyly */
		while(cp>word)
			if(isvowel(*--cp))	/* shyness */
				return 0;
	if(ep[-1]=='i')
		return i_to_y(ep,d,a,lev,flag);
	return cstrip(ep,d,a,lev,flag);
}

Bits
bility(char* ep, char* d, char* a, int lev, int flag)
{
	*ep++ = 'l';
	return y_to_e(ep,d,a,lev,flag);
}

Bits
i_to_y(char* ep, char* d, char* a, int lev, int flag)
{
	Bits h;
	int temp;

	if(isupper(*word))
		return 0;
	if((temp=ep[-1])=='i' && !isvowel(ep[-2])) {
		ep[-1] = 'y';
		a = d;
	}
	h = cstrip(ep,"",a,lev,flag);
	ep[-1] = temp;
	return h;
}

Bits
es(char* ep, char* d, char* a, int lev, int flag)
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

Bits
subst(char* ep, char* d, char* a, int lev, int flag)
{
#pragma ref a
	char *u,*t;
	Bits h;

	if(skipv(skipv(ep-1)) < word)
		return 0;
	for(t=d; *t!='+'; t++)
		continue;
	for(u=ep; *--t!='-';)
		*--u = *t;
	h = strip(ep,"",d,lev,flag);
	while(*++t != '+')
		continue;
	while(*++t)
		*u++ = *t;
	return h;
}

Bits
tion(char* ep, char* d, char* a, int lev, int flag)
{
	switch(ep[-2]) {
	default:
		return trypref(ep,a,lev,flag);
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return y_to_e(ep,d,a,lev,flag);
	}
}

/*
 * possible consonant-consonant-e ending
 */
Bits
CCe(char* ep, char* d, char* a, int lev, int flag)
{
	Bits h;

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
		if(*ep == 'a')	/* prevent -able for -eable */
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
		if(!(ep[-2]=='n' && ep[-1]=='g'))
			return 0;
	}
	return VCe(ep,d,a,lev,flag);
}

/*
 * possible consonant-vowel-consonant-e ending
 */
Bits
VCe(char* ep, char* d, char* a, int lev, int flag)
{
	int c;
	Bits h;

	c = ep[-1];
	if(c=='e')
		return 0;
	if(!isvowel(c) && isvowel(ep[-2])) {
		c = *ep;
		*ep++ = 'e';
		h = trypref(ep,d,lev,flag);
		if(!h)
			h = trysuff(ep,lev,flag);
		if(h)
			return h;
		ep--;
		*ep = c;
	}
	return cstrip(ep,d,a,lev,flag);
}

Ptab*
lookuppref(char** wp, char* ep)
{
	Ptab *sp;
	char *bp,*cp;
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
	next:;
	}
	return 0;
}

/*	while word is not in dictionary try stripping
 *	prefixes. Fail if no more prefixes.
 */
Bits
trypref(char* ep, char* a, int lev, int flag)
{
	Ptab *tp;
	char *bp, *cp;
	char *pp;
	Bits h;
	char space[20];

	if(lev<DSIZ) {
		deriv[lev].mesg = a;
		deriv[lev].type = *a=='.'? NONE: SUFF;
	}
	if(h = tryword(word,ep,lev,flag)) {
		if(Set(h, flag&~MONO) && (flag&MONO) <= Set(h, MONO))
			return h;
		h = 0;
	}
	bp = word;
	pp = space;
	if(lev<DSIZ) {
		deriv[lev+1].mesg = pp;
		deriv[lev+1].type = 0;
	}
	while(tp=lookuppref(&bp,ep)) {
		*pp++ = '+';
		cp = tp->s;
		while(pp<space+sizeof(space) && (*pp = *cp++))
			pp++;
		deriv[lev+1].type += PREF;
		h = tryword(bp,ep,lev+1,flag);
		if(Set(h,NOPREF) ||
		   ((tp->flag&IN) && inun(bp-2,h)==0)) {
			h = 0;
			break;
		}
		if(Set(h,flag&~MONO) && (flag&MONO) <= Set(h, MONO))
			break;
		h = 0;
	}
	if(lev<DSIZ)
		deriv[lev+1] = deriv[lev+2] = emptyderiv;
	return h;
}

Bits
tryword(char* bp, char* ep, int lev, int flag)
{
	int  j;
	Bits h = 0;
	char duple[3];

	if(ep-bp <= 1)
		return h;
	if(flag&MONO) {
		if(lev<DSIZ) {
			deriv[++lev].mesg = duple;
			deriv[lev].type = SUFF;
		}
		duple[0] = '+';
		duple[1] = *ep;
		duple[2] = 0;
	}
	h = dict(bp, ep);
	if(vflag==0 || h==0)
		return h;
	/*
	 * when derivations are wanted, collect them
	 * for printing
	 */
	j = lev;
	prefcount = suffcount = 0;
	do {
		if(j<DSIZ && deriv[j].type) {
			strcat(affix, deriv[j].mesg);
			if(deriv[j].type == SUFF)
				suffcount++;
			else if(deriv[j].type != NONE)
				prefcount = deriv[j].type/PREF;
		}
	} while(--j > 0);
	return h;
}

int
inun(char* bp, Bits h)
{
	if(*bp == 'u')
		return Set(h, IN) == 0;
	/* *bp == 'i' */
	if(Set(h, IN) == 0)
		return 0;
	switch(bp[2]) {
	case 'r':
		return bp[1] == 'r';
	case 'm':
	case 'p':
		return bp[1] == 'm';
	}
	return bp[1] == 'n';
}

char*
skipv(char *s)
{
	if(s >= word && isvowel(*s))
		s--;
	while(s >= word && !isvowel(*s))
		s--;
	return s;
}

/*
 * crummy way to Britishise
 */
void
ise(void)
{
	Suftab *p;
	int i;

	for(i=0; i<26; i++)
		for(p = suftab[i]; p->suf; p++) {
			p->suf = ztos(p->suf);
			p->d1 = ztos(p->d1);
			p->a1 = ztos(p->a1);
		}
}

char*
ztos(char *as)
{
	char *s, *ds;

	for(s=as; *s; s++)
		if(*s == 'z')
			goto copy;
	return as;

copy:
	ds = strdupl(as);
	for(s=ds; *s; s++)
		if(*s == 'z')
			*s = 's';
	return ds;
}

Bits
dict(char* bp, char* ep)
{
	char *cp, *cp1, *w, *wp, *we;
	int n, f;

	w = bp;
	we = ep;
	n = ep-bp;
	if(n <= 1)
		return NOUN;

	f = w[0] & 0x7f;
	f *= 128;
	f += w[1] & 0x7f;
	bp = spacep[f];
	ep = spacep[f+1];

loop:
	if(bp >= ep) {
		if(xflag) 
			fprintf(stderr,"=%.*s\n",n, w);
		return 0;
	}
	/*
	 * find the beginning of some word in the middle
	 */
	cp = bp + (ep-bp)/2;

	while(cp > bp && !(*cp & 0x80))
		cp--;
	while(cp > bp && (cp[-1] & 0x80))
		cp--;

	wp = w + 2;	/* skip two letters */
	cp1 = cp + 2;	/* skip affix code */
	for(;;) {
		if(wp >= we) {
			if(*cp1&0x80)
				goto found;
			else
				f = 1;
			break;
		}
		if(*cp1&0x80) {
			f = -1;
			break;
		}
		f = *cp1++ - *wp++;
		if(f != 0)
			break;
	}

	if(f < 0) {
		while(!(*cp1&0x80))
			cp1++;
		bp = cp1;
		goto loop;
	}
	ep = cp;
	goto loop;
found:
	f = ((cp[0] & 0x7) << 8) |
		(cp[1] & 0xff);
	if(xflag) {
		fprintf(stderr,"=%.*s ",n,w);
		typeprint(encode[f]);
	}
	return encode[f];
}

void
typeprint(Bits h)
{

	pcomma("");
	if(h & NOUN)
		pcomma("n");
	if(h & PROP_COLLECT)
		pcomma("pc");
	if(h & VERB) {
		if((h & VERB) == VERB)
			pcomma("v");
		else
		if((h & VERB) == V_IRREG)
			pcomma("vi");
		else
		if(h & ED)
			pcomma("ed");
	}
	if(h & ADJ)
		pcomma("a");
	if(h & COMP) {
		if((h & COMP) == ACTOR)
			pcomma("er");
		else
			pcomma("comp");
	}
	if(h & DONT_TOUCH)
		pcomma("d");
	if(h & N_AFFIX)
		pcomma("na");
	if(h & ADV)
		pcomma("adv");
	if(h & ION)
		pcomma("ion");
	if(h & V_AFFIX)
		pcomma("va");
	if(h & MAN)
		pcomma("man");
	if(h & NOPREF)
		pcomma("nopref");
	if(h & MONO)
		pcomma("ms");
	if(h & IN)
		pcomma("in");
	if(h & _Y)
		pcomma("y");
	if(h & STOP)
		pcomma("s");
	fprintf(stderr, "\n");
}

void
pcomma(char *s)
{
	static flag;

	if(*s == 0) {
		flag = 0;
		return;
	}
	if(!flag) {
		fprintf(stderr, "%s", s);
		flag = 1;
	} else
		fprintf(stderr, ",%s", s);
}

int
ordinal(void)
{
	char *cp = word;
	static char sp[4];

	while(isdigit(*cp))
		cp++;
	strncpy(sp,cp,3);
	if(isupper(cp[0]) && isupper(cp[1])) {
		sp[0] = Tolower(cp[0]);
		sp[1] = Tolower(cp[1]);
	}
	return 0 == strncmp(sp,
		cp[-2]=='1'? "th":	/* out of bounds if 1 digit */
		*--cp=='1'? "st":	/* harmless */
		*cp=='2'? "nd":
		*cp=='3'? "rd":
		"th", 3);
}

/* layout of file entry: first byte has bit 0x80 turned on.
   next 4 bits count number of characters common between this
   entry and previous one.  last three bits concatenated with
   second byte are the affixing code, so arranged that the 0x80
   bit is zero in all bytes but the first. 3rd and following
   bytes are the remainder of the dictionary word.

   layout in memory: common prefixes are expanded, and the
   first two letters of each word are deleted and found
   instead by lookup in table spacep, which points to the
   first word for each two-letter prefix.
*/

void
readdict(char *file)
{
	char *s, *is, *lasts, *ls;
	int c, i, sp, p;
	int f;
	long l;

	f = open(file, 0);
	if(f == -1) {
		fprintf(stderr, "spell: cannot open %s\n", file);
		exit(1);
	}
	if(read(f, space, 2) != 2)
		goto bad;
	nencode = ((space[0]&0xff)<<8) | (space[1]&0xff);
	if(nencode > sizeof(encode)/sizeof(*encode))
		goto noroom;
	if(read(f, space, nencode*sizeof(*encode))
			!= nencode*sizeof(*encode))
		goto bad;
	s = space;
	for(i=0; i<nencode; i++) {
		l = (long)(s[0] & 0xff) << 24;
		l |= (s[1] & 0xff) << 16;
		l |= (s[2] & 0xff) << 8;
		l |= s[3] & 0xff;
		encode[i] = (Bits)l;
		s += 4;
	}
	l = read(f, space, sizeof(space));
	if(l >= sizeof(space))
		goto noroom;
	is = space + (sizeof(space) - l);
	memmove(is, space, l);

	s = space;
	c = *is++ & 0xff;
	sp = -1;
	i = 0;
	lasts = 0;		/* to prevent diagnostics */
loop:
	if(s > is)
		goto noroom;
	if(c < 0) {
		close(f);
		while(sp < 128*128)
			spacep[++sp] = s;
		*s++ = 0x80;		/* fence */
		return;
	}
	p = (c>>3) & 0xf;
	*s++ = c;
	*s++ = *is++ & 0xff;
	if(p <= 0)
		i = (*is++ & 0xff)*128;
	if(p <= 1) {
		if(!(*is & 0x80))
			i = i/128*128 + (*is++ & 0xff);
		if(i <= sp) {
			fprintf(stderr, "spell: the dict isn't "
			    "sorted or memmove didn't work\n");
			goto bad;
		}
		while(sp < i)
			spacep[++sp] = s-2;
	}
	ls = lasts;
	lasts = s;
	for(p-=2; p>0; p--)
		*s++ = *ls++;
	for(;;) {
		if(is >= space+sizeof(space)) {
			c = -1;
			break;
		}
		c = *is++ & 0xff;
		if(c & 0x80)
			break;
		*s++ = c;
	}
	*s = 0;
	goto loop;

bad:
	fprintf(stderr, "spell: trouble reading %s\n", file);
	exit(1);
noroom:
	fprintf(stderr,"spell: not enough space for dictionary\n");
	exit(1);
}

char *			/* same as strdup; portability hack */
strdupl(char *s)
{
	char *t = (char*)malloc(strlen(s)+1);
	strcpy(t, s);
	return t;
}

void
runout(char *s)
{
	int c;
	if(!cflag)
		printf("%s", s);
	else {
		putchar('-');
		fflush(stdout);
	}
	do {
		if((c=getchar()) == EOF)
	exit(0);
		if(!cflag)
	putchar(c);
	} while(c != '\n');
}
