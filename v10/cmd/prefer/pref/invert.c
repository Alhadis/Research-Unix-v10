/*
 * pinvert - create inverted index to a bibliographic database
 *	input:	records of lines, separated by blank lines
 *	output:	key:file1 start/length ... start/length:file2 start/length ...
 */

#include	<signal.h>
#ifdef BSD
#include	<strings.h>
#else
#include	<string.h>
#endif
#include "stdio.h"
#include "streams.h"
#include "bib.h"

#define isnull(x)  (*(x) == NULL)
#define makelow(c) ('A'<=(c) && (c)<='Z' ? (c)-'A'+'a' : c)
#define INVTEMPFILE "/tmp/invertXXXXXX"		/* tmp index */
#define HEADTEMPFILE "/tmp/headXXXXXX"		/* tmp header */

char *strrchr(), *mktemp();

int     max_kcnt = 100;		/* max number of keys */
int     max_klen =   MAXKLEN;	/* max length of keys */
char 	*plib =	PLIB;		/* library file */
char    *common;		/* name of file of common words */
char	*ignore;		/* name of file of %xxx to ignore */
char    *tempfile = INVTEMPFILE;	/*  name of temporary file */
char    *tmphead = HEADTEMPFILE;	/*  name of temporary file for header */
char	*DINPUT = BIBFILE;	/* default input file */
char	INDEX[maxstr];		/* output index file */
char	HEAD[maxstr];		/* output header file */
int	silent = 1;		/* 0 => statistics printed */

char	mvcmd[maxstr];
char	path_ign[maxstr];

FILE	*input, *output, *head;
char	*sort_it = "sort -u %s -o %s";
char	sortcmd[maxstr];

char	*libpath();
int	cleanup();

int     argc;
char    **argv;

#define USAGE	"pinvert [-c common -i ignore -k nkeys -l length -p database -v] [file...]\n"

main(argcount,arglist)
int argcount;
char **arglist;
{
	char            *filename;
	long int        start,length;
	char            word[maxstr];
	int             kcnt;
	char            tag_line[maxstr];
	char		outstring[maxstr];
	char		lenstring[5];
	char *f;
	long int	records = 0;  /*  number of records read           */
	long int	keys    = 0;  /*  number of keys read (occurences) */
	long int	distinct;     /*  number of distinct keys          */
	long int	shorten();
	int first = 1;
	int stat;

	/* initialize and open files */
	argc= argcount-1;
	argv= arglist+1;
	mktemp(tempfile);
	mktemp(tmphead);
	if(signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, cleanup);
	signal(SIGQUIT, cleanup);
	output= fopen(tempfile,"w");
	head = fopen(tmphead,"w");

	/* make path names */
	common = COMFILE;
	strcpy(path_ign,libpath(plib));
	strcat(path_ign,"/");
	strcat(path_ign,IGNFILE);
	ignore = path_ign;
	INDEX[0] = NULL;
	HEAD[0] = NULL;

	flags();
	if(load_ign(ignore) == -1) {
		baleout();
	}

	/* write out name of common file and max_klen to header */
	fprintf(head,"%s %d\n",common,max_klen);

	/* now index input files */
	for (; argc>=0 ; argc--, argv++) {
		if(argc == 0) {
			if(!first)
				break;
			else
				filename = DINPUT;
		}
		else {
			filename=   *argv;
		}

		first = 0;
		input = fopen(filename,"r");
		if (input==NULL) {
			fprintf(stderr, "pinvert: cannot open %s\n",
			    filename);
			baleout();
		}
		if(INDEX[0] == NULL) {
			strcpy(INDEX,filename);
			strcat(INDEX,".i");
			strcpy(HEAD,filename);
			strcat(HEAD,".h");
		}
		start=      0L;
		length=     0L;

		/* write out file name to header */
		if((f=strrchr(filename,'/')) != NULL)
			f++;
		else
			f = filename;
		fprintf(head,"%s\n",f);

		sprintf(lenstring,"%d",max_klen);
		strcpy(outstring,"%-");
		strcat(outstring,lenstring);
		strcat(outstring,"s%s");
		for(;;) {
			/* find start of next record (exit if none)  */
			start= nextrecord(input,start+length);
			if (start==EOF)   break;
			records++;
			kcnt= 0;
			length= recsize(input,start);
			sprintf(tag_line, " %-18s %08ld %08ld\n", f, start, length);

			while (ftell(input) < start+length && kcnt < max_kcnt) {
				getword(input,word);
				makekey(word,max_klen,common);
				if (!isnull(word)) {
					fprintf(output,outstring,word,tag_line);
					kcnt++;
					keys++;
				}
			}
		}
		fclose(input);
	}
	fclose(output);
	fclose(head);

	sprintf(sortcmd, sort_it, tempfile, tempfile);
	system(sortcmd);

	distinct = shorten(tempfile,INDEX);
	sprintf(mvcmd,"cp %s %s\n",tmphead,HEAD);
	if(stat = system(mvcmd)) {
		unlink(tmphead);
		exit(stat);
	}
	else {
		unlink(tmphead);
	}
	if( silent == 0 )
		fprintf(stderr,
		    "%ld documents   %ld distinct keys  %ld key occurrences\n",
		    records, distinct, keys);
	exit(0);
}


