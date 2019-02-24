
/*	Main for PED running on Blit	*/

#include "jped.h"

extern short user_com;
extern short noscale;

struct disper  dstack[16], *stp;


#define SMENU 10
#define MAX_MSIZEp 13
#define CRISIS MXOBJ-4

#define release3() for(;;wait(MOUSE)) if(!button3()) break

short nosk;
short def_font;
short def_size;
short lin_size;
char textmode;

Rectangle board;
Rectangle bulletin;
Rectangle lfside[MAX_MSIZEp];

char **auxmen;
char **oauxmen;


short m_size;
short om_size;

short c_menu;

short subsel;
short osubsel;

extern char *menuAA[];
extern short starters();
extern char *menuRO[];
extern short remote();
extern char *menuDR[];
extern short use_pen();
extern char *menuTR[];
extern short transf_obj();
extern char *menuSH[];
extern short shades();
extern char *menuCL[];
extern short color();
extern char *menuSP[];
extern short reshape();
extern char *menuMC[];
extern short arrange();
extern char *menuVV[];
extern short fine_view();

short idle(){ return(-1); }

char *menuZZ[10];
Menu menu3 = { menuZZ };

/*	defined in jped.h
	struct c_unit {
		char * menu_item;
		char **wlist;
		short (*fun)();
		short how_many;
		char imd;
		char txr;
		};
*/
	struct c_unit vault[SMENU] = {
	"basic",	menuAA,	starters,	10,	3,	0,
	"ch size",	menuTR,	transf_obj,	8,	0,	0,
	" move ",	menuMC,	arrange,	9,	0,	0,
	" draw ",	menuDR,	use_pen,	9,	1,	0,
	"reshape",	menuSP,	reshape,	10,	0,	0,
	"refine",	menuVV, fine_view,	10,	0,	0,
	"shade",	menuSH,	shades,		12,	0,	1,
	"color",	menuCL, color,		7,	0,	0,
	"remote",	menuRO,	remote, 	9,	2,	0,
	"itself",	menuZZ,	idle,		8,	1,	0
};


Point placelab;

objP obj[MXOBJ];
short no_obj;
short D_ON;
short drawpar;
Code dr_mode = F_XOR;
short grsize = 0;
short grihalf;

Bitmap *negsq, *poscross, *ball, *vball[8];

Font * fontrcv[MXFONT];
char font_notav[MXFONT];


char scbf[256];	/*scratch buffer used by various routines*/


