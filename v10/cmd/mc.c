#define	JERQ
/*
 * mc - columnate
 *
 * mc[-][-LINEWIDTH][-t][file...]
 *	- causes break on colon
 *	-LINEWIDTH sets width of line in which to columnate(default 80)
 *	-t suppresses expanding multiple blanks into tabs
 *
 */
#include	<stdio.h>
#ifdef	JERQ
#include	<sgtty.h>
#include	"/usr/jerq/include/jioctl.h"
#endif
#define WIDTH		80
#define NWALLOC		1024
#define NALLOC		4096
int linewidth=WIDTH;
int colonflag=0;
int tabflag=1;
char *cbuf, *cbufp;
char **word;
FILE *file;
char *malloc();
char *realloc();
int maxwidth=0;
int nalloc=NALLOC;
int nwalloc=NWALLOC;
int nchars=0;
int nwords=0;

main(argc, argv)
	char *argv[];
{
	FILE *fopen();
	register i;
	char buf[BUFSIZ];
#ifdef	JERQ
	struct winsize wbuf;

	if(ioctl(1, JWINSIZE, &wbuf)==0){
		linewidth=wbuf.bytesx;
		if(linewidth<0)
			linewidth=WIDTH;
	}
#endif
	while(argc>1 && argv[1][0]=='-'){
		--argc; argv++;
		switch(argv[0][1]){
		case '\0':
			colonflag=1;
			break;
		case 't':
			tabflag=0;
			break;
		default:
			linewidth=atoi(&argv[0][1]);
			if(linewidth<=1)
				linewidth=WIDTH;
			break;
		}
	}
	setbuf(stdout, buf);
	cbuf=cbufp=malloc(NALLOC);
	word=(char **)malloc(NWALLOC *(sizeof *word));
	if(word==0 || cbuf==0)
		error("out of memory");
	if(argc==1){
		file=stdin;
		readbuf();
	}else{
		for(i=1; i<argc; i++){
			file=freopen(*++argv, "r", stdin);
			if(file==NULL)
				fprintf(stderr, "mc: can't open %s\n", *argv);
			else
				readbuf();
		}
	}
	columnate();
	exit(0);
}
error(s)
	char *s;
{
	fprintf(stderr, "mc: %s\n", s);
	exit(1);
}
readbuf()
{
	register c, lastwascolon=0;
	int linesiz = 0;
	do{
		if(nchars++>=nalloc)
			morechars();
		*cbufp++=c=getc(file);
		linesiz++;
		if(c=='\t') {
			cbufp[-1] = ' ';
			while(linesiz%8 != 0) {
				if(nchars++>=nalloc)
					morechars();
				*cbufp++ = ' ';
				linesiz++;
			}
		}
		if(colonflag && c==':')
			lastwascolon++;
		else if(lastwascolon){
			if(c=='\n'){
				register n=1;
				--nchars; 	/* skip newline */
				while(nchars>0 && cbuf[--nchars]!='\n')
					n++;
				if(cbuf[nchars]=='\n')
					nchars++;
				columnate();
				if (nchars)
					putchar('\n');
				fwrite(cbuf+nchars, 1, n, stdout);
				nchars=0;
				cbufp=cbuf;
			}
			lastwascolon=0;
		}
		if(c=='\n')
			linesiz = 0;
	}while(c!=EOF);
}
scanwords(){
	register char *p, *q;
	register i;
	nwords=0;
	maxwidth=0;
	for(p=q=cbuf, i=0; i<nchars; i++){
		if(*p++=='\n'){
			if(nwords>=nwalloc){
				if((word=(char **)realloc((char *)word, (nwalloc+=NWALLOC)*sizeof(*word)))==0)
					error("out of memory");
			}
			word[nwords++]=q;
			p[-1]=0;
			if(p-q>maxwidth)
				maxwidth=p-q;
			q=p;
		}
	}
}
int words_per_line;
int nlines;
int col;
int tabcol;
int endcol;
int maxcol;
columnate(){
	register char *p;
	register i, j;
	scanwords();
	if(nwords==0)
		return;
	words_per_line=linewidth/maxwidth;
	if(words_per_line<=0)
		words_per_line=1;
	nlines=(nwords+words_per_line-1)/words_per_line;
	for(i=0; i<nlines; i++){
		col=0;
		endcol=0;
		for(j=0; i+j<nwords; ){
			endcol+=maxwidth;
			p=word[i+j];
			while(*p){
				putchar(*p++);
				col++;
			}
			if(i+(j+=nlines)<nwords){
				tabcol=(col|07)+1;
				if(tabflag)
					while(tabcol<=endcol){
						putchar('\t');
						col=tabcol;
						tabcol+=8;
					}
				while(col<endcol){
					putchar(' ');
					col++;
				}
			}
		}
		putchar('\n');
	}
}

morechars()
{
	if((cbuf=realloc(cbuf, nalloc+=NALLOC))==0)
		error("out of memory");
	cbufp=cbuf+nchars-1;
}
