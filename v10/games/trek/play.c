# include	"trek.h"

/**
 **	instruction read and main play loop
 **/

CVNTAB	Comtab[]
{
	"!",			"shell",
	"m",			"ove",
	"ram",			"",
	"q",			"uery",
	"st",			"atus",
	"sh",			"ield",
	"s",			"rscan",
	"l",			"rscan",
	"da",			"mages",
	"p",			"hasers",
	"quit",			"",
	"t",			"orpedo",
	"cl",			"oak",
	"c",			"omputer",
	"w",			"arp",
	"r",			"est",
	"do",			"ck",
	"i",			"mpulse",
	"u",			"ndock",
	"help",			"",
	"v",			"isual",
	"destruct",		"",
	"abandon",		"",
	"dump",			"",
	"events",		"",
	0
};

extern int shell();
extern int move();
extern int ram();
extern int info();
extern int status();
extern int shield();
extern int srscan();
extern int lrscan();
extern int dcrept();
extern int phaser();
extern int reset();
extern int torped();
extern int cloak();
extern int computer();
extern int setwarp();
extern int rest();
extern int dock();
extern int impulse();
extern int undock();
extern int help();
extern int visual();
extern int destruct();
extern int abandon();
extern int dumpgame();
extern int eventpr();

int (*comfn[])()
{
	&shell,
	&move,
	&ram,
	&info,
	&status,
	&shield,
	&srscan,
	&lrscan,
	&dcrept,
	&phaser,
	&reset,
	&torped,
	&cloak,
	&computer,
	&setwarp,
	&rest,
	&dock,
	&impulse,
	&undock,
	&help,
	&visual,
	&destruct,
	&abandon,
	&dumpgame,
	&eventpr
};

reset()
{
	longjmp(errjmp);
}

play()
{
	register int	r;

	while (1)
	{
		Move.free = 1;
		Move.delta = 0.0;
		Move.shldchg = 0;
		Move.newquad = 0;
		Move.resting = 0;
		if(chkbad() < 0)
			return(0);
		if((r=getcodpar("Command", Comtab))<0)
			continue;
		(*comfn[r])();
		if(chkbad())
			continue;
		events();
		attack(0);
		checkcond();
	}
}

char	eof;
chkbad()
{
	if(eof)
		return(-1);
	if(mkfault) {
		flushin();
		mkfault=0;
		printf("@\n");
		return(1);
	}
	return(0);
}
