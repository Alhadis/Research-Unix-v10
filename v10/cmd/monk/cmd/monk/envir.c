#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"file_info.h"
#include	"rd.h"

#define	PLUS_ZERO 	"+0"
#define	ZERO_STR	"0"

struct environment *
begin_envir(oldenv, hashkey, close_delim, textwrite)
struct environment	*oldenv;
char	*hashkey;
int	close_delim;
short	textwrite;
{
	struct definition	*def;
	struct environment	*env;

	def = get_envir(hashkey, close_delim, textwrite);
	if (def == (struct definition *) 0)
		return(oldenv);
	env = do_envir(oldenv, def, TOP_LEVEL, close_delim, textwrite);
	return(env);
}

struct definition *
get_envir(hashkey, close_delim, textwrite)
char	*hashkey;
int	close_delim;
short	textwrite;
{
	ENTRY	*entry;
	char	c;

	/* hashentry contains appropriate definition list */
	if ((entry = hashfind(ENV_TABLE, hashkey)) == (ENTRY *) NULL) {
		if (textwrite == U_TEXT) {
/* ZZ shouldn't all colon environments be called with textwrite == MONK_DB? */
			if (*hashkey != COLON)
				warn_user(PR_FILENAME | PR_LINENUMBER,
		"%s is not a monk command: use ``|sp(|)'' to put `|' in text\n",
								hashkey);
			textputc(BEGIN_PRIMITIVE, U_TEXT);
			textputs(hashkey, U_TEXT);
			if ((c = open_match(close_delim)) != (char) FALSE)
				textputc(c, U_TEXT);
		}
		return((struct definition *) 0);
	}
	return((struct definition *) entry->data);
}

struct environment *
do_envir(oldenv, def, level, close_delim, textwrite)
struct environment	*oldenv;
struct definition	*def;
int	level, close_delim;
short	textwrite;
{
	struct environment	*newenv;

	newenv = mk_envir(oldenv, def, level, close_delim);
	dodef(newenv, def->begin_def, textwrite);
	return(newenv);
}

struct environment *
mk_envir(oldenv, def, level, close_delim)
struct environment	*oldenv;
struct definition	*def;
int	level, close_delim;
{
	struct environment	*newenv;
	struct file_info	*fi;

	newenv = (struct environment *) mymalloc(sizeof(struct environment));
	newenv->def = def;
	newenv->state_list = (struct state *) 0;
	newenv->envname = def->name;
	if (level == TOP_LEVEL) {
		newenv->lastinstance = def->instance;
		def->instance = newenv;
	} else
		newenv->lastinstance = (struct environment *) SUB_LEVEL;
	newenv->previous = oldenv;
	fi = save_file_info();
	newenv->filename = fi->file_name;
	newenv->linenumber = fi->line_number;
	newenv->how_to_end = close_delim;
	return(newenv);
}

/* assoc_envir:
	execute any subenvironments called within the top-level environment
 */

struct environment *
assoc_envir(userfile, env, def, close_delim, endwhen, textwrite)
FILE	*userfile;
struct environment	*env;
struct definition	*def;
int	close_delim, endwhen;
short	textwrite;
{
	struct value	*v;
	int	found;
	char	c;

	if (def->sub_def == (struct definition *) 0)
		return(env);
	for (found=0; (v=read_values(userfile, TRUE, close_delim))
						!= (struct value *) 0;) {
		found = 1;
		env = do_subenv(env, def, v, close_delim, endwhen, textwrite);
		if ((c = r_nonblank(userfile)) == close_delim) {
			myungetc(c, userfile);
			break;
		}
	}
	if (found == 0)
		env = do_subenv(env, def, (struct value *) 0, close_delim,
							endwhen, textwrite);
	return(env);
}

struct environment *
do_subenv(env, def, v, close_delim, endwhen, textwrite)
struct environment	*env;
struct definition	*def;
struct value	*v;
int	close_delim, endwhen;
short	textwrite;
{
	struct definition	*d;

	if ((d = match_assoc(def->sub_def, v)) != (struct definition *) 0) {
		/* if there is a match (and there are no expanders)
				then can pass as definition values */
		d->values = v;
		env = do_envir(env, d, SUB_LEVEL, close_delim, textwrite);
		if (endwhen == END_NOW)
			env = end_envir(env, SUB_LEVEL);
	} else if (v != (struct value *) 0) {
		warn_user(PR_FILENAME | PR_LINENUMBER,
				"The %s command does not provide ``%s''\n",
				def->name, str_avalue(v, FALSE));
		textputs(str_avalue(v, FALSE), U_TEXT);
		/* run default case on failure */
		env = do_subenv(env, def, (struct value *) 0, close_delim,
							endwhen, textwrite);
	}
	return(env);
}

