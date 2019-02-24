/*                                           */
/*              demonk                       */
/*                                           */
/*   program to remove monk commands         */
/*     from monk source file(s)              */
/*                                           */
/*   usage:                                  */
/*     demonk [-b] [-i] [file1....filen]     */
/*                                           */
/*   -b option to remove blank lines         */
/*     resulting   when command between      */
/*     newlines is removed and when they     */
/*     occur within contents of command      */
/*     delimiters                            */
/*     default is to leave newlines          */
/*                                           */
/*   -i option to ignore files specified     */
/*     by |insert and |source statements;    */
/*     default is to insert files:           */
/*     if files are inserted, files specified*/
/*     by |insert are processed thru demonk  */
/*     while files specified by |source are  */
/*     output as is;                         */
/*                                           */
/*                                           */
/* extracts from input file(s) the text      */
/*     of the monk document:                 */
/*                                           */
/*   the following are removed in toto:      */
/*       |begin(command) *requires           */
/*          special handling                 */
/*       |end(command) *same as begin        */
/*       |comment(...)                       */
/*       |environment(...)                   */
/*          [note: name specified is entered */
/*                 in hash tables]           */
/*       |macro(...)                         */
/*       |reference(..)                      */
/*       |save(..)                           */
/*       |remember(..)                       */
/*       |savepage(..)                       */
/*       |ipa(...)                           */
/*   the following are removed in toto if the*/
/*     -i option is specified; otherwise, the*/
/*     file specified is read and if |insert */
/*     it is "demonked", if |source it is    */
/*     output as is:
/*       |source(...) 			     */
/*       |insert(...)			     */
/*   the following are removed in toto       */
/*     and a space is inserted if necessary  */
/*       |sp(..)                             */
/*       |p(..)                              */
/*       |blank_space(..)                    */
/*       |titlebox                           */
/*       |reference_placement                */
/*   the following are removed with delims   */
/*   but contents of delimiter pair is saved */
/*   if no contents, space inserted in place */
/*   of removed command                      */
/*       |signature(...)                     */
/*       |att(..)                            */
/*       |section(...)                       */
/*       |subsection(...)                    */
/*       |subsubsection(...)                 */
/*       |unnumbered_section(...)            */
/*       |unnumbered_subsection(...)         */
/*       |unnumbered_subsubsection(...)      */
/*       |abstract(...)                      */
/*       |title(...)                         */
/*   the following require special handling  */
/*       |table(..)                          */
/*       |equation(..)                       */
/*       |e(..)                              */
/*       |picture(..)                        */
/*       |ped(..)                            */
/*       |ideal(..)                          */
/*       |graph(..)                          */
/*       |bargraph(..)                       */
/*   the following are removed in toto       */
/*     but require checking for embedded     */
/*     delimiter pairs:                      */
/*       |attribute(...)                     */
/*       |associate(...)                     */
/*          [note: name specified is entered */
/*                 in hash tables]           */
/*   the following commands are removed but  */
/*     contents of delimiter pair is saved   */
/*     and a space inserted before contents  */
/*       |notation(...)                      */
/*       |footnote(...)                      */
/*       |dagnote(...)                       */
/*       |starnote(...)                      */
/*   all other monk commands which are       */
/*     "associate" commands are removed in   */
/*     total along with contents of delimiter*/
/*     pair following                        */
/*   all other monk commands which are NOT   */
/*     "associate" commands are removed      */
/*     with delimiters but contents of       */
/*     delimiter pair is saved               */
/*                                           */


#include	<stdio.h>
#include	<string.h>
#include	"demonk.h"
#include	"search.h"
#include	"rd.h"

void 	demonk(), ungetch(), get_filename(), proc_nxt_char(), zap();
void	zapall(), zapall_delim_check(), zap_save_contents(), process_e();
void	output_newline();

char	mcomnd[MAXCOMMAND];	/* monk command saved here                   */
char	buf[BUFSIZE];		/* buffer for ungetch                        */
char	*beglist[MAXBEGINS];	/* |begin(commands) saved here               */
char	doctype[MAXMAKE];
char	*db_path;

/* space to save white space surrounding command and its contents in case we */
/* need to output; especially useful when searching for filename for insert  */
char	betw1[MAXC], betw2[MAXC], betw3[MAXC], betw4[MAXC], betw5[MAXC];
char	betw6[MAXC], betw7[MAXC], betw8[MAXC];


struct	file_info filelist[MAXFILES];	/* storage for information on files
					   currently open                    */

int	bufp = 0;		/* next free position in buf                 */
int	eq_sw = NO;		/* switch to indicate if |e or |begin(e)
                           	   previously processed                      */
int	nbegins = 0;		/* number of nested begin commands           */
int	db_read_sw = NO;	/* switch to indicate if compressed database
				   read for current doctype                  */
int	first_time = YES;	/* switch to indicate if first time thru
					p_assoc_envir routine                */
int	nfiles = 0;		/* number of file currently open             */

int	bopt = 1;		/* switch to indicate -b option: 
					0: remove newlines
					1: leave  newlines (default)       */
int	iopt = 1;		/* switch to indicate -i option:
					0: do NOT insert files 
					1: insert files so specified (default) */

/* switches and values of delimiters in current command     */
int	old_d, old_dx;		
int	delim_found, first_delim;




main(argc, argv)
int	argc;
char	*argv[];
{
	FILE	*fp, *fopen();
	char	*s;
	char	*p;
	int	i;
	int	c;
	extern	int	optind;
	extern	char	*optarg;

	strcpy(doctype,DB_GLOBAL);
	db_path=DB_PATH;

/*  determine option for blank lines                                */

	while ((c=getopt(argc, argv, OPTIONS)) != -1) {
		switch (c) {
			case 'b':	bopt=0;
					break;
			case 'i':	iopt=0;
					break;
			case 'd':	db_path = optarg;
					break;
			case '?':  	argc=-1;
				   	break;
		}
	}

/*  determine input and loop thru                                   */

	if (argc < 0) {
		fprintf(stderr,"demonk: usage: %s\n",USAGE);
		exit(1);
	} else {
		if ((argc-optind) == 0) {/* no arguments - use standard input   */
			p=mymalloc((unsigned) 6);
			strcpy(p,"stdin");
			filelist[nfiles].name=p;
			filelist[nfiles++].line_count=1;
			demonk(stdin);
			myfree(p);
			--nfiles;
		} else {          /* argument(s) specifies input file(s)    */
			for (; optind < argc; optind++) {
				if ((fp = fopen(argv[optind], "r"))==NULL) {
				   fprintf(stderr,
					"demonk: can't open %s\n",argv[optind]);
				   exit(1);
				} else {
				   if (nfiles>MAXFILES-1) {
				   	fprintf(stderr,
			"demonk: too many files in use; can't open %s\n",argv[optind]);
				   } else {
					p=mymalloc((unsigned) strlen(argv[optind])+1);
					strcpy(p,argv[optind]);
					filelist[nfiles].name=p;
					filelist[nfiles++].line_count=1;
				   	demonk(fp);
					myfree(p);
					--nfiles;
				   	fclose(fp);
				   }
				}
			}
		}
	}

/* end of all input files reached: */
/* check to see if any "begins" are left without corresponding "ends" */
	if (nbegins > 0) { /* write error message for each begin w/out end */
		for (i=nbegins-1; i>-1; i--) {
			fprintf(stderr,
		"demonk: file %s: line %d: no end for begin %s\n",
		filelist[nfiles].name, filelist[nfiles].line_count,beglist[i]);
		}
	}
	exit(0);
}




