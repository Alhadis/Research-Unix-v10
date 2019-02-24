/* time command */

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/vtimes.h>

extern int errno;
extern char *sys_errlist[];
int	vflag = 0;
char	output[BUFSIZ];

#define	U	0001
#define	S	0002
#define	R	0004
#define	T	0010
#define	D	0020
#define	M	0040
#define	F	0100
#define	I	0200
#define	O	0400
#define	ALL	0777

#define	t(x)	(flags&x)

int	flags = U|S|R;
char	names[] = "usrtdmfio";

main(argc, argv)
char **argv;
{
	double x;
	int status;
	struct vtimes vt;
	time_t before, after;
	char buf[BUFSIZ];
	register char *p;
	register i, pid;

	setbuf(stderr, buf);
	if(argc>1 && argv[1][0]=='-') {
		flags = 0;
		for(flags=0; argc>1 && argv[1][0]=='-'; --argc, argv++)
			for(p=argv[1]+1; *p; p++) {
				if(*p == 'v')
					flags = ALL;
				else if((i=index(*p, names)) < 0)
					fprintf(stderr, "time: bad flag %c\n", *p);
				else
					flags |= 1<<i;
			}
	}
	if(argc <= 1) {
		fprintf(stderr, "usage: time [-usrtdmfio] [-v] cmd\n");
		exit(1);
	}
	time(&before);
	pid = fork();
	if(pid == -1) {
		perror("Try again");
		exit(1);
	}
	if(pid == 0) {
		execvp(argv[1], &argv[1]);
		fprintf(stderr, "%s: %s\n", argv[1], sys_errlist[errno]);
		exit(1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while(wait(&status) != pid)
		;
	time(&after);
	vtimes((struct vtimes *)0, &vt);
	if((status&0377) != 0)
		fprintf(stderr,"Command terminated abnormally.\n");
	x = vt.vm_utime+vt.vm_stime;
	if(t(U))
		add("%.1fu", (double)vt.vm_utime/60);
	if(t(S))
		add("%.1fs", (double)vt.vm_stime/60);
	if(t(R))
		add("%dr", after-before);
	if(t(T))
		add("%.0ft", vt.vm_ixrss/2/x);
	if(t(D))
		add("%.0fd", vt.vm_idsrss/2/x);
	if(t(M))
		add("%dm", vt.vm_maxrss/2);
	if(t(F))
		add("%df", vt.vm_majflt);
	if(t(I))
		add("%di", vt.vm_inblk);
	if(t(O))
		add("%do", vt.vm_oublk);
	add("\t");
	for(i=1; i<argc; i++) {
		add("%s", argv[i]);
		if(i > 4) {
			add("...");
			break;
		}
	}
	fprintf(stderr, "%s\n", output);
	return (status&0377)? -1 : (status>>8)&0377;
}
add(a, b, c, d, e)
	char *a;
	int b, c, d, e;
{
	static beenhere=0;
	if(beenhere)
		strcat(output, " ");
	sprintf(output+strlen(output), a, b, c, d, e);
	beenhere++;
}
index(c, s)
	register char *s;
{
	register i;
	for(i=0; *s; i++, s++)
		if(*s == c)
			return i;
	return -1;
}
