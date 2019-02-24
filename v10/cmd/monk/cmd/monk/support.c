#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"dbcompress.h"
#include	"rd.h"

#include	<sys/types.h>
#include	<sys/times.h>

FILE *
fopendb(filename, db_path,  mode, exit_on_fail)
char	*filename, *db_path, *mode;
int	exit_on_fail;
{
	char	tempname[BUFSIZ];

	strcpy(tempname, db_path);
	strcat(tempname, SLASH_STRING);	
	strcat(tempname, filename);
	/* Add to filelist maintained by fopenncheck */	
	return(fopenncheck(tempname, mode, FALSE, exit_on_fail));
}

/*VARARGS5*/
readncheck(database, lookfor, flags, format_string, s1, s2)
FILE	*database;
char	lookfor;
int	flags;
char	*format_string, *s1, *s2;
{
	int	i;

	if ((i = r_nonblank(database)) == EOF)
		return(EOF);
	if (i != lookfor) {
		if (flags & MESSAGE) {
			warn(flags, PR_FILENAME|PR_LINENUMBER, format_string,
									s1, s2);
			warn(flags, 0, ": expecting %c, read %c;", lookfor, i);
		}
		if (flags & GOBBLE) {
			if (flags & MESSAGE)
				warn(flags, 0, "\n\t\t\tgobbling til ");
			while ((i = mygetc(database)) != EOF && i != lookfor);
			if (flags & MESSAGE)
				if (i == EOF) {
					warn(flags, PR_LINENUMBER, "EOF\n");
				} else 
					warn(flags, PR_LINENUMBER, "\n");
			if (i == EOF)
				return(EOF);
		} else {
			myungetc(i, database);
			warn(flags, PR_LINENUMBER, "\n");
		}
		return(TRUE);
	}
	return(FALSE);
}

hashncheck(hashtable, entry, type, mesg_mode)
int	hashtable;
ENTRY	*entry;
char	*type;
int	mesg_mode;
{
	ENTRY	*exist;

	if ((exist = hashfind(hashtable, entry->key)) != (ENTRY *) 0) {
		if (*entry->key != ':')
			warn(mesg_mode&U_REDEF_MSG, PR_FILENAME|PR_LINENUMBER,
				"Redefining %s %s\n", type, entry->key);
		exist->data = entry->data;
		return(FALSE);
	}
	hashenter(hashtable, entry);
	return(TRUE);
}

struct loop *
isitaloop(s, messages, mesg_mode)
char	*s;
short	messages;
int	mesg_mode;
{
	static struct loop	loop;
	struct valu_str	*sv;
	int	close_delim;

	if (strncmp(s, FOR_LOOP, strlen(FOR_LOOP)) != 0)
		return((struct loop *) 0);
	s += strlen(FOR_LOOP);
	while (*s != '0' && isspace(*s))
		++s;
	/* for loop variable must be single alphabetic character */
	loop.loopchar = *s++;
	if (!isalpha(loop.loopchar) || !isspace(*s)) {
		if (messages == TRUE)
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
			"Improper %s variable %c%c\n", FOR_LOOP, loop.loopchar, *s);
		return((struct loop *) 0);
	}
	while (*s != '0' && isspace(*s))
		++s;
	if (strncmp(s, FOR_ARGS, strlen(FOR_ARGS)) != 0) {
		if (messages == TRUE)
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
			"Improper %s syntax - expecting %s\n", FOR_LOOP, FOR_ARGS);
		return((struct loop *) 0);
	}
	s += strlen(FOR_ARGS);
	/* get list of arguments to loop */
	sv = parse_values(s);
	loop.args = sv->value;
	for (s=sv->newp; *s != '\0' && (close_delim=close_match(*s)) == FALSE; ++s)
		if (!isspace(*s)) {
			if (messages == TRUE)
				warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
				"Improper %s syntax - expecting open delim\n",
									FOR_LOOP);
			return((struct loop *) 0);
		}
	loop.current = (struct value *) 0;
	loop.close_delim = close_delim;
	loop.newp = s+1;
	return(&loop);
}

