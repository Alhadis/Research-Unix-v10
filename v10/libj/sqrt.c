#define MAXROOT 0xb504
sqrt(x)
	register long x;
{
	register long high=MAXROOT;
	register long low=0;
	register long current=MAXROOT/2;
	if(x<=0)
		return 0;
	if(x>=MAXROOT*MAXROOT)
		return(MAXROOT);
	while(high>low+1){
		if(current*current==x)
			return (current);
		if(current*current>x)
			high=current;
		else
			low=current;
		current=(high+low)>>1;
	}
	return(current);
}
