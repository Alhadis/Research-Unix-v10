#include	<stdio.h>

/*
 *  gcat -- spooler to send troff output to GCOS phototypesetter
 */

#define	DPR	0
#define	FGET	0
#define	FSEND	0
#define	GCAT	1
#define	LPR	0

#define	NAME	"gcat"
#define	BF	'B'
#define	MAXCOPY	4096000L
#define FIRSTCHAR 'G'-1

char	cfname[]	= "/usr/spool/dpd/cfFXXXXX";
char	dfname[]	= "/usr/spool/dpd/dfFXXXXX";
char	lfname[]	= "/usr/spool/dpd/lfFXXXXX";
char	tfname[]	= "/usr/spool/dpd/tfFXXXXX";
char	zfname[]	= "/usr/spool/dpd/zfFXXXXX";

#include	"spool.c"

int photo, stare, text, dummy;
char *string, *str;

main(agc, agv)
int agc;
char *agv[];
{

	argc = agc;    argv = agv;
	pidfn();
	remotsw = TOSS;		/*default is to throw out accounting. MRW*/
	wantmail = 1;		/*default is send mail.*/
	grade = '1';

	while (argc>1 && (arg = argv[1])[0]=='-') {
		if(comopt(arg[1])) ;
		else switch (arg[1]) {

		case 'd':
			if( arg[2]=='u' ) { dummy=1;  remotsw = ONL; }
			else goto unknown;
			break;

		case 'f':		/*option to set filename for mail. MRW*/
			mailfile = getarg('f');
			wantmail = 1;
			break;

		case 'p':
			if( arg[2]=='h' ) photo=1;
			else goto unknown;
			break;

		case 's':
			if( arg[2]=='t' ) stare=1;
			else goto unknown;
			break;

		case 't':
			if( arg[2]=='x' ) { text=1;  remotsw = ONL; }
			else if(!arg[2])
				remotsw = TOSS;
			    else goto unknown;
			break;

		case 'u':
			identf = getarg(arg[1]);
			break;

unknown:
		default:
			fprintf(stderr, "%s: Unrecognized option: %s\n", NAME, arg);
			break;
		}
		argc--;
		argv++;
	}

	spool1();		/*setup initial GCOS cards.*/

	filargs();		/*process file arguments.*/

	if( photo || stare || text || dummy ) ;
	else photo=1;

	if(gcdebug)
		card('L', "$	select	cc/test/troff");
	else
		card('L', "$	select	cc/troff/select");
	card('L', "$	file	ot,x1r");
	card('L', "$	data	d*");
	str = string = " :trview ot*  -xx -xx -xx -xx";
	str += 15;
	if( photo ) {
		*str = 'p'; *(str+1) = 'h';
		str += 4;
	}
	if( stare ) {
		*str = 's'; *(str+1) = 't';
		str += 4;
	}
	if( text ) {
		*str = 't'; *(str+1) = 'x';
		str += 4;
	}
	if( dummy ) {
		*str = 'd'; *(str+1) = 'u';
		str += 4;
	}
	*(str-2) = '\0';
	card('L', string);

	spool2();		/*put out final cards, and spool job.*/
	out();
}

nuact()
{

	if(gcdebug)
		card('L', "$	select	cc/test/gcat");
	else
		card('L', "$	select	cc/unixsupport/gcat");
}


archive()
{
}
