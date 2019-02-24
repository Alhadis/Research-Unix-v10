#include "stdio.h"
typedef unsigned long ul;
typedef struct {
	char *fname;
	int len;	/* how many counts have been seen */
	int quot;	/* how much has been allocated for cnt */
	unsigned long *cnt;
} stab;
stab *tab;
int ntab, ltab;
FILE *fd, *sfd, *cfd;
char buf[256];
char fname[512] = "/";
extern char *malloc();
char flg[128];
unsigned long val;

main(argc, argv)
char **argv;
{	int i, j;
	if(argc <= 1) {
		fprintf(stderr, "subtract from where?\n");
		exit(1);
	}
	if((fd = fopen("prof.out", "r")) == 0) {
		perror("prof.out");
		exit(1);
	}
	readall(fd);
	fclose(fd);
	sfd = fopen(argv[1], "r");
	if(sfd == 0) {
		perror(argv[1]);
		exit(1);
	}
	suball(sfd);
	fclose(sfd);
	if((fd = fopen("prof.out", "w")) == 0) {
		perror(argv[1]);
		exit(1);
	}
	for(i = 0; i < ntab; i++)
		rewrite(tab + i);
	exit(0);
}

readall(fd)
FILE *fd;
{	int c, i, index;
	stab *curtab = 0;
sawnl:
	if((c = getc(fd)) == EOF)
		return;
	if(c == '\n')
		goto sawnl;
	if(c == '/') {
		fscanf(fd, "%s", fname+1);
		for(i = 0; i < ntab; i++)
			if(strcmp(fname, tab[i].fname) == 0)
				break;
		if(i >= ntab) {	/* new file */
			if(ltab == 0) {
				tab = (stab *)malloc(20 * sizeof(stab));
				ltab = 20;
			}
			else if(ntab >= ltab)
				tab = (stab *)realloc((char *)tab,
					(ltab += 20) * sizeof(stab));
			tab[ntab].fname = malloc(sizeof(fname) + 1);
			strcpy(tab[ntab].fname, fname);
			tab[ntab].len = tab[ntab].quot = 0;
			ntab++;
		}
		curtab = tab + i;
		index = 0;
	}
	else if(c < '0' || c > '9') {
		fprintf(stderr, "prof.out has weird format\n");
		abort();
	}
	else {
		ungetc(c, fd);
		fscanf(fd, "%d", &val);
		if(curtab->len <= index) {
			if(curtab->quot == 0) {
				curtab->cnt = (ul *)malloc(100*sizeof(long));
				curtab->quot = 100;
				for(i = 0; i < 100; i++)
					curtab->cnt[i] = 0;
			}
			else if(curtab->len >= curtab->quot) {
				curtab->cnt = (ul *)realloc(curtab->cnt,
					(curtab->quot += 200) * sizeof(long));
				for(i = curtab->quot-200; i < curtab->quot; i++)
					curtab->cnt[i] = 0;
			}
			curtab->len++;
		}
		curtab->cnt[index++] += val;
	}
	goto sawnl;
}

rewrite(x)
stab *x;
{	int i;
	for(i = 0; i < x->len; i++)
		if(x->cnt[i])
			break;
	if(i >= x->len)
		return;
	fprintf(fd, "%s\n", x->fname);
	for(i = 0; i < x->len; i++)
		fprintf(fd, "%u\n", x->cnt[i]);
}
suball(fd)
FILE *fd;
{	int c, i, index;
	stab *curtab = 0;
sawnl:
	if((c = getc(fd)) == EOF)
		return;
	if(c == '\n')
		goto sawnl;
	if(c == '/') {
		fscanf(fd, "%s", fname+1);
		for(i = 0; i < ntab; i++)
			if(strcmp(fname, tab[i].fname) == 0)
				break;
		if(i >= ntab) {	/* new file */
			fprintf(stderr, "new file %s ignored\n", fname);
			goto sawnl;
		}
		curtab = tab + i;
		index = 0;
	}
	else if(c < '0' || c > '9') {
		fprintf(stderr, "prof.out has weird format\n");
		abort();
	}
	else {
		ungetc(c, fd);
		fscanf(fd, "%d", &val);
		curtab->cnt[index++] -= val;
	}
	goto sawnl;
}
