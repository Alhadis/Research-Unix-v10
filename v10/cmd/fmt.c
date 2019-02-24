/*% cyntax % && cc -go # %
 * block up paragraphs, possibly with indentation
 */
#include <libc.h>
#include <stdio.h>
#include <ctype.h>
int indent=0;		/* how many spaces to indent */
int length=70;		/* how many columns per output line */
main(argc, argv)
char *argv[];
{
	register i;
	register FILE *f;
	for(i=1;i!=argc && argv[i][0]=='-';i++){
		switch(argv[i][1]){
		case 'i':
			if(argv[i][2])
				indent=atoi(argv[i]+2);
			else if(i+1<argc){
				indent=atoi(argv[i+1]);
				i++;
			}
			else
				goto Usage;
			break;
		case 'w':
		case 'l':
			if(argv[i][2])
				length=atoi(argv[i]+2);
			else if(i+1<argc){
				length=atoi(argv[i+1]);
				i++;
			}
			else
				goto Usage;
			break;
		default:
		Usage:
			fprintf(stderr,
				"Usage: %s [-i number] [-l number] [file ...]\n",
				argv[0]);
			exit(1);
		}
	}
	if(length<=indent){
		fprintf(stderr, "%s: line length<=indentation\n", argv[0]);
		exit(1);
	}
	if(i==argc)
		fmt(stdin);
	else{
		for(;i!=argc;i++){
			f=fopen(argv[i], "r");
			if(f==0)
				perror(argv[i]);
			else{
				fmt(f);
				fclose(f);
				if(i!=argc-1)
					putchar('\n');
			}
		}
	}
	exit(0);
}
fmt(f)
register FILE *f;
{
	register c;
	while((c=getc(f))!=EOF)
		outchar(c);
	flush();
}
#define TAB 8
#define	NWORD	(TAB*32)
char word[NWORD+1];
char *wp=word;
int col=0;	/* output column number */
int bol=1;	/* at beginning of output line? */
int punct=0;	/* last character out was punctuation? */
int newline=1;	/* last char read was newline(1) or init space(2) */
outchar(c){
	switch(c){
	case '\0':
		break;
	case '\n':
		switch(newline){
		case 0:
			outword();
			break;
		case 1:
			flush();
		case 2:
			putchar('\n');
			wp=word;
		}
		newline=1;
		break;
	case ' ':
	case '\t':
		switch(newline) {
		case 0:
			outword();
			break;
		case 1:
			flush();
			newline=2;
		case 2:
			do {
				addchar(' ');
			} while(c=='\t' && (wp-word)%TAB);
		}
		break;
	default:
		addchar(c);
		newline=0;
	}
}
addchar(c)
{
	if(wp==&word[NWORD]) {
		if(strchr(" \t",wp[-1]))
			wp=word;
		outword();
	}
	*wp++=c;
}
outword(){
	register i;
	if(wp==word)
		return;
	if(wp-word+col+(bol?0:punct?2:1)>length){
		putchar('\n');
		col=0;
		bol=1;
	}
	if(col==0){
		for(i=0;i+8<=indent;i+=8)
			putchar('\t');
		while(i++<indent)
			putchar(' ');
		col=indent;
	}
	if(bol)
		bol=0;
	else{
		if(punct){
			putchar(' ');
			col++;
		}
		putchar(' ');
		col++;
	}
	*wp='\0';
	puncttest();
	printf("%s", word);
	col+=wp-word;
	wp=word;
}
/* is the word followed by major punctuation, .?:! */
/* disregard short things followed by periods; they are probably
   initials or titles like Mrs. and Dr. */
puncttest()
{
	char *ep;
	punct = 0;
	for(ep=wp; --ep>=word; ) {
		switch(*ep) {
		case ')': case '\'': case '"':
			continue;
		case '.':
			if(isupper(*word)&&ep-word<=3)
				return;
		case '?': case ':': case '!':
			punct = 1;
		default:
			return;
		}
	}
}
flush(){
	outword();
	if(col!=0){
		putchar('\n');
		col=0;
		bol=1;
	}
}
