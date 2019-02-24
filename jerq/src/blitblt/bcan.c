#include <stdio.h>
#include <pwd.h>
#include "imPRESS.h"

#define PARGVAL	((*argv)[2] ? (*argv)+2 : --argc ? *++argv : (char *)0)

#define min(a,b)	((a<=b) ? a : b)

char rasters[32][100] = { 0 };

char filnam[40], command[64];

main(argc,argv)
int argc; char **argv;
{
	struct passwd *userentry, *getpwuid(); long bintim; int pflag = 1;
	char *username = NULL, *getlogin(), *tspool, *ctime();
	FILE *in = stdin, *out = (FILE *)0;

	while (--argc > 0) {
		if ((*++argv)[0] == '-') switch ((*argv)[1]) {
			case 'n':
				pflag = 0; break;
			case 's':
				out = stdout; break;
			case 'u':
				username=PARGVAL; break;
			default:
				fprintf(stderr,"bcan.out: unknown option %s\n",*argv);
				exit(1);
		} else {
			fprintf(stderr,"bcan.out: unknown argument %s\n",*argv);
			exit(1);
		}
	}

	if (username == NULL && (username=getlogin()) == NULL)
		if ((userentry=getpwuid(getuid())) == NULL)
			username="guess who!"; else username=userentry->pw_name;

	time(&bintim); tspool=ctime(&bintim); tspool[strlen(tspool)-1]=0;

	mktemp(strcpy(filnam,"/tmp/bcXXXXXX"));
	strncat(strcat(filnam,"."),username,5);
	if (out == (FILE *)0 && (out = fopen(filnam,"w")) == (FILE *)0) {
		fprintf(stderr,"bcan.out: cannot open %s\n",filnam);
		exit(1);
	}

	fputs("@document(name \"",out);
	fputs(username,out);
	fputs("\", spooldate \"",out);
	fputs(tspool,out);
	fputs("\", language imPRESS, jobheader on, messagedetail on)",out);

	if (glyphpage(in,out) < 0) {
		fprintf(stderr,"bcan.out: error in input file\n");
		if (out != stdout) {
			fclose(out);
			sprintf(command,"rm -f %s &",filnam);
			system(command);
		}
		exit(2);
	}

	putc(imP_EOF,out);
	if (out == stdout) exit(0);
	fclose(out);

	sprintf(command,"ipr -p 1 %s 0</dev/null 1>/dev/null 2>&1 &",filnam);
	if (pflag) system(command);
	else fprintf(stderr,"bcan.out: %s\n",command);
	exit(0);
}

glyphpage(in,out)
FILE *in, *out;
{
	int nrasters, rastwid, rastbyt, nghoriz, ngvert, krast, i, iprev, j;

	nrasters=getw(in);
	rastwid =getw(in);
	if (nrasters <= 0 || nrasters > 1024) return -1;
	if (rastwid  <= 0 || rastwid  >  800) return -1;
	ngvert  =(nrasters+31)/32;
	nghoriz =(rastwid + 1)/ 2;
	rastbyt = rastwid * 2;

	putc(imP_SET_ABS_H, out);
	putc(            0, out);
	putc(           32, out);
	putc(imP_SET_ABS_V, out);
	putc(            0, out);
	putc(           32, out);
	putc(imP_SET_MAGN , out);
	putc(            1, out);
	putc(imP_BITMAP   , out);
	putc(         0x0F, out);
	putc(      nghoriz, out);
	putc(      ngvert , out);

	for (; nrasters>0; nrasters -= krast) {
		krast=min(nrasters,32);
		for (iprev=31,i=0; i<krast; iprev=i++) {
			if (readrast(rasters[i],rastwid,in) < 0) return -1;
			for (j=0; j<rastbyt; j++)
				rasters[i][j] ^= rasters[iprev][j];
		}
		for (i=krast; i<32; i++) for (j=0; j<rastbyt; j++)
			rasters[i][j]=0;

		for (j=0; j<rastbyt; j+=4) for (i=0; i<32; i++)
			fwrite(&rasters[i][j],4,1,out);
	}
	putc(imP_ENDPAGE, out);
	return 0;
}

readrast(p1,nw,in)
char *p1; int nw; FILE *in;
{
	int count, ctype;

	while (nw>0) {
		if ((count=getc(in)) <= 0) return -1;
		ctype=count&128; count &= 127;
		nw -= count; count *= 2;

		if (ctype) {
			if (fread(p1,2,1,in) <= 0) return -1;
			for (count-=2; count>0; count--) {
				*(p1+2) = *p1;
				p1 += 1;
			}
			p1 += 2;
		} else {
			if (fread(p1,count,1,in) <= 0) return -1;
			p1 += count;
		}
	}
	return (nw == 0 ? 0 : -1);
}

getw(stream)
FILE *stream;
{
	register l = getc(stream);
	return (getc(stream)<<8) | l;
}
