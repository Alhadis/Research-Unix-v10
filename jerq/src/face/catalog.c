
#include <jerq.h>
#include <font.h>

#define bool int
#define true 1
#define false 0

#define ICONSIZE 48

typedef struct Icon
{
	Point Where;
	char *Which;
	struct Icon *Next;
} Icon;

struct Icon *IconList = 0;
struct Icon *NoIcon = 0;
Point Pos,NamePoint;
char Name[100];

bool CursPresent = true;

struct Icon *NewIcon(Where,Which,Next)
Point Where;
char *Which;
struct Icon *Next;
{
	Icon *New;
	New = (Icon *)alloc(sizeof(Icon));
	if (New==0) return(0);
	New->Where = Where;
	New->Which = alloc(strlen(Which)+1);
	strcpy(New->Which, Which);
	New->Next = Next;
	return(New);
}

struct Icon *IconAt(p)
Point p;
{
	Icon *Ic;
	p = sub(p,Drect.origin);
	for (Ic = IconList; Ic != 0; Ic = Ic->Next) {
		if ((p.x>=Ic->Where.x) && (p.y>=Ic->Where.y) &&
		    (p.x<=Ic->Where.x+ICONSIZE) && (p.y<=Ic->Where.y+ICONSIZE))
			return(Ic);
	}
	return(NoIcon);
}

getc()
{
	register ch;
	while((ch=rcvchar())==-1)
		wait(RCV);
	return ch;
}
OpLoad(b)
Bitmap *b;
{
	char buf[100];
	register i,j;
	char *np=Name;
	char *bp=(char *)b->base;
	while((*np=getc())!='\0')
		np++;
	for(i=0; i<48; i++){
		for(j=0; j<48/8; j++)
			bp[i*48/8+j]=getc();
		bp+=2;	/* MAC-32 to the rescue */
	}
}

Bitmap *b;
struct Icon *LastIcon = 0;

Setup()
{
	request(KBD|MOUSE);
	rectf(&display,Drect,F_CLR);
	Pos.x = 16; 
	Pos.y = 32;
	NamePoint.x = 16; 
	NamePoint.y = 12;
	b = balloc(Rect(0,0,ICONSIZE,ICONSIZE));
	NoIcon = NewIcon(Pt(-1000,-1000),"",0);
	LastIcon = NoIcon;
}

outline(b,r)
Bitmap *b;
Rectangle  r;
{
	segment(b,r.origin,Pt(r.origin.x,r.corner.y),F_XOR);
	segment(b,Pt(r.origin.x,r.corner.y),r.corner,F_XOR);
	segment(b,r.corner,Pt(r.corner.x,r.origin.y),F_XOR);
	segment(b,Pt(r.corner.x,r.origin.y),r.origin,F_XOR);
}

Rectangle IconFrame(Ic)
struct Icon *Ic;
{
	Rectangle frame;
	frame.origin = add(Ic->Where,Drect.origin);
	frame.corner = add(frame.origin,Pt(ICONSIZE-1,ICONSIZE-1));
	return(frame);
}

ShowName()
{
	struct Icon *ThisIcon;
	Rectangle frame;
	ThisIcon = IconAt(mouse.xy);
	if (ThisIcon != LastIcon) {
		string(&defont,LastIcon->Which,&display,add(Drect.origin,NamePoint),F_XOR);
		string(&defont,ThisIcon->Which,&display,add(Drect.origin,NamePoint),F_XOR);
		frame = IconFrame(LastIcon);
		outline(&display,inset(frame,-2)); 
		outline(&display,inset(frame,-3));
		frame = IconFrame(ThisIcon);
		outline(&display,inset(frame,-2)); 
		outline(&display,inset(frame,-3));
		if (LastIcon == NoIcon) {
			cursinhibit(); 
			CursPresent=false;
		}
		if (ThisIcon == NoIcon) {
			cursallow(); 
			CursPresent=true;
		}
		LastIcon = ThisIcon;
	}
}

NextPage()
{
	int ch;
	ch = kbdchar();
	rectf(&display,Drect,F_CLR);
	IconList = 0; 
	LastIcon = NoIcon;
	cursallow(); 
	CursPresent = true;
	Pos.x = 16; 
	Pos.y = 32;
}

PrintIcon()
{
	if (own()&MOUSE) ShowName();
	if (!CursPresent) cursallow();
	OpLoad(b);
	if (!CursPresent) cursinhibit();
	bitblt(b,b->rect,&display,add(Drect.origin,Pos),F_XOR);
	IconList = NewIcon(Pos,Name,IconList);
	Pos.x += ICONSIZE+16;
	if ((Drect.origin.x+Pos.x) > (Drect.corner.x-(ICONSIZE+16))) {
		Pos.x = 16;
		Pos.y += ICONSIZE+16;
	}
	if (Drect.origin.y+Pos.y > Drect.corner.y-(ICONSIZE+16)) {
		for (;;) {
			wait(KBD|MOUSE);
			if (own()&KBD) {
				NextPage(); 
				break;
			}
			if (own()&MOUSE) ShowName();
		}
	}
}

main()
{
	Setup();
	for (;;) {
		wait(KBD|MOUSE|RCV);
		if (own()&RCV) PrintIcon();
		if (own()&KBD) {
			cursallow(); 
			exit();
		}
		if (own()&MOUSE) ShowName();
	}
}
