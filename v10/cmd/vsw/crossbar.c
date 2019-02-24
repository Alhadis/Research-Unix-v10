#include	<libc.h>
#include	"crossbar.h"

Crossbar::Crossbar(char *dkname, int n, int depth)
{
	dial = dkname;
	nouts = n;
	nwires = depth;
	fd = -1;
	nextc = 0;
	imap = 0;
}

void
Crossbar::link(int ll, Crossbar *c, int l)
{
	if(nextc)
		fprint(2, "warning: redefining link from crossbar %s\n", dial);
	froml = ll;
	nextc = c;
	nextl = l;
}

int
Crossbar::map(int input, Crossbar **cp, int *ip)
{
	int i, *oip = ip;

	if(fd < 0)
		startup();
	if(imap == 0)
		config();
	for(i = 0; i < nouts; i++)
		if(imap[i] == input){
			if(nextc && (i == froml)){
				int k = nextc->map(nextl, cp, ip);
				while(*cp)
					cp++, ip++;
			} else
				*cp++ = this, *ip++ = i;
		}
	*cp = 0;
	return(ip-oip);
}

void
Crossbar::startup()
{
	fprint(2, "no startup defined!!\n");
	abort();
}

void
Crossbar::config()
{
	fprint(2, "no config defined!!\n");
	abort();
}

int
Crossbar::connect(int, Crossbar *, int, int)
{
	fprint(2, "no connect defined!!\n");
	abort();
	return(0);
}
