#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"
#include	"dbcompress.h"

/* for each definition:
	copy begin and end lists, translate to troff equivalents */

void
read_define(database, name, type, close_delim, mesg_mode)
FILE	*database;
char	*name;
short	type;
int	close_delim, mesg_mode;
{
	ENTRY	entry;
	ENTRY	*ok;

	/* get key, which is the name of item being defined */
	entry.key = name;
	entry.data = (char *) read_list(database, name, (struct value *) 0,
				type, close_delim, mesg_mode);
	/* if redefinition, could set key to NULL if bufmalloced in compress */
	if (hashncheck(ENV_TABLE, &entry, ASSOC_ENV, mesg_mode) == TRUE) {
		ok = (ENTRY *) bufmalloc(B_HASH_ENV, sizeof(ENTRY));
		ok->key = entry.key;
		ok->data = entry.data;
	}
#ifdef DEBUG_ENVIRONMENT
	pr_def((struct definition *) entry.data);
#endif
}

struct definition	*
read_list(database, name, values, assoc, close_delim, mesg_mode)
FILE	*database;
char	*name;
struct value	*values;
short	assoc;
int	close_delim, mesg_mode;
{
	static char	*blankstring;
	struct definition	*def, *d;
	struct value	*v;
	char	inner_delim;
	char	*assoc_name;

	def = (struct definition *)bufmalloc(B_DEF, sizeof(struct definition));
	def->name = name;
	def->values = values;
	def->sub_def = (struct definition *) 0;
	def->instance = (struct environment *) 0;
	def->begin_def = trans_list(database, values, LIST_SEP, mesg_mode);
	if (assoc == FALSE) {
		def->end_def=trans_list(database,values,close_delim,mesg_mode);
		return(def);
	}
	def->end_def = trans_list(database, values, LIST_SEP, mesg_mode);
	for (d=def, v = (struct value *)0;
	    (inner_delim = read_delim(database, TRUE)) != (char) FALSE;
	    d = d->sub_def) {
		/* BLANKSTRING must be stored for compression routines;
			no use nor harm otherwise... */
		if (blankstring == NULL) {
			blankstring = bufmalloc(B_TEXT,
					(unsigned) strlen(BLANKSTRING)+1);
			strcpy(blankstring, BLANKSTRING);
		}
		assoc_name = blankstring;
		/* default case = associate with null arguments (v == 0) */
		v = read_values(database, TRUE, inner_delim);
		readncheck(database, LIST_SEP, MESSAGE|mesg_mode,
				"Reading %s %s", name, str_avalue(v, TRUE));
		/* ZZZ need a bubble case:
			not just matching name and pluncking in order
		*/
		d->sub_def = read_list(database, assoc_name, v, FALSE,
							inner_delim, mesg_mode);
	}
	readncheck(database, close_delim, MESSAGE|mesg_mode,
			"Reading closing %s %s", name, str_avalue(v, TRUE));
	return(def);
}

/* translist:
	read list of attributes with values, storing it at buffer->current;
	translate each attribute in list into troff equivalent
	generates linked list of def_elements, each one corresponding to
	an attribute
 */

