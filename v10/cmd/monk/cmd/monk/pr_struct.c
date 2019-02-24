/*LINTLIBRARY*/
#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"rd.h"

extern int	dont_tell_me;

pr_env(env)
struct environment	*env;
{
	if (dont_tell_me)
		return;
	warn_me(0, "***************************************Env stack:\n");
	if (env != (struct environment *) 0)
		do {
			warn_me(0, "Definition %s", env->def->name);
			if (env->how_to_end != (char) 0)
				warn_me(0, " ending on %c.\n\t",
							env->how_to_end);
			else
				warn_me(0, " until end\n\t");
			pr_states(env->state_list);
		} while ((env = env->previous) != (struct environment *) 0);
	warn_me(0, "\n");
}

/* pr_states: print list of states for one environment */

pr_states(state)
register struct state	*state;
{
	struct value	*v;
	short	on_this_line = 0;

	if (dont_tell_me)
		return;
	warn_me(0, "Env state list:\n");
	if (state == (struct state *) 0)
		return;
	/* USE str_avalue */
	do {
		warn_me(0, "`%s'<%d>", state->attribute, state->allocated);
		if ((v = state->value) == (struct value *) 0) {
			warn_me(0, ", ");
			continue;
		}
		do {
			warn_me(0, "`%s'", v->value);
		} while ((v = v->next) != (struct value *) 0);
		if (++on_this_line > 5) {
			warn_me(0, ",\n");
			on_this_line = 0;
		} else
			warn_me(0, ", ");
	} while ((state = state->next) != (struct state *) 0);
	warn_me(0, "\n");
}

/* pr_stack: print stack of states for one attribute,
					starting with current state */

pr_stack(statestack)
struct statestack	*statestack;
{
	register struct state	*state;
	register struct value	*v;
	short	on_this_line = 0;

	if (dont_tell_me)
		return;
	state = statestack->last;
	if (state == (struct state *) 0) {
		warn_me(0, "No state stack:\n");
		return;
	}
	warn_me(0,"State stack for attribute `%s':\n", state->attribute);
		do {
			warn_me(0, "<%d>", state->allocated);
			if ((v = state->value) == (struct value *) 0)
				continue;
			do {
				warn_me(0, " `%s'", v->value);
			} while ((v = v->next) != (struct value *) 0);
			if (++on_this_line > 10) {
				warn_me(0, ",\n");
				on_this_line = 0;
			} else
				warn_me(0, ", ");
		} while ((state = state->previous) != (struct state *) 0);
		warn_me(0, "\n");
}

pr_attfromkey(key)
char	*key;
{
	ENTRY	*exist;

	if (dont_tell_me)
		return;
	if ((exist = hashfind(ATT_TABLE, key)) != (ENTRY *) 0)
		pr_att(exist, TRUE);
	else
		fprintf(stderr, "Cannot find attribute %s\n", key);
}

pr_deffromkey(key)
char	*key;
{
	ENTRY	*exist;

	if (dont_tell_me)
		return;
	if ((exist = hashfind(ENV_TABLE, key)) != (ENTRY *) 0)
		pr_defentry(exist);
	else
		fprintf(stderr, "Cannot find definition or associate %s\n",
									key);
}

pr_att(att_entry, print_troff)
ENTRY	*att_entry;
short	print_troff;
{
	struct attrib_info	*a_info;
	struct attrib_case	*a_case;

	if (dont_tell_me)
		return;
	if (att_entry == (ENTRY *) 0) {
		warn_me(0, "No attribute definition to print\n");
		return;
	}
	a_info = (struct attrib_info *) att_entry->data;
	warn_me(0,"\t\t\t\t\t%s Attribute: `%s':\n",
		str_atype(a_info), att_entry->key);
	if ((a_case = a_info->firstcase) != (struct attrib_case *) 0)
		do {
			if (a_case->nvalues == 0)
				warn_me(0, "\t\t%s no args;\n",
						str_aspecial(a_case));
			else
				warn_me(0, "\t\t%s %d args: %s;\n",
					str_aspecial(a_case), a_case->nvalues,
						str_avalue(a_case->value,TRUE));
			if (print_troff == TRUE) {
				if (a_case->troff == (char *) 0)
					warn_me(0, "troff undefined");
				else
					warn_me(0, "%s ", a_case->troff);
			}
		} while ((a_case=a_case->next) != (struct attrib_case *) 0);
}

pr_defentry(entry)
ENTRY	*entry;
{
	struct definition	*d;

	if (dont_tell_me)
		return;
	d = (struct definition *) entry->data;
	warn_me(0, "Definition of primitive `%s':\n", entry->key);
	pr_def(d);
}

pr_def(definition)
struct definition	*definition;
{
	register struct definition	*d;

	if (dont_tell_me)
		return;
	warn_me(0, "\t\t\t\tBegin definition: %s %s\n", definition->name,
					str_avalue(definition->values, TRUE));
	pr_def_beg_end(definition);
	if ((d = definition->sub_def) != (struct definition *) 0)
		do {
			warn_me(0, "\t\t\t\tBegin subdefinition: %s %s\n",
					d->name, str_avalue(d->values, TRUE));
			pr_def_beg_end(d);
		} while ((d = d->sub_def) != (struct definition *) 0);
}

