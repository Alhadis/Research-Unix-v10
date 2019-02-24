#include <pads.pri>
#include <CC/signal.h>
SRCFILE("popen.c")
static int popen_pid[_NFILE];

int pipe(int[2]), wait(int*), vfork(), _exit(int);

FILE *Popen(char *cmd, char *mode)
{
	int parent = (*mode == 'r') ? 0 : 1;
	int child  = (*mode == 'r') ? 1 : 0;
	int p[2];

	if( pipe(p) < 0 ) return NULL;
	int pid = vfork();
	if( pid == 0) {
		dup2(p[child], child);
		setuid(getuid());
		setgid(getgid());
		for( int i = 0; i < _NFILE; ++i )
			if( i != child ) close(i);
		execl("/bin/sh", "sh", "-c", cmd, 0);
		_exit(1);
	}
	if(pid == -1)
		return NULL;
	close(p[child]);
	popen_pid[p[parent]] = pid;
	return(fdopen(p[parent], mode));
}

int Pclose(FILE *ptr)
{
	static SIG_TYP stat[4];
	static int sig[4] = { SIGINT, SIGQUIT, SIGHUP, SIGPIPE };
	int f, r, status, i;

	f = fileno(ptr);
	fclose(ptr);
	for( i = 0; i < 4; ++i )
		stat[i] = signal(sig[i], (SIG_ARG_TYP)SIG_IGN);
	while((r = wait(&status)) != popen_pid[f] && r != -1) {}
	if(r == -1)
		status = -1;
	for( i = 0; i < 4; ++i )
		signal(sig[i], (SIG_ARG_TYP)stat[i]);
	return(status);
}