struct def_element *
trans_list(database, values, delim, mesg_mode)
FILE	*database;
struct value	*values;
char	delim;
int	mesg_mode;
{
	struct def_element	*d;
	struct cond_def_el	*cond_def_el, *oldcond_def_el;
	struct loop	*loop;
	struct loop	copyloop;
	struct def_element	*firstd, *lv;
	int	c, close_delim, type;
	char	*p;

	for (firstd = (struct def_element *) 0,
				cond_def_el = (struct cond_def_el *) 0; ; ) {
		if ((p = r_buftoken(database, OK_SPACE)) == (char *) 0) {
			if ((c = mygetc(database)) != EMBED_IN_TROFF) {
				myungetc(c, database);
			/* toss or continue ??? */
				readncheck(database, delim, MESSAGE|mesg_mode,
				"EReading %s %s", ASSOCIATE, ENVIRONMENT);
				break;
			}
			/* can i know name here ??? */
			if ((p = read_token(database, NO_SPACE)) == (char *) 0){
				warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
				"Reading %s: `|' with null command\n",
						ASSOCIATE, ENVIRONMENT);
				continue;
			}
#ifdef MONK_VARIABLES
			/* MONKSAVE, MONKREMEMBER, FOR_LOOP or IF_[NOT]VALUE */
			if (strcmp(p, MONKSAVE) == 0 ||
						strcmp(p, MONKREMEMBER) == 0) {
				lv = (struct def_element *)
						bufmalloc(B_DEF_EL,
						sizeof(struct def_element));
				if (firstd == (struct def_element *) 0)
					d = firstd = lv;
				else {
					d->next = lv;
					d = d->next;
					if (cond_def_el != NULL) {
						cond_def_el->next_on_fail = d;
						cond_def_el = NULL;
					}
				}
				d->next = (struct def_element *) 0;
				d->cdl = (struct cond_def_el *) 0;
				d->attribute = p;
				if ((d->value=read_values(database, TRUE, NULL))
						== (struct value *) 0) {
					warn(mesg_mode,
						PR_FILENAME | PR_LINENUMBER,
						"Null name to save/remember\n");
					continue;
				}
				if ((c = r_nonblank(database)) == delim)
					break;
				continue;
			}
#endif
			if (strcmp(p, FOR_LOOP) == 0 &&
				(loop = readaloop(database, TRUE, mesg_mode))
							!= (struct loop *) 0) {
				if (*loop->newp != '\0')
					warn(mesg_mode,
					PR_FILENAME | PR_LINENUMBER,
					"Problem in top of loop syntax\n");
				/* here loops cannot be embedded using
							same close_delim */
				copy_struct_loop(loop, &copyloop);
				lv = trans_list(database, values,
					copyloop.close_delim, mesg_mode);
				/* allow ITEM_SEP, tho not necessary */
				if ((c = mygetc(database)) != ITEM_SEP)
					myungetc(c, database);
				if ((lv = xpand_loop_valu(lv, &copyloop)) ==
								NULL) {
					warn(mesg_mode, 0,
					"Expanding %c: %s til %c to nothing\n",
						copyloop.loopchar,
						str_avalue(copyloop.args, TRUE),
						copyloop.close_delim);
					continue;
				}
				if (firstd == (struct def_element *) 0)
					d = firstd = lv;
				else {
					d->next = lv;
					d = d->next;
					if (cond_def_el != NULL) {
						cond_def_el->next_on_fail = d;
						cond_def_el = NULL;
					}
				}
				while ((lv = d->next) != NULL)
					d = lv;
				continue;
			}
			if (strcmp(p, IF_VALUE) == 0)
				type = TRUE;
			else
				if (strcmp(p, IF_NOTVALUE) == 0)
					type = FALSE;
				else {
					warn(mesg_mode,
					PR_FILENAME | PR_LINENUMBER,
					"%s do not provide a command %s\n",
						ASSOCIATE, p);
					continue;
				}
			oldcond_def_el = cond_def_el;
			cond_def_el = readacond(database,values,type,mesg_mode);
			if (cond_def_el == (struct cond_def_el *) 0)
				continue;
			if ((close_delim = read_delim(database, FALSE)) == EOF)
				close_delim = FALSE;
			lv = trans_list(database,values,close_delim,mesg_mode);
			/* allow ITEM_SEP, tho not necessary */
			if ((c = mygetc(database)) != ITEM_SEP)
				myungetc(c, database);
			if (lv == NULL) {
				warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
					"Empty %s\n",p, cond_def_el->attribute);
				cond_def_el = (struct cond_def_el *) 0;
				continue;
			}
			if (firstd == (struct def_element *) 0)
				d = firstd = lv;
			else {
				d->next = lv;
				d = d->next;
				if (oldcond_def_el != NULL)
					oldcond_def_el->next_on_fail=d;
			}
			d->cdl = cond_def_el;
			while ((lv = d->next) != NULL)
				d = lv;
			continue;
		}
		if (firstd == (struct def_element *) 0)
			d =firstd= (struct def_element *) bufmalloc(B_DEF_EL,
					 	sizeof(struct def_element));
		else {
			d->next = (struct def_element *) bufmalloc(B_DEF_EL,
						sizeof(struct def_element));
			d = d->next;
			if (cond_def_el != (struct cond_def_el *) 0) {
				cond_def_el->next_on_fail = d;
				cond_def_el = (struct cond_def_el *) 0;
			}
		}
		d->next = (struct def_element *) 0;
		d->cdl = (struct cond_def_el *) 0;
		d->special = 0;
		d->attribute = p;
		d->value = read_values(database, TRUE, NULL);
		rename_values(d->value, values);
		if (translate(d, SETUP_TIME) == FALSE) {
		/* may be improperly entered attribute or a define 
			- during development, warn_db 
			- need to make list of undefined values and check them
				when databases are completely read!!! */
			if (d->value != (struct value *) 0)
				warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
					"No attribute found for %s %s\n",
					d->attribute,str_avalue(d->value,TRUE));
			else
				missing_def(d->attribute);
		}
	/* doesn't check what character separating items in list is */
		if ((c = r_nonblank(database)) == delim)
			break;
		if (c != ITEM_SEP)
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
				"Improper item separator %c in list\n", c);
		/* Health: spit back any `|'. */
	}
	if (c == EOF) {
		free_def_el(firstd);
		return((struct def_element *) 0);
	}
	return(firstd);
}

