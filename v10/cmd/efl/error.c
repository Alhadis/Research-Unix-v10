#include "defs"

char *linerr()
{
static char buff[50];
register int i;

for(i = filedepth; i>0 && filenames[i]==NULL ; --i)
	;
if(i > 0)
	sprintf(buff, "on line %d of file %s", yylineno, filenames[i]);
else
	sprintf(buff, "on line %d", yylineno);
return(buff);
}



laberr(s,t)
char *s;
char *t;
{
errmess("Label error", s, t);
}


labwarn(s,t)
char *s, *t;
{
char junk[100];
sprintf(junk, s, t);
warn1("Label warning: %s", junk);
}



exprerr(s,t)
char *s;
ptr t;
{
errmess("Expression error", s, t);
}




execerr(s,t)
char *s, *t;
{
errmess("Error", s, t);
}


errmess(m,s,t)
char *m, *s, *t;
{
fprintf(diagfile, "**%s %s:  ", m, linerr());
if(s)
	fprintf(diagfile, s, t);
fprintf(diagfile, "\n");
fflush(diagfile);
++nerrs;
}



dclerr(s, n)
char *s, *n;
{
extern int nerrs;

fprintf(diagfile, "**Error %s: Declaration for %s: %s\n",
		linerr(), n, s);
fflush(diagfile);
++nerrs;
}




badtag(routine, tag)
char *routine;
int tag;
{
char buff[100];
sprintf(buff, "impossible tag %d in routine %s", tag, routine);
fatal(buff);
}



fatal1(s,t)
char *s;
int t;
{

sprintf(msg, s, t);
fatal(msg);
}



fatal(s)
char *s;
{
fprintf(diagfile, "\n***Compiler error %s.", linerr());
if(s) fprintf(diagfile, "   %s.", s);
fprintf(diagfile, "\n");
fflush(diagfile);
fflush(stdout);

if(dumpcore)
	abort(0);
else	{
	rmiis();
	exit(-1);
	}
}



warn1(s,t)
char *s;
int t;
{
sprintf(msg, s, t);
warn(msg);
}




warn(s)
char *s;
{
++nwarns;
if( !nowarnflag) {
	fprintf(diagfile, "*Warning: %s\n", s);
	fflush(diagfile);
	}
}



yyerror(s)
char *s;
{
errmess(s, CNULL, CNULL);
}
