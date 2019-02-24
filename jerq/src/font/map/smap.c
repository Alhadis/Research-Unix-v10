#ifndef	JERQFONT
#define	JERQFONT	"/usr/jerq/Font"
#endif	JERQFONT

char tab[] = "X!=***C*D*F*G*H*L*P*Q*S*W*a*b*c*d*e*f*g*h*i*k*l*m*n*o*p*q*r*s*t*u*w\
*x*y*z+-->..121434<-<===>=L.Sl\\-\\_aaalapb0b9brbsbubvbxcacdcicoctcudadddedi\
eqesfafefmgagrhcibifipisl.lblclflhlkltmamimomunooborpdplppptrbrcrfrgrhrkrnrtrusb\
slspsqsrtetmtptsuaulvr~=~~";

main()
{
	int fd;
	char *f;
	char buf[256];

	tab[0] =  strlen(tab+1)/2;
	sprintf(buf, "%s/SMAP", JERQFONT);
	if((fd = creat(f = buf, 0664)) == -1)
	{
		perror(f);
		exit(1);
	}
	write(fd, tab, sizeof tab);
}
