#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<signal.h>

#ifndef WORD_LIST
#define WORD_LIST	"/usr/lib/spell/amspell"
#endif
#ifndef WORD_LISTB
#define WORD_LISTB	"/usr/lib/spell/brspell"
#endif
/* the next string should be the first word in spell's stop list */
#define STOPWORD	"Argentinan"
#define	VOWELS		"aeiouy"
#define	ALPHAS		26
#define	LENLIMIT	64
#define	talloc(t)	salloc(sizeof (t))
#define	MAP(c)		((c) - 'a')

typedef	long	Bits;		/* must agree with spell's pcode */

typedef struct word	word;
typedef unsigned long	ulong;

struct word
{
	char	*text;
	int	length;
	ulong	mask;
	word	*next;
};

typedef word	*set[LENLIMIT];

typedef struct
{
	int	count[ALPHAS];
	int	length;
	ulong	mask;
}
	target;

char	*myname;
char	*dict_name = WORD_LIST;
int	interrupt = 0;
int	allwords = 0;
int	fixed;
int	maxwords;
set	words;
word	*list[LENLIMIT];
FILE	*f_dict;

void
error(char *s)
{
	fprintf(stderr, "%s: %s\n", myname, s);
	exit(1);
}

void	*
salloc(ulong z)
{
	void	*p;

	if ((p = malloc(z)) == NULL)
		error("ran out of memory");

	return p;
}

int
word_ok(char *s)
{
	int	vowel = 0;

	while (*s != '\0')
	{
		if (!islower(*s))
			if (allwords) *s = tolower(*s);
			else	return 0;

		switch (*s)
		{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'y':
			vowel = 1;
		}

		s++;
	}

	return vowel;
}

ulong
str_to_mask(char *s)
{
	ulong	m;

	m = 0;

	while (*s != '\0')
		m |= 1 << MAP(*s++);

	return m;
}

char	*
get_word(char *bp)
{
	int	c, i, affix;
	static int n = 0, stop = 0, naffix = 0;

retry:
	if (n == -1)  return NULL;
	c = getc(f_dict);	/* affix encoding */
	affix = naffix | c;
	for (i = n; i < LENLIMIT && !((c = getc(f_dict)) & 0x80); i++) 
			bp[i] = c;
	if (i >= LENLIMIT || i == 0)
		error("Internal dictionary format bad");
	if (c == EOF) n = -1;
	else	{
		n = (c>>3)&017;	/* prefix count, next word */
		naffix = (c&07)<<8;
	}
	bp[i] = '\0';
/* the dictionary includes "stop" (illegal) words, flagged
   by a magic affix that will be different each time pcode is
   run.  Determine the flag dynamically by looking for a
   known word in the stop list, STOPWORD
*/
	if (stop == 0 && strcmp(bp, STOPWORD) == 0)
		stop = affix;	/* magic affix! */
	if ((stop && (affix == stop)) || !word_ok(bp)) goto retry;
	return bp;
}

void
make_word(char *s)
{
	word	*w;
	word	**p;
	int	i;

	w = talloc(word);
	w->length = i = strlen(s);
	w->text = strcpy(salloc(i+1), s);
	w->mask = str_to_mask(s);
	p = &words[w->length];
	w->next = *p;
	*p = w;
}

void
build_wordlist(void)
{
	char	bp[LENLIMIT];
	short	n;

	if ( (f_dict = fopen(dict_name, "r")) == NULL )
		error("can't open word list");

	n = (getc(f_dict)&0xFF) << 8;
	n |= getc(f_dict)&0xFF;

	if (fseek(f_dict, n*sizeof(Bits)+1, 1) != 0) {
		perror(WORD_LIST);
		exit(2);
	}

	if (allwords) {
		make_word("a");
		make_word("i");
		make_word("o");
	}
	while (get_word(bp) != NULL)
		make_word(bp);
}

void
count_letters(target *t, char *s)
{
	int	i;

	for (i = 0; i < ALPHAS; i++)
		t->count[i] = 0;

	t->length = 0;

	while (*s != '\0')
	{
		t->count[MAP(*s++)]++;
		t->length++;
	}
}

int
contained(word *i, target *t)
{
	int	n;
	char	*v;
	target	it;

	if ((i->mask & t->mask) != i->mask)
		return 0;

	count_letters(&it, i->text);

	for (n = 0; n < ALPHAS; n++)
	{
		if (it.count[n] > t->count[n])
			return 0;
	}

	if (it.length == t->length)
		return 1;

	for (v = VOWELS; *v != '\0'; v++)
	{
		if (t->count[MAP(*v)] > it.count[MAP(*v)])
			return 1;
	}

	return 0;
}