/* main demonk routine                                          */
void
demonk(f)
FILE	*f;
{
	int	dno;	/* delimiter number                   
		             # of left delimiter found (0-6), 
                             7=space or tab, 8=newline, 9=EOF,
                             right delimiter (10-16),-1=other, not delim,
			     -2=|begin(command)                            */
	int	key;	/* command group number                            */
	int	nl;	/* switch to indicate if | was preceeded by
			     newline char or not (0=no, 1=yes)             */
	int	lc;	/* switch to indicate what last char was 
			     (2=EOF, 0=not newline, 1=newline)             */
	int	i;
	
	lc=1;

	while (1) {

/* search for next "|"                      */

	        if (getupb(f,&nl,&lc)==NO) break;

/* get monkcommand following "|"            */

		key=getcmd(f,mcomnd,MAXCOMMAND,&dno,&nl,&lc);

/* zap source depending on command key;      */
/*    use dno to determine delimiter pair    */
/*    if delimiter was EOF, quit             */

		if (dno==9) break;
		zap(f,key,dno,&nl,&lc);

/* if last character read was EOF, quit      */

		if (lc==2) break;
	}

}






/* routine to get a (possibly pushed back) input character     */
getch(f)
FILE	*f;
{
	int c;
	c = (bufp > 0) ? buf[--bufp] : getc(f);
	if (c=='\n') ++(filelist[nfiles-1].line_count);
	return(c);
}





/* routine to push character back into input stream - into buf */
void
ungetch(c)
int	c;
{
	if (bufp > BUFSIZE) {
		fprintf(stderr,"demonk: ungetch: too many characters\n");
	} else {
		buf[bufp++]=c;
		if (c=='\n') --(filelist[nfiles-1].line_count);
	}
}	





/* routine to find start of next monkcommand                  */
getupb(f,nl,lc)
FILE	*f;
int	*nl,*lc; 
{
	int	c;
	int	ret_sw;

	while ((c=getch(f)) != '|' ) {
/* search text for | (while leaving or removing troff commands, as desired):
      if encounter EOF,              0 is returned 
      if encounter |,                1 or 2 is returned       
      if encounter neither EOF or |, 3 is returned                 */
		ret_sw = search_text('|',f,&c,lc,nl);
		switch(ret_sw) {
			case  0:	return(NO);
			case  1:
			case  2:	return(YES);
			default:	;	
		}
	}
	*nl = (*lc==1) ? 1 : 0 ;
	*lc=0;
	return(YES);
	
}




/* routine to search and output text (while leaving or eliminating troff 
                                      commands, as desired): 
	returns: 0 when finds eof; 
	         1 when finds |: 
			if | at beginning of new line nl set to 1, else 0
	         2 when it finds char specified as c_wanted
		 3 when finds char other than eof or | or c_wanted
	when returns, c contains last character read; nl and lc are set

	if finds troff command: leaves or eliminates entire line, as desired   */
search_text(c_wanted,f,c,lc,nl)
int	c_wanted;
FILE	*f;
int	*c, *lc, *nl;
{
	if (*c==EOF) return(0);
	if (*c=='\n') {
		*lc=1;
		if ((*c=getch(f)) != '.' && *c != '\'') {
			output_newline();
			if (*c==EOF) return(0);
			if (*c=='|') {
				*nl=1;
				*lc=0;
				return(1);
			}
			if (*c==c_wanted) {
				*nl=1;
				*lc=0;
				return(2);
			}
			if (*c!='\n') {
				*lc=0;
				putchar(*c);
			} else {
				output_newline();
			}
		} else {  /* process troff       */
			output_newline();
/* comment out next line to gobble troff commands  */
			putchar(*c);
			while ((*c=getch(f))!='\n') {
				if (*c==EOF) return(0);
/* comment out next line to gobble troff commands  */
				putchar(*c); /* leave troff in output */
			}
			/*if (bopt==1)*/ putchar('\n'); /* remove comments 
						    around if if gobbling troff*/
			*lc=1;
		}
	} else {
		if (*lc == 1 && (*c=='.' || *c=='\'')) { /*process troff*/
/* comment out next line to gobble troff commands  */
			putchar(*c);
			while ((*c=getch(f))!='\n') {
				if (*c==EOF) return(0);
/* comment out next line to gobble troff commands  */
				putchar(*c); /* leave troff in output */
			}
			/*if (bopt==1)*/ putchar('\n'); /*remove comments 
						    around if if gobbling troff*/

			*lc=1;
		} else {
			if (*c == '|') {
				*nl = (*lc==1) ? 1 : 0;
				*lc=0;
				return(1);
			}
			if (*c == c_wanted) {
				*nl = (*lc==1) ? 1 : 0;
				*lc=0;
				return(2);
			}
			putchar(*c);
			*lc=0;
		}
	}
	return(3);
}


/* routine to output newline: used when preserving lines (b option NOT specified)*/
/* a space is output before the newline character to insure that blank lines */
/*    will NOT be removed by deroff                                          */
void
output_newline()
{
	printf("XYZQZY\n");
}

/* routine to gobble text, preserving newlines if requested: 
      returns:	0 when finds eof
		1 when finds | and
		2 when finds any other character
      when returns, c contains last character read; nl and lc are set          */
gobble_text(f,c,lc,nl)
FILE	*f;
int	*c,*lc,*nl;
{

	if (*c==EOF) return(0);
	if (*c=='\n') {
		*lc=1;
		if (bopt==1) output_newline();
		if ((*c=getch(f)) == '|') {
			*nl=1;
			*lc=0;
			return(1);
		} else {
			if (*c==EOF) return(0);
			*lc = (*c=='\n') ? 1 : 0;
			return(2);
		} 
	} else {
		if (*c=='|') {
			*nl = (*lc==1) ? 1 : 0;
			*lc=0;
			return(1);
		}
		*lc=0;
		return(2);
	}
}



