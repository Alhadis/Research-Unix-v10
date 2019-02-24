# include "runup.h"
# include "sys/types.h"
# include "sys/timeb.h"
# include "time.h"
# include "signal.h"
char *Master = "/usr/dict/papers";
char *newplace = "other";
char rundir[50];
int verbose=0;
int chatty=0;
int vhash=0;
long start;
extern int garbage();
main(argc,argv)
	char *argv[];
{
int (*oldsig)();
/* this updates an index and files directly */
oldsig=signal(SIGINT, garbage);
if (oldsig==SIG_IGN) signal(SIGINT, SIG_IGN);
chatty = isatty(0);
while (argc>1)
	{
	if (argv[1][0]=='-')
	switch(argv[1][1])
		{
		case 'p': /* switch default index set */
			Master = argv[2];
			argc-= 2;
			argv+=2;
			continue;
		case 'n': /* new place for additions */
			newplace = argv[2];
			argc -= 2; argv += 2;
			continue;
		case 'v':
			verbose = chatty = 1;
			vhash = atoi(argv[1]+2);
# if D2
			printf("vhash %d\n",vhash);
# endif

			argc--; argv++;
			continue;
		case 'c':
			chatty = 1;
			if (argv[1][1]=='0') chatty=0;
			argc--;argv++;
			continue;
		}
	break;
	}
	if (verbose)
		{start=time(0); printf("start at %s", ctime(&start));}
getpwd(rundir);
	if (verbose) printf("past getpwd got %.1f of %d sec.\n", tuse(), tlapse());
readbib();
if (bibs[0].bibfno<0 && argc<=1 ) {printf("No work\n"); exit(0);}
	if (verbose) printf("past readbib got %.1f of %d sec.\n", tuse(), tlapse());
lock();
sortbib();
	if (verbose) printf("past sortbib got %.1f of %d sec.\n", tuse(), tlapse());
assert(chdir(Master)==0);
	if (verbose) printf("past chdir got %.1f of %d sec.\n", tuse(), tlapse());
copysource();
	if (verbose) printf("past copysource got %.1f of %d sec.\n", tuse(), tlapse());
makenew(argc,argv);
	if (verbose) printf("past makenew got %.1f of %d sec.\n", tuse(), tlapse());
append(argc,argv);
	if (verbose) printf("past append got %.1f of %d sec.\n", tuse(), tlapse());
fixic();
	if (verbose) printf("past fixic got %.1f of %d sec.\n", tuse(), tlapse());
fixib();
	if (verbose) printf("past fixib got %.1f of %d sec.\n", tuse(), tlapse());
signal(SIGINT, SIG_IGN); /* too late-better finish rather than abort */
remove();
unlock();
	if (verbose) printf("past remove got %.1f of %d sec.\n", tuse(), tlapse());
	if (chatty) printf("Done\n");
}

getpwd(s)
	char *s;
{
int fpip[2], fr, fw;
pipe(fpip);
fr = fpip[0]; fw = fpip[1];
if (fork()==0)
	{
	close(1);
	close(fr);
	assert(dup(fw)==1);
	close(fw);
	execl ("/bin/pwd", "pwd", 0);
	execl ("/usr/bin/pwd", "pwd", 0);
	assert(0);
	}
close(fw);
wait(0);
read(fr, s, 100);
close(fr);
trim(s);
assert(s[0]=='/');
}

trim(s)
	char *s;
{
while (*s) s++;
if (*--s=='\n') *s=0;
}

double
tuse()
{
struct tbuffer {
	long proc_user;
	long proc_sys;
	long child_user;
	long child_sys;
	} ;
struct tbuffer tb;
double t;
times(&tb);
t = (tb.proc_user+tb.proc_sys+tb.child_user+tb.child_sys);
return(t/60.0);
}

tlapse()
{
return( (int) (time(0)-start));
}
char lockf[100];
lock()
{
int f;
sprintf(lockf, "%s/pub-lock.n", Master);
f = creat(lockf, 000);
if (f<0)
	{
	fprintf(stderr, "Someone else updating\n");
	exit(1);
	}
close(f);
}
unlock()
{
unlink(lockf);
}
