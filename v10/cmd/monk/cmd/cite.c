/**
 ** cite.c
 **	Read in file and set definitions for citations.
 **	Form is as follows:
 **		.CD "key" "definition"
 **		.CU "key" endtext
 **/

#include	<stdio.h>
#include	<ctype.h>

#define		TRUE			(1)
#define		FALSE			(0)

typedef struct nlist nlist;

struct nlist {
	char *n_key;			/* Key for this element */
	char *n_def;			/* Definition of this element */
	char *n_reg;			/* Where stuff is stored in troff */
	nlist *n_next;			/* Pointer to next element */
	char n_flags;			/* Flags for element */
};

#define		HASHSIZE		101
static nlist *hashtab[HASHSIZE];

/*
 * subchar[sizeof(DEFCHARS) - 1][NPOSS]:
 *	All possible combinations of , or @ and all alphanumeric characters.
 */

#define		DEFCHARS		",@"
#define		NPOSS			62
char subchar[sizeof(DEFCHARS) - 1][NPOSS];

unsigned int flags, numitems;
char *name, *nextline;

/*
 * Flags for elements of hash table and/or substitutions.
 */
#define		N_USED			001
#define		N_DEFINED		002

/*
 * Flags for program
 */
#define		PRINTUNUSED		001
#define		CITESORT		002

nlist *lookup(), **sortlist();
char *calloc(), *getnextchar(), *malloc(), *slashify(), *strsave(), *strtok();
char *strchr(), *get_word();
int strlen(), ncompare();
void cfree();


main(argc, argv)
register int  argc;
register char *argv[];
{
 register nlist *np, **items;
 register FILE *fp;
 extern int optind;
 extern char *optarg;
 register c, nitem;

	name = strsave(argv[0]);
	while ((c = getopt(argc, argv, "su")) != EOF)
		switch(c) {

		case 's':		/* Sort definitions */
			flags |= CITESORT;
			break;

		case 'u':		/* Print unused citations */
			flags |= PRINTUNUSED;
			break;

		case '?':		/* Not a legal flag */
			errexit(1, "%s: options are -su\n", name);

		}

	fp = stdin;

	do {
		if (argc > optind && (fp = fopen(argv[optind], "r")) == NULL)
			errexit(1, "%s: cannot open %s\n", name, argv[optind]);
		doit(fp);
		fclose(fp);
	} while (++optind < argc);

	if (flags & PRINTUNUSED) {
		for (optind = 0, nitem = 0; optind < HASHSIZE; ++optind)
			for (np = hashtab[optind]; np != NULL; np = np->n_next)
				if (!(np->n_flags & N_USED) ||
					!(np->n_flags & N_DEFINED))
					++nitem;

		items = sortlist(nitem, N_USED, N_DEFINED);
		for (c = 0; c < nitem; ++c)
			if (items[c]->n_flags & N_DEFINED)
				fprintf(stderr, "%s unused\n", items[c]->n_key);
			else
				fprintf(stderr, "%s undefined\n", items[c]->n_key);
	}

	if (flags & CITESORT) {
		items = sortlist(numitems, 0, 0);
		for (c = 0; c < numitems; ++c)
			fprintf(stdout, ".CD \"%s\" \"%s\"\n", items[c]->n_key,
				items[c]->n_def);
	}
	exit(0);
}

/*
 * sortlist(nitem, flg, flg2)
 *	Sorts all items that don't have flg set
 */

nlist **
sortlist(nitem, flg, flg2)
register nitem, flg, flg2;
{
 register nlist **items, *np;
 register x, c;

	c = 0;
	items = (struct nlist **) calloc(nitem + 1, sizeof(items[0]));
	for (x = 0; x < HASHSIZE; ++x)
		for (np = hashtab[x]; np != NULL; np = np->n_next)
			if (!flg || (!(np->n_flags & flg) || !(np->n_flags & flg2))) {
				items[c] = (struct nlist *) malloc(sizeof(items[0]));
				items[c++] = np;
			}

	qsort(items, nitem, sizeof(items[0]), ncompare);
	return items;
}