/* routine to get monk command                               */
/*   save characters between end of command name and delimiter   in betw1  */
/*   save characters between delimiter and beginning of contents in betw2  */
/*   save characters between end of contents and closing delim   in betw3  */
/*   in the case of |begin(command) constructions:                         */
/*     save characters before contents in betw4                            */
/*     save characters after  contents in betw5                            */
/*   i.e.  |command (   contents   )                                       */
/*     would have some spaces in each of betw1,betw2 and betw3             */
/*     because there are spaces after command and around contents          */
/*         |begin ( make )    tm   |end(make)                              */
/*     would also have some spaces in each of betw4 and betw5 because there*/
/*     are spaces around "tm"                                              */
/*   this is done so that if the command is in error, the original text can*/
/*   be output                                                             */
getcmd(f,s,lim,dno,nl,lc) 
FILE	*f;    
char	s[];
int	lim;
int	*dno, *nl, *lc;
{
	int	c, i, k, kk, cc, white_sw, reset_sw, old_dno, process_sw;
	char	fname[MAXPATH];
	char	*p;

	for (i=0; i<lim-1 && 
                     (c=getch(f)) !=EOF && (*dno=delimno(c))==-1; i++)  {

/* if hit | while getting monk command, stop   */

		if (c=='|') {
			ungetch(c);
			break;
		} else {
			s[i] = c;
		}
	}
	s[i]='\0';
	if (c==EOF) *dno=9;
	if (i==0) {
		putchar('|');
		if (c=='\n') {
			*lc=1;
			output_newline();
		} else {
			if (c!='|' && c!=EOF) putchar(c);
		}
		return(NOTCOMMAND);
	}

/* gobble spaces or tabs between command and 1st non space/tab               */
	i=*dno;
	k=0;
	if (i==7) {
		betw1[k++]=c;
		while ((c=getch(f))==' ' || c=='\t' ) {
			if (k<MAXC-1) betw1[k++]=c;
		}
/*   if first non space/tab is a non-delimiter, put char back               */  
                if ((*dno=delimno(c))==-1) {
			ungetch(c);
			*dno=7;
		}
	}
	betw1[k]='\0';
	*lc = (*dno==8) ? 1 : 0;
	old_d=*dno;


/*    if command is "end", process                                     */
	if ((strcmp(s,"end")==0) && *dno<7 && *dno!=-1) {
		get_cmd_contents(f,s,lim,betw2,betw3,MAXC,dno,nl,lc);
		k=check_pp(f,s,nl,lc,2);
		kk=-1;
		if (k==0) {
			for (i=0; todo[i].cname!=NULL; i++) { 
				if (strcmp(s,todo[i].cname) == 0) { 
					kk=todo[i].ckey;
					break;
				}
			}
			if (strcmp(s,"make")==0) kk=1;
			if (kk==-1) {
				if (p_assoc_envir(s,FIND,1)==-1) {
					printf("|end%s%c%s%s%s%c",
						betw1,delimiters[*dno].left,betw2,
						s,betw3,delim_found);
				} else {
					kk=0;
				}
			}
		}
		if (k==1 || kk!=-1) {
			if (strcmp(s,beglist[nbegins-1])==0) {
				p=beglist[--nbegins];
				myfree(p);
				proc_nxt_char(f,nl,lc,k);
				return(-2);
			} else {
				fprintf(stderr,
			 "demonk: file %s: line %d: end %s has no begin\n",
			 filelist[nfiles-1].name,filelist[nfiles-1].line_count,s);
			proc_nxt_char(f,nl,lc,k);
			}
		}
		return(NOTCOMMAND);
	}	
		

/*    if command is "begin", process                                    */
	if ((strcmp(s,"begin")==0) && *dno<7 && *dno!=-1) {
		get_cmd_contents(f,s,lim,betw2,betw3,MAXC,dno,nl,lc);

/*    at this point, command of |begin is in s                          */
/*      save old dno in old_dno                                         */
/*      save closing delimiter found in first_delim                     */
/*      set dno to search for |end(comand)                              */
/*      save command in beglist                                         */
		old_dno=*dno;
		first_delim=delim_found;
		*dno=-2;
		if (nbegins<MAXBEGINS-1) {
			p=mymalloc((unsigned) strlen(s)+1);
			strcpy(p,s);
			beglist[nbegins++]=p;
		}

/*    check for command = preprocessor; if so, process and return       */

		k=check_pp(f,s,nl,lc,1);
		if (k==1) {
			proc_nxt_char(f,nl,lc,k);
			return(NOTCOMMAND);
		}
/*    otherwise, continue processing command designated by "begin"      */
	}


/*    if command is "insert" or "source", get filename, check iopt and process */
/*    process_sw is used to distinguish between insert (=1) and source (=0)    */
	process_sw=2;
	if (strcmp(s,"insert")==0) {
		process_sw=1;
	} else {
		if (strcmp(s,"source")==0) process_sw=0;
	}
	if (process_sw<2 && *dno<7 && *dno!=-1) {
		if (*dno==-2) proc_nxt_char(f,nl,lc,0);
		get_filename(f,fname,MAXPATH,dno,nl,lc,process_sw);

		if (iopt) {

/* process next character of input line partially BEFORE inserting file  */
			reset_sw=0;
			if ((c=getch(f))=='\n') {
				if  (*nl==1 && bopt==0) {
					*lc=1;
					reset_sw=1;
				} else {
					if (*nl==1 && bopt==1) {
						*lc=1;
						reset_sw=1;
						output_newline();
					} else {
						ungetch(c);
						reset_sw=2;
					}
				}
			}

/* try to insert file    */
			i=putin(fname,dno,reset_sw,process_sw);

/* finish processing next character of input line (line with insert command or
      line with |end(insert) command                                           */ 
			switch (reset_sw) {
				case	0:   ungetch(c);
				case	1:   break;
				case	2:   if (i) {
					        ++(filelist[nfiles-1].line_count);
					     }
					     break;
			}

/* insert option NOT on: process next character and quit  */
		} else {
			proc_nxt_char(f,nl,lc,0);
		}
		return(NOTCOMMAND);
	}

/*    if command is "associate" or "environment", process                      */
/*    use process_sw to indicate associate (1), environment (0) or neither (2) */
	process_sw=2;
	if (strcmp(s,"associate")==0) {
		process_sw=1;
	} else {
		if (strcmp(s,"environment")==0) process_sw=0;
	}
	if (process_sw<2 && *dno<7 && *dno!=-1) {
		k=kk=white_sw=0;
		for (i=0; i<lim-1 && ((c=getch(f)) !=';') ; ) {
			if (c==EOF) break;
			if (c!=' ' && c!='\t' && c!='\n') { /* non white */
				if (white_sw) { /* illegal after white sw set  */
					fprintf(stderr,
				"demonk: file %s: line %d: Expected ;, got %c\n",
				filelist[nfiles-1].name, 
				filelist[nfiles-1].line_count, c);
					break; /* c gobbled  */
				}
				s[i++] = c;
			} else { /* white space; ignore  */
				if (i>0) {
					white_sw=1;/*set switch AFTER non white*/
					if (k<MAXC-1) betw5[k++]=c;
				} else {
					if (kk<MAXC-1) betw4[kk++]=c;
				}
			}
			if (c=='\n' && bopt==1) output_newline();
			*lc = (c=='\n') ? 1 : 0;
		}
		s[i]='\0';
		betw4[kk]='\0';
		betw5[k]='\0';
		delim_found = (c==EOF) ? ' ' : c;

/* try to enter associate or environment in hash tables         */
		k=p_assoc_envir(s,ENTER,process_sw);
		if (k==0) {
/* associate or environment named NOT entered into hash tables: */
/*      write input to output                                   */
			if (*dno==-2) {
				printf("%s%s%s%c",betw4,s,betw5,delim_found);
			} else {
				if (process_sw) {
					printf("|associate");
				} else {
					printf("|environment");
				}
				printf("%s%c%s%s%s%c",
				    betw1,delimiters[old_d].left,betw4,s,betw5,
		    		    delim_found);
			}
		} else {
/* associate or environment named entered into hash tables:     */
/*      gobble rest of definition                               */
/* associate can have embedded same delimiters, so must check   */
			if (process_sw) { /* associate   */
				zapall_delim_check(f,*dno,nl,lc);
			} else { /* environment */
				zapall(f,*dno,nl,lc);
			} 
		}
		return(NOTCOMMAND);
	}


	
/*    if command is "make", save doctype specified inside delimiters     */
	if ((strcmp(s,"make")==0) && *dno<7 && *dno!=-1) {
		if (*dno==-2) proc_nxt_char(f,nl,lc,0);
		get_cmd_contents(f,s,lim,betw4,betw5,MAXC,dno,nl,lc);
		k=check_type(s,doctype);
		if (k==0) {
			if (*dno==-2) {
			   printf("%s%s%s",
				betw4,s,betw5);
			} else {
			   printf("|make%s%c%s%s%s%c",
				betw1,delimiters[old_d].left,betw4,s,betw5,
		    		delim_found);
			}
		} else {
			proc_nxt_char(f,nl,lc,0);
		}
		return(NOTCOMMAND);
	}

/* return key value corresponding to command name                            */
	for (i=0; todo[i].cname!=NULL; i++) { 
		if (strcmp(s,todo[i].cname) == 0) { 
			if (*dno==-2) proc_nxt_char(f,nl,lc,0);
			return(todo[i].ckey);
		}
	}


/* command name NOT found in table:                                           */ 
/*     determine if command is an associate by looking up s in the hashtables */
/*         k will equal -1 if s is not in the hash tables                     */
/*         k will equal  1 if s is an associate                               */
/*         k will equal  0 if s is not an associate                           */
	k=p_assoc_envir(s,FIND,1);
	if (*dno==-2 && k > -1) proc_nxt_char(f,nl,lc,0);
	if (k==1) return(0); /* is associate; gobble all */
	if (k==0) return(6); /* is not associate; save contents of delimiters */
/* not monk command  */
	if (*dno==-2) {
		printf("|begin%s%c%s%s%s%c",
			betw1,delimiters[old_d].left,betw2,s,betw3,delim_found);
		p=beglist[--nbegins];
		myfree(p);
	} else {
		printf("|%s%s",s,betw1);
		if (*dno>-1 && *dno<7) printf("%c",delimiters[old_d].left);
		if (bopt==0 && *dno==8) putchar('\n');
	}
	return(NOTCOMMAND);
}




