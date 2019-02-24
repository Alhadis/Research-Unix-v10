%{
#include "pp.h"
#include <stdio.h>
#ifdef TEST
main(){
	register type, c;
	while((type=yylex())!=0) switch(type){
	case OTHER:		printf("%s", yytext); break;
	case KEYWORD:		printf("\\fB%s\\fR", yytext); break;
	case FUNCTION:		printf("Function: %s", yytext); break;
	case COMMENT:
		printf("\\fI%s", yytext);
		for(;;){
			putchar(c=input());
			if(c==0)
				break;
			if(c=='*'){
			GotStar:
				putchar(c=input());
				if(c=='/')
					break;
				if(c=='*')
					goto GotStar;
			}
			if(c=='\n'){
				while((c=input())=='\t')
					putchar(c);
				if(c==' ')
					putchar('^');
				else{
					putchar(c);
					if(c=='*')
						goto GotStar;
				}
			}
		}
		printf("\\fR");
		break;
	}
}
#endif
char *keyword[]={
"asm", "auto", "break", "char", "case", "continue", "double",
"default", "do", "extern", "else", "enum", "for", "float",
"fortran", "goto", "if", "int","long",  "return", "register",
"switch", "struct", "sizeof", "short", "static", "unsigned",
"union", "void", "while", "typedef",
NULL
};
#define	NWORD	150
char *hashtab[NWORD];
int kwinit=0;
hash(s)
register char *s;
{
	register i=0, j=0;
	while(*s)
		i += *s++ * ++j;	/* got that? */
	if(i<0)
		return(i%NWORD+NWORD);
	return(i%NWORD);
}
char *copy(s)
register char *s;
{
	char *malloc();
	register char *t=malloc(strlen(s)+1);
	if(t==NULL)
		error("Out of space", (char *)0);
	strcpy(t, s);
	return(t);
}
insert(s)
register char *s;
{
	register char **h;
	register i;
	h = &hashtab[hash(s)];
	for(i=0;i!=NWORD && *h!=NULL;i++){
		if(strcmp(*h, s)==0)
			return;
		if(++h==&hashtab[NWORD])
			h=hashtab;
	}
	if(*h!=NULL)
		error("keyword hash table overflow", (char *)0);
	*h = s;
}
lookup(s)
register char *s;
{
	register char **h;
	register i;
	if(!kwinit){
		for(h=keyword;*h!=NULL;h++)
			insert(*h);
		kwinit++;
	}
	h = &hashtab[hash(s)];
	for(i=0;i!=NWORD && *h!=NULL;i++){
		if(strcmp(s, *h)==0)
			return(KEYWORD);
		if(++h==&hashtab[NWORD])
			h=hashtab;
	}
	return(OTHER);
}
ckeywords(file)
	char *file;
{
	char buf[128], *fgets(), *malloc();
	FILE *f=fopen(file, "r");
	register char *p;
	if(f==NULL){
		if(*file!='/'){
			sprintf(buf, "/usr/lib/pp/%s", file);
			f=fopen(buf, "r");
		}
		if(f==NULL)
			error("can't find keyword file", file);
	}
	while(fgets(buf, sizeof buf, f)){
		buf[strlen(buf)-1]=0;
		p=malloc(strlen(buf)+1);
		if(p==NULL)
			error("can't malloc a string", (char *)0);
		strcpy(p, buf);
		insert(p);
	}
	fclose(f);
}
%}
WS	([ \t]*)
%%
^#{WS}ifdef			return(KEYWORD);
^#{WS}ifndef			return(KEYWORD);
^#{WS}endif			return(KEYWORD);
^#{WS}else			return(KEYWORD);
^#{WS}if			return(KEYWORD);
^#{WS}define			return(KEYWORD);
^#{WS}undef			return(KEYWORD);
^#{WS}include			return(KEYWORD);
^#{WS}line			return(KEYWORD);
[_a-zA-Z][_a-zA-Z0-9]*		return(lookup(yytext));
"/*"				return(COMMENT);
^[^ \t#\n].*"(".*[){]{WS}$	return(FUNCTION);
\"(\\(.|\n)|[^"\n])*["\n]	return(OTHER);
.|\n				return(OTHER);
