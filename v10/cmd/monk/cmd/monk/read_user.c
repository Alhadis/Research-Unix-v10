#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"

short	db_mode;

struct environment *
r_userfile(filename, db_path, env, cmd_line)
char	*filename, *db_path;
struct environment	*env;
int	cmd_line;
{
	FILE	*userfile;
	int	use_path;

	/* this routine called for inserted files */
	use_path = (cmd_line == TRUE) ? FALSE : TRUE;
	if (filename != (char *) 0) {
		if ((userfile = fopenncheck(filename, "r", use_path, NO_EXIT))
							== (FILE *) NULL)
			return(env);
	} else
		userfile = stdin;
#ifdef TIMING
	newtime("Start processing userfile %s:\t", filename);
#endif
	if (env == (struct environment *) 0)
		env = r_usertop(userfile, db_path, env, cmd_line, FALSE);
	else
		env = begin_file(env);
	if (env != (struct environment *) 0) {
		env = r_usertext(userfile, db_path, env, cmd_line);
		fclosencheck(userfile);
		env = end_file(env);
	} else
		fclosencheck(userfile);
#ifdef TIMING
	newtime("Finished processing userfile %s:\t", filename);
#endif
	return(env);
}

struct environment *
r_usertop(userfile, db_path, env, cmd_line, only_top)
FILE	*userfile;
char	*db_path;
struct environment	*env;
int	cmd_line, only_top;
{
	int	command;
	int	c, close_delim;
	char	*key;

	while ((c = mygetc(userfile)) != EOF) {
		/* gobbling space at top of file ??? */
		while(isspace(c))
			if ((c = mygetc(userfile)) == EOF)
				break;
		if (c != BEGIN_PRIMITIVE)
			break;
		key = read_token(userfile, NO_SPACE);
		if ((close_delim = read_delim(userfile, FALSE)) == EOF)
			close_delim = FALSE;
		if (((command = isacommand(key)) < INFO_COMMANDS) && only_top)
			return(env);
		if (command == N_COMMENT) {
			read_comment(userfile, close_delim, U_MSG);
			continue;
		}
		if (env == NULL && command < MAKE_COMMANDS)
			env = begin_file(init_db_env(NULL, db_path));	
		if (key == (char *) EOF || key == (char *) 0) {
			textputc(BEGIN_PRIMITIVE, userfile);
			return(env);
		}
		if (command == NOT_ACOMMAND)
			return(notacommand(userfile, env, key, close_delim));
		key = read_env(userfile, command, &close_delim);
		env = do_envcommands(command, key, db_path, userfile, env,
						close_delim, cmd_line);
		/* INSERT could still potentially not initialize env */
	}
	if (c != EOF) {
		myungetc(c, userfile);
		if (env == NULL)
			env = begin_file(init_db_env(NULL, db_path));	
	}
	return(env);
}

struct environment *
r_usertext(userfile, db_path, env, cmd_line)
FILE	*userfile;
char	*db_path;
struct environment	*env;
int	cmd_line;
{
	int	command;
	int	c, close_delim;
	char	lchar_in = NULL;
	char	*key;

	if ((c = mygetc(userfile)) != NEW_LINE)
		myungetc(c, userfile);
	while ((c = mygetc(userfile)) != EOF) {
#ifdef SPECIAL_IS_IN
		if (lchar_in == '\\') {
			/* '\' special character only relative to '|' */
			if ((lchar_in = c) != BEGIN_PRIMITIVE)
				textputc('\\', U_TEXT);
			if ( c != '\\')
				textputc(c, U_TEXT);
			continue;
		}
		if (c == '\\') {
			lchar_in = c;
			continue;
		}
#endif
		if (c != BEGIN_PRIMITIVE) {
			if ( env->how_to_end != (char) 0 &&
					(char) c == env->how_to_end ) {
				env = end_envir(env, TOP_LEVEL);
				nl_nlgobble(lchar_in, userfile);
			} else {
				textputc(c, U_TEXT);
				lchar_in = c;
			}
			continue;
		}
		if ((key = read_token(userfile, NO_SPACE)) == (char *) EOF)
			break;
		if (key == (char *) 0) {
			textputc(BEGIN_PRIMITIVE, U_TEXT);
			lchar_in = BEGIN_PRIMITIVE;
			continue;
		}
		/* gobbles white space looking for open_delim, returns
					matching close_delim */
		if ((close_delim = read_delim(userfile, FALSE)) == EOF)
			close_delim = (char) FALSE;
		/* there are two enduser commands: begin and end */
		if ((command = isacommand(key)) == NOT_ACOMMAND) {
			/* short form of environment invocation */
			env = notacommand(userfile, env, key, close_delim);
			nl_nlgobble(lchar_in, userfile);
			continue;
		}
		if (command != N_COMMENT)
			key = read_env(userfile, command, &close_delim);
		env = do_envcommands(command, key, db_path, userfile,
						env, close_delim, cmd_line);
		nl_nlgobble(lchar_in, userfile);
	}
	/* force end of all environments started in this file, til DOC_FILE */
	return(env);
}

