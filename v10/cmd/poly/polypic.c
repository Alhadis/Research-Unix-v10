#include	<stdio.h>
#define	READFN
#include	<poly.h>

extern debug;
Polyhedron net;
char names[MAXNETS][128];
int ns;
int prh = 0;
int prf = 0;

main(argc, argv)
	char **argv;
{
	char *input = 0;
	int i;
	int plt = 1;

	for(argc--, argv++; argc-- && (**argv == '-'); argv++)
		switch(argv[0][1])
		{
		case 'p':
			plt = 0;
			break;
		case 'h':
			prh++;
			break;
		case 'f':
			prf++;
			break;
		case 'i':
			input = &argv[0][2];
			break;
		}

	if((ns = poly_finit(input)) == 0)
		exit(1);
	for(i = 0; i < ns; i++)
	{
		poly_read(&net, i);
		strcpy(&names[i][0], net.name);
	}

	while(*argv)
	{
		i = lkpoly(*argv++);
		if(i < 0) continue;
		poly_read(&net, i);
		if(plt)
			plot(&net, i);
		else
			pic(&net, i);
	}

	exit(0);
}

lkpoly(s)
	char *s;
{
	register i;
	register char *ss;
	int n = strlen(s);

	for(i = 0; i < ns; i++)
		if(strncmp(&names[i][0], s, n) == 0) return(i);
	for(ss = s; *ss; ss++)
		if((*ss < '0') || (*ss > '9')) break;
	if(*ss == 0)
	{
		n = atoi(s);
		if((n >= 0) || (n < ns))
			return(n);
		fprintf(stderr, "number %d out of range\n", n);
		return(-1);
	}
	fprintf(stderr, "no solid '%s'\n", s);
	return(-1);
}

plot(s, num)
	register Polyhedron *s;
{
	register Point *p, *lp;
	register i, n;
	float xmin, xmax, ymin, ymax, c, r, x, y;

	ymin = xmin = 999999;
	xmax = ymax = -xmin;
	for(i = 0; i < s->nfaces; i++)
	{
		p = s->pts + s->faces[i].f;
		lp = &p[s->faces[i].n];
		while(p < lp)
		{
			if(p->x > xmax) xmax = p->x;
			if(p->x < xmin) xmin = p->x;
			if(p->y > ymax) ymax = p->y;
			if(p->y < ymin) ymin = p->y;
			p++;
		}
	}
	r = ymax-ymin;
	if(r < (xmax-xmin)) r = xmax-xmin;
	r /= 2;
	c = (xmax+xmin)/2; xmin = c-r; xmax = c+r;
	c = (ymax+ymin)/2; ymin = c-r; ymax = c+r;
	printf("open\n");
	printf("range %g %g %g %g\n", xmin, ymin, xmax, ymax);
	printf("move %g %g\ntext %d: %s\n", xmin, ymax, num, s->name);
	for(i = 0; i < s->nfaces; i++)
	{
		if(s->faces[i].n < 3) continue;
		p = s->pts + s->faces[i].f;
		lp = &p[s->faces[i].n];
		n = 0;
		x = y = 0;
		printf("poly { { %g %g", lp[-1].x, lp[-1].y);
		while(p < lp)
		{
			printf(" %g %g", p->x, p->y);
			n++, x += p->x, y += p->y;
			p++;
		}
		printf(" } }\n");
		if(prf)
			printf("move %g %g\ntext \\C%d\n", x/n, y/n, i);
	}
	printf("close\n");
}

pic(s, num)
	register Polyhedron *s;
{
	register Point *p, *lp;
	register i, n;
	float xmin, xmax, ymin, ymax, x, y;

	ymin = xmin = 999999;
	xmax = ymax = -xmin;
	for(i = 0; i < s->nfaces; i++)
	{
		p = s->pts + s->faces[i].f;
		lp = &p[s->faces[i].n];
		while(p < lp)
		{
			if(p->x > xmax) xmax = p->x;
			if(p->x < xmin) xmin = p->x;
			if(p->y > ymax) ymax = p->y;
			if(p->y < ymin) ymin = p->y;
			p++;
		}
	}
	printf(".PS\nscale=%d\n", 3);
	printf("\"%d: %s\" at %g,%g center\n", num, s->name, (xmin+xmax)/2, ymax+.2);
	for(i = 0; i < s->nfaces; i++)
	{
		if(s->faces[i].n < 3) continue;
		p = s->pts + s->faces[i].f;
		lp = &p[s->faces[i].n];
		n = 0;
		x = y = 0;
		printf("line from %g,%g", lp[-1].x, lp[-1].y);
		while(p < lp)
		{
			printf(" to %g,%g", p->x, p->y);
			n++, x += p->x, y += p->y;
			p++;
		}
		printf("\n");
		x/=n, y/=n;
		if(prf)
			printf("\"%d\" at %g,%g center\n", i, x, y);
	}
	if(prh) for(i = 0; i < s->nhinges; i++)
	{
		int fc = s->hinges[i].fa;

		p = s->pts + s->faces[fc].f + s->hinges[i].ea;
		lp = (s->hinges[i].ea == s->faces[fc].n-1)?
			s->pts + s->faces[fc].f : p+1;
		x = (p->x+lp->x)/2;
		y = (p->y+lp->y)/2;
		printf("\"\\f3\\s-3%d\\fP\\s+3\" at %g,%g center\n", i, x, y);
	}
	printf(".PE\n");
}
