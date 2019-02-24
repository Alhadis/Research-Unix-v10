#include	<stdio.h>

/*
 *  fsend -- spooler to send file to GCOS
 */


#define	DPR	0
#define	FGET	0
#define	FSEND	1
#define	GCAT	0
#define	LPR	0

#define	NAME	"fsend"
#define	BF	'B'
#define	MAXCOPY	204800L
#define FIRSTCHAR 'N'-1

char	cfname[]	= "/usr/spool/dpd/cfMXXXXX";
char	dfname[]	= "/usr/spool/dpd/dfMXXXXX";
char	lfname[]	= "/usr/spool/dpd/lfMXXXXX";
char	tfname[]	= "/usr/spool/dpd/tfMXXXXX";
char	zfname[]	= "/usr/spool/dpd/zfMXXXXX";

int	namef	= 0;
char	fname[13] = "pipe.end"  /* gcos file name */;
char	*fmode;
char	*fzf;

#include	"spool.c"

main(agc, agv)
int agc;
char *agv[];
{
	int i;

	argc = agc;    argv = agv;
	pidfn();
	remotsw = TOSS;		/*default is toss out online output*/
	wantmail = 1;		/*default is send mail.*/
	grade = '1';
	fmode = "a";

	while (argc>1 && (arg = argv[1])[0]=='-') {
	    if(!comopt(arg[1]))
		switch (arg[1]) {

		case 'a':
			fmode = "a";
			break;

		case 'b':
			fmode = "b";
			break;

		case 'f':
			arg = getarg('f');
			i = 0;
			while((fname[i] = arg[i]))
				if(++i > 12) {
					fname[i] = '\0';
					break;
				}
			mailfile = fname;
			namef = 1;
			break;

		case 'u':
			arg = getarg('u');
			i = 0;
			while((gcosid[i] = arg[i]))
				if(++i > 12) {
					gcosid[i] = '\0';
					break;
				}
			uidf = 1;
			break;

		case 'x':
			archsw++;
			break;

		default:
			fprintf(stderr, "%s: Unrecognized option: %s\n", NAME, arg);
			break;
		}
		argc--;
		argv++;
	}

	spool1();		/*setup initial GCOS cards.*/
	card('L',"$	select	cc/unixsupport/access");

	filargs();		/*process file arguments.*/

	spool2();		/*put out final cards, and spool job.*/
	out();
}

nuact(file)
char file[];
{
	int c, i, j;

	if(namef)
		namef = 0;
	else {
		i = 0;
		j = 0;
		while((c = file[i++]) != '\0') {
			if(c == '/') { j = 0; continue; }
			if(j >= 12) break;
			fname[j++] = c;
		}
		fname[j++] = '\0';
	}
	if(gcdebug)
		card('L', "$	select	cc/test/fsend.1");
	else
		card('L', "$	select	cc/unixsupport/fsend.1");
	card('L', " ignore errs");
	card('L', sprintf(buf, "fcreat %s/%s,blocks/%d,unlimited/,read,modify/%s/",
			gcosid, fname, gsize, gcosid) );
	card('L', sprintf(buf, "fcreat %s/%s,blocks/1,unlimited/,read,modify/%s/",
			gcosid, fname, gcosid) );
	if(gcdebug)
		card('L', "$	select	cc/test/fsend.2");
	else
		card('L', "$	select	cc/unixsupport/fsend.2");
	card('L', sprintf(buf, "%s %s/%s %ld", fmode, gcosid, fname, usize) );
	card('L', "$	data	i*,ncksum");
}

archive()
{
	card('L', "$      select  cc/unixsupport/arch" );
	card('L', sprintf(buf, "s   %-12s %-12s", gcosid, fname) );
}
