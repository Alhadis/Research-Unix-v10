#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"file_info.h"
#include	"rd.h"
#include	"dbcompress.h"

/* read_attribute:
	add an attribute to hashtable, to state stack if stacking variable,
	and to initialize definition if a default is given
 */

char	*init_troff, *i_stack;

void
read_attribute(database, name, close_delim, mesg_mode)
FILE	*database;
char	*name;
int	close_delim, mesg_mode;
{
	register ENTRY	att_entry;
	ENTRY	*ok;
	struct attrib_info	*att_new;
	struct init_def	*init_def;
	struct value	*att_values;
	char	*att_format;

	if ((init_def = add_att_type(database, name, mesg_mode))
						== (struct init_def *) EOF)
		return;
	att_entry.key = name;
	att_entry.data=bufmalloc(B_ATT_INFO,sizeof(struct attrib_info));
	att_new = (struct attrib_info *) att_entry.data;
	att_new->stacking = init_def->stacking;
	att_new->firstcase = (struct attrib_case *) 0;
	for (att_format = (char *) 0;
		(att_values = read_values(database, FALSE, close_delim))
						!= (struct value *)EOF;) {
		att_format = r_fmtcommands(database,name,att_values,mesg_mode);
		if (att_format == (char *) 0)
			break;
		make_case(att_new, att_format, att_values, mesg_mode);
	}
	if (att_values == (struct value *) EOF || att_format == (char *) EOF)
		return;
	readncheck(database, close_delim, MESSAGE | mesg_mode, "Reading %s %s",
							ATTRIBUTE, name);
	if (hashncheck(ATT_TABLE, &att_entry, ATTRIBUTE, mesg_mode) == TRUE) {
		ok = (ENTRY *) bufmalloc(B_HASH_ATT, sizeof(ENTRY));
		ok->key = att_entry.key;
		ok->data = att_entry.data;
	}
	add_init(init_def, mesg_mode);
#ifdef DEBUG_ATTRIBUTE
	pr_att(&att_entry, TRUE);
#endif
}

char *
r_fmtcommands(database, name, att_values, mesg_mode)
FILE	*database;
char	*name;
struct value	*att_values;
int	mesg_mode;
{
	struct buffer	buffer;
	struct file_info	*fi;
	short	len;
	char	inner_close;
	char	*store_format;

	/* not an open delim->end of attribute definition */
	if ((inner_close = read_delim(database, TRUE)) == (char) FALSE)
		return((char *) 0);
	fi = save_file_info();
	create_buffer(&buffer);
	if (r_fmt(database, &buffer, inner_close, FALSE, mesg_mode) != TRUE) {
		warn(mesg_mode, 0,
			" in Attribute-case %s%s starting at line %d\n",
			name, str_avalue(att_values, TRUE), fi->line_number);
		free_buffer(&buffer);
		return((char *) 0);
	}
	map_args(buffer.start, att_values, mesg_mode);
	len = strlen(buffer.start)-1;
	store_format = bufmalloc(B_TEXT, (unsigned) len+1);
	strncpy(store_format, buffer.start, len);
	store_format[len] = '\0';
	free_buffer(&buffer);
	return(store_format);
}

r_fmt(database, b, delim, checkopen, mesg_mode)
FILE	*database;
struct buffer	*b;
char	delim;
short	checkopen;
int	mesg_mode;
{
	struct conditional	*cond;
	struct loop	*loop;
	short	ok;
	char	inner_delim;

	for (;;) {
		if (b->current == b->empty)
			if ((ok = read_into_til(database, b, delim, mesg_mode))
									!= TRUE)
				return(ok);
		if (*b->current == delim) {
			++b->current;
			return(TRUE);
		}
		if (checkopen == TRUE) {
			if ((inner_delim = close_match(*b->current++)) != FALSE)
				if ((ok=read_into_til(database, b, inner_delim,
							mesg_mode)) != TRUE)
					return(ok);
			continue;
		}
		if (*b->current++ != EMBED_IN_TROFF)
			continue;
		if ((loop = isitaloop(b->current, TRUE, mesg_mode))
						!= (struct loop *) 0) {
			if((ok = read_inner(database, b, loop->newp,
					loop->close_delim, mesg_mode)) != TRUE)
				return(ok);
			continue;
		}
		if ((cond = isitaconditional(b->current, TRUE, mesg_mode))
						!= (struct conditional *) 0) {
			if((ok = read_inner(database, b, cond->newp,
					cond->close_delim, mesg_mode)) != TRUE)
				return(ok);
			continue;
		}
	}
}