int
prune(set in, int m, target *filter, set out)
{
	word	*i, *o, *t;
	int	n;
	int	nz;

	nz = 0;

	for (n = 1; n < LENLIMIT; n++)
	{
		if (n > m)
		{
			out[n] = NULL;
			continue;
		}

		o = NULL;

		for (i = in[n]; i != NULL; i = i->next)
		{
			if (contained(i, filter))
			{
				t = talloc(word);
				*t = *i;
				t->next = o;
				o = t;
				nz = 1;
			}
		}

		out[n] = o;
	}

	return nz;
}

ulong
target_mask(int c[])
{
	ulong	m;
	int	i;

	m = 0;

	for (i = 0; i < ALPHAS; i++)
	{
		if (c[i] != 0)
			m |= 1 << i;
	}

	return m;
}

void
dump_list(word **e)
{
	word	**p;

/*	fprintf(stdout, "%d", (int)(e - list + 1));
*/
	for (p = list; p < e; p++) {
		fputs((*p)->text, stdout);
		putchar(' ');
	}
	puts((*e)->text);
}

void
free_set(set s)
{
	int	i;
	word	*p, *q;

	for (i = 1; i < LENLIMIT; i++)
	{
		for (p = s[i]; p != NULL; p = q)
		{
			q = p->next;
			free(p);
		}
	}
}

void
enumerate(word **p, target *i, set c)
{
	target	t;
	set	o;
	word	*w, *h;
	char	*s;
	int	l, m, b;

	l = p - list;
	b = (i->length + (maxwords - l - 1)) / (maxwords - l);

	for (m = i->length; m >= b; m--)
	{
		h = c[m];

		for (w = h; w != NULL; w = w->next)
		{
			if (interrupt) return;
			if (i->length == m)
			{
				*p = w;
				dump_list(p);
				continue;
			}

			if (l == maxwords - 1)
				continue;

			t = *i;
			t.length -= m;

			for (s = w->text; *s != '\0'; s++)
				t.count[MAP(*s)]--;

			t.mask = target_mask(t.count);
			c[m] = w->next;

			if (prune(c, m, &t, o))
			{
				*p = w;
				enumerate(p + 1, &t, o);
				free_set(o);
			}
		}

		c[m] = h;
	}
}

void
clean(char *s)
{
	char	*p, *q;

	for (p = s, q = s; *p != '\0'; p++)
	{
		if (islower(*p))
			*q++ = *p;
		else if (isupper(*p))
			*q++ = tolower(*p);
	}

	*q = '\0';
}

void
anagramulate(char *s)
{
	target	t;
	set	subjects;

	clean(s);

	if (fixed)
		maxwords = fixed;
	else if ((maxwords = strlen(s) / 4) < 3)
		maxwords = 3;

	fprintf(stdout, "%s:\n", s);
	t.mask = str_to_mask(s);
	count_letters(&t, s);

	if (!prune(words,t.length, &t, subjects))
		return;

	enumerate(&list[0], &t, subjects);
}

void
set_fixed(char *s)
{
	if ((fixed = atoi(s)) < 1)
		fixed = 1;
}

void
read_words(void)
{
	char	*s;
	char	b[LENLIMIT];
	char	b2[LENLIMIT];


	while ((s = fgets(b, LENLIMIT, stdin)) != NULL) {
		if (b[0] == 'q' && b[1] == '\n') exit(0);
		if (isdigit(b[0]))
		{
			set_fixed(s);
			continue;
		}
		if ( b[0] == '\n' )
			s = &b2[0];
		else	s = strcpy(b2, b);

		anagramulate(s);
		if (interrupt) {
			interrupt = 0;
			puts("Interrupt");
		}
		else	puts("Done.");
	}
}

void
fbreak(int dummy)
{
	dummy = dummy;		/* avert use/set diagnostics */
	interrupt += 1;
	signal(SIGINT, fbreak);
	return;
}
void
main(int argc, char **argv)
{
	myname = argv[0];
	if (argc > 1)
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'b')
				dict_name = WORD_LISTB;
			else if (argv[1][1] == 'a')
				allwords = 1;
			else	error("invalid command argument");
			if (argc > 2) set_fixed(argv[2]);
		}
		else	set_fixed(argv[1]);

	build_wordlist();
	signal(SIGINT, fbreak);
	read_words();
	exit(0);
}
