/* ce is the parameter for "clear to end of line" */
# include "stdio.h"
# include "ctype.h"
char *tgetent(), *tgoto(), *tputs(), *tgetstr(), *getenv();
static char tname[25]="", tcapbuf[1024], *Movcur, strarea[1204], *p_area=strarea;
static char *Home=NULL, *Clr2nd=NULL, *Clear=NULL, *ClrLine=NULL;
static int tset=0;
tgate()
{
	static gate=0; char *envp;
	if (gate==0)
		{
		gate=1;
		if (!isatty(1)) return; /* don't ask pointless questions */
		if (tset==0)
			{
			envp = getenv("TERM");
			if (tname[0]==0 && envp != NULL)
			    strncpy (tname, envp, 25);
			if (! *tname) {
				fputs ("Terminal type? ", stdout);
				fflush(stdout);
				gets(tname);
				}
			}
		if (*tname==0) return;
		switch ( tgetent(tcapbuf, tname))
			{
			case -1: fprintf(stderr, "No termcap file\n");
				    exit(1);
			case 0: /* unknown terminal */
				return;
			default: break;
			}
		if (strcmp(tname, "adm3a")==0)
			{
			extern char *UP;
			UP = "xxx";
			}
		Movcur = tgetstr("cm", &p_area);
		Home = tgetstr("ho", &p_area);
		Clr2nd = tgetstr("cd", &p_area);
		Clear = tgetstr("cl", &p_area);
		ClrLine = tgetstr("ce", &p_area);
		}
}
tpos (line, col)
{
	tgate();
	if (Movcur!=NULL)
		{
		char tmpstr[100];
		strncpy(tmpstr, tgoto(Movcur, col, line), 100);
		if (strcmp(tmpstr, "OOPS")==0) return;
		termwr(tmpstr);
		}
	else
	if (Home!=NULL)
		{
		termwr(Home);
		while (line-- > 0)
			termwr("\n");
		while (col-- > 0)
			termwr(" ");
		}
	return;
}
startsc()
{
tgate();
if (Home != NULL)
	termwr(Home);
else
if (Movcur != NULL)
	tpos(0,0);
if (Clr2nd != NULL)
	termwr(Clr2nd);
else if (Clear !=NULL)
	termwr(Clear);
}
clr2end ()
{
tgate();
if (Clr2nd !=NULL)
	termwr(Clr2nd);
}
clrline()
{
tgate();
if (ClrLine !=NULL)
	termwr(ClrLine);
}
ifclear()
{
return (Clr2nd!=NULL);
}
ifclrline()
{
return(ClrLine!=NULL);
}
stterm(t)
	char *t;
{
strcpy(tname, t);
tset=1;
}
char *
gtterm()
{
return(tname);
}
termwr(s)
	char *s;
{
int delay; char *sr;
for(sr=s; isdigit(*sr); sr++)
	;
fputs(sr, stdout);
if (sr<=s) return;
for (delay=atoi(s)/10; delay>0; delay--)
	putc(000, stdout);
}