struct environment *
notacommand(userfile, env, key, close_delim)
FILE	*userfile;
struct environment	*env;
char	*key;
int	close_delim;
{
	struct environment	*newenv;

	/* short form of environment invocation */
	newenv = begin_envir(env, key, close_delim, U_TEXT);
	if (newenv != env) {
		env = newenv;
		if (env->def->sub_def != NULL) {
			if (close_delim != FALSE)
				env = assoc_envir(userfile, env, env->def,
						close_delim, END_NOW, U_TEXT);
			else
				env = do_subenv(env, env->def,(struct value *)0,
						close_delim, END_NOW, U_TEXT);
		}
		/* Handle environ wo delims,eg |paragraph */
		if (close_delim == FALSE)
			env = end_envir(env, TOP_LEVEL);
	}
	return(env);
}

char *
read_env(userfile, command, close_delim)
FILE	*userfile;
int	command;
int	*close_delim;
{
	char	*key;
	int	c;

	if (command != N_INSERT && command != N_SOURCE && command != N_PART)
		key = read_token(userfile, OK_SPACE);
	else
		key = read_filename(userfile, OK_SPACE);
	if ((c = r_nonblank(userfile)) == *close_delim)
		*close_delim = FALSE;
	else
		/* ITEM_SEP if instance; LIST_SEP if definition */
		if (c != ITEM_SEP && c != LIST_SEP) {
			wrong_delim(key, *close_delim, c);
				/* put back any char for health?? */
			if (c == BEGIN_PRIMITIVE)
				myungetc(c, userfile);
		}
	return(key);
}

void
nl_nlgobble(lchar_in, userfile)
char	lchar_in;
FILE	*userfile;
{
	int	c;

	if (lchar_in == NEW_LINE)
		if ((c = mygetc(userfile)) != EOF && (char) c != NEW_LINE)
			myungetc(c, userfile);
}

void
read_databases(doctype, db_path)
char	*doctype, *db_path;
{
	init_dbread();
	if (db_mode == STANDARD)
		read_dbfile(DB_ATTRIBUTE, db_path, db_mode, FALSE);
	read_dbfile(DB_GLOBAL, db_path, db_mode, FALSE);
	if (db_mode == STANDARD)
		read_dbfile(DB_MACRO, db_path, db_mode, FALSE);
	if (doctype != NULL)
		make_doc(doctype, db_path, db_mode, FALSE);
}

read_dbfile(datafilename, db_path, mode, mesg_mode)
char	*datafilename, *db_path;
int	mode, mesg_mode;
{
	FILE	*database;
	char	cfile[24];	

	if (mode == STANDARD) {
		database = fopendb(datafilename, db_path, "r", EXIT_ON_FAIL);
		read_entries(database, db_path, mesg_mode);
	} else if (mode == COMPRESSED) {
		strcpy(cfile, datafilename);
		strcat(cfile, DB_COMP_EXT);
		database = fopendb(cfile, db_path, "r", EXIT_ON_FAIL);
		r_buffers(database, db_path);
	}
	fclosencheck(database);
}

