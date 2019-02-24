/*% 3cc -c %
 * Fixed-point 3d coordinate transform routines for machines with
 * 16 bit shorts and 32 bit longs.
 */
#include	<CC/jerq.h>
#include	<CC/3d.h>
#include	<CC/font.h>
static Hcoordl cur;
static Point cen;
Point Ap, Bp;
static int wid;
/*
 * angles are scaled so that PI=>32768
 * Since unsigned shorts are 16 bits, 2*PI overflows and yields
 * 0, which seems appropriate.
 */
angle cvtangle[361]={
0,	182,	364,	546,	728,	910,	1092,	1274,
1456,	1638,	1820,	2002,	2184,	2366,	2548,	2730,
2912,	3094,	3276,	3458,	3640,	3822,	4004,	4187,
4369,	4551,	4733,	4915,	5097,	5279,	5461,	5643,
5825,	6007,	6189,	6371,	6553,	6735,	6917,	7099,
7281,	7463,	7645,	7827,	8009,	8192,	8374,	8556,
8738,	8920,	9102,	9284,	9466,	9648,	9830,	10012,
10194,	10376,	10558,	10740,	10922,	11104,	11286,	11468,
11650,	11832,	12014,	12196,	12379,	12561,	12743,	12925,
13107,	13289,	13471,	13653,	13835,	14017,	14199,	14381,
14563,	14745,	14927,	15109,	15291,	15473,	15655,	15837,
16019,	16201,	16384,	16566,	16748,	16930,	17112,	17294,
17476,	17658,	17840,	18022,	18204,	18386,	18568,	18750,
18932,	19114,	19296,	19478,	19660,	19842,	20024,	20206,
20388,	20571,	20753,	20935,	21117,	21299,	21481,	21663,
21845,	22027,	22209,	22391,	22573,	22755,	22937,	23119,
23301,	23483,	23665,	23847,	24029,	24211,	24393,	24576,
24758,	24940,	25122,	25304,	25486,	25668,	25850,	26032,
26214,	26396,	26578,	26760,	26942,	27124,	27306,	27488,
27670,	27852,	28034,	28216,	28398,	28580,	28763,	28945,
29127,	29309,	29491,	29673,	29855,	30037,	30219,	30401,
30583,	30765,	30947,	31129,	31311,	31493,	31675,	31857,
32039,	32221,	32403,	32585,	32768,	32950,	33132,	33314,
33496,	33678,	33860,	34042,	34224,	34406,	34588,	34770,
34952,	35134,	35316,	35498,	35680,	35862,	36044,	36226,
36408,	36590,	36772,	36955,	37137,	37319,	37501,	37683,
37865,	38047,	38229,	38411,	38593,	38775,	38957,	39139,
39321,	39503,	39685,	39867,	40049,	40231,	40413,	40595,
40777,	40960,	41142,	41324,	41506,	41688,	41870,	42052,
42234,	42416,	42598,	42780,	42962,	43144,	43326,	43508,
43690,	43872,	44054,	44236,	44418,	44600,	44782,	44964,
45147,	45329,	45511,	45693,	45875,	46057,	46239,	46421,
46603,	46785,	46967,	47149,	47331,	47513,	47695,	47877,
48059,	48241,	48423,	48605,	48787,	48969,	49152,	49334,
49516,	49698,	49880,	50062,	50244,	50426,	50608,	50790,
50972,	51154,	51336,	51518,	51700,	51882,	52064,	52246,
52428,	52610,	52792,	52974,	53156,	53339,	53521,	53703,
53885,	54067,	54249,	54431,	54613,	54795,	54977,	55159,
55341,	55523,	55705,	55887,	56069,	56251,	56433,	56615,
56797,	56979,	57161,	57344,	57526,	57708,	57890,	58072,
58254,	58436,	58618,	58800,	58982,	59164,	59346,	59528,
59710,	59892,	60074,	60256,	60438,	60620,	60802,	60984,
61166,	61348,	61531,	61713,	61895,	62077,	62259,	62441,
62623,	62805,	62987,	63169,	63351,	63533,	63715,	63897,
64079,	64261,	64443,	64625,	64807,	64989,	65171,	65353,
65536,	
};
static fract sintab[128+2]={
0,	201,	402,	603,	803,	1004,	1205,	1405,
1605,	1805,	2005,	2204,	2404,	2602,	2801,	2998,
3196,	3393,	3589,	3785,	3980,	4175,	4369,	4563,
4756,	4948,	5139,	5329,	5519,	5708,	5896,	6083,
6269,	6455,	6639,	6822,	7005,	7186,	7366,	7545,
7723,	7900,	8075,	8249,	8423,	8594,	8765,	8934,
9102,	9268,	9434,	9597,	9759,	9920,	10079,	10237,
10393,	10548,	10701,	10853,	11002,	11150,	11297,	11442,
11585,	11726,	11866,	12003,	12139,	12273,	12406,	12536,
12665,	12791,	12916,	13038,	13159,	13278,	13395,	13510,
13622,	13733,	13842,	13948,	14053,	14155,	14255,	14353,
14449,	14543,	14634,	14723,	14810,	14895,	14978,	15058,
15136,	15212,	15286,	15357,	15426,	15492,	15557,	15618,
15678,	15735,	15790,	15842,	15892,	15940,	15985,	16028,
16069,	16107,	16142,	16175,	16206,	16234,	16260,	16284,
16305,	16323,	16339,	16353,	16364,	16372,	16379,	16382,
16384,	16382,	
};
fract isin(x)
register angle x;
{
	register fract a, b;
	register i, f=0;
	if(x>=PI){
		x-=PI;
		f=1;
	}
	if(x>PI/2)
		x=PI-x;
	i=x>>7;
	a=sintab[i];
	b=sintab[i+1]-a;
	i=x&0177;
	a+=(b*i+64)/128;
	return(f?-a:a);
}
fract icos(x)
register angle x;
{
	return(isin(x+PI/2));
}
long isqrt(a)
register long a;
{
	register long x, y;
	if(a<=0)return(0);
	for(y=a,x=1;y!=0;y>>=2,x<<=1);
	while((y=(a/x+x)>>1)<x)x=y;
	return(x);
}
#define	NSTACK	32
Hcoord hcoord(x, y, z, w)
fract x, y, z, w;
{
	Hcoord r;
	r.x=x;
	r.y=y;
	r.z=z;
	r.w=w;
	return(r);
}
matrix stack3[NSTACK]={
	ONE,0,0,0,
	0,ONE,0,0,
	0,0,ONE,0,
	0,0,0,ONE,
};
matrix *sp3=stack3;		/* matrix stack pointer */
/*
 * Initialize the matrix stack.
 * v is the viewport into which to map the image.  (d/s) is the distance to the
 * screen, whose half-width is 1. (equivalently, d is the distance to a screen of
 * half-size s.)
 */
