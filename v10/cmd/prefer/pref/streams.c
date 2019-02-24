# include "stdio.h"
# include "streams.h"
# include "ctype.h"

char *strncpy();
void exit();

#define IWORD	20
#define	IMAX	5
char igntab[IWORD][IMAX];
int igcnt;

/*  getword(stream,p):
        read next sequence of alpha-numberic on current line into *p.
    null if no more words on current line.
    *p is a null terminated string (char p[maxstr]).
*/
getword(stream,p)
FILE *stream;
char *p;
{   
	char c;
	char *oldp, *stop;
	register int i;
	static ignored = 0;

	oldp= p;
	stop= p+maxstr-1;
	do { 
		c= getc(stream);
		if(c == EOF) {
			*p = NULL;
			return;
		}
	}   while (!isalnum(c) && c!='%');

	do {
		*p= c;
		if (p < stop)  p++;
		c= getc(stream);
	} while(isalnum(c));
	*p= NULL;
	if (oldp[0]=='%') {   
		oldp[0]= NULL;
		ignored = 0;
		for(i=0;i<igcnt;i++) {
			if(strncmp(&igntab[i][0],&oldp[1],IMAX-1)== 0) {
				ignored = 1;
				while(c != '\n' && c != EOF) c=getc(stream);
				break;
			}
		}
	}
	else if (ignored) {
		oldp[0] = NULL;
		while(c != '\n' && c != EOF) c=getc(stream);
	}
}



/*  recsize(stream,start):
    returns length of record beginning at start
    (record ends at blank line or eof)
    assumes and retains stream positioned at start
*/
long int recsize(stream,start)
FILE *stream;
long int start;
{
	char line[256];
	long int length;

	while(fgets(line,256,stream)) {
		if(line[0] == '\n') {
			length = ftell(stream)-start-1;
			pos(start);
			return(length);
		}
	}
	length = ftell(stream)-start;
	pos(start);
	return(length);
}

/*  nextrecord(stream,x): seeks in stream for first non-blank line
        at or after char x in stream. seeks to eof if x is past last record.
        x is the index of a character in the file (not eof).
    returns position in stream.  (returns EOF, if seeks to EOF)
*/
long int nextrecord(stream,x)
FILE *stream;
long int x;
{   
	long int start;         /*  position of the beginning of the line  */
	char c;                 /*      containing c                       */

	pos(x);
	start= x;
	/*  find start of first non-blank record        */
	for(;;){   
		c= getc(stream);
		if      (c=='\n')           start= ftell(stream);
		else if (!isspace(c))       break;
	}

	if (feof(stream))  { 
		pos(start);  
		start= EOF;  
	}
	else pos(start);
	return(start);
}

/*  nextline(stream,x): seeks in stream after first newline at or after
        char x in stream. seeks to eof if x is in last line.
        x is the index of a character in the file (not eof).
    returns position in stream
*/
long int nextline(stream,x)
FILE *stream;
long int x;
{   
	pos(x);
	while (getc(stream)!='\n') ;
	return(ftell(stream));
}


/*  printline(stream): copies stream up to a newline
*/
printline(stream)
FILE *stream;
{   
	int c;
	while ((c=getc(stream)) != '\n' && c!=EOF)  putchar(c);
	putchar('\n');
}

/*  getline(stream,p):  store in *p next chars in stream up to \n
        advance stream past \n.
    limit of  maxstr-1 chars may be stored at p.
*/
getline(stream,p)
FILE *stream;
char *p;
{   
	char *stop;
	int ci;

	stop= p+maxstr-1;
	while (((ci= getc(stream)) != '\n') && (ci!=EOF)) {
		if (ci == EOF)
			break;
		*p = ci;
		if (p<stop)    p++;
	}
	*p= NULL;
}


#define MAXL	512
#define MIN(a,b)	(a<b?a:b)
load_ign(istring)
char *istring;
{
	FILE *fig;
	char line[MAXL];

	if((fig = fopen(istring,"r")) == NULL) {
		fprintf(stderr,"invert: error reading %s\n",istring);
		return(-1);
	}
	igcnt=0;
	while(fgets(line,MAXL,fig)) {
		if(igcnt == IWORD) {
			fprintf(stderr,
			    "load_ign: warning:too many words in ignore file\n");
			return(igcnt);
		}
		strncpy(&igntab[igcnt++][0],line+1,MIN(strlen(line+1)-1,IMAX-1));
	}
	return(igcnt);
}
