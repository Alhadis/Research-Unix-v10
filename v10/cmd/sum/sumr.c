#define		BSIZE		1024		/* research! */

int
sumr(register long sum, char *buf, int n)
{
	register unsigned char *s, *send;

	if(buf == 0){
		sum &= 0xFFFF;
		printf("%05u%6ld", sum, (n + (BSIZE - 1)) / BSIZE);
		return(0);
	}
	for(s = (unsigned char *)buf, send = s+n; s < send; s++)
		if (sum & 1)
			sum = 0xffff & ((sum>>1) + *s + 0x8000);
		else
			sum = 0xffff & ((sum>>1) + *s);
	return(sum);
}
