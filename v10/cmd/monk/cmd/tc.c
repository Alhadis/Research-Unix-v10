/** 
 ** Program tc.c: monk preprocessor for table of contents.
 ** Appropriate format for input entries is as follows,
 ** with tabs between fields
 **
 ** .TC	<category>	<level>	<sequence>	<text>	<page>
 ** .TC	section		2	5.1	DAA Subtasks	18	
 **
 ** Output format is: 
 ** |tc_entry("<level>" "<sequence>" "<text>" "page")
 ** |tc_entry("2" "5.1" "DAA Subtasks" "18")
 **
 ** The default ordering of output categories for the table of contents is:
 ** 	section, with title "Table of Contents",
 ** 	table, with title "List of Tables", and
 ** 	figure, with title, "List of Figures."
 **
 ** This ordering can be modified using the -a (augment) or -c (change) option.
 ** In both cases the option must be followed by the name of the file that
 ** contains the information.  With -a the categories listed are appended to
 ** the end, while in -c the entire list must be indicated.  The format for
 ** the file entries has, in the desired order, a line for each category.
 ** If the same category is listed twice, or if a default category is
 ** repeated using the -a option, only the first instance will apply.
 ** Each line begins with the category name, followed by a tab, and then
 ** the title that goes with that category.  An example follows.
 **
 ** section	Contents
 ** diagram	Diagram List
 ** equation	List of Equations
 ** example	Examples
 **
 ** The -l option is used to indicate the maximum level of entries that
 ** should be included, which generally refers to the table of contents.
 ** For example, the sequence, 3.2.5 has a level of 3.
 ** 
 ** The output includes a title for each non-null category, for example,
 ** |tc_heading(Table of Contents), followed by the associated items.
 **/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define 	TRUE		(1)
#define		FALSE		(0)
#define		HASHSIZE	(101)
#define		PARTSIZE	(30)
#define		OPTIONS		"a:l:c:"
#define		CHNGMAX		(100)
#define		CATMAX		(10)
#define		DELIM		"\t\n"

typedef struct entry nlist;

struct entry {
	int   cat;
	char *lev;
	char *seq;
	char *text;
	char *page;
	char *partline;
	int  roman;
	nlist *next;
};

struct node {
	char *cat;
	char *title;
	int count;
} order[CATMAX] = {
	"section", "TABLE OF CONTENTS", 0,
	"table", "LIST OF TABLES", 0,
	"figure", "LIST OF FIGURES", 0,
	0, 0, 0
};

nlist *hashtab[HASHSIZE];
char *optarg, *name;
int nitems;
int optind;
int ncats = 3;
int levels = 2;

/*
 * 		main program
 * After options are processed, the file containing the input entries
 * is opened.  "Doit" checks whether the entries begin with .TC, 
 * and prints invalid entries on stderr.  Valid entries are processed
 * by "define", which calls "getfields" to parse the entry.  After
 * entries are installed, they are sorted according to the default
 * or revised category ordering and printed on stdout.
*/

main(argc, argv)
register int argc;
register char *argv[];
{
	FILE *fp, *ordfile;
	struct node *ordp;
	nlist **items, **ptr;
	register c, index;
	int revflg = 0;
	int augflg = 0;
	int levflg = 0;
	int errflg = 0;
	int getopt();
	nlist **sortlist();
	void change(), doit();

	while ((c = getopt(argc, argv, OPTIONS)) != EOF)
		switch (c) {
		case 'a':           /* augment table */
			if (revflg || augflg)
				errexit(5, "%s: category order already changed\n",
				    name);
			if ((ordfile = fopen(optarg, "r")) == NULL)
				errexit(2, "%s: cannot open %s\n", name, optarg);
			change(ordfile);
			fclose(ordfile);
			augflg++;
			break;
		case 'c':           /* revise table  */
			if (revflg || augflg)
				errexit(5, "%s: category order already changed\n",
				    name);
			if ((ordfile = fopen(optarg, "r")) == NULL)
				errexit(2, "%s: cannot open %s\n", name, optarg);
			ncats = 0;
			change(ordfile);
			fclose(ordfile);
			revflg++;
			break;
		case 'l':	    /* number of levels */
			if (levflg)
				errexit(4, "%s: levels already specified\n", name);
			levels = atoi(optarg);
			levflg++;
			break;
		case '?':
			errflg++;
		}
	if (errflg) {
		fprintf(stderr, "options are %s\n: ", OPTIONS);
		exit(2);
	}
	fp = stdin;
	do {
		if (argc > optind && (fp = fopen(argv[optind], "r")) == NULL)
			errexit(2, "%s: cannot open %s\n", name, argv[optind]);
		doit(fp);
		fclose(fp);
	} while (++optind < argc);
	items = sortlist(nitems);

	ptr = items;
	for (ordp = order; *ordp->cat; ordp++) {
		if (ordp->count != 0) {
			fprintf(stdout, "|tc_heading(%s)\n", ordp->title);
			for (index = 0; index < ordp->count; ++index, ++ptr)
				fprintf(stdout, "|tc_entry(\"%s\" \"%s\" \"%s\" \"%s\")\n",
				    (*ptr)->lev, (*ptr)->seq, (*ptr)->text, (*ptr)->page);	
		}
	}

}

