#include	<stdio.h>
#include	<signal.h>
#include	<sgtty.h>
#include	"/usr/jerq/include/jioctl.h"
#include	"view2d.h"

#define		PIXMAX		32766

struct sgttyb sttybuf, sttysave;
char *zflag = "";
int shade = 0;
int grid = 1;
typedef struct Point
{
	int x, y;
} Point;

short z[4096];
double ztime;
double fmid, fscale;

int zmax = -1000000000, zmin = 1000000000;
int nsleep, ntime;
int nframes = 0;
int period = 5;
int floor = -BIG;
char *progname;
short timewarp;
double ts, te;
Rd2d rd;

main(argc, argv)
	char **argv;
{
	char buf[512];
	int zc, scale, m, i;
	short ragnorok, dim;
	short *nz, *p, *q;
	int fd;
	int cv, tmin, tmax;
	short nx, ny, fru, frv;
	char *s;
	char **av = argv;
	int ac = argc;
	extern char *getenv();
	int magic[4];

	timewarp = 0;
	progname = argv[0];
	cv = ((s = getenv("TERM")) == 0) || strcmp(s, "5620");
	cv = 1;  /* I can't stand -T5 as default!!!  ehg */
	ioctl(0, TIOCGETP, &sttysave);
	strcpy(buf, "level");
	for(argc--, argv++; *argv && (**argv == '-'); argv++)
		switch(argv[0][1])
		{
		case 'z':
			zflag = "-z";
			break;
		case 's':
			shade++;
			break;
		case 'g':
			grid = 0;
			break;
		case 'T':
			if(argv[0][2] == 'c') cv = 1;
			else if(argv[0][2] == '5') cv = 0;
			else if(argv[0][2] == 'p') cv = 2;
			else if(argv[0][2] == 'h') cv = 3;
			else if(argv[0][2] == 'd') cv = 4;
			else if(argv[0][2] == 's') cv = 5;
			else goto err;
			break;
		case 't':
			i = sscanf(&argv[0][2], "%hd, %hd", &ts, &te);
			if(i==0) error("bad TS,TE");
			timewarp = i;
			break;
		case 'p':
			sscanf(&argv[0][2], "%d", &period);
			break;
		case 'i':
			if(cv==0) sscanf(&argv[0][2], "%d", &floor);
			break;
		case 'B':
		case 'D':
		case 'S':
		case 'b':
		case 'm':
		case 'w':
			/* these only apply in color, so assume -Tc */
			cv = 1;
			break;
		case 'c':
			if(cv==0) cv = 1; /* maybe -Tp, but probably -Tc */
		case 'v':
		case 'x':
		case 'y':
			break;
		default:
		err:
			fprintf(stderr, "Usage: view2d [-Tdev] [-ptime] [-cn] [-ms]\n");
			exit(1);
		}
	if(*argv){
		if((fd = open(*argv, 0)) == -1)
			quit("cannot open input", 1);
	}else{
		fd = 0;  /* if no file argument, try standard input */
	}
	if(cv==1)
	{
		if(access((s="/usr/lib/view2d/level"), 1) == -1)
		{
			fprintf(stderr, "go to swift for color\n");
			exit(1);
		}
		dup2(fd,0);     /* close(0); dup(fd); close(fd); */
		strcpy(*av, "level");
		execvp(s, av);
		perror(s);
		exit(1);
	} else if(cv==2) {
		if(access((s="/usr/lib/view2d/contour"), 1) == -1)
		{
			fprintf(stderr, "can't find %s\n",s);
			exit(1);
		}
		dup2(fd,0);     /* close(0); dup(fd); close(fd); */
		strcpy(*av, "cntour");
			/* WARNING: strlen("cntour") must <= strlen("view2d") */
		execvp(s, av);
		perror(s);
		exit(1);
	} else if(cv==3)
	{
		if(access((s="/usr/lib/view2d/halft"), 1) == -1)
		{
			fprintf(stderr, "can't find %s\n",s);
			exit(1);
		}
		dup2(fd,0);     /* close(0); dup(fd); close(fd); */
		strcpy(*av, "halft");
		execvp(s, av);
		perror(s);
		exit(1);
	} else if(cv==4)
	{
		if(access((s="/usr/lib/view2d/viewtd"), 1) == -1)
		{
			fprintf(stderr, "can't find %s\n",s);
			exit(1);
		}
		dup2(fd,0);     /* close(0); dup(fd); close(fd); */
		strcpy(*av, "viewtd");
		execvp(s, av);
		perror(s);
		exit(1);
	} else if(cv==5)
	{
		if(access((s="/usr/lib/view2d/slice"), 1) == -1)
		{
			fprintf(stderr, "can't find %s\n",s);
			exit(1);
		}
		dup2(fd,0);     /* close(0); dup(fd); close(fd); */
		strcpy(*av, "slice");
		execvp(s, av);
		perror(s);
		exit(1);
	}
	rd2dh(fd, &nx, &ny);
	if(nx*ny*sizeof(short) > sizeof z)
	{
		sprintf(buf, "nx*ny=%d: must be less than 4096\n", nx*ny);
		quit(buf, 1);
	}
	sttybuf = sttysave;
	sttybuf.sg_flags |= RAW;
	sttybuf.sg_flags &= ~ECHO;
	ioctl(0, TIOCSETP, &sttybuf);
	sprintf(buf, "/usr/jerq/bin/32ld %s /usr/lib/view2d/view2d.m", zflag);/**/
	if(system(buf)) quit(0, 1);
	putchar('X'); sendn(nx);
	putchar('Y'); sendn(ny);
	if(grid) putchar('G');
	if(shade) putchar('S');
	putchar('P');
	sendn(period);
	putchar('n');
	sendn(rd.nfr);
	putchar('I');
	sendn(floor);
	m = 2*nx*ny;
	fmid = (rd.pmin+rd.pmax)/2.0;
	fscale = PIXMAX/(rd.pmax - fmid);
	while(rd2di(&ztime, z))
	{
		char buf[56];

		putchar('F');
		sprintf(buf, "%g", ztime);
		sends(buf);
		for(q = nx*ny + (p = z); q != p; p++)
			sendn(*p < floor ? floor : (short)((*p-fmid)*fscale));
	}

	putchar('Q');
	fflush(stdout);
	(void)getchar();
	quit(0, 0);
}

quit(s, e)
	char *s;
{
	ioctl(0, JTERM, 0);
	ioctl(0, TIOCSETP, &sttysave);
	if(s)
		printf("view2d: %s\n", s);
	exit(e);
}

sendn(n)
{
	putchar(n>>8);
	putchar(n);
}

sends(s)
	char *s;
{
	while(*s) putchar(*s++);
	putchar(0);
}

abs(n)
{
	return(n<0? -n:n);
}
