#include	<stdio.h>
#define	READFN
#include	<poly.h>

#define		RAD	(180/3.1415926535897932384626433832795028841971693993751)

Polyhedron net;
char names[MAXNETS][128];
int ns;
int fflag;
int sflag;
int nflag;
int cflag;
int iflag;
int hflag;
int pflag;
int full;
double minx, minz, maxx, maxz, miny, maxy;

main(argc, argv)
	char **argv;
{
	register i, k;

	for(argc--, argv++; argc-- && (**argv == '-'); argv++)
		switch(argv[0][1])
		{
		case 's':	sflag++; break;
		case 'f':	fflag++; break;
		case 'n':	nflag++; break;
		case 'c':	cflag++; sflag++; break;
		case 'i':	iflag++; break;
		case 'h':	hflag++; break;
		case 'p':	pflag++; break;
		default:	printf("usage: polypr [-fcsiehn] [n]\n"); exit(1);
		}
	full = !(fflag || nflag || sflag || hflag);

	if((ns = poly_finit(iflag? "/dev/stdin" : (char *)0)) == 0)
		exit(1);

	for(i = 0; i < ns; i++)
	{
		poly_read(&net, i);
		strcpy(&names[i][0], net.name);
	}

	if(*argv)
	{
		while(*argv)
		{
			k = lkpoly(*argv++);
			if(k < 0) continue;
			poly_read(&net, k);
			dumppoly(&net, k);
		}
	}
	else
		for(i = 0; i < ns; i++)
		{
			poly_read(&net, i);
			dumppoly(&net, i);
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

dumppoly(p, num)
	register Polyhedron *p;
{
	register i, j;
	register Point *pp;

	if(nflag)
	{
		printf("%3d: %s\n", num, p->name);
		return;
	}
	if(cflag)
		cpr(p);
	if(full || hflag)
		printf("[%d] %s: %d faces, %d hinges\n", num, p->name,
			p->nfaces, p->nhinges);
	if(full || fflag)
	{
		if(full)
			printf("Flat:\n");
		for(i = 0; i < p->nfaces; i++)
		{
			printf("data=%d %d vert:", p->faces[i].data, p->faces[i].n);
			for(j = 0, pp = p->pts+p->faces[i].f; j < p->faces[i].n; j++, pp++)
				printf(" (%.4g,%.4g,%.4g)", pp->x, pp->y, pp->z);
			printf("\n");
		}
	}
	if(full || sflag)
	{
		if(full)
			printf("Solid:\n");
		for(i = 0; i < p->nfaces; i++)
		{
			if(cflag)
				printf("p -128", p->faces[i].data);
			else
				printf("data=%d %d vert:", p->faces[i].data, p->faces[i].n);
			for(j = 0, pp = p->pts+p->faces[i].s; j < p->faces[i].n; j++, pp++)
				printf(cflag?" %.8g %.8g %.8g":" (%.4g,%.4g,%.4g)",
					pp->x, pp->y, pp->z);
			if(cflag) printf(";");
			printf("\n");
		}
	}
	if(full)
		for(i = 0; i < p->nhinges; i++)
			printf("%d:: %d,%d --- %d,%d  theta=%g (%.1f)\n", i,
			p->hinges[i].fa, p->hinges[i].ea,
			p->hinges[i].fb, p->hinges[i].eb,
			p->hinges[i].dihedral, p->hinges[i].dihedral*RAD);

}

cpr(p)
	register Polyhedron *p;
{
	double cx, cz, eyey, fov, d;
	register i;
	register Point *pp;

	{
		minx = 1e30, maxx = -1e30, minz = 1e30;
		maxz = -1e30, miny = 1e30, maxy = -1e30;
		for(i = p->npts/2, pp = &p->pts[i]; i < p->npts; pp++, i++)
		{
			if(minx > pp->x) minx = pp->x;
			if(maxx < pp->x) maxx = pp->x;
			if(minz > pp->z) minz = pp->z;
			if(maxz < pp->z) maxz = pp->z;
			if(miny > pp->y) miny = pp->y;
			if(maxy < pp->y) maxy = pp->y;
		}
	}
	cx = (minx+maxx)/2;
	cz = (minz+maxz)/2;
	fov = pflag? 30:1;
	d = maxx - minx;
	if((maxz-minz) > d) d = maxz-minz;
	eyey = miny - (pflag? 2:100)*d;
	miny = miny-eyey-.1;
	maxy = maxy-eyey+.1;
	printf("v %g %g %g %g %g %g 0 1 0 0 0 1\n", fov, miny, maxy, cx, eyey, cz);
	printf("l .4 1 .5\n");
	printf("c 128 30 30 255 255\n");
}