/*
 * 		change(fp)
 * Modifies list and order of output categories when -a or -r option chosen
 * Reads from designated file the name and title for each category
 * and initializes the count for each to 0.
 */

void
change(fp)
FILE *fp;
{
	char *strsave();
	char buf[BUFSIZ];
	int max;

	max = CATMAX - 1;
	while(ncats < max && fgets(buf, sizeof(buf), fp) != NULL) {
		order[ncats].cat = strsave(strtok(buf, DELIM));
		order[ncats].title = strsave(strtok((char *)NULL, DELIM));
		order[ncats].count = 0;
		++ncats;
	}
	order[ncats].cat = 0;
}

/*
 * 		doit(fp)
 * Checks for entries that begin with .TC
 * Invalid entries are printed on stderr
 * Function calls define on valid entries to create entry in hash table
*/

void
doit(fp)
register FILE *fp;
{
	void define();
	char buf[BUFSIZ];

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (!strncmp(buf, ".TC", sizeof(".TC") - 1))
			define(buf);
		else fputs(buf, stderr);
	}
}

/*
 * 		define(line)
 * The function checks whether entry already installed (in hash table).
 * It updates the page number if a new entry differs from a previous
 * one only in the page field, and indicates (on stderr) that such a
 * change was made.  If there are too many (defined by CHNGMAX)
 * repeated entries, the program aborts with an error message.
 * The "getfields" function is called to parse the entry and provide
 * the desired fields.  If "getfields" returns a value of 1, the
 * processed entry is installed.
*/

void
define(line)
register char *line;
{
	int changed = 0;
	static int changes = 0;
	nlist *lookup();
	char *malloc(), *strsave();
	int getfields();
	void install();
	register nlist *np, *hashnp;

	if ((np = (nlist *) malloc(sizeof(nlist))) == NULL)
		errexit(1, "%s: out of memory\n", name);
	if (getfields(line, np) == 0)
		return;
	if (((hashnp = lookup(np->partline)) == NULL) ||
			*np->seq == '\0') {
		install(np);
		++nitems;
		return;
	}
	order[np->cat].count--;
	if ((strcmp (hashnp->text, np->text) != NULL) &&
			*np->seq) {
		if (changes > CHNGMAX)
			errexit(6, "%s: too many changes\n", name);
		error("text has changed");
		hashnp->text = np->text;
		fprintf(stderr, "%s\n", line);
		++changed;
	}
	if ((strcmp(hashnp->page, np->page) != NULL) &&
			*np->seq) {
		if (changes > CHNGMAX)
			errexit(6, "%s: too many changes\n", name);
		error("page number has changed");
		hashnp->page = np->page;
		fprintf(stderr, "%s\n", line);
		++changed;
	}
	if (changed)
		++changes;
}

/*
 * 		getfields(line)
 * This function is called by "define" to parse an input entry.
 * If an entry is valid, the count for the associated category 
 * (e.g., section) is incremented.  Error messages are provided
 * if there are not enough or too many fields.  A return value of
 * 1 indicates that the item is valid, while 0 indicates invalid.
 */

