/* These are unguaranteed math routines lifted from NORGEN "K" code */

static float alogtab[] = {
	-.00645354,
	.03608849,
	-.09532938,
	.16765407,
	-.24073380,
	.33179902,
	-.49987412,
	.999964239,
};

extern long _fsign2;
float exp(exparg)
float exparg;
{
	float ffr(); 
	long fex();
	float expd,expz;
	float two();
	long expc;
	expz = exparg*1.44269504;
	/* 1/LN(2) */
	expc=expz;
	expd=expc-expz;
	if(expz>=0.0){
		expd = expd+1.0;
		expc = expc+1;
	}
	expz=expd*expd;
	return(two(expc)*(1.-(2.*expd)/(expd+.034657359*(expz)+9.9545948-
	    617.97227/(expz+87.417497))));
}
float atan(atanx)
float atanx;
{
	if(atanx<0.0)	return(-atan(-atanx));
	if(atanx>1.0)	return(1.57079632-atan(1./atanx));
	if(atanx>2.67949192e-1)	
		return(5.23598775e-1+atan((7.32050808e-1*atanx-1.+atanx)/
			    (1.732050808+atanx)));
	return(atanx*(.60310579-.05160454*atanx*atanx+.55913709/(atanx*atanx+
			    1.4087812)));
}
float atan2(atan2x1,atan2x2)
float atan2x1,atan2x2;
{
	float atan2z;
	if(atan2x1<0.0)		return(-atan2(-atan2x1,atan2x2));
	if(!atan2x2)		return(1.5707963);
	atan2z = (atan2x1/atan2x2);
	if(atan2z<0.)		atan2z= -atan2z;
	if(atan2z>1.67777e7)	return(1.5707963);
	if(atan2x2>0.0)		return(atan(atan2z));
	return(3.14159265-atan(atan2z));
}
float log(alogxx)
float alogxx;
{
	float ffr(); 
	long fex();
	float p();
	float alogx;
	float temp;
	alogx=ffr(alogxx)*2.-1.;
	/* "c" won't take this lovely expression! 
	return(6.9314718e-1*(fex(alogxx)-1.)+alogx*(.999964239+
		alogx*(-.49987412+alogx*(.33179902+alogx*(-.24073380+alogx*(.16765407+
		alogx*(-.09532938+alogx*(.03608849+alogx*(-.00645354)))))))));
	*/

	return(6.9314718e-1*(fex(alogxx)-1)+p(alogx,alogtab,8));
}
float sin(sinx)
float sinx;
{
	float psin(),pcos();
	long sinq,sinqz;
	float sinr,sinz;
	sinz=1.2732395*(sinx>=0.0?sinx:-sinx);
	sinq=sinz;
	sinr=sinz-sinq;
	if(sinx<0.0)sinq = sinq+4;
	sinqz=sinq&07;
	if(!sinqz)	return(psin(sinr));
	if(sinqz==1)	return(pcos(1.0-sinr));
	if(sinqz==2)	return(pcos(sinr));
	if(sinqz==3)	return(psin(1.0-sinr));
	if(sinqz==4)	return(-psin(sinr));
	if(sinqz==5)	return(-pcos(1.0-sinr));
	if(sinqz==6)	return(-pcos(sinr));
	return(-psin(1.0-sinr));
}
float cos(cosx)
float cosx;
{
	float sin();
	return(sin(cosx+1.5707963));
}
float psin(psinss)
float psinss;
{
	float psinst,psinst2;
	psinst2=.78539816*psinss;
	psinst=psinst2*psinst2;
	return(psinst2*(1.-psinst*(1.66666667e-1-
	    psinst*(8.33333333e-3-psinst*(1.98412698e-4-psinst*
	    (2.75573192e-6))))));
}
float pcos(pcoscs)
float pcoscs;
{
	float pcosct;
	pcosct=.616850275*pcoscs*pcoscs;
	return(1.-pcosct*(.5-pcosct*(4.16666667e-2-pcosct*
	    (1.38888888e-3-pcosct*(2.48015873e-5-pcosct*(2.75573192e-7))))));
}
float sqrt(sqrtarg)
float sqrtarg;
{
	float ffr(); 
	long fex();
	float sqrtiarg;
	float two();
	sqrtiarg = two((long)(fex(sqrtarg )*.5-1.))*(1.+ffr(sqrtarg));
	sqrtiarg = .5*(sqrtiarg+sqrtarg/sqrtiarg);
	sqrtiarg = .5*(sqrtiarg+sqrtarg/sqrtiarg);
	sqrtiarg = .5*(sqrtiarg+sqrtarg/sqrtiarg);
	return(.5*(sqrtiarg+sqrtarg/sqrtiarg));
}

static long fex(arg)
long arg;
{
	if(_fsign2==0100000L)	arg >>= 7; 
	else 			arg >>= 23;
	return((arg&0377)-0200);
}

static long ffr(arg)
long arg;
{
	if(arg==0)	return(0);
	if(_fsign2==0100000L) {
		arg = arg & (~077600L);
		return(arg|040000L);
	}
	arg = arg & (~(077600L<<16));
	return(arg|(040000L<<16));
}

static long two(arg)
long arg;
{
	if(_fsign2==0100000L)	return((arg+0201L)<<7);
	return((arg+0201L)<<23);
}
static float p(x,t,n)
float x,t[];
int n;
{
	int i;
	float ans;
	ans=t[0]; 
	for(i=1; i<n; i++)	ans=ans*x+t[i];
	return(x*ans);
}