struct def_element *
xpand_loop_valu(del, loop)
struct def_element	*del;
struct loop	*loop;
{
	struct def_element	*firstl, *l;
	struct value		*arg;
	char	*newtext;

	firstl = (struct def_element *) 0;
	for (arg = loop->args; arg != (struct value *) 0; arg = arg->next) {
		if (firstl == (struct def_element *) 0)
			firstl = l = copy_def_el(del);
		else {
			l->next = copy_def_el(del);
			l = l->next;
		}
		for (loop->current = arg; ; l = l->next) {
			if ((newtext=fillin_loop_valu(l->troff, loop))
								!= l->troff){
				l->troff = newtext;
				l->allocated = TRUE;
			}
			if (l->next == (struct def_element *) 0)
				break;
		}
	}
	return(firstl);
}

char	*
fillin_loop_valu(text, loop)
char	*text;
struct loop	*loop;
{
	short	change;
	char	*p, *q, *newtext;
	char	buf[BUFSIZ];

	for (p = text, q = buf, change = FALSE; *p != '\0'; )
		if (*p == DEF_ARGUMENT && *(p+1) == loop->loopchar) {
			change = TRUE;
			strcpy(q, loop->current->value);
			q += strlen(q);
			p += 2;
		} else
			*q++ = *p++;
	if (change == TRUE) {
		*q = '\0';
		newtext = bufmalloc(B_TEXT, (unsigned) strlen(buf)+1);
		strcpy(newtext, buf);
		return(newtext);
	}
	return(text);
}

/* rename each variable argument in args using the choices in valuelist */
rename_values(arglist, valuelist)
struct value	*arglist, *valuelist;
{
	struct value	*arg, *v;
	short	n;
	static char	argpos[STR_SIZE];
	char	*newstr;

	for (v = valuelist, n = 0; v != (struct value *) 0; v = v->next) {
		if (++n > 9) {
			warn_db(PR_FILENAME | PR_LINENUMBER,
			"Limit of nine arguments exceeded: ignoring extras\n");
			break;
		}
		newstr = (char *) 0;
		for (arg = arglist; arg != (struct value *) 0; arg = arg->next)
			if ((arg->type == VAR_ARG || arg->type == REGEX_ARG)
					&& strcmp(arg->value, v->value) == 0){
				if (newstr == (char *) 0) {
					sprintf(argpos, "%c%d", DEF_ARGUMENT,n);
					newstr = bufmalloc(B_TEXT,
						(unsigned) strlen(argpos)+1);
					strcpy(newstr, argpos);
				}
				arg->value = newstr;
			}
	}
}