struct environment	*
do_envcommands(command, name, db_path, file, env, close_delim, cmd_line)
int	command;
char	*name, *db_path;
FILE	*file;
struct environment	*env;
int	close_delim, cmd_line;
{
	struct definition	*def;
	struct environment	*newenv;
	char	c;

							/* textwrite? */
	switch(command) {
		case N_BEGIN:
			def = get_envir(name, close_delim, U_TEXT);
			if (def == (struct definition *) 0)
				break;
			if (def->sub_def != (struct definition *) 0
					&& close_delim != FALSE) {
				env = assoc_envir(file, env, def,
					close_delim, END_AT_END, U_TEXT);
				if ((c = r_nonblank(file)) !=  close_delim) {
					wrong_delim(name, close_delim, c);
					myungetc(c, file);
				}
			}
			env = do_envir(env, def, TOP_LEVEL, (char)NULL, U_TEXT);
			break;
		case N_END:
			/* if key doesn't match next expected ??? */
			if (env->how_to_end != (char) 0
					|| strcmp(name, env->def->name) != 0) {
				if (*env->def->name == COLON)
					warn_user(PR_FILENAME | PR_LINENUMBER,
					"Unexpected end for %s\n", name);
				else {
					warn_user(PR_FILENAME | PR_LINENUMBER,
					"Expecting end for %s begun at line %d",
					env->def->name, env->linenumber);
					warn_user(0,"; got end for %s\n", name);
				}
#ifdef HEALTH
				env_health(env, name);
#endif
			} else
				env = end_envir(env, TOP_LEVEL);
			break;
		case N_INSERT:
			env = r_userfile(name, db_path, env, FALSE);
			break;
		case N_SOURCE:
			env = begin_file(env);
			copy_userfile(name);
			env = end_file(env);
			break;
		case N_PART:
			/* Error message for part command buried in file? */
				/* get document type from parent file */
			if ((env = make_type(env, name, db_path)) == NULL) {
				env = init_db_env(NULL, db_path);
				env = begin_file(env);
			}
			if (cmd_line == TRUE) {
				newenv = begin_envir(env, MONK_RESTORE,
							NULL, MONK_DB);
				newenv = begin_envir(newenv, DOC_RESTORE,
							NULL, MONK_DB);
			} else {
				newenv = begin_envir(env, MONK_SAVE,
							NULL, MONK_DB);
				newenv = begin_envir(newenv, DOC_SAVE,
							NULL, MONK_DB);
			}
			if (newenv != env) {
				if ((newenv=end_envir(newenv, TOP_LEVEL))!=env)
					newenv = end_envir(newenv, TOP_LEVEL);
				env = newenv;
			}
			break;
		case N_MAKE:
			if (env == (struct environment *) 0) {
				env = init_db_env(name, db_path);
				env = begin_file(env);
			} else
				make_doc(name, db_path, STANDARD, U_REDEF_MSG);
			break;
		default:
			do_commands(command, name, db_path, file, close_delim,
						U_REDEF_MSG | U_SYNTX_MSG);	
	}
	return(env);
}

void
make_doc(doctype, db_path, compressed, mesg_mode)
char	*doctype, *db_path;
int	compressed, mesg_mode;
{
	struct makes {
		char	*document;
		struct makes	*next;
	};
	static struct makes *makes;
	struct makes	*m;
	char	*temp;

	if (doctype == NULL)
		return;
	/* prevents reread of the same document type `doctype';
		ignores the db directory and the compression style */
	for (m = makes; m != NULL; m = m->next)
		if (strcmp(doctype, m->document) == 0)
			return;
	m = (struct makes *) mymalloc(sizeof(struct makes));
	if (makes == NULL)
		makes = m;
	m->document = mymalloc((unsigned) strlen(doctype)+1);
	strcpy(m->document, doctype);
	m->next = NULL;	
	read_dbfile(doctype, db_path, compressed, mesg_mode);
}

/* make_type:
	now retrieves commands > INFO_COMMANDS from top of parent files
			ASSOCIATE, ATTRIBUTE, ENVIRONMENT, MACRO
			COMMENT, SOURCE, INSERT, MAKE, PART
	  need	1. to add style as a special command
		2. to retrieve commands affecting style & db from anywhere in
			document and even from earlier children (via .cite?)
 */

struct environment *
make_type(env, filename, db_path)
struct environment	*env;
char	*filename;
char	*db_path;
{
	FILE	*file;

	if ((file = parentopen(filename)) ==  NULL)
		return(env);
	env = r_usertop(file, db_path, env, FALSE, TRUE);
	fclose(file);
	return(env);
}
