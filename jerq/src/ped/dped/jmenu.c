/*	menu drawing and selection	*/

#include "jped.h"

extern short typify();

short setup_b(menu_num)
{
	register j, y, dy;
	bulletin.origin = Drect.origin;
	bulletin.corner.x = Drect.corner.x;
	bulletin.corner.y = Drect.origin.y + defont.height+10;
	board.origin.x = Drect.origin.x + strwidth(&defont,"markers(t)")+10;
	board.origin.y = Drect.origin.y + defont.height+10;
	board.corner = Drect.corner;
	y = bulletin.corner.y;
	dy = Drect.corner.y - y;
	menu_num++;
	dy /= menu_num;
	for(j=0;j<menu_num;j++){
		lfside[j].origin.x = Drect.origin.x;
		lfside[j].corner.x = board.origin.x;
		lfside[j].origin.y = y;
		y += dy;
		lfside[j].corner.y = y;
	}
}

short draw_menu(pick)
{
	register i, x1, x2, yoff;
	auxmen = vault[pick].wlist;
	if(auxmen == oauxmen) return(-1);
	oauxmen = auxmen;
	osubsel = subsel = -1;	/*throw selection if new menu*/
	cursswitch(&livemouse);
	m_size = vault[pick].how_many;
	if(om_size != m_size) {
		setup_b(m_size);
		om_size = m_size;
	}
	x1 = Drect.origin.x; x2 = board.origin.x - 1;
	rectf(&display,Rpt(lfside[0].origin,Pt(x2+1,Drect.corner.y)),F_CLR);
	yoff = lfside[0].corner.y - lfside[0].origin.y;
	yoff /= 3;	if(yoff<3) yoff = 3;
	if(vault[pick].txr){
		for(i=0;i<m_size;i++){
			texture(&display,lfside[i],T_tab[i],F_STORE);
		}
		pstring(add(lfside[m_size].origin,Pt(3,yoff)),"unfill");
	}
	else {
		for(i=0;i<=m_size;i++) {
			pstring(add(lfside[i].origin,Pt(3,yoff)),*(auxmen+i));
			segment(&display,
			Pt(x1,lfside[i].corner.y),Pt(x2,lfside[i].corner.y),
			F_STORE);
		}
	}
	pline(Pt(x2,lfside[0].origin.y),Pt(x2,Drect.corner.y));
	return(-1);
}


short wheremouse(menu_num)
	register menu_num;
{
	register i;
	for(i=0;i<=menu_num;i++) {
		if(ptinrect(mouse.xy,lfside[i])) break;
	}
	if(i==subsel) return(i);
	if(i>menu_num) return(-1);
	/*	selection made	*/
	if(vault[c_menu].txr) cursswitch(T_tab[i]);
	else {
		cursswitch((Texture *)0);
		if(osubsel>=0 && osubsel != i )
			rectf(&display,lfside[osubsel],dr_mode);
		rectf(&display,lfside[i],dr_mode);
	}
	osubsel = i;
	return(i);
}

/*	menu definitions and actions		*/


short unselect(){
	if(subsel>=0) rectf(&display,lfside[subsel],dr_mode);
	osubsel = subsel = -1;
	cursswitch(&livemouse);
}

char *menuAA[] = {
	"type comm",
	"view host",
	"bring in",
	"markers(t)",
	"HELP/plot",
	"zoom",
	"unzoom",
	"scroll",
	"size?",
	"blitblt",
	"exit",
	NULL};

short notx, noln;

