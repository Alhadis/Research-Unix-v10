/* changed to only include mymalloc */
#include	<stdio.h>
#include	<ctype.h>
		/* should split rd.h, rather than include search.h here */
#include	"search.h"
#include	"rd.h"

#ifdef NOT_DEMONK
char *
read_until(database, delim)
FILE	*database;
char	delim;
{
	int	i;
	register char	*p;
	char	*key;
	char	temp[SAFESIZ];

	for (p=temp; ((i = mygetc(database)) != EOF) && (*p=i) != delim; ++p);
	if (i == EOF)
		return((char *) EOF);
	if (p == temp)
		return((char *) 0);
	*p = '\0';
	key = mymalloc((unsigned) strlen(temp)+1);
	strcpy(key, temp);
	return(key);
}

/* need to bring together read_token and r_buftoken */
char *
read_token(file, space)
FILE	*file;
int	space;
{
	register int	i;
	register char	*p;
	char	*token;
	char	temp[BUFSIZ];
	
	if (space == OK_SPACE) {	
		while ((i = mygetc(file)) != EOF && isspace(i));
		myungetc(i, file);
	}
	for (p = temp; (i = mygetc(file)) != EOF && isokchar(i); )
		 *p++ = i;
	myungetc(i, file);
	if (p == temp)
		if (i == EOF)
			return((char *) EOF);
		else
			return((char *) 0);
	*p = '\0';
	token = mymalloc((unsigned) strlen(temp)+1);
	strcpy(token, temp);
	return(token);
}

/* Really need one read_token with an argument selecting character set */
char *
read_filename(file, space)
FILE	*file;
int	space;
{
	int	i;
	register char	*p;
	char	*token;
	char	temp[BUFSIZ];

	if (space == OK_SPACE) {	
		while ((i = mygetc(file)) != EOF && isspace(i));
		myungetc(i, file);
	}
	for (p = temp; (i = mygetc(file)) != EOF && isfilenamechar(i); )
		 *p++ = i;
	/* return EOF only if read EOF as first character */
	if (i == EOF)
		return((char *) EOF);
	myungetc(i, file);
	if (p == temp)
		return((char *) 0);
	*p = '\0';
	token = mymalloc((unsigned) strlen(temp)+1);
	strcpy(token, temp);
	return(token);
}

r_nonblank(database)
FILE	*database;
{
	int	i;

	while ((i = mygetc(database)) != EOF && isspace(i));
	return(i);
}

/* read_delim:
	from paired definitions of delims;
	return close character corresponding to one read
		should not gobble \n --- tosses too much of enduser
		white space. e.g what if
|p
(i)
	looks like this would be == |p(i), which seems to crash with too
	many reasonable possibilities --- is this really wanted for db reads?
	also
|p
	can i remove initial white, or coordinate with other routines
		-- should read_user handle this extra white or here
 */

read_delim(file, newlineok)
FILE	*file;
int	newlineok;
{
	int	open_delim, close_delim;

	if (newlineok == TRUE)
		while(((open_delim=mygetc(file))!=EOF) && isspace(open_delim));
	else
		while(((open_delim=mygetc(file))!=EOF) && iswhite(open_delim));
	if (open_delim == EOF)
		return(EOF);
	if ((close_delim = close_match(open_delim)) == FALSE)
		myungetc(open_delim, file);
	return(close_delim);
}

close_match(c)
int	c;
{
	switch (c) {
		case '(':
			return(')');
		case '[':
			return(']');
		case '{':
			return('}');
		case '<':
			return('>');
		case '\"':
			return('\"');
		case '\'':
			return('\'');
		case '\`':
			return('\'');
		default:
			return(FALSE);
	}
}

#ifdef CHECKKKK
close_match(c)
int	c;
{
	extern struct delim_pairs	delim_pairs[];

	struct delim_pairs	*d;

	for (d = &delim_pairs[0]; d->open != '\0'; ++d)
		if (d->open == c)
			return(d->close);
	return(FALSE);
}

open_match(c)
int	c;
{
	extern struct delim_pairs	delim_pairs[];

	struct delim_pairs	*d;

	for (d = &delim_pairs[0]; d->open != '\0'; ++d)
		if (d->close == c)
			return(d->open);
	return(FALSE);
}
#endif

open_match(c)
int	c;
{
	switch (c) {
		case ')':
			return('(');
		case ']':
			return('[');
		case '}':
			return('{');
		case '>':
			return('<');
		case '\"':
			return('\"');
		case '\'':
			return('\`');
		default:
			return(FALSE);
	}
}

isokchar(c)
char	c;
{
	if (isalnum(c) || c == '_' || c == '.' || c == '$' || c == ':')
		return(TRUE);
	else
		return(FALSE);
}

#ifdef SUPERTOKEN
/* this is not final version - typing in old one... */
supertoken(file, okdelim, okspace, item_sep, list_sep, outer_delim)
FILE	*file;
int	okdelim, okspace, item_sep, list_sep, outer_delim;
{
	int	close_delim = NULL;

	if (okspace)
		while ((i = mygetc(file)) != EOF && isspace(i));
	else
		i = mygetc(file);
	p = temp;
	pe = &temp[BUFSIZ];
	/* new version: soemthing for okdelim here 
		close_delim = (okdelims) ? close_match(i) : NULL; */
	if (okdelims == NULL || (close_delim = close_match(i)) == FALSE) {
		/* are quotes "dflkjashdf" still ok? --- okdelims NULL when
			reading arguments for attributes, are quotes allowed? */
		if (isfirstchar(i))		/* first char:
							\n(bU can be an argument
						 - need to accept \
						 - need to accept quotes even
							if other delim not ok
						*/
			*p++ = i;
		else {
			myungetc(i, file);
			return((char *) 0);
		}
	}
	while ((i = mygetc(file)) != EOF && p < pe)
		if (isokchar(i, close_delim, item_sep, list_sep, outer_delim))
			*p++ = i;
	myungetc(i, file);
	if (p == temp)
		return((i == EOF) ? (char *) EOF : (char *) NULL);
	*p = '\0';
	return(temp);
}

