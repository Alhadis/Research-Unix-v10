/* aread.c: read and parse an airspace file */

#include <stdio.h>
#include <sys/ttyio.h>
#include <sys/param.h>
#include <sys/timeb.h>
#include <ctype.h>
#include <signal.h>
#include "ahdr.h"

/* The following section reads a file containing info on the
 * current airspace.  A sample looks like:
Apple1
	size: 15x24
	airway: 1=(0,13) [20-30] SE 8=(10,23) [10-5]
	airway: 0=(4,0) S 9=(4,23)
	airway: 2=(14,15) NW 7=(0,1)
	airway: 3=(0,9) NE 6=(9,0)
	airway: 4=(14,7) SW 5=(0,21)
	airport: %=(4,11) S
	airport: #=(10,11) NE
	navaid: *=(4,5)
	navaid: *=(4,17)
 * where the airspace id starts in the first column, and everything else
 * is indented one tab and has syntax exactly as shown.
 * Numbers in square brackets are proportion of entries-exits from-to this
 * point.  Default for normal places is 10 and default for airports is 16.
 */

/* note: [prob] code is obsolete */

#define I_SIZE 1        /* these are the tokens for the airspace identifiers*/
#define I_AIRWAY 2
#define I_AIRPORT 3
#define I_NAVAID 4

struct namestr
{       char *i_name;
	int i_token;
} name[] =
{       "size",         I_SIZE,
	"airway",       I_AIRWAY,
	"airport",      I_AIRPORT,
	"navaid",       I_NAVAID,
	0
};

extern int npaths;      /* number of paths in the path file */
extern int maxflow;
extern struct flow fpath[MAXPATHS];

#define BUFSIZE 100

extern struct pstruct entry[EMAX],airport[AMAX],navaid[NMAX];
extern char *airspace,*airfile,*flowfile;
extern int height,width;
extern char screen[MAXWIDTH][MAXHEIGHT];
extern int nentry,nairport,nnavaid;
extern int inawtot,inaptot,outawtot,outaptot;

char *strchr();

readspace()     /* read the airspace file to get user's desired airspace */
{       FILE *asfile;
	char lbuf[BUFSIZE]; /* line from the airspace file */
	char ibuf[BUFSIZE];  /* airspace identifier */
	char *c;
	struct namestr *n;
	int i,org,dst;
	FILE *flowstream;
	struct flow *path;

	if ((asfile = fopen(airfile,"r")) == NULL)
		err("Cannot open airspace file, %s\n",airfile);
	do      /* scan through the file until we get the right airspace */
	{       if (fgets(lbuf,BUFSIZE,asfile) == NULL)
		    err("Couldn't find airspace %s in %s\n",airspace,airfile);
		if (lbuf[0] == '\t') continue; /* not an airspace id */
		for (c=lbuf; *c; c++) if (*c == '\n') *c = 0; /*kill newline*/
	}
	while (strcmp(lbuf,airspace));
	/* next consider each line of this airspace in turn */
	while (fgets(lbuf,BUFSIZE,asfile) != NULL & lbuf[0] == '\t')
	{       for (c=lbuf; *c == '\t' || *c == ' '; c++); /* skip blanks */
		strcpy(ibuf,c);
		if ((c = strchr(ibuf,':')) == 0)
			err("No colon in %s, line %s",airfile,lbuf);
		*c = 0; /* terminate the command id */
		for (n = name; n->i_name != 0; n++) /* get symbol */
			if (strcmp(ibuf, n->i_name) == 0) break;
		switch(n->i_token)
		{   case I_SIZE: sizeread(lbuf); break;
		    case I_AIRWAY: airwayread(lbuf); break;
		    case I_AIRPORT: apread(lbuf); break;
		    case I_NAVAID: navaidread(lbuf); break;
		    default: err("Unrecognized id in %s, line %s",
				airfile,lbuf);
		}
	}
	fclose(asfile);
	/* now put commas on all the airways */
	for (i=0; i<nentry; i++) /* go both ways -- doubles the work */
	{       int x,y,dx,dy;

		x = entry[i].p_x;
		y = entry[i].p_y;
		dx = entry[i].p_dx;
		dy = entry[i].p_dy;
		while (x>=0 && y>=0 && x<width && y<height)
		{       if (screen[x][y] == '.') screen[x][y] = ',';
			x += dx;
			y += dy;
		}
	}
	if (!flowfile)  /* if no flow file specified, look for one */
	{       char *s,*t;
		static char flowbuf[80];
		FILE *test;

		if (*(t = airfile) == '/')      /* full airfile path? */
		{       for (s = flowbuf; *s++ = *t++; ); /* copy airfile */
			while (*--s != '/'); /* back to last slash */
			s++;
		}
		else    s = flowbuf;
		for (t = airspace; *t; *s++ = *t++);/*air space*/
		for (t = ".flow"; *s++ = *t++; );
		if ((test = fopen(flowbuf,"r")) != NULL) /* it's there */
		{       fclose(test);
			flowfile = flowbuf;
		}
	}
	if (flowfile)   /* flow control */
	{       maxflow = 0;
		if ((flowstream = fopen(flowfile,"r")) == NULL)
			err("Cannot open flow control file: %s\n",flowfile);
		path = &fpath[npaths = 0];
		while (fgets(lbuf,BUFSIZE,flowstream) != NULL)
		{       if (lbuf[0] == '\n') continue; /* ignore newlines */
			org = *(c = lbuf);      /* 1st char is origin */
			if (*++c != '-')
				err("Path format: x->y 123\nYou gave me %s\n",
					lbuf);
			if (*++c != '>')
				err("Path format: x->y 123\nYou gave me %s\n",
					lbuf);
			dst = *++c;
			/* we need to code these in their flightpath() fmt */
			pcode(org,path,1);
			pcode(dst,path,0);
			path->f_freq = atoi(++c);       /* get relative freq*/
			maxflow += path->f_freq;        /* total frequency */

			/* read distances for minimal paths */
			while (*++c) if (*c == ' ') break; /* next fld */
			path->f_cmds = (*c == 0? 1 : atoi(c));
			if (*c) while (*++c) if (*c != ' ') break;
			if (*c) while (*++c) if (*c == ' ') break;
			path->f_dist[0] = (*c == 0? 1 : atoi(c));
			for (i=1;i<7;i++) path->f_dist[i] = path->f_dist[0];
			for (i = 7; i < NALT; i++)
			{       if (*c) while (*++c) if (*c != ' ') break;
				if (*c) while (*++c) if (*c == ' ') break;
				if (*c) path->f_dist[i] = atoi(c);
				else path->f_dist[i] = path->f_dist[i-1];
			}

			path++;
			npaths++;
		}
		fclose(flowstream);
	}
}

