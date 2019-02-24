#include <stdio.h>
#include "trace.h"

#define DEBUG	0

#if DEBUG
#define log(e, u, d)	event[e][u] += (long) d;
#else
#define log(e, u, d)
#endif

#define A_LARGE 64
#define A_USER 0x55000000

#define POOL	0
#define ALLOC	1
#define FREE	2
#define NREVENT	3

 union M {
	long size;
	union M *link;
 };

 union M *freelist[A_LARGE];
 long	 req[A_LARGE];
 long	 event[NREVENT][A_LARGE];

char *
talloc(u)
	long u;
{ union M *m;
  register r;
  char *Smalloc();

	u = ((u-1)/sizeof(union M) + 2);

	if( u >= A_LARGE )
	{	log(ALLOC, 0, (long) 1);
		m = (union M *) malloc( u * sizeof(union M) );
		if (m == NULL)
			whoops("malloc fault");
	} else
	{	if( freelist[u] == NULL )
		{	r = req[u] += req[u] ? req[u] : 1;
			if (r > NOTOOBIG)
				r = req[u] = NOTOOBIG+1;
			log(POOL, (int) u, (long) r);
			freelist[u] = (union M *) Smalloc( r*u*sizeof(union M) );
			if (freelist[u] == NULL)
				whoops("malloc fault");

			(freelist[u] + u*(r-1))->link = 0;
			for (m = freelist[u] + u*(r-2); m >= freelist[u]; m -= u)
					m->link = m+u;
		}
		log(ALLOC, (int) u, (long) 1);

		m = freelist[u];
		freelist[u] = m->link;
	}
	m->size = u | A_USER;
/*
	for (r = 1; r < u; )
		(&m->size)[r++] = 0;
*/
	return (char *) (m+1);
}

tfree(v)
	char *v;
{ register union M *m = (union M *) v;
  register long u;

	--m;
	if ( (m->size&0xFF000000) != A_USER)
	{	fprintf(stderr, "panic: releasing a free block\n");
		kill(getpid(), 3);
	}

	u = (m->size &= 0xFFFFFF);
	if ( u >= A_LARGE )
	{	log(FREE, (int) 0, (long) 1);
		free(m);
	}
	else
	{	log(FREE, (int) u, (long) 1);
		m->link = freelist[u];
		freelist[u] = m;
	}
}

#if DEBUG
double
a_stats()
{ register int i;
  long p, a, f, j;
  long sum = 0;

	fprintf(stderr, "chunk\t  pool\tallocs\t frees\t spill\n");
	for (i = 0; i < A_LARGE; i++)
	{
		p = event[POOL][i];
		a = event[ALLOC][i];
		f = event[FREE][i];

		if( !(p|a|f) )
			continue;

		j = (long) (i * sizeof(union M));
		fprintf(stderr, "%5d\t%6ld\t%6ld\t%6ld\t%6ld\n", j, p, a, f, a-f);

		sum += p*j;
	}
	fprintf(stderr, "total pools  %7u\n", sum);

	return (double) sum;
}
#endif
