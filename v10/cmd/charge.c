/*
**	Command to change/print costs for Share Scheduling Algorithm
*/

char *	Usage =
"Usage: %s [-C] [-D<normal half-life secs>,<maxnice half-life secs>]\\\n\
	[-E<rate half-life secs>] [-F<octal flags>] [-K<half-life>]\\\n\
	[-P<maxpri>] [-Q<maxupri>] [-R<delta>] [-S<maxusers>]\\\n\
	[-U<maxusage>] [-X<maxushare>] [-Y<mingshare>]\\\n\
	[-b<bio>] [-m<click>] [-s<syscall>] [-t<tic>] [-y<tio>] [%%]\n";

char *	ShowUsage =	"Usage: %s [-[v]]\n";


#include	<sys/types.h>
#include	<sys/lnode.h>
#include	<sys/share.h>
#include	<sys/charges.h>
#include	<stdio.h>
#include	<trace.h>
#include	<math.h>
#include	<libc.h>


char *		Charges		= "charges";
char *		Name;
long		Oldtick;
float		Onef		= 1.0;
float		Oneh		= 100.0;
float		Percent		= 100.0;
int		Traceflag;
int		Verbose;

char		Unk[]		= "<unknown>";
char *		Flags[16] =
{
	"NOSHARE",
	"ADJGROUPS",
	"LIMSHARE",
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk,
	Unk
};

void		error(), getcosts(), printcharges();

#define	NULLSTR	(char *)0
#define	STREQUAL 0
#define	SYSERROR (-1)

#ifdef	EBUG
#define	DEBUG		EBUG	/* for tests */
#define	DODEBUG(A)	A
#else
#define	DODEBUG(A)
#endif	EBUG





main(argc, argv)
	register int		argc;
	register char *		argv[];
{
	register long		n;
	register float		f;
	register char *		cp;
	register long		newdecay = 0;
	register long		pdecay1 = 0;
	register long		pdecay2 = 0;
	register long		rdecay = 0;
	extern char		_sobuf[];

	if ( (Name = strrchr(*argv, '/')) != NULLSTR )
		Name++;
	else
		Name = *argv;
	argv++;
	argc--;

	Oldtick = shconsts.sc_tick;

	if ( strcmp(Name, Charges) == STREQUAL )
	{
		Usage = ShowUsage;
		(void)setbuf(stdout, _sobuf);

		if ( argc == 1 && strcmp(argv[0], "-") == STREQUAL )
		{
			Verbose = 1;
			printcharges(1);
			return 0;
		}

		getcosts();
	}

	while ( argc > 0 )
	{
		int	seconds;

		if ( argv[0][0] != '\0' )
		{
			n = atol(&argv[0][1]);
			f = atof(&argv[0][1]);

			if ( strchr(&argv[0][1], 's') != NULLSTR )
				seconds = 1;
			else
				seconds = 0;
		}

		switch ( argv[0][0] )
		{
		case '\0':	break;
		case '-':	argv[0]++;
				continue;
		case 'C':	getcosts();
				break;
		case 'D':	if ( (cp = strchr(&argv[0][1], ',')) == NULLSTR )
					error("Priority decay specification requires a range of half-lives\n");
				pdecay1 = n;
				pdecay2 = atol(cp+1);
				if ( pdecay1 >= pdecay2 )
					error("The decay half-life for maxnice processs should be less than that for normal processes");
				break;
		case 'E':	rdecay = n;
				break;
		case 'F':	Shareflags = atoo(&argv[0][1]);
				break;
		case 'G':	MAXGROUPS = n;
				break;
		case 'K':	newdecay = n;
				if ( !seconds )
					newdecay *= 60 * 60;
				break;
		case 'P':	MAXPRI = f;
				break;
		case 'Q':	MAXUPRI = f;
				break;
		case 'R':	Delta = n;
				break;
		case 'S':	MAXUSERS = n;
				break;
		case 'T':	Traceflag = n?n:1;
				break;
		case 'U':	MAXUSAGE = f;
				break;
		case 'X':	MAXUSHARE = f;
				break;
		case 'Y':	MINGSHARE = f;
				break;
		case 'b':	shconsts.sc_bio = f;
				break;
		case 'm':	shconsts.sc_click = f;
				break;
		case 's':	shconsts.sc_syscall = f;
				break;
		case 't':	shconsts.sc_tick = f;
				break;
		case 'v':	Verbose = 1;
				break;
		case 'y':	shconsts.sc_tio = f;
				break;
		default:	if ( (n = atol(argv[0])) > 0 )
				{
					Percent = n;
					break;
				}
				error("unrecognised argument %s", argv[0]);
		case '?':	fprintf(stderr, Usage, Name);
				return 1;
		}

		argc--;
		argv++;
	}

	if ( newdecay )
		DecayUsage = pow(0.5, (Onef*Delta)/newdecay);

	if ( pdecay1 )
	{
		Trace(("old PriDecay = %f", PriDecay));
		PriDecay = pow(0.5, Onef/pdecay2);
		Trace((", new PriDecay = %f\n", PriDecay));
		Trace(("old PriDecayBase = %ld", PriDecayBase));
		f = pow(0.5, Onef/pdecay1) / PriDecay;
		PriDecayBase = ((2*NZERO-1) * f - NZERO) / (Onef - f);
		Trace((", new PriDecayBase = %ld\n", PriDecayBase));
	}

	if ( rdecay )
	{
		Trace(("old RateDecay = %f", DecayRate));
		DecayRate = pow(0.5, Onef/((float)rdecay));
		Trace((", new RateDecay = %f\n", DecayRate));
	}


	if ( Percent != Oneh )
	{
		Percent /= Oneh;

		shconsts.sc_bio = shconsts.sc_bio * Percent;
		shconsts.sc_tio = shconsts.sc_tio * Percent;
		shconsts.sc_tick = shconsts.sc_tick * Percent;
		shconsts.sc_click = shconsts.sc_click * Percent;
		shconsts.sc_syscall = shconsts.sc_syscall * Percent;
	}

	if ( Usage == ShowUsage )
		printcharges(0);
	else
	if ( limits((struct lnode *)&shconsts, L_SETCOSTS) == SYSERROR )
		error("L_SETCOSTS");

	return 0;
}



