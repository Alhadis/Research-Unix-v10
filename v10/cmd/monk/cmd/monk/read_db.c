#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"
#include	"dbcompress.h"

mk_hashtables()
{
	hcreate(ATT_TABLE, ATT_ENTRIES);
	hcreate(ENV_TABLE, ENV_ENTRIES);
	hcreate(STACK_TABLE, STACK_ENTRIES);
	hcreate(DEFINE_TABLE, DEFINE_ENTRIES);
/*	hcreate(STORE_TABLE, STORE_ENTRIES);	*/
}

read_entries(database, db_path, mesg_mode)
FILE	*database;
char	*db_path;
int	mesg_mode;
{
	int	command, messages, close_delim;
	char	*name, *type;

	messages = MESSAGE;
	while ((type=read_dbtype(database, messages, mesg_mode))!=(char *)EOF){
		if (type == (char *) 0) {
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
				"Null primitive %s\n", type);
			continue;
		}
		if ((close_delim = read_delim(database, TRUE)) == (char) FALSE){
			messages = FALSE;
			continue;
		}
		if (messages == FALSE) {
			warn(mesg_mode, 0, "\n\t\t\tcontinued gobbling til ");
			warn(mesg_mode, PR_LINENUMBER, "\n");
			messages = MESSAGE;
		}
		if ((command = isdbcommand(type)) == NOT_ACOMMAND) {
			warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
					"Unknown primitive %s\n", type);
			continue;
		}
		if (command != N_COMMENT) {
			if ((name = r_buftoken(database, OK_SPACE)) == 
								(char *) EOF)
				return;
			if (name == (char *) 0) {
				readncheck(database, close_delim,
					GOBBLE|MESSAGE|mesg_mode,
					"Reading %s, null name given", type);
				continue;
			}
			readncheck(database, LIST_SEP, MESSAGE|mesg_mode,
						"Reading %s %s", type, name);
		}
		do_dbcommands(command, name, db_path, database, close_delim,
								mesg_mode);
	}
}

/* read_dbtype:
	returns type of database entry - attribute, comment, define, macro
 */

char *
read_dbtype(database, messages, mesg_mode)
FILE	*database;
int	messages, mesg_mode;
{
	static char	*dbtype = (char *) 0;

	readncheck(database, BEGIN_PRIMITIVE, GOBBLE|messages|mesg_mode,
						"Looking for primitive");
	if (dbtype != (char *) 0 && dbtype != (char *) EOF)
		myfree(dbtype, 3);
	dbtype = read_token(database, OK_SPACE);
	return(dbtype);	
}

/* commands which can appear in enduser files - includes database commands */

isacommand(type)
char	*type;
{
	if (type == (char *) 0 || type == (char *) EOF)
		return(NOT_ACOMMAND);
	if (strcmp(type, BEGIN) == 0)
		return(N_BEGIN);
	if (strcmp(type, END) == 0)
		return(N_END);
	if (strcmp(type, MAKE) == 0)
		return(N_MAKE);
	if (strcmp(type, SOURCE) == 0)
		return(N_SOURCE);
	if (strcmp(type, PART) == 0)
		return(N_PART);
	if (strcmp(type, PART) == 0)
		return(N_PART);
	return(isdbcommand(type));
}

/* commands which can appear in database files */

isdbcommand(type)
char	*type;
{
	if (type == (char *) 0 || type == (char *) EOF)
		return(NOT_ACOMMAND);
	if (strcmp(type, ASSOCIATE) == 0)
		return(N_ASSOCIATE);
	if (strcmp(type, ATTRIBUTE) == 0)
		return(N_ATTRIBUTE);
	if (strcmp(type, COMMENT) == 0)
		return(N_COMMENT);
	if (strcmp(type, DEFINE) == 0)
		return(N_DEFINE);
	if (strcmp(type, ENVIRONMENT) == 0)
		return(N_ENVIRONMENT);
	if (strcmp(type, INSERT) == 0)
		return(N_INSERT);
	if (strcmp(type, MACRO) == 0)
		return(N_MACRO);
	return(NOT_ACOMMAND);
}

void
do_dbcommands(command, name, db_path, file, close_delim, mesg_mode)
int	command;
char	*name, *db_path;
FILE	*file;
int	close_delim, mesg_mode;
{
	if (do_commands(command, name, db_path, file, close_delim, mesg_mode)
								== FALSE)
		/* N_SOURCE is not a database command -
					all database files monk-processed */
		if (command == N_INSERT)
			read_asciidb(name, db_path, mesg_mode);
}

