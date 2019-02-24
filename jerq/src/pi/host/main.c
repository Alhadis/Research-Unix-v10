#include "termcore.h"
#include "master.pri"
#include "format.pri"
#include <CC/signal.h>
SRCFILE("main.c")

void ErrExit(char *e)
{
	fprintf(stderr, "%s\n", e);
	exit(1);
}

#define SIZE 4096
void Copy(int from, int to)
{
	char buf[SIZE];

	for( ;; ) write(to, buf, read(from, buf, SIZE));
}

void ReadWrite(char *dev)
{
	Remote *r = new Remote(dev);
	if( fork() )
		Copy( r->fd, 1 );
	else
		Copy( 0, r->fd );
}

void KillGroup(int minutes)
{
	sleep(minutes*60);
	::kill(0, SIGKILL);
}

Remote *Share(char *comment = "")
{
	Remote *r = new Remote("/dev/tty");
	CoreVersion(r, comment);
	r->share();
	return r;
}

void Spawn(char *cmd)
{
	Remote *r = Share();
	r->pktstart(C_SPER);
	r->sendlong(getpid());
	r->sendstring(cmd);
	r->pktflush();
	KillGroup(10);
}

void Mail(char *to, char *cmd )
{
	FILE *fp, *Popen(char*,char*);
	if( fp = Popen(sf("/bin/mail %s", to), "w") )
		fprintf( fp, "%s\n", cmd );
	if( !fp || Pclose(fp)==-1 )
		ErrExit( "cannot send mail" );
	Share( sf("3pi: waiting for %s...", to) );
	KillGroup(24*60);
}

char *SYS()
{
	char *sysfile = "/etc/whoami";
	static char sys[32];
	if( !sys[0] ){
		FILE *fp = fopen(sysfile, "r");
		if( !fp || !fgets(sys, sizeof sys -1, fp) )
			ErrExit( "cannot determine host name" );
		fclose(fp);
		sys[strlen(sys)-1] = '\0';
	}
	return sys;
}

int nametty(int,char*);
char *getenv(char*), *Getwd();
char *CD;
char *COREDEV;
char *COREHOST;
char *DEVKMEM = "/dev/kmemr";
char *HOME;
char *MUXTERM = "/usr/jerq/lib/muxterm";
char *PADSTERM;
char *PIHOST = "/usr/jerq/bin/3pi";
char *PITERM = "32ld /usr/jerq/mbin/pi.m";
char *SNETROM = "/usr/snet/rom";
char *SNETSYMS = "/usr/gerard/snet/sys/sys.out";
char *TAPTO;
char *UNIX = "/unix";
char *WERKSYMS;

void LoadTerm()
{
	if( PadsInit(PADSTERM) )
		ErrExit("cannot load terminal");
}

Remote *NewRemote()
{
	if( COREHOST ){
		printf("connecting to %s %s...", COREHOST, COREDEV);
		fflush(stdout);
		char *server = sf( "%s -r %s", PIHOST, COREDEV );
		int fd = ipcexec(COREHOST, "light", server);
		if( fd<0 ) {
			extern char *errstr;
			ErrExit(errstr ? errstr : "ipcexec failed");
		}
		return new Remote(fd);
	} else
		return new Remote(COREDEV);
}

void mainbatch(char **av)
{
	char *core = "core", *aout = "a.out";
	if( *av ) core = *av++;
	if( *av ) aout = *av++;
	new BatchMaster(core, aout);
	exit(0);
}

void mainpi(char **av)
{
	if( !strcmp(av[0],"-t") )
		mainbatch(av+1);
	LoadTerm();
	extern char *TapTo;
	TapTo = TAPTO;
	NewHelp();
	NewWd();
	new HostMaster();
	PadsServe();
}

void mainspi()
{
	LoadTerm();
	NewHelp();
	NewWd();
	new SnetMaster();
	PadsServe();
}

void mainwpi()
{
	LoadTerm();
	NewHelp();
	NewWd();
	new WerkMaster();
	PadsServe();
}

void main3pi(char **av)
{
	Remote *r;
	if( COREDEV ){
		r = NewRemote();
		LoadTerm();
		NewHelp();
		NewWd();
		new TermMaster(r);
		PadsServe();
	}
	char *programmer = 0;
	while( *av ){
		if( eqstr(*av, "-r") && *++av )
			ReadWrite(*av);
		else if( eqstr(*av, "-p") && *++av )
			programmer = *av++;
		else
			ErrExit("use: 3pi [-p programmer]" );
	}
	COREDEV = sf("/tmp/3pi.%d", getpid());
	if( creat(COREDEV, 0666) < 0 )
		ErrExit("cannot create tty file");
	if( nametty(0, COREDEV) != 1 )
		ErrExit("cannot name tty");
	if( system(PITERM) )
		ErrExit("cannot load terminal agent");
	Bls spawn;
	if( PADSTERM )
		spawn.af( "PADSTERM=\"%s\" ", PADSTERM );
	spawn.af( "PIHOST=%s COREDEV=%s MUXTERM=%s UNIX=%s DEVKMEM=%s %s",
			PIHOST, COREDEV, MUXTERM, UNIX, DEVKMEM, PIHOST );
	if( programmer )
		Mail(programmer, sf("COREHOST=%s %s", SYS(), spawn.text) );
	Spawn( sf("%s %s", CD, spawn.text) );
}

void main(int, char **av)
{
	char *e;
	if( e = getenv("COREHOST") ) COREHOST = e;
	if( e = getenv("COREDEV") ) COREDEV = e;
	if( e = getenv("MUXTERM") ) MUXTERM = e;
	if( e = getenv("PADSTERM") ) PADSTERM = e;
	if( e = getenv("PIHOST") ) PIHOST = e;
	if( e = getenv("PITERM") ) PITERM = e;
	if( e = getenv("UNIX") ) UNIX = e;
	if( e = getenv("DEVKMEM") ) DEVKMEM = e;
	if( e = getenv("HOME") ) HOME = e;
	if( e = getenv("SNETSYMS") ) SNETSYMS = e;
	if( e = getenv("SNETROM") ) SNETROM = e;
	if( e = getenv("WERKSYMS") ) WERKSYMS = e;
	CD = sf( "builtin cd %s;", Getwd() );
//	if( HOME ) TAPTO = sf("%s/.pilog", HOME);
//	if( e = getenv("TAPTO") ) TAPTO = e;

	char *base = basename(*av++);
	if( !strcmp(base,"3pi") )
		main3pi(av);
	else if( !strcmp(base,"spi") )
		mainspi();
	else if( !strcmp(base,"wpi") )
		mainwpi();
	else
		mainpi(av);
}
