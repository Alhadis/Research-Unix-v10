#include "stdio.h"
#include "cbt.h"
extern char *malloc(), *realloc();
extern mbuf bkey();
extern bfile *bopen();
int rflag;
int rsize = 64;
mbuf key, rec;
char tabchar = '\t';
main(argc, argv) char **argv;
{
	rec.mdata = malloc(rsize);
	key.mdata = malloc(NDSZ);
	for(argc--, argv++; argc > 0; argc--, argv++) {
		switch(*argv[0]) {
		case '-':
			if(argv[0][1] == 'R')
				rflag = 1;
			else fprintf(stderr, "unknown option %s\n", argv[0]);
			break;
		default:
			copy(argv[0]);
			break;
		}
	}
	exit(0);
}
copy(s) char *s;
{	bfile *bt;
	int n;
	bt = bopen(s, 0);
	if(bt == NULL) {
		perror(s);
		exit(1);
	}
	if(bfirst(bt) == EOF) {
		fprintf(stderr, "%s empty\n", s);
		return;
	}
	while((n = breclen(bt)) != EOF) {
		if(n > rsize) {
			rsize = n;
			rec.mdata = realloc(rec.mdata, rsize);
		}
		(void) bread(bt, &key, &rec);
		if(!rflag) {
			out(key);
			putchar(tabchar);
			out(rec);
			putchar('\n');
		}
		else {
			fwrite((char *)&key.mlen, 1, sizeof(short), stdout);
			fwrite(key.mdata, key.mlen, 1, stdout);
			fwrite((char *)&rec.mlen, 1, sizeof(short), stdout);
			fwrite(rec.mdata, rec.mlen, 1, stdout);
		}
			
	}
	bclose(bt);
}
out(a) mbuf a;
{	int i;
	for(i = 0; i < a.mlen; i++) putchar(*(a.mdata + i));
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/4/8:btcat.c\n"};
/*1110000011011001*/
