/*
**	tpr - text formatter
**		Ken Yap, June 1981
*/
#include	<stdio.h>
#include	<ctype.h>

#define	min(a,b)	(a<b?a:b)
#define	max(a,b)	(a>b?a:b)
#define	skipbl(p)	{while(*p == ' ' || *p == '\t')p++;}
#define	skipnbl(p)	{while(*p != ' ' && *p != '\t' && *p != '\n')p++;}
#define	CHARNULL	((char *)NULL)
#define	NO		0
#define	YES		1
#define	COMMAND		'.'
#define	PAGENUM		'%'
#define	HUGE		10000
#define	MAXIN		132
#define	MAXOUT		132
#define	MAXCHARS	14
#define	MAXMAC		50
#define	MAXPB		50
#define	MAXTABS		20
#define	PAGLEN		66
#define	PAPERSIZE	65
#define	M1DEF		3
#define	M2DEF		1
#define	M3DEF		1
#define	M4DEF		3
#define	PAGEWIDTH	60
#define	ARABIC		0
#define	ROMAN		1

struct linelink{
	char *lineptr;
	struct linelink *lastline;
	};
struct	macro{
	char macnam[3];
	struct linelink *macend;
	} macros[MAXMAC];
short	maccnt = 0;		/* counter for current macro */
char	*pbptr[MAXMAC];		/* pointers to pushed back lines */
short	pblev = 0;		/* indicates level of macro nesting during collection */

char	outbuf[MAXOUT];	/* lines to be filled collect here */
char	*outp = outbuf;	/* last char position in outbuf; init = 0 */
short	outw = 0;	/* width of text currenty in outbuf; init = 0 */
short	outwds = 0;	/* number of words in outbuf; init = 0 */

short curpag	= 0;
short newpag	= 1;
short lineno	= 0;
char blnkhdr[]	= "\n";
struct	envir{
	short plval;
	short m1val;
	short m2val;
	short m3val;
	short m4val;
	short bottom;
	char *evenhdr,*oddhdr;
	char *evenftr,*oddftr;
	char comchr;
	char tabchr;
	char ubchr;
	short fill;
	short adjust;
	short numtyp;
	short lsval;
	short llval;
	short inval;
	short tival;
	short poval;
	short ceval;
	short ulval;
	short litval;
	short blval;
	short skpval;
	short tabpos[MAXTABS];
	struct envir *lastenv;
	};
struct envir env	= {
	PAGLEN, M1DEF, M2DEF, M3DEF, M4DEF, (PAGLEN-M3DEF-M4DEF),
	blnkhdr, blnkhdr,
	blnkhdr, blnkhdr,
	'.', '\t', ' ',
	YES, YES, ARABIC, 1, PAGEWIDTH, 0, 0, 0, 0, 0, 0, 0, 0,
	{ 8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,0 },
	NULL
	};
struct envir *curenv	= NULL;
struct cmdents{
	char cmdname[3];
	short notredefd;
	};
struct cmdents builtins[] ={
	{ "ad",YES },
	{ "ar",YES },
	{ "bl",YES },
	{ "bp",YES },
	{ "br",YES },
	{ "cc",YES },
	{ "ce",YES },
	{ "de",YES },
	{ "ef",YES },
	{ "eh",YES },
	{ "fi",YES },
	{ "fo",YES },
	{ "he",YES },
	{ "in",YES },
	{ "ix",YES },
	{ "li",YES },
	{ "ll",YES },
	{ "ls",YES },
	{ "m1",YES },
	{ "m2",YES },
	{ "m3",YES },
	{ "m4",YES },
	{ "na",YES },
	{ "ne",YES },
	{ "nf",YES },
	{ "of",YES },
	{ "oh",YES },
	{ "pl",YES },
	{ "po",YES },
	{ "re",YES },
	{ "ro",YES },
	{ "se",YES },
	{ "sk",YES },
	{ "sp",YES },
	{ "ta",YES },
	{ "tc",YES },
	{ "ti",YES },
	{ "ub",YES },
	{ "ul",YES }
	};

