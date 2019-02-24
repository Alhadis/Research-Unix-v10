#define P       98
#define Q       27
#define DELAY   9800    /* 98*40 might be adequate */

/* Feedback shift register random number generator.
 * Steve Zucker -- March 5, 1975
 * Period is 2**98 - 1.
 * See JACM, Vol 20 No 3, July, 1973, pp. 456-468.
 * Initialization modified: J. Gillogly, 11 Mar 75
 * Modified to use long ints 12 Apr 77
 */

int rand_t[P],rand_x[P];
int *rand_end = {&rand_t[P]};
int *rand_j   = {&rand_t[P]};

random()
{       register int *j, *k, *l;
        if ( (j = ++rand_j) >= (k = rand_end) )
                rand_j = j = rand_t;
        return (*j ^= ( (l = j+Q) >= k ? *(l-P) : *l) );
}


#define TABSIZE  8000    /* table of initial values */
char *ivp;              /* pointer to the table */
int pshift;             /* will be set to P/16 */
int mask[16];           /* mask[0] has a 1-bit in the 0 position, etc */
long p2n[27], pq2n[27];

srandom(n)        /* compatible with unix initialization */
int n;
{       char buf[14];
	sprintf(buf,"%d",n & 0xffff);    /* convert number to string */
	initr(buf);             /* and use that to initialize */
}

initr(c)
char *c;
{       register int col, *k, j;
	long offset;
        int m,l,*i,deb;
        auto char iv[TABSIZE];
        auto int column[P/16+1];
        pshift=P/16;
        for (j=0; j<16; j++) mask[j]=1<<j;
	p2n[0]=P;      /* store 2**N*P */
	pq2n[0]=P-Q;  /* and 2**N*(P-Q) */
	for (j=1; j<27; j++)
	{       p2n[j]=p2n[j-1]<<1;
		pq2n[j]=pq2n[j-1]<<1;
	}
        if (c)
        {       for (i=rand_x, j=0, ivp=iv; j<P; j++)   /* bits in rand_x */
                        *ivp++ = *i++ = (*(c+(j>>3)) >> (j&07)) & 01;
                for (k=rand_t; k<rand_end;) *k++ = 0;
                for (ivp = &iv[P]; ivp<&iv[TABSIZE]; ivp++) /* init table */
                        *ivp = *(ivp-P+Q) ^ *(ivp-P);
                ivp=iv; /* point to table so find can find it */
		offset=P*5;     /* getting up to P*5000 */
		offset=(offset<<10)-P*120; /* NG for higher offsets */
                for (l=15; l--;)        /* for each column */
		{       find(offset+=DELAY,column);    /* result stored in column */
                        for (j=0; j<P; j++)
                        {       col=column[j>>4];       /* pick right result*/
                                m = 1<<(15-(j%16));     /* and bit posn */
                                if ((col&m) != 0)       /* the bit is on */
                                    rand_t[j] |= 1<<l;  /* turn on in init */
                        }
                }
                for (k=rand_x, i=rand_t; i<rand_end;) *k++ = *i++;
        }
        rand_j = rand_end;
        for (i=rand_t, k=rand_x; i<rand_end;) *i++ = *k++;
}

find(offset,result)
long offset;
int *result;           /* P/16-long column of results */
{       register int n,l;
        register char *i;
	long j,k;
        auto int lcol[P/16+1],rcol[P/16+1];      /* to save results in */
	if (offset<TABSIZE-P)
        {       i=ivp+(n=offset);               /* pt at right place in iv */
                for (l=0; l<=pshift; l++)       /* abt P/16 result words */
                {       result[l]=0;            /* turn off all bits */
                        for (n=0; n<16; n++)    /* cycle thru the bits in wd*/
                                if (*i++)       /* next initial bit on */
                                        result[l] |= mask[15-n];
                        /* put next initial bit into the right result bit */
                }
                return;
        }
        /* now find max n for which offset-p*2**n>0, and will use that n */
	for (n=0; p2n[n]<=offset; n++)
		if (n>26) printf("ERROR: exceeded 2**N*P table\n");
	if (n==0) printf("ERROR: finding max after done!\n");
	j=offset-pq2n[n-1];
	k=offset-p2n[n-1];
	if (j<TABSIZE-P)                        /* no need to call again */
        {       i=ivp+(n=j);                    /* pt at right place in iv */
                for (l=0; l<=pshift; l++)       /* abt P/16 result words */
                {       lcol[l]=0;              /* turn off all bits */
                        for (n=0; n<16; n++)    /* cycle thru the bits in wd*/
                                if (*i++)       /* next initial bit on */
                                        lcol[l] |= mask[15-n];
                        /* put next initial bit into the right result bit */
                }
        }
        else find(j,lcol);
	if (k<TABSIZE-P)                        /* no need to call again */
        {       i=ivp+(n=k);                    /* pt at right place in iv */
                for (l=0; l<=pshift; l++)       /* abt P/16 result words */
                {       rcol[l]=0;              /* turn off all bits */
                        for (n=0; n<16; n++)    /* cycle thru the bits in wd*/
                                if (*i++)       /* next initial bit on */
                                        rcol[l] |= mask[15-n];
                        /* put next initial bit into the right result bit */
                }
        }
        else find(k,rcol);
        for (l=0; l<=P>>4; l++) result[l] = lcol[l]^rcol[l];
}