do_commands(command, name, db_path, file, close_delim, mesg_mode)
int	command;
char	*name, *db_path;
FILE	*file;
int	close_delim, mesg_mode;
{
	switch(command) {
		case N_ATTRIBUTE:
			read_attribute(file, name, close_delim, mesg_mode);
			break;
		case N_COMMENT:
			read_comment(file, close_delim, mesg_mode);
			break;
		case N_DEFINE:
			read_word(file, name, close_delim, mesg_mode);
			break;
		case N_ENVIRONMENT:
			read_define(file, name, FALSE, close_delim, mesg_mode);
			break;
		case N_ASSOCIATE:
			read_define(file, name, TRUE, close_delim, mesg_mode);
			break;
		case N_MACRO:
			read_macro(file, name, db_path, close_delim, mesg_mode);
			break;
		default:
			return(FALSE);
		}
	return(TRUE);
}

read_asciidb(datafilename, db_path, mesg_mode)
char	*datafilename, *db_path;
int	mesg_mode;
{
	FILE	*database;

	database = fopendb(datafilename, db_path, "r", EXIT_ON_FAIL);
	read_entries(database, db_path, mesg_mode);
	fclosencheck(database);
}

/* read_word: reads the equivalent (word to be used to replace tokens) */
void
read_word(database, name, close_delim, mesg_mode)
FILE	*database;
char	*name;
int	close_delim, mesg_mode;
{
	ENTRY	*entry;

	entry = (ENTRY *) bufmalloc(B_HASH_DEFINE, sizeof(ENTRY));
	entry->key = name;
	entry->data = read_token(database, OK_SPACE);
/* new read_token: supertoken plus malloc
	entry->data = read_token(database, TRUE, TRUE, NULL, NULL, close_delim);
 */
	readncheck(database, close_delim, FALSE, TRUE, "Reading %s %s\n",
							DEFINE, name);
	hashncheck(DEFINE_TABLE, entry, DEFINE, mesg_mode);
}

/* read_comment: gobbles comment */

void
read_comment(database, close_delim, mesg_mode)
FILE	*database;
int	close_delim, mesg_mode;
{
	int	c;

	while (((c = mygetc(database)) != EOF) && c != close_delim);
	/* ZZZZ: starting line of comment */
	if (c == EOF)
		warn(mesg_mode, PR_FILENAME,"Reached EOF reading %s\n",COMMENT);
}

struct value *
read_values(database, delims, outer_delim)
FILE	*database;
char	outer_delim;
{
	struct value	*firstv, *v;
	char	*arg;

	for(firstv = (struct value *) 0;
		(arg=r_bufarg(database, delims, outer_delim)) != (char *)EOF;) {
		if (arg == (char *) 0)
			break;
		if (firstv == (struct value *) 0) {
			firstv = v = make_value(arg);
		} else {
			v->next = make_value(arg);
			v = v->next;
		}
		v->next = (struct value *) 0;
	}
	return(firstv);
}

struct valu_str *
parse_values(s)
char	*s;
{
	static struct valu_str	vs;
	struct value	*v;
	struct strings	*sting;
	char	*arg;

	for(vs.value = (struct value *) 0;
		(sting=strarg(s, FALSE)) != (struct strings *) 0;){
		if ((arg = sting->token) == (char *) 0)
			break;
		s = sting->newp;
		if (vs.value == (struct value *) 0) {
			vs.value = v = make_value(arg);
		} else {
			v->next = make_value(arg);
			v = v->next;
		}
		v->next = (struct value *) 0;
	}
	vs.newp = s;
	return(&vs);
}

struct value *
make_value(newarg)
char	*newarg;
{
	struct value	*newv;

	if (newarg == (char *) 0)
		return((struct value *) 0);
	newv = (struct value *) bufmalloc(B_VALUE, sizeof(struct value));
	newv->value = newarg;
	newv->next = (struct value *) 0;
	if (*newarg != DEF_ARGUMENT)
		newv->type = FIXED_ARG;
	else
		if (*++newarg == XPAND_CHAR)
			newv->type = XPAND_ARG;
		else
			if (*newarg == REGEX_CHAR)
				/* cannot compile regular expression until
					after argument substitutions in
					formatting commands */
				newv->type = REGEX_ARG;
			else
				newv->type = VAR_ARG;
	return(newv);
}