enum cmdnum	{ADJ, ARA, BLN, BPG, BRE, CMC, CEN, DFN, EFO, EHD, FIL,
		FOT, HED, IND, INX, LIT, LNL, LNS, M1, M2, M3,
		M4, NAD, NED, NFL, OFO, OHD, PGL, POF, RNV, ROM, SNV, SKP,
		SPA, TCL, TCH, TMI, UBC, UDL, MAC, UNKNOWN};

short echodir	= 0;
char *filename	= "stdin";
short fileline	= 0;
char *nomem	= "Out of dynamic memory\n";	/* canned message */

/*
**	main
*/
main(argc,argv)
	short argc;
	char **argv;{
	char stdoutbuf[BUFSIZ];
	FILE *file;
	
	for(argc--, argv++; argc > 0 && **argv == '-' && (*argv)[1] != '\0'; argc--, argv++)
		setoptions(*argv);
	setbuf(stdout,stdoutbuf);
	if(argc == 0)
		tpr(stdin,"stdin");
	else
		for( ; argc > 0; argc--, argv++){
			if(**argv == '-' && (*argv)[1] == '\0')
				tpr(stdin,"stdin");
			else{
				if((file = fopen(*argv,"r")) == NULL){
					perror(*argv);
					continue;
					}
				tpr(file,*argv);
				fclose(file);
				}
			}
	if(lineno > 0)
		spc(HUGE);		/* flush last output */
	fflush(stdout);
}

setoptions(arg)
	char *arg;{
	register char c;

	while((c = *++arg) != '\0')
		switch(c){
			case 'd':	/* echo directives */
				echodir++;
				break;
			case 'e':	/* divert errors */
				close(2);
				if(creat(++arg,0600) < 0){
					open("/dev/tty",1);
					perror(arg);
					exit(1);
					}
				return;
			default:
				fprintf(stderr,"Usage - see manual (ho, ho)\n");
				exit(1);
			}
}

/*
**	tpr - here is the main routine for each file
**	the name is passed along so that the 'include' directive
**	can call 'tpr' recursively
*/
tpr(file,name)
	FILE *file;
	char *name;{
	register char *savename;
	register short saveline;
	char inbuf[MAXIN];
	char *ngetl();

	savename = filename;	/* save old name */
	filename = name;	/* make new one available for error routine */
	saveline = fileline;	/* and line number */
	fileline = 0;
	while(ngetl(inbuf,file) != CHARNULL){
		++fileline;
		if(*inbuf == env.comchr)	/* it's a command */
			cmd(inbuf,file);
		else			/* it's text */
			text(inbuf);
		}
	filename = savename;	/* restore name and linenumber */
	fileline = saveline;
}

/*
**	error - prints filename and linenumber
*/
error(msg,arg)
	char *msg,*arg;{

/*ROB
	fprintf(stderr,"At line %d in file %s: ",fileline,filename);
	fprintf(stderr,msg,arg);
*/
}

/*
**	blnk - space n lines (to new page if necessary, cf spc)
*/
blnk(n)
	short n;{
	register short i;
	
	linebreak();
	while(n > 0){
		if(lineno > env.bottom){
			pfoot();
			lineno = 0;
			}
		if(lineno == 0)
			phead();
		i = min(n,env.bottom + 1 - lineno);
		skip(i);
		n -= i;
		lineno += i;
		}
	if(lineno > env.bottom)
		pfoot();
}

/*
**	linebreak - end current filled line
*/
linebreak(){
	
	if(outp != outbuf){
		*outp++ = '\n';
		*outp = '\0';
		put(outbuf);
		}
	outp = outbuf;
	outw = 0;
	outwds = 0;
}

/*
**	centre - centre a line by setting tival
*/
center(buf)
	char *buf;{
	short width();
	
	env.tival = max((env.llval + env.tival - width(buf))/2,0);
}

