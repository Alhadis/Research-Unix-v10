/*                                                      */
/* program to find monk |insert(filename) commands      */
/*   or |begin(insert) filename |end(insert) sequences  */
/*   in monk source file and actually insert the        */
/*   contents of filename so that the output would      */
/*   be the original monk source file with |insert's    */
/*   replaced by the contents of the file to be         */
/*   inserted plus comments to flag the insertion       */
/*     eg. |comment<inserting filename starting here>   */
/*         |comment<inserting filename ending here>     */
/*                                                      */
/* usage:                                               */
/*    monkmerge [file1 ... filen ]                      */
/*                                                      */
/*                                                      */

#include <stdio.h>
#include <string.h>
#include "monkmerge.h"

void	monkmerge(), ungetch(), get_file_name(), putin();
char	*mymalloc();

int	nfiles = 0;		/* number of files currently open            */
int	bufp = 0;		/* next free position in buf                 */   

struct	file_info filelist[MAXFILES];	/* info on files currently open */

char	buf[BUFSIZE];		/* buffer for ungetch                        */


/* set up space to save white space surrounding command and its contents        */
/* in case we need to output                                                    */
char	betw1[MAXC], betw2[MAXC], betw3[MAXC], betw4[MAXC], betw5[MAXC];
char	betw6[MAXC], betw7[MAXC], betw8[MAXC];

int	old_d, old_dx;		     /* values of delimiters in current command*/
int	delim_found, first_delim;    /* delimiter character encountered        */


main(argc, argv)
int	argc;
char	*argv[];
{

	FILE	*fp, *fopen();
	char	*p;

/* determine input and loop through                                 */

	if (argc == 1) {    /* no arguments: use standard input     */
		p=mymalloc((unsigned) 6);
		strcpy(p,"stdin");
		filelist[nfiles].name=p;
		filelist[nfiles++].line_count=1;
		monkmerge(stdin);
		myfree(p);
		--nfiles;
	} else {            /* argument(s) specify input file(s)    */
		while (--argc > 0) {
			if ((fp = fopen(*++argv, "r"))==NULL) {
				fprintf(stderr, "monkmerge: can't open %s\n",*argv);
				printf("|comment<cannot open %s>\n",*argv);
			} else {
				if (nfiles>MAXFILES-1) {
					printf(
		"|comment<too many files; cannot insert %s>\n",*argv);
					fprintf(stderr,
		"monkmerge: too many files; cannot insert %s\n",*argv);
				} else {
					p=mymalloc((unsigned) strlen(*argv) +1);
					strcpy(p,*argv);
					filelist[nfiles].name=p;
					filelist[nfiles++].line_count=1;
					monkmerge(fp);
					fclose(fp);
					myfree(p);
					--nfiles;
				}
			}
		}
	}
	exit(0);
}


/* main monkmerge routine                    */
void 
monkmerge(f)
FILE	*f;
{
	char	fname[MAXPATH];	          /* filename saved here              */
	int	dno;		          /* # of command delimiter found (0-6),
				            7=space,tab,8=newline,9=EOF,-1=other*/

/* search for next "|insert"; if no |insert found, terminate              */
	while (getinsert(f,&dno)) {

/* get filename specified after |insert:  insert contents of fname        */
		get_file_name(f,fname,MAXPATH,&dno);
		putin(fname,&dno);
	}
}



/* routine to get a (possibly pushed back) input character   */
getch(f)
FILE	*f;
{
	int	c;

	c = (bufp > 0) ? buf[--bufp] : getc(f);
	if (c=='\n') ++(filelist[nfiles-1].line_count);
	return(c);
}


/* routine to push character back into input stream - into buf  */
void
ungetch(c)
int	c;
{
	if (bufp > BUFSIZE) {
		printf("ungetch: too many characters\n");
	} else {
		buf[bufp++]=c;
		if (c=='\n') --(filelist[nfiles-1].line_count);
	}
}