int
getfields(line, np)
register char *line;
register nlist *np;
{
	char *strsave(), *getname();
	char *oldline, *category, *restline, *partptr;
	char partline[PARTSIZE];
	int found;
	int i;
	register index;
		
	oldline = strsave(line);
	category = getname(oldline + sizeof(".TC"), &restline);
	found = FALSE;
	for (index = 0; index < ncats && !found; index++)
		if (!strcmp(order[index].cat, category)) {
			order[index].count++;
			np->cat = index;
			found = TRUE;
		}
	if (!found) 		
		return(0);
	np->lev = getname(restline, &restline);
	if (atoi(np->lev) > levels) {
		fprintf(stderr, "%s\n",line);
		order[np->cat].count--;
		return(0);
	}
	np->seq = getname(restline, &restline);
	np->text = getname(restline, &restline);
	np->page = getname(restline, &restline);
	if (*restline) {
		error("too many fields");
		fprintf(stderr, "%s\n", line);
		order[np->cat]. count--;
		return(0);
	}
	if (np->page == NULL) {
		error("not enough fields");
		fprintf(stderr, "lev = %s, seq = %s, text = %s, page = %s\n", 
			np->lev, np->seq, np->text, np->page);
		fprintf(stderr, "%s\n", line);
		order[np->cat].count--;
		return(0);
	}
	partline[0] = np->cat + '0';
	partline[1] = '\0';
	strcat(partline, np->lev);
	strcat(partline, np->seq);
	np->partline = strsave(partline);
	if (isalpha(*(np->page))) 
		np->roman = rtoi(np->page);
	else
		np->roman = 0;
	return(1);		
}

/*		getname(str, after)
 * This function gets the next token on the line.
 * The rest of the line is left in restline.
 */

char *
getname(str, after)
register char *str, **after;
{
 register char *sp;

	for (sp = str; *sp && *sp != '\t' && *sp != '\n'; ++sp)
		;
	if (*sp == '\t') {
		*after = sp + 1;
		*sp = NULL;
	}
	else if (*sp == '\n') {
		*after = NULL;
		*sp = NULL;
	}
	else
		return(NULL);
	return(strsave(str));
}

/*
 * 		lookup(s)
 * Looks for entry in hash table.  Returns pointer to entry (np)
 * if found, and NULL otherwise.  In checking items, page number
 * is disregarded to allow updating if necessary.
 */

nlist *
lookup(s)register char *s;
{
	int hash();
	register nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (!strcmp(s, np->partline))
			return(np);
	return NULL;

}

/*
 * 		install(np)
 * Installs an entry into hash table.
 */

void
install(np)
register nlist *np;
{
	int hash();
	register hashval;
	
	hashval = hash(np->partline);
	np->next = hashtab[hashval];
	hashtab[hashval] = np;
}


/*
 * 		hash(s)
 * Hashes string s and returns the hashed value.
 */

int
hash(s)
register char *s;
{
	register hashval;
	for (hashval = 0; *s != NULL;)
		hashval += *s++;
	return(hashval % HASHSIZE);
}

/*
 *		sortlist(nitems)
 * Sorts entries in hash table, using system qsort function.
 * Creates sorted list, called "items" and returns pointer.
 * Uses "ncompare" function defined below.
 */

nlist **
sortlist(num)
register num;
{
	char *calloc();
	int ncompare();
	register nlist **items, *np;
	register x, c;
	c = 0;
	items = (nlist **) calloc((unsigned int)num + 1, sizeof(nlist *));
	for (x = 0; x < HASHSIZE; ++x)
		for (np = hashtab[x]; np != NULL; np = np->next)
			items[c++] = np;
	qsort((char *)items, num, sizeof(items[0]), ncompare);
	return (items);
}

/*
 *		ncompare(n1, n2)
 * Function used by qsort to compare entries and produce sorted list.
 * Orders sequentially by category, page, and sequence.
 * Returns positive, negative, or 0 value according to whether first
 * item is greater than, less than, or equal to the second.
 * Calls the compare function below for entries in same category.
 */

int
ncompare(n1, n2)
register nlist **n1, **n2;
{
	int compare();
	int diff;

	if ((*n1)->roman && (*n2)->roman)
		return((*n1)->roman - (*n2)->roman);
	else if (diff = (*n1)->roman - (*n2)->roman) 
		return (-diff);
	else if (diff = (*n1)->cat - (*n2)->cat)
		return (diff);
	else if (diff = compare((*n1)->page, (*n2)->page))
		return (diff);
	else 
		return(compare((*n1)->seq, (*n2)->seq));
}

