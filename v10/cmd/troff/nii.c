#include "tdef.h"
#include "ext.h"

Stack	*frame, *stk, *ejl;
Stack	*nxf;

int	pipeflg;
int	hflg;	/* used in nroff only */
int	eqflg;	/* used in nroff only */

#ifndef NROFF
int	xpts;
int	ppts;
int	pfont;
int	mpts;
int	mfont;
int	cs;
int	ccs;
int	bd;
#endif

int	stdi;
int	quiet;
int	stop;
char	ibuf[IBUFSZ];
char	xbuf[IBUFSZ];
char	*ibufp;
char	*xbufp;
char	*eibuf;
char	*xeibuf;
Tchar	pbbuf[NC];		/* pushback buffer for arguments, \n, etc. */
Tchar	*pbp = pbbuf;		/* next free slot in pbbuf */
Tchar	*lastpbp = pbbuf;	/* pbp in previous stack frame */
int	nx;
int	mflg;
Tchar	ch = 0;
int	ibf;
int	ttyod;
int	iflg;
int	rargc;
char	**argp;
ushort	trtab[NTRTAB];
int	lgf;
int	copyf;
Offset	ip;
int	nlflg;
int	donef;
int	nflush;
int	nfo;
int	ifile;
int	padc;
int	raw;
int	ifl[NSO];
int	ifi;
int	flss;
int	nonumb;
int	trap;
int	tflg;
int	ejf;
int	dilev;
Offset	offset;
int	em;
int	ds;
Offset	woff;
int	app;
int	ndone;
int	lead;
int	ralss;
Offset	nextb;
Tchar	nrbits;
int	nform;
int	oldmn;
int	newmn;
int	macerr;
Offset	apptr;
int	diflg;
int	evi;
int	vflag;
int	noscale;
int	po1;
int	nlist[NTRAP];
int	mlist[NTRAP];
int	evlist[EVLSZ];
int	ev;
int	tty;
int	sfont	= FT;	/* appears to be "standard" font; used by .ul */
int	sv;
int	esc;
int	widthp;
int	xfont;
int	setwdf;
int	over;
int	nhyp;
Tchar	**hyp;
Tchar	*olinep;
int	dotT;
char	*unlkp;
Wcache	widcache[NWIDCACHE];
Diver	d[NDI];
Diver	*dip;
