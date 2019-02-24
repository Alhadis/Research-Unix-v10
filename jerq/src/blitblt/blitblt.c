#include <stdio.h>
#include <sys/filio.h>
#include "/usr/jerq/include/jioctl.h"

#define PARGVAL	((*argv)[2] ? (*argv)+2 : --argc ? *++argv : (char *)0)

#define putch(c)	(cbuf=c, write(1,&cbuf,1))

FILE *filep, *popen(); unsigned char buffer[256], cbuf;

char *jpgm = { "/usr/jerq/mbin/blitblt.m" }; int zflag = 0;

char *ppgm = { "bcan" }; int pflag = 0;

extern errno;

main(argc,argv)
int argc; char **argv;
{
	int i, rcvstat;
	while (--argc > 0) {
		if ((*++argv)[0] == '-') switch ((*argv)[1]) {
		case 'j':
			jpgm = PARGVAL; break;
		case 'z':
			zflag++; break;
		case 'p':
			ppgm = PARGVAL; break;
		default:
			fprintf(stderr,"unknown option %s\n",*argv);
			exit(1);
		} else {
			fprintf(stderr,"unknown argument %s\n",*argv);
			exit(1);
		}
	}

	if (jload(jpgm,zflag)) exit(1);
	rawtty(1);
	putch(0); putch(255);
	strncpy(buffer, ppgm, 30);
	buffer[30] = 0;
	write(1, buffer, strlen(buffer)+1);

	while ((rcvstat=recvbuf()) > 0) {
		if (buffer[1] != '|')
			filep=fopen(buffer+1, "w");
		else
			filep=popen(buffer+2, "w"), pflag = 1;
		if (filep)
			putch(0);
		else {
			putch(1); pflag = 0; continue;
		}
		while ((rcvstat=recvbuf()) > 0)
			fwrite(buffer+1, *buffer-1, 1, filep);
		if (!pflag) {
			fclose(filep); putch(0);
		} else {
			putch(pclose(filep)); pflag = 0;
		}
		if (rcvstat < 0) break;
	}
	rawtty(0);
	ioctl(0, JTERM, (char *)0);
	if (rcvstat < 0) fprintf(stderr,"error %d reading from jerq\n",errno);
	exit(0);
}

recvbuf()
{
	register nr, nb = 0;
	putch(0);
	do {
		if ((nr = read(0,buffer+nb,(sizeof buffer)-nb)) <= 0) return -1;
		nb += nr;
	} while (nb < *buffer);
	return *buffer;
}

jload(prog,zflag)
char *prog; int zflag;
{
	static char *cmd[] = { "32ld", (char *)0, (char *)0, (char *)0 };
	if (zflag) { cmd[1] = "-z"; cmd[2] = prog; }
	else { cmd[1] = prog; cmd[2] = (char *)0; }
	if (systemv("/usr/jerq/bin/32ld",cmd)) return 1;
	sleep(2);
	return 0;
}

systemv(name,argv)
char *name, **argv;
{
	int status, pid, l;
	if ((pid = fork()) == 0) { execv(name,argv); _exit(127); }
	while ((l = wait(&status)) != pid && l != -1);
	return (l == -1) ? -1 : status;
}

#include <sgtty.h>

rawtty(flag)
{
	static struct sgttyb ttyb, ttybsave;
	static struct ttydevb ttyd, ttydsave;
	static int tty_raw = 0, was_ld;
	extern int tty_ld;

	if (flag == tty_raw) return;

	if (flag) {
		ioctl(0, TIOCGETP, &ttyb);
		ioctl(0, TIOCGDEV, &ttyd);
		ttybsave = ttyb;
		ttydsave = ttyd;
		ttyb.sg_flags |= RAW;
		ttyd.flags |= F8BIT;
		ioctl(0, TIOCSDEV, &ttyd);
		ioctl(0, TIOCSETP, &ttyb);
		if ((was_ld = ioctl(0, FIOLOOKLD, 0)) == tty_ld)
			ioctl(0, FIOPOPLD, 0);
	} else {
		if (was_ld == tty_ld)
			ioctl(0, FIOPUSHLD, &tty_ld);
		ioctl(0, TIOCSDEV, &ttydsave);
		ioctl(0, TIOCSETP, &ttybsave);
	}
	tty_raw = flag;
}
