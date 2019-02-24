/*	`really main' of ped on VAX	*/

#include "hped.h"

char user_com[64] = {" "};

#define BOUND 1
#define UBOUND 0

objP obj[MXOBJ];	/* Object pointer array */
short no_obj;	/* Number of obj pointers used */

objP work_obj[MXOBJ];
short work_no;

Rectangle park;
short parked;

objP bb[128];
short bn;
short modif;
short no_scale = 0;
short def_font;

Rectangle viewport;
Rectangle Drect;	/*approx blit Drect */
char mailer[128];	/* place messages for blit from host I/O	*/
int tsize;
double xmin, ymin, xmax, ymax;
double scale = 1;
short xshft = 0, yshft = 0;

short blit_mem;

short not_in_blit;

short fndefined = 0;

Point pp1, pp2, pp3;
double width, height;
int fontsent[MXFONT];

short legal_gr[8] = { 4, 8, 16, 32, 64, 128 };
editor(argms,abt_font){
	register j, n;
	register char *sp;
	register keys = 0;	/* commands are typed. Else by mouse */
	char s[80];
	char v[128];
	char tname[64];
	short renew;
	short num_rd;
	char z;
	int i;
	double tmp_x, tmp_y;
	Rectangle tmp_rec;
	Point tmp_pnt;
	char xmailer[72];

	no_obj = 0;
	def_font = abt_font;
	for(i=0;i<MXFONT;i++) fontsent[i] = 0;

	parked = 0;

	z = def_font? 'F': 'N';
	sprintf(s,"%c%d Drect",z,no_scale);
	sendstring(s);
	fflush(toblit);

	while((z=g_char()) != '^') rrecord("%c",z);
	Drect.origin.x = g_short() + 5;
	Drect.origin.y = g_short() + 5;
	Drect.corner.x = g_short() - 5;
	Drect.corner.y = g_short() - 5;
	fprintf(frecord,"DRECT: %g %g %g %g\n",
	Drect.origin.x,Drect.origin.y,Drect.corner.x,Drect.corner.y);
	blit_mem = g_short();
	rrecord("blit has %d bytes of space",blit_mem);
	if((z=g_char()) != '$') {
		error(1,"Synchronization error in DR transfer");
	}
	sprintf(xmailer,"%d bytes ",blit_mem);
	scale_to_1();
	if(argms){
		rd_ped(fname,obj,&no_obj);
		fndefined = 1;
		tot_range(1);
		reset_scal();
		strcat(xmailer,mailer);
		strcpy(tname,fname);
	}
	else strcat(xmailer,"No input file");
	if(strlen(xmailer)>71){
		rrecord("truncated mailer");
		xmailer[71] = 0;
	}
	sendstring(xmailer);
	fflush(toblit);

	modif = 0;
	not_in_blit = 1;

	while( (n = getstring(s)) > 0) {
		sp = s;
		strcpy(v,"H:");
		v[2] = NULL;
		renew = 0;
		if(keys && (*sp>'z' || *sp<'a') ) {
			strcat(v," Unknown command: ");
			strcat(v,s);
			goto cancel;
		}
		if(*sp=='e' || *sp=='w' || *sp=='r' || *sp=='f'){
			if(n>1){
				if(*(sp+1) != ' ') {
					strcat(v," Unknown command: ");
					strcat(v,s);
					goto cancel;
				}
				else {
					while(*(++sp)==' ');
					if(*sp != NULL)	{
						strcpy(tname,sp);
						if(!fndefined) {
							strcpy(fname,tname);
							fndefined = 1;
						}
						renew = 1;
					}
					else strcpy(tname,fname);
				}
			}
			else {	/* n==1 */
				if(!fndefined){
					strcat(v," No file name");
					goto cancel;
				}
				else strcpy(tname,fname);
			}
		}
		sp = s;
		switch(*sp){
		case 'A':	/*data to be used in transforms	*/
			rrecord("expect 3 pts");
			get3();
			rrecord("received 3 pts");
			break;
		case 'B':
			rrecord("blitblt starts");
			getbmap();
			rrecord("blitblt ends");
			break;
		case 'C':
			rrecord("choose");
			choose();
			break;
		case 'D':
			rrecord("keyboard input");
			keys = 1;
			continue;
		case 'E':
			rrecord("place all in bb");
			choose_all();
			break;
		case 'F':
			switch(*(sp+1)){
			case 'Y':
				def_font = 1;
				break;
			case 'N':
				def_font = 0;
				break;
			case 'y':
				no_scale = 0;
				break;
			case 'n':
				no_scale = 1;
				break;
			case 'F':
				i = g_short();
				rrecord("Blit needs font %d",i);
				loadfont(i);
				fflush(toblit);
				rrecord("took care of font");
				break;
			}
			break;
		case 'G':
			switch(*(sp+1)){
			case 'Q':
				rrecord("grid request %g",scale);
				if(scale==1) {
					s_char('Y');
					s_short(xshft);
					s_short(yshft);
				}
				else s_char('N');
				break;
			case 'G':
				s_short(xshft);
				s_short(yshft);
				break;
			}
			fflush(toblit);
			break;
		case 'H':
			{
				char request[32];
				short tmpn;
				tmpn = g_short();
				getstring(request);
				send_help(tmpn,request);
				rrecord("help: %d %s",tmpn,request);
			}
			break;
		case 'I':
			rrecord("scale reset");
			scale = 1;
			tmp_pnt.x = (viewport.origin.x + viewport.corner.x)/2;
			tmp_pnt.y = (viewport.origin.y + viewport.corner.y)/2;
			tmp_x = (Drect.corner.x - Drect.origin.x)/2;
			tmp_y = (Drect.corner.y - Drect.origin.y)/2;
			viewport.origin.x = tmp_pnt.x - tmp_x;
			viewport.corner.x = tmp_pnt.x + tmp_x;
			viewport.origin.y = tmp_pnt.y - tmp_y;
			viewport.corner.y = tmp_pnt.y + tmp_y;
			s_short(xshft);
			s_short(yshft);
			fflush(toblit);
			break;
		case 'K':
			rrecord("blit changed window");
			Drect.origin.x = g_short() + 5;
			Drect.origin.y = g_short() + 5;
			Drect.corner.x = g_short() - 5;
			Drect.corner.y = g_short() - 5;
			fprintf(frecord,"revised DRECT: %g %g %g %g\n",
			Drect.origin.x,Drect.origin.y,
			Drect.corner.x,Drect.corner.y);
			break;
		case 'M':
			break;
		case 'P':
			rrecord("host process starts");
			{
				objP tmpob[4];
				short tmpn;
				tmpn = 0;
				while(rec_one(tmpob,&tmpn));
				rrecord("T=%c tmpn=%d",tmpob[0]->type,tmpn);
				not_in_blit = 0;	/*they came fom there*/
				actupon(*(sp+1),tmpob[0]);
				if(def_font) qfont(tmpob,1);
				trans_one(tmpob[0]);
				s_char('$');
				fflush(toblit);
			}
			rrecord("host process ends");
			break;
		case 'Q':
			rrecord("remote operation");
			g_Rect(&tmp_rec);
			g_Point(&tmp_pnt);
			goperate(sp+1,tmp_pnt,tmp_rec,v);
			modif = 1;
			break;
		case 'R':
			rrecord("receive starts");
			receive();
			modif = 1;
			rrecord("receive ends");
			break;
		case 'T':
			rrecord("transmit starts");
			transmit(sp+1);
			rrecord("transmit ends");
			break;
		case 'U':
			rrecord("unzoom");
			exp_rect(&viewport);
			scal(UBOUND,viewport);
			break;
		case 'V':
			rrecord("viewport");
			g_Rect(&viewport);
			scal(UBOUND,viewport);
			break;
		case 'X':
			rrecord("XX: merged bb into obj"); /*display everything*/
			not_in_blit = 1;
			merge_bb();
			if(no_obj) {
				tot_range(1);
				reset_scal();
			}
			else scale_to_1();
			break;
		case 'Y':
			rrecord("scroll");
			tmp_pnt.x = g_short();
			tmp_pnt.y = g_short();
			tmp_pnt.x /= scale;
			tmp_pnt.y /= scale;
			fprintf(frecord,"V= %g, %g -> %g, %g\n",
			viewport.origin.x,viewport.origin.y,
			viewport.corner.x,viewport.corner.y);
			fprintf(frecord,"move= %g, %g \n",
			tmp_pnt.x,tmp_pnt.y);
			viewport.origin.x -= tmp_pnt.x;
			viewport.origin.y -= tmp_pnt.y;
			viewport.corner.x -= tmp_pnt.x;
			viewport.corner.y -= tmp_pnt.y;
			fprintf(frecord,"V= %g, %g -> %g, %g\n",
			viewport.origin.x,viewport.origin.y,
			viewport.corner.x,viewport.corner.y);
			scal(UBOUND,viewport);
			break;
		case 'Z':
			rrecord("zoom");
			shr_rect(&viewport);
			scal(UBOUND,viewport);
			break;
		case 'c':
			mailer[0] = 0;
			if(*(sp+1)=='d') {
				sp += 2;
				while(*sp==' ') sp++;
				if(!(*sp)) {
					rrecord("Null directory");
					strcat(v,"Null directory name");
				}
				else
				if(chdir(sp)) {
					strcat(v,"No change");
					rrecord("failed to cd to %s",sp);
				}
				else {
					strcat(v,"Changed");
					rrecord("cd to %s",sp);
				}
			}
			break;
		case 'e':
			rrecord("edit new file");
			parked = 0;
			if(no_obj || bn) {	/*already something there*/
				v[0] = 'Z';
				if(modif) {
					wr_objs("ped.save");
					modif = 0;
					strcat(v,mailer);
					sendstring(v);
				}
				rrecord("no_o=%d bn=%d",no_obj,bn);
				purge();
				rrecord("no_o=%d bn=%d",no_obj,bn);
				strcpy(v,"H:");
			}
			if(renew) {
				strcpy(fname,tname);
			}
		case 'r':
			rrecord("read %s",tname);
			num_rd = rd_ped(tname,obj,&no_obj);
			strcat(v,mailer);
			if(*sp=='e'){
				 if(num_rd>0){
					tot_range(1);
					reset_scal();
				}
				else scale_to_1();
			}
			break;
		case 'f':
			rrecord("file name");
			if(renew) strcpy(fname,tname);
			strcat(v,fname);
			break;
		case 'w':
			rrecord("start writing");
			wr_objs(tname);
			strcat(v,mailer);
			rrecord("%s",mailer);
			modif = 0;
			break;
		case 'm':
			rrecord("reactivate mouse");
			keys = 0;
			break;
		case 'p':
			mailer[0] = 0;
			if(*(sp+1)=='w' && *(sp+2)=='d' ) {
				FILE *fpip;
				extern FILE *popen();
				fpip = popen("/bin/pwd","r");
				fgets(mailer,80,fpip);
				pclose(fpip);
				rrecord("pwd=%s",mailer);
				strcat(v,mailer);
			}
			break;
		case 'q':
			if(n==2 && *(sp+1)=='q') goto stopl;
		case 'u':
			strcat(v,"User comm=");
			while(*(++sp)==' ');
			if(!*sp) strcat(v,user_com);
			else {
				if(*sp=='-') {
					strcpy(user_com," ");
					rrecord("Undefine user com.");
					strcat(v,"Undefine user com.");
				}
				else {
					strcpy(user_com,sp);
					rrecord("User defined command =%s",sp);
					strcat(v,sp);
				}
			}
			break;
		default:
			strcat(v," Unknown command: ");
			strcat(v,s);
		}
		cancel:
		if(keys || s[0]=='Q') {
			rrecord("mess. to blit: %s",v);
			sendstring(v);
			fflush(toblit);
		}
	}
	stopl:;
	return(0);
}

