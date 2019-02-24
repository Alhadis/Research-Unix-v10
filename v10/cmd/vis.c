#include <stdio.h>
#define	STDIN	(char *)0
int	col=0;
int	lastc=0;
int 	nprintfiles=0;
int	sflag=0;
int	tflag=0;
main(argc, argv)
	char *argv[];
{
	while(argc > 1) {
		--argc; argv++;
		if(argv[0][0] == '-'){
			switch(argv[0][1]){
			case 's':
				sflag++;
				break;
			case 't':
				tflag++;
				break;
			}
		}else{
			printfile(argv[0]);
			nprintfiles++;
		}
	}
	if(nprintfiles == 0)
		printfile(STDIN);
}
printfile(s)
	register char *s;
{
	register c;
	register FILE *f=s? fopen(s, "r"): stdin;
	if(f == NULL){
		perror(s);
		return;
	}
	while((c=getc(f)) != EOF)
		if(sflag)
			sput(c);
		else
			put(c);
	fclose(f);
}
sput(c)
	register c;
{
	if((c>=' ' && c<='~') || c=='\t' || c=='\n')
		putchar(c);
}
put(c)
	register c;
{
	register inc=c;
	if (c=='\n') {
		if(lastc == ' ') {
			putchar('\\');
			putchar('n');
		}
	} else {
		if (col >= (72-4-2)) {
			putchar('\\');
			putchar('\n');
			putchar('\t');
			col = 8;
		}
		if(tflag && c=='\t')
			col |= 7;
		else if (c=='\b' || c=='\\' || c=='\t') {
			putchar('\\');
			c = c=='\b'? 'b' : c=='\t'? 't' : '\\';
			col++;
		} else if ((c&0200) || c<' ' || c=='\177') {
			putchar('\\');
			putchar(((c>>6)&03)+'0');
			putchar(((c>>3)&07)+'0');
			c     = ( c    &07)+'0';
			col += 3;
		}
		col++;
	}
	putchar(c);
	if(c == '\n')
		col=0;
	lastc=inc;
}