/* routine to determine if supplied character is delimiter                  */
delimno(x)
int	x;
{
	int	i;

	for (i=0; i<7 ; i++) {
		if ( x==delimiters[i].left ) return(i);
	}
	for (i=0; i<7 ; ++i) {
		if ( x==delimiters[i].right ) return(i+10);
	}
	switch(x)  {
		case  ' ':	return(7);
		case '	':	return(7);
		case '\n':	return(8);
		case  EOF:	return(9);   
		default  :	return(-1);
	}
}




/* routine to get contents of delimiter pair of monk command                */
/*	ie. x of |begin(x) or |make(x) OR                                   */
/*              x of |begin(make)x|end(make)                                */
/*	stores any white space before 1st non white character and after     */
/*            last non white char in t and w                                */
/*      stores delimiter found in delim_found                               */
/* returns: 0 if invalid delimiter sensed                                   */
/*          1 if valid delimiter sensed                                     */
get_cmd_contents(f,s,lim1,t,w,lim2,dno,nl,lc)
FILE	*f;    
char	s[], t[], w[];
int	lim1, lim2;
int	*dno, *nl, *lc;
{
	int i, j, k, c, cc, white_sw;

	white_sw=0;  /* marks when white space sensed AFTER non-white */
	j=k=0;

	if (*dno==-2) {
/* contents are up to |end statement: take up to | since "|" would be illegal */
/*  character in any contents retrieved                                       */
		for (i=0; i<lim1-1 && ((c=getch(f)) != '|'); ) {
			if (c==EOF) break;
			if (c!=' ' && c!='\t' && c!='\n') { /* non white */
				if (white_sw) { /* illegal after white_sw set  */
					s[i]='\0';
					t[j]='\0';
					w[k]='\0';
					fprintf(stderr,
"demonk: file %s: line %d: Mismatched delims around %s; expected |end, got %c\n",
		filelist[nfiles-1].name, filelist[nfiles-1].line_count,s,c);
					delim_found=c;
					return(0); /* c gobbled  */
				}
				s[i++] = c;
			} else { /* white space; ignore  */
				if (i>0) {
					white_sw=1;/*set switch AFTER non white*/
					if (k<lim2-1) {
						if (c!='\n') {
							w[k++]=c;
						} else {
							w[k++]=' ';
						}
					}
				} else {
					if (c!='\n' && j<lim2-1) t[j++]=c;
				}
			}
/* if | occurs immediately after \n, put both back  */
			if (c=='\n') {
				cc=getch(f);
				ungetch(cc);
				if (cc=='|') {
					s[i]='\0';
					t[j]='\0';
					w[k]='\0';
					delim_found=cc;
					ungetch(c);
					return(1);
				} else {
					if (bopt==1) output_newline;
				}
			}
			*lc = (c=='\n') ? 1 : 0;
		}
		s[i]='\0';
		t[j]='\0';
		w[k]='\0';
		delim_found=c;
		if (c==EOF) *dno=9;
		ungetch(c);
		if (c=='|') return(1);
		return(0);
	}

/* contents are up to right delimiter  */
	for (i=0; i<lim1-1 && 
		     (c=getch(f)) != delimiters[*dno].right; )  {
		if (c==EOF) break;
		if (c!=' ' && c!='\t' && c!='\n') { /* non white */
			if (white_sw) { /* illegal after white_sw set */
				s[i]='\0';
				t[j]='\0';
				w[k]='\0';
				fprintf(stderr,
"demonk: file %s: line %d: Mismatched delims around %s; expected %c, got %c\n",
	filelist[nfiles-1].name, filelist[nfiles-1].line_count,s,
	delimiters[*dno].right,c);
				delim_found=c;
				return(0);  /* c gobbled  */
			}
			s[i++] = c;
		} else { /* white space; ignore  */
			if (i>0) {
				white_sw=1; /* set switch if AFTER non white */
/* check if non-white occurs after newline and it is NOT right delimiter */
				if (c=='\n') {
					if (k<lim2-1) w[k++]=c;
					cc=getch(f);
					if (cc!=' ' && cc!='\t' && cc!='\n' &&
					  cc!=delimiters[*dno].right) {
						s[i]='\0';
						t[j]='\0';
						w[k]='\0';
						fprintf(stderr,
"demonk: file %s: line %d: Mismatched delims around %s; expected %c, got %c\n",
	filelist[nfiles-1].name, filelist[nfiles-1].line_count,s,
	delimiters[*dno].right,cc);
						delim_found=cc;
						ungetch(c); /* save newline */
						return(0);  /* cc gobbled  */
					} else {
						ungetch(cc);
					}
				} else {
					if (k<lim2-1) w[k++]=c;
				}
			} else {
				if (j<lim2-1) t[j++]=c;
			}
		}
		if (c=='\n' && bopt==1) output_newline();
		*lc = (c=='\n') ? 1 : 0;
	}
	s[i]='\0';
	t[j]='\0';
	w[k]='\0';
	delim_found=c;
	if (c==EOF) *dno=9;
	if (c==delimiters[*dno].right) return(1);
	return(0);
}