/* routine to find next |insert or |begin(insert) monkcommand                 */
getinsert(f,dno)
FILE	*f;
int	*dno; 
{
	int	c, i, j;
	char	s[MAXC];

	while(1) {
 
/* search for |                                                               */ 
		while ((c=getch(f)) != '|' ) {
			if (c==EOF) return(NO);
			putchar(c);
		}

/* found |: now search for "insert" or "begin"                                */
		for (i=0; i<MAXC-1 && 
			   (c=getch(f)) !=EOF && (*dno=delimno(c))==-1; )  {
			if (c=='|') {
				ungetch(c);
				break;
			} else {
				s[i++] = c;
			}
		}
		s[i]='\0';
		if (c==EOF) *dno=9;
		if (i==0) {
			putchar('|');
			if (c==EOF) return(NO);
			if (c!='|') putchar(c);
		} else {
			if (strcmp(s,INSERT) == 0 || strcmp(s,BEGIN) == 0) {

/* found either insert or begin:                                              */
/*    ignore spaces or tabs between insert (or begin) and 1st non space/tab   */
/*    save white space in betw1 in case we have to write it out               */
				i=*dno;
				j=0;
				if (i==7) {
					betw1[j++]=c;
					while ((c=getch(f))==' ' || c=='\t') {
						if (j<MAXC-1) betw1[j++]=c;
					}
					if ((*dno=delimno(c))==-1) {
						ungetch(c);
						*dno=7;
					}
				}
				betw1[j]='\0';
				old_d=*dno;

/* if hit valid delimiter after |insert : return YES  */
/*   otherwise, output input and continue             */
				if (strcmp(s,INSERT)==0) { 
					if (*dno>-1 && *dno<7) return(YES);
					printf("|%s%s",s,betw1);
/*					if (*dno==7) putchar(' ');
					if (*dno==8) putchar('\n');*/
					print_white(dno);
					if (*dno==9) return(NO);
				} else {

/* if hit valid delimiter after |begin : get contents & check for insert    */
/*   if insert is contents of begin command, then set dno for |end(insert)  */
/*       and return to calling routine to continue processing insert        */
/*   otherwise, output input and continue                                   */
					if (*dno>-1 && *dno<7) {
						get_cmd_contents(f,s,MAXC,betw2,
							betw3,MAXC,dno);
						if (strcmp(s,INSERT)==0) {
							first_delim=delim_found;
							*dno=-2;
							return(YES);
						} 
					    	printf("|begin%s%c%s%s%s%c",
						   betw1,delimiters[*dno].left,
						   betw2,s,betw3,
						   delim_found);
					} else {
						printf("|%s%s",s,betw1);
/*						if (*dno==7) putchar(' ');
						if (*dno==8) putchar('\n');*/
						print_white(dno);
						if (*dno==9) return(NO);
					}
				}
			} else {

/* if not insert or begin: output input & continue */
				printf("|%s",s);
				if (*dno>-1 && *dno<7) 
					printf("%c",delimiters[*dno].left);
/*				if (*dno==7) putchar(' ');
				if (*dno==8) putchar('\n');*/
				print_white(dno);
				if (*dno==9) return(NO);
			}
		}
	}
}


/* routine to output space or newline character depending on value of *dno  */
print_white(dno)
int	*dno;
{
	if (*dno==7) putchar(' ');
	if (*dno==8) putchar('\n');
}



/* routine to determine if supplied character is delimiter                    */
delimno(x)
int	x;
{
	int	i;

	for (i=0; i<7 ; ++i) {
		if ( x==delimiters[i].left ) return(i);
	}
	switch(x)  {
		case  ' ': return(7);
		case '	': return(7);
		case '\n': return(8);
		case  EOF: return(9);
		default  : return(-1);
	}
}



