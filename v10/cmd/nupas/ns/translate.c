#include <ctype.h>
#include <stdio.h>
#include "mail.h"
#include "string.h"
#include "aux.h"

/* imports */
extern char **getflds();
extern char *getenv();

/* predeclared */
string *getdbfiles();
int translate();
int lookup();
int compare();

/* loop through the names to be translated */
main(ac, av)
	int ac;
	char *av[];
{
	string *alias;		/* the alias for the name */
	char *thissys;		/* name of this system */
	string *files;		/* list of files to search */
	int i;

	/* get environmental info */
	thissys = sysname_read();
	files = getdbfiles();
	alias = s_new();

	/* loop through the names to be translated (from standard input) */
	for(i=1; i<ac; i++) {
		(void)translate(av[i], thissys, files, alias);
		if (*s_to_c(alias) == '\0')
			printf("local!%s\n", av[i]);
		else
			printf("%s\n", s_to_c(alias));
		fflush(stdout);
	}
	return 0;
}

/* get the list of dbfiles to search */
string *
getdbfiles()
{
	FILE *fp;
	string *files = s_new();

	/* system wide aliases */
	if (chdir(UPASROOT) < 0) {
		perror("translate(chdir):");
		return files;
	}
	if ((fp = fopen(SYSALIAS, "r")) != NULL) {
		while (s_gettoken(fp, files) != NULL)
			s_append(files, " ");
		fclose(fp);
	}

	return files;
}

/* loop through the translation files */
int
translate(name, thissys, files, alias)
	char *name;		/* name to translate */
	char *thissys;		/* name of this system */
	string *files;
	string *alias;		/* where to put the alias */
{
	string *file = s_new();
	string *fullname;
	char *home;

	/*
	printf("translate(%s, %s, %s, %s)\n", name, thissys,
		s_to_c(files), s_to_c(alias));
	*/
	/* create the full name to avoid loops (system!name) */
	fullname = s_copy(thissys);
	s_append(fullname, "!");
	s_append(fullname, name);

	/* look at user's local names */
	home = getenv("HOME");
	if (home != NULL) {
		s_append(file, home);
		s_append(file, USERALIAS);
		if (lookup(name, fullname, file, alias, thissys)==0) {
			s_free(fullname);
			s_free(file);
			return 0;
		}
	}

	/* look at system-wide names */
	s_restart(files);
	while (s_parse(files, s_restart(file)) != NULL) {
		if (lookup(name, fullname, file, alias, thissys)==0) {
			s_free(fullname);
			s_free(file);
			return 0;
		}
	}

	/* first look for mailbox */
	s_restart(file);
	abspath(name, MAILROOT, file);
	if (access(s_to_c(file), 0) == 0) {
		s_append(alias, "local");
		s_append(alias, "!");
		s_append(alias, name);
		s_free(file);
		return 0;
	}

	return -1;
}

/*  Loop through the entries in a translation file looking for a match.
 *  Return 0 if found, -1 otherwise.
 */
int
lookup(name, fullname, file, alias, thissys)
	char *name;
	string *fullname;
	string *file;
	string *alias;	/* returned string */
	char *thissys;
{
	FILE *fp;
	string *line = s_new();
	string *token = s_new();
	int rv = -1;
	char *cp;

/*
	printf("lookup(%s, %s, %s, %s)\n", name, s_to_c(fullname),
		s_to_c(file), s_to_c(alias));
*/
	s_reset(alias);
	if ((fp = fopen(s_to_c(file), "r")) == NULL)
		return -1;

	/* look for a match */
	while (s_getline(fp, s_restart(line))!=NULL) {
		if (s_parse(s_restart(line), s_restart(token))==NULL)
			continue;
		if (compare(token, name)!=0)
			continue;
		/* match found, get the alias */
		while(s_parse(line, s_restart(token))!=NULL) {
			/* avoid definition loops */
			if (compare(token, name)==0 ||
			    compare(token, s_to_c(fullname))==0){
				s_append(alias, "local");
				s_append(alias, "!");
				if(cp = strrchr(s_to_c(token), '!'))
					cp++;
				else
					cp = s_to_c(token);
				s_append(alias, cp);
			} else {
				s_append(alias, s_to_c(token));
			}
			s_append(alias, " ");
		}
		rv = 0;
		break;
	}
	s_free(line);
	s_free(token);
	return rv;
}

/* compare two strings (case insensitive) */
#define lower(c) (isupper(c)?c-('A'-'a'):c)
int
compare(s1, p2)
	string *s1;
	register char *p2;
{
	register char *p1 = s_to_c(s1);
	register int rv;

	while(1) {
		rv = lower(*p1) - lower(*p2);
		if (rv)
			return rv;
		if (*p1 == '\0')
			return 0;
		p1++;
		p2++;
	}
}
