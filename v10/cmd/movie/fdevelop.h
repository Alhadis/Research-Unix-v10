/* develop.h: header file for develop */

#include <stdio.h>

typedef struct Symbol { /* symbol table entry */
	struct Symbol	*next;
	char	*instr;
	int	innum;
	int	outnum;
} Symbol;

#define	eq(s, t)	(strcmp(s,t) == 0)

extern char	*cmdname;
extern int	lineno;

extern char	*NULLSTR;
extern Symbol 	*lookup();
extern 		insert(), delete(), opensymtab(), closesymtab();

#define FATAL	1
#define WARN	0
extern 		error();
extern char	*emalloc();
extern		efree();

/* MAXSTR is for view names, click names, etc.
   MAXBUF is for text strings
 */
#define MAXSTR	40
#define MAXBUF	500
extern int	moreinput();
extern		lex(), lexstr(), gobble();
extern		char buf[];
extern		lexsaweof;

extern void	exit();
extern char	*mktemp();
