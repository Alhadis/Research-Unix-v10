#include	<stdio.h>
#include	"file_info.h"
#include	"warn.h"

#define	SLASH		'/'
#define	U_MSG		01	/* must agree with U_MSG in rd.h */

void	warn_user(), warn_db(), warn_me();
#ifdef WARN_WHOM_ROUTINE_POINTERS
/* in calling routines:
	whose = (whom == U_TEXT) ? warn_whom[0] : warn_whom[1];
	(*whose)(mode, format_string, arg1, arg2, arg3, arg4, arg5);
 */
void	(*warn_whom[])() = {warn_user, warn_db};
void	(*whose)();
#endif

int dont_tell_me = 1;

struct missing {
	char	*name;
	char	*filename;
	short	linenumber;
	struct missing	*next;
};

struct filestack	*filestack;
struct missing		*missingstack;

char	*_file_path = NULL;
char	*_parent = NULL;

struct missing	*add_missing(), *known_missing();
char	*mymalloc(), *add_path();
void	add_to_filestack(), get_path();

FILE *
fopenncheck(filename, mode, usepath, exit_on_fail)
char	*filename, *mode;
int	usepath, exit_on_fail;
{
	char	*add_path();

	FILE	*file = NULL;
	char	*name;

	if (filename == (char *) 0)
		return((FILE *) NULL);
	if (usepath) {
		name = add_path(filename);
		file = fopen(name, mode);
	}
	if (file == NULL) {
		/* usepath == FALSE */
		if (usepath == 0 || strcmp(name, filename) != 0)
			file = fopen(filename, mode);
		if (file == NULL) {
			if (usepath && strcmp(name, filename) != 0)
				warn_user(0, "Cannot open %s or %s\n",
							name, filename);
			else
				warn_user(0, "Cannot open %s\n", filename);
			if (exit_on_fail)
				exit(exit_on_fail);
			return((FILE *) NULL);
		}
		name = filename;
	}
	add_to_filestack(name);
	return(file);
}

void
add_to_filestack(name)
char	*name;
{
	struct filestack	*temp;

	/* track filenames and line numbers for error messages */
	if (filestack == (struct filestack *) 0) {
		filestack = (struct filestack *)
					mymalloc(sizeof(struct filestack));
		filestack->previous = (struct filestack *) 0;
	} else {
		temp = (struct filestack *) mymalloc(sizeof(struct filestack));
		temp->previous = filestack;
		filestack = temp;
	}
	/* toss path and keep only filename?? */
	filestack->name = mymalloc((unsigned) strlen(name)+1);
	strcpy(filestack->name, name);
	filestack->line_number = 1;
}

fclosencheck(file)
FILE	*file;
{
	if (fclose(file) == EOF)
		warn_me(PR_FILENAME|PR_LINENUMBER,"fclosencheck: cannot close");
	if (filestack == (struct filestack *) 0) {
		warn_me(PR_FILENAME | PR_LINENUMBER,
				"fclosencheck: no file to pop from filestack");
		return;
	}
	/* do not free name;	myfree(filestack->name, 16);
		environments have pointers to their origin filename */	
	myfree(filestack, 17);
	filestack = filestack->previous;
}

struct file_info *
save_file_info()
{
	static struct file_info	fi;

	fi.file_name = filestack->name;
	fi.line_number = filestack->line_number;
	return(&fi);
}

char	*
get_file_name()
{
	if (filestack == (struct filestack *) 0)
		return(":monk");
	return(filestack->name);
}

get_line_number()
{
	if (filestack == (struct filestack *) 0)
		return(0);
	return(filestack->line_number);
}

missing_def(token)
char	*token;
{
	struct missing	*m;

	if (missingstack == (struct missing *) 0) {
		missingstack = add_missing(token, (struct missing *) 0);
		return;
	}
	if ((m = known_missing(token, missingstack)) == (struct missing *) 0)
		return;
	add_missing(token, m);
}

/*	if token to be added to missingstack, returns last structure;
	if token is known to be missing, returns (struct missing *) 0
 */

struct missing *
known_missing(token, m)
char	*token;
struct missing	*m;
{
	if (m != (struct missing *) 0)
		for (;; m = m->next) {
			if (strcmp(m->name, token) == 0)
				return((struct missing *) 0);
			if (m->next == (struct missing *) 0)
				break;
		}
	return(m);
}