Bitmap *viewport;
init3(v, d, s)
Bitmap *v;
{
	sp3=stack3;
	ident(*sp3);
	(*sp3)[0][0]=(*sp3)[2][2]=d;
	(*sp3)[1][1]=s;
	(*sp3)[3][3]=s;
	viewport=v;
	cen=div(add(v->rect.origin, v->rect.corner), 2);
	wid=min(v->rect.corner.x-v->rect.origin.x,
		v->rect.corner.y-v->rect.origin.y)/2;
}
/*
 * Push a copy of the top of the matrix stack onto the stack
 */
push3(){
	register i, j;
	;if(sp3==stack3+NSTACK-1)
		return(-1);
	for(i=0;i!=4;i++) for(j=0;j!=4;j++)
		sp3[1][i][j]=sp3[0][i][j];
	sp3++;
	return(0);
}
/*
 * pop the top matrix off the stack
 */
pop3(){
	if(sp3==stack3)
		return(-1);
	--sp3;
	return(0);
}
/*
 * Multiply the top matrix on the stack by rotation of angle theta about
 * the given axis.
 */
rot3(theta, axis)
angle theta;
{
	rotsc3(isin(theta), icos(theta), axis);
}
/*
 * rotate, given the sin and cosine of the rotation angle.
 */
rotsc3(s, c, axis)
short s, c;
{
	matrix m;
	register i=(axis+1)%3, j=(axis+2)%3;
	ident(m);
	m[i][i] = c;
	m[i][j] = -s;
	m[j][i] = s;
	m[j][j] = c;
	xform3(m);
}
/*
 * Scale by a factor of [xyz]/w about axis [xyz]
 */
scale3(p)
Hcoord p;
{
	matrix m;
	ident(m);
	m[0][0]=p.x;
	m[1][1]=p.y;
	m[2][2]=p.z;
	m[3][3]=p.w;
	xform3(m);
}
/*
 * translate by (dx/dw, dy/dw, dz/dw)
 */
tran3(p)
Hcoord p;
{
	matrix m;
	ident(m);
	m[0][0]=p.w;
	m[1][1]=p.w;
	m[2][2]=p.w;
	m[3][3]=p.w;
	m[0][3]=p.x;
	m[1][3]=p.y;
	m[2][3]=p.z;
	xform3(m);
}
/*
 * Store an identity matrix in m
 */