pr_def_beg_end(definition)
struct definition	*definition;
{
	if (dont_tell_me)
		return;
	warn_me(0, "Begin:");
	pr_def_el(definition->begin_def);
	warn_me(0, "End:");
	pr_def_el(definition->end_def);
}

pr_def_el(el)
struct def_element	*el;
{
	struct def_element	*end_of_cond[32];
			/* ZZZZ limit of embedded conditionals 32 */
	int	layer = 0;

	if (dont_tell_me || (el == (struct def_element *) 0))
		return;
	do {
		while (layer > 0 && end_of_cond[layer] == el)
			warn_me(0, "End of conditional %d\n", layer--);
		if (el->cdl != 0) {
			pr_cdl(el->cdl, ++layer);
			end_of_cond[layer] = el->cdl->next_on_fail;
		}
		warn_me(0, "\t\t%d %s = ", el->special, el->attribute);
		if (el->troff == (char *) 0)
			warn_me(0, "troff undefined;\n");
		else
			warn_me(0, "%s\n", el->troff);
	} while ((el = el->next) != (struct def_element *) 0);
	while (layer > 0 && end_of_cond[layer] == el)
		warn_me(0, "End of conditional %d\n", layer--);
}

pr_cdl(cdl, layer)
struct cond_def_el	*cdl;
int	layer;
{
	struct def_element	*next;
	char	*type;

	if (dont_tell_me)
		return;
	type = (cdl->type == TRUE) ? IF_VALUE : IF_NOTVALUE;
	warn_me(0, "Conditional %d %s: %s %s;",
		layer, type, cdl->attribute, str_avalue(cdl->value, FALSE));
	if ((next = cdl->next_on_fail) == 0)
		warn_me(0, " End on fail;\n");
	else
		if (next->cdl == 0)
			warn_me(0, " Next on fail: %s %s;\n", next->attribute,
				str_avalue(next->value, FALSE));
		else
			warn_me(0, " Next on fail Cond: %s %s;\n",
				next->cdl->attribute,
				str_avalue(next->cdl->value, FALSE));
}

pr_def_valu(el)
struct def_element	*el;
{
	struct value	*v;

	if (dont_tell_me || (el == (struct def_element *) 0))
		return;
	do {
		warn_me(0, "attribute `%s'", el->attribute);
		if ((v = el->value) != (struct value *) 0)
			do
				warn_me(0, "%s value `%s'", str_vtype(v),
								v->value);
			while ((v = v->next) != (struct value *) 0);
		warn_me(0, "\n");
	} while ((el = el->next) != (struct def_element *) 0);
}


pr_valu(v)
struct value	*v;
{
	if (dont_tell_me)
		return;
	warn_me(0, "%s\n", str_avalue(v, TRUE));
}

pr_init_def(i)
struct init_def	*i;
{
	struct add_att	*a;
	struct value	*v;

	if (dont_tell_me)
		return;
	warn_me(0, "Stacking %d ", i->stacking);
	if ((a = i->add_att) == (struct add_att *) 0) {
		warn_me(0, "\n");
		return;
	}
	do {
		warn_me(0, "attribute `%s'", i->attribute);
		if ((v = a->value) == (struct value *) 0) {
			warn_me(0, "\n");
			continue;
		}
		do {
			warn_me(0, " value `%s'", v->value);
		} while ((v = v->next) != (struct value *) 0);
		warn_me(0, "\n");
	} while ((a = a->next) != (struct add_att *) 0);
}

char	*
str_avalue(v, prtype)
struct value	*v;
{
	static char	arglist[BUFSIZ];

	if (v == (struct value *) 0)
		return(BLANKSTRING);
	*arglist = '\0';
	do {
		if (*arglist != '\0' || prtype == TRUE)
			strcat(arglist, BLANKSTRING);
		strcat(arglist, v->value);
		if (prtype == TRUE) {
			strcat(arglist, "{");
			strcat(arglist, str_vtype(v));
			strcat(arglist, "}");
		}
	} while ((v = v->next) != (struct value *) 0);
	return(arglist);
}

char	*
str_atype(att_info)
struct attrib_info	*att_info;
{
	if (att_info->stacking == TRUE)
		return("stacking");
	else
		return("nonstacking");
}

char	*
str_aspecial(att_case)
struct attrib_case	*att_case;
{
	if (att_case->special == TRUE)
		return("special");
	else
		return(BLANKSTRING);
}

char	*
str_vtype(v)
struct value	*v;
{
	if (v->type == FIXED_ARG)
		return("fixed");
	if (v->type == VAR_ARG)
		return("variable");
	if (v->type == REGEX_ARG)
		return("reg_exp");
	if (v->type == XPAND_ARG)
		return("xpanding");
	return("????????");
}