baleout()
{
	unlink(tempfile);
	unlink(tmphead);
	exit(1);
}

/*  Flag    Meaning                             Default
    -ki     Keys per record                     100
    -li     max Length of keys                  6
    -cfile  file contains Common words          /usr/lib/prefer/common
            do not use common words as keys
    -ifile  %xxx lines in input file to ignore 	/usr/lib/prefer/ignore
    -pfile  name of output file                 INDEX
    -s	    do not print statistics		statistics printed
*/

# define    operand     (strlen(*argv+2)==0 ? (argv++,argc--,*argv) : *argv+2)

flags()
{
	char *tmp;
	for (; argc>0 && *argv[0]=='-';  argc--,argv++) {
		switch ((*argv)[1]) {
		case 'k':
			max_kcnt= atoi(operand);
			break;
		case 'l':
			max_klen= atoi(operand);
			break;
		case 'c':
			common=  operand;
			break;
		case 'i':
			ignore=  operand;
			break;
		case 'p':
			tmp = operand;
			strcpy(INDEX,tmp);
			strcat(INDEX,".i");
			strcpy(HEAD,tmp);
			strcat(HEAD,".h");
			break;
		case 'v':
			silent= 0;
			break;
		default:
			fprintf(stderr,USAGE);
			baleout();
		}
	}
}


/*  shorten(inf,outf): file "inf" consists of lines of the form:
        key file start length
    sorted by key and file.  replace lines with the same key
    with one line of the form:
        key:file1 start/length ... start/length:file2 start/length ...
    rename as file "outf"
    returns number of lines in output
*/
long shorten(inf,outf)
char *inf, *outf;
{
	FILE *in, *out;
	char line[maxstr];
	char key[maxstr],  newkey[maxstr],
	file[maxstr], newfile[maxstr];
	long int start, length;
	long int lines = 0;

	in=  fopen(inf, "r");
	out= fopen(outf, "w");
	if (in==NULL || out==NULL) {
		fprintf(stderr, "pinvert: error in opening file for compression\n");
		return(0);
	}

	getline(in,line);
	sscanf(line,"%s%s%ld%ld", key, file, &start, &length);
	fprintf(out, "%s :%s %ld/%ld", key, file, start, length);
	for ( getline(in, line) ; !feof(in);  getline(in, line)) {
		sscanf(line,"%s%s%ld%ld", newkey, newfile, &start, &length);
		if (strcmp(key,newkey)!=0) {
			strcpy(key, newkey);
			strcpy(file, newfile);
			fprintf(out, "\n%s :%s %ld/%ld",  key, file, start, length);
			lines++;
		}
		else if (strcmp(file,newfile)!=0) {
			strcpy(file,newfile);
			fprintf(out, ":%s %ld/%ld", file, start, length);
		}
		else
			fprintf(out, " %ld/%ld", start, length);
	}
	fprintf(out, "\n");
	lines++;

	fclose(in);
	fclose(out);
	unlink(inf);
	return (lines);
}

cleanup()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	fclose(output);
	fclose(head);
	unlink(tempfile);
	unlink(tmphead);
	exit(1);
}