struct definition *
match_assoc(d, values)
struct definition	*d;
struct value	*values;
{
	struct value	*matchthese;
	short	nvalues;

	for (nvalues = 0, matchthese = values; values != (struct value *) 0;
							values = values->next)
		++nvalues;
	if (d != (struct definition *) 0) {
		do {
			if (match_arg(matchthese, d->values, nvalues) == TRUE)
				return(d);
		} while ((d = d->sub_def) != (struct definition *) 0);
		/* ZZnow - does it match with substituting arguments b4 match */
		
	}
	return((struct definition *) 0);
}

/* looking for good ideas for trying to recover from author errors.
	this attempts to handle without adding problems the case where
	one environment is not ended, or two adjacent ones are switched.
	On a mismatch, it will pop two environments if the previous one
	matches the key given.
	which cases will be aggravated?

env_health(env, key)
struct environment	*env;
char	*key;
{
}

 */

/* nstates loops through def_elements:
		writing troff strings to output file
		adding absolute value troff strings to state stack
	 */

void
dodef(newenv, define, textwrite)
struct environment	*newenv;
register struct def_element	*define;
short	textwrite;
{
	struct state	*add_to_stack();

	struct state	*nstate;
	struct statestack	*states;
	struct value	*definevalues;
	struct cond_def_el	*cdl;
	int	run_time_def = FALSE;
	ENTRY	*grabit;
	struct state	*tempstate;
	char	*modtext;

	if (define == (struct def_element *) 0)
		return;
	nstate = (struct state *) 0;
	definevalues = newenv->def->values;
	textwrite = (textwrite == U_TEXT) ? MONK_DB : textwrite;
	do {
		while ((cdl = define->cdl) != (struct cond_def_el *) 0) {
			if (iscondtrue(cdl->attribute,cdl->value,cdl->type))
				break;
			define = cdl->next_on_fail;
			if (define == (struct def_element *) 0)
				break;
		}
		if (define == (struct def_element *) 0)
				break;
#ifdef MONK_VARIABLES
ZZZ if monksave,remember
save = name (then if save != 0, call read utext til close_delim)
close_delim, end_envir
#endif
		if (define->troff == (char *) 0) {
			/* really should check for run-time definition of
				attribute only for assoc if () { */
			if (translate(define, RUN_TIME) == TRUE)
					run_time_def = TRUE;
			else
				use_define(newenv, define, textwrite);
			continue;
		}
		if (textwrite != NO_WRITE) {
			if (definevalues == (struct value *) 0
						&& define->special == FALSE)
				textputs(define->troff, textwrite);
			else {
				modtext = replace_args(define->troff,
						define->special, definevalues,
						newenv);
				textputs(modtext, textwrite);
				if (modtext != define->troff)
					myfree(modtext, 44);
			}
		}
		if (run_time_def == TRUE) {
			define->troff = (char *) 0;
			run_time_def = FALSE;
		}	
		if ((grabit = hashfind(STACK_TABLE, define->attribute))
							== (ENTRY *) NULL)
			continue;
		/* building list of states to undo at end of envir:
			first done is the last undone */
		if (nstate == (struct state *) 0) {
			nstate = (struct state *)
						mymalloc(sizeof(struct state));
			nstate->next = (struct state *) 0;
		} else {
			tempstate = (struct state *)
						mymalloc(sizeof(struct state));
			tempstate->next = nstate;
			nstate = tempstate;
		}
		nstate->attribute = define->attribute;
		states = (struct statestack *) grabit->data;
		nstate->previous = states->last;
	/* attribute values are not replaced with define_level values */
		nstate->value = define->value;
		nstate->allocated = FALSE;
		if ((tempstate = add_to_stack(states, nstate)) != nstate) {
			myfree(nstate);
			nstate = tempstate;
		}
	} while ((define = define->next) != (struct def_element *) 0);
	newenv->state_list = nstate;
}

	/* previous == previous instance of this stacking attribute */
	/* nstate == instance of this stacking attribute to put on stack */

