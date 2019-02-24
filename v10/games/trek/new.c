# include "trek.h"

# define	LOGFILE	"/usr/src/games/trek/log"

/**
 **	write out log
 **/

extern long	score();
extern float atof();
CVNTAB	Lentab[]
{
	"s",		"hort",
	"m",		"edium",
	"i",		"ntermediate",
	"l",		"ong",
	0
};

CVNTAB	Skitab[]
{
	"n",		"ovice",
	"f",		"air",
	"g",		"ood",
	"e",		"xpert",
	"c",		"ommodore",
	"i",		"mpossible",
	0
};


char *reason[] {
	"win......",
	"cheating.",
	"destruct.",
	"burned...",
	"too fast.",
	"no help..",
	"suffocate",
	"supernova",
	"nova.....",
	"barrier..",
	"destroyed",
	"energy...",
	"time.....",
	"captured.",
	"no crew..",
};

char b[200];
char *a[20];
float days;

main()
{
	int winlose;
	int k1, k2;
	long s;

	while(getlin()) {
		Game.skill=index(*a[4],"nfgeci");
		Game.length=index(*a[5],"smil");
		days=atof(a[6]);
		Status.ship=last(a[6]);
		sscanf(a[7],"%d/%d",&k1,&k2);
		Game.gkillk = k1;
		Status.kling = k2-k1;
		Game.helps=atoi(a[8]);
		Game.killb=atoi(a[9]);
		Game.kills=atoi(a[10]);
		Game.killinhab=atoi(a[11]);
		Game.deaths=atoi(a[12]);
		winlose=lookup(a[3]);
		Move.endgame=0; Game.killed=0;
		if(winlose==0)
			Move.endgame++;
		else if(winlose<12)
			Game.killed++;

		s=score();
		printf("%s\t%s\t%D\t%s\t%s\t%s\t%.2f%c\t%d/%d\t%d\t%d\t%d\t%d\t%d\n",
			a[0],
			a[1],
			s,
			a[3],
			(Skitab[Game.skill-1].abrev),
			(Lentab[Game.length-1].abrev),
			days,
			Status.ship,
			Game.gkillk, Game.gkillk+Status.kling,
			Game.helps,
			Game.killb,
			Game.kills,
			Game.killinhab,
			Game.deaths
			);
	}
}

long score()
{
	register int		u;
	long			s, t;	/* should really be long */
	double			r;

	Initial.kling=Game.skill*Game.length*5;
	u = Initial.kling*1.05+5;
	if(u>Game.gkillk) u=Game.gkillk;
	Param.klingpwr=100+150*Game.skill+(Game.skill>=6?150:0);
	s = t = Param.klingpwr / 4 * u;;
	r = days;
	if (r < 1.0)
		r = 1.0;
	r = u / r;
	s =+ (t = 400 * r);
	r = Status.kling;
	r =/ u + 1;
	s =+ (t = -1000 * r);
	if (Move.endgame > 0)
	{
		s =+ (t = 100 * (u = Game.skill));
	}
	if (Game.killed)
	{
		s =- 200;
	}
	s =+ (t = -100 * (u = Game.killb));
	s =+ (t = -100 * (u = Game.helps));
	s =+ (t = -20 * (u = Game.kills));
	s =+ (t = -150 * (u = Game.killinhab));
	if (Status.ship != ENTERPRISE)
	{
		s =- 200;
	}
	s =+ (t = 3 * (u = Game.captives));
	s =+ (t = -(u = Game.deaths));
	return (s);
}

getlin()
{
	register char *bp=b, **ap=a, *ip=b;
	char c;

	while(read(0,&c,1)==1) {
		if(c=='\t' || c=='\n') {
			*ap++=ip;
			*bp++ = 0;
			ip=bp;
			if(c=='\n') break;
		} else	*bp++ = c;
	}
	return(c=='\n' ? 1 : 0);
}

index(c,s)
char c;
char *s;
{
	register char *p=s;

	while(*p && c != *p)
		p++;
	return(p-s+1);
}

last(s)
char *s;
{
	register char c;

	while(*s) {
		c = *s; s++;
	}
	return(c);
}

lookup(s)
{
	int r=0;

	while(cf(s,reason[r]))
		r++;
	return(r);
}

cf(s1,s2)
char *s1, *s2;
{
	while(*s1 == *s2 && *s2) {
		s1++; s2++;
	}
	return(*s1 - *s2);
}