pcode(c,path,from)   /* find flightplan() entry/exit code for character c */
char c;
struct flow *path;
int from;
{       int i;

	for (i=0; i<nentry; i++)
		if (c == entry[i].p_sym) break;
	if (i<nentry)
	{       if (from)
		{       path->f_from = i;
			path->f_fair = 0;
		}
		else
		{       path->f_to = i;
			path->f_tair = 0;
		}
		return;
	}
	for (i=0; i<nairport; i++)
		if (c == airport[i].p_sym) break;
	if (i<nairport)
	{       if (from)
		{       path->f_from = i;
			path->f_fair = 1;
		}
		else
		{       path->f_to = i;
			path->f_tair = 1;
		}
		return;
	}
	err("Unrecognized point %c in path file.\n",c);
}


sizeread(buf)
char *buf;
{       char *c;

	c = strchr(buf,':'); /* previously checked for existence */
	width = atoi(++c);
	if ((c = strchr(buf,'x')) == 0)
		err("A size line must be of the form size: 15x25");
	height = atoi(++c);
	if (width > MAXWIDTH || width < MINWIDTH)
		err("Width must be in the range %d to %d",MINWIDTH,MAXWIDTH);
	if (height > MAXHEIGHT || height < MINHEIGHT)
	     err("Height must be in the range %d to %d",MINHEIGHT,MAXHEIGHT);
}


char *
getpoint(buf,p) /* read an entry point, airport, or beacon from file */
char *buf;
struct pstruct *p;
{       char *c;
	int i;

	c = strchr(buf,'=');
	if (c == 0) return(0);
	p->p_sym = *--c;
	c = strchr(c,'(');
	if (c == 0) return(0);
	p->p_x = atoi(++c);
	c = strchr(c,',');
	if (c == 0) return(0);
	p->p_y = atoi(++c);
	c = strchr(c,')');
	if (c == 0) return(0);
	screen[p->p_x][p->p_y] = p->p_sym;
	++c;    /* space or return after ) */
/* note: [prob] code is obsolete */
	if (*(c+1) == '[') /* if next char is [, then we have a proportion */
	{       p->p_inprop = atoi(c+2);
		c = strchr(c,'-');
		if (c == 0) return(0);
		p->p_outprop = atoi(++c);
		c = strchr(c,']');
		if (c == 0) return(0);
		c++;
	}
	return(c);
}

char *
getdirec(c,p)
char *c;
struct pstruct *p;
{       while (*c != ' ' && *c && *c != '\n')
	{       switch(*c++)
		{   case 'N':
			p->p_dy = -1;
			break;
		    case 'S':
			p->p_dy = 1;
			break;
		    case 'E':
			p->p_dx = 1;
			break;
		    case 'W':
			p->p_dx = -1;
			break;
		    default : return(0);
		}
	}
	return(c);
}

airwayread(buf)
char *buf;
{       char *c;
	struct pstruct *p,*q;

	p = &entry[nentry++];
	p->p_inprop = p->p_outprop = 10;
	c = getpoint(buf,p);
	inawtot += p->p_inprop;
	outawtot += p->p_outprop;
	if (c == 0) err("Airway syntax error");
	c = strchr(c,' ');       /* now get direction */
	if (c == 0) err("Airway syntax error");
	p->p_dx = p->p_dy = 0;
	c = getdirec(++c,p);
	if (c == 0) err("Airway direction error");
	q = &entry[nentry++];
	q->p_inprop = q->p_outprop = 10;
	c = getpoint(c,q);
	inawtot += q->p_inprop;
	outawtot += q->p_outprop;
	if (c == 0) err("Airway syntax error");
	q->p_dx = -p->p_dx;       /* reverse direction from other entry */
	q->p_dy = -p->p_dy;
}

apread(buf)
char *buf;
{       char *c;
	struct pstruct *p;

	p = &airport[nairport++];
	p->p_inprop = p->p_outprop = 26;
	c = getpoint(buf,p);
	inaptot += p->p_inprop;
	outaptot += p->p_outprop;
	if (c == 0) err("Airport syntax error");
	p->p_dx = p->p_dy = 0;
	c = getdirec(++c,p);
	if (c == 0) err("Airport direction syntax error");
}

navaidread(buf)
char *buf;
{
	if (getpoint(buf,&navaid[nnavaid++]) == 0)
		err("Navaid syntax error");
}