struct state *
add_to_stack(statestack, nstate)
struct statestack	*statestack;
struct state	*nstate;
{
	struct state	*s, *s2;
	short	toadd;

	for (s = nstate->next; s != (struct state *) 0; s = s->next)
		if (strcmp(nstate->attribute, s->attribute) == 0) {
			toadd = add_values(s, nstate->value);
			if (toadd == FALSE ||
					match_state(s, statestack->last)==TRUE){
				if (s == (s2 = nstate->next)) {
					statestack->last = s->previous;
					return(s->next);
				}
				for (; s2 != (struct state *) 0; s2 = s2->next)
					if (s == s2->next) {
						s2->next = s->next;
						statestack->last = s->previous;
						/*free_state(s);*/
						break;
					}
			}
			return(nstate->next);
		}
	if (match_state(nstate, statestack->last) == TRUE)
		return(nstate->next);
	if (statestack->first = (struct state *) 0) {
		nstate->previous = (struct state *) 0;
		statestack->first = nstate;
	} else
		nstate->previous = statestack->last;
	statestack->last = nstate;
	return(nstate);
}	

match_state(s, s2)
struct state	*s, *s2;
{
	struct value	*v, *v2;

	if (s == (struct state *) 0 || s2 == (struct state *) 0)
		return(FALSE);
	/* attribute matches */
	if ((v = s->value) == (struct value *) 0) {
		if (s2->value == (struct value *) 0)
			return(TRUE);
		else
			return(FALSE);
	}
	if ((v2 = s2->value) == (struct value *) 0)
		return(FALSE);
	do {
		if (strcmp(v->value, v2->value) != 0)
			return(FALSE);
		if (*v->value == '+' || *v->value == '-')
			return(FALSE);
		/* don't check type - delayed arg matching for assoc */
		v = v->next;
		if ((v2 = v2->next) == (struct value *) 0)
			break;
	} while (v != (struct value *) 0);
	if (v == v2)
		return(TRUE);
	else
		return(FALSE);
}

use_define(env, define, textwrite)
struct environment	*env;
register struct def_element	*define;
short	textwrite;
{
	struct environment	*temp_env, *old_env;

	temp_env = begin_envir(env, define->attribute, 0, textwrite);
	if (temp_env != env) {
		old_env=end_envir(temp_env, TOP_LEVEL);
		if (env != old_env)
			warn_me(PR_FILENAME | PR_LINENUMBER,
				"PANIC: temp_env changed ongoing env stack\n");
	}
}

/* returns FALSE is all values are zero */
add_values(olds, newv)
register struct state	*olds;
register struct value	*newv;
{
	short	nonzero = FALSE;
	short	need_to_add = FALSE;
	char	*sum_value();

	struct value	*oldv, *v, *tmp;
	struct value	*orig = (struct value *) 0;

	if ((v = newv) == (struct value *) 0
			|| (oldv = olds->value) == (struct value *) 0) {
		olds->value = newv;
		return(TRUE);
	}
	do {
		if (*v->value == '+' || *v->value == '-') {
			need_to_add = TRUE;
			break;
		}
	} while ((v = v->next) != (struct value *) 0);
	if (olds->allocated == TRUE)
		orig = olds->value;
	if (need_to_add == FALSE) {
		olds->value = newv;
	/*	free_valu(orig); */
		return(TRUE);
	}
	do {
		tmp = (struct value *) mymalloc(sizeof(struct value));
		if (olds->value == oldv)	
			olds->value = tmp;
		else
			v->next = tmp;
		v = tmp;
		if (oldv != (struct value *) 0 &&
				(*newv->value == '+' || *newv->value == '-'))
			v->value = sum_value(oldv->value, newv->value);
		else {
			v->value = mymalloc((unsigned) strlen(newv->value)+1);
			strcpy(v->value, newv->value);
		}
		oldv = oldv->next;
		/* 0 may be significant; only +0 can be tossed */
		if (strcmp(v->value, PLUS_ZERO) != 0)
			nonzero = TRUE;
	} while ((newv = newv->next) != (struct value *) 0);
	v->next = (struct value *) 0;
	/* 
	free_valu(orig);
	if (nonzero == FALSE)
		free_valu(olds->value);	*/
	return(nonzero);
}

