#include "jped.h"

char garbage[] = {" Garbage from Host"};

short user_com;
extern short not_obj();

#define NOOB ((objP)0)
#define LZERO ((long)0)

short g_char()
{
	register short c;
	while ((c = rcvchar()) == -1)
		wait (RCV);
	return c;
}

short g_short()
{
	char c[2];
	register a;
	short j;
	a = g_char();	c[0] = a&0377;
	a = g_char();	c[1] = a&0377;
	j = c[0];	j &= 0377;	j |= c[1]<<8;
	return(j);
}

short getnchars(k,s) char *s;
{
	register j;
	register char *sp;
	register chabf;
	for(j=0, sp=s; j<k; j++,sp++){
		while( (chabf = rcvchar())== -1)
			wait(RCV);
		*sp = chabf;
	}
	*sp = 0;
	return(atoi(s));
}

#define HBFSIZE 256
#define HBFLIMIT 255
#define HBFLIMIT2 254
char outbf[HBFSIZE];
short nout = 0;

short s_char(k) char k;
{
	outbf[nout++] = k;
	if(nout>HBFLIMIT) s_flush();
}

short s_short(n) short n;
{
	char c[2];
	c[0] = n & 0377;
	c[1] = (n>>8) & 0377;
	if(nout>HBFLIMIT2) s_flush();
	outbf[nout++] = c[0];
	outbf[nout++] = c[1];
}

short s_string(n,s) char *s;
{
	register j;
	if((nout+n)>HBFLIMIT) s_flush();
	for(j=0;j<n;j++) outbf[nout++] = s[j];
}

short s_flush(){
	if(nout) sendnchars(nout,outbf);
	nout = 0;
}

/* Unbuffered. It communicates commands to host */

short sendstring (s)
char *s;
{
	register short n;
	s_flush();
	n = strlen (s);
	s_short(n);
	s_string(n,s);
	s_flush();
}

/* send rectangle */

short s_rect(r)
	Rectangle r;
{
	s_short(r.origin.x);
	s_short(r.origin.y);
	s_short(r.corner.x);
	s_short(r.corner.y);
}

/* direct line to host (actually typing instructions) */

short direct()
{
	register n, g;
	Texture *oldcur;
	if(no_obj) transmit();	/*wasteful, but safe*/
	sendstring("DD");

	/* flush junk */ while(kbdchar() != -1);
	oldcur = cursswitch(&deadmouse);
	jtag(1," ");
	for(;;sleep(2)){
		g = wait(KBD|MOUSE);
		if(g&MOUSE) {
			if(button123()) strcpy(scbf,"m");
			else if(!(g&KBD)) continue;
		}
		if(g&KBD) {
			jtag(1," ");
			n = kbdstr(scbf,placelab,0);
			if(n<1) strcpy(scbf,"m");
		}
		placelab.x += strwidth(&defont,scbf);
		if( (scbf[0]=='p' || scbf[0]=='b') && !scbf[1] ) {
			sendstring("mm");
			if(scbf[0]=='p') {
				dr_mode = F_STORE;
				sendstring("XX");
				receive('D');
				dr_mode = F_XOR;
			}
			else blitblt(board);
			sendstring("DD");
			cursswitch(&deadmouse);
			continue;
		}
		if(scbf[0]=='u' && scbf[1]) {
			if (scbf[1]==' ') user_com = 1;
			else user_com = 0;
		}
		/*clean house*/
		if(scbf[0]=='e' && (scbf[1]==' ' || n==1) ) {
			purge(0);
			D_ON = 0;
		}
		/*	host will take care of its own cleaning	*/
		sendstring(scbf);
		if(scbf[0]=='m') break;
		do {
			n = g_short();
			getnchars(n,scbf);
			jtag(0,scbf);
		} while( scbf[0] != 'H');
	}
	rectf(&display,bulletin,F_STORE);
	cursswitch(oldcur);
	return(-1);
}


/*	if no display, bring all, else select area	*/