main()
{
	register j, mselect;
	register objP aa;
	short n, m;
	no_obj = D_ON = drawpar = 0;
	nosk = 1;
	stp = dstack;
	for(j=0;j<9;j++) menuZZ[j] = vault[j].menu_item;
	menuZZ[9] = 0;

	request(MOUSE|RCV|SEND|KBD);
	cursswitch(&clock);	/*be patient during init*/

	if(!(m=inalloc())) {
		string(&defont,"No working space",&display,Drect.origin,dr_mode);
		for(;;sleep(2),wait(MOUSE)) if(button1()) break;
		exit(1);
	}

	if(bconstruct()) {	/* set up bitmaps */
		string(&defont,"No bitmap space",&display,Drect.origin,dr_mode);
		for(;;sleep(2),wait(MOUSE)) if(button1()) break;
		exit(1);
	}

	c_menu = 0;
	oauxmen = 0;
	om_size = 0;
	def_size = 10;
	lin_size = 9;
	textmode = 'L';
	user_com = 0;
	draw_menu(c_menu);	/*subsel = osubsel = -1*/
	drawpar = 0;	/*	no markings on plots	*/
	plot_all();	/*just clears screen, since no_obj=0*/

	/* get intro message */
	n = g_short();
	getnchars(n,scbf);
	if(scbf[0]=='F') {
		def_font = 1;
		for(j=0;j<MXFONT;j++) fontrcv[j] = 0, font_notav[j] = 0;
	}
	else def_font = 0;
	noscale = scbf[1] - '0';
	jtag(1,"Version 4:");	/*that initializes placelab*/
	if(noscale) jtag(0,"NO");
	jtag(0,"scaling of text");
	/* send to host info abt Drect and memory */
	s_char('^');
	s_rect(board);
	s_short(m);
	s_char('$');
	s_flush();
	n = g_short();	/* receive from host info abt initialization */
	getnchars(n,scbf);
	jtag(0,scbf);	/* display it */
	cursswitch(&livemouse);
	get_help("all");

	for(;;sleep(2),wait(MOUSE)){
		if(button123()) checkwin();	/*  check if window is changed */
		if(button2()) {
			while(button2()) wait(MOUSE);
			if(MX < board.origin.x){
				/*mouse on menu*/
				subsel = wheremouse(m_size);
				if(vault[c_menu].txr) get_help("shade");
				else
				if(c_menu==7) get_help("C");	/*color menu */
				else {
					if(subsel>=0) get_help(*(auxmen+subsel));
					else get_help("GEN");
				}
				if(subsel>=0 && vault[c_menu].txr)
					rectf(&display,lfside[subsel],dr_mode);
				if(subsel>=0) unselect();
			}
			else {
				if(ptinrect(mouse.xy,bulletin)){
					blitblt(Drect);
				}
				else {
					draw_menu(c_menu=0); /*c_menu=0! basics*/
					spilld();
					jtag(1,"basic");
				}
			}
			goto relax;
		}
		if(button3()) {
			mselect = menuhit(&menu3,3);
			release3();
			if(mselect>=0) {
				if(D_ON && mselect>0 && mselect<8){
					jtag(1,"Remote only");
					goto relax;
				}
				else if(mselect==8 && !D_ON && no_obj) {
					jtag(1,"Local only");
					goto relax;
				}
				c_menu = mselect;
				jtag(1,vault[c_menu].menu_item);
				draw_menu(c_menu);
				if(!c_menu) spilld();
			}
			goto relax;
		}
		if(button1() ) {
			if(subsel<0) jtag(1,vault[c_menu].menu_item);
			else jtag(1," ");
			if(no_obj>CRISIS) {
				jtag(1,"Too many objects. Saved on host");
				transmit();
				purge(0);
				goto relax;
			}
			if(MX < board.origin.x){ /*mouse on menu*/
				release1();
				subsel = wheremouse(m_size);
				if(vault[c_menu].imd>2) vault[c_menu].fun();
				else {
					if(vault[c_menu].imd==1 &&
					(!subsel || subsel==1 || subsel==9) ) {
						vault[c_menu].fun();
						unselect();
					}
				}
				goto relax;
			}
			else {
				if(vault[c_menu].imd)  vault[c_menu].fun();
				else {	/*point to objects*/
					aa = s_locate(obj,no_obj,mouse.xy);
					if(aa) vault[c_menu].fun(aa);
					else jtag(0,"NULL ");
				}
			}
		}
		relax: for(;;wait(MOUSE)) if(!(button123())) break;
	}
}

short spilld(){
	while(stp>dstack) {
		stp--;
		plot_one(stp->a);
		tr_obj(stp->a,stp->x,stp->y);
		plot_one(stp->a);
	}
}

short checkwin(){
	register wt;
	wt = 1;			
	if(P->state & MOVED) {
		wt = 0;
		P->state &= ~MOVED; P->state &= ~RESHAPED;
	}
	else if(P->state & RESHAPED) {
		wt = 0;
		P->state &= ~RESHAPED;
	}
	if(wt) return(0);
	/*	Drect has changed	*/
	oauxmen = 0;
	om_size = 0;	/*that forces set up*/
	draw_menu(c_menu=0);
	sendstring("KK");
	s_rect(board);
	s_flush();
	D_ON = 0;
	jtag(1,"View host after window change");
	adisplay();	/* view host */
	return(1);
}