scal(bound,v)
	Rectangle v;
{
	fprintf(frecord,"in scal: %g %g   %g %g\n",
		v.origin.x, v.origin.y,
		v.corner.x, v.corner.y);
	if(bound){
		if(f_ins_l(v,Drect)){
			rrecord("object within blitboard");
			scale = 1.;
			xshft = yshft = 0;
			viewport = Drect;
			return(0);
		}
	}
	mapr(bound,Drect,v,&scale,&xshft,&yshft);
}

mapr(bound,r1,r2,sf,dx,dy)	/*map r2 into r1*/
	Rectangle r1, r2;
	double *sf;
	short *dx, *dy;
{
	double xscale, yscale;
	rrecord("in mapr");
	if((r2.corner.x-r2.origin.x) == 0 || (r2.corner.y-r2.origin.y) == 0)
	return(-1);
	xscale = (r1.corner.x - r1.origin.x)/(r2.corner.x - r2.origin.x);
	fprintf(frecord,"%g = %g / %g\n",xscale,
	r1.corner.x-r1.origin.x, r2.corner.x - r2.origin.x);
	yscale = (r1.corner.y - r1.origin.y)/(r2.corner.y - r2.origin.y);
	fprintf(frecord,"%g = %g / %g\n",
	yscale, r1.corner.y-r1.origin.y,r2.corner.y - r2.origin.y);
	if(xscale <= yscale) *sf = xscale;
	else *sf = yscale;
	if(bound && *sf > 1.) *sf = 1.;
	if(*sf>0.95 & *sf<1.05) *sf = 1.;	/*keep scale=1, if possible*/
	*dx = (r1.corner.x + r1.origin.x)/2
		- *sf * ((r2.corner.x+r2.origin.x)/2) + 0.5;
	*dy = (r1.corner.y + r1.origin.y)/2
		- *sf * ((r2.corner.y+r2.origin.y)/2) + 0.5;
	fprintf(frecord,"Scale v: %g,%g -> %g,%g, scale = %g\n",
	r2.origin.x,r2.origin.y,r2.corner.x,r2.corner.y,*sf);
	return(0);
}

reset_scal()
{
	rrecord("resetting scale to max obj dim");
	viewport.origin.x = xmin;
	viewport.origin.y = ymin;
	viewport.corner.x = xmax;
	viewport.corner.y = ymax;
	scal(BOUND,viewport);
}

exp_rect(r)
	Rectangle *r;
{
	double del;
	del = (r->corner.x - r->origin.x)*0.125;
	r->corner.x += del; r->origin.x -= del;
	del = (r->corner.y - r->origin.y)*0.125;
	r->corner.y += del; r->origin.y -= del;
}

shr_rect(r)
	Rectangle *r;
{
	double del;
	del = (r->corner.x - r->origin.x)*0.1;
	r->corner.x -= del; r->origin.x += del;
	del = (r->corner.y - r->origin.y)*0.1;
	r->corner.y -= del; r->origin.y += del;
}

scale_to_1(){
	viewport.origin.x = xmin = Drect.origin.x;
	viewport.origin.y = ymin = Drect.origin.y;
	viewport.corner.x = xmax = Drect.corner.x;
	viewport.corner.y = ymax = Drect.corner.y;
	scale = 1;
	xshft = yshft = 0;
}
