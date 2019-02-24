/*
	Program to split file(s) containing Fortran, Ratfor, or Efl
	procedures into separate files, one per procedure.
	Procedure X is put in file X.f, X.r, or X.e depending on
	the language option (-f, -r, -e); Fortran (-f) is default.
	The -s option causes Fortran procedures to be stripped to 72
	or fewer characters, with trailing blanks removed.
        The -i option forces filenames to lower case.
*/

#include <stdio.h>
#include <ctype.h>
#define BIG 1000

#define NO	0
#define YES	1

#define FORTRAN	0
#define RATFOR	1
#define EFL	2

int blkdatano 	= 0;
int language	= FORTRAN;
char suffix	= 'f';
int strip	= NO;
int iflag;

#define SKIP	while( isspace(*s) ) ++s;


main(argc, argv)
int argc;
char **argv;
{
FILE *fd;
int i;
register char *s;

while(argc>1 && argv[1][0]=='-')
	{
	for(s = argv[1]+1 ; *s ; ++s)
		switch(*s)
			{
			case 'y':
				fprintf(stderr,"split: -y obsolete; -i assumed\n");
			case 'i':
				iflag++;
				break;

			case 's':
				strip = YES;
				break;

			case 'f':
				language = FORTRAN;
				suffix = 'f';
				break;

			case 'r':
				language = RATFOR;
				suffix = 'r';
				break;

			case 'e':
				language = EFL;
				suffix = 'e';
				break;

			default:
				fprintf(stderr, "bad option %c\n", *s);
				exit(1);
			}

	--argc;
	++argv;
	}

if(strip && language!=FORTRAN)
	fprintf(stderr, "implausible to strip non-Fortran programs\n");


if(argc <= 1)
	splitup(stdin);

else for(i = 1 ; i < argc ; ++i)
	{
	if( (fd = fopen(argv[i], "r")) == NULL)
		{
		fprintf(stderr, "cannot open %s\n", argv[i]);
		exit(1);
		}
	splitup(fd);
	fclose(fd);
	}

exit(0);
}



splitup(fin)
FILE *fin;
{
FILE *fout;
char in[BIG], fname[20], *s;
int i, c;

while( fgets(in,BIG,fin) )
	{
	if( *in=='c' || *in=='C' || *in=='*' )
		continue;
	s = in;
	SKIP
	if (*s=='\0' || *s=='\n' )
		continue;
	if(strip)
		shorten(in);

	getname(s, fname);
	if(iflag)
		lowercase(fname);
	if( (fout = fopen(fname, "w")) == NULL)
		{
		fprintf(stderr, "can't open %s", fname);
		exit(1);
		}
	fputs(in,fout);
	while( !endcard(in) && fgets(in, BIG, fin) )
		{
		if(strip)
			shorten(in);
		fputs(in, fout);
		}
	fclose(fout);
	}
}



lowercase(s)
	register char *s;
{
	do
		if(isupper(*s))
			*s=tolower(*s);
	while(*s++);
}


getname(s,f)
char *s,*f;
{
int i,j,c;
   loop:
	if( compar(&s,"subroutine") )	goto bot;
	else if( compar(&s,"function") )	goto bot;
	else if( compar(&s,"procedure") )	goto bot;
	else if( compar(&s,"program") )	goto bot;
	else if( compar(&s,"real") )	goto loop;
	else if( compar(&s,"integer") )	goto loop;
	else if( compar(&s,"logical") )	goto loop;
	else if( compar(&s,"double") )	goto loop;
	else if( compar(&s,"precision") )	goto loop;
	else if( compar(&s,"complex") )	goto loop;
	else if( compar(&s,"character") ) goto loop;
	else if( compar(&s,"*") )	/* complex *16  etc */
		{
		for( ++s ; isdigit(*s) || isspace(*s)  ; ++s)
			;
		goto loop;
		}
	else if( compar(&s,"blockdata") )
		{
		SKIP
		if(*s == '\0')	/* no block data name */
			{
			sprintf(f, "BLOCKDATA%d.%c", ++blkdatano, suffix);
			return;
			}
		goto bot;
		}
	else
		s = "";

bot:
	SKIP
	for(i=0 ; isalpha(*s) || isdigit(*s) ; i++)
		f[i] = *s++;
	if(i > 0)
		{
		f[i++] = '.';
		f[i++] = suffix;
		f[i++] = '\0';
		}
	else
		sprintf(f, "MAIN.%c", suffix);
}

/* compare two strings for equality.  assume that
   t is all lower case.  ignore blanks and decase s
   during comparison.  s0 points to next character after
   successful comparison.
*/
compar(s0, t)
char **s0,*t;
{
register char *s;
register int s1;
s = *s0;
while( *t )
	{
	SKIP
	s1 = *s++;
	if(isupper(s1))
		s1 = tolower(s1);
	if(s1 != *t++)
		return(NO);
	}
*s0 = s;
return(YES);
}


endcard(s)
char *s;
{
register int i;

if( *s==0 )
	return(YES);
SKIP
if( s[0]!='e' && s[0]!='E' )
	return(NO);
if( s[1]!='n' && s[1]!='N' )
	return(NO);
if( s[2]!='d' && s[2]!='D' )
	return(NO);
for(i = 3; i<66; ++i)
	if(s[i] == '\n')
		return(YES);
	else if(s[i] != ' ')
		return(NO);
return(YES);
}



shorten(s0)
register char *s0;
{
register char *s, *s72;
s72 = s0 + 72;

for(s=s0 ; s<s72; ++s)
	if(*s=='\n' || *s=='\0')
		break;

while(s>s0 && s[-1]==' ')
	--s;
s[0] = '\n';
s[1] = '\0';
}
