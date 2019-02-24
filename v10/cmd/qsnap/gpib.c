/*
 * This software is provided solely for use with
 * the National Instruments GPIB11-series interfaces.
 *
 * Copyright 1980, 1983 National Instruments
 *
 * Jeffrey Kodosky
 * REV D:  10/09/83
 */
#include	<sgtty.h>

#define OK	1
#define EIO	5
#define ENOFUN	(-7)
#define ELOOKU	(-100)
#define stty(a,b)	ioctl(a, TIOCSETP, b)
#define gtty(a,b)	ioctl(a, TIOCGETP, b)

static int ibfd= -1, timo= 10;
static char rmd= -1, eod= 0, wmd= -1;
static spfn[3];
extern int errno;

gpib(f, a1, a2, a3, a4)
{
	register int x;

	if(ibfd < 0){
		if((ibfd = open("/dev/ib", 2)) < 0)
			return(geterror());
	}
	spfn[0] = f-2;
	switch(f)
	{
	default:
		return ENOFUN;
	case 0:
		spfn[0] = 0;				/* command */
		if(stty(ibfd,spfn) < 0)
		{	printf("hit\n");
			return(geterror());
		}
		if((x = write(ibfd, a1, a2)) < 0)
		{	printf("write returns %d\n", x);
			return geterror();
		}
		return(x);
	case 1:
		if(wmd != a3){				/* write */
			wmd = a3;
			if(setmodes() < 0)
				return(geterror());
		}
		if((x = write(ibfd, a1, a2)) < 0)
			return geterror();
		return(x);
	case 2:
		if((rmd != a3) || (a3&2) && (eod != a4)){	/* read */
			rmd = a3;
			eod = a4;
			if(setmodes() < 0)
				return(geterror());
		}
		if((x = read(ibfd, a1, a2)) < 0)
			return(geterror());
		return(x);
	case 16:					/* spbyte */
		spfn[0]--;
	case 9:				/* setstat, monitor, testsrq */
	case 10:			/* setstat, monitor, testsrq */
	case 13:			/* setstat, monitor, testsrq */
		spfn[1] = a1;
	case 3:				/* transfer, clear, remote */
	case 4:				/* transfer, clear, remote */
	case 5:				/* transfer, clear, remote */
	case 6:				/* local, passctrl */
	case 8:				/* local, passctrl */
		if(stty(ibfd, spfn) < 0)
			return(geterror());
		return(1);
	case 15:					/* status */
		spfn[2] = a2;
		spfn[1] = a1;
		spfn[0]--;
	case 7:				/* ppoll, readcmd */
	case 11:				/* ppoll, readcmd */
		if(stty(ibfd, spfn) < 0)
			return(geterror());
		if(gtty(ibfd, spfn) < 0)
			return(ELOOKU);
		return(spfn[0]);
	case 12:
		timo= a1;				/* setparam */
		if(setmodes() < 0)
			return(geterror());
		return(1);
	case 14:
		if(close(ibfd) < 0)
			return(geterror());	/* finish */
		ibfd = -1;
		timo = 10;
		rmd = 0;
		eod = 0;
		wmd = 0;
		return(1);
	}
}

static
setmodes()
{
	spfn[0] = 10;
	spfn[1] = timo;
	spfn[2] = (wmd&017) | ((rmd&017)<<4) | (eod<<8);
	return(stty(ibfd,spfn));
}

static
geterror()
{
	if((errno != EIO) || (gtty(ibfd,spfn) < 0))
		return(ELOOKU);
	return(spfn[0]);
}