ident(m)
matrix m;
{
	register fract *s = &m[0][0];
	*s++=ONE;*s++=0;*s++=0;*s++=0;
	*s++=0;*s++=ONE;*s++=0;*s++=0;
	*s++=0;*s++=0;*s++=ONE;*s++=0;
	*s++=0;*s++=0;*s++=0;*s++=ONE;
}
/*
 * Multiply the top of the matrix stack by m
 */
xform3(m)
matrix m;
{
	register i, j, k;
	long sum, max=0;
	long tmp[4][4];
	char buf[128];

	for(i=0;i!=4;i++) for(j=0;j!=4;j++){
		sum=0;
		for(k=0;k!=4;k++)
			sum+=(long)(*sp3)[i][k]*m[k][j];
		tmp[i][j]=sum;
		if(sum<0)
			sum = -sum;
		if(sum>max)
			max=sum;
	}
	if((k = max/(ONE-1)) == 0) k = 1;
	for(i=0;i!=4;i++) for(j=0;j!=4;j++)
		(*sp3)[i][j]=tmp[i][j]/k;
}
long dot(a, b)
Hcoord a, b;
{
	return((long)a.x*b.x+(long)a.y*b.y+(long)a.z*b.z);
}
Hcoord unitize(a)
Hcoord a;
{
	short l=isqrt(dot(a, a));
	Hcoord v;
	if(l==0){
		v=a;
		v.w=0;
	}
	else{
		if(a.w<0) l = -l;
		v.x=muldiv(a.x, ONE, l);
		v.y=muldiv(a.y, ONE, l);
		v.z=muldiv(a.z, ONE, l);
		v.w=ONE;
	}
	return(v);
}
Hcoord cross(a, b)
Hcoord a, b;
{
	Hcoord r;
	r.x=((long)a.y*b.z-(long)a.z*b.y)/ONE;
	r.y=((long)a.z*b.x-(long)a.x*b.z)/ONE;
	r.z=((long)a.x*b.y-(long)a.y*b.x)/ONE;
	r.w=ONE;
	return(r);
}
/*
 * multiply the top of the maxrix stack by a view-pointing transformation
 * with the eyepoint at e, looking in direction (lx, ly, lz),
 * with up direction (ux, uy, uz)
 */
look3(e, l, u)
Hcoord e, l, u;
{
	matrix m;
	fract len;
	Hcoord r;

	/*printf("look3(e=(%d,%d,%d,%d), l=(%d,%d,%d,%d), u=(%d,%d,%d,%d))\n",
		e.x,e.y,e.z,e.w,l.x,l.y,l.z,l.w,u.x,u.y,u.z,u.w);*/
	l=unitize(l);
	if(l.w==0){
		l.y=ONE;
		l.w=ONE;
	}
	/* Adjust u to be perpendicular to l */
	u=unitize(u);
	len=dot(u, l)/ONE;
	u.x-=muldiv(len, l.x, ONE);
	u.y-=muldiv(len, l.y, ONE);
	u.z-=muldiv(len, l.z, ONE);
	u=unitize(u);
	if(u.w==0){
		u.z=ONE;
		u.w=ONE;
	}
	r=cross(l, u);
	/*printf("r=(%d,%d,%d,%d), l=(%d,%d,%d,%d), u=(%d,%d,%d,%d))\n",
		r.x,r.y,r.z,r.w,l.x,l.y,l.z,l.w,u.x,u.y,u.z,u.w);*/
	/* make the matrix to transform from (rlu) space to (xyz) space */
	ident(m);
	m[0][0]=r.x; m[0][1]=r.y; m[0][2]=r.z;
	m[1][0]=l.x; m[1][1]=l.y; m[1][2]=l.z;
	m[2][0]=u.x; m[2][1]=u.y; m[2][2]=u.z;
	xform3(m);
	tran3(hcoord(e.x, e.y, e.z, -e.w));
}
Hcoordl xformp(p)
Hcoord p;
{
	Hcoordl r;
	r.x=(long)(*sp3)[0][0]*p.x+(long)(*sp3)[0][1]*p.y
		+(long)(*sp3)[0][2]*p.z+(long)(*sp3)[0][3]*p.w;
	r.y=(long)(*sp3)[1][0]*p.x+(long)(*sp3)[1][1]*p.y
		+(long)(*sp3)[1][2]*p.z+(long)(*sp3)[1][3]*p.w;
	r.z=(long)(*sp3)[2][0]*p.x+(long)(*sp3)[2][1]*p.y
		+(long)(*sp3)[2][2]*p.z+(long)(*sp3)[2][3]*p.w;
	r.w=(long)(*sp3)[3][0]*p.x+(long)(*sp3)[3][1]*p.y
		+(long)(*sp3)[3][2]*p.z+(long)(*sp3)[3][3]*p.w;
	return(r);
}
move3(p)
Hcoord p;
{
	cur=xformp(p);
	/*printf("move3(%d,%d,%d,%d)\n", p.x, p.y, p.z, p.w);*/
}
line3(p)
Hcoord p;
{
	Hcoordl new;
	/*printf("line3(%d,%d,%d,%d)\n", p.x, p.y, p.z, p.w);*/
	new=xformp(p);
	clipanddraw(cur, new);
	cur=new;
}
/*
 * clip points to the viewing volume, then do perspective division
 * and mapping to screen coordinates and draw lines.  We only clip against the
 * eye plane, trusting segment to clip in x and z.  There probably should
 * be some overflow precaution taken in the multiplication by n/d.
 */