char *
sum_value(oldv, newv)
char	*oldv, *newv;
{
	char	*convn();

	short	arg;
	char	*p, *q;

	if ((arg = atoi(newv) + atoi(oldv)) == 0) {
		if (isdigit(*oldv))
			return(ZERO_STR);
		else
			return(PLUS_ZERO);
	}
	p = convn(arg, 10);
	/* If in the environment state list (to restore at end) an attribute
		repeats, then combine: if the old value is an absolute number,
		restore the previous value:
	old	+2	new	+2	now	+4	restore	-4
	old	12	new	+2	now	14	restore	<previous>
	old		new	+2	now	+2	restore	-2
	old	+2	new	-15	now	-13	restore	+13
	old	12	new	-15	now	-3	restore	<previous>
	*/
	if (isdigit(*oldv))
		++p;
	/* ZZZ: free the space allocated for the replaced values */
	q = mymalloc((unsigned) strlen(p)+1);
	strcpy(q, p);
	return(q);
}

struct environment *
force_envir(env, til_envir)
struct environment	*env;
char	*til_envir;
{
	struct definition	*def;
	ENTRY	*entry;

	if (strcmp(env->envname, til_envir) == 0)
		return(end_envir(env, TOP_LEVEL));
	/* check that there is an instance of til_envir */
	if ((entry = hashfind(ENV_TABLE, til_envir)) == (ENTRY *) NULL)
		return(env);
	def = (struct definition *) entry->data;
	if (def->instance == NULL)
		return(env);
	do {
		if (*env->def->name != COLON)
			warn_user(PR_FILENAME,
					"No end for %s begun at line %d\n",
					env->envname, env->linenumber);
		env = end_envir(env, TOP_LEVEL);
	} while (strcmp(env->envname, til_envir) != 0);
	return(env);
}

/* end_envir:
	restore previous environment
	generate end_def troff
 */

struct environment *
end_envir(env, level)
struct environment	*env;
int	level;
{
	struct environment	*newenv;

	if (level == TOP_LEVEL)
		/* pop top_environment and any associated sub_envir*/
		do {
			newenv = do_end_envir(env);
			if (newenv == env)
				break;
			env = newenv;
		} while (env->lastinstance == (struct environment *) SUB_LEVEL);
	else
		env = do_end_envir(env);
	return(env);
}

struct environment *
do_end_envir(env)
struct environment	*env;
{
	ENTRY	*grabit;
	register struct state	*s;
	struct state	*nstate, *ps;
	struct statestack	*states;
	register struct definition	*d;
	register struct def_element	*define;
	struct cond_def_el	*cdl;
	char	*troff_string;

	for (s = env->state_list; s != (struct state *) 0; s = s->next) {
		grabit = hashfind(STACK_TABLE, s->attribute);
		if (grabit == (ENTRY *) NULL) {
			warn_me(PR_FILENAME | PR_LINENUMBER,
				"End_envir: %s not in stack!\n", s->attribute);
			continue;
		}
		states = (struct statestack *) grabit->data;
		if (s->previous == (struct state *) 0)
			states->first = states->last = (struct state *) 0;
		else
			states->last = s->previous;
		/* if relative values, restore the state structure values */
		/* if absolute numbers, restore the previous values */
		if (isrelative_values(s))
			ps = s;
		else
			if ((ps = s->previous) == (struct state *) 0)
				/* no previous to restore */
				continue;
		troff_string = trans_state(ps);
		textputs(troff_string, MONK_DB);
		/* ZZZ: isn't all this state info malloced? release now? */
	}
	d = (struct definition *) env->def;
	define = d->end_def;
	if (define != (struct def_element *) 0)
		do {
			while ((cdl = define->cdl) != (struct cond_def_el *) 0){
				if (iscondtrue(cdl->attribute, cdl->value,
								cdl->type))
					break;
				define = cdl->next_on_fail;
				if (define == (struct def_element *) 0)
					break;
			}
			if (define == (struct def_element *) 0)
					break;
			if (define->troff == (char *) 0) {
				use_define(env, define, MONK_DB);
				continue;
			}
			if (d->values == (struct value *) 0
						&& define->special == FALSE)
				textputs(define->troff, MONK_DB);
			else {
				troff_string = replace_args(define->troff,
					define->special, d->values, env);
				textputs(troff_string, MONK_DB);
				if (troff_string != define->troff)
					myfree(troff_string, 44);
			}
			if ((grabit = hashfind(STACK_TABLE, define->attribute))
							== (ENTRY *) NULL)
				continue;
			states = (struct statestack *) grabit->data;
		/* do not add anything new to stack;
			rather replace last or add value_list to last */
			add_values(states->last, define->value);
		} while ((define = define->next) != (struct def_element *) 0);
	/* previous NULL if end invoked before any other command, incl make */
	if (env->previous == (struct environment *) 0)
		return(env);
	if (env->lastinstance != (struct environment *) SUB_LEVEL)
		env->def->instance = env->lastinstance;
	free_env(env);
	return(env->previous);
}