/* routine to get file name specified in monk insert command      */
/*   ie. x of:                                                    */
/*        |insert(x)                                              */
/* or                                                             */
/*        |begin(insert)  x |end(insert)                          */
/* betw4 and betw5 are used to save white space surrounding x     */
void
get_file_name(f,s,lim,dno)
FILE	*f;
char	s[];
int	lim; 
int	*dno;
{
	int	delim_sw;	/*marks when required delimiter found   */
	int	white_sw;	/*marks when white space sensed AFTER filename */
	int	orig_dno;	/*saves original dno value on entering routine */
	int	c, d, i, ii, j;
	char	sx[MAXC], xend[MAXC];

	white_sw=0;
	orig_dno=*dno;  

/* get contents up to either delimiter specified or non-white char AFTER white */
/* old_dx is set to -1 if correct delimiter for filename is NOT found          */
/* if *dno!=-2 return with file name whether or not specified delimiter found  */
/* if *dno==-2, valid delimiter is |end(insert); search until it is found      */
	old_dx=-1;
	delim_sw=get_cmd_contents(f,s,lim,betw4,betw5,MAXC,dno);
	if (orig_dno!=-2) {
		if (delim_sw) old_dx=orig_dno;
		return;
	}

/* if hit EOF while searching for delimiter, return with filename              */
	if (*dno==9) {
		*dno=orig_dno;
		return;
	}

/* at this point, *dno==-2 which means we are looking for |end(insert)         */
/* if we havent found | yet, continue searching until it is found              */
/*    otherwise, start looking for end(insert)                              */
	if (!delim_sw) {
		white_sw=1;
		while (!get_cmd_contents(f,xend,MAXC,betw7,betw8,MAXC,dno)) {
			if (*dno==9) {
				*dno=-2;
				return;
			}
		}
	}

/* *dno==-2 AND "|" reached                                                    */
/*     now search for "end(insert)"                                            */
	while (1) {
		i=strlen(s);
		if (i>0 && (strlen(betw5))>0) white_sw=1;
		for (ii=0; ii<MAXC-1 && (c=getch(f))!=EOF && c!='|' &&
						(d=delimno(c))==-1; ) {
			xend[ii++]=c;
		}
		xend[ii]='\0';
		if (c==EOF) *dno=9;
		if (ii==0) {
			if (white_sw==0) {
				s[i++]='|';
				if (c!='|' && c!=EOF && d<7) s[i++]=c;
				s[i]='\0';
				if (d==7 || d==8) white_sw=1;
			}
		} else {
			if (strcmp(xend,END)==0 && c!='|' && c!=EOF) {

/* at this point we have found "|end" and some delimiter;                  */
/*     note + gobble leading white space while looking for valid delimiter    */
				ii=d;
				j = 0;
				if (ii==7) {
					betw6[j++]=c;
					while ((c=getch(f))==' ' || c=='\t') {
						if (j<MAXC-1) betw6[j++]=c;
					}
					if ((d==delimno(c))==-1) {
						ungetch(c);
						d=7;
					}
				}
				betw6[j]='\0';
				if (ii>7) j=1;

/*  if valid delimiter after "end", search for "insert"                   */
/*      if insert is found, return                                        */
				if (d>-1 && d<7) {
				    get_cmd_contents(f,sx,MAXC,betw7,betw8,MAXC,&d);
				    if (strcmp(sx,INSERT)==0) {
					old_dx=d;
					return;
				    }

/* at this point we have found |end and a valid delimiter, but not insert */
/*   add appropriate characters to filename                               */
				    if (!white_sw) {
					s[i++]='|';
					s[i]='\0';
					strcat(s,xend);
					i=strlen(s);
					if (j==0) {
					   s[i++]=delimiters[d].left;
					   s[i]='\0';
					   if (strlen(betw7)==0) {
					       strcat(s,sx);
					       i=strlen(s);
					       if (strlen(betw8)==0) {
						    if (delim_found != ' '  &&
							delim_found != '\t' &&
							delim_found != '\n' &&
						        delim_found != EOF)    {
							   s[i++]=delim_found;
							   s[i]='\0';
						    } else white_sw=1;
					       } else white_sw=1;
					   } else white_sw=1;
					} else white_sw=1;
				    } else {
					print_err_msg();
				    }
				} else {

/* at this point we have found |end and an invalid delimiter: add appropriate */
/*   characters to filename                                                     */
				    if (!white_sw) {
					s[i++]='|';
					s[i]='\0';
					strcat(s,xend);
					i=strlen(s);
					if (j==0) {
						s[i++]=c;
						s[i]='\0';
					} else white_sw=1;
				    } else {
					print_err_msg();
				    }
				}

/*  at this point we have either found |end and some invalid delimiter OR */
/*  we have a value in xend not equal to "end"                            */
/*  IF we have encountered NO white space since the last char in s then   */
/*     add non-white characters "|", contents of xend, and c to s         */
/*     since they may be part of the filename                             */
			} else {
				if (!white_sw) {
					s[i++]='|';
					s[i]='\0';
					strcat(s,xend);
					i=strlen(s);
					if (j==0 && c!='|' && c!=EOF && d<7) {
						s[i++]=c;
						s[i]='\0';
					} else {
						if (d>7) white_sw=1;
					}
				} else {
					print_err_msg();
				}
			}
		}
		while (!get_cmd_contents(f,xend,MAXC,betw7,betw8,MAXC,dno)) {
			if (i>0 && strlen(betw7)>0) white_sw=1;
			if (strlen(xend)>0 && !white_sw) {
				strcat(s,xend);
			}
			if (*dno==9) {
				*dno=-2;
				return;
			}
		}
	} 

}


