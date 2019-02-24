/*
 *	encryption service routines
 */

#include <stdio.h>

/* encryption parameters and tables */
#define ROTORSZ 256
#define MASK 0377
static char	t1[ROTORSZ];
static char	t2[ROTORSZ];
static char	t3[ROTORSZ];
static char	t4[ROTORSZ];

/* current rotor settings */
static int N1, N2;

resetN12()
{
	N1 = N2 = 0;
}

setup(pw)
char *pw;
{
	int ic, i, k, temp, pf[2];
	unsigned random;
	char buf[13];
	long seed;
	int pid;

	resetN12();

	strncpy(buf, pw, 8);
	while (*pw)
		*pw++ = '\0';
	buf[8] = buf[0];
	buf[9] = buf[1];
	pipe(pf);

	switch (pid = fork()) {

	case -1:
		fprintf (stderr, "seal: cannot fork\n");
		exit (1);

	case 0:
		close(0);
		close(1);
		dup(pf[0]);
		dup(pf[1]);
		execl("/usr/lib/makekey", "-", 0);
		execl("/lib/makekey", "-", 0);
		exit(1);

	default:
		write(pf[1], buf, 10);
		while (wait ((int *) NULL) != pid)
			;
	}

	if (read(pf[0], buf, 13) != 13) {
		fprintf(stderr, "seal: cannot generate key\n");
		exit(1);
	}
	seed = 123;
	for (i=0; i<13; i++)
		seed = seed*buf[i] + i;
	for(i=0;i<ROTORSZ;i++)
		t1[i] = i;
	for(i=0;i<ROTORSZ;i++) {
		seed = 5*seed + buf[i%13];
		random = seed % 65521;
		k = ROTORSZ-1 - i;
		ic = (random&MASK)%(k+1);
		random >>= 8;
		temp = t1[k];
		t1[k] = t1[ic];
		t1[ic] = temp;
		if(t3[k]!=0) continue;
		ic = (random&MASK) % k;
		while(t3[ic]!=0) ic = (ic+1) % k;
		t3[k] = ic;
		t3[ic] = k;
	}
	for(i=0;i<ROTORSZ;i++){
		t2[t1[i]&MASK] = i;
		t4[i] = (t1[i] + t3[i]) & 0377;
	}
}

mangle (buf, limit)
	char *buf;
	register char *limit;
{
	register int i;
	register char *p;
	register int n1 = N1, n2 = N2;
	int n3;

	p = buf;

	while(p < limit) {
		i = *p;
		n3 = t4[n1];
		i = t2[(t3[(t1[(i+n3)&MASK]+n2)&MASK]-n2)&MASK]-n3;
		*p++ = i;
		n1++;
		if(n1==ROTORSZ) {
			n1 = 0;
			n2++;
			if(n2==ROTORSZ) n2 = 0;
		}
	}
	N1 = n1;
	N2 = n2;
}