short bring_in() {
	Rectangle sweep;
	register k;
	/*	there is display	*/
	if(D_ON) {
		jtag(0," Enclose by rectangle");
		k = ownrec(&sweep);
	}
	if(k || !D_ON) {	/*bring all: empty rect. or no display */
		if(no_obj) transmit();
		jtag(0,"Bringing all");
		sendstring("EE");
		purge(0);
		receive('R');
		return(-1);
	}
	sendstring("CC");
	s_rect(sweep);
	s_flush();
	k = g_short();
	getnchars(k,scbf);
	D_ON = 0;
	purge(0);	/*host has taken care of bb*/
	switch(scbf[0]){
	case 'N':
		jtag(0,"  Nothing found");
		break;
	case 'T':
		receive('R');
		break;
	default:
		jtag(0,scbf);
	}
	return(-1);
}

/* transmit */
short transmit()
{
	register i;
	register objP a;
	Texture *oldcur;
	register touch = 0;

	old_fam = 0;
	for(i=0;i<no_obj;i++){
		a = obj[i];
		if(!a) continue;
		if( a->status > 10 || !a->status ) {
			/*	status=11 is modified	*/
			touch = 1;
			break;
		}
	}
	if(!touch) return(0);
	sendstring("RR");
	oldcur = cursswitch(&wave1);
	jtag(0," to host ");
	for(i=0;i<no_obj;i++){
		a = obj[i];
		if(!a) continue;
		trans_one(a);
		if(!a->status) a = 0;	/*forget it after informing host*/
	}
	s_char('Q');
	s_flush();
	g_char();	/* get a char from host */
	cursswitch(oldcur);
	return(-1);
}

short trans_one(a) objP a;
{
	register j;
	register pointP pp;
	short n;
	if(a->status<0) a->status = -a->status;
	if( a->n<=1 && (a->type=='p' || a->type=='P' || a->type=='Q' ||
		a->type=='s' || a->type=='S') ) a->status = 0;
	s_char(a->status);
	if(!a->status) return(0);	/*erased object*/
	s_short(a->n);
	s_short(a->size);
	s_short(a->ps);
	s_char(a->type);
	s_char(a->color);
	s_char(a->pen);
	if(a->type=='t' || a->type=='s' || a->type=='S'){
		n = strlen(a->txt);
		s_short(n);
		s_string(n,a->txt);
	}
	cursswitch(&wave2);
	for(pp=a->p0,j=0; j<a->n; pp++,j++) {
		s_short(pp->x);	s_short(pp->y);
	}
	if(a->type=='f'){
		for(j=0;j<a->size;j++){
			trans_one(a->mem[j]);
		}
		s_char('Q');
	}
	cursswitch(&wave1);
	return(1);
}

/* receive */

objP rec_one(b)
	objP b;
{
	register j;
	register objP a;
	register pointP pp;
	short argm;
	short n;
	char c1;

	cursswitch(&wave2);
	if(b != NOOB) {
		a = b;
		argm = 1;
	}
	else {
		a = (objP)Alloc((unsigned)(sizeof(objS)));
		a->p0 = LZERO;	a->txt = LZERO;	a->mem = LZERO;
		argm = 0;
	}
	a->status = g_char();
	a->n = g_short();
	a->size = g_short();
	a->ps = g_short();
	while(g_char() != '#');
	cursswitch(&wave1);
	a->type = g_char();
	a->color = g_char();
	a->pen = g_char();
	if(a->type=='t' || a->type=='s' || a->type=='S') {
		if(g_char() != '#') return(0);;
		n = g_short();
		if(a->txt==LZERO) a->txt = Alloc((unsigned)(n+1));
		getnchars(n,a->txt);
	}
	if(a->p0==LZERO)
	a->p0 = (pointP)Alloc((unsigned)((a->n+2)*sizeof(pointS)));
	for(j=0,pp=a->p0;j<a->n;j++,pp++){
		if(g_char() != 'v') continue;
		pp->x = g_short();
		pp->y = g_short();
	}
#ifdef PRIVATE
	jtag(0,"/");
#endif
	if(a->type=='f'){
		if(a->mem==LZERO)
		a->mem = (objP *)Alloc((unsigned)(a->size*sizeof(objP)));
		for(j=0;j<a->size;j++){
			if( (c1=g_char()) != '^'){
				if(c1=='X') getex();
				else jtag(0,garbage);
				a->mem[j] = NOOB;
				continue;
			}
			if(argm && a->mem[j] != NOOB) rec_one(a->mem[j]);
			else
			a->mem[j] = rec_one((objP)0);
		}
	}
	return(a);
}