read_inner(database, b, newp, delim, mesg_mode)
FILE	*database;
struct buffer	*b;
char	*newp;
char	delim;
int	mesg_mode;
{
	short	ok;

	b->current = newp;
	/* ZZZZ - checkopen??? TRUE or FALSE what for???
			was TRUE */
	if((ok=r_fmt(database, b, delim, FALSE, mesg_mode)) != TRUE)
		return(ok);
	return(ok);
}

read_into_til(database, b, delim, mesg_mode)
FILE	*database;
struct buffer	*b;
char	delim;
int	mesg_mode;
{
	int	i;
	register char	*s;
	char	*se;

	for (s = b->empty, se = b->end;
			(i = mygetc(database)) != EOF && (*s=i) != delim; )
		if (++s > se) {
			b->empty = s;
			if (grow_buffer(b) == FALSE) {
				warn_db(PR_FILENAME,
			"Filled %db buffer searching for delim %c\n\t\t",
							10*BUFSIZ, delim);
				warn(mesg_mode, PR_FILENAME,
			"Filled %db buffer searching for delim %c\n\t\t",
							10*BUFSIZ, delim);
				return(FALSE);
			}
			s = b->empty;
		}
	if (i == EOF) {
		warn(mesg_mode, PR_FILENAME,
			"Reached EOF searching for delim %c\n\t\t", delim);
		return(EOF);
	}
	*++s = '\0';
	b->empty = s;
	return(TRUE);
}

void
make_case(attrib_info, text, v, mesg_mode)
struct attrib_info	*attrib_info;
char	*text;
struct value	*v;
int	mesg_mode;
{
	char	*regcmp(), *strchr();
	struct attrib_case	*newcase;
	int	ifile, iline, pfile, len;
	char	*p;

	newcase = (struct attrib_case *)
			bufmalloc(B_ATT_CASE, sizeof(struct attrib_case));
	newcase->expanding = newcase->nvalues = 0;
	newcase->value = v;
	if (v != (struct value *) 0)
		do {
			newcase->nvalues++;
			if (v->type == REGEX_ARG) {
				/* compile argument which follows 2 chars:
					DEF_ARGUMENT and REGEX */
				if ((p = regcmp(&v->value[2], 0)) == 0) {
					warn(mesg_mode,
					PR_FILENAME | PR_LINENUMBER,
					"Omitting attribute-case: ");
					warn(mesg_mode,0,
					"can't compile regular expression %s\n",
					v->value);
					buffree(newcase, 40);
					return;
				} else {
					/* Health? HMMMMMM */
					len = strlen(p);
					v->value = bufmalloc(B_TEXT,
							(unsigned) len+1);
					strncpy(v->value, p, len);
				}
			} else
				if (v->type == XPAND_ARG)
					newcase->expanding = newcase->nvalues;
		} while ((v = v->next) != (struct value *) 0);
	newcase->special = FALSE;
	newcase->troff = p = text;
	if ((p = strchr(p, SPECIAL_IN_TROFF)) != 0) {
		ifile = strlen(FILENAME);
		iline = strlen(LINENUMBER);
		pfile = strlen(PARENT);
		do {
			/* look for special cookies, e.g. __FILE__, __LINE__ */
			if (strncmp(p, FILENAME, ifile) == 0) {
				newcase->special = TRUE;
				break;
			}
			if (strncmp(p, LINENUMBER, iline) == 0) {
				newcase->special = TRUE;
				break;
			}
			if (strncmp(p, PARENT, pfile) == 0) {
				newcase->special = TRUE;
				break;
			}
		} while ((p = strchr(++p, SPECIAL_IN_TROFF)) != 0);
	}
	newcase->next = (struct attrib_case *) 0;
	bubble_case(attrib_info, newcase);
}

/* map_args:
	maps attribute arguments from those used in database to $1, $2, ...
	expands for.loops with fixed arguments, copies those with expanding args
 */

/* REALLY should pass struct buffer and put in overrun protection for text */

void
map_args(text, values, mesg_mode)
char	*text;
struct value	*values;
int	mesg_mode;
{
	struct loop	*loop;
	struct value	*v;
	struct strings	*strings;
	register char	*p, *q;
	char	*begloop, *pe;
	char	buf[SAFESIZ];

	if (values == (struct value *) 0)
		return;
	loop = (struct loop *) 0;
	for (p=text, q=buf, pe=text+strlen(text); p < pe; ) {
		if (*p == DEF_ARGUMENT
				&& (strings = rename_arg(p, values, loop))
						!= (struct strings *) 0){
			strcpy(q, strings->token);
			q += strlen(strings->token);
			p = strings->newp;
			continue;
		}
		if (*p != EMBED_IN_TROFF ||
			(loop=isitaloop(p,FALSE,mesg_mode))==(struct loop *)0){
			*q++ = *p++;
			continue;
		}
		if ((p = loop->newp) == '\0')
			continue;
		/* Expand loops for all fixed or variable loop parameters
				(must expand $* for each instance) */
		for (begloop=p, v=loop->args; v!=(struct value *) 0; v=v->next){
			loop->current = v;
			if (v->type == XPAND_ARG) {
				q = copy_text_loop(q, loop);
				*q = *(begloop-1);
			}
			for (p = begloop+1; *p != loop->close_delim;)
				if (*p == DEF_ARGUMENT
					&& (strings = rename_arg(p,values,loop))
							!= (struct strings *)0){
						strcpy(q, strings->token);
						q += strlen(strings->token);
						p = strings->newp;
				} else
					*q++ = *p++;
			if (v->type == XPAND_ARG)
				*q++ = loop->close_delim;
		}
		++p;
	}
	*q = '\0';
	strcpy(text, buf);
}

