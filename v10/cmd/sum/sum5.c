#define		BSIZE		512		/* system 5 */

int
sum5(register long sum, char *buf, int n)
{
	register unsigned char *s, *send;

	if(buf == 0){
		sum = ((sum>>16)+sum&0xFFFF)&0xFFFF;
		printf("%05u%6ld", sum, (n + (BSIZE - 1)) / BSIZE);
		return(0);
	}
	for(s = (unsigned char *)buf, send = s+n; s < send; s++)
		sum += 0xffff & *s;
	return(sum);
}