/*
**	cmd - perform formatting command
*/
cmd(buf,file)
	char *buf;
	FILE *file;{
	enum cmdnum comtyp(),ct;
	register short spval,val;
	short macnum;
	char argtyp;
	short getval(),set();
	char *gettl();
	
	if(echodir)
		putdir(buf);
	ct = comtyp(buf,&macnum);
	val = 0;
	val = getval(buf,&argtyp);
	switch(ct){
		case SPA:
			spval = set(0,val,argtyp,1,0,HUGE);
			spc(spval);
			break;
		case IND:
		case INX:
			if(ct == IND)
				linebreak();
			env.inval = set(env.inval,val,argtyp,0,0,env.llval - 1);
			env.tival = env.inval;
			break;
		case TMI:
			linebreak();
			env.tival = set(env.tival,val,argtyp,0,0,env.llval);
			break;
		case CEN:
			linebreak();
			env.ceval = set(env.ceval,val,argtyp,1,0,HUGE);
			break;
		case UDL:
			env.ulval = set(env.ulval,val,argtyp,0,1,HUGE);
			break;
		case FIL:
			linebreak();
			env.fill = YES;
			break;
		case NFL:
			linebreak();
			env.fill = NO;
			break;
		case BRE:
			linebreak();
			break;
		case BLN:
			env.blval = set(env.blval,val,argtyp,1,0,HUGE);
			blnk(env.blval);
			env.blval = 0;
			break;
		case NED:
			if(val > env.bottom - lineno + 1)
				spc(HUGE);
			break;
		case LNS:
			env.lsval = set(env.lsval,val,argtyp,1,1,HUGE);
			break;
		case LNL:
			env.llval = set(env.llval,val,argtyp,PAGEWIDTH,env.tival + 1,HUGE);
			break;
		case PGL:
			env.plval = set(env.plval,val,argtyp,PAGLEN,
				env.m1val + env.m2val + env.m3val + env.m4val + 1,HUGE);
			env.bottom = env.plval - env.m3val - env.m4val;
			break;
		case BPG:
			if(lineno > 0)
				spc(HUGE);
			curpag = set(curpag,val,argtyp,curpag + 1, - HUGE,HUGE);
			newpag = curpag;
			break;
		case HED:
			env.evenhdr = env.oddhdr = gettl(buf);
			break;
		case FOT:
			env.evenftr = env.oddftr = gettl(buf);
			break;
		case EHD:
			env.evenhdr = gettl(buf);
			break;
		case EFO:
			env.evenftr = gettl(buf);
			break;
		case OHD:
			env.oddhdr = gettl(buf);
			break;
		case OFO:
			env.oddftr = gettl(buf);
			break;
		case NAD:
			env.adjust = NO;
			break;
		case ADJ:
			env.adjust = YES;
			break;
		case ROM:
			env.numtyp = ROMAN;
			break;
		case ARA:
			env.numtyp = ARABIC;
			break;
		case LIT:
			env.litval = set(env.litval,val,argtyp,1,0,HUGE);
			break;
		case M1:
			env.m1val = set(env.m1val,val,argtyp,M1DEF,
				0,env.plval - (env.m2val + env.m3val + env.m4val + 1));
			break;
		case M2:
			env.m2val = set(env.m2val,val,argtyp,M2DEF,
				0,env.plval - (env.m1val + env.m3val + env.m4val + 1));
			break;
		case M3:
			env.m3val = set(env.m3val,val,argtyp,M3DEF,
				0,env.plval - (env.m1val + env.m2val + env.m4val + 1));
			env.bottom = env.plval - env.m3val - env.m4val;
			break;
		case M4:
			env.m4val = set(env.m4val,val,argtyp,M4DEF,
				0,env.plval - (env.m1val + env.m2val + env.m3val + 1));
			env.bottom = env.plval - env.m3val - env.m4val;
			break;
		case CMC:
			if(argtyp != '\n')
				env.comchr = argtyp;
			else
				env.comchr = COMMAND;
			break;
		case POF:
			env.poval = set(env.poval,val,argtyp,0,0,PAPERSIZE);
			break;
		case SKP:
			env.skpval = set(env.skpval,val,argtyp,1,0,HUGE);
			break;
		case DFN:
			getmac(buf,file);
			break;
		case TCH:
			if(argtyp != '\n')
				env.tabchr = argtyp;
			else
				env.tabchr = '\t';
			break;
		case TCL:
			tabcol(buf);
			break;
		case UBC:
			if(argtyp != '\n')
				env.ubchr = argtyp;
			else
				env.ubchr = ' ';
			break;
		case RNV:
			if(val <= 0)
				val = 1;
			resenv(val);
			break;
		case SNV:
			savenv();
			break;
		case MAC:
			expand(macnum);
			break;
		case UNKNOWN:
			error("Unrecognised directive:\n%s",buf);
			return;
		}
}