struct strings	*
rename_arg(s, values, loop)
char	*s;
struct value	*values;
struct loop	*loop;
{
	struct strings	*strings;

	if (*(s+1) == '*')
		return((struct strings *) 0);
	if ((strings = parse_token(s)) == (struct strings *) 0)
		return((struct strings *) 0);
	/* does it match one of the values in the list */
	if (doesargmatch(strings, values) == TRUE)
		return(strings);
	if (loop != (struct loop *) 0 && *(s+1) == loop->loopchar) {
		strings->token = loop->current->value;
		strings->newp = s+2;
		return(strings);
	}
	/* Kludge: becasue now there are no delims for argument_tags,
		drop last character working back to check for match of names
		of less than the full token length - this allows the `u' for
		units to sit up against the argument */
	while (--strings->newp > s) {
		strings->token[strlen(strings->token)-1] = '\0';
		if (doesargmatch(strings, values) == TRUE)
			return(strings);
	}
	return((struct strings *) 0);
}

doesargmatch(strings, values)
struct strings	*strings;
struct value	*values;
{
	static char	argpos[STR_SIZE];
	short	n;

	for (n = 0; values != (struct value *) 0; values = values->next) {
		/* BAD - no good reason for this limit of 9..... */
		if (++n > 9) {
			/* ZZZ: should use mesg_mode;but limit should go AWAY */
			warn_db(PR_FILENAME | PR_LINENUMBER,
			"Limit of nine arguments exceeded: ignoring extras\n");
			break;
		}
		if ((values->type == VAR_ARG || values->type == REGEX_ARG)
				&& strcmp(strings->token, values->value) == 0) {
			sprintf(argpos, "%c%d", DEF_ARGUMENT, n);
			strings->token = argpos;
			return(TRUE);
		}
	}
	return(FALSE);
}

/* bubble_case:
	rule for case ordering
		All non-expanding cases, in order of increasing #arguments
		All expanding cases, in order of decreasing #arguments
 */

void
bubble_case(att_info, newcase)
struct attrib_info	*att_info;
struct attrib_case	*newcase;
{
	struct attrib_case	*lastpos, *pos;
	struct value	*v;
	short	fixed, regexp;

	fixed = regexp = 0;
	if ((v = newcase->value) != (struct value *) 0)
		do {
			if (v->type == FIXED_ARG)
				fixed++;
			else if (v->type == REGEX_ARG)
				regexp++;
		} while ((v = v->next) != (struct value *) 0);
	pos = att_info->firstcase;
	if (nextcase(newcase, pos, fixed, regexp) == TRUE) {
		att_info->firstcase = newcase;
		newcase->next = pos;
		return;
	}
	for (lastpos = pos; (pos = pos->next) != (struct attrib_case *) 0;
								lastpos = pos)
		if (nextcase(newcase, pos, fixed, regexp) == TRUE) {
			lastpos->next = newcase;
			newcase->next = pos;
			return;
		}
	lastpos->next = newcase;
}

/* nextcase:	true if case to be added precedes thiscase */

nextcase(addcase, thiscase, addfixed, addregexp)
struct attrib_case	*addcase, *thiscase;
short	addfixed, addregexp;
{
	struct value	*v;
	short	thisfixed, thisregexp;

	if (thiscase == (struct attrib_case *) 0)
		return(TRUE);
	thisfixed = thisregexp = 0;
	if ((v = thiscase->value) != (struct value *) 0)
		do {
			if (v->type == FIXED_ARG)
				thisfixed++;
			else if (v->type == REGEX_ARG)
				thisregexp++;
		} while ((v = v->next) != (struct value *) 0);
	if (addcase->expanding == FALSE) {
		if (thiscase->expanding == TRUE
					|| thiscase->nvalues > addcase->nvalues)
			return(TRUE);
		if (thiscase->nvalues == addcase->nvalues)
			if (thisfixed < addfixed || thisregexp < addregexp)
				return(TRUE);
	} else
		if (thiscase->expanding == TRUE) {
			if (thiscase->nvalues < addcase->nvalues)
				return(TRUE);
		if (thiscase->nvalues == addcase->nvalues)
			if (thisfixed < addfixed || thisregexp < addregexp)
				return(TRUE);
		}
	return(FALSE);
}