struct missing *
add_missing(token, m)
char	*token;
struct missing	*m;
{
	struct file_info	*fi;

	if (m == (struct missing *) 0)
		m = (struct missing *) mymalloc(sizeof(struct missing));
	else {
		m->next = (struct missing *) mymalloc(sizeof(struct missing));
		m = m->next;
	}
	m->name = mymalloc((unsigned) strlen(token)+1);
	strcpy(m->name, token);
	fi = save_file_info();
	m->filename = mymalloc((unsigned) strlen(fi->file_name)+1);
	strcpy(m->filename, fi->file_name);
	m->linenumber = fi->line_number;
	m->next = (struct missing *) 0;
	return(m);
}

checkifmissing()
{
	struct missing	*m;

	if ((m = missingstack) != (struct missing *) 0) {
		do {
			if (isdefined(m->name) == 0)
				warn_db(0,
				"%s: line %d: No definition found for %s\n",
				m->filename, m->linenumber, m->name);
		} while ((m = m->next) != (struct missing *) 0);
		free_missing(missingstack);
		missingstack = (struct missing *) 0;
	}
}

free_missing(m)
struct missing	*m;
{
	if (m != (struct missing *) 0)
		do {
			myfree(m->name, 44);
			myfree(m->filename, 45);
			myfree(m, 46);
		} while ((m = m->next) != (struct missing *) 0);
}

/*VARARGS2*/
void
warn(mesg_mode, mode, format_string, arg1, arg2, arg3, arg4, arg5)
int	mesg_mode, mode;
char	*format_string;
{
	if (mesg_mode == 0)
		return;
	if (mesg_mode & U_MSG)
		warn_user(mode, format_string, arg1, arg2, arg3, arg4, arg5);
	else
		warn_db(mode, format_string, arg1, arg2, arg3, arg4, arg5);
}

/* warn_user:
	spew message onto stderr and exit if exit == nonzero
 */
/*VARARGS2*/
void
warn_user(mode, format_string, arg1, arg2, arg3, arg4, arg5)
int	mode;
char	*format_string;
{
	if (filestack != (struct filestack *) 0) {
		if (mode & PR_FILENAME)
			fprintf(stderr, "%s: ", filestack->name);
		if (mode & PR_LINENUMBER)
			fprintf(stderr, "line %d: ", filestack->line_number);
	}
	fprintf(stderr, format_string, arg1, arg2, arg3, arg4, arg5);
}

/*VARARGS2*/
void
warn_db(mode, format_string, arg1, arg2, arg3, arg4, arg5)
int	mode;
char	*format_string;
{
	static FILE	*warn_db_file = (FILE *) 0;

#ifdef PRODUCTION
	char	*filename;

	if (warn_db_file == (FILE *) 0) {
		filename = mymalloc((unsigned) strlen(WARN_DB)+10);
		sprintf(filename, "%s%d", WARN_DB, getuid());
		if ((warn_db_file = fopen(filename, "w")) == NULL) {
			fprintf(stderr, "warn: cant open %s file for writing\n",
						filename);
			exit(-3);
		}
		setbuf(warn_db_file, NULL);
		myfree(filename, 18);
	}
#else
	warn_db_file = stderr;
#endif
	if (filestack != (struct filestack *) 0) {
		if (mode & PR_FILENAME)
			fprintf(warn_db_file, "%s: ", filestack->name);
		if (mode & PR_LINENUMBER)
			fprintf(warn_db_file, "line %d: ",
						filestack->line_number);
	}
	fprintf(warn_db_file, format_string, arg1, arg2, arg3, arg4, arg5);
}

/*VARARGS2*/
void
warn_me(mode, format_string, arg1, arg2, arg3, arg4, arg5)
int	mode;
char	*format_string;
{
	static FILE	*warn_me_file;
	char	*filename;

	if (dont_tell_me)
		return;
	if (warn_me_file == (FILE *) 0) {
		filename = mymalloc((unsigned) strlen(WARN_ME)+10);
		sprintf(filename, "%s%d", WARN_ME, getuid());
		if ((warn_me_file = fopen(filename, "w")) == NULL) {
			fprintf(stderr,
				"warn: cannot open %s file for writing\n",
						filename);
			exit(-3);
		}
		setbuf(warn_me_file, NULL);
		myfree(filename, 19);
	}
	if (filestack != (struct filestack *) 0) {
		if (mode & PR_FILENAME)
			fprintf(warn_me_file, "%s: ", filestack->name);
		if (mode & PR_LINENUMBER)
			fprintf(warn_me_file, "line %d: ",
						filestack->line_number);
	}
	fprintf(warn_me_file, format_string, arg1, arg2, arg3, arg4, arg5);
}

