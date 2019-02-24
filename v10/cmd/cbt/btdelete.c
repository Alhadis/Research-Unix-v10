#include "stdio.h"
#include "cbt.h"

/* read keys from stdin, and delete the records from argv[1] */
char keybuf[NDSZ];
char recbuf[32767];
mbuf key = { keybuf, 0};
mbuf rec = { recbuf, 0};
bfile *bf;
int cflag;

main(argc, argv)
char **argv;
{	int n;
	if(argv[1][0] == '-') {
		cflag++;
		argc--;
		argv++;
	}
	if(argc != 2) {
		fprintf(stderr, "usage: btdelete bfile < recs\n");
		exit(1);
	}
	if((bf = bopen(argv[1], 2)) == NULL) {
		perror(argv[1]);
		exit(1);
	}
	for(;;) {
		get(2, (char *)&key.mlen);
		if(key.mlen > MAXKLEN) {
			fprintf(stderr, "key too long\n");
			exit(1);
		}
		get(key.mlen, key.mdata);
		get(2, (char *)&rec.mlen);
		if(rec.mlen > sizeof(recbuf)) {
			fprintf(stderr, "rec len %ud: recompile btdelete\n",
				rec.mlen);
			abort();
		}
		get(rec.mlen, rec.mdata);
		(void)bdelete(bf, key);
		if(cflag)
			bcheck(bf);
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
			fprintf(stderr, "io error in btdelete\n");
			abort();
		}
		s += m;
	}
}

mbuf new, old;
char newbuf[NDSZ], oldbuf[NDSZ];
bcheck(bf)
bfile *bf;
{	long i;
	int j, k;
	char *p;
	bfirst(bf);
	new.mdata = newbuf;
	old.mdata = oldbuf;
	for(i = 0;; i++) {
		if(bread(bf, &new, (mbuf *)NULL) == EOF)
			break;
		if(i > 0) {
			k = new.mlen;
			if(old.mlen < new.mlen)
				k = old.mlen;
			for(j = 0; j < k; j++)
				if(new.mdata[j] != old.mdata[j])
					break;
			if(j != k) {
				if(new.mdata[j] > old.mdata[j])
					continue;
bad:
				fprintf(stderr, "disorder at key %ld\n", i);
				key.mdata[key.mlen] = 0;
				fprintf(stderr, "key is:%s:\n", key.mdata);
				abort();
			}
			if(old.mlen >= new.mlen)
				goto bad;
		}
		p = old.mdata;
		old = new;
		new.mdata = p;
	}
}
