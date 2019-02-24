#include "stdio.h"
#define MAX	8192
char tab = '\t';
char *line, *oline;
char abuf[MAX], bbuf[MAX];
int cnt, ocnt;
main()
{	long lcnt = 0;
	line = abuf;
	oline = bbuf;
	for(; !feof(stdin); ) {
		lcnt++;
		(void) fgets(line, MAX, stdin);
		if(feof(stdin)) cnt = 0;
		else cnt = strlen(line);
		if(cnt >= MAX-1) {
			fprintf(stderr, "line %ld too long\n", lcnt);
			cnt = 0;
		}
		output();
	}
	exit(0);
}
output()
{	unsigned short u;
	char *p;
	if(ocnt <= 1) {	/* oline was used to give ooline a newline */
		swtch();
		return;
	}
	if(cnt != 1)	/* line isnt a newline to be added to oline */
		oline[--ocnt] = 0;
	else oline[ocnt] = 0;
	for(p = oline; *p && *p != tab; p++);
	u = p - oline;
	(void) fwrite((char *)&u, 1, sizeof(u), stdout);
	(void) fwrite(oline, (int)u, 1, stdout);
	if(*p == tab)
		p++;
	if(oline + ocnt - p < 0)
		u = 0;
	else u = oline + ocnt - p;
	(void) fwrite((char *)&u, 1, sizeof(u), stdout);
	(void) fwrite(p, (int)u, 1, stdout);
	swtch();
}
swtch()
{	char *x;
	x = line;
	line = oline;
	oline = x;
	ocnt = cnt;
}
static char VER[] = "\n80/2/13:btran.c\n";
/*1111100011110011*/
