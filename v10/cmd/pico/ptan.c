static qatan2(), Qatan2();

patan2(xx, yy)
{
	register x, y;
	x=abs(xx);
	y=abs(yy);
	if(xx>=0 && yy>=0)
		return(qatan2(x, y));
	if(xx<0 && yy<=0)
		return(180+qatan2(x, y));
	if(xx<0 && yy>0)
		return(180-qatan2(x, y));
	return(360-qatan2(x, y));
}

static
qatan2(x, y)
	register x, y;
{
	if(x<y)
		return(90-(45*((long)x)/y));
	if(y==0)
		return(0);
	return(45*((long)y)/x);
}

Patan2(xx, yy)
{	return 100*patan2(xx,yy);
/*	register x, y;
	x=abs(xx);
	y=abs(yy);
	if(xx>=0 && yy>=0)
		return(Qatan2(x, y));
	if(xx<0 && yy<=0)
		return(18000+Qatan2(x, y));
	if(xx<0 && yy>0)
		return(18000-Qatan2(x, y));
	return(36000-Qatan2(x, y));
*/
}
/*
static
Qatan2(x, y)
	register x, y;
{
	if(x<y)
		return(9000-(45*((long)(x*100))/y));
	if(y==0)
		return(0);
	return(45*((long)(y*100))/x);
}
*/