/*
**	comtyp - decode command type
*/
enum cmdnum comtyp(buf,macnum)
	char *buf;
	short *macnum;{
	register char a,b;
	register struct cmdents *p;
	register char *q;
	register short l,h,m;
	
	a = buf[1];
	b = buf[2];
	for(l = 0, h = (sizeof(builtins)/sizeof(builtins[0])) - 1; l <= h; ){
		p = &builtins[m = (l + h) / 2];
		if(a < p->cmdname[0] || a <= p->cmdname[0] && b < p->cmdname[1])
			h = m - 1;
		else if(a > p->cmdname[0] || a <= p->cmdname[0] && b > p->cmdname[1])
			l = m + 1;
		else
			break;
		}
	if(l <= h && p->notredefd)
		return((enum cmdnum)m);
	for(l = maccnt - 1; l >= 0; l--){
		q = macros[l].macnam;
		if(a == *q++ && b == *q){
			*macnum = l;
			return(MAC);
			}
		}
	return(UNKNOWN);
}

/*
**	expand - pushback macro definition onto input
*/
expand(macnum)
	short macnum;{
	register struct linelink *lp;
	
	for(lp = macros[macnum].macend; lp != NULL; lp = lp->lastline){
		if(pblev > MAXPB){
			error("Not enough pushback space\n",CHARNULL);
			break;	/* to catch stack overflow */
			}
		pbptr[pblev] = lp->lineptr;
		pblev++;
		}
}

/*
**	getmac - collect macro
*/
getmac(buf,file)
	char *buf;
	FILE *file;{
	register char *p;
	register struct macro *mp;
	register struct linelink *lp;
	register short dotlev;
	enum cmdnum ct;
	short macnum;
	char line[MAXIN];
	enum cmdnum comtyp();
	char *strcpy();
	char *malloc();
	
	skipnbl(buf);
	skipbl(buf);
	if(*buf == '\n'){
		error("Missing macro name\n",CHARNULL);
		return;
		}
	if(maccnt >= MAXMAC){
		error("Too many macro definitions",CHARNULL);
		return;
		}
	mp = &macros[maccnt];
	p = mp->macnam;
	if((ct = comtyp(buf - 1,&macnum)) != UNKNOWN && ct != MAC)
		builtins[(int)ct].notredefd = NO;
	*p++ = *buf++;	/* record name */
	*p++ = *buf++;
	*p = '\0';
	mp->macend = NULL;
	dotlev = 1;
	do{
		if((p = ngetl(line,file)) == CHARNULL)
			break;	/* unexpected EOF */
		++fileline;
		if(*p++ == env.comchr){
			if(*p == '.')
				dotlev--;
			else if(comtyp(line,&macnum) == DFN)
				dotlev++;	/* included .de */
			}
		if(dotlev > 0){
			if((lp = (struct linelink *)malloc((unsigned)sizeof(*mp->macend))) == NULL){
				error(nomem,CHARNULL);
				break;
				}
			lp->lastline = mp->macend;
			mp->macend = lp;
			if((lp->lineptr = malloc((unsigned)(strlen(line) + 1))) == NULL){
				error(nomem,CHARNULL);
				break;
				}
			strcpy(lp->lineptr,line);
			}
		}while(dotlev > 0);
	maccnt++;
}

/*
**	getseg - puts out part of header
*/
char *getseg(buf,copy,copyend,term,pageno)
	char *buf,*copy,*copyend,term;
	short pageno;{
	register short i;
	register char *p;
	short itorom();
	
	p = buf;
	for( ;copy != copyend && *p != term && *p != '\0' && *p != '\n'; p++){
		if(*p == PAGENUM){
			if(env.numtyp == ARABIC){
				sprintf(copy, "%d", pageno);
				i=strlen(copy);
			}
			else
				i = itorom(pageno,copy,min(MAXCHARS,(short)(copyend - copy)));
			copy += i;
			}
		else
			*copy++ = *p;
		}
	if(*p == term)
		p++;
	*copy = '\0';
	return(p);
}

