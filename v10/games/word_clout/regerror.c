#include <stdio.h>
regerror(s)
	char *s;
{
	char buf[132];

	strcpy(buf, "regerror: ");
	strcat(buf, s);
	strcat(buf, "\n");
	write(2, buf, strlen(buf));
	exit(1);
}
int storcnt;
static max;
char *MALLOC(x)
{
	int *p;
	if(x>max) {
		fprintf(stderr,"MALLOC(%d)\n",max=x);
	}
	p=(int*)malloc(x+3*sizeof*p);
	if(p) {
		storcnt+=*p=x;
		p[1]=-2*x;
		p[2+(x+7)/sizeof*p]=-3*x;
		return (char*)(p+2);
	}
	return 0;
}
FREE(p)
int *p;
{
	p-=2; storcnt-=*p;
	if(*p>max||*p<=0)ERR("big free",p);
	if(p[1]!=-2*p[0])ERR("free head scribble",p);
	if(p[2+(*p+7)/sizeof*p]!=-3**p)ERR("free tail scribble",p);
	free(p);
}
ERR(s,p)
char *s;
int *p;
{
	fprintf(stderr,"ERR: %s\n",s);
	fprintf(stderr,"p=%o\n",p);
	fprintf(stderr,"p[0]=%d\n",p);
	fprintf(stderr,"p[1]=%d\n",p[1]);
	fprintf(stderr,"p[2]='%s'\n",(char*)(p+2));
	fprintf(stderr,"p[trail]=%d",p[2+(*p+7)/sizeof*p]);
	fflush(stdout);
	abort();
}
