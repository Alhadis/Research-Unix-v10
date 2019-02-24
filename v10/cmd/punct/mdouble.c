#include <stdio.h>
#include "cbtype.h"
char *getl();
char *filename;
int lineno;
char *beg1, *beg2;
char left = '\0';
char right = '\0';
main(argc,argv)
char *argv[];
{
	int doub = 0;

	if(argc <= 1){
	filename = "stdin";
		doub = work();
	}
	else {
		while(argc > 1){
			argc--; argv++;
			if(freopen(argv[0], "r", stdin) == NULL){
				fprintf(stderr,"DOUBLE CANNOT OPEN FILE %s\n",argv[0]);
				continue;
			}
			filename = argv[0];
			doub += work();
		}
	}
	exit(doub);
}
char buf1[512];
char buf2[512];
int toggle;
char *p1, *p2;
char word[80], prev[80];
char delim[] = "delim";
work()
{
	char *b;
	int doub;
	int flag, lend;
	int skip;
	int l;
	doub = 0;
	lend = 1;
	lineno = 1;
	skip = 0;
	beg1 = p1 = getl(buf1);
	toggle = 1;
        strcpy(prev,"qqxz");
	while((flag=getword()) != 0){
		if(flag == 2)
			if(toggle == 1)b=beg2;
			else b=beg1;
		else if(toggle == 1)b = beg1;
		else b = beg2;
		if(*b == '.'){
			if(strcmp(word,"TS") == 0 || strcmp(word,"EQ")== 0 ||
			    strcmp(word,"DS") == 0 || strcmp(word,"PS") == 0){
				skip++;
				continue;
			}
			else if(strcmp(word, "TE") == 0|| strcmp(word,"EN") == 0 ||
			      strcmp(word,"DE") == 0 ||strcmp(word,"PE") == 0){
				if(--skip < 0)skip = 0;
				continue;
			}
		}
		if(strcmp(word, delim) == 0 && skip){
			getdel();
			continue;
		}
                if (strcmp(word, prev) ==0 && skip == 0){
                        if(word[1]=='\0' && word[0] !='a')
                                ;
			else {
				if(lend == 1 && flag != 3){
					if(flag == 2)l=lineno-1;
					else l=lineno;
					printf("%s twice line %d file %s\n",prev,l,filename);
					if(flag == toggle)
						printf("%s\n",buf1);
					else printf("%s\n",buf2);
				}
				else {
					printf("%s twice beginning line %d file %s\n",prev,lineno-1,filename);
					if(toggle == 2)printf("%s%s\n",buf1,buf2);
					else printf("%s%s\n",buf2,buf1);
				}
                                doub++;
				flag = 1;
                        }
                }
                strcpy(prev,word);
		lend = flag;
        }
        return(doub);
}
getword()
{
	int c;
	char *p, *w;
	int reset;
	int ret;
	int skip;
	ret = 1;
	reset=skip=0;
	w = word;
	if(toggle == 1)p=p1;
	else p=p2;
rest:
	while(*p != '\n'){
		if(*p == left){
			if(skip == 1 && *p == right)skip = 0;
			else {
				skip = 1;
				strcpy(prev,"qqxz");
			}
			p++;
			continue;
		}
		if(*p == right){
			skip = 0;
			p++;
			continue;
		}
		if(skip){
			p++;
			continue;
		}
		if(isalpha(*p) ||(w > word &&(*p == '-' || *p == '\''|| isdigit(*p))))
			*w++ = *p++;
		else if(w == word){
			if(*p == '\\'){
				p++;
				if(*p == '(')p+=2;
				strcpy(prev,"qqxz");
				ret=1;
				reset=1;
			}
			if(isdigit(*p) || isop(*p)|| *p == '/' || *p == '('){
				strcpy(prev,"qqxz");
				ret=1;
				reset=1;
			}
			p++;
		}
		else {
			*w = '\0';
			if(toggle == 1)p1 = p;
			else p2 = p;
			return(ret);
		}
        }
	lineno++;
	if(toggle == 1){
		beg2 = p = p2 = getl(buf2);
		toggle = 2;
	}
	else {
		beg1 = p = p1 = getl(buf1);
		toggle = 1;
	}
	if(p == EOF)return(0);
	if(w == word){
		if(reset == 0)ret = 3;
		goto rest;
	}
	*w = '\0';
	return(2);
}
char *
getl(p)
char *p;
{
	int c;
	char *q;
	q = p;
	while((*q++ = c = getchar()) != '\n'){
		if(q >= p + 511){
			printf("out of buffer space\n");
			exit(1);
		}
		if(c == EOF)
			return((char *)c);
	}
	*q = '\0';
	return(p);
}
getdel(){
	char *p;
	if(toggle == 1)p=p1;
	else p=p2;
	while(*p == ' ')p++;
	left = *p++;
	right = *p++;
	if(toggle == 1)p1=p;
	else p2=p;
}
