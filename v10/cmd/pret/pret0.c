#include <stdio.h>
#include "pret.h"
#include "pret.d"

#define DEBUG6	0
#define DEBUG7	0

extern struct COL column[MANY];
extern struct ROW row[MANY];
extern int nrrows, nrcols, redrows;

struct ENTRY * find();
char * Emalloc();

char  **triangle;
short **successor;
short *tmpsucc;
int *unmap;		/* reverses maptwo values to original row numbers */

MAP(x)
{ int y = row[x].maptwo;	/* gives a value between 0 and redrows    */

	unmap[y] = x;
	return y;
}

minimize()
{ int i, j;
  int n = redrows;

	unmap = (int *)
		Emalloc( n * sizeof (int) );
	triangle = (char **)
		Emalloc( n * sizeof (char *) );
	/*
	** allocate a triangle of states
	** triangle[i][j] == 1
	** will indicate that the states i and j are
	** in the same equivalence class
	** initially, claim all pairs to be equivalent
	*/

	for (i = 1; i < n; i++)
	{	triangle[i] = (char *)
			Emalloc(i * sizeof (char));
		for (j = 0; j < i; j++)
			triangle[i][j] = 1;
	}
	tmpsucc = (short *)
		Emalloc( n * sizeof (short));
	successor = (short **)
		Emalloc( n * sizeof (short *) );

	for (i = 0; i < n; i++)
	{	successor[i] = (short *)
			Emalloc(n * sizeof (short));
		for (j = 0; j < n; j++)
			successor[i][j] = 0;
	}
	partition();

#if DEBUG6
	putriangle();
#endif
	squash();

	for (i = 1; i < n; i++)
		free(triangle[i]);
	free(triangle);
	free(unmap);
}

/*
newgeneration()
{ register int i,j;

	for (j = 0; j < n; i++)
		tmpsucc[j] = 0;
	for (i = 0; i < n; i++)
	{	for (j = 0; j < n; j++)
			addkids(successor[i][j]);
		for (j = 0; j < n; j++)
		{	successor[i][j] = tmpsucc[j];
			tmpsucc[i] = 0;
		}
	}
}

addkids(whosekids)
{ register int h,k,x,y;
  struct ENTRY *one = find(i, 0);
  struct ENTRY *two = find(j, 0);
  struct PILAR *a;


	for (k = 0; k < nrcols; k++)
	{	for (h = 0; h < one->nrpils; h++)
		{	a = one->pilar;

			tmpsucc[target(a->transf)] = 1;

			a = a->nxtp;
		}
		one = one->nextcol;
		two = two->nextcol;
	}
}
*/

partition()
{ register int i, j, K;
  int anychange = 1;

	/*
	** recursively scan through the
	** triangle to find non-equivalent pairs
	** until pi(1) -> pi(k) == pi(k-1)
	*/

	for (K = 1; anychange == 1; K++)
	{	anychange = 0;
#if DEBUG7
		printf("%d Equivalence:\n", K);
#endif DEBUG7
		for (i = nrrows-1; i > 0; i--)
		{	if (row[i].mapping != NOSTATE)
				continue;

			for (j = 0; j < i; j++)
			{	if (row[j].mapping != NOSTATE
				||  triangle[MAP(i)][MAP(j)] == 0)
					continue;

				if ( !equiv(K, i, j) )
				{	triangle[MAP(i)][MAP(j)] = 0;
					anychange = 1;
				}
#if DEBUG7
				else
				{	printf("\tstates: %2d,", MAP(i));
					printf("%2d\n", MAP(j));
				}
#endif DEBUG7
}	}	}	}

#if DEBUG6
putriangle()
{ int i, j;
  int n = redrows;
  int cn = 0;
  char * class = (char *) Emalloc (n * sizeof (char) );

	for (i = 0; i < n; i++)
		class[i] = 0;
	
	for (i = 1; i < n; i++)
	for (j = 0; j < i; j++)
		if ( triangle[i][j] )
		{	if (class[i] == 0 && class[j] == 0)
				class[i] = class[j] = ++cn;
			else if (class[i] != 0)
				class[j] = class[i];
			else if (class[j] != 0)
				class[i] = class[j];
			else
				whoops("cannot happen, classes");
		}

	for (i = 1; i <= cn; i++)
	{	printf("class %2d: ", i);
		for (j = 0; j < n; j++)
			if (class[j] == i)
				printf("%d,", j);
		printf("\n\n");
	}

	free(class);
}
#endif

squash()
{ int i, j;
  int n = redrows;
	for (i = 1; i < n; i++)
	for (j = 0; j < i; j++)
		if ( triangle[i][j] )
		{	row[unmap[i]].mapping = unmap[j];
			row[unmap[j]].labeled |= row[unmap[i]].labeled;
		}
}

outequiv(A, B)
	struct ENTRY *A, *B;
{ struct PILAR *at = A->pilar;
  struct PILAR *it = B->pilar;

	/*
	** don't look at the at->transf's, 
	** equal columns implicitly give
	** equal inputs or outputs, so
	** just compare the cargo codes
	*/

	while (it != NULL && at != NULL)
	{	if (at->code != it->code)
			break;

		at = at->nxtp;
		it = it->nxtp;
	}

	return (it == NULL && at == NULL);
}

equiv(K, i, j)
{ int h, k, x, y, z;
  struct ENTRY *one = find(i, 0);
  struct ENTRY *two = find(j, 0);
  struct PILAR *a, *b;

	if (one->nrpils != two->nrpils)
		return 0;

	if (K == 1)
	{	for (k = 0; k < nrcols; k++)
		{	if ( !outequiv(one, two) )
				return 0;

			one = one->nextcol;
			two = two->nextcol;
		}
	} else
		for (k = 0; k < nrcols; k++)
		{	a = one->pilar;
			b = two->pilar;
			for (h = 0; h < one->nrpils; h++)
			{
				x = target(a->transf);
				y = target(b->transf);
				x = MAP(x); y = MAP(y);
				if (y > x) { z=x; x=y; y=z; }
				if (x != y && triangle[x][y] == 0)
					return 0;

				a = a->nxtp;
				b = b->nxtp;
			}
			one = one->nextcol;
			two = two->nextcol;
		}

	return 1;
}
