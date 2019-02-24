/* remove formatting information from collins thesaurus entries */
#include <stdio.h>
#include "/usr/jerq/include/jioctl.h"

int linesize = 65;
struct winsize winsize;

main()
{
	int c, d, n;
	if(ioctl(1,JWINSIZE,&winsize)==0)
		linesize = winsize.bytesx;
	for(;;) {
		c = getchar();
		switch(c) {
		case EOF:
			return(0);
		case '*':
			c = getchar();
			if(c=='S') {
				put('(');
				put(getchar());
				put(')');
			}
			break;
		case '#':
			c = getchar();
			switch(getchar()) {
			case '1': put('\'');
			case '2': put('`');
			case '3': put('^');
			case '4': put('"');
			case '5': put(',');
			}
			put('\b');
			put(c);
			break;
		case '+':
		case '<':
			break;
		default:
			put(c);
		}
	}
}

char word[50];
int linelen, wordlen;

put(c)
int c;
{
	word[wordlen++] = c;
	switch(c){
	case '\b':
		wordlen -= 2;
		break;
	case '-':
	case ' ':
		putword();
		break;
	case '\n':
		putword();
		newline();
		break;
	}
}

newline()
{
	putchar('\n');
	linelen = 0;
}

putword()
{
	word[wordlen] = 0;
	if(linelen+wordlen > linesize-2)
		newline();
	printf("%s", word);
	linelen += wordlen;
	wordlen = 0;
}
