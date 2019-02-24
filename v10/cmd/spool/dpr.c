#include	<stdio.h>

/*
 *  dpr -- spooler for off line print via dataphone daemon to GCOS
 */

#define	DPR	1
#define	FGET	0
#define	FSEND	0
#define	GCAT	0
#define	LPR	0

#define	NAME	"dpr"
#define	BF	'F'
#define	MAXCOPY	204800L
#define FIRSTCHAR 'A'-1

char	cfname[]	= "/usr/spool/dpd/cf@XXXXX";
char	dfname[]	= "/usr/spool/dpd/df@XXXXX";
char	lfname[]	= "/usr/spool/dpd/lf@XXXXX";
char	tfname[]	= "/usr/spool/dpd/tf@XXXXX";
char	zfname[]	= "/usr/spool/dpd/zf@XXXXX";

#include	"spool.c"

int	indent	= 8;		/*amount to indent line on printer. MRW*/

main(agc, agv)
int agc;
char *agv[];
{
	argc = agc;    argv = agv;
	pidfn();
	remotsw = ONL;
	wantmail = 1;		/*default is send mail.*/
	grade = '2';

	while (argc>1 && (arg = argv[1])[0]=='-') {
		if(arg[1] != 's' && arg[2] && !arg[3]){
			remote[12] = arg[1];
			remote[13] = arg[2];
			remote[14] = 0;
		} else if(comopt(arg[1])) ;
		else switch (arg[1]) {

		case '-':
			remote[12] = 'r';
			remote[13] = '1';
			remote[14] = '\0';
			break;

		case 'f':		/*option to set filename for mail. MRW*/
			mailfile = getarg('f');
			wantmail = 1;
			break;

		case '0':		/*allow indent of 0. MRW*/
			indent = 0;
			break;

		default:
			fprintf(stderr, "%s: Unrecognized option: %s\n", NAME, arg);
			break;
		}
		argc--;
		argv++;
	}

	spool1();		/*setup initial GCOS cards.*/
	if(indent == 0)		/*set GCOS switch 32 for no indent. MRW*/
		card('L', "$	set	32");
	if(gcdebug)
		card('L', "$	select	cc/test/dpr");
	else
		card('L', "$	select	cc/unixsupport/dpr");
	card('L', "$	data	i*,ncksum,copy");

	filargs();		/*process file arguments.*/
	card('L', "$	endcopy");

	spool2();		/*put out final cards, and spool job.*/
	out();
}


archive()
{
}


nuact()
{
}