/*
**	gettl - copy title from buf to ttl
*/
char *gettl(buf)
	char *buf;{
	register char *p,*q;
	char *strcpy();
	char *malloc();
	
	p = buf;
	skipnbl(p);
	skipbl(p);	/* find argument */
	if((q = malloc((unsigned)(strlen(p) + 1))) == NULL){
		error(nomem,CHARNULL);
		return(q);
		}
	strcpy(q,p);
	return(q);
}

/*
**	getval - evaluate optional numeric argument
*/
short getval(buf,argtyp)
	char *buf;
	char *argtyp;{
	int atoi();
	
	skipnbl(buf);
	skipbl(buf);	/* find argument */
	*argtyp = *buf;
	if(!isdigit(*buf))
		buf++;
	return(atoi(buf));
}

/*
**	getwrd - get a non - blank word from instr(i) to out, increment i
*/
char *getwrd(instr,out)
	char *instr,*out;{
	register char c;
	register char *p,*q;
	
	p = instr;
	q = out;
	while((*p == ' ' || *p == '\t') && *p != env.tabchr)
		p++;
	instr = p;
	if(*p == env.tabchr)
		*q++ = *p;
	else{
		while((c = *p) != '\0' && c != ' ' && c != '\t'
			&& c != '\n' && c != env.tabchr){
			*q++ = *p++;
			}
		}
	*q = '\0';
	return(p == instr ? NULL : p);
}

/*
**	itorom - converts integer to roman numerals
*/
short itorom(num,str,flen)
	char *str;
	short num,flen;{
	register short i,j;
	char *p;
	static short romval[]	= { 1000,500,100,50,10,5,1,0 };
	static short reltab[]	= { 2,1,2,1,2,1,1,0 };
	static char romlet[]	= "mdclxvi0";
	
	p = str;
	if(num < 0 && flen > 1){
		num = -num;
		*p++ = '-';
		}
	for(i = 0; num > 0; i++){
		while(num >= romval[i]){
			num -= romval[i];
			*p++ = romlet[i];
			}
		j = i + reltab[i];
		if(num >= (romval[i] - romval[j])){
			num -= (romval[i] - romval[j]);
			*p++ = romlet[j];
			*p++ = romlet[i];
			}
		}
	*p = '\0';
	return((short)(p - str));
}

/*
**	leadbl - delete leading blanks, set tival
*/
leadbl(buf)
	char *buf;{
	register char *p;
	
	linebreak();
	p = buf;
	skipnbl(buf);
	if(*buf != '\n')
		env.tival = (buf - p);
	while(*buf != '\0')		/* move line to left */
		*p++ = *buf++;
	*p = '\0';
}

/*
**	nextab - returns position of next tab stop
*/
short nextab(pos)
	short pos;{
	register short i,k;
	
	for(i =  0; i < MAXTABS; i++){
	       if(env.tabpos[i] == 0)
			break;
	       if(env.tabpos[i] > pos){
	               k = env.tabpos[i];
	               return(k);
	               }
	       }
	k = pos + 1;
	return(k);
}

/*
**	ngetl - gets line from input or pushback buffer
*/
char *ngetl(buf,file)
	char *buf;
	FILE *file;{
	char *fgets();
	char *strcpy();
	
	if(pblev <= 0)
		return(fgets(buf,MAXIN,file));
	else{
		pblev--;
		strcpy(buf,pbptr[pblev]);
		}
	return(buf);
}

/*
**	pfoot - put out page footer
*/
pfoot(){
	
	skip(env.m3val);
	if(env.m4val > 0){
		puttl(curpag % 2 ? env.oddftr : env.evenftr, curpag);
		skip(env.m4val - 1);
		}
}

/*
**	phead - put out page header
*/
phead(){
	
	curpag = newpag;
	newpag++;
	if(env.m1val > 0){
		skip(env.m1val - 1);
		puttl(curpag % 2 ? env.oddhdr : env.evenhdr, curpag);
		}
	skip(env.m2val);
	lineno = env.m1val + env.m2val + 1;
}

