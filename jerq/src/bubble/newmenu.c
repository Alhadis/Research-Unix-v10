#include <jerq.h>
#include <font.h>
#include <menu.h>
#include <3d.h>
#include "nodes.h"

extern long Ro[3];
extern short step, delta, skel, shadow, direction;
extern struct BALLS store[NNODES];
extern char buf[];

static short which=0;

static NMitem tmp;
NMitem *genflayo(),  *genfrot(),  *genfplay(),  *genlist();
NMitem *genfscal(),  *droplist(), *purelist();

NMenu Drop = { 0, droplist };

void F0() { center(0); redraw(); }
void F1() { mover(); redraw(); }
void F2() { center(1); resizer(); redraw(); }
void F3() { skel = 1 - skel; redraw(); }
void F4() { mask(which); partial(); blast(); }
void F5() { shadow = 1 - shadow; redraw(); }
void F6() { int j=max(1, delta/10); delta = max(1, delta-j); resizer(); redraw(); }
void F7() { int j=max(1, delta/10); delta += j; resizer(); redraw(); }
void F8() { if (which >= 0) playback(which); }
void F9() { if (confirm(3)!=3) sendchar('0'); }

void S0() { spin(which); }
void S1() { step += which; if (step < 1) step = 1; }
void S2() { direction = -direction; }
void S3()
{	int i; Node *n;
	extern Node *tab[];
	extern short ncount;
	extern struct LRot *frot;

	Ro[0] = Ro[1] = Ro[2] = 0;
	frot = (struct LRot *)NULL;
	for (i = 0; i < ncount; i++)
	{	n = tab[i];
		n->p = n->op;
		n->z = n->oz;
	}
	redraw();
}

NMenu Layo = { 0, genflayo };
NMenu Rota = { 0, genfrot };
NMenu Play = { 0, genfplay };
NMenu Scal = { 0, genfscal };

NMitem this[] = {
	{ "rotate  ", 0, &Rota },
	{ "playback", 0, &Play },
	{ "layout  ", 0, &Layo },
	{ "scale   ", 0, &Scal },
	{ "exit", 0, 0, 0, 0, 0, (long)F9 },
	{ 0 }
};

NMenu nmenu  = { this };

nmenuhit(butt)
{	NMitem *ret, *mhit();
	typedef void (*vfn)();
	Texture *TT;
	extern Texture coffee;

	ret = mhit(&nmenu, butt, 0);
	if (!ret)
		return -1;

	if (ret->data)
	{	TT = cursswitch(&coffee);
		(*(vfn)(ret->data))();
		(void)cursswitch(TT);
	}
	return 0;
}

NMitem *
genflayo(i)
{
	tmp.text = 0; tmp.help = 0; tmp.next = 0; tmp.data = 0;
	which = i;
	switch(i)
	{
	case  0: tmp.data=(long)F3;
		 sprintf(buf, "%s", (skel)?"bubbles":"bonds"); break;
	case  1: if (!skel)
		 {	tmp.data=(long)F5;
		 	sprintf(buf, "shadows %s",(!shadow)?"on ":"off");
		 } else
		 {	tmp.next = &Drop;
		 	sprintf(buf, "mask atoms");
		 }
		 break;
	case  2: if (!skel)
		 {	tmp.next = &Drop;
		 	sprintf(buf, "mask atoms"); break;
		 }
	default: return &tmp;
	}
	tmp.text = buf;

	return &tmp;
}

NMitem *
genfplay(i)
{	extern int MAX;

	tmp.text = 0; tmp.help = 0; tmp.next = 0;
	which = i-1; tmp.data = (long)F8;
	switch(i)
	{
	case  0: sprintf(buf, "--%d frames--", MAX); break;
	case  1: sprintf(buf, "repeat"); break;
	case  2: sprintf(buf, "loop"); break;
	case  3: sprintf(buf, "step"); break;
	case  4: sprintf(buf, "%s", (direction<0)?"forward":"backward");
		 tmp.data = (long)S2; break;
	default: return &tmp;
	}
	tmp.text = buf;

	return &tmp;
}

NMitem *
genfrot(i)
{	tmp.text = 0; tmp.help = 0; tmp.next = 0;
	switch(i)
	{
	case  0: sprintf(buf, "rotate x (%d)", (Ro[0]*180)/PI);
		 which = i; tmp.data = (long)S0; break;
	case  1: sprintf(buf, "rotate y (%d)", (Ro[1]*180)/PI);
		 which = i; tmp.data = (long)S0; break;
	case  2: sprintf(buf, "rotate z (%d)", (Ro[2]*180)/PI);
		 which = i; tmp.data = (long)S0; break;
	case  3: sprintf(buf, ">> angle (%d)", step);
		 which =  1; tmp.data = (long)S1; break;
	case  4: sprintf(buf, "<< angle (%d)", step);
		 which = -1; tmp.data = (long)S1; break;
	case  5: sprintf(buf, "%s", (direction<0)?"forward":"backward");
		 tmp.data = (long)S2; break;
	case  6: sprintf(buf, "reset");
		 tmp.data = (long)S3; break;
	default: return &tmp;
	}
	tmp.text = buf;

	return &tmp;
}

NMitem *
genfscal(i)
{
	tmp.text = 0; tmp.help = 0; tmp.next = 0; tmp.data = 0;
	switch(i)
	{
	case  0: sprintf(buf, "center"); tmp.data = (long)F0; break;
	case  1: sprintf(buf, "move");   tmp.data = (long)F1; break;
	case  2: sprintf(buf, "window size"); tmp.data = (long)F2; break;
	case  3: sprintf(buf, "10%%  larger"); tmp.data = (long)F6; break;
	case  4: sprintf(buf, "10%% smaller"); tmp.data = (long)F7; break;
	default: return &tmp;
	}
	tmp.text = buf;

	return &tmp;
}

NMitem *
purelist(i)
{
	tmp.text = 0; tmp.help = 0; tmp.next = 0;
	tmp.data = (long) F4;
	switch(i)
	{
	case  0: which = 'H'-'A'; sprintf(buf, "hydrogen ");   break;
	case  1: which = 'C'-'A'; sprintf(buf, "carbon   ");  break;
	case  2: which = 'N'-'A'; sprintf(buf, "nitrogen ");   break;
	case  3: which = 'O'-'A'; sprintf(buf, "oxygen   ");  break;
	case  4: which = 'P'-'A'; sprintf(buf, "phosphor "); break;
	case  5: which = 'S'-'A'; sprintf(buf, "sulfur   ");  break;
	default: return &tmp;
	}
	tmp.text = buf;

	return &tmp;
}

NMitem *
genlist(i)
{	char str[128];

	purelist(i);	/* sets tmp */
	if (tmp.text)
	{	sprintf(str, "(%d)", store[which].n);
		strcat(tmp.text, str);
	}
	return &tmp;
}

NMitem *
droplist(i)
{	char str[128];

	genlist(i);
	if (tmp.text)
	{	strcpy(str, tmp.text);
		sprintf(buf, "%s %s", (store[which].vis)?"mask  ":"unmask",str); 
		tmp.text = buf;
	}
	return &tmp;
}