/* routine to print error message for invalid delim when searching for |end(insert) */
print_err_msg()
{
	fprintf(stderr,
		"monkmerge: file %s: line %d: Mismatched delims around filename;",
		filelist[nfiles-1].name, filelist[nfiles-1].line_count);
	fprintf(stderr, 
		" continuing search for |end(insert)\n");
}


/* routine to print input; used before error message printed when problem with */
/* filename                                                                    */
print_input(fname,dno)
char	fname[];
int	*dno;
{
	if (*dno==-2) {
		printf("|begin%s%c%sinsert%s%c%s%s%s",
			betw1,delimiters[old_d].left,betw2,betw3,
			first_delim,betw4,fname,betw5);
		if (old_dx!=-1) {
			printf("|end%s%c%sinsert%s%c",
				betw6,delimiters[old_dx].left,betw7,betw8,
				delim_found);
		} else {
			printf("%c",delim_found);
		}
	} else {
		printf("|insert%s%c%s%s%s%c",betw1,delimiters[old_d].left,
			betw4,fname,betw5,delim_found);

	}
}



/* routine to get the contents of a |begin command : ie x of |begin(x)  */
/* may also be used to obtain file name and check if | terminated file  */
/*    name specification;                                               */      
/* if *dno==-2 contents are up to "|"; otherwise, contents are up to    */
/*      right delimiter specified by dno                                */
/* t is used to save white space occuring BEFORE non-white character(s) */
/* w is used to save white space occuring AFTER  non-white character(s) */
/*                                                                      */
/* returns: 0 if invalid delimiter sensed                               */
/*          1 if valid delimiter sensed                                 */ 
/*                                                                      */
get_cmd_contents(f,s,lim1,t,w,lim2,dno)
FILE	*f;
char	s[], t[], w[];
int	lim1, lim2;
int	*dno;
{
	int	cc, c, i, j, k, white_sw;

	j=k=white_sw=0;

/* for *dno==-2: contents are up to |               */ 
/*         !=-2: contents are up to right delimiter */

	if (*dno==-2) {
		cc='|';
	} else {
		cc=delimiters[*dno].right;
	}
	for (i=0; i<lim1-1 && ((c=getch(f)) != cc) ; ) {
		if (c==EOF) break;
		if (c!=' ' && c!='\t' && c!='\n') { /* non-white */
			if (white_sw) { /* illegal after white_sw set */
				s[i]='\0';
				t[j]='\0';
				w[k]='\0';
				fprintf(stderr,
	"monkmerge: file %s: line %d: Mismatched delims around %s; expected ",
	filelist[nfiles-1].name,filelist[nfiles-1].line_count,s);
				if (*dno==-2) {
					fprintf(stderr,"|end, got %c\n",c);
				} else {
					fprintf(stderr,"%c, got %c\n",
						delimiters[*dno].right,c);
				}
				delim_found=c;
				return(0); /* c gobbled */
			}
			s[i++]=c;
		} else { /* white space; ignore */
			if (i>0) {
				white_sw=1;/*set switch AFTER non-white*/
				if (k<lim2-1) w[k++]=c;
			} else {
				if (j<lim2-1) t[j++]=c;
			}
		}
	}
	s[i]='\0';
	w[k]='\0';
	t[j]='\0';
	if (c==EOF) *dno=9;
	delim_found=c;
	if (c==cc) return(1);
	return(0);
}



