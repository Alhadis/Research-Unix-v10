#include "stdio.h"
struct rec {
	long len;
	struct rec *next;
	char *fname;
	long cnt[1];
} *proFptr = (struct rec *)-1;	/* end of list marker */
exit(n)
{	int i;
	char xbuf[4096];
	setbuf(stderr, xbuf);
	if(freopen("prof.out", "a", stderr) == 0)
		_cleanup(), _exit(n+1);
	while(proFptr != (struct rec *)-1) {
		fprintf(stderr, "%s\n", proFptr->fname);
		for(i = 3; i < proFptr->len; i++)
			fprintf(stderr, "%d\n", proFptr->cnt[i-3], i);
		proFptr = proFptr->next;
	}
	fflush(stderr);
	_cleanup();
	_exit(n);
}
