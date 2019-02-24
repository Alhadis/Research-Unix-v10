/* takes the output fromt slvg | sort and puts it back into
 * form suitable for btbuild -R
 * it reduces the two byte codes, and it only keeps the
 * last version of each record.
 * the input format is
 * key tab 8-bytes-of-sequence 4-byete-of-value-len value newline
 */
#include "stdio.h"
#define BUF	20000
char bufa[BUF], bufb[BUF];
char *old = bufa;
char *new = bufb;
int len;

main()
{	char *p, *q;
	int c, i;
loop:
	for(p = new; (c = getchar()) != '\t' && c != EOF; *p++ = c)
		if(p - new >= BUF) {
			fprintf(stderr, "recompile slvg2 with more BUF\n");
			exit(1);
		}
	if(c == EOF)
		exit(0);
	*p++ = 0;
	for(i = 0; i < 8; i++)
		getchar();
	if((p - new != len || strcmp(old, new) != 0) && len > 0) {
		out(old);
		dorest();
	}
	len = p - new;
	q = old;
	old = new;
	new = q;
	ignore();
	goto loop;
}

out(s)
char *s;
{	short n;
	char c;
	n = (len - 1)/2;
	fwrite((char *)&n, 1, 2, stdout);
	for(; *s; s++) {
		c = (*s - '3') << 6;
		c |= *++s- ' ';
		putchar(c);
	}
}

ignore()
{	int c;
	while((c = getchar()) != '\n' && c != EOF)
		;
	if(c == EOF)
		exit(0);
}

dorest()
{	unsigned short n;
	int i;
	/* 4 bytes of length */
	for(i = n = 0; i < 4; i++)
		n |= (getchar() - 'a') << (4*i);
	fwrite((char *)&n, 1, 2, stdout);
	while((i = getchar()) != '\n' && i != EOF) {
		n = (i - '3') << 6;
		n |= getchar() - ' ';
		putchar(n);
	}
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/8/9:slvg2.c\n"};
/*0010100001111110*/
/*1110001101011101*/