/* add_att_type:
	if stacking attribute, adds to statestack hashtable
	if default specified, modifies internal initialize definition
 */

struct init_def	*
add_att_type(database, name, mesg_mode)
FILE	*database;
char	*name;
int	mesg_mode;
{
	static struct init_def	*init_def;
	struct add_att	*a;
	short	hasdefault, i;
	char	*p;

/* interesting: when a definition is added at run-time, i do not want to
			bother with the initialization definitions.
			run these additions now
 */
	/* Again, can gobble ridiculous amounts if LIST_SEP missing... */
	if(init_def == (struct init_def *) 0) {
		init_def =(struct init_def *) mymalloc(sizeof(struct init_def));
	} else
		/*reuse allocated init, but free add_att structure list */
		free_attlist(init_def->add_att);
	hasdefault = FALSE;
	init_def->attribute = name;
	init_def->stacking = FALSE;
	init_def->add_att = (struct add_att *) 0;
	do {
		if ((p = read_token(database, OK_SPACE)) == (char *) EOF)
			return((struct init_def	*) 0);
		if (p == (char *) 0) {
			if (r_nonblank(database) != LIST_SEP)
				warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
				"%s attribute has no initialization field\n",
				name);
			break;
		}
		if (strcmp(p, STACKING) == 0) {
			init_def->stacking = TRUE;
			myfree(p, 1);
			continue;
		}
		if ((i=strcmp(p, INITIALIZE))==0 || strcmp(p, DEFAULT)==0){
			if (init_def->add_att == (struct add_att *) 0)
				a = init_def->add_att = (struct add_att *)
					mymalloc(sizeof(struct add_att));
			else {
				a->next = (struct add_att *)
					mymalloc(sizeof(struct add_att));
				a = a->next;
			}
			if (i == 0)
				a->defname = init_troff;
			else {
				a->defname = i_stack;
				hasdefault = TRUE;
			}
			a->value = read_values(database, FALSE, NULL);
			a->next = (struct add_att *) 0;
		}
	} while (r_nonblank(database) != LIST_SEP);
	if (init_def->stacking == TRUE) {
		if (init_def->add_att == (struct add_att *) 0)
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
				"Stacking attribute %s needs a default value\n",
				init_def->attribute);
	} else
		if (hasdefault == TRUE)
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
				"Nonstacking attribute %s cannot have a %s\n",
				init_def->attribute, DEFAULT);
	return(init_def);
}

add_init(init_def, mesg_mode)
struct init_def	*init_def;
int	mesg_mode;
{
	ENTRY	*entry;
	struct add_att	*a;
	struct def_element	*el;
	struct definition	*d;
	struct value	*v;

	if ((a = init_def->add_att) == (struct add_att *) 0)
		return;
	do {
		if ((el = mk_def_el(init_def, a)) == (struct def_element *) 0) {
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
					"%s: cannot find attribute-case %s",
					a->defname, init_def->attribute);
			if ((v = a->value) != (struct value *) 0)
				do
					warn(mesg_mode, 0, " %s", v->value);
				while ((v = v->next) != (struct value *) 0);
			warn(mesg_mode, 0, "\n");
			continue;
		}
		/* build initialize - get and update hashtable definition */
		if ((entry=hashfind(ENV_TABLE, a->defname)) == (ENTRY *) NULL) {
			entry = (ENTRY *)
					bufmalloc(B_HASH_ENV, sizeof(ENTRY));
			entry->key = a->defname;
			/* building begin_def for initialization */
			entry->data =
				bufmalloc(B_DEF, sizeof(struct definition));
			d = (struct definition *) entry->data;
			d->name = a->defname;
			d->values = (struct value *) 0;
			d->begin_def = el;
			d->end_def = (struct def_element *) 0;
			d->sub_def = (struct definition *) 0;
		} else {
			d = (struct definition *) entry->data;
			/* warning given if attribute redefined; no reason to
				warn at redefinition of default for attribute */
			d->begin_def = merge_def_el(d->begin_def, el);
		}
		hashenter(ENV_TABLE, entry);
	} while ((a = a->next) != (struct add_att *) 0);
}

init_dbread()
{
	i_stack = bufmalloc(B_TEXT, (unsigned) strlen(INIT_STACK)+1);
	strcpy(i_stack, INIT_STACK);
	init_troff = bufmalloc(B_TEXT, (unsigned) strlen(INIT_TROFF)+1);
	strcpy(init_troff, INIT_TROFF);
	mk_hashtables();
}