int clip3scale = 0;

static clipanddraw(a, b)
Hcoordl a, b;
{
	Point A, B;
	Hcoordl t;
	int clip=0;
	long n, d;
	long m;
	long as, bs;
#define		OV	(32767)		/* 2**(15.5) */
	/*
	 * we clip against the plane p=(0,1,0,-1)  [this corresponds
	 * to the euclidean plane y=1.  This is picked rather than y=0
	 * to avoid a divide check in the perspective divide]
	 * First we check to see if one endpoint is on each side of the plane.
	 * and if necessary switch them so that a is behind and b is in front.
	 * then we replace a by a+(b-a)*n/d, where p.(a+(b-a)*n/d)=0
	 * i.e.	a.y+(b.y-a.y)*n/d-a.w-(b.w-a.w)*n/d=0
	 *	(b.y-a.y-b.w+a.w)*n/d=a.w-a.y
	 *	n/d=(a.w-a.y)/(b.y-a.y-b.w+a.w)
	 */
	/*printf("clipanddraw(a=(%d,%d,%d,%d), b=(%d,%d,%d,%d)\n",
		a.x,a.y,a.z,a.w,b.x,b.y,b.z,b.w);*/
	if(a.w>0?a.y<a.w:a.y>a.w)	/* eqv. a.y/a.w<1, but avoids divide */
		clip++;
	if(b.w>0?b.y<b.w:b.y>b.w){
		if(clip)		/* both points behind y=1 plane */
		{
			/*printf("clipped out of existence\n");*/
			return;
		}
		t=a;
		a=b;
		b=t;
		clip++;
	}
	/* check for overflow */
	m = max(max(abs(a.x), abs(a.y)), max(abs(a.z), abs(b.x)));
	m = max(m, max(max(abs(b.y), abs(b.z)), max(abs(a.w), abs(b.w))));
	if(m > OV)
	{
		m = (m+OV-1)/OV;
		a.x /= m;
		a.y /= m;
		a.z /= m;
		a.w /= m;
		b.x /= m;
		b.y /= m;
		b.z /= m;
		b.w /= m;
	}
	if(clip){
		n=a.w-a.y;
		d=b.y-a.y-b.w+a.w;
		if(d==0)			/* never happens? */
			return;
		a.x+=(b.x-a.x)*n/d;
		a.y+=(b.y-a.y)*n/d;
		a.z+=(b.z-a.z)*n/d;
		a.w+=(b.w-a.w)*n/d;	/* don't use w below */
		/*printf("clipped to a=(%d,%d,%d,%d), b=(%d,%d,%d,%d)\n",
			a.x,a.y,a.z,a.w,b.x,b.y,b.z,b.w);*/
	}
	if(clip3scale)
	{
		as = a.w*clip3scale;
		bs = b.w*clip3scale;
	}
	else
	{
		as = a.y;
		bs = b.y;
	}
	A=add(Pt((short)muldiv(a.x, wid, as), (short)muldiv(-a.z, wid, as)), cen);
	B=add(Pt((short)muldiv(b.x, wid, bs), (short)muldiv(-b.z, wid, bs)), cen);
#define	safe(P)	if(P.x<-16000)P.x= -16000;if(P.x>16000)P.x=16000;if(P.y<-16000)P.y= -16000;if(P.y>16000)P.y=16000
	safe(A);
	safe(B);
	Ap = A;
	Bp = B;
	segment(viewport, A, B, F_OR);
	/*printf("---->drew points (%d,%d) - (%d,%d)\n", A.x, A.y, B.x, B.y);*/
}
