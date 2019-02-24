/*	@(#)nmf.c	1.5	83/08/05	*/
#if defined(UNIX5) && !defined(pdp11)
#include "stdio.h"

main(argc, argv)
char	*argv[];
{
	char name[BUFSIZ], buf[BUFSIZ], *fname = NULL, *pty, *strncpy();
	register char *p;
	int nsize, tysize, lineno;

	strcpy(name, argc > 1? argv[1] : "???");
	if (argc > 2)
		fname = argv[2];
	else
		fname = "???";
	while (gets(buf))
	{
		p = buf;
		while (*p != ' ' && *p != '|')
			++p;
		nsize = p - buf;
		do ; while (*p++ != '|');		/* skip rem of name */
		do ; while (*p++ != '|');		/* skip value */
		do ; while (*p++ != '|');		/* skip class */
		while (*p == ' ')
			++p;
		if (*p != '|')
		{
			pty = p++;
			while (*p != '|')
				++p;
			tysize = p - pty;
		}
		else
			pty = (char *) NULL;
		++p;
		do ; while (*p++ != '|');		/* skip size */
		while (*p == ' ')
			++p;
		lineno = atoi(p);
		do ; while (*p++ != '|');		/* and xlated line */
		while (*p == ' ')
			++p;
		if (!strncmp(p, ".text", 5) || !strncmp(p, ".data", 5))
		{					/* it's defined */
			strncpy(name, buf, nsize);
			name[nsize] = '\0';
			printf("%s = ", name);
			if (pty)
				printf("%.*s", tysize, pty);
			else
			{
				fputs("???", stdout);
				if (!strncmp(p, ".text", 5))
					fputs("()", stdout);
			}
			printf(", <%s %d>\n", fname, lineno);
		}
		else
			printf("%s : %.*s\n", name, nsize, buf);
	}
	exit(0);
}
#else
#include "stdio.h"

#if u3b || u3b5 || vax
#define SYMSTART 11
#define SYMCLASS 9
#endif

#if pdp11
#define SYMSTART 9
#define SYMCLASS 7
#endif

main(argc, argv)
char	*argv[];
{
	char	name[15], buf[64];
	char	*fname = NULL;
	char	*p;

	strcpy(name, argc > 1? argv[1] : "");
	if (argc > 2)
		fname = argv[2];
	while (gets(buf)) {
		p = &buf[SYMSTART];
		if (*p == '_')
			++p;
		switch (buf[SYMCLASS]) {
		case 'U':
			printf("%s : %s\n", name, p);
			continue;
		case 'T':
			printf("%s = text", p);
			strcpy(name, p);
			break;
		case 'D':
			printf("%s = data", p);
			if (strcmp(name, "") == 0)
				strcpy(name, p);
			break;
		case 'B':
			printf("%s = bss", p);
			break;
		case 'A':
			printf("%s = abs", p);
			break;
		default:
			continue;
		}
		if (fname != NULL)
			printf(", %s", fname);
		printf("\n");
	}
	exit(0);
}