/* ZZZ: merge messages and mesg_mode as in warn - MESSAGE | mesg_mode in call */
struct loop *
readaloop(database, messages, mesg_mode)
FILE	*database;
short	messages;
int	mesg_mode;
{
	static char	buf[BUFSIZ];
	struct loop	*ok;
	int	i;
	char	*p, *pe;
/* DARN kludge ------ this is ridiculous - now need to put
				FOR_LOOP back into buffer ---
				must straighten out who reads */
	/* XZZZZZX */
	strcpy(buf, FOR_LOOP);
	p = buf + strlen(FOR_LOOP);
	*p++ = ' ';
	for (pe = &buf[BUFSIZ]; (i = mygetc(database)) != EOF && p < pe;) {
		*p = i;
		if (close_match(*p++) != FALSE)
			break;
	}
	*p = '\0';
/* ZZZZZZZZZZZZZZZZZZZZZZ hey - wait: no assurance that myungets will work! */
	if ((ok = isitaloop(buf, messages, mesg_mode)) == (struct loop *) 0)
		myungets(buf, database);
	return(ok);
}

char	*
copy_text_loop(q, loop)
char	*q;
struct loop	*loop;
{
	char	s[2];
	
	/* put down for loop itself */
	strcpy(q, FOR_LOOP);
	strcat(q, BLANKSTRING);
	s[0] = loop->loopchar;
	s[1] = '\0';
	strcat(q, s);
	strcat(q, FOR_ARGS);
	strcat(q, BLANKSTRING);
	strcat(q, loop->current->value);
	q += strlen(q);
	return(q);
}

copy_struct_loop(origloop, newloop)
struct loop	*origloop, *newloop;
{
	newloop->loopchar = origloop->loopchar;
	newloop->args = origloop->args;
	newloop->current = origloop->current;
	newloop->close_delim = origloop->close_delim;
	newloop->newp = origloop->newp;
}

struct cond_def_el *
readacond(database, values, type, mesg_mode)
FILE	*database;
struct value	*values;
int	type, mesg_mode;
{
	struct cond_def_el	*cdl;
	struct attrib_info	*z;
	ENTRY	*att;
	char	*p;

	if ((p = r_buftoken(database, OK_SPACE)) == (char *) 0)
		return((struct cond_def_el *) 0);
	if ((att = hashfind(ATT_TABLE, p)) == (ENTRY *) 0) {
		notstacking(database, values, type, p, mesg_mode);
		return((struct cond_def_el *) 0);
	}
	z = (struct attrib_info *) att->data;
	if (z->stacking == FALSE) {
		notstacking(database, values, type, p, mesg_mode);
		return((struct cond_def_el *) 0);
	}
	cdl = (struct cond_def_el *) bufmalloc(B_COND_DEF_EL,
						sizeof(struct cond_def_el));
	cdl->attribute = p;
	cdl->type = type;
	cdl->value = read_values(database, FALSE, NULL);
	cdl->next_on_fail = (struct def_element *) 0;
	return(cdl);
}

void
notstacking(database, values, type, p, mesg_mode)
FILE	*database;
struct value	*values;
int	type;
char	*p;
int	mesg_mode;
{
	int	close_delim, c;
	char	*ifvalue;

	ifvalue = (type == TRUE) ? IF_VALUE : IF_NOTVALUE;
	warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
		"Nonstacking variable %s can't be tested using %s\n",p,ifvalue);
	read_values(database, TRUE, NULL);
	read_delim(database, FALSE);
	if ((close_delim = read_delim(database, FALSE)) == EOF)
		close_delim = FALSE;
	trans_list(database, values, close_delim, mesg_mode);
	/* allow ITEM_SEP, tho not necessary */
	if ((c = mygetc(database)) != ITEM_SEP)
		myungetc(c, database);
}