short starters(){
	Rectangle sweep;
	long dx, dy;
	register i;

	switch(subsel){
	case 0:
		direct();
		break;
	case 1:
		dr_mode = F_STORE;
		adisplay();
		dr_mode = F_XOR;
		break;
	case 2:
		tog_grid(0);
		bring_in();
		for(i=no_obj-1, notx = noln = 1; i>=0 && notx && noln; i--) {
			if(!typify(obj[i])) break;
		}
		if(no_obj) {
			sprintf(scbf," txt ps=%d ln ps=%d",def_size,lin_size);
			jtag(0,scbf);
		}
		break;
	case 3:
		drawpar = 1 - drawpar;
		if(D_ON) {
			dr_mode = F_STORE;
			receive('D');
			dr_mode = F_XOR;
		}
		else plot_all();
		break;
	case 4:
		get_help("all");
		break;
	case 5:
	case 6:
		if(D_ON) dr_mode = F_STORE;
		tog_grid(0);
		zoom(subsel-5);
		dr_mode = F_XOR;
		break;
	case 7:
		scroll();
		break;
	case 8:
		if((ownrec(&sweep))) 	sweep = board;
		dx = (sweep.corner.x - sweep.origin.x)*75;
		dy = (sweep.corner.y - sweep.origin.y)*75;
		dx /= 100;	dy /= 100;	/* (96?) for d202 */
		sprintf(scbf,
			"width=%d.%d height=%d.%d in",dx/100,dx%100,dy/100,dy%100);
		jtag(1,scbf);
		break;
	case 9:
		if(!(ownrec(&sweep))) blitblt(sweep);
		else blitblt(board);
		break;
	case 10:
		get_out();
		break;
	}
	for(;;wait(MOUSE)) if(!(button123())) break;
	unselect();	/*one shot functions*/
}

short typify(a)
	objP a;
{
	register i;

	switch(a->type) {
	case 'Q': case 'p': case 'P': case 's': case 'S':
		lin_size = a->ps;
		noln = 0;
		break;
	case 't':
		def_size = a->ps;
		notx = 0;
		break;
	case 'f':
		for(i=a->size-1;i>=0 && noln && !notx;i--) {
			if(!typify(a->mem[i])) return(0);
		}
	}
	return(noln+notx);
}

short adisplay(){
	Rectangle sweep;
	register k;
	if(D_ON) {	/* already something */
		jtag(0,"Enclose by rectangle");
		k = ownrec(&sweep);
	}
	if(k || !D_ON) {
		if(no_obj) transmit();
		sendstring("XX");
	}
	else {
		sendstring("VV");
		s_rect(sweep);
		s_flush();
	}
	receive('D');
}

short get_help(s)
	char * s;
{
	register k;
	Point cp;
	sendstring("HH");
	s_short(c_menu);
	sendstring(s);
	cp = board.origin;
	cp.x += 6;
	cp.y += 6;
	rectf(&display,board,F_CLR);
	while( (k=g_short()) != 0) {
		getnchars(k,scbf);
		pstring(cp,scbf);
		cp.y += (defont.height + 2);
	}
	while(!button123()) wait(MOUSE);
	rectf(&display,board,F_CLR);
	while(button123()) wait(MOUSE);
	if(D_ON) {
		dr_mode = F_STORE;
		receive('D');
		dr_mode = F_XOR;
	}
	else plot_all();
}

short scroll()
{
	Point cs, ct;

	if(no_obj) transmit();
	cs.x = (Drect.origin.x + Drect.corner.x)/2;
	cs.y = (Drect.origin.y + Drect.corner.y)/2;
	ct = cs;
	for(;;nap(2),wait(MOUSE)){
		pline(cs,ct);
		ct = mouse.xy;
		pline(cs,ct);
		if(button123()) break;
	}
	ct.x -= cs.x;
	ct.y -= cs.y;
	sendstring("YY");
	s_short(ct.x);
	s_short(ct.y);
	s_flush();
	if(D_ON) {
		dr_mode = F_STORE;
		receive('D');
		dr_mode = F_XOR;
	}
	else if(no_obj) {
		purge(1);
		receive('R');
		if(grsize){
			sendstring("GG");
			xshift = g_short();
			yshift = g_short();
			plot_grid();
		}
	}
}

char *menuCL[]={
	"black",
	"red",
	"green",
	"yellow",
	"blue",
	"magenta",
	"cyan",
	"white",
	NULL};
short color(a)
	objP a;
{	
	if(!a) return(-1);
	if(!a->status) return(-1);
	if(a->type=='f') return(-1);
	if(subsel<0) return(-1);
	plot_one(a);
	a->pen = subsel%8;
	plot_one(a);
}
