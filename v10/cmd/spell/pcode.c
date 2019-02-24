
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "code.h"

#ifndef __cplusplus

void	exit(int);
void	qsort(void*, unsigned, int, int(*)(void*, void*));

#else

#include <memory.h>
extern "C" {
void	exit(int);
void	qsort(void*, unsigned, int, int(*)(void*, void*));
}

#endif

/* read an annotated spelling list in the form
	word <tab> affixcode [ , affixcode ] ...
   print a reencoded version
	octal <tab> word
 */

typedef long Bits;
typedef	struct	Dict	Dict;
struct	Dict
{
	char*	word;
	Bits	encode;
};

Dict	words[200000];
char	space[500000];
Bits	encodes[4094];
long	nspace;
long	nwords;
int	ncodes;

void	readinput(FILE*);
long	typecode(char*);
int	wcmp(void*, void*);
void	pdict(void);
void	sput(int);

main(int argc, char *argv[])
{
	FILE* f;

	nwords = 0;
	nspace = 0;
	ncodes = 0;
	if(argc <= 1)
		readinput(stdin);
	while(argc > 1) {
		f = fopen(argv[1], "r");
		if(f == 0) {
			fprintf(stderr, "Cannot open %s\n", argv[1]);
			exit(1);
		}
		readinput(f);
		fclose(f);
		argc--;
		argv++;
	}
	fprintf(stderr, "words = %ld; space = %ld; codes = %d\n",
		nwords, nspace, ncodes);
	qsort(words, nwords, sizeof(words[0]), wcmp);
	pdict();
	return 0;
}

wcmp(void *a, void *b)
{

	return strcmp(((Dict*)a)->word, ((Dict*)b)->word);
}

void
readinput(FILE* f)
{
	long i;
	char *code, *bword;
	char line[200];
	long lineno = 0;

	while(fgets(line, sizeof(line), f)) {
		line[strlen(line)-1] = 0;
		lineno++;
		code = line;
		while(isspace(*code))
			code++;
		bword = code;
		while(*code && !isspace(*code))
			code++;

		i = code-bword;
		memcpy(space+nspace, bword, i);
		words[nwords].word = space+nspace;
		nspace += i;
		space[nspace] = 0;
		nspace++;

		if(*code) {
			*code++ = 0;
			while(isspace(*code))
				code++;
		}
		words[nwords].encode = typecode(code);
		nwords++;
		if(nwords >= sizeof(words)/sizeof(words[0])) {
			fprintf(stderr, "words array too small\n");
			exit(1);
		}
		if(nspace >= sizeof(space)/sizeof(space[0])) {
			fprintf(stderr, "space array too small\n");
			exit(1);
		}
	}
}


typedef	struct	Class	Class;
struct	Class
{
	char*	codename;
	long	bits;
};
Class	codea[]  =
{
	{ "a", ADJ },
	{ "adv", ADV },
	0
};
Class	codec[] =
{
	{ "comp", COMP },
	0
};
Class	coded[] =
{
	{ "d", DONT_TOUCH},
	0
};

Class	codee[] =
{
	{ "ed",	ED },
	{ "er", ACTOR },
	0
};

Class	codei[] =
{
	{ "in", IN },
	{ "ion", ION },
	0
};

Class	codem[] =
{
	{ "man", MAN },
	{ "ms", MONO },
	0
};

Class	coden[] =
{
	{ "n", NOUN },
	{ "na", N_AFFIX },
	{ "nopref", NOPREF },
	0
};

Class	codep[] =
{
	{ "pc", PROP_COLLECT },
	0
};
Class	codes[] =
{
	{ "s", STOP },
	0
};

Class	codev[] =
{
	{ "v", VERB },
	{ "va", V_AFFIX },
	{ "vi", V_IRREG },
	0
};

Class	codey[] =
{
	{ "y", _Y },
	0
};

Class	codez[] =
{
	0
};
Class*	codetab[] =
{
	codea,
	codez,
	codec,
	coded,
	codee,
	codez,
	codez,
	codez,
	codei,
	codez,
	codez,
	codez,
	codem,
	coden,
	codez,
	codep,
	codez,
	codez,
	codes,
	codez,
	codez,
	codev,
	codez,
	codez,
	codey,
	codez,
};

long
typecode(char *str)
{
	Class *p;
	long code;
	int n, i;
	char *s, *sp, *st;

	code = 0;

loop:
	for(s=str; *s != 0 && *s != ','; s++)
		;
	for(p = codetab[*str-'a']; sp = p->codename; p++) {
		st = str;
		for(n=s-str;; st++,sp++) {
			if(*st != *sp)
				goto cont;
			n--;
			if(n == 0)
				break;
		}
		code |= p->bits;
		if(*s == 0)
			goto out;
		str = s+1;
		goto loop;
	cont:;
	}
	fprintf(stderr, "Unknown affix code \"%s\"\n", str);
	return 0;
out:
	for(i=0; i<ncodes; i++)
		if(encodes[i] == code)
			return i;
	encodes[i] = code;
	ncodes++;
	return i;
}

void
sput(int s)
{

	putchar(s>>8);
	putchar(s);
}

void
lput(long l)
{
	putchar(l>>24);
	putchar(l>>16);
	putchar(l>>8);
	putchar(l);
}

/*
 * spit out the encoded dictionary
 * all numbers are encoded big-endian.
 *	struct
 *	{
 *		short	ncodes;
 *		int	encodes[ncodes];
 *		struct
 *		{
 *			short	encode;
 *			char	word[*];
 *		} words[*];
 *	};
 * 0x8000 flag for code word
 * 0x7800 count of number of common bytes with previous word
 * 0x07ff index into codes array for affixes
 */
void
pdict(void)
{
	long i, count;
	Bits encode;
	int j, c;
	char *lastword, *thisword, *word;

	sput(ncodes);
	for(i=0; i<ncodes; i++)
		lput(encodes[i]);

	count = ncodes*4 + 2;
	lastword = "";
	for(i=0; i<nwords; i++) {
		word = words[i].word;
		thisword = word;
		for(j=0; *thisword == *lastword; j++) {
			if(*thisword == 0) {
				fprintf(stderr, "identical words: %s\n", word);
				break;
			}
			thisword++;
			lastword++;
		}
		if(j > 15)
			j = 15;
		encode = words[i].encode;
		c = (1<<15) | (j<<11) | encode;
		sput(c);
		count += 2;
		for(thisword=word+j; c = *thisword; thisword++) {
			putchar(c);
			count++;
		}
		lastword = word;
	}
	fprintf(stderr, "output bytes = %ld\n", count);
}
