#include	<getflags.h>
#include	<sys/types.h>
#include	<sys/filio.h>
#include	<sys/ttyio.h>
#include	<ctype.h>

int logfd;
int verbose;
int vswfd;
int row, col;
int mode;
int didoutput;

main(argc, argv)
	char **argv;
{
	int i, n, in, out;
	char buf[256];
	struct sgttyb vvec;
	extern int tty_ld;

	if((argc = getflags(argc, argv, "vsfi:1", 0)) < 0)
		usage("");
	verbose = flag['v'] != 0;
	logfd = creat("vswconlog", 0666);
	if(flag['i']){
		if((vswfd = open(flag['i'][0], 0)) < 0){
			perror(flag['i'][0]);
			exit(1);
		}
	} else {
		sprint(buf, "/cs/dk!vswcon");
		vswfd = ipcopen(buf, "light");
		if(vswfd < 0){
			perror(buf);
			exit(1);
		}
		if(ioctl(vswfd, FIOLOOKLD, 0) != tty_ld){
			if(ioctl(vswfd, FIOPUSHLD, &tty_ld) < 0){
				perror("tty_ld");
				exit(1);
			}
		}
		vvec.sg_flags = RAW;
		vvec.sg_erase = 0;
		vvec.sg_kill = 0;
		if(ioctl(vswfd, TIOCSETP, &vvec) < 0){
			perror("ioctl");
			exit(1);
		}
	}
	if(flag['f'])
		fileinput();
	else
		interactive();
}

fileinput()
{
	int i, n, in, out;
	long t0;
	char buf[256];
	fd_set fds;

	FD_ZERO(fds);
	for(;;){
		t0 = time((long *)0)+2;
		didoutput = 0;
		for(;;){
			FD_SET(vswfd, fds);
			if((select(vswfd+1, &fds, (fd_set *)0, 2000) > 0) && FD_ISSET(vswfd, fds)){
				if(read(vswfd, buf, 1) == 1){
					Fputc(logfd, buf[0]);
					vt100(buf[0]);
				}
			}
			if(time((long *)0) >= t0){
				if(didoutput)
					didoutput = 0, t0 = time((long *)0)+2;
				else
					break;
			}
		}
		if(read(0, buf, 1) <= 0)
			break;
if(0)fprint(2, "<%c>", buf[0]);
		if(buf[0] == '_'){
			while(read(0, buf, 1) == 1)
				if(buf[0] == '\n')
					break;
		} else
			kbd(buf[0]);
	}
}

interactive()
{
	int i, n, in, out;
	char buf[256];
	struct sgttyb vvec;
	fd_set fds;

	FD_ZERO(fds);
	for(;;){
		FD_SET(0, fds);
		FD_SET(vswfd, fds);
		if(select(vswfd+1, &fds, (fd_set *)0, 1000*3600) < 0){
			perror("select");
			exit(1);
		}
		if(FD_ISSET(vswfd, fds)){
			if(read(vswfd, buf, 1) == 1){
				Fputc(logfd, buf[0]);
				vt100(buf[0]);
				continue;
			}
		}
		if(FD_ISSET(0, fds)){
			read(0, buf, 1);
			kbd(buf[0]);
		}
	}
}

typedef enum { IDLE, ESC, NUM1, NUM2 } State;

vt100(c)
{
	static State state = IDLE;
	static prchar = 0;
	static char buf[256];
	static next = 0;
	static num;

	switch(state)
	{
	case IDLE:
		if(c == 033){
			state = ESC;
		flush:
			if(next && !flag['s'])
				next = screen(buf, next);
			if(next){
				buf[next] = 0;
				if(mode)
					print("[%d,%d]: *%s*\n", row, col, buf);
				else
					print("[%d,%d]: %s\n", row, col, buf);
				didoutput = 1;
				next = 0;
			}
		} else {
			buf[next++] = c;
			if(next+1 == sizeof buf)
				goto flush;
		}
		break;
	case ESC:
		switch(c)
		{
		case '[':
			state = NUM1;
			num = 0;
			break;
		case '=':
			state = IDLE;
			break;
		default:
			fprint(2, "*************ESC %c\n", c);
			state = IDLE;
			break;
		}
		break;
	case NUM1:
		switch(c)
		{
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			num = 10*num+c-'0';
			break;
		case ';':
			row = num;
			num = 0;
			state = NUM2;
			break;
		case 'H':	/* short address? */
			row = num;
			state = IDLE;
			break;
		case 'm':	/* mode select */
			mode = num;
			state = IDLE;
			break;
		case 'K':	/* line erase */
		case 'J':	/* screen erase */
			state = IDLE;
			break;
		default:
			fprint(2, "***********NUM1 error 0%o '%c'\n", c, c);
			state = IDLE;
		}
		break;
	case NUM2:
		switch(c)
		{
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			num = 10*num+c-'0';
			break;
		case 'H':
			col = num;
			state = IDLE;
			break;
		default:
			fprint(2, "**********NUM2 error 0%o '%c'\n", c, c);
			state = IDLE;
		}
		break;
	}
}

screen(buf, n)
	char *buf;
{
	if(n == 8){
		if((buf[2] == ':') && (buf[5] == ':'))
			return(0);
	}
	return(n);
}

kbd(c)
{
	switch(c)
	{
	case '*':
		while(read(0, &c, 1) == 1)
			if(c == '\n') break;
		exit(0);
	case '\\':
		c = '\r';
	default:
		fprint(vswfd, "%c", c);
		break;
	}
}
