#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define ICONSIZE 48
#define ICONBYTES (ICONSIZE*(ICONSIZE/8))
#define ICONWORDS (ICONSIZE*(ICONSIZE/16))
#define ICONPATH "/n/face/48x48x1"
#define	FACEPATH "/n/face"
#define UNKNOWNMACH "misc."
#define UNKNOWNUSER "unknown"
#define BLANKFACE "/usr/jerq/icon/face48/blank.face"
#define RESOLUTION "/48x48x1"
char iconfile[100];
char *strchr(), *strrchr();

main(argc, argv)
	char *argv[];
{
	register i;
	struct sgttyb modes;
	if(argc==1){
		fprintf(stderr, "usage: face moniker ...\n");
		return 1;
	}
	if(system("32ld /usr/jerq/mbin/catalog.m")){
		fprintf(stderr, "face: 32ld errors\n");
		return 1;
	}
	ioctl(1, TIOCGETP, &modes);
	modes.sg_flags|=RAW;
	ioctl(1, TIOCSETP, &modes);
	for(i=1; i<argc; i++){
		char *p=strrchr(argv[i], '/');
		if(p)
			p++;	/* skip / */
		else
			p=argv[i];
		if(p[0]!='.' && translate(argv[i])){
			write(1, p, strlen(p)+1);
			sendicon();
		}
	}
	getchar();
	modes.sg_flags&=~RAW;
	ioctl(1, TIOCSETP, &modes);
}
char *
iconpath(m, u)
	char *m, *u;
{
	sprintf(iconfile, "%s/%s/%s/48x48x1", FACEPATH, m, u);
	return iconfile;
}
checkdir()
{
	struct stat statb;
	int again=0;
   	for(;;){
		if(stat(iconfile, &statb)!=0)
			return 0;
		if((statb.st_mode&S_IFDIR)==0)
			return 1;
		if(again++)
			return 0;
		strcat(iconfile, RESOLUTION);
	}
}
translate(s)
{
	register char *p;
	if(p=strchr(s, '!')){
		*p++=0;
		if(access(iconpath(s, p), 0)==0 ||
		   access(iconpath(UNKNOWNMACH, p), 0)==0 ||
		   access(iconpath(s, UNKNOWNUSER), 0)==0 ||
		   access(iconpath(UNKNOWNMACH, UNKNOWNUSER), 0)==0 ||
		   (strcpy(iconfile, BLANKFACE), access(BLANKFACE, 0))==0){
			*--p='!';
			return 1;
		}
		*--p='!';
		return 0;
	}
	strcpy(iconfile, s);
	if(checkdir())
		return 1;
	if(strchr(s, '/')==0){
		sprintf(iconfile, "%s/%s", ICONPATH, s);
		if(checkdir())
			return 1;
	}
	return access(iconpath(UNKNOWNMACH, UNKNOWNUSER), 0)==0 ||
	   	(strcpy(iconfile, BLANKFACE), access(BLANKFACE, 0))==0;
}
HexToDec(Hex)
int Hex;
{
	if((Hex>='0') && (Hex<='9'))
		return(Hex-'0');
	if((Hex>='A') && (Hex<='F'))
		return(10+Hex-'A');
	if((Hex>='a') && (Hex<='f'))
		return(10+Hex-'a');
	return(-1);
}
sendicon()
{
	FILE *icon;
	char IconBuf[ICONBYTES];
	int i,val;

	if ((icon = fopen(iconfile, "r")) != NULL){
		for (i=0; i<ICONBYTES; ) {
			do; while(getc(icon)!='x' && !feof(icon));
			val = HexToDec(getc(icon))<<4; val |= HexToDec(getc(icon));
			IconBuf[i++]=val;
			val = HexToDec(getc(icon))<<4; val |= HexToDec(getc(icon));
			IconBuf[i++]=val;
		}
		fclose(icon);
	}
	write(1, IconBuf, ICONBYTES);
}
