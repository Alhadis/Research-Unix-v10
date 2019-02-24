#include <stdio.h>
#include <string.h>

#define VERSION 1

extern char *xs();
extern char *getpass(), *getlogin();
extern char *ctime(), *malloc(), *realloc();
extern long time();
extern void exit();
char *slurp();
char name[9];

main(ac,av)
char **av;
{
	long tloc;
	int n, headlen;
	FILE *fil;
	char *date, *doc, *sig, *key, *head;
	char prompt[100], hexdate[10];

	if(ac>2 && strcmp(av[1], "-n")==0){
		strncpy(name, av[2], 8);
		av += 2;
		ac -= 2;
	} else
		strcpy(name,getlogin());

	switch(ac){
	case 2:
		fil = fopen(av[1],"r");
		if(fil == 0){
			write(2,"sign: ",6);
			perror(av[1]);
			exit(1);
		}
		break;
	case 1:
		fil = stdin;
		break;
	default:
		fprintf(stderr,"usage: sign [-n name] [file]\n");
		exit(1);
		break;
	}

	(void)time(&tloc);
	date = ctime(&tloc);
	date[24] = 0;
	sprintf(hexdate,"%08lx",tloc);
	headlen = 24+8+strlen(name);
	head = malloc(headlen+1);
	strcpy(head,date);
	strcat(head,hexdate);
	strcat(head,name);

	doc = slurp(fil, &n, head);
	sprintf(prompt, "%s's signing key: ", name);
	key = getpass(prompt);
	if(key == 0) {
		fprintf(stderr, "sign: failed to get password\n");
		exit(1);
	}
	sig = xs(key, doc, n);
	n -= headlen;
	while(*key)
		*key++ = 0;

	printf("Signed by %s, %s\n", name, date);
	printf("sum=%s, date=%s, count=%d, ver=%d\n",
			sig, hexdate, n, VERSION);
	printf("------\n");
	fwrite(doc+headlen,1,n,stdout);
	printf("------\n");
	printf("sum=%s, date=%s, count=%d, ver=%d\n",
			 sig, hexdate, n, VERSION);
	printf("End %s, %s\n", name, date);
	exit(0);
}

char *
slurp(fil, np, date)
FILE *fil;
int  *np;
char *date;
{
	char *s;
	unsigned n, ntotal;
	int nl, c;
	int nleft;

	nleft = 0;
	s = strdup(date);
	ntotal = n = strlen(s);

	nl = 1;
	while((c=getc(fil)) != EOF) {
		if(nleft<=2) {
			int d = 500+ntotal/2;
			nleft += d;
			ntotal += d;
			s = realloc(s, ntotal);
			if(s==0) {
				fprintf(stderr,"sign: out of space\n");
				exit(1);
			}
		}
		if(nl) {
			s[n++] = ',';
			nleft--;
		}
		nl = 0;
		s[n++] = c;
		nleft--;
		if(c=='\n')
			nl = 1;
	}
	if(!nl) {
		s[n++] = '\n';
		fprintf(stderr,"sign: newline appended\n");
	}
	
	*np = n;
	return s;
}
