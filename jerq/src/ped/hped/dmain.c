#include "hped.h"
#include "Unix.h"

#include "/usr/jerq/include/jioctl.h"
#include <signal.h>

#ifdef UNIX5
#include <termio.h>
struct termio ottymode, nttymode;
#else
#include <sgtty.h>
struct sgttyb ottymode, nttymode;
#endif

extern short modif;
extern short no_scale;
extern char * getenv();

extern char * ctime();
extern long time();

short public = 1;

done(){

#ifdef UNIX5
	ioctl(fileno(fromblit),TCSETAW,&ottymode);	/* popmode in jx.c */
#else
	ioctl(fileno(fromblit),TIOCSETP,&ottymode);	/* popmode in jx.c */
#endif
	ioctl(fileno(fromblit),JTERM,0);
	if(modif) wr_objs("ped.save");
	printf("Leaving ped.");
	printf(" See `.pederr' for `stderr' info.");
	if(!public) printf(" See REC. ");
	if(modif) printf(" Work has been saved in ped.save\n");
	else printf("\n");
	exit(0);
}

FILE *toblit, *fromblit;
FILE *fd1;
FILE *frecord;

char flocus[64];
char tlocus[64];
char xlocus[128];

char fntplace[2][64] = { FNT_5620, FNT_blit };
char tfntplace[2][64] = { TFNT_5620, TFNT_blit };

char loader[2][64] = { LOAD_32X, LOAD_68X };

char module[2][64] = { PED_5620M, PED_blitM };
char zmodule[2][64] = { ZED_5620M, ZED_blitM };

char docdir[64] = DOC_LIB;

char name[32];
char command[256];
char options[512] = {"public"};

char fname[64];

main(arc,arv)
	char ** arv;
{
	int havearg;
	int havefont = 1;
	short unreal = 0;
	register char *p;
	register char *pop, *poq;
	register term_type = 0;
	long timess;

	strcpy(name,arv[0]);

	p = getenv("TERM");
	if(!p) term_type = 0;
	else {
		if( !strcmp(p,"5620")|| !strcmp(p,"DMD")|| !strcmp(p,"dmd") )
		term_type = 0; else term_type = 1;
	}

	strcpy(flocus,fntplace[term_type]);
	strcpy(tlocus,tfntplace[term_type]);

	p = getenv("PEDOPTS");
	if(p) strcpy(options,p);

	/*	look at options		*/
	pop = options;
	do {
		poq = pindex(pop,',');
		if(poq) *poq = 0;
		if( !(strncmp("priv",pop,4)) ) public = 0;
		pop = poq+1;
	} while(poq);

	if(public) frecord = fopen("/dev/null","w");
	else {
		if( (frecord = fopen("REC","a")) ==NULL) frecord = stderr;
		setbuf(frecord,NULL);
		timess = time( (long *) 0 );
		fprintf(frecord,
		"\n- - - - - - - - NEW RUN OF PED on %s",
		(char *)ctime(&timess));
		strcpy(docdir,PRIV_LIB);
		strcpy(module[term_type],PRIV_PED);
	}

	while(arc>1 && arv[1][0]=='-'){
		switch(arv[1][1])	{
		case 'F':
			strcpy(tlocus,"");
		case 'f':
			if(!arv[1][2] || arv[1][2]=='0'){
				havefont = 0;
				break;
			}
			strcat(tlocus,arv[1]+2);
			strcpy(xlocus,tlocus);
			strcat(xlocus,".10");
			if(!(fd1=fopen(xlocus,"r"))) {
				rrecord("Cannot find");
				rrecord(xlocus);
			}
			else {
				rrecord("found");
				rrecord(xlocus);
				fclose(fd1);
				strcpy(flocus,tlocus);
			}
			break;
		case 's':
			no_scale = 1 - no_scale;
			break;
		case 't':
			if(name[0]=='z') strcpy(zmodule[term_type],arv[1]+2);
			else strcpy(module[term_type],arv[1]+2);
			break;
		case 'u':
			unreal = 1;
			break;
		default:
			rrecord("Unknown flag");
			break;
		}
		arc--; arv++;
	}

	if(name[0]=='z')
	sprintf(command,"%s %s",loader[term_type],zmodule[term_type]);
	else
	sprintf(command,"%s %s",loader[term_type],module[term_type]);
	if(unreal) {
		printf("PEDOPTS: %s\n",options);
		printf("command: %s\n",command);
		printf("fonts: %s\n",flocus);
		exit(1);
	}
	else rrecord(command);

	freopen(".pederr","w",stderr);	/*stderr goes into file ERR"*/
	setbuf(stderr,NULL);
	fromblit=fopen("/dev/tty","r");
	toblit = fopen("/dev/tty","w");

#ifdef UNIX5
	ioctl(fileno(fromblit),TCGETA,&ottymode); /*gtty: pushmode in jx.c*/
	nttymode = ottymode;
	nttymode.c_iflag &= ~ (ISTRIP | IXOFF | IXON);
	nttymode.c_iflag &= (IGNBRK | IGNPAR);
	nttymode.c_cflag |= CS8;
	nttymode.c_cflag &= ~ PARENB;
	nttymode.c_cc[VMIN] = 1;
	nttymode.c_cc[VTIME] = 0;
	nttymode.c_lflag &= ~ (ECHO | ISIG | ICANON);
	nttymode.c_oflag &= ~ (TAB3 | ONLCR | OCRNL );
	ioctl(fileno(fromblit),TCSETAW,&nttymode);	/*stty*/
	/* signal(SIGINT,SIG_IGN); */
	signal(SIGHUP,done);
	/* signal(SIGQUIT,SIG_IGN); */
#else
	ioctl(fileno(fromblit),TIOCGETP,&ottymode); /*gtty: pushmode in jx.c*/

	nttymode = ottymode;
	nttymode.sg_flags &= ~ (ECHO | CRMOD | XTABS);
	nttymode.sg_flags |= RAW;
	ioctl(fileno(fromblit),TIOCSETP,&nttymode);	/*stty*/
	signal(SIGINT,done);
	signal(SIGHUP,done);
	signal(SIGQUIT,done);
#endif

	/* next: really fork and execl ...: loads slave to blit*/

	if( system(command) != 0) {
#ifdef UNIX5
		ioctl(fileno(fromblit),TCSETAW,&ottymode);
#else
		ioctl(fileno(fromblit),TIOCSETP,&ottymode);
#endif
		ioctl(fileno(fromblit),JTERM,0);
		fprintf(stderr,"ped failed to load the blit.\n");
		printf("See `.pederr'\n");
		exit(1);
	}

	if(arc>1) {
		strcpy(fname,arv[1]);
		havearg = 1;
	}
	else havearg = 0;
	editor(havearg,havefont);	/*get into loop to converse with Blit*/
	done();
}
