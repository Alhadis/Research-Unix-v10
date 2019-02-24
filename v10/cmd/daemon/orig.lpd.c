#
/*
 * lpd -- line printer daemon dispatcher
 *
 */

#define	SPIDER	0
#define	PHONE	0
#define	LPD	1

char	dpd[]	= "/usr/spool/lpd";
char	dfname[30] = "/usr/spool/lpd/";
char	lock[]	= "/usr/spool/lpd/lock";

#include	"daemon.c"

/*
 * The remaining part is the line printer interface.
 */

char	banbuf[64];

dem_con()
{
	return(0);
}

dem_dis()
{

}

dem_open(file)
char	*file;
{
	banbuf[0] = 0;
}

dem_close()
{
}

get_snumb()
{
}

lwrite()
{
	register char *p1, *p2;

	p1 = line+1;
	p2 = banbuf;
	while (*p2++ = *p1++);
}


sascii(fff)
{
	int p;

	if (p = fork()) {
		if (p == -1)
			return(1);
		wait(&p);
		return(p);
	}
	if (banbuf[0]) {
		execl("/usr/bin/vpr", "vpr", "-b", banbuf, line+1, 0);
		return(1);
	}
	execl("/usr/bin/vpr", "vpr", line+1, 0);
	return(1);
}

trouble(s, a1, a2, a3, a4)
char	*s;
{
	if(retcode > 0){
		FCLOSE(dfb);
	}
	longjmp(env, 1);
}

logerr()
{
}

getowner()
{
}

maildname()
{
	fprintf(pmail, "Your printer job is done.\n");
}

