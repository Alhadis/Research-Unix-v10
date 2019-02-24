#include "cbt.h"
extern char *malloc();

bfile *bt;
mbuf *key;
char *state;
#define IN	1
#define OUT	2
int cnt, keysz;
char *buf;
char cmdbuf[128];
mbuf rec = {"", 0};
extern char *outkey();
char *filename;

main(argc, argv)
char **argv;
{	int i;
	if(argc != 3)
		error("usage: file key-cnt");
	bt = bopen(filename = argv[1], 2);
	if(bt == 0)
		error("bopen");
	sprintf(cmdbuf, "btdiag %s", argv[1]);
	cnt = atoi(argv[2]);
	printf("cnt = %d\n", cnt);
	key = (mbuf *)malloc(cnt * sizeof(mbuf));
	keysz = NDSZ/5;
	if(keysz > MAXKLEN)
		keysz = MAXKLEN;
	printf("keysz %d\n", keysz);
	buf = malloc((keysz + 1) * cnt);
	state = malloc(cnt);
	if(buf == 0 || state == 0)
		error("key malloc");
	for(i = 0; i < cnt; i++)
		state[i] = OUT;
	for(i = 0; i < cnt; i++)
		key[i].mlen = keysz;
	for(i = 0; i < cnt; i++)
		key[i].mdata = buf + i * (keysz + 1);
	srand(0);
	for(i = 0; i < cnt * (keysz + 1); i++)
		buf[i] = rand() % 94 + ' ' + 1;
	for(i = 0; i < cnt; i++)
		buf[i * (keysz + 1) + keysz] = 0;
	doit();
	printf("sbrk %d\n", sbrk(0));
	exit(0);
}

doit()
{
	allin();
	test("allin ok\n");
	checkseek();
	out(500);
	test("out 500 ok\n");
	checkseek();
	in(500);
	test("in 500 ok\n");
	checkseek();
	out(700);
	test("out 700 ok\n");
	checkseek();
	in(700);
	test("in 700 ok\n");
	checkseek();
	out(1000);
	test("out 1000 ok\n");
	checkseek();
	in(100);
	test("in 100 ok\n");
	checkseek();
	in(200);
	test("in 200 ok\n");
	checkseek();
	in(300);
	test("in 300 ok\n");
	checkseek();
}

test(s)
char *s;
{	int n;
	bclose(bt);
	n = system(cmdbuf);
	if(n) {
		printf("%s returned %d\n", cmdbuf, n);
		exit(1);
	}
	else
		printf(s);
	bt = bopen(filename, 2);
	if(bt == NULL)
		error(filename);
}

error(s)
char *s;
{
	perror(s);
	exit(1);
}

allin()
{	int i;
	for(i = 0; i < cnt; i++) {
		if(state[i] == IN)
			continue;
		if(bwrite(bt, key[i], rec) == EOF)
			printf("write error %d on key %d\n", errno, i);
		else
			state[i] = IN;
	}
}

in(n)
{	int i;
	for(i = 0; i < cnt; i++) {
		if(state[i] == IN || (rand() % 1007) >= n)
			continue;
		else if(bwrite(bt, key[i], rec) == EOF)
			printf("write error %d on key %d\n", errno, i);
		else
			state[i] = IN;
	}
}

out(n)
{	int i, count = 0;
	for(i = 0; i < cnt; i++) {
		if(state[i] == OUT || (rand() % 1007) >= n)
			continue;
		if(bdelete(bt, key[i], rec) != 1) {
			printf("bdelete error %d on key %s\n", errno, outkey(i));
			printf("out(%d) deleted %d\n", n, count);
			bflush(bt);
			exit(1);
		}
		else if(bt->fatal)
			printf("set fatal flag, bdelete key %d %s err %d\n", i, outkey(i), errno);
		else {
			count++;
			state[i] = OUT;
		}
	}
}

checkseek()
{	int i, j, count = 0;
	mbuf x;
	printf("\tcheckseek");
	for(i = 0; i < cnt; i++) {
		if(state[i] == OUT)
			continue;
		if(bseek(bt, key[i]) != FOUND) {
			printf("sought key %s, not found\n", key[i].mdata);
			continue;
		}
		x = bkey(bt);
		for(j = 0; j < keysz; j++)
			if(key[i].mdata[j] != x.mdata[j]) {
				printf("bkey mismatch at key %d\n", i);
				break;
			}
		count++;
	}
	printf(" saw %d\n", count);
}

char *
outkey(n)
{	static char kb[MAXKLEN + 1];
	int i;
	strncpy(kb, key[n].mdata, keysz);
	return(kb);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n82/10/9:btexer.c\n"};
/*1101101100011000*/