/*
**	put - put out line with proper spacing and indenting
*/
put(buf)
	char *buf;{
	short nextab();
	register char c;
	register short col,i;
	
	if(lineno == 0 || lineno > env.bottom)
		phead();
	for(i = (env.tival + env.poval); i--; )
		putchar(' ');
	col = env.tival;
	env.tival = env.inval;
	for(; (c = *buf) != '\0'; buf++){
		if(c == env.ubchr)
			c = ' ';	/* put blanks instead of blank replacement */
		if(c == env.tabchr){
			i = nextab(col);	/* nextab wants last used column */
			for(; col < i; col++)
				putchar(' ');
			continue;
			}
		else if(c == '\b')
			col--;
		else
			col++;
		putchar(c);
		}
	skip(min(env.lsval - 1,env.bottom - lineno));
	lineno += env.lsval;
	if(lineno > env.bottom){
		pfoot();
		if(env.skpval > 0)
			skpage(env.skpval);
			env.skpval = 0;
		}
}

/*
**	putdir - output a directive
*/
putdir(buf)
	char *buf;{

	fprintf(stderr,"%.10s",buf);	/* first 10 chars */
}

/*
**	puttl - put out title line with optional page number
*/
puttl(buf,pageno)
	char *buf;
	short pageno;{
	char copy[MAXOUT],term;
	char *getseg();
	register char *p;
	register short i,col,newcol;
	
	for(i = env.poval; i--; )
		putchar(' ');
	term = *buf;
	if(term == '\n'){
		putchar('\n');
		return;
		}
	col = 1;
	p = buf + 1;
	p = getseg(p,copy,&copy[MAXOUT],term,pageno);
	printf("%s", copy);
	col += strlen(copy);
	buf = p;
	p = getseg(p,copy,&copy[MAXOUT],term,pageno);
	newcol = (env.llval - strlen(copy))/2 + 1;	/* start of centre */
	for(; col < newcol; col++)
		putchar(' ');
	printf("%s", copy);
	col += strlen(copy);
	p = getseg(p,copy,&copy[MAXOUT],term,pageno);
	newcol = env.llval - strlen(copy) + 1;	/* start of right */
	for(; col < newcol; col++)
		putchar(' ');
	printf("%s",copy);
	putchar('\n');
}

/*
**	putwrd - put a word in outbuf; includes margin justification
*/
putwrd(wrdbuf)
	char *wrdbuf;{
	char *strcpy();
	register short l,w,lnval,nextra;
	short width(), nextab();
	int strlen();
	
	lnval = env.llval - env.tival;
	if(*wrdbuf == env.tabchr){
		outw = nextab(outw + env.tival) - env.tival;
		/* because outw floats from the indent */
		/* and nextab is absolute */
		if(outp  != outbuf && outw > lnval){
			linebreak();
			outw = nextab(outw + env.tival) - env.tival;
			}
		*++outp = env.tabchr;
		outwds = 0;		/* adjust from next word */
		}
	else{
		w = width(wrdbuf);
		l = strlen(wrdbuf);
		if(outp != outbuf && (outw + w > lnval ||	/* too big */
			(char *)(outp + l) >= &outbuf[MAXOUT])){
			--outp;	/* we put in a blank earlier */
			if(env.adjust == YES){
				nextra = lnval - outw + 1;
				spread(outp,nextra,outwds);
				outp += nextra;
				}
			linebreak();		/* flush previous line */
			}
		strcpy(outp,wrdbuf);
		outp += l;
		*outp++ = ' ';		/* blank between words */
		outw += w + 1;			/* 1 for blank */
		outwds++;
		}
}

/*
**	resenv - restore environment n levels back
*/
resenv(n)
	short n;{
	register struct envir *ep,*tp;
	
	linebreak();		/* to flush any latent output */
	for(ep = curenv; ep != NULL && --n > 0; free(tp))
		ep = (tp = ep)->lastenv;
	if(ep != NULL){
		env = *ep;
		curenv = ep->lastenv;
		free(ep);
		}
}

/*
**	savenv - keep environment for later restoration
*/
savenv(){
	register struct envir *ep;
	
	if((ep = (struct envir *)malloc((unsigned)sizeof(env))) == NULL){
		error(nomem,CHARNULL);
		return;
		}
	*ep = env;	/* structure copy */
	ep->lastenv = curenv;
	curenv = ep;
}

/*
**	set - set parameter and check range
*/
short set(param,val,argtyp,defval,minval,maxval)
	short param,val,defval,minval,maxval;
	char argtyp;{
	
	switch(argtyp){
		case '\n':
			param = defval;
			break;
		case '+':
			param = param + val;
			break;
		case '-':
			param = param - val;
			break;
		default:
			param = val;
		}
	param = min(param,maxval);
	param = max(param,minval);
	return(param);
}