short receive(rkind) char rkind;
{
	register objP a;
	register j;
	char c1;
	Texture *oldcur;

	old_fam = 0;
	switch(rkind){
		case 'D':
			jtag(0," Display only");
			purge(0);	/*OK local only*/
			D_ON = 1;
			sendstring("TD");
			break;
		case 'R':
			D_ON = 0;
			sendstring("TT");
			s_short(room);
			s_short(no_obj);
			s_flush();
			if(g_char()=='N') {
				jtag(0," No room");
				return(-1);
			}
			break;
		default:
			return(-1);
	}
	oldcur = cursswitch(&wave1);
	j = 0;
	while( (c1 = g_char()) != '$') {
		if(c1=='^') {
			a = rec_one((objP)0);
			if(!a) break;
			plot_one(a);
			j++;
			if(rkind=='R') obj[no_obj++] = a;
			else Free(0);
		}
		else j += not_obj(c1);
	}
	if(D_ON && !j) D_ON = 0;
	cursswitch(oldcur);
	return(-1);
}

short zoom(k)
	short k;
{
	transmit();
	if(!k) sendstring("ZZ");
	else   sendstring("UU");
	if(D_ON) receive('D');
	else {
		purge(0);	/*OK local*/
		receive('R');
	}
	return(-1);
}

short no_kill(){
	Texture *oldcur;
	oldcur = cursswitch(&skull);
	while (!button123())  wait(MOUSE);
	if (button3()) {
		while(button3()) wait(MOUSE);
		cursswitch( (Texture *)0 );
		return(0);
	} else {
		while(button12()) wait(MOUSE);
		cursswitch(oldcur);
		return(1);
	}
}

short get_out(){
	if(no_kill()) return(-1);
	if(no_obj) transmit();
	exalloc();
	sendstring("qq");
	return(0);
}


short bailout()
{
	jtag(1,"No space");
	cursswitch(&skull);
	for(;;wait(MOUSE)) if(button123()) break;
	/*	make effort to salvage	*/
	/*	it will not work when in receive mode	*/
	exalloc();
	sendstring("qq");
	exit();
}

short affine(pa,pb,pc)
	Point pa, pb, pc;
{
	sendstring("AA");
	s_short(pa.x);	s_short(pa.y);
	s_short(pb.x);	s_short(pb.y);
	s_short(pc.x);	s_short(pc.y);
	return(0);
}

short getex(){
	char c;
	short x1, x2, y;
	short xt;
	Texture *shade;

	while((c=g_char()) != 'X'){
		if(c>='E' && c<='P') shade = T_tab[c-'E'];
		else shade = T_tab[0];
		x1 = g_short();	x2 = g_short(); y = g_short();
		if(x1>x2) { xt = x2; x2 = x1; x1 = xt; }
		texture(&display,Rect(x1,y,x2,y+1),shade,dr_mode);
	}
}

short not_obj(c)	/* receive other than object from host */
	char c;
{
	register i;

	switch(c){
	case 'F':
		loadfont();
		break;
	case 'X':
		getex();
		return(1);
	case 'Y':
		i = g_short();
		jtag(0,"Font here ");
		scbf[0] = i+'0';
		scbf[1] = 0;
		jtag(0,scbf);
		break;
	case 'N':
		jtag(0,"Missing font");
		break;
	default:
		jtag(0,garbage);
	}
	return(0);
}
