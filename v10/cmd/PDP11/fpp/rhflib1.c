/* floating point utility operations for software-float */

extern float _fsign; /* 0100000L for norgen, 020000000000L for unix */
extern long _fsign2; /* same constant, different type */
extern float _one; /* the constant 1.0 */
long _funmin(f)
long f;
{
if(f)return(f^_fsign2); else return(0L);
}

float _funplus(f)
float f;
{
return(f);
}

float _finc1(pf)
float *pf;
{
return(*pf= *pf+_one);
}

float _fdec1(pf)
float *pf;
{
return(*pf= *pf-_one);
}

float _finc2(pf)
float *pf;
{
float temp;
temp= *pf;
*pf= *pf+_one;
return(temp);
}

float _fdec2(pf)
float *pf;
{
float temp;
temp= *pf;
*pf = *pf-_one;
return(temp);
}

float _fmod(b,a)
float b,a;
{
return(a-((long)(a/b))*b);
}

float _feqpl(b,pa)
float b,*pa;
{
return(*pa= *pa+b);
}

float _feqmi(b,pa)
float b,*pa;
{
return(*pa= *pa-b);
}

float _feqmu(b,pa)
float b,*pa;
{
return(*pa= *pa*b);
}

float _feqdv(b,pa)
float b,*pa;
{
return(*pa= *pa/b);
}

float _feqan(b,pa)
float b,*pa;
{
return(*pa= *pa&b);
}

float _feqer(b,pa)
float b,*pa;
{
return(*pa= *pa^b);
}

float _feqmo(b,pa)
float b,*pa;
{
return(*pa= *pa%b);
}

float _feqor(b,pa)
float b,*pa;
{
return(*pa= *pa|b);
}

float _feqsr(b,pa)
float b,*pa;
{
return(*pa= *pa>>b);
}

float _feqsl(b,pa)
float b,*pa;
{
return(*pa= *pa<<b);
}

int _fgt(b,a)
float b,a;
{
float temp;
if(temp=a-b)return((temp&_fsign)==0.0);
return(0);
}

int _fge(b,a)
float b,a;
{
return(((a-b)&_fsign)==0.0);
}

int _fle(b,a)
float b,a;
{
float temp;
if(temp=a-b)return((temp&_fsign)==_fsign);
return(1);
}
int _flt(b,a)
float b,a;
{
return(((a-b)&_fsign)==_fsign);
}