isrelative_values(state)
struct state	*state;
{
	struct value	*oldv, *v, *newv;

	if ((v = state->value) == (struct value *) 0)
		return(FALSE);
	do {
		if (*v->value == '+' || *v->value == '-') {
			state->allocated = TRUE;
			break;
		}
	} while ((v = v->next) != (struct value *) 0);
	if (state->allocated == FALSE)
		return(FALSE);
	oldv = state->value;
	do {
		newv = (struct value *) mymalloc(sizeof(struct value));
		if (state->value == oldv)	
			state->value = newv;
		else
			v->next = newv;
		v = newv;
		v->value = mymalloc((unsigned) strlen(oldv->value)+1);
		strcpy(v->value, oldv->value);
		if (*v->value == '+')
			*v->value = '-';
		else if (*v->value == '-')
			*v->value = '+';
	} while ((oldv = oldv->next) != (struct value *) 0);
	v->next = (struct value *) 0;
	return(TRUE);
}

struct environment *
init_db_env(doctype, db_path)
char	*doctype, *db_path;
{
	read_databases(doctype, db_path);
	return(init_envir());
}

/* use this one stack for both environment structures and state change info */

struct environment *
init_envir()
{
	struct environment	*env;

	/* run constructed initialize */
	checkifmissing();
	i_statestack();
	env = begin_envir((struct environment *)0, INIT_STACK, NULL, NO_WRITE);
	env = begin_envir(env, INIT_TROFF, NULL, MONK_DB);
	env = begin_envir(env, MONK_TEXT, NULL, MONK_DB);
	env = begin_envir(env, DOC_TEXT, NULL, MONK_DB);
	return(env);
}

struct environment *
begin_file(env)
struct environment	*env;
{
	return(begin_envir(env, MONK_FILE, NULL, MONK_DB));
}

struct environment *
end_file(env)
struct environment	*env;
{
	return(force_envir(env, MONK_FILE));
}

end_allenvir(env)
struct environment	*env;
{
	if (env == (struct environment *) 0)
		return;
	for(;;) {
		if (*env->def->name != COLON)
			warn_user(0, "No end for %s begun at line %d\n",
					env->def->name, env->linenumber);
		if (env->previous == (struct environment *) 0) {
			end_envir(env, TOP_LEVEL);
			return;
		}
		env = end_envir(env, TOP_LEVEL);
	}
}

/* build hash table of stacking variables from attribute hash table */
i_statestack()
{
	struct table_info	*gethashtbl(), *t;
	ENTRY	*p, *pe;
	struct attrib_info	*ai;

	t = gethashtbl(ATT_TABLE);
	for (p = t->start, pe = p + t->length; p < pe; ++p)
		if (p->key != NULL) {
			ai = (struct attrib_info *) p->data;
			if (ai->stacking == TRUE)
				add_state(p->key);
		}
}

add_state(attribute)
char	*attribute;
{
	ENTRY	*stack_att;
	struct statestack	*s;

	if (hashfind(STACK_TABLE, attribute) == (ENTRY *) NULL) {
		stack_att = (ENTRY *) mymalloc(sizeof(ENTRY));
		stack_att->key = attribute;
		stack_att->data = mymalloc(sizeof(struct statestack));
		s = (struct statestack *) stack_att->data;
		s->first = (struct state *) 0;
		s->last = (struct state *) 0;
		hashenter(STACK_TABLE, stack_att);
	}
}

/*
 * convn converts a number n in base b to an ascii string
 */
char	*
convn(n, b)
int	n;
{
	static char	buf[11];
	register char	*cp;
	int	orign;

	if ((orign = n) < 0)
		n = (unsigned) (-n);
	cp = &buf[10];
	*cp = '\0';
	do {
		*--cp = "0123456789abcdef"[n%b];
		n /= b;
	} while (n);
	if (orign < 0)
		*--cp = '-';
	else
		*--cp = '+';
	return(cp);
}
