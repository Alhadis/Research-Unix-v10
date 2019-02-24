#include	<pwd.h>
#include	<grp.h>
#include	<errno.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"backup.h"

struct backfile h;
extern int errno;

#define	WRITE(b,n)	if((ret = write(ofd,b,n))!=n){if(ret<0){perror("write");unlink(argv[2]);exit(4);}else{fprint(2,"less bytes than expected\n");}}else

main(argc, argv)
	char **argv;
{
	register ifd, ofd, n;
	char buf[4096];
	struct passwd *p;
	struct group *g;
	int ret;
	int remove = 0;
	int uid, gid;
	long size;
	char mbuf[256];
	extern long atol();

	if((argc == 4) && (strcmp(argv[1], "-r") == 0)){
		remove = 1;
		argv++;
		argc--;
	}
	if(argc != 3){
		fprint(2, "Usage: bcp [-r] from to\n");
		exit(1);
	}
	if(toname(argv[2]))
		exit(6);
	if((ifd = open(argv[1], 0)) < 0){
		perror(argv[1]);
		exit(1);
	}
	if(read(ifd, (char *)&h, sizeof h) != sizeof h){
		perror(argv[1]);
		exit(2);
	}
	if(p = getpwnam(h.uname))
		uid = p->pw_uid;
	else
		uid = -1;
	if(g = getgrnam(h.gname))
		gid = g->gr_gid;
	else
		gid = -1;
	if((ofd = creat(argv[2], h.sbuf.st_mode&0444)) < 0){
		perror(argv[2]);
		exit(3);
	}
	if(chown(argv[2], uid, gid) < 0){
		perror("chown");
		exit(4);
	}
	WRITE((char *)&h, sizeof(h));
	size = h.sbuf.st_size;
	while(size > 0){
		n = read(ifd, buf, sizeof buf);
		if(n <= 0){
			sprint(mbuf, "reading %s (continuing)", argv[1]);
			perror(mbuf);
			unlink(argv[2]);
			exit(0);
		}
		if(n > size) n = size;
		WRITE(buf, n);
		size -= n;
	}
	if(remove)
		if(unlink(argv[1]) < 0)
			perror(argv[1]);
	exit(0);
}

toname(s)
	char *s;
{
	register char *ss;

	if(access(s, 0) != -1){
		fprint(2, "%s exists\n", s);
		return(1);
	}
	ss = &s[strlen(s)-1];
	while((*ss >= '0') && (*ss <= '9'))
		ss--;
	if((*ss-- == '/') && (ss > s)){
		while((*ss >= '0') && (*ss <= '9'))
			ss--;
		if(ss > s){
			ss -= 3;
			if((ss > s) && (strncmp(ss, "/v/v", 4) == 0))
				return(0);
		}
	}
	fprint(2, "%s must end in ...'/v/vnnn/nnn'\n", s);
	return(1);
}
