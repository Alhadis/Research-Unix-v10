#include	<fio.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"backup.h"
#include	<errno.h>
#include	<pwd.h>
#include	<grp.h>
#include	<libc.h>

struct backfile h;
char *todir = 0;
int verbose = 0;
int dflag = 0;
int Fflag = 0;
char *inp, *outp;
char bkpdir[FNAMELEN];
char map[FNAMELEN];

#define	WRITE(b,n)	if(write(ofd,b,n)!=n){perror("write");unlink(oname);exit(4);}else

main(argc, argv)
	char **argv;
{
	int ifd, ofd;
	char *line, *name;
	char *dev = "0";

	signal(SIGPIPE, SIG_IGN);
	backupdir(bkpdir);
	line = strchr(bkpdir, 0)+1;
	line = strchr(line, 0)+1;
	if(*line && strcmp(line, "worm"))
		dev = line;
	for(argv++; *argv && **argv == '-'; argv++)
		switch(argv[0][1])
		{
		case 'v':
			verbose++;
			break;
		case 'd':
			dflag++;
			break;
		case 'F':
			Fflag++;
			break;
		case 'f':
			if(argv[1] && (argv[0][2] == 0))
				dev = *++argv;
			else
				dev = &argv[0][2];
			break;
		case 'D':
			inp = &argv[0][2];
			outp = strchr(inp, '=');
			if(outp == 0)
				usage();
			*outp++ = 0;
			break;
		case 'o':
			if(todir = *++argv)
				break;
			else
				usage();
		default:
			usage();
		}
	if(connect(dev, &ifd, &ofd) < 0)
		exit(1);
	Finit(ofd, (char *)0);
	if(*argv){
		while(*argv)
			Fprint(ofd, "%s\n", *argv++);
	} else {
		while(line = Frdline(0)){
			if((name = strrchr(line, ' ')) == 0)
				name = line;
			Fprint(ofd, "%s\n", name);
		}
	}
	Fprint(ofd, "bye\n");
	Fflush(ofd);
	Finit(ifd, (char *)0);
	while(do1(ifd))
		;
	ldump();
	exit(0);
}

usage()
{
	Fprint(2, "Usage: backup wfetch [-v] [-F] [-Do=n] [-fdev] [-d] [-o destdir] [files]\n");
	Fprint(2, "\t-v\tverbose\n");
	Fprint(2, "\t-d\tcreate missing intermediate directories\n");
	Fprint(2, "\t-o dir\trestore to specified directory\n");
	Fprint(2, "\t-F\trestore dir as a file (ls -f)\n");
	Fprint(2, "\t-Do=n\tstrip prefix o from filenames and replace with n\n");
	Fprint(2, "\t-fdev\tuse dev (default /dev/worm0) for optical disk\n");
	exit(1);
}

connect(dev, ifd, ofd)
	char *dev;
	int *ifd, *ofd;
{
	char *mach, *s;
	char buf[256], buf1[256], status[1];
	struct passwd *p;
	struct group *g;

	if(s = strchr(dev, '!')){
		mach = dev;
		dev = s;
		*dev++ = 0;
	} else {
		mach = bkpdir;
		while(*mach++)
			;
	}
	sprint(buf, "bkpworm%s", dev);
	sprint(buf1, "startworm%s", dev);
loop:
	if(rx(mach, buf, buf1, "heavy", ifd, ofd))
		return(-1);
	read(*ifd, status, 1);
	if(status[0] == 'B'){
		fprint(2, "%s busy, trying again\n", buf);
		sleep(30);
		goto loop;
	}
	g = getgrgid(getegid());
	p = getpwuid(geteuid());
	fprint(*ofd, "U%s %s\n", p?p->pw_name:"?", g?g->gr_name:"?");
	return(0);
}

