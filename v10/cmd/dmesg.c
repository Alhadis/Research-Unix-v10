/*
 *	Suck up system messages
 *	dmesg
 *		print current buffer
 *	dmesg -
 *		print and update incremental history
 */

#include <stdio.h>
#include <libc.h>
#include <sys/param.h>
#include <nlist.h>
#include <signal.h>
#include <sys/msgbuf.h>

long	msgptr;
struct	msgbuf msgbuf;
char	*msgbufp;
int	sflg;
int	wflg;

struct	msgbuf omesg;
struct	nlist nl[2] = {
	{ "_msgbuf" },
	{ 0 }
};

#define	BUFFER	"/usr/adm/msgbuf"

main(argc, argv)
char **argv;
{
	int mem;
	int timeout();

	signal(SIGALRM, timeout);
	alarm(60);
	if (argc>1 && argv[1][0] == '-') {
		sflg++;
		switch (argv[1][1]) {
		default:		/* ugh */
			wflg++;
			break;

		case 'i':
			break;
		}
		argc--;
		argv++;
	}
	if (sflg)
		openbuf();
	sflg = 0;
	timeout("can't read namelist\n");
	nlist(argc>2? argv[2]:"/unix", nl);
	if (nl[0].n_type==0)
		done("No namelist\n");
	if ((mem = open((argc>1? argv[1]: "/dev/kmem"), 0)) < 0)
		done("No /dev/kmem\n");
	lseek(mem, (long)nl[0].n_value, 0);
	read(mem, &msgptr, sizeof(msgptr));
	lseek(mem, msgptr, 0);
	read(mem, &msgbuf, sizeof (msgbuf));
	if (msgbuf.msg_magic != MSG_MAGIC)
		done("Magic number wrong (namelist mismatch?)\n");
	if (bufdiff(&omesg, &msgbuf) == 0)
		exit(0);
	prdiff(&omesg, &msgbuf);
	done((char *)NULL);
}

bufdiff(om, nm)
register struct msgbuf *om, *nm;
{

	if (om->msg_bufx != nm->msg_bufx)
		return (1);
	return (memcmp(om->msg_bufc, nm->msg_bufc, MSG_BSIZE));
}

prdiff(om, nm)
register struct msgbuf *om, *nm;
{
	register int ox, nx;
	register int c;

	if ((ox = om->msg_bufx) < 0 || ox >= MSG_BSIZE)
		ox = 0;
	if ((nx = nm->msg_bufx) < 0 || nx >= MSG_BSIZE)
		nx = 0;
	pdate();
	/*
	 * did we lose something?
	 */
	if (ox < nx) {
		if (memcmp(&om->msg_bufc[0], &nm->msg_bufc[0], ox)
		||  memcmp(&om->msg_bufc[nx], &nm->msg_bufc[nx], MSG_BSIZE - nx)) {
			ox = nx;
			printf("...\n");
		}
	} else if (ox > nx) {
		if (memcmp(&om->msg_bufc[ox], &nm->msg_bufc[ox], nx - ox)) {
			ox = nx;
			printf("...\n");
		}
	}
	/*
	 * else ox == nx; something might be lost, but assume not
	 * now print the new part: from ox to nx-1
	 */
	do {
		c = nm->msg_bufc[ox++];
		if ((c & 0200) == 0 && c != 0)	/* sanity */
			putchar(c);
		if (ox >= MSG_BSIZE)
			ox = 0;
	} while (ox != nx);
}

done(s)
char *s;
{

	if (s && s!=(char *)omesg.msg_magic && sflg==0) {
		pdate();
		printf(s);
	}
	if (wflg) {
		wflg = 0;
		writebuf();
	}
	exit(s!=NULL);
}

openbuf()
{
	int f;

	timeout("can't read buffer file\n");
	if ((f = open(BUFFER, 0)) < 0)
		return;
	if (read(f, (char *)&omesg, sizeof(omesg)) != sizeof(omesg)
	||  omesg.msg_magic != MSG_MAGIC)
		memset((char *)&omesg, sizeof(omesg), 0);
	close(f);
}

writebuf()
{
	int f;

	timeout("can't write buffer file\n");
	if ((f = open(BUFFER, 1)) < 0)
		done("can't open buffer\n");
	if (write(f, (char *)&msgbuf, sizeof(msgbuf)) != sizeof(msgbuf))
		done("error writing buffer\n");
	close(f);
}

pdate()
{
	extern char *ctime();
	static firstime;
	time_t tbuf;

	if (firstime==0) {
		firstime++;
		time(&tbuf);
		printf("\n%.12s\n", ctime(&tbuf)+4);
	}
}

char *terr;

timeout(s)
char *s;
{
	extern ttrap();

	terr = s;
	signal(SIGALRM, ttrap);
	alarm(60);
}

ttrap()
{
	char buf[100];

	sprintf(buf, "timeout: %s", terr);
	done(buf);
}