isokchar(i, close_delim, item_sep, list_sep, outer_delim)
int	i, close_delim, item_sep, list_sep, outer_delim;
{
	if (i == NULL || i == EOF)
		return(FALSE);
	if (close_delim == NULL) {
		if (isprint(i) && !isspace(i)) {
			if (i == item_sep || c == list_sep || c == outer_delim)
				return(FALSE);
			if (close_match(i) != FALSE)
				return(FALSE);
		}
	} else
		/* within a delimited pair, can have any char */
		if (i == close_delim)
			return(FALSE);
	return(TRUE);
}

#endif

#ifdef HANDLE_DELIMITERS_AND_SPACES_HERE
/* Need to separate read_values for values which must terminate on any delim
	single user instance:
		|author(name XXXXXX,		ITEM_SEP or close_delim
	db uses:
	-	stack, init on;			ITEM or LIST_SEP
		init on, stack;
	-	on [				any open delimiter
	-	blank.lines 3,			ITEM or LIST_SEP or close_delim
		blank.lines;
		blank.lines 2)
	-	[ name $;			LIST_SEP
	-	|ifvalue draft on [		any open delimiter
 */
isargchar(c, inner_delim, outer_delim)
char	c, inner_delim, outer_delim;
{
	int	okchar = TRUE;

	if (isprint(c)) {
		/* cannot be delimiter:
				inner or outer: any of the close delimiters
		 	<ok within inner delim>
				item: 		ITEM_SEP
				list: 		LIST_SEP	*/
		if (inner_delim == NULL) {
			if (c == ITEM_SEP || c == LIST_SEP || isspace(c))
				okchar = FALSE;
			if (outer_delim == c)
				okchar = FALSE;
		} else if (inner_delim == c)
			okchar = FALSE;
	} else
		okchar = FALSE;
	return(okchar);
}
#endif

isargchar(c)
int	c;
{
/* OLDLIST: if (isalnum(c) || c == '_' || c == '.' || c == '$' || c == '\\'
		|| c == '(' || c == '+' || c == '-' || c == '|' || c == '*') */
/* not called within inner delimiters - uses read_until():
	hey, do you see that it accepts '(' as char, meaning that db manager
	cannot use '(' to start an attribute case definition!!!!!!
	if (isgraph(c) && c != ITEM_SEP && c != LIST_SEP
				!isopendelim(c) && c != outer_delim) */
	if (isalnum(c) || c == '_' || c == '.' || c == '$' || c == '\\'
		|| c == '(' || c == '+' || c == '-' || c == '|' || c == '*'
		|| c == '&')
		return(TRUE);
	return(FALSE);
}

isfilenamechar(c)
char	c;
{
	if (isalnum(c) || c == '_' || c == '.' || c == '$'
				|| c == '+' || c == '-' || c == '/')
		return(TRUE);
	else
		return(FALSE);
}

struct strings *
parse_token(s)
char	*s;
{
	register char	*p;
	static struct strings	str;
	int	length;
	
	for (; *s != '\0' && *s != '\n' && isspace(*s); ++s);
	if (*s == '\n')
		return((struct strings *) 0);
	for (p = s; *p != '\0' && isokchar(*p); ++p);
	if ((length = p - s) == 0)
		return((struct strings *) 0);
	str.newp = p;
	str.token = mymalloc((unsigned) length+1);
	strncpy(str.token, s, length);
	str.token[length] = '\0';
	return(&str);
}

char	*
tmpstrcat(s1, s2)
char	*s1, *s2;
{
	char	tmpstring[512];

	strcpy(tmpstring, s1);
	strcat(tmpstring, s2);
	return(tmpstring);
}

char	*
strconcat(s1, s2)
char	*s1, *s2;
{
	char	*new;

	/* must return copy even if one string is null */
	new = mymalloc((unsigned) strlen(s1) + strlen(s2) + 1);
	strcpy(new, s1);
	strcat(new, s2);
	return(new);
}
#endif

char *
mymalloc(size)
unsigned	size;
{
	char	*malloc();
	char	*p;

	if ((p = malloc(size)) == (char *) 0)
		fprintf(stderr, "Malloc: cannot allocate %d bytes\n", size);
	return(p);
}

#ifdef NOT_DEMONK
char *
myrealloc(p, size)
char	*p;
unsigned	size;
{
	char	*realloc();

	char	*newp;

	if ((newp = realloc(p, size)) == (char *) 0) {
		fprintf(stderr, "Myrealloc: cannot reloc %o for %d bytes\n",
							p, size);
		return((char *) 0);
	}
	return(newp);
}
#endif

myfree(pointer, number)
char	*pointer;
short	number;
{
	if (pointer == (char *) 0 || pointer == (char *) EOF) {
		fprintf(stderr, "Free %d: no block to free\n", number);
		return;
	}
	free(pointer);
}

#ifdef NOT_DEMONK
char *
ifdefined(s)
char	*s;
{
	ENTRY	*exist;

	if ((exist = hashfind(DEFINE_TABLE, s)) == (ENTRY *) NULL)
		return(s);
	return(exist->data);
}
#endif
