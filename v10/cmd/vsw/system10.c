#include	<libc.h>
#include	<fio.h>
#include	<ctype.h>
#include	<sysent.h>
#include	<string.h>
#include	"crossbar.h"

int rread(int fd, char *buf, int n);

System10::System10(char *dkname, int n, int depth) : (dkname, n, depth)
{
}

void
System10::startup()
{
	int i, n;
	char buf[256];
	extern int opendk(char *);

	fd = opendk(dial);
	for(i = 0; i < 3; i++){
		write(fd, "@\r", 2);
		read(fd, buf, 3);
		if((n = read(fd, buf, sizeof buf)) != 3){
			fprint(2, "read %d, wanted %d: %s: ", n, 3, buf);
			perror("read");
			exit(1);
		}
		if(strncmp(buf, "@\r", 2) == 0)
			break;
	}
}

int
System10::connect(int input, Crossbar *c, int output, int nw)
{
	char buf[256];
	int n;

	if(fd < 0)
		startup();
	if(c != this){
		if(nextc->connect(nextl, c, output, nw) == 0)
			return(0);
		output = froml;
	}
	nw /= nwires;
	while(nw-- > 0){
		fprint(fd, "%d%d\r", output, input);
		n = read(fd, buf, 7);
		if(n != 7){
			fprint(2, "%s.con %d->%d failed; %d!=7\n", name(), input, output, n);
			return(0);
		}
		input++, output++;
	}
	return(1);
}

void
System10::config()
{
	int i, n;
	char buf[256], *s;

	imap = new int[nouts];
	write(fd, "?\r", 2);
	read(fd, buf, 3);
	if((n = rread(fd, buf, 31)) < 0){
		perror("read2");
		exit(1);
	}
	if(n != 31){
		fprint(2, "erk! read expected 31, got %d '%s'\n", n, buf);
		exit(1);
	}
	for(i = 0, s = buf; i < 10; i++){
		if(!isdigit(*s)){
			fprint(2, "erk: output must be digit '%s'\n", s);
			exit(1);
		}
		if(!isdigit(s[1])){
			fprint(2, "erk: input must be digit '%s'\n", s+1);
			exit(1);
		}
		imap[*s-'0'] = s[1]-'0';
if(0)Fprint(1, "%c<-%c ", s[0], s[1]);
		s += 3;
	}
if(0)Fprint(1, "\n"), Fflush(1);
}

int
rread(int fd, char *buf, int n)
{
	int k, m = 0;

	while(m != n){
buf[m]=0;fprint(2, "read %d of %d '%s'\n", m, n, buf);
		k = read(fd, &buf[m], n-m);
		if(k < 0)
			return(k);
		if(k == 0)
			break;
		m += k;
	}
	return(m);
}
