#include "stdio.h"
#include "cbt.h"

/* like btbuild, but random access */
char keybuf[NDSZ];
char recbuf[32767];
mbuf key = { keybuf, 0};
mbuf rec = { recbuf, 0};
bfile *bf;
int rflag;

main(argc, argv)
char **argv;
{	int n;
	if(argv[1][0] == '-') {
		rflag++;
		argc--;
		argv++;
	}
	if(argc != 2) {
		fprintf(stderr, "usage: btlook bfile < recs\n");
		exit(1);
	}
	if((bf = bopen(argv[1], 0)) == NULL) {
		perror(argv[1]);
		exit(1);
	}
	for(;;) {
		if(rflag)
			getrkey();
		else 
			getakey();
		if(bseek(bf, key) != 1) {
			fprintf(stderr, "not found:%s:\n",key.mdata);
			continue;
		}
		if(bread(bf, 0, &rec) != 0) {
			fprintf(stderr, "vanished:%s:\n", key.mdata);
			abort();
		}
		if(rec.mlen >= sizeof(recbuf)) {
			fprintf(stderr, "rec len %ud: recompile btlook\n",
				rec.mlen);
			abort();
		}
		rec.mdata[rec.mlen] = 0;
		if(rflag) {
			put(2, &key.mlen);
			put(key.mlen, key.mdata);
			put(2, &rec.mlen);
			put(rec.mlen, rec.mdata);
		} else
			printf("%s	%s\n", key.mdata, rec.mdata);
	}
}

get(n, s)
char *s;
{	int m;
	for(m = 0; n > 0; n -= m) {
		m = read(0, s, n);
		if(m == 0) {
			bclose(bf);
			exit(0);
		}
		if(m < 0) {
			fprintf(stderr, "btlook: io error\n");
			abort();
		}
		s += m;
	}
}

put(n, s)
char *s;
{
	if(fwrite(s, 1, n, stdout) != n) {
		fprintf(stderr, "btlook: io error\n");
		abort();
	}
}

getrkey()
{
	get(2, (char *)&key.mlen);
	if(key.mlen >= sizeof(keybuf)) {
		fprintf(stderr, "key too long\n");
		exit(1);
	}
	get(key.mlen, key.mdata);
	key.mdata[key.mlen] = 0;
	get(2, (char *)&rec.mlen);
	if(rec.mlen >= sizeof(recbuf)) {
		fprintf(stderr, "rec len %ud: recompile btlook\n", rec.mlen);
		abort();
	}
	get(rec.mlen, rec.mdata);
}

getakey()
{
	int c;
	char *s;
	for(s=key.mdata;;s++) {
		c = getchar();
		switch(c) {
		case EOF:
			exit(0);
		case '\t':
			for(;;) {
				switch(getchar()) {
				case EOF:
					exit(0);
				case '\n':
					goto out;
				}
			}
		case '\n':
			goto out;
		default:
			*s = c;
		}
	}
out:
	*s = 0;		
	key.mlen = s - key.mdata;
	if(key.mlen >= sizeof(keybuf)) {
		fprintf(stderr, "key too long:%s:\n", key.mdata);
		abort();
	}
}
