struct defs {
     int cbits;          
     int ibits;          
     int sbits;          
     int lbits;          
     int ubits;          
     int fbits;          
     int dbits;          
     float fprec;        
     float dprec;        
     int flgs;           
     int flgm;           
     int flgd;           
     int flgl;           
     int rrc;            
     int crc;            
     char rfs[8];        
   };
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
struct record{
	int field1,field2,field3;
	};
	struct list { int f1; double f2; float f3; } ;
	union rec { int recf1; float recf2; double recf3; } ;
struct list rfun(x)  struct list x;
{
x.f1=x.f1+100;
x.f2=x.f2+200.0;
x.f3=x.f3+300.0;
return x;
}
union rec ufun(x) union rec x;
{
x.recf2=x.recf2+100.0;
return x;
}

s141(pd0)	
	struct defs *pd0;
{
static char s141er[]="s141,er%d\n";
static char qs141[8]="s141   ";
struct list r1,r2,r3;
union rec rec1, rec2;
char *ps = qs141,
     *pt = pd0->rfs;
int rc = 0 ;

 while ( *(pt)++ = *(ps)++ );

r1.f1=1000; r1.f2=2000.0; r1.f3=3000.0;
r3=r2=r1;

if (1000!=r2.f1 || 2000.0!=r2.f2 || 3000.0!=r2.f3
   || r1.f1!=r3.f1 || r1.f2!=r3.f2 || r1.f3!=r3.f3)
	{ rc++; if (pd0->flgd) printf(s141er,1); }
r2.f3=r2.f2=r2.f1=1;
r3=rfun(r2=r1);

if (r3.f1!=r1.f1+100 || r1.f1!=r2.f1 || r1.f1!=1000 ||
    r3.f2!=r1.f2+200.0 || r1.f2!=r2.f2 || r1.f2!=2000.0 ||
    r3.f3!=r1.f3+300.0 || r1.f3!=r2.f3 || r1.f3!=3000.0 )
	{ rc+=2; if (pd0->flgd) printf(s141er,2); }
if (r3.f3!=rfun(r1).f3)
	{ rc+=4; if (pd0->flgd) printf(s141er,4); }
rec1.recf2=33.0;
rec2=ufun(rec1);
if (ufun(rec1).recf2!=133.0 || rec1.recf2!=33.0)
	{ rc +=8 ; if (pd0->flgd) printf(s141er,8); }
return rc;
}