/*
**	skip - output n blank lines
*/
skip(n)
	short n;{
	
	while(n-- > 0)
		putchar('\n');
}

/*
**	skpage - skip n pages
*/
skpage(n)
	short n;{
	
	while(n-- > 0){
		phead();
		skip(env.bottom + 1 - lineno);
		lineno = env.bottom + 1;
		pfoot();
		}
}

/*
**	spc - space n lines or to bottom of page (cf blnk)
*/
spc(n)
	short n;{
	
	linebreak();
	if(lineno > env.bottom)
		return;
	if(lineno == 0)
		phead();
	skip(min(n,env.bottom + 1 - lineno));
	lineno += n;
	if(lineno > env.bottom){
		pfoot();
		if(env.skpval > 0)
			skpage(env.skpval);
			env.skpval = 0;
		}
}

/*
**	spread - spread words to justify right margin
*/
spread(ptr,nextra,nwrds)
	char *ptr;
	short nextra,nwrds;{
	register char *p,*q;
	register short nb,nholes;
	static short dir	= 0;
	
	if(nextra <= 0 || nwrds <= 1)
		return;
	dir = !dir;		/* reverse previous direction */
	nholes = nwrds - 1;
	p = ptr - 1;
	q = ptr + nextra;
	*q-- = '\0';
	while(p < q){
		if((*q = *p) == ' '){
			nb = dir ? (nextra - 1) / nholes + 1 :
				nextra / nholes;
			nextra -= nb;
			nholes--;
			while(nb-- > 0){
				*--q = ' ';
				}
			}
		p--;
		q--;
		}
}

/*
**	tabcol - enters pseudotab stops, checking validity
*/
tabcol(buf)
	char *buf;{
	int atoi();
	register short tp,incr,val;
	
	for(tp = 0; tp < MAXTABS - 1; tp++){
		skipnbl(buf);
		skipbl(buf);
		if(*buf == '\n')
			break;	/* end of list */
		incr = *buf++ == '+' ? YES : NO;
		val = atoi(buf);
		if(incr == YES && tp > 1)	/* relative tab */
			val = env.tabpos[tp - 1] + val;
		env.tabpos[tp] = val;
		if(val < 0 || (tp > 1 && val < env.tabpos[tp - 1]))
			tp--;
		}
	env.tabpos[tp] = 0;	/* end of list */
}

/*
**	text - process text lines
*/
text(inbuf)
	char *inbuf;{
	register char *p;
	char wrdbuf[MAXIN];
	char *getwrd();
	
	if(env.litval > 0){
		put(inbuf);
		env.litval--;
		return;
		}
	if(*inbuf == ' ' || *inbuf == '\n')
		leadbl(inbuf);	/* move left, set tival */
	if(env.ulval > 0){			/* underlining */
		underl(inbuf,wrdbuf,&wrdbuf[MAXIN]);
		env.ulval--;
		}
	if(env.ceval > 0){			/* centering */
		center(inbuf);
		put(inbuf);
		env.ceval--;
		}
	else if(*inbuf == '\n')	/* all blank line */
		put(inbuf);
	else if(env.fill == NO)		/* unfilled text */
		put(inbuf);
	else
		for(p = inbuf; (p = getwrd(p,wrdbuf)) != NULL;)
			putwrd(wrdbuf);
}

/*
**	underl - underline a line
*/
underl(buf,tbuf,tend)
	char *buf,*tbuf,*tend;{
	register char c;
	register char *p,*q;
	char *strcpy();
	
	p = buf;
	q = tbuf;
	while(*p != '\n' && q < tend){
		if(isalnum(c = *p++)){
			*q++ = '_';
			*q++ = '\b';
			}
		*q++ = c;
		}
	*q++ = '\n';
	*q = '\0';
	strcpy(buf,tbuf);	/* copy it back to buf */
}

/*
**	width - compute width of character string
*/
short width(buf)
	char *buf;{
	register short i;
	register char c;
	
	for(i = 0; (c = *buf) != '\0'; buf++){
		if(c == '\b')
			i--;
		else if(c != '\n')
			i++;
		}
	return(i);
}