struct conditional *
isitaconditional(s, messages, mesg_mode)
char	*s;
short	messages;
int	mesg_mode;
{
	static struct conditional	cond;
	struct strings	*strings;
	struct valu_str	*vs;
	register char	*p;

	if ((strings = parse_token(s)) == (struct strings *) 0)
		return((struct conditional *) 0);
	if (strcmp(strings->token, IF_VALUE) == 0)
		cond.type = TRUE;
	else
		if (strcmp(strings->token, IF_NOTVALUE) == 0)
			cond.type = FALSE;
		else {
			myfree(strings->token, 30);
			return((struct conditional *) 0);
		}
	myfree(strings->token, 31);
	/* if spew == TRUE, parse and check if attribute given matches state */
	if ((strings = parse_token(strings->newp)) == (struct strings *) 0) {
		if (messages == TRUE)
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
			"Found null attribute_name after %s or %s\n");
		return((struct conditional *) 0);
	}
	vs = parse_values(strings->newp);
	for (p = vs->newp; *p != '\0' && isspace(*p); ++p);
	if ((cond.close_delim = close_match(*p)) == (char) FALSE) {
		if (messages == TRUE)
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
			"Looking for open delim after %s or %s; found %c\n",
						IF_VALUE, IF_NOTVALUE, *p);
		free_valu(vs->value);
		return((struct conditional *) 0);
	}
	cond.attribute = strings->token;
	cond.value = vs->value;
	cond.newp = p+1;
	return(&cond);
}

/* copy_def_el:
	creating copy of def_element list so that links and/or values
		can be changed (not copying attributes, copying the linked
		list of values)
 */

struct def_element	*
copy_def_el(el)
struct def_element	*el;
{
	struct def_element	*firstel, *newel;

	if (el == (struct def_element *) 0)
		return(el);
	firstel = (struct def_element *) 0;
	do {
		if (firstel == (struct def_element *) 0)
			firstel = newel = (struct def_element *)
			bufmalloc(B_DEF_EL,sizeof(struct def_element));
		else {
			newel->next = (struct def_element *)
			bufmalloc(B_DEF_EL,sizeof(struct def_element));
			newel = newel->next;
		}
		newel->attribute = el->attribute;
		newel->value = copy_valu(el->value);
		newel->cdl = el->cdl;
		newel->troff = el->troff;
		newel->allocated = FALSE;
		newel->stacking = el->stacking;
		newel->next = (struct def_element *) 0;
	} while ((el = el->next) != (struct def_element *) 0);
	return(firstel);
}

struct value *
copy_valu(v)
struct value	*v;
{
	struct value	*firstv, *newv;

	if (v == (struct value *) 0)
		return(v);
	firstv = (struct value *) 0;
	do {
		if (firstv == (struct value *) 0)
			firstv = newv = (struct value *)
				bufmalloc(B_VALUE, sizeof(struct value));
		else {
			newv->next = (struct value *)
				bufmalloc(B_VALUE, sizeof(struct value));
			newv = newv->next;
		}
		/* why not stroing same pointer ? */
		newv->value = bufmalloc(B_TEXT, (unsigned) strlen(v->value)+1);
		strcpy(newv->value, v->value);
		newv->type = v->type;
		newv->next = (struct value *) 0;
	} while ((v = v->next) != (struct value *) 0);
	return(firstv);

}

struct def_element *
mk_def_el(init_def, a)
struct init_def	*init_def;
struct add_att	*a;
{
	struct def_element	*newel;

	newel = (struct def_element *)
			bufmalloc(B_DEF_EL, sizeof(struct def_element));
	newel->attribute = init_def->attribute;
	newel->value = a->value;
	newel->cdl = (struct cond_def_el *) 0;
	newel->stacking = init_def->stacking;
	/* troff must be defined at this point; cannot be a define!!! */
	if (translate(newel, SETUP_TIME) == FALSE)
		return((struct def_element *) 0);
	newel->next = (struct def_element *) 0;
	return(newel);
}

/* merge lists of def_elements: substitute first occurence of an attribute from
	modify into define; if it does not already appear in define, add it to
	the end */

struct def_element *
merge_def_el(define, modify)
struct def_element	*define, *modify;
{
	struct def_element	*del, *lastdefine;
	char	*attribute;

