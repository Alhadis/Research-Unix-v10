#include	<stdio.h>
#include	"../comm.h"
#include	"host.h"

term()
{
	char c;

	(void)read(jerq, &c, 1);
	return(c&0xFF);
}

termn()
{
	register short k;

	k = term() << 8;
	return(k |= term());
}

terms(s)
	register char *s;
{
	while(*s++ = term());
}

send(c)
{
	char buf[1];

	buf[0] = c;
	write(jerq, buf, 1);
	if(debug&&verbose)fprintf(debug, "sent char %d %x\n", c, c);
}

sendn(n)
{
	char buf[2];

	buf[0] = n>>8;
	buf[1] = n;
	write(jerq, buf, 2);
	if(debug&&verbose)fprintf(debug, "sent num %d %x\n", n, n);
}

sends(s)
	char *s;
{
	write(jerq, s, strlen(s)+1);
	if(debug&&verbose)fprintf(debug, "sent str '%s'\n", s);
}

lump(s, e)
	char *s, *e;
{
	int n;
	char *lasts = s, *origs = s;

	while(s < e)
	{
		if(s-origs > 255)
		{
			if(lasts == origs) quit("BAD PACKET!!", 1);
			return(lasts-origs);
		}
		lasts = s;
		switch(*s++&0xFF)
		{
		case C_BSIZE:
			s++; break;
		case C_ABSH:
			s += 2; break;
		case C_ABSV:
			s += 2; break;
		case C_DEV:
			switch(*s++)
			{
			case RES:
				s += 2; break;
			case FONT:
				for(s++; *s++; )
					if(s-origs > 255) break;
				break;
			case MODE:
				s++; break;
			}
			break;
		case C_DRAW:
			switch(*s++)
			{
			case LINE:
				s += 4; break;
			case CIRCLE:
				s += 2; break;
			case ELLIPSE:
				s += 4; break;
			case ARC:
				s += 8; break;
			case SPLINE:
				n = *s++; s += n*4; break;
			}
		}
	}
	return(origs-s);	/* negative to show we hit end */
}