do1(fd)
{
	char *oname, *setoname();
	int ofd, n, readn;
	char buf[16*1024L], nbuf[16*1024L];	/* over a network */
	time_t timep[2];
	long size;
	unsigned short csum;

	if((oname = Frdline(fd)) == 0)
		return(0);
	if(*oname != 'F'){
		fprint(2, "%s\n", oname);
		return(1);
	}
	if(Fread(fd, (char *)&h, (long)sizeof h) != sizeof h){
		perror("header read");
		return(0);
	}
	oname = setoname(h.oname);
	csum = sum(((unsigned char *)&h)+2, sizeof h - 2, 0);
	switch(h.sbuf.st_mode&S_IFMT)
	{
	case S_IFDIR:
		if(Fflag){
			print("dir (-F): ");
			goto filecopy;
		}
		if(verbose)
			print("mkdir %s\n", oname);
		execute("/bin/mkdir",  "mkdir", oname, (char *)0, (char *)0, (char *)0);
		if(chmod(oname, h.sbuf.st_mode&07777) < 0){
			perror(oname);
		}
		ofd = open("/dev/null", 1);
		goto filecopy2;	/* suck those bytes */
		break;
	case S_IFLNK:
		n = h.sbuf.st_size;
		if(n != Fread(fd, nbuf, (long)n)){
			fprint(2, "short read on %s\n", oname);
			return(0);
		}
		csum = sum((unsigned char *)buf, n, csum);
		if(csum != h.checksum){
			fprint(2, "%s: checksum disagree o=%d n=%d\n", oname, h.checksum, csum);
			return(1);
		}
		nbuf[n] = 0;
		if(verbose)
			print("%s symbolic link to %s\n", oname, nbuf);
		if(execute("/bin/ln", "ln", "-s", nbuf, oname, (char *)0))
			return(-1);
		return(0);
	case S_IFREG:
		if(lchk(oname, &h.sbuf))
			break;
	filecopy:
		if(verbose)
			print("%s -> %s\n", h.bname, oname);
		if((ofd = creat(oname, h.sbuf.st_mode)) == -1)
			if(dflag){
				createdirs(oname);
				ofd = creat(oname, h.sbuf.st_mode);
			}
		if(ofd == -1){
			perror(oname);
			ofd = open("/dev/null", 1);
		}
	filecopy2:	/* see dir case above */
		size = h.sbuf.st_size;
		for(n = sizeof buf; size > 0; size -= readn){
			if(n > size)
				n = size;
			readn = Fread(fd, buf, (long)n);
			if(readn < 0){
				perror("read");
		err:
				unlink(oname);
				close(ofd);
				return(-1);
			} else if(readn == 0){
				fprint(2, "unexpected EOF; %ld bytes to read for %s\n", size, oname);
				return(0);
			}
			csum = sum((unsigned char *)buf, readn, csum);
			WRITE(buf, readn);
		}
		if(csum != h.checksum){
			fprint(2, "%s: checksum disagree o=%d n=%d\n", oname, h.checksum, csum);
			goto err;
		}
		close(ofd);
		break;
	default:
		print("%s is an unsupported type of file: st_mode=0%o\n",
			h.bname, h.sbuf.st_mode);
		return(0);
	}
	if((h.sbuf.st_uid != geteuid()) || (h.sbuf.st_gid != getegid())){
		if(chown(oname, h.sbuf.st_uid, h.sbuf.st_gid) < 0){
			fprint(2, "chown(%d,%d): ", h.sbuf.st_uid, h.sbuf.st_gid);
			perror(oname);
		}
	}
	timep[0] = h.sbuf.st_atime;
	timep[1] = h.sbuf.st_mtime;
	if(utime(oname, timep) < 0)
		perror("set times");
	return(1);
}

char *
setoname(name)
	char *name;
{
	static char goo[2*FNAMELEN];
	extern char *strrchr();
	register char *s;

	if(todir){
		if(s = strrchr(name, '/'))
			s++;
		else
			s = name;
		sprint(goo, "%s/%s", todir, s);
	}
	else if(inp && (strncmp(name, inp, strlen(inp)) == 0))
		sprint(goo, "%s%s", outp, name+strlen(inp));
	else
		strcpy(goo, name);
	return(goo);
}

execute(a1, a2, a3, a4, a5, a6)
	char *a1, *a2, *a3, *a4, *a5, *a6;
{
	int status;

	switch(fork())
	{
	case -1:
		perror(a1);
		return(1);
	case 0:
		execl(a1, a2, a3, a4, a5, a6);
		perror(a1);
		exit(1);
	default:
		wait(&status);
		return(status);
	}
}

#define	NL	200
struct fdi
{
	dev_t dev;
	ino_t ino;
	short nlk;
	char name[256];
} links[NL];
int nl = 0;

lchk(s, sb)
	char *s;
	register struct stat *sb;
{
	register struct fdi *l;
	char buf[4096];

	if(sb->st_nlink <= 1)
		return(0);
	for(l = links; l < &links[nl]; l++)
		if((l->dev == sb->st_dev) && (l->ino == sb->st_ino)){
			l->nlk--;
			if(verbose)
				print("linking %s to %s\n", s, l->name);
			unlink(s);
			sprint(buf, "IFS='' /bin/ln %s %s", l->name, s);
			system(buf);
			return(1);
		}
	if(l < &links[NL]){
		l->nlk = sb->st_nlink-1;
		strcpy(l->name, s);
		l->dev = sb->st_dev;
		l->ino = sb->st_ino;
		nl++;
	}
	return(0);
}

ldump()
{
	register struct fdi *l;

	for(l = links; l < &links[nl]; l++)
		if(l->nlk)
			print("Warning: %s has %d links broken\n", l->name, l->nlk);
}

createdirs(s)
	char *s;
{
	char *ls, *ss;

	for(ls = s; *ls == '/'; ls++)
		;
	for(; *ls && (ss = strchr(ls, '/')); ls = ss+1){
		*ss = 0;
		if(access(s, 0) < 0){
			if(mkdir(s, 0777) < 0){
				perror(s);
				return;
			} else if(verbose)
				fprint(2, "created %s\n", s);
		}
		*ss = '/';
	}
}
