#ifndef	JERQFONT
#define	JERQFONT	"/usr/jerq/Font"
#endif	JERQFONT

char tab[] = "Xdgemenfiflhysc";

main()
{
	int fd;
	char *f;
	char buf[256];

	tab[0] =  strlen(tab+1)/2;
	sprintf(buf, "%s/IMAP", JERQFONT);
	if((fd = creat(f = buf, 0664)) == -1)
	{
		perror(f);
		exit(1);
	}
	write(fd, tab, sizeof tab);
}
