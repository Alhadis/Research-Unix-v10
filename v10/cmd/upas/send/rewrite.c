#include <stdio.h>
#include <regexp.h>
#include "mail.h"
#include "string.h"
#include "dest.h"
#include "aux.h"

/* configuration */
#define RULEFILE "rewrite"

/* 
 *	Routines for dealing with the rewrite rules.
 */

/* imports */
extern char *malloc();
extern regexp *regcomp();
extern int regexec();
extern void regsub();
extern void exit();
extern char *thissys;

/* globals */
typedef struct rule rule;

#define NSUBEXP 10
struct rule {
	string *matchre;	/* address match */
	string *repl1;		/* first replacement string */
	string *repl2;		/* second replacement string */
	d_status type;		/* type of rule */
	regexp *program;
	regsubexp subexp[NSUBEXP];
	rule *next;
};
static rule *rulep;

/* predeclared */
static string *substitute();

/*
 *  Get the next token from `line'.  The symbol `\l' is replaced by
 *  the name of the local system.
 */
string *
rule_parse(line)
	string *line;
{
	string *token;
	string *expanded = (string *)NULL;
	char *cp;

	token = s_parse(line, (string *)NULL);
	if(token == (string *)NULL)
		return(token);
	if ((cp = strchr(s_to_c(token), '\\'))==NULL)
		return(token);
	expanded = s_new();
	if (expanded == NULL) {
		s_free(token);
		return((string *)NULL);
	}
	for(cp = s_to_c(token); *cp; cp++) {
		if(*cp == '\\') switch(*++cp) {
		case 'l':
			s_append(expanded, thissys);
			break;
		case '\\':
			s_putc(expanded, '\\');
			break;
		default:
			s_putc(expanded, '\\');
			s_putc(expanded, *cp);
			break;
		} else
			s_putc(expanded, *cp);
	}
	s_free(token);
	s_terminate(expanded);
	return(expanded);
}

/*
 *  rules are of the form:
 *	<reg exp> <string> <repl exp> [<repl exp>]
 */
extern int
getrules()
{
	FILE	*rfp;
	rule	*rp, *rlastp=NULL;
	string	*line;
	string	*type;
	string	*file;
	string	*re;

	file = abspath(RULEFILE, UPASROOT, (string *)NULL);
	rfp = fopen(s_to_c(file), "r");
	if (rfp == NULL) {
		rulep = NULL;
		return -1;
	}
	line = s_new();
	type = s_new();
	while(s_getline(rfp, s_restart(line)) != NULL) {
		/* get a rule */
		rp = (rule *)malloc(sizeof(rule));
		if (rp == NULL) {
			perror("getrules:");
			exit(1);
		}
		rp->next = NULL;
		re = rule_parse(s_restart(line));
		s_tolower(re);
		rp->matchre = s_new();
		if(*s_to_c(re)!='^')
			s_append(rp->matchre, "^");
		s_append(rp->matchre, s_to_c(re));
		s_append(rp->matchre, "$");
		s_restart(rp->matchre);
		s_free(re);
		s_parse(line, s_restart(type));
		rp->repl1 = rule_parse(line);
		rp->repl2 = rule_parse(line);
		rp->program = NULL;
		if (strcmp(s_to_c(type), "|") == 0)
			rp->type = d_pipe;
		else if (strcmp(s_to_c(type), ">>") == 0)
			rp->type = d_cat;
		else if (strcmp(s_to_c(type), "alias") == 0)
			rp->type = d_alias;
		else if (strcmp(s_to_c(type), "translate") == 0)
			rp->type = d_translate;
		else if (strcmp(s_to_c(type), "auth") == 0)
			rp->type = d_auth;
		else {
			s_free(rp->matchre);
			s_free(rp->repl1);
			s_free(rp->repl2);
			free((char *)rp);
			fprintf(stderr,"illegal rewrite rule: %s\n", s_to_c(line));
			continue;
		}
		if (rulep == NULL)
			rulep = rlastp = rp;
		else
			rlastp = rlastp->next = rp;
	}
	s_free(type);
	s_free(line);
	s_free(file);
	fclose(rfp);
	return 0;
}

