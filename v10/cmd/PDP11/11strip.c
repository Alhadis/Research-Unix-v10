#include "a.out.h"		/*god: using pdp11 a.out fmt */
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

char	*tname;
char	*mktemp();
struct exec head;
int 	a_magic[] = {A_MAGIC1, A_MAGIC2, A_MAGIC3, A_MAGIC4, 0};
int	status;
int	tf;

main(argc, argv)
char *argv[];
{
	register i;

	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	tname = mktemp("/tmp/sXXXXX");
	close(creat(tname, 0600));
	tf = open(tname, 2);
	if(tf < 0) {
		printf("cannot create temp file\n");
		exit(2);
	}
	if (argc == 1) {		/*god: default 11a.out */
	    argv[1] = A_DOT_OUT;	/*god*/
	    ++argc;			/*god*/
	}				/*god*/
	for(i=1; i<argc; i++) {
		strip(argv[i]);
/*god*/		/*if(status > 1)
			break;*/	/*god: no reason to abort*/
	}
	close(tf);
	unlink(tname);
	exit(status);
}

strip(name)
char *name;
{
	register f;
	long size;
	int i;
	time_t tm[2];
	struct stat stbuf;

	tm[0] = 0;
	f = open(name, 0);
	if(f < 0) {
		printf("cannot open %s\n", name);
		status = 1;
		goto out;
	}
	fstat(f, &stbuf);
	tm[0] = stbuf.st_atime;
	tm[1] = stbuf.st_mtime;
	read(f, (char *)&head, sizeof(head));
	for(i=0;a_magic[i];i++)
		if(a_magic[i] == head.a_magic) break;
	if(a_magic[i] == 0) {
		printf("%s not in 11a.out format\n", name);	/*god*/
		status = 1;
		goto out;
	}
	if(head.a_syms == 0 && (head.a_flag&1) != 0) {
		printf("%s already stripped\n", name);
		goto out;
	}
	size = (long)head.a_text + head.a_data;
	head.a_syms = 0;
	head.a_flag |= 1;

	lseek(tf, (long)0, 0);
	write(tf, (char *)&head, sizeof(head));
	if(copy(name, f, tf, size)) {
		status = 1;
		goto out;
	}
	size += sizeof(head);
	close(f);
	f = creat(name, 0666);
	if(f < 0) {
		printf("%s cannot recreate\n", name);
		status = 1;
		goto out;
	}
	lseek(tf, (long)0, 0);
	if(copy(name, tf, f, size))
		status = 2;

out:
	close(f);
	if (tm[0])
		utime(name, tm);
}

copy(name, fr, to, size)
char *name;
long size;
{
	register s, n;
	char buf[512];

	while(size != 0) {
		s = 512;
		if(size < 512)
			s = size;
		n = read(fr, buf, s);
		if(n != s) {
			printf("%s unexpected eof\n", name);
			return(1);
		}
		n = write(to, buf, s);
		if(n != s) {
			printf("%s unexpected write eof\n", name);
			return(1);
		}
		size -= s;
	}
	return(0);
}
