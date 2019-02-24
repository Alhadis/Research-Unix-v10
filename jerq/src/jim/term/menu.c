#include "jim.h"
static unsigned char menupos[MAXFILES];
static int nnames=1;		/* 1 for DIAG, initially there */
char	*name[MAXFILES+1];
char	*gnamegen();
Menu generic = {
	0,
	gnamegen
};
char *lnames[] = {
	"cut",
	"paste",
	"snarf",
	"look",
	"<mux>",
	0,	/* will be search string */
	0,	/* will be unix string */
	0,
};
Menu locmenu = {
	lnames
};
keepsearch(which){	/* icky routine */
	static char searchstring[11];
	static char unixstring[11];
	static seenunix=0;
	char *p=searchstring;
	char *cmd=diag->frame->str.s;
	int n=SEARCH;
	if(which==0){	/* search */
		if(lnames[UNIX]==0 && seenunix)
			lnames[UNIX]=unixstring;
	}else{	/* unix */
		if(lnames[UNIX] || (lnames[SEARCH] && !seenunix))
			n=UNIX;
		p=unixstring;
		if(*cmd=='*')
			cmd++;
		seenunix=1;
	}
	movstring(10, cmd, lnames[n]=p);
	p[min(10, diag->frame->str.n)]=0;
}
Text *
textoffile(f)
	register unsigned f;
{
	register Text *t;
	for(t=text; t; t=t->next)
		if(f==t->file)
			break;
	return t;
}
setchar(f, n, c)
	unsigned f;
{
	name[menupos[f]][n]=c;
}
delmenu(f)
	register f;
{
	register m=menupos[f];
	register char *x;
	register i;
	/* don't free it; save needless alloc() thrashing (see insmenu) */
	x=name[m];		/* save the allocated string... */
	for(i=m; i<nnames-1; i++)
		name[i]=name[i+1];
	name[--nnames]=x;	/* ... and put it at the end */
	for(i=1; i<MAXFILES; i++)
		if(menupos[i]>m)
			--menupos[i];
	menupos[f]=0;
}
insmenu(f, s)
	register f;
	register char *s;
{
	register i, m;
	register char **p;
	register char *x;
	for(m=1; m<nnames && strcmp(name[m]+3, s)<0; m++)	/* +3 skips '*  */
		;
	x=name[nnames];		/* a previously allocated string (maybe) */
	for(i=nnames-1; i>=m; i--)
		name[i+1]=name[i];
	for(i=1; i<MAXFILES; i++)
		if(menupos[i]>=m)
			++menupos[i];
	menupos[f]=m;
	p=name+m;
	if((*p=x)==0){
		if((*p=alloc(3+NAMELEN+2))==0)	/* '* name_\0 */
			panic("insmenu");
	}
	strcpy(*p+3, s);
	strcat(*p+3, "\240");	/* 0240 == 0x80|' ' */	
	++nnames;
}
strcmp(s, t)
	register char *s, *t;
{
	while(*s==*t++)
		if(*s==0)
			return 0;
		else
			s++;
	if(*s==0240)
		return -t[-1];
	return *s-t[-1];
}
setname(f, s)
	register f;
	register char *s;
{
	register exists;
	char buf[3];
	if((exists=menupos[f])){	/* assignment = */
		movstring(3, name[menupos[f]], buf);
		delmenu(f);
	}else
		movstring(3, "   ", buf);	
	insmenu(f, s);
	movstring(3, buf, name[menupos[f]]);
}
modified(t, mod)
	register Text *t;
{
	if(t && t!=diag)
		setchar(t->file, PRIME, " '"[mod]);
}
/*ARGSUSED*/
menulocal(t, pt, but)
	register Text *t;
	Point pt;
	int but;
{
	register Frame *f=t->frame;
	String hold;
	int hit=menuhit(&locmenu,2);
	if(t->selecthuge && CUT<=hit && hit<=EXCH){
		mesg("sorry; can't edit huge selection\n", 1);
		buttons(UP);
		return;
	}
	switch(hit) {
	case CUT:
		Send(O_CUT, 0, 0, "");
		cut(t, TRUE, t==diag? diagclr : F_CLR);
		break;
	case PASTE:
		if(snarfbuf.n<=0)
			break;
		if(snarfhuge){
			mesg("sorry; can't paste with huge snarf buffer\n", 1);
			return;
		}
		/*
		 * Because selection of source and dest can
		 * be done in arb. order, must send selection.
		 */
		Send(O_SELECT, f->s1, 2, data2(f->s2-f->s1));
		Send(O_PASTE1, 0, 0, (char *)0);
		cut(t, FALSE, t==diag? diagclr : F_CLR);
		/*
		 * PASTE goes in two parts because cut() can call loadfile();
		 */
		Send(O_PASTE2, 0, 0, (char *)0);
		if(snarfhuge){
			move(t, Pt(0, f->scrolly), B2);
		}else{
			frinsert(f, &snarfbuf, f->s1);
			f->s2=f->s1+snarfbuf.n;
			t->selecthuge=0;
			selectf(f, F_XOR);
		}
		break;
	case SNARF:
		if(f->s2>f->s1){
			Send(O_SNARF, 0, 0, "");
			snarf(&f->str, f->s1, f->s2, &snarfbuf);
		}
		break;
	case EXCH:
		hold.s=0;
		hold.n=hold.size=0;
		getmuxbuf(&hold);
		setmuxbuf(&snarfbuf);
		snarf(&hold, 0, hold.n, &snarfbuf);
		gcfree(hold.s);
		sendsnarf();
		break;
	case -1:
		break;
	default:
		if(worktext){
			/* hit-SEARCH: -2: look for text; 0: prev search; 1: prev unix */
			send(worktext->file, O_SEARCH, hit-SEARCH, 0, (char *)0);
			waitunix(&diagdone);
		}
		break;
	}
}
char *
gnamegen(i){
	static char *gmenu[]={"new", "reshape", "close", "write"};
	if(i<=WRITE)
		return gmenu[i];
	if(i>=nnames+WRITE)
		return 0;
	return name[i-WRITE];	/* DIAG is not in menu */
}
/*ARGSUSED*/
menugeneric(t, pt, but)
	Text *t;
	Point pt;
	int but;
{
	menughit(t, menuhit(&generic, 3), 0);
}
menughit(t, hit, remote)
	Text *t;
	register unsigned hit;
{
	Rectangle r;
	register i;
	if(remote)
		hit=menupos[hit]+WRITE;
	switch(hit){
	case -1:	/* no hit */
		break;
	case NEW:
		/* ask Unix for a file number */
		send(0, O_FILENAME, 0, 0, (char *)0);
		waitunix(&filedone);
		if(filedone>=0){
			hit=filedone;
			goto Get_it;
		}
		break;
	case RESHAPE:
	case CLOSE:
	case WRITE:
		cursswitch(&bullseye);
		buttons(DOWN);
		t=textofpoint(mouse.xy);
		if(!button12() && t && t!=diag) switch(hit){
		case WRITE:
			send(t->file, O_WRITE, 0, 0, (char *)0);
			waitunix(&diagdone);
			break;
		case CLOSE:
			send(t->file, O_CLOSE, 0, 0, (char *)0);
			break;
		case RESHAPE:
			buttons(UP);
			if(userrect(&r)){
				if(worktext){
					setchar(worktext->file, STARDOT, '*');
					Rectf(worktext->frame->scrollrect, F_XOR);
				}
				ontop(t);
				Rectf(t->frame->totalrect, F_CLR);
				frsetrects(t->frame, r);
				dodraw(t);
				if(current==worktext)
					current=t;
				if(current==diag)
					curse(t->frame);
				worktext=t;
				setchar(t->file, STARDOT, '.');
			}
			break;
		}
		cursswitch((Texture *)0);
		break;
	default:	/* must be a file; get it */
		hit-=WRITE;	/* reduce to file number */
		for(i=1; i<MAXFILES; i++)
			if(menupos[i]==hit)
				break;
		hit=i;
	Get_it:
		t=textoffile(hit);
		if(t==0){	/* get it */
			if(userrect(&r) == 0){
				buttons(UP);
				return;
			}
			t=txalloc(r);
			t->file=hit;
			workintext(t);
			if(current==diag)
				curse(t->frame);
			setchar(hit, STARDOT, '*');
			seek(t, Pt(0, 0), 0);
		}
		if(current==diag){
			rXOR(diag->frame->rect);
			curse(diag->frame);		/* on */
			curse(worktext->frame);	/* off */
		}
		workintext(t);
		current=t;
		setchar(hit, STARDOT, '.');
		break;
	}
	buttons(UP);	
}
userrect(rp)
	register Rectangle *rp;
{
	*rp=getrect3();
	if(rp->corner.x && rp->corner.x-rp->origin.x<=5 && rp->corner.y-rp->origin.y<=5)
		*rp=Drect;
	return rectclip(rp, screenrect)
	   && (rp->corner.x-rp->origin.x)>100 && (rp->corner.y-rp->origin.y)>40;
}
extern int move(), seek(), menugeneric(), menulocal(), txselect();
int (*butfunc[3][3])()={
	txselect,	menulocal, menugeneric,
	move,		seek,	 move,
	(int (*)())opnull, (int (*)())opnull,  (int (*)())opnull
};
char whichbut[]={	/* depends on def'n of button1(), etc. */
	B1, B3, B2, B2, B1, B1, B1, B1
};
whichrect(t, pt)
	register Text *t;
	Point pt;
{
	if(ptinrect(pt, t->frame->scrollrect))
		return SCROLLBAR;
	return FRAMERECT;
}
buttonhit(pt, but)
	Point pt;
	register but;
{
	register Text *t=textofpoint(pt);
	but=whichbut[but&7];
	/* icky special case to make scroll bar always active */
	if(t && t==worktext && ptinrect(pt, t->frame->scrollrect)){
		if(current!=t)
			curse(t->frame);
		(*(but==B2? seek : move))(t, pt, but);
		if(current!=t)
			curse(t->frame);
	}else if(but==B1 && t!=current){
		if(t==diag || current==diag)
			rXOR(diag->frame->rect);
		/* get the cursor right first */
		if(current==diag || (current==worktext && t==diag)){
			curse(diag->frame);
			curse(worktext->frame);
		}
		if(t==diag)
			diagclr=F_OR;
		else if(t){
			if(t!=worktext)
				workintext(t);
			diagclr=F_CLR;
		}
		current=t;
		buttons(UP);
	}else{
		if(current==0 && but!=B3)
			return;	/* nothing to do */
		(*butfunc[whichrect(current, pt)][but])(current, pt, but); 
	}
}