/*
 *		compare(pa, pb):  Adapted from Kowalski
 * Called by "ncompare" to compare two items.
 * Returns positive number if first is larger, negative if second
 * is larger, and 0 for equality.  Can compare sequences such as
 * 4.2.3 with other sequences or with integers.  
 *	
*/

int 
compare(pa, pb)
register char *pa, *pb;
{
	int a, b;
	register char *la, *lb;
	int sa, sb;
	register char *ipa, *ipb;

	la = &pa[strlen(pa)];
	lb = &pb[strlen(pb)];
	for ( ; ; ++pa, ++pb) {
		for ( ; *pa == *pb; ++pa, ++pb) {
			if (pa == la)
				return(0);
			if (isdigit(*pa) && isdigit(*pb))
				break;
		}
		if (!isdigit(*pa) || !isdigit(*pb))
			return(*pa - *pb);
		sa = sb = 1;
		while(isspace(*pa))
			pa++;
		while(isspace(*pb))
			pb++;
		if(*pa == '-') {
			pa++;
			sa = -sa;
		}
		if(*pb == '-') {
			pb++;
			sb = -sb;
		}
		for(ipa = pa; ipa<la&&isdigit(*ipa); ipa++) ;
		for(ipb = pb; ipb<lb&&isdigit(*ipb); ipb++) ;
		a = 0;
		if(sa==sb)
			while(ipa > pa && ipb > pb)
				if(b = *--ipa - *--ipb)
					a = b;
		while(ipa > pa)
			if(*--ipa != '0')
				return(sa);
		while(ipb > pb)
			if(*--ipb != '0')
				return(-sb);
		if(a) return(a*sa);
	}
}    /* end of compare */

/*
 * 		strsave(str)
 * Allocates space for string, fills it, and returns address.
 */

char *
strsave(str)
register char *str;
{
	char *malloc();
	register char *save;

	if (str == NULL)
		return(NULL);
	if ((save = malloc((unsigned int) strlen(str) + 1)) == NULL)
		errexit(1, "%s: out of memory\n", name);
	strcpy(save, str);
	return(save);
}

/* 
 * 		rtoi(numeral)  
 * Converts roman numeral less than 900 to integer and returns its value.
 * No checks for well formed roman numeral or other invalid input.
*/

rtoi(numeral)
char *numeral;
{
	char *sp;
	int numb = 0;

	sp = numeral;
	while (*sp != '\0') {
		switch (*sp) {
		case 'd': case 'D':
			numb = numb + 500;
			break;
		case 'c': case 'C':
			numb = numb + 100;
			break;
		case 'l': case 'L':
			numb = numb + 50;
			break;
		case 'x': case 'X':
			if (*(sp + 1) == 'c' || (*sp+1) == 'C') {
				numb = numb + 90;
				sp++;
			}
			else if (*(sp+1) == 'l' || (*sp+1) == 'L') {
				numb = numb + 40;
				sp++;
			}
			else {
				numb = numb + 10;
			}
			break;
		case 'v': case 'V':
			numb = numb + 5;
			break;
		case 'i': case 'I':
			if (*(sp + 1) == 'x' || (*sp+1) == 'X') {
				numb = numb + 9;
				sp++;
			}
			else if (*(sp + 1) == 'v' || (*sp+1) == 'V') {
				numb = numb + 4;
				sp++;
			}
			else {
				numb = numb + 1;
			}
			break;
		}
		sp++;
	}
	return(numb);
}

/* 
 *		error(str, a1, a2, a3, a4)
 * Prints error message
 */

/* VARARGS */
error(str, a1, a2, a3, a4)
register char *str;
char *a1, *a2, *a3, *a4;

{
	fprintf(stderr, "%s: ", name);
	fprintf(stderr, str, a1, a2,a3, a4);
	putc('\n', stderr);
}

/* 
 * 		errexit(ret_code, s1, s2, s3, s4)
 * Prints error message on stderr and exits with return code.
 */

/* VARARGS2 */
errexit(ret_code, s1, s2, s3, s4)
char *s1, *s2, *s3, *s4;
{
	fprintf(stderr, s1, s2, s3, s4);
	exit(ret_code);
}