/* look up a matching rule */
static rule *
findrule(addrp, authorized)
	string *addrp;
{
	rule *rp;
	static rule defaultrule;
	string *matcher;

	if (rulep == NULL)
		return &defaultrule;
	for (rp = rulep; rp != NULL; rp = rp->next) {
		if (rp->type==d_auth && authorized)
			continue;
		if (rp->program == NULL)
			rp->program = regcomp(rp->matchre->base);
		if (rp->program == NULL)
			continue;
		if (regexec(rp->program, s_to_c(addrp), rp->subexp, NSUBEXP))
			return rp;
	}
	return NULL;
}

/*  Transforms the address into a command.
 *  Returns:	-1 if address not matched by reules
 *		 0 if address matched and ok to forward
 *		 1 if address matched and not ok to forward
 */
extern int
rewrite(dp, s)
	dest *dp;		/* destination to rewrite */
	char *s;		/* string that matches \s */
{
	rule *rp;		/* rewriting rule */
	string *lower;		/* lower case version of destination */
	char *cp;

	/*
	 *  Rewrite the address.  Matching is case insensitive.
	 */
	lower = s_clone(s_restart(dp->addr));
	s_tolower(s_restart(lower));
	rp = findrule(lower, dp->authorized);
	if (rp == NULL)
		return -1;
	strcpy(s_to_c(lower), s_to_c(dp->addr));
	dp->repl1 = substitute(rp->repl1, rp->subexp, s);
	dp->repl2 = substitute(rp->repl2, rp->subexp, s);
	dp->status = rp->type;
	return 0;
}

static string *
substitute(source, subexp, s)
	string *source;		/* substitution expression */
	regsubexp *subexp;	/* the matches */
	char *s;		/* string to substitute for \s */
{
	register char *sp;
	register char *ssp;
	register string *stp;
	register int i;
	
	if (source == NULL)
		return NULL;
	sp = s_to_c(source);

	/* someplace to put it */
	stp = s_new();

	/* do the substitution */
	while (*sp != '\0') {
		if (*sp == '\\') {
			switch (*++sp) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				i = *sp-'0';
				if (subexp[i].sp != NULL)
					for (ssp = subexp[i].sp;
					     ssp < subexp[i].ep;
					     ssp++)
						s_putc(stp, *ssp);
				break;
			case '\\':
				s_putc(stp, '\\');
				break;
			case '\0':
				sp--;
				break;
			case 's':
				for(ssp = s; *ssp; ssp++)
					s_putc(stp, *ssp);
				break;
			default:
				s_putc(stp, *sp);
				break;
			}
		} else if (*sp == '&') {				
			if (subexp[0].sp != NULL)
				for (ssp = subexp[0].sp;
				     ssp < subexp[0].ep; ssp++)
					s_putc(stp, *ssp);
		} else
			s_putc(stp, *sp);
		sp++;
	}
	s_terminate(stp);

	return s_restart(stp);
}

regerror(s)
char* s;
{
	fprintf(stderr, "rewrite: %s\n", s);
}

dumprules()
{
	rule *rp;

	for (rp = rulep; rp != NULL; rp = rp->next) {
		fprintf (stderr, "'%s'", rp->matchre->base);
		switch (rp->type) {
		case d_pipe:
			fprintf(stderr, " |");
			break;
		case d_cat:
			fprintf(stderr, " >>");
			break;
		case d_alias:
			fprintf(stderr, " alias");
			break;
		case d_translate:
			fprintf(stderr, " translate");
			break;
		default:
			fprintf(stderr, " UNKNOWN");
			break;
		}
		fprintf (stderr, " '%s'", rp->repl1 ? rp->repl1->base:"...");
		fprintf (stderr, " '%s'\n", rp->repl2 ? rp->repl2->base:"...");
	}
}

