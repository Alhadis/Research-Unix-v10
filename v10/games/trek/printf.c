#include "/usr/lib/a68defs"

int	mkfault;
int	stdout 1;
char printbuf[255]; char *printptr printbuf, *digitptr;

long itol(x,y)
{
	return((((long)x)<<16)|y);
}


printc(c)
char c;
{

	IF (*printptr++ = c)==NL ORF c==0
	THEN IF mkfault==0 THEN write(stdout,printbuf,printptr-printbuf); FI
	     printptr=printbuf;
	FI
}

charpos()
{	return(printptr-printbuf);
}

flushout()
{	IF printptr!=printbuf
	THEN printc(0);
	FI
}

printf(fmat,a1)
char *fmat, **a1;
{
	char *fptr, *s; int *vptr; long *dptr; double *rptr;
	double real, rnd;
	int x, decpt, n; long lx;
	int width, prec; char c, adj; char flush;
	char digits[64];

	fptr=fmat; vptr = &a1;

	WHILE c = *fptr++
	DO  IF c!='%'
	    THEN printc(c);
	    ELSE IF *fptr=='-' THEN adj='l'; fptr++; ELSE adj='r'; FI
		 width=convert(&fptr);
		 IF *fptr=='.' THEN fptr++; prec=convert(&fptr); ELSE prec = -1; FI
		 digitptr=digits;
		 dptr=rptr=vptr; lx = *dptr; x = *vptr++;
		 s=0; flush=0;
		 switch (c = *fptr++) {

		    case 'n':
			flush++; break;
		    case 'd':
		    case 'u':
			printnum(x,c,10); break;
		    case 'o':
			printoct(0,x,0); break;
		    case 'q':
			lx=x; printoct(lx,-1); break;
		    case 'x':
			printdbl(0,x,c,16); break;
		    case 'Y':
			printdate(lx); vptr++; break;
		    case 'D':
		    case 'U':
			printdbl(lx,c,10); vptr++; break;
		    case 'O':
			printoct(lx,0); vptr++; break;
		    case 'Q':
			printoct(lx,-1); vptr++; break;
		    case 'X':
			printdbl(lx,'x',16); vptr++; break;
		    case 'c':
			printc(x); break;
		    case 's':
			s=x; break;
		    case 'f':
		    case 'F':
			vptr =+ 3;
			rnd=1.0;
			for(n=prec;n>=0;n--)
				rnd =* 10;
			real = *rptr + (5 / rnd);
			s=ecvt(real, 8, &decpt, &n);
			IF n THEN *digitptr++='-'; FI
			*digitptr++ = (decpt<=0 ? '0' : *s++);
			WHILE *s ANDF --decpt>0 DO *digitptr++ = *s++; OD
			*digitptr++ = '.';
			WHILE *s ANDF prec-- DO *digitptr++ = (++decpt<0?'0':*s++); OD
			*digitptr++=0;
			s=0; prec = -1; break;
		    case 'm':
			vptr--; break;
		    case 'M':
			width=x; break;
		    case 'T':
		    case 't':
			IF c=='T'
			THEN width=x;
			ELSE vptr--;
			FI
			IF width
			THEN width =- charpos()%width;
			FI
			break;
		    default:
			printc(c); vptr--;
		}

		IF s==0
		THEN *digitptr=0; s=digits;
		FI
		n=length(s);
		n=(prec<n ANDF prec>=0 ? prec : n);
		width =- n;
		IF adj=='r'
		THEN WHILE width-- > 0
		     DO printc(SP); OD
		FI
		WHILE n-- DO printc(*s++); OD
		WHILE width-- > 0 DO printc(SP); OD
		digitptr=digits;
		IF flush THEN flushout(); FI
	    FI
	OD
}

printdate(tvec)
long tvec;
{
	STRING timeptr; REG INT i;
	timeptr = ctime(&tvec);
	FOR i=20; i<24; i++ DO *digitptr++ = *(timeptr+i); OD
	FOR i=3; i<19; i++ DO *digitptr++ = *(timeptr+i); OD
} /*printdate*/

prints(s)
char *s;
{	printf("%s",s);
}

convert(cp)
char **cp;
{	char c; int n;
	n=0;
	WHILE ((c = *(*cp)++)>='0') ANDF (c<='9') DO n=n*10+c-'0'; OD
	(*cp)--;
	return(n);
}

printnum(n,fmat,base)
{	char k; int digs[15]; int *dptr;
	long	N;
	dptr=digs;
	IF n<0 ANDF fmat=='d' THEN n = -n; *digitptr++ = '-'; FI
	N= (unsigned)n;
	WHILE N
	DO  *dptr++ = N%base;
	    N /= base;
	OD
	IF dptr==digs THEN *dptr++=0; FI
	WHILE dptr!=digs
	DO  k = *--dptr;
	    *digitptr++ = (k+(k<=9 ? '0' : 'a'-10));
	OD
}

printoct(o,s) long o; int s;
{
	int i;
	long po;
	char digs[12];
	po = o;
	IF s
	THEN IF po<0
	     THEN po = -po; *digitptr++='-';
	     ELSE IF s>0 THEN *digitptr++='+'; FI
	     FI
	FI
	FOR i=0;i<=11;i++
	DO digs[i] = po&7; po =>> 3; OD
	digs[10] =& 03; digs[11]=0;
	FOR i=11;i>=0;i--
	DO IF digs[i] THEN EXITFOR; FI OD
	FOR i++;i>=0;i--
	DO *digitptr++=digs[i]+'0'; OD
}

printdbl(lx,ly,fmat,base)
unsigned lx, ly; char fmat; int base;
{	int digs[20]; int *dptr; char k;
	double f ,g; long q;
	dptr=digs;
	IF fmat!='D'
	THEN	f=(lx); f =* itol(1,0); f =+ (ly);
		IF fmat=='x' THEN *digitptr++='#'; FI
	ELSE	f=itol(lx,ly);
		IF f<0 THEN *digitptr++='-'; f = -f; FI
	FI
	WHILE f
	DO  q=f/base; g=q;
	    *dptr++ = f-g*base;
	    f=q;
	OD
	IF dptr==digs THEN *dptr++=0; FI
	WHILE dptr!=digs
	DO  k = *--dptr;
	    *digitptr++ = (k+(k<=9 ? '0' : 'a'-10));
	OD
}

length(s)
char *s;
{
	REG CHAR *t;

	t=s;
	WHILE *t++ DONE
	return(t-s-1);
}
