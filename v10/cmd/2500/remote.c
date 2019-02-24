#include	<fio.h>
#include	"hdr.h"

#define		RELAY		"/v/bin/2500relay"

static int frameno;
static int ifd, ofd;
static spawn(char *sys, char *relay, char *prog, int *i, int *o);

remsetup(char *system, char *prog, char *relay)
{
	if(relay == 0)
		relay = RELAY;
	if(spawn(system, relay, prog,  &ifd, &ofd))
		exit(1);
	Finit(ofd, (char *)0);
}

remsframe(char *str)
{
	char *s;

	fprint(ifd, "%s\n", str);
	if(s = Frdline(ofd)){
		if(*s)
			fprint(2, "[remote] %s\n", s);
	} else {
		perror("remote died");
		exit(1);
	}
}

remiframe(int fr)
{
	char buf[64];

	if(fr >= 0)
		frameno = fr;
	sprint(buf, "%d", frameno);
	frameno++;
	remsframe(buf);
}

shoot(int start, int nframes, int snapc)
{
	int n = nframes;
	frameno = start;
	print("shooting frames %d..%d\n", start, start+nframes-1);
	while(nframes-- > 0){
		remiframe(-1);
		snap(snapc);
	}
	print("shot %d frames\n", n);
}

static
spawn(char *sys, char *relay, char *prog, int *i, int *o)
{
	int toremote[2], fromremote[2];
	int n;

	if((pipe(toremote) < 0) || (pipe(fromremote) < 0)){
		perror("pipe");
		return(1);
	}
	if((n = fork()) < 0){
		perror("fork");
		return(1);
	}
	if(n){	/* parent */
		*i = toremote[1];
		close(toremote[0]);
		*o = fromremote[0];
		close(fromremote[1]);
		remsframe(prog);
	} else {
		char buf[256];

		close(logfd);
		dup2(toremote[0], 0);
		dup2(fromremote[1], 1);
		close(toremote[0]);
		close(toremote[1]);
		close(fromremote[0]);
		close(fromremote[1]);
		sprint(buf, "rsh %s '%s'", sys, relay);
		system(buf);
		sleep(5);
		exit(1);
	}
	return(0);
}
