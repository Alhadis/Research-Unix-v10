/*%cc -O %
 */
#include <stdio.h>
struct line{
	struct line *next;
	int count;
	char str[1];
}line;
copy(p,q)
	register char *p, *q;
{
	while(*q++ = *p++);
}
int eof;
char *
getstr(buf)
	char *buf;
{
	register char *s, c;
	s=buf;
	if(eof){
		*s++='\0';
		return(buf);
	}
	while((c=getchar())!='\n' && c!=EOF)
		*s++=c;
	*s++='\0';
	if(c==EOF)
		eof++;
	return(buf);
}
int spcnt;
putch(c){
	register i;
	if(c==' ')
		spcnt++;
	else if(c=='\n'){
		spcnt=0;
		putchar(c);
	}
	else{
		for(i=0;i<spcnt;i++)
			putchar(' ');
		spcnt=0;
		putchar(c);
	}
}
length(s)
	register char *s;
{
	register i;
	for(i=0;*s++;i++);
	return(i);
}
char buf[2048];
char obuf[BUFSIZ];
main(){
	register struct line *p, *q;
	register done;
	p=0;
	setbuf(stdout,obuf);
	for(;;){
		getstr(buf);
		if(eof) break;
		q=p;
		p=(struct line *)malloc(sizeof line+length(buf));
		p->next=q;
		p->count=0;
		copy(buf, p->str);
	}
	for(;;){
		done=1;
		for(q=p;q;q=q->next){
			if(q->str[q->count]){
				putch(q->str[q->count++]);
				done=0;
			}
			else
				putch(' ');
		}
		if(done) break;
		putch('\n');
	}
}