int
ncompare(n1, n2)
register nlist **n1, **n2;
{
	return(strcmp((*n1)->n_reg, (*n2)->n_reg));
}

doit(fp)
register FILE *fp;
{
 char buf[BUFSIZ];

	while (fgets(buf, sizeof(buf), fp)) {
		if (!strncmp(buf, ".CD", sizeof(".CD") - 1)) {
			define(buf);
			if (nextline)
				fputs(nextline, stdout);
		}
		else if (!(flags & CITESORT)) {
			if (!strncmp(buf, ".CU", sizeof(".CU") - 1))
				use(buf);
			else
				fputs(buf, stdout);
		} else fprintf(stderr, "%s", buf);
	}
}

/*
 * define(line)
 *	.CD "key" "definition"
 */

define(line)
register char *line;
{
 register nlist *np;
 register char *key, *def;
 char new;
 int length;

	new = FALSE;
	if (strlen(line) <= 4) {
		error("no definition contained in line");
		return;
	}

	if ((key = get_word(line + sizeof(".CD ") - 1, &nextline)) == NULL) {
		error("no definition contained in line");
		return;
	}
	if ((def = get_word(nextline, &nextline)) == NULL) {
		error("no definition contained in line");
		return;
	}

	if (*nextline == '\n') {
		cfree(nextline);
		nextline = NULL;
	} else
		while (*nextline == ' ' || *nextline == '\t')
			++nextline;

	if ((np = lookup(key)) == NULL) {
		if ((np = (nlist *) calloc(1, sizeof(nlist))) == NULL)
			errexit(-1, "%s: out of memory\n", name);
		new = TRUE;
		++numitems;
	} else if (flags & CITESORT) {
		if (strcmp(np->n_def, def)) {
			error("%s redefined as %s (old value %s)", np->n_key,
				def, np->n_def);
			cfree(np->n_def);
			np->n_def = def;
		}
		return;
	}

	if (!np->n_key)
		np->n_key = key;
	else
		cfree(key);
	if (np->n_def)
		cfree(np->n_def);
	np->n_def = def;

	if (!np->n_reg)
		if ((np->n_reg = strsave(getnextchar())) == NULL) {
			error("cannot find another register");
			return;
		}

	if (new)
		install(np);
	np->n_flags |= N_DEFINED;

	if (!(flags & CITESORT)) {
		fprintf(stdout, ".ds %s %s\n", np->n_reg, np->n_def);
		fprintf(stdout, ".tm .CD \"%s\" \"%s\"\n", slashify(np->n_key),
			np->n_def);
	}
}

/*
 * use(line)
 *	.CU "key" endtext
 */

use(line)
register char *line;
{
 register char *key;
 register nlist *np;

	if (strlen(line) <= 4) {
		error("no key included in line");
		return;
	}

	if ((key = get_word(line + sizeof(".CU ") - 1, &nextline)) == NULL) {
		error("no key included in line");
		return;
	}

	if ((np = lookup(key)) == NULL) {
		if ((np = (nlist *) calloc(1, sizeof(nlist))) == NULL)
			errexit(-1, "Out of memory\n");
		np->n_key = key;
		fprintf(stdout, "ZZ%s", nextline);
		np->n_flags |= N_USED;
		install(np);
	} else {
		if (np->n_flags & N_DEFINED)
			fprintf(stdout, "\\*(%s%s", np->n_reg, nextline);
		else
			fprintf(stdout, "ZZ%s", nextline);
		np->n_flags |= N_USED;
	}
}	

/*
 * get_word(str, after)
 *	Return next word, where a word is one word or several in "'s
 *	After will contain all text after the end of the word
 */