void
getcosts()
{
	if ( limits((struct lnode *)&shconsts, L_GETCOSTS) == SYSERROR )
		error("L_GETCOSTS");
}



char *
shflags(flags)
	u_short		flags;
{
	register char *	sp;
	register int	i;
	register int	j;
	static char	s[200];

	for ( sp = s, i = 1, j = 0 ; flags ; j++, i <<= 1 )
		if ( flags & i )
		{
			flags &= ~i;
			if ( sp != s )
				*sp++ = ',';
			sp += strlen(strcpy(sp, Flags[j]));
		}

	return sp==s ? "<null>" : s;
}



void
printcharges(showdefault)
	int		showdefault;
{
	register float	decay = ((log(.5)/log(DecayUsage))*Delta)/(60*60);

	if ( Percent == Oneh )
	{
		if ( Oldtick != shconsts.sc_tick )
			Percent = (shconsts.sc_tick * Oneh) / Oldtick;
	}
	else
		Percent *= Oneh;

	printf
	(
		"\
Scheduling flags = %s, charging percentage = %g%%,\n\
usage decay rate = %.8f (half-life %.1f %s),\n"
		,shflags(shconsts.sc_fl)
		,Percent
		,DecayUsage
		,decay < Onef ? decay * (60*60) : decay
		,decay < Onef ? "seconds" : "hours"
	);

	printf
	(
		"\
Max. users = %d, max group nesting = %d.\n"
		,MAXUSERS
		,MAXGROUPS
	);

	if ( Verbose && !showdefault )
	printf
	(
		"\
active users = %d, active groups = %d.\n"
		,Users, Groups
	);

	if ( !showdefault )
	{
		register float	itot_charge = Oneh /
			(
			  (float)shconsts.sc_syscallc * (float)shconsts.sc_syscall
			+ (float)shconsts.sc_bioc * (float)shconsts.sc_bio
			+ (float)shconsts.sc_tioc * (float)shconsts.sc_tio
			+ (float)shconsts.sc_tickc * (float)shconsts.sc_tick
			+ (float)shconsts.sc_clickc * (float)shconsts.sc_click
			);

		printf
		(
			"\n\
Charge: syscall %6.0f%%  bio %6.0f%%  tio %6.0f%%  tick %6.0f%%  click %6.0f%%\n"
			,shconsts.sc_syscallc * (shconsts.sc_syscall * itot_charge)
			,shconsts.sc_bioc * (shconsts.sc_bio * itot_charge)
			,shconsts.sc_tioc * (shconsts.sc_tio * itot_charge)
			,shconsts.sc_tickc * (shconsts.sc_tick * itot_charge)
			,shconsts.sc_clickc * (shconsts.sc_click * itot_charge)
		);
	}

	if ( !Verbose )
		return;

	if ( showdefault )
		printf("\n");

	printf
	(
		"\
Costs:  syscall %7lu  bio %7lu  tio %7lu  tick %7lu  click %7lu\n"
		,shconsts.sc_syscall
		,shconsts.sc_bio
		,shconsts.sc_tio
		,shconsts.sc_tick
		,shconsts.sc_click
	);

	if ( !showdefault )
	printf
	(
		"\
Counts: syscall %7lu  bio %7lu  tio %7lu  tick %7lu  click %7lu\n"
		,shconsts.sc_syscallc
		,shconsts.sc_bioc
		,shconsts.sc_tioc
		,shconsts.sc_tickc
		,shconsts.sc_clickc
	);

	printf
	(
		"\n\
Process priority decay rate biased by \"nice\":-\n\
high priority (nice -%2d) %.4f (half-life %5.1f seconds),\n\
avg. priority (nice   0) %.4f (half-life %5.1f seconds),\n\
 low priority (nice  %2d) %.4f (half-life %5.1f seconds).\n\
\n\
Run rate decay rate      %.4f (half-life %5.1f seconds).\n\
\n\
Minimum group share factor	   = %.3f,\n\
maximum user share factor	   = %.3f.\n\
\n\
Max. value for normal usage	   = %.3e,\n\
max. value for normal p_sharepri   = %.3e,\n\
max. value for idle p_sharepri	   = %.3e.\n"
		,NZERO
		,PriDecay*(float)PriDecayBase/(float)(PriDecayBase+2*NZERO-1)
		,log(.5)/log(PriDecay*(float)PriDecayBase/(float)(PriDecayBase+2*NZERO-1))
		,PriDecay*(float)(PriDecayBase+NZERO)/(float)(PriDecayBase+2*NZERO-1)
		,log(.5)/log(PriDecay*(float)(PriDecayBase+NZERO)/(float)(PriDecayBase+2*NZERO-1))
		,NZERO-1
		,PriDecay
		,log(.5)/log(PriDecay)
		,DecayRate
		,log(.5)/log(DecayRate)
		,MINGSHARE
		,MAXUSHARE
		,MAXUSAGE
		,MAXUPRI
		,MAXPRI
	);

	if ( showdefault )
		return;

	printf
	(
		"\n\
High value of current normal usage = %.3e,\n\
high value of current p_sharepri   = %.3e.\n"
		,MaxUsage
		,MaxSharePri
	);
}



/*VARARGS1*/
void
error(a, b)
	char		*a, *b;
{
	extern		errno;
	extern int	sys_nerr;
	extern char	*sys_errlist[];

	fprintf(stderr, "%s: error - ", Name);
	fprintf(stderr, a, b);
	if ( errno )
	{
		if ( errno < sys_nerr )
			fprintf(stderr, " - %s\n", sys_errlist[errno]);
		else
			fprintf(stderr, " - Unknown error %d\n", errno);
	}
	else
		putc('\n', stderr);
	fprintf(stderr, Usage, Name);
	exit(1);
}



/*VARARGS1*/
int
atoo(p)
	register char *	p;
{
	register int	n = 0;
	register int	c;

	while ( (c = *p++) == ' ' || c == '\t' );

	while ( c >= '0' && c <= '7' )
	{
		n = n*8 + c - '0';
		c = *p++;
	}

	return n;
}
