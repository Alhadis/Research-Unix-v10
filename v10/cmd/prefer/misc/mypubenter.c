/*
 * Facilitate entry of references
 */

#include	<stdio.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#define		TEMPLATE	"/usr/frodo/lib/refer/template"
#define		REFER		"refer.out"
#define		TMPREF		"/tmp/referXXXXXX"

#ifdef	PREFER
#define		TYPE		"%type"
#endif	/* PREFER */

#define		TRUE		(1)
#define		FALSE		(0)

#define		EDITOR		"EDITOR"
#define		VISUAL		"VISUAL"

struct ref {
	struct ref *r_next;	/* next reference node */
	char *r_prompt;		/* prompt for reference */
	struct att *r_att;	/* attributes */
};

struct att {
	struct att *a_next;	/* next attribute node */
	char a_flag;		/* prompt flag */
	char *a_prompt;		/* prompt for attribute */
	char *a_def;		/* default value for attribute */
	char *a_refer;		/* refer string to output */
	char *a_mcont;		/* middle continue string to output */
	char *a_econt;		/* end continue string to output */
};

char
	*getenv(),
	*malloc(),
	*mktemp(),
	*strchr(),
	*strtok(),
	*tmpref
;

int cleanup();

FILE
	*tmpopen(),
	*tmp,
	*refer
;

void
	fixrefer(),
	edit()
;

struct ref
	*ref,
	*sr
;

main(argc, argv)
int argc;
char *argv[];
{
	register char
		*tempname,
		*refname
	;

	signal(SIGINT, cleanup);
	signal(SIGQUIT, cleanup);

	tempname = refname = NULL;
	while (--argc) {
		if (!strcmp("-o", *++argv)) {
			refname = *++argv;
			--argc;
		} else
			tempname = *argv;
	}
	readtemplate(tempname ? tempname : TEMPLATE);

	if ((refer = fopen(refname ? refname : REFER, "a")) == NULL)
		errexit(1, "Can't open reference file, %s\n", REFER);

	doreferences();
}

/*
 * Process all user input.
 */
doreferences()
{
	int cont;
	struct ref
		*rr,
		*r
	;
	struct att *a;
	char buf[BUFSIZ];
	register char
		*p,
		*bufp
	;
	char comm[80];

	tmpref = mktemp(TMPREF);
	sr = NULL;
	for (;;) {
		sprintf(comm, "Reference type [%s] ", ref->r_prompt);
		if (getln(comm, buf, sizeof(buf)) == NULL)
			cleanup();

		if ((p = strchr(&buf[0], '\n')) != NULL)
			*p = '\0';

		if (!strlen(&buf[0]))
			strcpy(&buf[0], ref->r_prompt);

		switch (match(&rr, buf)) {
		case 0:
			printf("Legal reference types are:\n");
			for (r = ref; r; r = r->r_next)
				printf("%s\n", r->r_prompt);
			continue;

		case 1:
			r = rr;
			break;

		default:		/* handled in match() */
			continue;
		}

		if (!strcmp(r->r_prompt, "quit"))
			cleanup();

		fixrefer();

		if (!strcmp(r->r_prompt, "help")) {
			help();
			continue;
		}

		printf("referencing a %s\n", r->r_prompt);
		sr = r;
		if ((tmp = fopen(tmpref, "w")) == NULL)
			errexit(1, "Can't open %s\n", tmpref);

		for (a = r->r_att; a; a = a->a_next) {
			if (a->a_flag == '!')
				continue;
			do {
				do {
					if (a->a_def[0])
						sprintf(comm, "%s [%s]: ",
						     a->a_prompt, a->a_def);
					else
						sprintf(comm, "%s: ", a->a_prompt);
					if (getln(comm, buf, sizeof(buf)) == NULL)
						cleanup();

					p = strchr(buf, '\n');
					*p = '\0';

					if (a->a_flag == '\0' && !strlen(buf)
					    && !a->a_def[0])
						printf("The %s field is not optional\n",
						     a->a_prompt);
					else if (a->a_def[0])
						break;
				} while (a->a_flag == '\0' && !strlen(buf));

				if ((cont = strlen(buf)) != 0 && buf[strlen(buf) - 1] == '&')
					buf[strlen(buf) - 1] = '\0';
				else
					cont = 0;

				bufp = buf;
				while (*bufp == ' ' || *bufp == '\t')
					++bufp;
				if (strlen(buf))
					fprintf(tmp, "%s: %s\n", a->a_prompt, bufp);
				else
					fprintf(tmp, "%s: %s\n", a->a_prompt, a->a_def);
				fflush(tmp);
			} while (cont != 0);
			fflush(tmp);
		}
		fprintf(tmp, "\n");
		fclose(tmp);
	}
}

/*
 * Print out all attributes for a given reference; ``?'' for all.
 */
