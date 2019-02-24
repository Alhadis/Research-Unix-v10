#include	<fio.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"backup.h"
#include	<libc.h>

struct backfile h;
char *todir = 0;
int verbose = 0;
int dflag = 0;
int Fflag = 0;
char *inp, *outp;
int erred = 0;
char map[FNAMELEN];

#define	WRITE(b,n)	if(write(ofd,b,n)!=n){perror("write");unlink(oname);exit(4);}else

main(argc, argv)
	char **argv;
{
	char *line, *name;

	for(argv++; *argv && **argv == '-'; argv++)
		switch(argv[0][1])
		{
		case 'v':
			verbose++;
			break;
		case 'd':
			dflag++;
			break;
		case 'f':
			break;	/* compatability with wfetch */
		case 'F':
			Fflag++;
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
	if(*argv){
		while(*argv)
			do1(*argv++);
	} else {
		while(line = Frdline(0)){
			if((name = strrchr(line, ' ')) == 0)
				name = line;
			do1(name);
		}
	}
	ldump();
	exit(erred);
}

usage()
{
	Fprint(2, "Usage: backup fetch [-vd] [-o destdir] [files]\n");
	Fprint(2, "\t-v\tverbose\n");
	Fprint(2, "\t-d\tcreate missing intermediate directories\n");
	Fprint(2, "\t-o dir\trestore to specified directory\n");
	Fprint(2, "\t-F\trestore dir as a file (ls -f)\n");
	Fprint(2, "\t-Do=n\tstrip prefix o from filenames and replace with n\n");
	exit(1);
}

do1(iname)
	char *iname;
{
	char *oname, *setoname();
	int ofd, ifd, n;
	int bad;
	char buf[60*1024L], nbuf[60*1024L];	/* a hack but a largish number */
	time_t timep[2];
	long size;

	if(oname = strrchr(iname, ' '))
		iname = oname+1;
	{
		if((ifd = open(iname, 0)) < 0){
			perror(iname);
			erred = 1;
			return;
		}
		if(read(ifd, (char *)&h, sizeof h) != sizeof h){
			perror(iname);
			exit(2);
		}
	}
	/* protection against non-backup files */
	if((h.version < 0) || (h.version > CURVERSION)){
		fprint(2, "backup fetch: %s: bad version %d\n", iname, h.version);
		exit(3);
	}
	h.oname[sizeof h.oname - 1] = 0;
	oname = setoname(h.oname);
	switch(h.sbuf.st_mode&S_IFMT)
	{
	case S_IFDIR:
		if(Fflag){
			print("dir (-F): ");
			goto filecopy;
		}
		if(verbose)
			print("mkdir %s\n", oname);
		sprint(buf, "IFS='' /bin/mkdir %s", oname);
		if(execute("/bin/mkdir",  "mkdir", oname, (char *)0, (char *)0, (char *)0)) return;
		if(chmod(oname, h.sbuf.st_mode&07777) < 0){
			perror(oname);
			close(ifd);
			return;
		}
		break;
	case S_IFLNK:
		n = h.sbuf.st_size;
		n = read(ifd, nbuf, n);
		nbuf[n] = 0;
		if(verbose)
			print("%s symbolic link to %s\n", oname, nbuf);
		close(ifd);
		sprint(buf, "IFS='' /bin/ln -s %s %s", nbuf, oname);
		if(execute("/bin/ln", "ln", "-s", nbuf, oname, (char *)0)) return;
		return;
	case S_IFREG:
		if(lchk(oname, &h.sbuf))
			break;
	filecopy:
		if(verbose)
			print("%s -> %s\n", iname, oname);
		if((ofd = creat(oname, h.sbuf.st_mode)) == -1)
			if(dflag){
				createdirs(oname);
				ofd = creat(oname, h.sbuf.st_mode);
			}
		if(ofd == -1){
			perror(oname);
			return;
		}
		size = h.sbuf.st_size;
		for(n = sizeof buf; size > 0; size -= n){
			if(n > size)
				n = size;
			{
				bad = 0;
				if(read(ifd, buf, n) != n)
					if(n >= 0){
						fprint(2, "Warning: short read on %s; %ld bytes left\n", iname, size-n);
						size = n;
					} else
						bad = 1;
			}
			if(bad){
				perror("read");
				unlink(oname);
				close(ifd);
				return;
			}
			WRITE(buf, n);
		}
		close(ofd);
		break;
	default:
		print("%s is an unsupported type of file: st_mode=0%o\n",
			iname, h.sbuf.st_mode);
		close(ifd);
		return;
	}
	close(ifd);
	if((h.sbuf.st_uid != geteuid()) || (h.sbuf.st_gid != getegid())){
		if(chown(oname, h.sbuf.st_uid, h.sbuf.st_gid) < 0)
			perror("chown to owner");
	}
	timep[0] = h.sbuf.st_atime;
	timep[1] = h.sbuf.st_mtime;
	if(utime(oname, timep) < 0)
		perror("set times");
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
