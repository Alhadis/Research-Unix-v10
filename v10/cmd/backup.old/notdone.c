#include	<fio.h>
#include	<cbt.h>
#include	<libc.h>
#include	"backup.h"
#include	<utsname.h>
#include	<signal.h>

int cflag = 0;
int nflag = 0;
extern int errno;
bfile *db;
char myname[FNAMELEN];
char bdir[FNAMELEN];
char tmpfile[] = "/tmp/notdXXXXXX";
long ask, ok;
typedef struct Csum
{
	unsigned short csum;	
	long nchars;
} Csum;
Csum send, rcv;

main(argc, argv)
	char **argv;
{
	char buf[4096];
	long t1, t2;
	register i;
	int fd;
	char *s;
	int (*fns)();
	int rcsum(), xcsum(), null();

	backupdir(bdir);
	if(argv[1] && (strcmp(argv[1], "-S") == 0)){
		fns = xcsum;
		dup2(0, 1);	/* write on 1 */
		mktemp(tmpfile);
		close(creat(tmpfile, 0666));
		fd = open(tmpfile, 2);
		if((s = Frdline(0)) == 0)
			s = "???";
		strcpy(argv[1] = myname, s);
		while(s = Frdline(0)){
			i = FIOLINELEN(0);
			s[i++] = '\n';
			if(rcsum(s, i))
				Fwrite(fd, s, (long)i);
			else
				break;
		}
		Fflush(fd);
		lseek(fd, 0L, 0);
		Finit(fd, (char *)0);
		signal(SIGHUP, SIG_IGN);	/* sighups before now are ok */
	} else {
		fns = null;
		fd = 0;
	}
	sprintf(buf, "%s/%s", bdir, BDBM);
	if((db = bopen(buf, 0)) == 0){
		perror(buf);
		exit(1);
	}
	(void)time(&t1);
	ask = ok = 0;
	lseek(1, 0L, 0);
	while(s = Frdline(fd))
		process(s, fns, *argv);
	(*fns)(0, 0, *argv);
	(void)time(&t2);
	if(*++argv == 0)
		*--argv = "??";
	bclose(db);
	unlink(tmpfile);
	logprint("notdone.%s: %ld/%ld (%lds)\n", *argv, ok, ask, t2-t1);
	exit(0);
}

process(s, fn, whoami)
	char *s;
	int (*fn)();
	char *whoami;
{
	mbuf d, key, gup;
	register char *unique;
	long t1, t2;
	char buf[512];
	char goo[4096], goo1[4096];
	int kk;

	ask++;
	d.mdata = goo;
	gup.mdata = goo1;
	for(unique = s; *unique; unique++)
		if((unique[0] == '/') && (unique[1] == '/'))
			break;
	if(*unique == 0){
		logprint("notdone %s: '%s' missing '//'\n", whoami, s);
		return;
	}
	*unique++ = 0;
	*unique++ = 0;
	key.mdata = s;
	key.mlen = strlen(s)+1;
	if(bseek(db, key) == 1){
		bread(db, (mbuf *)0, &d);
		d.mdata[d.mlen] = 0;
		t1 = atol(unique);
		t2 = atol(d.mdata);
		if(t1 <= t2){
			return;
		}
	}
	unique[-2] = unique[-1] = '/';
	kk = strlen(s);
	s[kk++] = '\n';
	(*fn)(s, kk);
	Fwrite(1, s, (long)kk);
	ok++;
}

null()
{
	return(1);
}

xcsum(s, n)
	char *s;
{
	if(s){
		send.nchars += n;
		send.csum = sum((unsigned char *)s, n, send.csum);
	} else {
		Fprint(1, "%c%-6ud%ld %ld %ld\n", 0, send.csum, send.nchars, ask, ok);
		Fflush(1);
	}
}

rcsum(s, n)
	char *s;
{
	unsigned short tsum;
	long tnchars;

	if(*s){
		rcv.nchars += n;
		rcv.csum = sum((unsigned char *)s, n, rcv.csum);
		return(1);
	} else {
		tsum = atoi(s+1);
		tnchars = atol(s+7);
		if((tsum != rcv.csum) || (tnchars != rcv.nchars)){
			logprint("notdone: xmiterr(%s): rcv=(#%ux,%ld) sent=(#%ux,%ld) debris=%s\n",
				myname, rcv.csum&0xFFFF, rcv.nchars, tsum&0xFFFF, tnchars, tmpfile);
			exit(1);
		}
		return(0);
	}
}
