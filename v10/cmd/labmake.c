/*  lablist and labmake commands */

#include <stdio.h>
#include <pwd.h>


char direct[] = "/usr/spool/dpd";

FILE *fin, *fout;

#define NO	0
#define YES	1



#define FIRST 12
int dbgflag = NO;
int mailflag = 0;
int lablist ;

char df[30];
char tf[30];
struct passwd *pwp, *getpwuid();

struct par
	{
	char *name;
	char *value;
	}  pars[20];



main(argc,argv)
int argc;
char **argv;
{
int i, j, pid, unit;
char c, *p, *q, *val();

for(p=q=argv[0] ; *p!= '\0' ; ++p)
	if(*p == '/') q = p+1;
lablist = ! strcmp(q, "lablist");

pid = getpid();
sprintf(df, "%s/df%dxx", direct, pid);
sprintf(tf, "%s/tf%dxx", direct, pid);

pwp = getpwuid( getuid() );
if(pwp == NULL)
	{
	fprintf(stderr, "bad getpwuid!\n");
	exit(3);
	}
if(pwp->pw_gecos && pwp->pw_gecos[0]!='\0')
	setval("job", pwp->pw_gecos);
setval("copies", "1");
setval("lines", "6");
setval("forms", "avery");
setval("each", "1");
setval("sel", "");

for(i=1; i<argc; ++i)
	if(argv[i][0]=='-'  &&  argv[i][1]!='\0')
		{
		for(j=1; (c=argv[i][j])!='\0' ; ++j)
			switch(c)
			{
			case 't':
				dbgflag = YES;
				break;

			case 'l':
				lablist = YES;
				break;

			case 'm':
				mailflag = YES;
				break;

			default:
				fprintf(stderr,"Bad flag %c\n", c);
				exit(1);
			}
		argv[i] = 0;
		}

	else	{
		for(p = argv[i] ; *p!='\0' ; ++p)
			if(*p == '=')
				{
				*p = '\0';
				setval(argv[i], p+1);
				argv[i] = 0;
				break;
				}
		}

if(dbgflag)
	fout = stdout;
else if( (fout = fopen(tf,"w")) == NULL)
	err("Cannot open temporary file");

if(mailflag)
	fprintf(fout, "M%s\n", pwp->pw_name);
if(!val("job"))
	{
	setval("job", "m5192,m044");
	fprintf(stderr, "Output will be put in box M044\n");
	}

fprintf(fout, "S\nL$\tSGRADE\t2\nL$\tIDENT\t%s,LABELS\n", val("job") );
fprintf(fout, "L$\tREMOTE\t**,ONL\n");
fprintf(fout, "L$\tOPTION\tFORTRAN\nL$\tSELECT\t./LAB.OBJ\n");

if(lablist)
	fprintf(fout, "L$\tEXECUTE\tON1\n");
else	fprintf(fout, "L$\tEXECUTE\n");

fprintf(fout, "L$\tLIMITS\t10,12K\nL$\tFILE\tP*,NULL\nL$\tSYSOUT\t06\n");
if(lablist)
	fprintf(fout, "L$\tSYSOUT\t42\n");
else	fprintf(fout, "L$\tMASS\t42,X20S,10L\nL$\tFFILE\t42,NOSLEW\n");

fprintf(fout, "$L\tDATA\t05\n");
fprintf(fout, "L%s-lines\nL%s\n", val("lines"), val("sel") );

unit = FIRST;
for(i=1 ; i<argc ; ++i)
	if(argv[i] != 0)
		fprintf(fout, "L*%d %s\n", unit++ , argv[i]);

if(unit == FIRST)
	fprintf(fout, "L*%d standard input\n", FIRST);

unit = FIRST;
for(i=1 ; i<argc; ++i)
	if(argv[i] != 0)
		{
		fprintf(fout, "L$\tDATA\t%d\n", unit++);
		if(argv[i][0]=='-' && argv[i][1]=='\0')
			fin = stdin;
		else if( (fin = fopen(argv[i],"r")) == NULL)
			{
			fprintf(stderr,"Cannot open file %s", argv[i]);
			err("");
			}

		copyfile();
		fclose(fin);
		}

if(unit == FIRST)
	{
	fprintf(fout, "L$\tDATA\t%d\n", FIRST);
	copyfile();
	}

if(!lablist)
	{
	if( (p = val("nc")) || (p = val("copy")) )
		setval("copies", p);

	if(p = val("neach"))
		setval("each", p);
	fprintf(fout, "L$\tSELECT\tCC/LABEL1\n");

	fprintf(fout, "L%s-each  %s  %s-lines  %s-copies\n",
		val("each"), val("forms"),
		val("lines"), val("copies") );

	fprintf(fout, "L$\tMASS\tIN,X20D\nL$\tSELECT\tCC/LABEL2\n");
	}

fprintf(fout, "L$\tENDJOB\n");

if(!dbgflag)
	{
	fclose(fout);
	if( link(tf,df) )
		err("Cannot make df file");
	unlink(tf);
	execl("/etc/dpd", "/etc/dpd", 0);
	err("Couldn't invoke /etc/dpd");
	}

exit(0);
}


char *val(s)
register char *s;
{
register struct par *p;

for(p=pars; p->name!=0 ; ++p)
	if(! strcmp(p->name, s))
		return(p->value);

return(0);
}


setval(n,v)
char *n, *v;
{
register struct par *p;

for(p=pars ; p->name!=0 && strcmp(p->name,n) ; ++p)
	;

p->name = n;
p->value = v;
}




err(s)
char *s;
{
fprintf(stderr, "%s\n", s);
unlink(tf);
exit(1);
}





copyfile()	/* precede every line with a letter L */
{
char line[200];

line[0] = 'L';

while( fgets(line+1,199, fin) )
	fputs(line, fout);
}
