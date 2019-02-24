#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"

textputs(s, whose)
register char	*s;
{
	if (whose == U_TEXT)
		while (*s != '\0')
			textputc(*s++, whose);
	else /* whose == MONK_DB or FORCE */
		checkputs(s, whose, '\0');
}

char	*
checkputs(s, spew, end_char)
register char	*s;
short	spew;
char	end_char;
{
	static short	layer;
	char	*news;

	if (end_char == '\0')
		layer = 0;
	else
		++layer;
	while (*s != end_char && *s != '\0')
		if (*s == EMBED_IN_TROFF) {
			if ((news = checkif(s++, spew)) == (char *) 0) {
				if (spew != FALSE)
					textputc(EMBED_IN_TROFF, spew);
				continue;
			} else {
				s = news;
			}
		} else {
			if (spew != FALSE)
				textputc(*s, spew);
			++s;
		}
	if (*s != end_char)
		fprintf(stderr, "Reached end_of_string b4 delim %c,%o\n",
							end_char, end_char);
	if (*s != '\0') {
		s += 1;
		if (--layer < 0)
			fprintf(stderr, "Negative layer count\n");
	}
	return(s);
}

char	*
checkif(s, spew)
register char	*s;
short	spew;
{
	struct conditional	*cond;
	int	true;

/* ZZZZ: can a user-defined conditional fail here w no earlier message?? */
	if ((cond = isitaconditional(s+1, FALSE, FALSE))
						== (struct conditional *) 0) {
		if (spew != FALSE)
			textputc(*s, spew);
		return(s+1);
	}
	if (spew != FALSE) {
		true = iscondtrue(cond->attribute, cond->value, cond->type);
		spew = (true == FALSE) ? FALSE : spew;
	}
	free_cond(cond);
	return(checkputs(cond->newp, spew, cond->close_delim));
}

iscondtrue(attribute, value, type)
char	*attribute;
struct value	*value;
{
	ENTRY	*grabit;
	struct statestack	*st;
	int	true;

	if ((grabit = hashfind(STACK_TABLE, attribute))== (ENTRY *) NULL)
		return(FALSE);
	st = (struct statestack *) grabit->data;
	if (st->last != (struct state *) 0) {
		true = cmp_valu(value, st->last->value);
		if (type == FALSE)
			true = (true == TRUE) ? FALSE : TRUE;
	}
	return(true);
}

void
copy_userfile(filename)
char	*filename;
{
	FILE	*userfile;
	int	c;

	/* fopenncheck and mygetc assure that the filename and linenumbers
		are correctly updated for database manager */
	if ((userfile = fopenncheck(filename, "r", TRUE, NO_EXIT)) == NULL)
		return;
	while ((c = mygetc(userfile)) != EOF)
		fputc(c, stdout);
	fclosencheck(userfile);
}

/* put text into formatted output, suppressing multiple newlines and white space
	following new-lines --- note: isspace true for newlines! */

/* new version:
	hold over monk char til next call to textputc ????
	if lastchar == newline
		if (lastwhose == USER && whose == USER)
			beginnendenv(usernewlines)
		if (lchar_whose == MONK_DB && isspace(thischar)
			skip it < or skip last \n >
			continue
 */

textputc(ic, whose)
int	ic, whose;
{
	static int	lchar_whose = MONK_DB;
	static short	lchar_nl = TRUE;
	char	c;

	c = ic;
	if (whose != FORCE && lchar_nl == TRUE) {
		if (whose == U_TEXT) {
			switch(c) {
			case NEW_LINE:
				if (lchar_whose == U_TEXT)
					u_nl_nl();
				return;
			case BLANK:
			case TAB:
				u_nl_white(c);
				lchar_whose = U_TEXT;
				/* do not know what actual lastchar is */
				return;
			}
		} else
			/* monk can never put out successive new-lines */
			if (c == NEW_LINE)
				return;
	}
	fputc(c, stdout);
	lchar_nl = (c == NEW_LINE) ? TRUE : FALSE;
	lchar_whose = whose;
}

/* routines for handling new-line cases:
	check what i do when definitions are not found
	all touch env stack, but all should return to original environment */

/* do not use environment stack - saves time 
	must be SUB_LEVEL, so will not affect stack of definition instances */

void
u_nl_nl()
{
	static ENTRY	*entry;
	static int	tried = FALSE;

	if (entry == NULL) {
		if (tried == TRUE)
			return;
		if ((entry = hashfind(ENV_TABLE, U_NL_NL))
							== (ENTRY *) NULL) {
			warn_db(PR_FILENAME | PR_LINENUMBER,
				"Cannot find definition for %s\n", U_NL_NL);
			return;
		}
		tried = TRUE;
	}
	do_envir((struct environment *) NULL, (struct definition *)entry->data,
					SUB_LEVEL, NULL, FORCE);
}

void
u_nl_white(space_char)
char	space_char;
{
	static int	tried = FALSE;
	static ENTRY	*entry;
	static struct environment	*env;
	static struct value	*vstring;
	static char	*string;

	if (entry == NULL) {
		if (tried == TRUE)
			return;
		if ((entry = hashfind(ENV_TABLE, U_NL_WHITE))
							== (ENTRY *) NULL) {
			warn_db(PR_FILENAME | PR_LINENUMBER,
				"Cannot find definition for %s\n",
							U_NL_WHITE);
			return;
		}
		tried = TRUE;
		env = mk_envir((struct environment *) NULL,
			(struct definition *) entry->data, SUB_LEVEL, NULL);
		string = mymalloc(sizeof(char)*2);
		*string = space_char;
		*(string+1) = '\0';
		vstring = make_value(string);
	} else
		*string = space_char;
	do_subenv(env, (struct definition *) entry->data, vstring, NULL,
							END_NOW, FORCE);
}