char *
get_word(str, after)
register char *str, **after;
{
 register char *sp;

	while (*str == ' ' || *str == '\t')
		++str;

	if (*str == '"') {
		++str;
		if ((sp = strchr(str, '"')) == NULL)
			return NULL;
		*sp = NULL;
		*after = strsave(sp + 1);
		return strsave(str);
	} else {
		for (sp = str; *sp && *sp != ' ' && *sp != '\t'; ++sp)
			;
		if (!*sp)
			return NULL;
		*sp = NULL;
		*after = strsave(sp + 1);
		return strsave(str);
	}
}

/*
 * hash(s)
 *	Hash string s.
 */

hash(s)
register char *s;
{
 register hashval;

	for (hashval = 0; *s != NULL; )
		hashval += *s++;
	return(hashval % HASHSIZE);
}

/*
 * lookup(s)
 *	Look for s in hash table.
 */

nlist *
lookup(s)
register char *s;
{
 register nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->n_next)
		if (!strcmp(s, np->n_key))
			return(np);
	return NULL;
}

/*
 * install(np)
 *	Install a citation into hash table.
 */

install(np)
register nlist *np;
{
 register hashval;

	hashval = hash(np->n_key);
	np->n_next = hashtab[hashval];
	hashtab[hashval] = np;
}

/*
 * strsave(str)
 *	Allocate space for str, fill it therewith, return address.
 */

char *
strsave(str)
register char *str;
{
 register char *save;

	if (str == NULL)
		return(NULL);
	if ((save = calloc(1, (unsigned int) strlen(str) + 1)) == NULL)
		errexit(-1, "%s: out of memory\n", name);
	strcpy(save, str);
	return(save);
}

/*
 * slashify(string)
 *	Change all \ into \\.
 */

char *
slashify(str)
register char *str;
{
 register char *pp, *ptr;
 static char *string = 0;
 register int n_back = 0;

	if (str == NULL)
		return(NULL);

	if (!(flags & CITESORT))
		for (ptr = str; *ptr; ++ptr)
			if (*ptr == '\\')
				++n_back;

	if (string)
		cfree(string);
	string = calloc(1, strlen(str) + (sizeof("\\") - 1) * n_back + 1);
	strcpy(string, str);
	ptr = string;
	while (*ptr)
		if (*ptr == '\\' && !(flags & CITESORT)) {
			for (pp = &string[strlen(string)]; pp > ptr; --pp)
				pp[sizeof("\\") - 1] = *pp;
			ptr[1] = '\\';
			ptr += sizeof("\\");
		} else
			++ptr;

	return(string);
}

/*
 * getnextchar()
 *	Return n_next available substitution string.
 */

char *
getnextchar()
{
 register x;
 register char *y;
 static char str[3];

	for (x = 0; x < sizeof(DEFCHARS) - 1; ++x)
		for (y = subchar[x]; y < &subchar[x][NPOSS]; ++y)
			if (!*y) {
				*y = 1;
				str[0] = DEFCHARS[x];
				str[1] = convert(y - subchar[x]);
				return(str);
			}

	errexit("Out of substitutions\n");
}

/*
 * convert(n)
 *	Switch an integer value into the corresponding string
 */

convert(n)
register n;
{
	if (n <= 9)
		return(n + '0');
	else if (n <= 35)
		return(n - 10 + 'A');
	else if (n < NPOSS)
		return(n - 36 + 'a');
	errexit(1, "s_unhash: %d not legal\n", n);
}

/* VARARGS */
error(str, a1, a2, a3, a4)
register char *str;
char *a1, *a2, *a3, *a4;
{
	fprintf(stderr, "%s: ", name);
	fprintf(stderr, str, a1, a2, a3, a4);
	putc('\n', stderr);
}

/* VARARGS */
errexit(ret_code, s1, s2, s3, s4)
{
	fprintf(stderr, s1, s2, s3, s4);
	exit(ret_code);
}