/* routine to get file name specified in monk insert command      */
/*   ie. x of:                                                    */
/*        |insert(x)                                              */
/* or                                                             */
/*        |begin(insert)  x |end(insert)                          */
/* betw4 and betw5 are used to save white space surrounding x     */
void
get_filename(f,s,lim,dno,nl,lc,process_sw)
FILE	*f;
char	s[];
int	lim; 
int	*dno, *nl, *lc;
int	process_sw;
{
	int	delim_sw;	/*marks when required delimiter found   */
	int	white_sw;	/*marks when white space sensed AFTER filename */
	int	c, d, i, ii, j;
	int	orig_dno;
	char	sx[MAXC], xend[MAXC];
	char	*p;

	white_sw=0;

/* get contents up to either delimiter specified or non-white char AFTER white */
/* old_dx is set to -1 if correct delimiter for filename is NOT found          */
/* orig_dno saves *dno when enter routine                                      */
/* if *dno!=-2 return with file name whether or not specified delimiter found  */
/* if *dno==-2, valid delimiter is |end(insert) or |end(source); */
/*              search until it is found      */
	old_dx=-1;
	orig_dno=*dno;
	delim_sw=get_cmd_contents(f,s,lim,betw4,betw5,MAXC,dno,nl,lc);
	if (orig_dno!=-2) {
		if (delim_sw) old_dx=orig_dno;
		return;
	}

/* if hit EOF while searching for delimiter, return with filename              */
	if (*dno==9) {
		*dno=orig_dno;
		return;
	}

/* at this point, *dno==-2 which means we are looking for |end(insert)   */
/*         or |end(source)                    */
/* if we havent found | yet, continue searching until it is found              */
/*    otherwise, start looking for end(insert) or end(source)            */
	if (!delim_sw) {
		white_sw=1;
		while (!get_cmd_contents(f,xend,MAXC,betw7,betw8,MAXC,dno,nl,lc)) {
			if (*dno==9) {
				*dno=-2;
				return;
			}
		}
	}

/* the following is done to actually read thru the | since get_cmd_contents    */
/*   puts back the | once it is found!                                         */
	while ((c=getch(f)) != '|') {
		if (c=='\n' && bopt==1) output_newline();
	}

/* *dno==-2 AND "|" reached                                                    */
/*     now search for "end(insert)"  or "end(source)"                       */
	while (1) {
		i=strlen(s);
		if (i>0 && (strlen(betw5))>0) white_sw=1;
		for (ii=0; ii<MAXC-1 && (c=getch(f))!=EOF && c!='|' &&
						(d=delimno(c))==-1; ) {
			xend[ii++]=c;
		}
		if (c=='\n' && bopt==1) output_newline();
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

/*  if valid delimiter after "end", search for "insert"  or "source"      */
/*      if insert or source is found, return                         */
				if (d>-1 && d<7) {
				    get_cmd_contents(f,sx,MAXC,betw7,betw8,MAXC,
					&d,nl,lc);
				    if ((process_sw && strcmp(sx,INSERT)==0) ||
					(!process_sw && strcmp(sx,SOURCE)==0)) {
					    old_dx=d;
					    if (strcmp(sx,beglist[nbegins-1])==0) {
						p=beglist[--nbegins];
						myfree(p);
					    }
					    return;
				    }

/* at this point we have found |end and a valid delimiter, but not insert */
/*    or source      */
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
					print_err_msg(process_sw);
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
					print_err_msg(process_sw);
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
					print_err_msg(process_sw);
				}
			}
		}
		while (!get_cmd_contents(f,xend,MAXC,betw7,betw8,MAXC,dno,nl,lc)) {
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



/* routine to print err msg for invalid delim when searching for |end(insert) */
/*   or |end(source)    */
print_err_msg(process_sw)
int	process_sw;
{
	fprintf(stderr,
		"demonk: file %s: line %d: Mismatched delims around filename;",
		filelist[nfiles-1].name, filelist[nfiles-1].line_count);
	if (process_sw) {
		fprintf(stderr, " continuing search for |end(insert)\n");
	} else {
		fprintf(stderr, " continuing search for |end(source)\n");
	}
}





/* routine to check document type specified in make and set doctype to it if
     it matches an allowable type (make_list[])                              */
check_type(s,doctype)
char	s[];
char	*doctype;
{
	int	i;

	if (strcmp(s,doctype)==0) return(YES);
	for (i=0; make_list[i]!=NULL; i++) {
		if (strcmp(s,make_list[i])==0) {
			strcpy(doctype,s);
			db_read_sw=NO;
			return(YES);
		}
	}
	fprintf(stderr,
"demonk: file %s: line %d: %s is invalid specification type for make command\n",
filelist[nfiles-1].name, filelist[nfiles-1].line_count,s);
	return(NO);
}



/* routine to check if s specifies a preprocessor:
      if so - output troff for monk macro preprocessor commands   */
check_pp(f,s,nl,lc,boe)
FILE	*f;
char	s[];
int	boe;
int	*nl, *lc;
{
	int	i, k;

/*  if s specifies a preprocessor, output troff                              */
	k=0; /* switch to indicate if troff command output */
	for (i=0; pp_names[i].monkname!=NULL; i++) {
		if (strcmp(s,pp_names[i].monkname)==0) {
			if (*nl==0) output_newline();
			switch(boe) {
				case  1:  printf("%s\n",pp_names[i].macrostart);
					  k=1;
					  break;
				case  2:  printf("%s\n",pp_names[i].macroend);
					  k=1;
				default:  break;
			}
		}
	}

	*lc=0;
	if (*nl==1) *lc=1;
	return(k);
}



/* routine to handle next character following end of monk command       
   if character is NOT a newline, put it back;
   if character IS a newline, then must check if newline began command line:
      if it did and -b option specified (remove extra lines) then gobble newline
        and set lc to 1 to show that last character read was newline
      if it did and -b option not specified (leave extra lines) output newline
        and set lc to 1 to show that last character read was newline
      whether or not newline began command line, if preprocessor troff command
        switch is on (k==1), also gobble newline since newline character output
        following troff command
      in all other cases, put newline character back                           */
void
proc_nxt_char(f,nl,lc,k)
FILE	*f;
int	k;
int	*nl, *lc;
{
	int	c;

	if ((c=getch(f))!='\n') {
		ungetch(c);
	} else {

		if ((*nl==1 && bopt==0)|| (k==1)) {
			*lc=1;
		} else {
			if (*nl==1 && bopt==1) {
				*lc=1;
				output_newline();
			} else {
				ungetch(c);
			}
		}
	}
}



/* routine to zap monk command                                             */
void
zap(f,k,d,nl,lc)
FILE	*f;  
int	k, d;
int	*nl, *lc;
{
	int	c, sp_switch;

	if (d < 7 && d!=-1) {
		switch(k) {
			case -2:
			case -1:	break;
			case  0:	zapall(f,d,nl,lc);
					break;
			case  1:	sp_switch=0;
					if (*nl==0) {
						printf(" ");
						sp_switch=1;
					}
					zapall(f,d,nl,lc);
/*					if (*lc==0 && sp_switch==0) {
					   c=getch(f);
					   if (c!='\n' && c!='\t' &&
						c!=' ') putchar(' ');
						ungetch(c);
					}*/
					break;
			case  2:	zap_save_contents(f,d,nl,lc);
					break;    
			case  3:	zapall_delim_check(f,d,nl,lc);
					break;
			case  4:	if (strcmp(mcomnd,"e")==0)  {
						process_e(nl,1);
						zap_save_contents(f,d,nl,lc);
						process_e(nl,2);
					} else {
						check_pp(f,mcomnd,nl,lc,1);
						proc_nxt_char(f,nl,lc,1);
						zap_save_contents(f,d,nl,lc);
						check_pp(f,mcomnd,nl,lc,2);
						proc_nxt_char(f,nl,lc,1);
					}
					break;
			case  5:	printf(" ");
			default:	zap_save_contents(f,d,nl,lc);
					break;
		}
	} else {
		/* any command that is completely zapped when terminated by \n
		    will have \n zapped automatically; the following will ensure
		    that there is a \n when needed (bopt=1 and k=2)          */
		if ((k==1 || k==2) && (*nl==0 && d!=8)) putchar(' ');
		if (d==8) {
			if (bopt==1) {
				output_newline();
			} else {
				putchar('\n');
			}
		}
	}
}


/* routine to zap everything up to & including corresponding right delimiter 
     plus following newline if command started with newline AND -b option on  */
void
zapall(f,dno,nl,lc)
FILE	*f;
int	dno;
int	*nl, *lc;
{
	int	c, d, i, onl;
	char	s[MAXCOMMAND];
	char	*p;

	*lc=0;
	onl=*nl;

/* dno value determines what routine should look for as right delimiter:
   dno=-2: look for |end(beglist[nbegins])
   dno!=-2:  look for delimiters[dno].right                                */

	if (dno!=-2) {
		while ((c=getch(f)) != delimiters[dno].right) {
			if (c=='\n' && bopt==1) output_newline();
			if (c==EOF) break;
		}
	} else {
		while(1) {
			while((c=getch(f))!='|') {
				if (gobble_text(f,&c,lc,nl) < 2) break;
			}
			if (c == EOF) break;
			for (i=0; i<MAXCOMMAND-1 &&
			    (c=getch(f))!=EOF && (d=delimno(c))==-1; i++) {
				if (c=='|') {
					ungetch(c);
					break;
				}
				s[i]=c;
			}
			s[i]='\0';
			if (c==EOF) break;
			i=d;
			if (i==7) {
				while ((c=getch(f))==' ' || c=='\t') ;
				if ((d=delimno(c))==-1) {
					ungetch(c);
					d=7;
				}
			}
			*lc = (d==8) ? 1 : 0;
			if (d<7 && d>-1 && strcmp(s,"end")==0) {
				get_cmd_contents(f,s,MAXCOMMAND,betw2,betw3,
					MAXC,&d,nl,lc);
				if (strcmp(s,beglist[nbegins-1])==0) {
					p=beglist[--nbegins];
					myfree(p);
					break;
				}
			} 
		}
	} 

/*    if command not followed by newline, leave next character alone           */
/*    if command followed by newline, AND newline immediately preceeded     */
/*      | denoting start of command: gobble newline  if -b option on        */
/*      otherwise, leave next char alone                                    */
	*nl=onl;
	*lc=0;
	if (*nl==1) *lc=1;
	if (c!=EOF) {
		proc_nxt_char(f,nl,lc,0);
	} else {
		*lc=2;
	}
}


/* routine to zap everthing up to & including corresponding right delimiter
      taking into account that there may be embedded pairs of delimiters
      like the one left delimiter setting off the command arguments         */
void
zapall_delim_check(f,dno,nl,lc)
FILE	*f;
int	dno;
int	*nl, *lc;   
{
	int	c, i, d, onl, ndelims;
	char s[MAXCOMMAND];
	char *p;

	ndelims=0;
	*lc=0;
	onl=*nl;

/* if |begin(command) construction, need to search for |end(command) taking into
   account that there may be embedded |begin......|end  constructions with same
   command type which must be ignored                                           */
	if (dno==-2) {
		while(1) {
			while((c=getch(f))!='|') {
				if (gobble_text(f,&c,lc,nl) < 2) break;
			}
			if (c == EOF) break;
			for (i=0; i<MAXCOMMAND-1 &&
			    (c=getch(f))!=EOF && (d=delimno(c))==-1; i++) {
				if (c=='|') {
					ungetch(c);
					break;
				}
				s[i]=c;
			}
			s[i]='\0';
			if (c==EOF) break;
			i=d;
			if (i==7) {
				while ((c=getch(f))==' ' || c=='\t') ;
				if ((d=delimno(c))==-1) {
					ungetch(c);
					d=7;
				}
			}
			*lc = (d==8) ? 1 : 0;
			if (d<7 && d>-1 && strcmp(s,"end")==0) {
				get_cmd_contents(f,s,MAXCOMMAND,betw2,betw3,
					MAXC,&d,nl,lc);
				if (strcmp(s,beglist[nbegins-1])==0) {
					--ndelims;
					if (ndelims<0) {
						p=beglist[--nbegins];
						myfree(p);
						break;
					}
				}
			} 
			if (d<7 && d>-1 && strcmp(s,"begin")==0) {
				get_cmd_contents(f,s,MAXCOMMAND,betw2,betw3,
					MAXC,&d,nl,lc);
				if (strcmp(s,beglist[nbegins-1])==0) {
					++ndelims;
				}
			} 
		}
/* if command not followed by newline, leave next character alone           */
/* if command IS  followed by newline, AND newline immediately preceded |   */
/*    denoting start of command: gobble newline if -b option on             */
/*                               otherwise, leave next character alone      */
		*nl=onl;
		*lc=0;
		if (*nl==1) *lc=1;
		if (c!=EOF) {
			proc_nxt_char(f,nl,lc,0);
		} else {
			*lc=2;
		}
		return;
	}

/* search for right delimiter, taking into account that there may be embedded
   pairs of delimiters like the left delimiter setting off the command arguments*/
	while (1)  {
		while (((c=getch(f)) != delimiters[dno].right) && 
                            c != delimiters[dno].left) {
			if (c=='\n' && bopt==1) output_newline();
			if (c==EOF) break;
		}
		if (c==EOF) {
			*lc=2;
			break;
		}
		if (c==delimiters[dno].left) ++ndelims; 
		if (c==delimiters[dno].right) --ndelims;

/* if command preceeded by newline, see if it is followed by newline        */
/*    if command not followed by newline, leave next character alone        */
/*    if command followed by newline, AND -b option not used:
                                      gobble newline and set switch          
           otherwise, leave next character alone                            */
		if (ndelims<0) 	{
			proc_nxt_char(f,nl,lc,0);
			break;
		}
	}
}


/* routine to zap only the corresponding right delimiter   
     taking into account that there may be embedded monk commands with 
     the same or different delimiters                                     */
void
zap_save_contents(f,dno,nl,lc)
FILE	*f; 
int	dno;
int	*nl, *lc;
{
	int	c, key, i, ret_sw, n;

	n=nbegins-1;
	if (dno==-2) {
		while(1) {
			if (getupb(f,nl,lc)==NO) {
				*lc=2;
				return;
			}
			key=getcmd(f,mcomnd,MAXCOMMAND,&i,nl,lc);
			if (key==-2 && n==nbegins) return;
			if (i==9) {
				*lc=2;
				return;
			}
			zap(f,key,i,nl,lc);
			if (*lc==2) return;
		}
	}
	*nl=0; 
	*lc=0; 
	while ((c=getch(f)) != delimiters[dno].right) {
		ret_sw = search_text(delimiters[dno].right,f,&c,lc,nl);
		switch(ret_sw) {
			case  0:	*lc=2;
					break;
			case  1:	key=getcmd(f,mcomnd,MAXCOMMAND,&i,nl,lc);
					if (i==9) {
						*lc=2;
						break;
					}
					zap(f,key,i,nl,lc);
					break;
			case  2:	i=dno+10;
					break;
			default:	i=0;
					break;
		}
		if ((i-10)==dno || *lc==2 ) break;
	}
}

   


/* routine to handle monk command e                                          */
void
process_e(nl,boe)
int	*nl;
int	boe;
{
	switch(boe) {
		case  1: 
			if (eq_sw==0) {
				eq_sw=1;
				if (*nl==0) output_newline();
				printf(".EQ\n");
				printf("delim \n");
				printf(".EN\n");
			}
		case  2:
			printf("");
		default:
			break;
	}
}
		


/* routine to: */
/*         handle definition of s as an environment or associate and           */
/*         determine if command s is an associate                              */
/* if action is ENTER: process_sw determines which type we are entering        */
/*      if name s is already in hashtables, set subdef to NULL for environment */
/*                 and non-NULL for associate                                  */
/*      if name s is NOT     in hashtables, enter name and set subdef to NULL  */
/*                 for environment and non-NULL for associate                  */
/*      returns: 1 if entry is made, 0 if entry is not made (error msg printed)*/
/* if action is FIND: we are really just looking for associates; returns:      */
/*    1 if command is  in hashtables as an associate                           */
/*    0 if command NOT in hashtables as an associate (=in table as environment)*/
/*   -1 if command NOT in the hashtables at all                                */
p_assoc_envir(s,action,process_sw)
char	*s;
ACTION	action;     /* FIND or ENTER */
int	process_sw; /* 0 for environment, 1 for associate; used w/ action ENTER*/
{
	FILE	*fopen(), *database;
	char	cfile[MAXPATH];
	ENTRY	*exist;
	struct	definition *def;
	ENTRY	*entry;
	char	*data_s;

/* if first time thru this routine, initialize hash tables and read global.comp
     data base into hash tables  */

	if (first_time) {
		first_time = NO;
/* the following code is modified from read_databases + read_dbfile
       in read_user.c  */
		mk_hashtables(); /*   see read_db.c            */
		strcpy(cfile,db_path);
		strcat(cfile,"/");
		strcat(cfile,DB_GLOBAL);
		strcat(cfile,DB_COMP_EXT);
		if ((database=fopen(cfile,"r"))==NULL) {
			fprintf(stderr,
	"demonk: file %s: line %d: can't open compressed database\n",
	filelist[nfiles-1].name, filelist[nfiles-1].line_count); 
			exit(1); 
		}
		r_buffers(database,db_path); /* see read_cmpress.c */
		fclose(database);
	}


/* read hash tables from compressed database for doctype if not previously read */

	if (db_read_sw == NO && strcmp(doctype,DB_GLOBAL)!=0) {
/* the following code is modified from read_databases + read_dbfile 
       in read_user.c */
		strcpy(cfile,db_path);
		strcat(cfile,"/");
		strcat(cfile,doctype);
		strcat(cfile,DB_COMP_EXT);
		if ((database=fopen(cfile,"r"))==NULL) {
			fprintf(stderr,
	"demonk: file %s: line %d: can't open compressed database\n",
	filelist[nfiles-1].name, filelist[nfiles-1].line_count); 
			exit(1); 
		}
/* r_buffers can be found in read_cmpress.c      */
		r_buffers(database,db_path); 
		db_read_sw=YES;
		fclose(database);
	}

/* check to see if command s is in the hash tables                              */
	
	if ((exist = hashfind(ENV_TABLE,s)) != (ENTRY *) 0) {/*is envir or assoc*/

/* command s is in hash table as either an environment or an associate          */
/* if just looking for s, return 1 if s is associate and 0 if environment       */
/* if entering s, make sure that subdef is set to proper value                  */

		def = (struct definition *) exist->data;
 
		if (def->sub_def != NULL) { /* s is an associate */
			if (action==FIND) return(1);
			if (!process_sw) {  /* set to environment */
				def->sub_def = NULL;
			}
			return(YES);
		} else { /* s is an environment */
			if (action==FIND) return(0);
			if (process_sw) {  /* set s to associate */
				def->sub_def = (struct definition *)ASSOCIATE;
			} 
			return(YES);
		}

	} else {

/* command s is not defined as environment or associate;                       */
/*   if we are trying to FIND command s:                                       */
/*      since we have already checked for special commands as reserved keywords*/
/*      (begin, end, comment, etc) then this must not be a monk command        */
/*  however, if we are trying to ENTER command s, then do so                   */

		if (action==FIND) {
			fprintf(stderr,
			  "demonk: file %s: line %d: %s is not a monk command\n",
			  filelist[nfiles-1].name, filelist[nfiles-1].line_count,
			  s);
			return(NOTCOMMAND);

		} else { /* try to enter s into table: if process_sw is */
                         /*      1 -- enter s as an associate:          */
                         /*          use ASSOCIATE for data so that field will */
                         /*          be non NULL, ie. = associate    */
                         /*      0 -- enter s as an environment         */
                         /*          set data to NULL to indicate environment */   
			entry=(ENTRY *) mymalloc((unsigned) sizeof(ENTRY));
			data_s=mymalloc((unsigned) strlen(s)+1);
			strcpy(data_s,s);
			entry->key = data_s;
			entry->data=mymalloc((unsigned)sizeof(struct definition));
			def = (struct definition *) entry->data;
			if (process_sw) { /* set to indicate associate */
				def->sub_def = (struct definition *)ASSOCIATE;
			} else { /* set to indicate environment */
				def->sub_def = NULL;
			}
			if (hashenter(ENV_TABLE,entry)==(ENTRY *) NULL) {
				/* cannot enter s in ENV_TABLE  */
			    fprintf(stderr,
			      "demonk: file %s: line %d: cannot define %s\n",
			      filelist[nfiles-1].name, 
			      filelist[nfiles-1].line_count,s);
			    return(NO);
			} 
			return(YES);
		}
	}

}


/* routine to insert contents of filename                                    */
/* process_sw is used to determine whether file should be demonked           */
/*    1: insert command: demonk file before outputting                       */
/*    0: source command: output file as is, do not demonk                    */
/* returns YES if file was able to be used; NO if file not used              */
putin(fname,dno,reset_sw,process_sw)
char	fname[];
int	*dno;
int	reset_sw, process_sw;
{
	char	*p;
	FILE	*fopen(), *fp;
	int	line_numb;
/* set line_numb equal to line number of insert command                         */
/* if reset_sw is equal to 1: this means that \n was gobbled in calling routine */
/*  need to subtract 1 from line_count before printing error message since line */
/*  count would have been advanced to next line by reading \n and not ungetching*/
	line_numb=filelist[nfiles-1].line_count;
	if (reset_sw==1) --line_numb;

/* check if file specified can be inserted: if not, write error message to  */
/*     standard error and continue processing input file                    */ 
	if (!checkname(fname,dno,line_numb)) return(NO);

/* see if file can be opened; if not, error message and return              */
/* otherwise, process file specified in insert command                      */
	if ((fp = fopen(fname, "r"))==NULL) {
		fprintf(stderr,
		  "demonk: file %s: line %d: can't open %s inserted in file %s\n",
		   filelist[nfiles-1].name, line_numb, fname,
		   filelist[nfiles-1].name);
		return(NO);
	} else {

/* set data to take input from file specified by fname                   */
		p=mymalloc((unsigned) strlen(fname)+1);
		strcpy(p,fname);
		filelist[nfiles].name=p;
		filelist[nfiles++].line_count=1;

/* if reset_sw is equal to 2: this means that \n was read and ungetched in     */
/*   calling routine so that first character read for new file will really be  */
/*   newline character of old file; therefore, line count needs to be set back */
/*   1 so that it will be correct                                              */
		if (reset_sw==2) --(filelist[nfiles-1].line_count);

/* process file specified by fname                                       */
/*  if process_sw = 1, insert command: demonk file specified and         */
/*                                     output resulting text;            */
/*                  0, source command: read file and output as is        */
		if (process_sw) {
			demonk(fp);
		} else {
			copy_file(fp);
		}
		fclose(fp);
		myfree(p);
		--nfiles;
		return(YES);
	}
}


/* routine to check if filename is legal and NOT on open file list         */
/*     returns NO if filename is illegal or already on list                */
/*     returns YES if filename legal AND not on list                       */
checkname(fname,dno,line_numb)
char	fname[];
int	*dno;
int	line_numb;
{
	int	i, c, j;

/* check that there is a file name specified             */
	if (fname[0]=='\0') {
		fprintf (stderr,
	  "demonk: file %s: line %d: no file specified for insert\n",
	   filelist[nfiles-1].name, line_numb);
		return(NO);
	}

/* check that number of files open is less than maximum  */
	if (nfiles>MAXFILES-1) {
		fprintf(stderr,
	"demonk: file %s: line %d: too many files open: cannot insert file %s\n",
	filelist[nfiles-1].name, line_numb, fname);
		return(NO);;
	}

/* check that file name does not have any illegal characters  */
	for (i=0; fname[i]!='\0'; i++) {
		if (fname[i]=='\\' && ((c=fname[i+1])=='*' || c=='n')) {
			fprintf(stderr,
	"demonk: file %s: line %d: illegal file name: cannot insert file %s\n",
	filelist[nfiles-1].name, line_numb ,fname);
			return(NO);
		}
	}

/* check that file name is not already on list of files already opened */
	for (i=0; i < nfiles; ++i) {
		if (strcmp(fname,filelist[i].name) == 0) {
			fprintf(stderr,
"demonk: file %s: line %d: file already open: cannot recursively insert file %s\n",
		filelist[nfiles-1].name, line_numb, fname);
			fprintf(stderr,"     current file stack:\n");
			for (j=nfiles-1; j> -1; j--) {
				fprintf(stderr,"        %s\n",filelist[j].name);
			}
			return(NO);
		}

	}
	return(YES);	
}


/* routine to read file specified and output as is    */
copy_file(f)
FILE	*f;
{
	int	c;

	while ((c=getch(f)) != EOF) {
		if (c=='\n') {
			output_newline();
		} else {
			putchar(c);
		}
	}
}
