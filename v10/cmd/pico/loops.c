#include <stdio.h>
#include "pico.h"

int uniq = 0;

Node *
newhile(lcondi, lbody)
	Node *lcondi, *lbody;
{
	Node *n = new(OCOMMA, lbody, new(GOTO, Z, Z, uniq), Z);

	return new(LABL, new(CONDI, n, Z, lcondi), Z, uniq++);
}

Node *
newuntil(lbody, lcondi)
	Node *lbody, *lcondi;
{
	Node *n;

	n = new(GOTO, Z, Z, uniq);
	n = new(OCOMMA, lbody, new(CONDI, n, Z, lcondi), Z);

	return new(LABL, n, Z, uniq++);
}

Node *
newloop(linit, lcondi, lincr, lbody)
	Node *linit, *lcondi, *lincr, *lbody;
{
	Node *n;

	n = new(OCOMMA, new(OCOMMA, lbody, lincr, Z), new(GOTO, Z, Z, uniq), Z);
	n = new(LABL, new(CONDI, n, Z, lcondi), Z, uniq++);

	return new(OCOMMA, linit, n, Z);
}

commacount(n)
	Node *n;
{
	if (n == Z)
		return 0;
	if (n->type != OCOMMA && n->type != ACOMMA)
		return 1;
	return commacount(n->right) + commacount(n->left);
}

argcount(nrp, n, s)
	long nrp;
	Node *n;
	char *s;
{
	int pcnt;

	if ((pcnt=commacount(n)) != (int) nrp)
	if (strcmp(s, "println") != 0 && strcmp(s, "printf") != 0)
	{	fprintf(stderr, "function %s takes %d ", s, nrp);
		fprintf(stderr, "argumen%s, ", (nrp==1)?"t":"ts");
		yyerror("found %d", pcnt);
	}
}
