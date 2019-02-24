#include	<fio.h>
#include	<ipc.h>
#include	<pwd.h>
#include	<grp.h>
#include	<errno.h>
#include	"backup.h"
#include	<string.h>
#include	<signal.h>

struct backfile h;
extern int errno;
int ofd;
char *fname;
unsigned short csum;

main(argc, argv)
	char **argv;
{
	char *s, *ss, buf[FNAMELEN];
	char *mach;
	int ifd;
	int retry = 0;
	extern long atol();

	signal(SIGPIPE, SIG_IGN);/**/
	chdir("/tmp");
	backupdir(buf);
	mach = strchr(buf, 0)+1;
	if(rx(mach, "bkprcv", (char *)0, "heavy", &ifd, &ofd))
		exit(1);
	if(*++argv == 0)
		*--argv = "??";
	write(ofd, *argv, strlen(*argv)+1);
	while(s = Frdline(0)){
		if((ss = strrchr(s, ' ')) == 0){
			exit(2);
		}
		*ss++ = 0;
		fname = s;
		while(*s)
			if((s[0] == '/') && (s[1] == '/')){
				*s = 0;
				break;
			} else
				s++;
		while(do1(fname, atol(ss))){
			if(!retry)
				goto done;
			h.version = -1;
			WRITE((char *)&h, sizeof h);
			close(ifd);
			close(ofd);
			sleep(20);
			fprint(2, "retrying: ");
			if(rx(mach, "bkprcv", (char *)0, "heavy", &ifd, &ofd))
				exit(1);
			write(ofd, *argv, strlen(*argv)+1);
		}
	}
done:
	h.version = -1;
	WRITE((char *)&h, sizeof h);
	Finit(ifd, (char *)0);
	s = Frdline(ifd);
	if(argc > 2){
		if(s)
			fprint(2, "file transfer: %s\n", s);
		else
			fprint(2, "uh oh! i didn't get the transmission statistics!\n");
	}
	exit(0);
}

do1(file, size)
	char *file;
	long size;
{
	register ifd, n;
	char buf[512];
	struct passwd *p;
	struct group *g;
	int ret = 0;

	if((ifd = open(file, 0)) < 0){
		if(errno != ENOENT)	/* don't bitch about missing files */
			perror(file);
		return(ret);
	}
	memset((char *)&h, 0, sizeof h);
	h.version = CURVERSION;
	h.checksum = 0;		 /* done later */
	if(getflab(file, &h.lab) < 0){
		perror(file);
		exit(9);
	}
	if(lstat(file, &h.sbuf) < 0){
		perror(file);
		exit(4);
	}
	if(size > sizeof h + h.sbuf.st_size)
		size = sizeof h + h.sbuf.st_size;
	else if(size < sizeof h + h.sbuf.st_size)
		h.sbuf.st_size = size - sizeof h;
	if(p = getpwuid(h.sbuf.st_uid))
		strcpy(h.uname, p->pw_name);
	else
		sprint(h.uname, "#%d", h.sbuf.st_uid);
	setgrent();
	if(g = getgrgid(h.sbuf.st_gid))
		strcpy(h.gname, g->gr_name);
	else
		sprint(h.gname, "#%d", h.sbuf.st_gid);
	strncpy(h.oname, file, FNAMELEN);
	h.oname[FNAMELEN-1] = 0;
	h.bname[0] = 0;
	csum = sum(((unsigned char *)&h)+2, sizeof h - 2, 0);
	WRITE((char *)&h, sizeof(h));
	size -= sizeof h;
	switch(h.sbuf.st_mode&S_IFMT)
	{
	case S_IFLNK:
		if(readlink(file, buf, sizeof buf) < 0){
			perror(file);
			exit(5);
		}
		WRITE(buf, (int)size);
		csum = sum((unsigned char *)buf, (int)size, csum);
		break;
	case S_IFDIR:
		if(dsend(file, size)){
			perror(file);
			exit(6);
		}
		break;
	case S_IFREG:
		if(send(ifd, size)){
			perror(file);
			exit(3);
		}
		break;
	}
	close(ifd);
	WRITE((char *)&csum, 2);
	ifd = 0;
	while((n = read(ofd, buf, sizeof buf)) > 0){
		if((n == 1) && (buf[0] == 'X'))
			break;
		if(buf[0] == '@')
			write(2, buf+1, n-1);
		else if(buf[0] == '*'){
			if(ret == 0)
				write(2, buf+1, n-1);
			ret = 1;
		} else {
			write(2, buf, n);
			ifd = 1;
		}
	}
	if(ifd)
		exit(8);
	return(ret);
}

send(ifd, size)
	long size;
{
	char buf[512];
	int n;

	while((n = read(ifd, buf, sizeof buf)) > 0){
		if(n < 0)
			return(1);
		if(n > size) n = size;
		if(n)
			WRITE(buf, n);
		csum = sum((unsigned char *)buf, n, csum);
		if((size -= n) == 0) break;
	}
	memset(buf, 0, sizeof buf);
	while(size){
		n = min((int)size, sizeof buf);
		write(ofd, buf, n);
		csum = sum((unsigned char *)buf, n, csum);
		size -= n;
	}
	return(0);
}

#include	<ndir.h>

dsend(name, size)
	char *name;
	long size;
{
	char buf[512];
	int n;
	DIR *dirp;
	register struct direct *d;

	memset(buf, 0, 4);
	WRITE(buf, 4);
	csum = sum((unsigned char *)buf, 4, csum);
	size -= 5;	/* one later on */
	dirp = opendir(name);
	while(d = readdir(dirp)){
		n = d->d_namlen+1;
		if(n > size)
			break;
		WRITE(d->d_name, n);
		csum = sum((unsigned char *)d->d_name, n, csum);
		size -= n;
	}
	closedir(dirp);
	memset(buf, 0, sizeof buf);
	csum = sum((unsigned char *)buf, 1, csum);
	WRITE(buf, 1);
	while(size){
		n = min((int)size, sizeof buf);
		WRITE(buf, n);
		csum = sum((unsigned char *)buf, n, csum);
		size -= n;
	}
	return(0);
}

WRITE(b, n)
	char *b;
{
	register ret;

	if((ret = write(ofd, b, n)) != n){
		fprint(2, "%s: write(%d)=%d", fname, n, ret);
		perror("");
		exit(7);
	}
}
