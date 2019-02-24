/*
 * Retrieve files from GCOS via Spider or dataphone
 */

#include	<errno.h>
#include	<setjmp.h>

char	dpd[]	= "/usr/spool/dpd";
char	lock[]	= "/usr/spool/dpd/glock";
char	error[]	= "/usr/spool/dpd/errors";

#include	"daemon0.c"

#define MINUTES 60

#define OKTRB 0
#define BADTRB 1
#define FATTRB 2

char	zfname[]	= "/usr/spool/dpd/zf";
int	LZFNAME	= 0;	/*length of pathname of zf-file thru 'f'. MRW */
char	badbuf[]	= "/usr/spool/dpd/fget.badbuf";
char	okmes[] = "OK nd =%3d, nf = %-19d";
char	fgeto[]	= "fget.odemon";
char	*argvo[] = {fgeto, NULL, NULL};

FILE	*ofd = NULL;		/*data file*/
FILE	*tfd = NULL;		/*badbuf file*/
int	tlaps = 0;
int	tlimit = 6*MINUTES;
int	tfac = 1;
int	nbaderr = 0;
int	gotit = 0;
int	nfiles = 0;
int	ndelim = 0;
long	nnull = 0;		/*count of consecutive nulls*/
char	fname[108];
jmp_buf	env;

int	(*output)();


main(argc, argv)

int argc;
char *argv[];

{

int i;
extern	ignore();

	dem_setup();
	LZFNAME = 0;		/*calculate length of zfname. MRW*/
	while(zfname[LZFNAME])  LZFNAME++;

	if (argc == 2) {
		argvo[1] = argv[1];
		tlimit = 0;
		i = 0;
		while ( argv[1][i] ) tlimit = 10*tlimit +  argv[1][i++] - '0' ;
		}

#ifdef	DEBUG
	fprintf(stderr, "tlimit = %d\n",tlimit); /* PRINTF */
#endif

	setjmp(env);

	ofd = NULL;
	output = ignore;

	fgd_read();	/*perform input appropriate to device.*/

	if (gotit == 0)
		trouble(OKTRB, -20, "no output");

	if (gotit == 1) {
		gotit = 2;
		trouble(OKTRB, 20, okmes, ndelim, nfiles);
		}

	trouble(OKTRB, 0, okmes, ndelim, nfiles);
}



/* VARARGS */
trouble(lev, t, s, a1, a2, a3)
int	lev;
int	t;
char	*s;
int	a1, a2, a3;
{

#ifdef	DEBUG
	fprintf(stderr, s, a1, a2, a3); putc('\n', stderr); /* PRINTF */
#endif
	alarm(0);
	dem_dis();	/*close appropriate device.*/
	if ( lev >= BADTRB ) nbaderr++;

	if ( lev <= BADTRB && nbaderr < 2 && t != 0 && tlaps < tlimit ) {
		if (t < 0) t = -t;
		t *= tfac;
		if (t > 4*MINUTES) t = 4*MINUTES;
#ifdef	DEBUG
		fprintf(stderr, "holding, t = %d, tlaps =  %d\n",t,tlaps); /* PRINTF */
#endif
		sleep(t);
		tlaps += t;
		tfac += tfac;
		if(access(lock, 0) == 0)
			longjmp(env, 1);
		logerr("glock has disappeared");
	}

	if(gotit)
		logerr(okmes, ndelim, nfiles);
	if(t >= 0 && lev > OKTRB)
		logerr(s, a1, a2, a3);

	if ( lev >= BADTRB ) {
		sleep(20);
		unlink(lock);
#ifndef DEBUG
#if SPIDER
		execv("/etc/fget.odemon", argvo);
		execv("/usr/lib/fget.odemon", argvo);
#endif
#endif
		}

	unlink(lock);
	exit(0);
}




ignore(b, i)
{
}


outputa(b, i)
char	*b;
int	i;
{
	for ( ; i -- > 0; b++ ) {
		if (*b == '\0') nnull++;
		else {
			for ( ; nnull > 0; nnull -- ) putc( '\0', ofd );
			putc( *b, ofd );
			}
		}
}


outputb(b, i)
char	*b;
int	i;
{
	for ( ; i-- > 0; b++ ) putc( *b, ofd );
}


firstln(b, i)
char	b[];
int	i;
{
	char	*fp, *sp;
	extern	ignore(), outputa(), outputb();

	output = ignore;
	if ( i == 0 ) return;
	b[i-1] = '\0';
	if (i < 104 || b[1] != ' ') {
		logerr("illeg contr line  %s", b);
		return;
		}

	switch (b[0]) {

		case 'a':
		case 'A':
			output = outputa;
			break;

		case 'b':
		case 'B':
			output = outputb;
			break;

		default:
			logerr("illegal control char - %s\n", b);
			return;

		}

	fp = fname;
	sp = &b[2];
	while (*fp++ = *sp++);

#ifdef	DEBUG
	fprintf(stderr, "file = %s\n",fname); /* PRINTF */
#endif

	if ( (ofd = fopen(fname, "w")) == NULL) {
		output = ignore;
		logerr("cannot open: %s",  fname);
		}

	nnull = 0;
}



endfile()
{
int	n;
char	*fp;
extern	firstln();

	gotit = 1;
	ndelim++;
	output = firstln;
	if (ofd == NULL) return;
	FCLOSE(ofd);

	nfiles++;
	nnull = 0;

	n = 0;
	fp = fname;
	while (n  < LZFNAME) {
		if (*fp++ != zfname[n++]) return;
		}

	unlink(fname);


}
