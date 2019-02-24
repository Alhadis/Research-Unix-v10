#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RENBASE 	100
#define RENINCR 	10
#define MAXWORD 	16

char		*tempfile = "_basicedit";
static char	*cmds[] = {
	"old", "save", "bye", "dump", "edit", "vi", "!", "fp",
	"int", "load", "renumber", "list", "del", "scr", "run",
	"clear", "auto", "catalog", "unsave", "bsave", "ren", "new", 
	"spool", 0
	};

Linep		findline();
Lnr		cvtlnr();


/*
 * moncmd --- determine if the current line contains a monitor command
 */

moncmd()
{
	register int	i;
	register char	*s;
	char		*file, word[MAXWORD];
	int		line1, line2, line3, line4;

	inptr = line;
	skipbl();
	getword(word, MAXWORD);
	for (i = 0; s = cmds[i]; ++i) {
		if (*word == *s && strcmp(word, s) == 0) {
			inptr += strlen(s);
			skipbl();
			switch (i) {
			case 0: 	/* old */
			case 9: 	/* load */
				old(inptr);
				fprintf(stderr, "%d lines\n",linecnt);
				break;
			case 22:	/* spool - listing & run on lp 
				file = (*inptr == 0)? curfile : inptr;
				exec("/bin/spool", "spool", file, 0);	*/
				break;
			case 1: 	/* save */
			case 19:	/* bsave */
				file = (*inptr == 0)? curfile : inptr;
				if (i == 1)	/* save */
					save(file);
				else		/* bsave */
					fastsave(file);
				fprintf(stderr, "%s saved\n", file);
				break;
			case 2: 	/* bye */
				exit(0);
			case 3: 	/* dump */
				dumpsym();
				break;
			case 4: 	/* edit */
				edit("/usr/ucb/edit");
				break;
			case 5: 	/* vi */
				edit("/usr/ucb/vi");
				break;
			case 6: 	/* ! */
				exec("/bin/sh", "sh", "-c", inptr, 0);
				break;
			case 7: 	/* fp */
				apple();
				prompt = "]";
				break;
			case 8: 	/* int */
				apple();
				prompt = ">";
				break;
			case 20:	/* ren	*/
			case 10:	/* renumber */
				line1 = RENBASE;
				line2 = RENINCR;
				getlnrs2(&line1, &line2, DEFAULTLNRS);
				if (line1 == 0)
					line1 = RENBASE;
				if (line2 == 0)
					line2 = RENINCR;
				getlnrs(&line3, &line4, LISTLNRS);
				renumber(line1, line2, line3, line4);
				break;
			case 11:	/* list */
				getlnrs(&line1, &line2, LISTLNRS);
				list(line1, line2, stdout);
				break;
			case 12:	/* delete */
				getlnrs(&line1, &line2, LISTLNRS);
				delete(line1, line2);
				break;
			case 21:
			case 13:	/* scratch */
				delete(MINLNR, MAXLNR);
				clrstk();
				clrsym();
				break;
			case 14:	/* run */
				run();
				break;
			case 15:	/* clear */
				clrstk();
				clrsym();
				break;
			case 16:	/* auto */
				line1 = RENBASE;
				line2 = RENINCR;
				getlnrs(&line1, &line2, DEFAULTLNRS);
				autonumber(line1, line2);
				break;
			case 17:	/* catalog */
				exec("/bin/ls", "ls", inptr, 0);
				break;
			case 18:	/* unsave */
				exec("/bin/rm", "rm", inptr, 0);
				break;
			default:
				return (NO);
				}
			return (YES);
			}
		}
	return (NO);
}


/*
 * getword --- fetch next word from input line
 */

getword(word, len)
char	*word;
{
	register char	*p, *w;
	register int	c;
	
	p = inptr;
	w = word;
	if (*p == '!')
		*w++ = *p;
	else
		do {
			c = *p++;
			if (isupper(c))
				c = tolower(c);
			if (islower(c))
				*w++ = c;
			else
				break;
			} while (--len > 0);
	*w = 0;
}
		

/*
 * apple ---
 */

apple()
{

	fprintf(stderr, "Now in Apple mode\n");
}


/*
 * edit --- invoke and editor on the current program
 */

edit(file)
char	*file;
{
	struct stat	statb;
	long		now;

	save(tempfile);
	time(&now);
	exec(file, file, tempfile, 0);
	stat(tempfile, &statb);
	if (statb.st_mtime > now)
		old(tempfile);
	unlink(tempfile);
}


/*
 * exec --- execute the given command
 */

exec(file, args)
char	*file, **args;
{
	int		status;
	register int	pid, i, (*oldattn)();

	if ((pid = fork()) == 0) {
		closeall();
		execv(file, &args);
		fprintf(stderr, "no %s\n", file);
		exit(1);
		}
	oldattn = signal(SIGINT, SIG_IGN);
	while ((i = wait(&status)) != -1 && i != pid)
		;
	signal(SIGINT, oldattn);
	return (status);
}


/*
 * chain --- replace the current program with a new one and execute it
 */

chain()
{
	int		len;
	char		*ptr, file[32];
	Lnr		lnr;

	expr();
	popstring(&ptr, &len);
	move(len, ptr, file);
	file[len] = 0;
	if (*inptr == COMMA) {
		++inptr;
		lnr = cvtlnr();
		}
	else
		lnr = MINLNR;
	endchk();
	old(file);
	curline = findline(lnr, NEXTLNR);
	inptr = NULL;
}


/*
 * notimpl --- report an unimplemented construct
 */

notimpl()
{

	err("not implemented");
}
