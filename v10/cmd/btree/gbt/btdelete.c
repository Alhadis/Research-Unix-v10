/* read keys from stdin, and delete the records from argv[1] */
#include "stdio.h"
#include "cbt.h"

char buf[NDSZ];
bfile *bf;
mbuf key;

main(argc, argv)
char **argv;
{	char *p;
	int c = 0;
	if(argc != 2) {
		fprintf(stderr, "usage: delete b-tree < keys\n");
		exit(1);
	}
	if((bf = bopen(argv[1], 2)) == NULL) {
		perror(argv[1]);
		exit(1);
	}
	key.mdata = buf;
loop:
	if(c == EOF) {
		bclose(bf);
		exit(0);
	}
	for(p = buf; (c = getchar()) != '\n' && c != EOF; *p++ = c)
		;
	*p = 0;
	key.mlen = p - buf;
	if(key.mlen == 0)
		goto loop;
	if(!bdelete(bf, key))
		fprintf(stderr, "not in file:%s\n", buf);
	goto loop;
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/8/9:btdelete.c\n"};
/*0010110100010101*/
