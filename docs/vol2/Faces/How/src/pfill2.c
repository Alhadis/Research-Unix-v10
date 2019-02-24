#include <stdio.h>

#define SZ	512

typedef struct Node {
	int x, y;
	struct Node *nxt;	/* contour */
	int soc;		/* node is start of new curve */
} Node;

Node *frst = (Node *) 0;
int lcnt=0;
extern int Factor;

zaplist()
{	Node *ptr = frst;
	Node *save;

	while (ptr)
	{	save = ptr->nxt;
		free(ptr);
		ptr = save;
	}
	frst = (Node *) 0;
}

addnode(n, m, p)
{	Node *tmp, *hunt, *last;

	tmp = (Node *) malloc(sizeof(Node));
	if (!tmp)
		return;

	tmp->nxt = (Node *) 0;
	tmp->x = n;
	tmp->y = m;
	tmp->soc = p;
	if (!frst)
		frst = tmp;
	else
	{	last = frst; hunt = last->nxt;
		while (hunt)
		{	last = hunt;
			hunt = last->nxt;
		}
		last->nxt = tmp;
	}
}

compar(a, b)
	int *a, *b;
{
	return (*a>*b)?1:(*a<*b)?-1:0;
}

runcontour()
{	Node *hunt;
	int miny, maxy;
	int *xx;
	int x, y, cnt, total, partial;

	if (!frst)
		return;

	hunt = frst;
	miny = hunt->y; maxy = hunt->y;
	cnt = 0;
	while (hunt)
	{	if (hunt->y < miny) miny = hunt->y;
		if (hunt->y > maxy) maxy = hunt->y;
		hunt = hunt->nxt;
		cnt++;
	}
	xx = (int *) malloc(cnt * sizeof(int));	/* max # intersections */
	if (!xx)
	{	fprintf (stderr, "out of memory\n");
		return;
	}
	for (y = miny; y < maxy; y++)
	{	if ((cnt = intersect(xx, y)) <= 1)
			continue;
		qsort(xx, cnt, sizeof(int), compar);
		for (x = total = 0; x < cnt-1; x += 2)
			total += xx[x+1] - xx[x];
		for (x = partial = 0; x < cnt-1; x += 2)
		{	doline(xx[x], xx[x+1]+1, SZ-y, partial, total);
			partial += xx[x+1] - xx[x];
	}	}
	free(xx);
}

doline(a, b, c, p, t)
{	int q;

	if (a >= b) return;
	lcnt++;
	if (lcnt%100 == 0)
		printf("}\ndef curve%d() {\n", lcnt);
	if (b-a == t)
		printf("mapper(%d,%d,0,%d,%d)\n", a, b, SZ-1, c);
	else
	{	if (p == 0)
		{	printf("setter(0, %d, %d, %d)\n", a, c, c*255/(SZ-1));
			printf("mapper(%d,%d,0,(%d*%d)/%d,%d)\n",a,b,SZ-1,b-a,t,c);
		} else
		{	printf("x=(%d*%d)/%d;\n", SZ-1, p, t);
			printf("y=x+(%d*%d)/%d;\n", SZ-1, (b-a), t);
			printf("mapper(%d,%d,x,y,%d)\n", a, b, c);
			if (p+b-a == t)
			printf("setter(%d, %d, %d, %d)\n",b,SZ-1,c,c*255/(SZ-1));
		}
	}
}

#define Sign(n)	((n>0)?1:0)	

intersect(xx, y)
	int *xx;
{
	Node *hunt, *last;
	int i, j, n, m;
	int nhit=0;

	last = frst;
	hunt = last->nxt;
	while (hunt)
	{	n = last->y - y;
		m = hunt->y - y;
		if (Sign(n) != Sign(m))
		{	i = last->x; j = hunt->x;
			if (hunt->x > last->x)
				xx[nhit++] = i + (j-i)*n/(n-m);
			else
				xx[nhit++] = i - (i-j)*n/(n-m);
		} /* else if (n == 0 && m == 0)
		{	if (hunt->x > last->x)
				doline(last->x, hunt->x, SZ-y);
			else
				doline(hunt->x, last->x, SZ-y);
		} */
		last = hunt;
		hunt = last->nxt;
		if (hunt && hunt->soc)
		{	last = hunt;
			hunt = last->nxt;
		}
	}
	return nhit;
}
