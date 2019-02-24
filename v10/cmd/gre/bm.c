#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

/*
	this next one is dirty but i can't think of a good way out now.
	essentially, LARGE has to be a bit bigger than the biggest buffer
	we are ever given by rdfn
*/
#define		LARGE	(100000000+2)

static int bb(re_bm *, unsigned char*, unsigned char*);

re_bm *
re_bmcomp(char *ppb, char *ppe, unsigned char *map)
{
	register unsigned char *pb = (unsigned char *)ppb;
	register unsigned char *pe = (unsigned char *)ppe;
	register int j;
	int delta[256];
	register re_bm *b;

	b = (re_bm *)malloc(sizeof *b);
	if(b == 0){
		re_error("b malloc fail");
		return 0;
	}
	b->patlen = pe-pb;
	memmove((char *)b->cmap, (char *)map, sizeof b->cmap);
	b->bmpat = malloc(b->patlen);
	if(b->bmpat == 0){
		re_error("bmpat malloc fail");
		free((char*)b);
		return 0;
	}
	if (bb(b, pb, pe) == 0){
		free((char*)b->bmpat);
		free((char*)b);
		return 0;
	}
	for(j = 0; pb+j < pe; j++)
		b->bmpat[j] = b->cmap[pb[j]];
	for(j = 0; j < 256; j++)
		delta[j] = b->patlen;
	for(pe--; pb < pe; pb++)
		delta[b->cmap[*pb]] = pe-pb;
	delta[b->cmap[*pb]] = LARGE;
	for(j = 0; j < 256; j++)
		b->delta0[j] = delta[b->cmap[j]];
	return(b);
}

static int
bb(register re_bm *b, register unsigned char *pb, register unsigned char *pe)
{
	int *f;
	register m = pe-pb;
	register i, k, j;

	f = (int *)malloc(sizeof(int)*(m+1));
	if(f == 0){
		re_error("delta2 f malloc");
		return 0;
	}
	pb--;
	b->delta2 = (int *)malloc(sizeof(int)*(b->patlen+1));
	if(b->delta2 == 0){
		re_error("delta2 malloc");
		free((char*)f);
		return 0;
	}		
	for(i = 1; i <= m; i++)
		b->delta2[i] = 2*m-i;
	j = m;
	k = m+1;
	while(j > 0){
		f[j] = k;
		while((k <= m) && (pb[j] != pb[k])){
			if(m-j < b->delta2[k])
				b->delta2[k] = m-j;
			k = f[k];
		}
		j--;
		k--;
	}
	for(i = 1; i <= k; i++){
		if(b->delta2[i] > m+k-i)
			b->delta2[i] = m+k-i;
	}
	free((char *)f);
	return 1;
}

re_bmexec(void *re_b, RDFN rdfn, MATCHFN matchfn)
{
	register re_bm *b = (re_bm*)re_b;
	register unsigned char *sp;
	register unsigned char *e, *s;
	unsigned char *re, *rs;
	register k;

	for(rs = 0; (k = (*rdfn)((char**)&rs, (char**)&re)) > 0; rs = s, re = e){
		e = re;
		s = rs;
		while(s < e){
			while((s += b->delta0[*s]) < e)
				;
			if(s < e+b->patlen){	/* no match */
				s = e;
				break;
			}
			s -= LARGE;
			for(k = b->patlen-2, sp = s-1; k >= 0; k--)
				if(b->cmap[*sp--] != b->bmpat[k])
					break;
/*print("k=%d s=%d sp=%d\n", k, s, sp);/**/
			if(k < 0){
				unsigned char *bm = ++sp, *em = s+1;
				if((k = (*matchfn)((char**)&bm, (char**)&em)) <= 0)
					return(k);
				s = bm;
				e = em;
			} else {
				/*j = b->delta2[k+1];
				k = b->delta0[cmap[*++sp]];
				if((j > k) || (k == LARGE))
					 k = j;
				s = sp+k;*/s++;
			}
		}
	}
	return(k);
}

void
re_bmfree(re_bm *pat)
{
	free((char *)pat->delta2);
	free(pat->bmpat);
	free((char *)pat);
}