mygetc(file)
FILE	*file;
{
	int	c;

	if ((c = getc(file)) == '\n')
		filestack->line_number++;
	return(c);
}

void
myungetc(c, file)
int	c;
FILE	*file;
{
	if (c != EOF) {
		if (c == '\n')
			filestack->line_number--;
		c = ungetc(c, file);
	}
}

/* used when isitaloop fails - must change: this won't work on stream input */
myungets(s, file)
char	*s;
FILE	*file;
{
	char	*reverse;

	for (reverse = s + strlen(s); --reverse >= s; )
		myungetc(*reverse, file);
}

wrong_delim(key, close_delim, c)
char	*key;
int	close_delim, c;
{
	warn_user(PR_FILENAME | PR_LINENUMBER,
				"Mismatched delims around %s;", key);
	if (c == EOF)
		warn_user(PR_NOTHING, "expecting %c, got EOF\n", close_delim);
	else
		warn_user(PR_NOTHING, "expecting %c, got %c\n", close_delim, c);
}

FILE *
parentopen(filename)
char	*filename;
{
	FILE	*file;
	int	n;
	char	*name;

	if (filename == (char *) 0)
		return((FILE *) NULL);
	name = add_path(filename);
	if ((file = fopen(name, "r")) == NULL) {
		if (strcmp(name, filename) == 0) {
			fprintf(stderr, "Cannot open parent file %s\n", name);
			return((FILE *) NULL);
		}
		if ((file = fopen(filename, "r")) == NULL) {
			fprintf(stderr, "Cannot open parent file %s or %s\n",
							name, filename);
			return((FILE *) NULL);
		}
		name = filename;
	}
	add_to_filestack(name);
	if (_parent != NULL)
		myfree(_parent, 71);
	n = strlen(name) +1;
	_parent = mymalloc((unsigned) n);
	strncpy(_parent, name, n);
	return(file);
}

char *
get_parent_name()
{
	return(_parent);
}

void
clear_parent_name()
{
	if (_parent != NULL)
		myfree(_parent, 72);
	_parent = NULL;
}

void
get_path(filename)
char	*filename;
{
	int	n;
	char	*strrchr(), *slash;

	if (_file_path != NULL)
		myfree(_file_path, 70);
	if ((slash = strrchr(filename, SLASH)) == NULL) {
		_file_path = NULL;
		return;
	}
	n = slash - filename + 1;
	_file_path = (char *) mymalloc((unsigned) n+1);
	strncpy(_file_path, filename, n);
	_file_path[n] = '\0';
}

/* Add path:	should db_path also be global and handled here ?? */

char	*
add_path(filename)
char	*filename;
{
	static char	*name = NULL;
	char	*path = NULL;
	int	n, m;

	if (name != NULL)
		myfree(name, 82);
#ifdef ALL_PATHS
	/* ALL PATHS HELD IN THIS FILE as two globals; 2nd arg == integer type */
	if (type == DATAPATH)
		path = DB_PATH; --- no may be replaced on comd line; 
	if (type == FILEPATH)
#endif
		path = _file_path;
	if (filename == NULL || *filename == SLASH || path == NULL)
		return(filename);
	n = strlen(path);
	m = strlen(filename);
	name = mymalloc((unsigned) n + m + 1);
	strcpy(name, path);
	strcpy(&name[n], filename);
	name[m+n] = '\0';
	return(name);
}

#ifdef TIMING
newtime(message, arg)
char	*message;
{
	static struct tms	tms1, tms2;
	static struct tms	*old, *new, *tmp;

	if (message != (char *) 0)
		fprintf(stderr, message, arg);
	if (new == (struct tms *) 0) {
		new = &tms1;
		old = &tms2;
		times(old);
		return;
	}
	times(new);
	fprintf(stderr, "\tUsertime %ld\tSystemtime %ld\n",
		new->tms_utime - old->tms_utime,
		new->tms_stime - old->tms_stime);
	tmp = new;
	new = old;
	old = tmp;
}
#endif
