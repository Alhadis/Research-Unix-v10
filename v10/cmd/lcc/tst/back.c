float af[10];
char buffer[136], *cp, *limit, *s1, *s2, **ss;
int bsize, i, *inc(), *ip, j, k, l, m, n, r;
int a[10], b[10], xg[3][4], *yg[3];
unsigned u;
struct node {
	int op, type;
	struct node *kids[6];
} *alloc(), nodes[10], *p, *q, **pp;
char *strings[] = {"if", "for", "else", "while", 0 };

float f;
double d;
signed char c;
signed int i;

main()
{
	return 0;
}

features(xf, yf)
int xf[][4], *yf[];
{
	int xl[3][4], *yl[3];

	/* simple expressions */

	i = 0;
	i = i + 1;
	i++;
	++i;
	i += 2;
	a[i] = b[i] = 1;
	a[i] = a[i+j] = a[i-j+1] = 0;
	n = 10*n + c - '0';
	limit = &buffer[136 + 1 + bsize];
	cp = limit = &buffer[136+1];
	foo(0, i, a[i]+'0', &a[i]);

	/* control structures */

	for (i = 0; i < 15; i++);
	if (c == ' ' || c == '\t' || c == '\n');
	if (a[i] && a[i+j] && a[i-j+1]);
	if (i+1);
	if ((i = getchar()) != -1 && isletter(i) == 0);
	if (c = isletter(c));
	if (i=j);
	if (i && s1 || s1 && *s1);

	/* structs */
	
	nodes[i].kids[0] = nodes[i].kids[1];
	nodes[i].kids[0] = nodes[i].kids[1] = 0;
	p->kids[0] = p->kids[1];
	p->kids[i] = p->kids[i+1] = 0;
	p++;
	++p;
	p - q;
	if (p > q);
	alloc()->op++;
	foo((*pp)->type);
	foo(&(*pp)->type);

	/* character pointers */

	*s1;
	*s1 = 0;
	s1++;
	++s1;
	*s1++ = 0;
	*s1++ = c;
	if (*s1 == 0);
	if (*s1 == *s2);
	if (*s1++ == 0);
	s1 - s2;
	*s1 - *s2;
	if (s1 > s2);
	if ((*--s1 = *--limit) != '\n');
	if (*s1 && *s1 != '\n');

	/* pointer pointers */

	ss = strings;
	*ss;
	*ss = 0;
	**ss = 0;
	ss++;

	/* expressions with side effects */

	i = ++j;
	i = j++;
	*s1++ = *s2++ = 0;
	*--s1 = *--s1 = 0;
	(*inc())++;
	i = (*inc())++;
	af[i] += 1;
	foo(i = j, --n, *s1++);

	/* multiply-dimensioned arrays */

	xg[i][j] = 0; yg[i][j] = 0;
	xf[i][j] = 0; yf[i][j] = 0;
	xl[i][j] = 0; yl[i][j] = 0;

	yg[i] = xg[i]; yg[i] = &xg[i][0];
	yf[i] = xf[i]; yf[i] = &xf[i][0];
	yl[i] = xl[i]; yl[i] = &xl[i][0];

	/* conditionals */

	i = j<3;
	i = (i&&j) + (i||j);
	i = i ? j : k;
	i = i ? j : (k ? l : m);
	i = i ? (j ? k : l) : m;
	i = j ? 0 : foo();

	/* conversions */

	u = 0xffffffff;d = u;u = d;

	/* all operators */

	d+5;f+5;i+5;ip+5;u+5;
	d-5;f-5;i-5;ip-5;ip-ip;u-5;
	u&5;i&5;u|5;i|5;u^5;i^5;~u;~i;
	i<<5;u<<5;i>>5;u>>5;
	d*5;f*5;i*5;u*5;
	d/5;f/5;i/5;u/5;
	i%5;u%5;
	-d;-f;-i;-u;
	if (c==5);if (d==5);if (f==5);if (i==5);if (u==5);
	s1=(char*) u;

	/* difficult calls and returns */

	foo(0)+foo(1);
	foo(0)+foo(1)+foo(2);
L1:	return i ? i++ : --i;
L2:	return *ip = i++;
L3:	return malloc(strlen(s1)+1);
}

passold(c, d, f, i, p, u)
char c, *p;
double d;
float f;
int i;
unsigned u;
{
	passold(c, d, f, i, p, u);
}

passnew(char c, double d, float f, int i, char *p, unsigned u) {
	passnew(c, d, f, i, p, u);
}

typedef struct point { int x,y; } point;
point structvalue(point p) {
	point q;

	q = p;
	p.y = 0;
	q.y = 0;
	if ((p=structvalue(p)).y)
		return p;
	else
		return structvalue(p);
}

struct node *alloc(){}
foo(){}
int *inc(){}
isletter(){}