help()
{
	register struct att *a;
	char helpref[BUFSIZ];
	struct ref *r;

	printf("which reference type: ");
	fgets(helpref, sizeof(helpref), stdin);
	helpref[strlen(&helpref[0]) - 1] = '\0';
	switch (match(&r, helpref)) {
	case 0:
		if (!strcmp(helpref, "?")) {
			for (r = ref; r; r = r->r_next) {
				printf("%s:\n", r->r_prompt);
				for (a = r->r_att; a; a = a->a_next)
					printf("\t%s\n", a->a_prompt);
			}
		} else
			printf("No matches for %s\n", helpref);
		break;

	case 1:
		printf("Attributes for `%s':\n", r->r_prompt);
		for (a = r->r_att; a; a = a->a_next)
			printf("\t%s\n", a->a_prompt);
		break;
	}
}

/*
 * Read in references from template file
 */
readtemplate(file)
char *file;
{
	FILE *template;
	register char
		*p,
		*pp
	;
	register struct att *a = NULL;
	char buf[BUFSIZ];
	struct ref *r = NULL;

	template = tmpopen(file);
	while (fgets(&buf[0], sizeof(buf), template) != NULL) {
		if ((p = strchr(&buf[0], '\n')) != NULL)
			*p = '\0';

		switch (buf[0]) {
		case ':':
			continue;

		case '\t':
			if (r == NULL)
				errexit(1, "Attribute without reference active\n");

			if (a == NULL) {
				if ((a = r->r_att =
				    (struct att *)malloc((unsigned)sizeof(struct att)))
				    == NULL)
					errexit(1, "Out of memory for attributes\n");
			} else if ((a = a->a_next =
			    (struct att *)malloc((unsigned)sizeof(struct att))) == NULL)
				errexit(1, "Out of memory for attributes\n");
			p = &buf[1];

			switch (*p) {
			case '?':
			case '-':
			case '!':
				a->a_flag = *p++;
				break;

			default:
				a->a_flag = '\0';
			}

			if ((p = strtok(p, "\n\t")) == NULL)
				errexit(1, "Attribute without prompt\n");

			if ((a->a_prompt = malloc((unsigned)strlen(p) + 1)) == NULL)
				errexit(1, "Out of memory for attribute prompt\n");
			strcpy(a->a_prompt, p);
			if ((pp = strchr(a->a_prompt, '[')) != NULL) {
				*pp = '\0';
				while (a->a_prompt[strlen(a->a_prompt) - 1] == ' ')
					a->a_prompt[strlen(a->a_prompt) - 1] = '\0';
				if ((a->a_def = malloc((unsigned)strlen(pp + 1) + 1))
				    == NULL)
					errexit(1, "Out of memory for attribute default\n");
				strcpy(a->a_def, pp + 1);
				if ((pp = strchr(a->a_def, ']')) == NULL)
					errexit(1, "Unmatched []\n");
				*pp = '\0';
			} else
				a->a_def = "";

			if ((p = strtok(NULL, "\t\n")) == NULL)
				errexit(1, "Attribute without refer\n");

			if ((a->a_refer = malloc((unsigned)strlen(p) + 1)) == NULL)
				errexit(1, "Out of memory for attribute refer\n");

			strcpy(a->a_refer, p);

			a->a_next = NULL;

			if ((p = strtok(NULL, "\n\t")) == NULL || !strcmp(p, "NULL"))
				a->a_mcont = "";
			else {
				if ((a->a_mcont = malloc((unsigned)strlen(p) + 1))
				    == NULL)
					errexit(1, "Out of memory for attribute continue\n");
				strcpy(a->a_mcont, p);
			}

			if ((p = strtok(NULL, "\n\t")) == NULL || !strcmp(p, "NULL"))
				a->a_econt = "";
			else {
				if ((a->a_econt = malloc((unsigned)strlen(p) + 1))
				    == NULL)
					errexit(1, "Out of memory for attribute continue\n");
				strcpy(a->a_econt, p);
			}

			break;

		default:
			addref(&r, buf);
			a = NULL;
		}
	}
	fclose(template);

	addref(&r, "help");
	addref(&r, "quit");
	r->r_next = NULL;
}

/*
 * Add a reference with prompt.
 */
addref(r, prompt)
register struct ref **r;
char *prompt;
{
	if (ref == NULL) {
		if ((*r = ref = (struct ref *)malloc((unsigned)sizeof(struct ref)))
		    == NULL)
			errexit(1, "Out of memory for reference.\n");
	} else if ((*r = (*r)->r_next = (struct ref *)malloc((unsigned)sizeof(struct ref)))
	    == NULL)
		errexit(1, "Out of memory for references\n");

	if (((*r)->r_prompt = malloc((unsigned)strlen(prompt) + 1)) == NULL)
		errexit(1, "Out of memory for reference prompt\n");
	strcpy((*r)->r_prompt, prompt);
	(*r)->r_next = NULL;
	(*r)->r_att = NULL;
}

/*
 * Get a line to process; if ~e or ~v call editors.
 */
getln(msg, str, sz)
register char *msg, *str;
int sz;
{
	for (;;) {
		printf(msg);
		if (fgets(str, sz, stdin) == NULL)
			return(NULL);

		if (!strcmp(str, "~e\n"))
			edit(EDITOR);
		else if (!strcmp(str, "~v\n"))
			edit(VISUAL);
		else
			return(1);
	}
}

