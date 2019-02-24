/*	@(#)hashcheck.c	1.1	*/
/* reads a compressed spelling list from stdin and prints on stdout
hash and usage codes in octal*/
#include <stdio.h>
#include "hash.h"
long fetch();
unsigned short index[NI];
unsigned *table;
unsigned wp;
int bp;
#define U (BYTE*sizeof(unsigned))
#define L (BYTE*sizeof(long))

unsigned long codetable[EXTRA];

main()
{
	int i;
	long u,v;
	long a;
	extern char *malloc();
	fread((char*)codetable,sizeof(codetable),1,stdin);
	for(i=0; i<EXTRA; i++)
		printf("%hd\n",codetable[i]);
	if(!rhuff(stdin))
		error("can't read huffman code");
	if(fread((char*)index, sizeof(*index), NI, stdin)!=NI)
		error("can't read index");
	table = (unsigned*)malloc(index[NI-1]*sizeof(*table));
	if(fread((char*)table, sizeof(*table),
	   index[NI-1], stdin)!=index[NI-1])
		error("can't read hash table");
	for(i=0;i<NI-1;i++) {
		bp = U;
		v = (long)i<<(HASHWIDTH-INDEXWIDTH);
		for(wp=index[i];wp<index[i+1]; ) {
			if(wp==index[i]&&bp==U)
				a = fetch(0);
			else {
				a = fetch(0);
				if(a==0)
					break;
			}
			if(wp>index[i+1]||
				wp==index[i+1]&&bp<U)
				break;
			v += a;
			u = fetch(CODEWIDTH);
			printf("%.9lo %.5ld\n",v,u);
		}
	}
	return(0);
}

long fetch(width)
{
	long w;
	long y = 0;
	int empty = L;
	int i = bp;
	int tp = wp;
	while(empty>=i) {
		empty -= i;
		i = U;
		y |= (long)table[tp++] << empty;
	}
	if(empty>0)
		y |= table[tp]>>i-empty;
	if(width==0) 
		i = decode((y>>1)&((1L<<(BYTE*L-1))-1), &w);
	else {
		i = width;
		w = (y>>(L-width))&((1<<(BYTE*width))-1);
	}
	bp -= i;
	while(bp<=0) {
		bp += U;
		wp++;
	}
	return(w);
}

error(s)
char *s;
{
	fprintf(stderr,"hashcheck: %s\n", s);
	exit(1);
}