/* routine to insert contents of filename with comment before and after      */
void
putin(fname,dno)  
char	fname[];
int	*dno;
{
	int	d,i;
	char	iname[MAXPATH];
	FILE	*fopen(), *fp;
	char	*p;

/* check if file specified can be inserted; if not, write error messages and  */
/* return to calling routine to continue processing input                     */
	if (!checkname(fname,dno)) return;

/* check if file specified can be opened; if not, error messages and return    */
/* otherwise, process file specified in insert command                         */
	if ((fp = fopen(fname, "r"))==NULL) {
		fprintf(stderr,
	"monkmerge: file %s: line %d: can't open %s: cannot insert file\n",
	filelist[nfiles-1].name, filelist[nfiles-1].line_count,fname);
/*		if (*dno==-2) {
			printf("|begin%s(%sinsert%s)%s%s%s",
				betw1,betw2,betw3,betw4,fname,betw5);
			printf("|end%s(%sinsert%s)",betw6,betw7,betw8);
		} else {
			printf("|insert%s%c%s%s%s%c",betw1,delimiters[*dno].left,
				betw4,fname,betw5,delimiters[*dno].right);
		}*/
		print_input(fname,dno);
		printf("|comment<cannot open %s: cannot insert file>",fname);
	} else {
		printf("|comment<inserting %s starting here>\n",fname);

/* copy filename checking for embedded "insert" commands                    */
		p=mymalloc((unsigned) strlen(fname) +1);
		strcpy(p,fname);
		filelist[nfiles].name=p;
		filelist[nfiles++].line_count=1;
		while (getinsert(fp,&d)) {
			get_file_name(fp,iname,MAXPATH,&d); 
			putin(iname,&d);
		}
		fclose(fp);
		myfree(p);
		printf("|comment<inserting %s ending here>",fname);
		--nfiles;
	}
}



/* routine to check if filename is legal and NOT on open file list         */
/*     returns NO if filename is illegal or already on list                */
/*     returns YES if filename legal AND not on list                       */
checkname(fname,dno)
char	fname[];
int	*dno;
{
	int	i, c, j;

/* check that there is a file name specified  */
	if (fname[0]=='\0') {
/*		if (*dno==-2) {
			printf("|begin(insert)%s|end(insert)",fname);
		} else {
			printf("|insert%c%s%c",delimiters[*dno].left,fname,
				delimiters[*dno].right);
		}*/
		print_input(fname,dno);
		printf("|comment<no file specified for insert>");
		fprintf(stderr,
    	"monkmerge: file %s: line %d: no file specified for insert\n",
     			filelist[nfiles-1].name, filelist[nfiles-1].line_count);
		return(NO);
	}

/* check that number of files open is less than maximum       */
	if (nfiles>MAXFILES-1) {
/*		if (*dno==-2) {
			printf("|begin(insert)%s|end(insert)",fname);
		} else {
			printf("|insert%c%s%c",delimiters[*dno].left,fname,
				delimiters[*dno].right);
		}*/
		print_input(fname,dno);
		printf("|comment<too many files open: cannot insert file %s>",
			fname);
		fprintf(stderr,
    "monkmerge: file %s: line %d: too many files open: cannot insert file %s\n",
     filelist[nfiles-1].name, filelist[nfiles-1].line_count, fname);
		return(NO);
	}

/* check that file name does not have any illegal characters  */
	for (i=0; fname[i]!='\0'; i++) {
		if (fname[i]=='\\' && ((c=fname[i+1])=='*' || c=='n')) {
/*			if (*dno==-2) {
				printf("|begin(insert)%s|end(insert)",fname);
			} else {
				printf("|insert%c%s%c",delimiters[*dno].left,
					fname,delimiters[*dno].right);
			}*/
			print_input(fname,dno);
			printf(
		"|comment<illegal file name: cannot insert file %s>",fname);
			fprintf(stderr,
    "monkmerge: file %s: line %d: illegal file name: cannot insert file %s\n",
    filelist[nfiles-1].name, filelist[nfiles-1].line_count, fname);
			return(NO);
		}
	}

/* check that file name is not already on list of files already opened */
	for (i=0; i < nfiles; ++i) {
		if (strcmp(fname,filelist[i].name) == 0) {
/*			if (*dno==-2) {
				printf("|begin(insert)%s|end(insert)",fname);
			} else {
				printf("|insert%c%s%c",delimiters[*dno].left,
					fname,delimiters[*dno].right);
			}*/
			print_input(fname,dno);
			printf(
	"|comment<file already open: cannot recursively insert file %s>",fname);
			fprintf(stderr,
"monkmerge: file %s: line %d: file already open: cannot recursively insert file %s\n",
    filelist[nfiles-1].name, filelist[nfiles-1].line_count, fname);
			printf("\n|comment<current file stack:>");
			fprintf(stderr,"monkmerge: current file stack:\n");
			for (j=nfiles-1; j>-1; j--) {
				printf("\n|comment<     %s>",filelist[j].name);
				fprintf(stderr,"    %s\n",filelist[j].name);
			}
			return(NO);
		}
	}
	return(YES);	
}