char
	editor[80],
	veditor[80]
;

/*
 * Call one of the editors on the text entered so far.
 */
void
edit(which)
char *which;
{
	char command[100];

	if (!editor[0])
		open_ed(editor, EDITOR);
	if (!veditor[0])
		open_ed(veditor, VISUAL);

	if (tmp)
		fclose(tmp);
	else {
		printf("There is no file yet.\n");
		return;
	}

	sprintf(command, "%s %s", (!strcmp(which, EDITOR) ? editor : veditor),
	    tmpref);
	system(command);
	tmp = fopen(tmpref, "a+");
}

/*
 * Get user's editors as set in environment or else use ed or vi.
 */
open_ed(ed, which)
register char *ed, *which;
{
	char *p;
	struct stat e_stat;

	if (p = getenv(which))
		strcpy(ed, p);
	if (!ed[0] || stat(ed, &e_stat) == -1)
		strcpy(ed, (!strcmp(which, EDITOR) ? "/bin/ed" : "/usr/bin/vi"));
}

/*
 * Write out to file.
 */
void
fixrefer()
{
	char
		buf[BUFSIZ],
		savestr[BUFSIZ]
	;
	register char
		*bufp,
		*p
	;
	register struct ref *r;
	register struct att
		*a,
		*t
	;
	int
		first = 1,
		printed
	;

	if ((tmp = fopen(tmpref, "r")) == NULL)
		return;

	if ((r = sr) == NULL)
		return;

	a = r->r_att;
	if (fgets(buf, sizeof buf, tmp) == NULL)
		return;
#ifdef	PREFER
	putrefer(refer, "%s %s\n", TYPE, r->r_prompt);
#endif	/* PREFER */

	if ((p = strchr(buf, '\n')) != NULL)
		*p = '\0';

	for (t = a; t; t = t->a_next)
		if (!strncmp(t->a_prompt, buf, strlen(t->a_prompt)))
			break;

	if (t) {
		bufp = buf + strlen(t->a_prompt);
		if (*bufp == ':')
			bufp += 2;
	} else
		return;
	a = t;
	if (a->a_flag != '!')
		strcpy(savestr, bufp);
	while (fgets(&buf[0], sizeof(buf), tmp) != NULL) {
		if ((p = strchr(buf, '\n')) != NULL)
			*p = '\0';

		for (t = a; t; t = t->a_next) {
			if (!strncmp(t->a_prompt, buf, strlen(t->a_prompt)))
				break;
		}

		if (t && t != a) {
			if (first && a->a_flag != '!')		/* first line */
				putrefer(refer, "%s %s\n", a->a_refer, savestr);
			else if (a->a_flag != '!')		/* last line */
				putrefer(refer, "%s %s\n", a->a_econt, savestr);
			first = TRUE;
			a = t;
			bufp = buf + strlen(t->a_prompt);
			if (*bufp == ':')
				bufp += 2;
		} else {
			printed = FALSE;
			if (first && a->a_flag != '!')		/* first line */
				printed = putrefer(refer, "%s %s\n", a->a_refer,
				    savestr);
			else if (a->a_flag != '!')		/* middle line */
				putrefer(refer, "%s %s\n", a->a_mcont, savestr);
			if (printed)
				first = FALSE;
			bufp = (t ? buf + strlen(t->a_prompt) : buf);
			if (*bufp == ':')
				bufp += 2;
		}
		strcpy(savestr, bufp);
	}
	if (first)
		putrefer(refer, "%s %s\n", a->a_refer, savestr);
	else
		putrefer(refer, "%s %s\n", a->a_econt, savestr);
	fprintf(refer, "\n");
}

/*
 * Print string onto fp, or nothing if arg2 is NULL.
 */
putrefer(fp, str, arg1, arg2)
FILE *fp;
char *str, *arg1, *arg2;
{
	if (strlen(arg2)) {
		fprintf(fp, str, arg1, arg2);
		return(1);
	}
	return(0);
}

/*
 * End of program.
 */
cleanup()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (tmp) {
		fflush(tmp);
		fclose(tmp);
	}
	fixrefer();
	unlink(tmpref);
	printf("\n");
	exit(0);
}

/*
 * Open template file.
 */
FILE *
tmpopen(file)
char *file;
{
	FILE *fp;

	if ((fp = fopen(file, "r")) == NULL)
		errexit(1, "Can't open template file, %s\n", file);
	return(fp);
}

/*
 * Find matches for buf in ref; set rr to last match.
 */
match(rr, buf)
register struct ref **rr;
char *buf;
{
	register struct ref *r;
	register matches;

	for (r = ref, matches = 0; r; r = r->r_next)
		if (!strncmp(buf, r->r_prompt, strlen(buf))) {
			++matches;
			*rr = r;
		}

	if (matches >= 2) {
		printf("`%s' is not unique: ", &buf[0]);
		for (r = ref; r; r = r->r_next)
			if (!strncmp(buf, r->r_prompt, strlen(buf)))
				printf("`%s' ", r->r_prompt);
		printf("all match\n");
	}
	return(matches);
}