	if (define == (struct def_element *) 0)
		return(modify);
	if (modify == (struct def_element *) 0)
		return(define);
	for (del = define, lastdefine = NULL;;) {
		attribute = modify->attribute;
		define = del;
		do {
			if (strcmp(define->attribute, attribute) == 0) {
				if (define == del) {
					define = modify;
					modify = modify->next;
					define->next = del->next;
					buffree(del, 5);
					del = define;
				} else {
					lastdefine->next = modify;
					modify = modify->next;
					lastdefine->next->next = define->next;
					buffree(define, 6);
				}
			}
			lastdefine = define;
		} while ((define = define->next) != (struct def_element *) 0);
		lastdefine->next = modify;
		if ((modify = modify->next) != (struct def_element *) 0)
			modify->next = (struct def_element *) 0;
		else
			break;
	}
	return(del);
}

short
cmp_valu(v1, v2)
struct value	*v1, *v2;
{
	if (v1 == (struct value *) 0) {
		if (v2 == (struct value *) 0)
			return(TRUE);
		else
			return(FALSE);
	}
	if (v2 == (struct value *) 0)
		return(FALSE);
	do {
		if (strcmp(v1->value, v2->value) != 0)
			break;
		v1 = v1->next;
		v2 = v2->next;
	} while (v1 != (struct value *) 0 && v2 != (struct value *) 0);
	if (v1 != v2)
		return(FALSE);
	return(TRUE);
}

isdefined(token)
char	*token;
{
	if (hashfind(ENV_TABLE, token) == (ENTRY *) NULL)
		return(FALSE);
	return(TRUE);
}

create_buffer(buffer)
struct buffer	*buffer;
{
	buffer->start = buffer->current = buffer->empty =
						mymalloc((unsigned)BUFSIZ);
	buffer->end = buffer->start + BUFSIZ-1;
}

grow_buffer(buffer)
struct buffer	*buffer;
{
	unsigned	size;
	char	*newstart;

	if ((size = buffer->end - buffer->start + 1 + BUFSIZ) > 10 * BUFSIZ)
		return(FALSE);
	newstart = myrealloc(buffer->start, size);
	buffer->current = newstart + (buffer->current - buffer->start);
	buffer->empty = newstart + (buffer->empty - buffer->start);
	buffer->start = newstart;
	buffer->end = newstart + size-1;
	return(TRUE);
}

free_buffer(buffer)
struct buffer	*buffer;
{
	myfree(buffer->start, 34);
}

free_def_el(el)
struct def_element	*el;
{
	struct value	*v;

	if (el == (struct def_element *) 0)
		return;
	do {
		buffree(el, 7);
		if ((v = el->value) == (struct value *) 0)
			continue;
		do {
			buffree(v, 8);
		} while ((v = v->next) != (struct value *) 0);
	} while ((el = el->next) != (struct def_element *) 0);
}

free_state(s)
struct state	*s;
{
	if (s->allocated)
		free_valu(s->value);
	myfree(s, 117);
}

free_valu(v)
struct value	*v;
{

	if (v != (struct value *) 0)
		do {
			buffree(v->value, 9);
			buffree(v, 10);
		} while ((v = v->next) != (struct value *) 0);
}

free_attlist(a)
struct add_att	*a;
{
	if (a != (struct add_att *) 0)
		do {
			myfree(a, 11);
		} while ((a = a->next) != (struct add_att *) 0);
}

free_cond(cond)
struct conditional	*cond;
{
	if (cond != (struct conditional *) 0) {
		myfree(cond->attribute, 19);
		free_valu(cond->value);
	}
}

free_env(env)
struct environment	*env;
{
	register struct state	*state;

	if (env == (struct environment *) 0)
		return;
	if ((state = env->state_list) != (struct state *) 0)
		do {
			if (state->allocated == TRUE)
				free_valu(state->value);
			myfree(state, 14);
		} while ((state = state->next) != (struct state *) 0);
	myfree(env, 15);
}
