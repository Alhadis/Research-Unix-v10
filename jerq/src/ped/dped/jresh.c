
#include "jped.h"

extern short eff_size;
extern short user_com;

extern short effs();

extern char story[];

objP salvage;
short isalvage, jseat;

objS rock;

objP old_fam;
short old_end;

#define UNDO_1 10
#define UNDO_2 9

short rdup(a)
	objP a;
{
	register i;
	register pointP pp, qq;
	rock.status = a->status;
	rock.type = a->type;
	rock.n = a->n;
	rock.size = a->size;
	rock.color = a->color;
	rock.pen = a->pen;
	rock.ps = a->ps;
	rock.p0 = pbuf;
	rock.txt = story;
	rock.n = a->n;
	if(a->txt) strcpy(rock.txt,a->txt);
	for(i=0,pp=a->p0,qq=rock.p0;i<a->n;i++) *qq++ = *pp++;
}

short obseat(a)
	objP a;
{
	register i;
	for(i=0;i<no_obj;i++) if(obj[i]==a) return(i);
	jtag(0,"?");
	return(-1);
}


char *menuSP[] = {
	"formal",
	"spline(t)",
	"arrow(t)",
	"corner(t)",
	"mv point",
	"del pnt",
	"insrt pnt",
	"close",
	"dis fam",
	"rest fam",
	"undo",
	NULL};


short reshape(a)
	objP a;
{
	register i;

	if(!a) {
		jtag(0,"ZERO");
		return(-1);
	}
	if(subsel != UNDO_1) {
		isalvage = obseat(a);
		if(a->type != 'f') {
			rdup(a);	salvage = &rock;
		}
		else salvage = 0;
	}
	if(nosk && !drawpar) eff_size = effs(a->ps);
	switch(subsel){
	case 0:
		if( (jseat = obseat(a))<0) break;
		host_one(jseat,"formal");
		break;
	case 1:
		poly_to_spl(a);
		break;
	case 2:
		put_arrow(a);
		break;
	case 3:
		repeat_pt(a);
		break;
	case 4:
		mv_pt(a);
		break;
	case 5:
		zap_pt(a);
		break;
	case 6:
		insrt_pt(a);
		break;
	case 7:
		if(a->type == 'p' || a->type == 's') {
			plot_one(a);
			a->type += ('P'-'p');
			plot_one(a);
		}
		break;
	case 8:
		if(a->type != 'f') break;
		old_fam = a;
		old_end = dis_fam(a);
		break;
	case 9:
		if(!old_fam) break;
		for(i=0;i<old_fam->size;i++){
			if(a==old_fam->mem[i]) goto found;
		}
		break;
		found:
		old_fam->status = 11;
		for(i=0;i<old_fam->size;i++) {
			old_fam->mem[i] = obj[old_end+i];
			dr_link(old_fam,old_fam->mem[i]);
		}
		no_obj = old_end;
		break;
	case 10:
		obj_restore(a);
		break;
	}
	/*	this has nothing to do with undo	*/
	if(salvage && subsel>=3 && subsel <7) {
		plot_one(salvage);
		plot_one(a);
	}
	eff_size = 0;
	return(0);
}

/*	poly to spline or spline to poly	*/
/*	3 = 's' - 'p'				*/

short poly_to_spl(a)
	register objP a;
{
	register j;
	plot_one(a);
	if(a->type=='p' || a->type=='P') {
		if(!a->txt)
		a->txt = Alloc((unsigned)(a->n+1));
		for(j=0;j<a->n;j++) a->txt[j] = '0';
		if(a->type=='p') {
			a->txt[0] = '1';
			a->txt[a->n-1] = '1';
		}
		a->type += 3; /* s or S */
		if(a->color==DOUBLE || a->color==HEAVY || a->color=='i')
			 a->color = SOLID;
	}
	else if(a->type=='s' || a->type=='S') {
		a->type -= 3;	/*undo*/
		for(j=0;j<a->n;j++) a->txt[j]='0';
	}
	plot_one(a);
	return(0);
}

/* make double points for splines (toggle) */

short repeat_pt(a)
	register objP a;
{
	register j;
	if(a->type != 's' && a->type != 'S') return(-1);
	if(!a->txt) return(-1);
	j = lockpnt - a->p0;
	if(*(a->txt + j)=='0') *(a->txt+j) = '1';
	else *(a->txt+j) = '0';
	return(0);
}


#ifdef OWNCLOSE
/* close */

clop_obj(a)
	register objP a;
{
	if(no_line(a->type)) return(-1);
	if(fullob(a)) return(-1);
	plot_one(a);
	if(klosed(a->type)){
		a->type -= ('A' - 'a');
		if(a->txt) {
			a->txt[0] = '1';
			a->txt[a->n-1] = '1';
		}
	}
	else {
		a->type += ('A'-'a');
		if(a->txt) {
			a->txt[0] = '0';
			a->txt[a->n-1] = '0';
		}
	}
	if(a->type=='S') a->size = ' ';
	plot_one(a);
	return(0);
}
#endif

/* place arrow */

short put_arrow(a)
	register objP a;
{
	char tag;
	if(no_line(a->type)) return(-1);

	tag = lockpnt==(a->p0+a->n-1) ? 'b': lockpnt==a->p0 ? 'a': ' ';
	if(tag==' ') return(-1);
	if(tag==a->size){
		plot_arrow(a,a->size);	/*erase*/
		a->size = ' ';
		return(0);
	}
	/*	tag not equal to a->size	*/
	/*	no previous arrow	*/
	if(a->size==' '){	
		a->size = tag;
		plot_arrow(a,a->size);
		return(0);
	}
	plot_arrow(a,a->size);
	if(a->size=='c'){	/*erase one of them*/
		if(tag=='a') a->size = 'b'; else a->size = 'a';
	}
	else a->size = 'c';
	plot_arrow(a,a->size);
	return(0);
}

/* -- delete a point -- */
short zap_pt(a)
	register objP a;
{
	register pointP pnt, pa;
	register j, k;
	register char *sp;
	if(no_line(a->type)) return(-1);
	zapper:
	if(a->n<=1){	/*erase object*/
		jtag(0,"object erased");
		plot_one(a);
		if(drawpar) bmark(*(a->p0),ball);
		a->status = 0;
		return(0);
	}
	a->status = 11;
	pa = lockpnt;	lockpnt = 0;
	if(pa == 0) return(-1);
	j = pa - a->p0;
	a->n--;
	if(j==a->n){	/*erase last pnt*/
		if((a->p0+a->n-1)->x < SILLY) {
			a->n--; if(a->n<1) goto zapper;
			j--;
		}
		if(a->type=='s') (*(a->txt+j-1))++;
	}
	else {
		k = 1;
		if(!j){	/* handle first point */
			if((a->p0+1)->x < SILLY ) {
				a->n--;  if(a->n<1) goto zapper;
				k = 2;
			}
			if(a->type=='s') (*(a->txt+k))++;
		}
		for(pnt=pa;j<a->n;j++,pnt++) *(pnt) = *(pnt+k);
		if(a->type=='s')
		for(sp=a->txt+j,j=pa-a->p0;j<a->n;j++,sp++) *sp = *(sp+k);
	}
	return(0);
}

/* -- Move a point -- */

short mv_pt(a)
	register objP a;
{
	register pointP pnt;
	if(no_line(a->type)) return(-1);
	pnt = lockpnt;	lockpnt = 0;
	if(pnt == 0) return(-1);
	a->status = 11;
	release1();
	pl_poly(a,pnt-1,pnt+1);
	for(;;wait(MOUSE)){
		nap(2);
		pl_poly(a,pnt-1,pnt+1);
		pnt->x = GMX;	pnt->y = GMY;
		pl_poly(a,pnt-1,pnt+1);
		if(button123()) break;
	}
	pl_poly(a,pnt-1,pnt+1);
	return(0);
}

/*	insert point in contour	*/

short insrt_pt(a)
	objP a;
{
	Point * pnt, *pp;
	register i, j, k, kn;
	static Point pb, pa, pc;
	char *ap, *tp;

	if(no_line(a->type)) return(-1);
	pnt = lockpnt;	if(!pnt) return(-1);
	k = lockpnt - a->p0;
	lockpnt = 0;
	a->status = 11;
	release1();
	pb = *pnt;
	if(!rubber(pb,&pa)) return(-1);	/*cancel*/
	release1();
	pline(pb,pa);
	if(!rubber(pa,&pc)) goto abend;
	if(s_locate(obj,no_obj,pc) != a) goto abend;
	if(!lockpnt) goto abend;
	j = lockpnt - a->p0;
	/*	insert pa between j and k position	*/
	if(j<k) { i = j; j = k; k =i; }
	/* now j is greater */
	if(j==k+1 || (j==a->n-1 && k==0)){	/*next point*/
		pline(pb,pa);		
		pp = (pointP)Alloc((unsigned)((a->n+2)*sizeof(pointS)));
		pnt=pp;
		if(j==k+1) kn = j; else kn = a->n;
		for(i=0;i<kn;i++) *pnt++ = *(a->p0+i);
		*pnt++ = pa;
		if(j==k+1) for(;i<a->n;i++,pnt++) *pnt = *(a->p0+i);
		a->n++;	a->p0 = pp;
		if(a->type=='s' || a->type=='S') {
			tp = ap = Alloc((unsigned)(a->n+1));
			for(i=0;i<kn;i++) *tp++ = *(a->txt+i);
			*tp++ = '0';			
			if(j==k+1) for(;i<a->n;i++) *tp++ = *(a->txt+i);
			a->txt = ap;
		}
		return(0);		
	}

	abend: pline(pb,pa);
	return(-1);	/*cancel*/
}

short obj_restore(a)
	objP a;
{
	if( (jseat=obseat(a)) <0) return(0);
	if(jseat != isalvage) return(0);
	unselect();	/*undone used*/
	if(salvage) {
		plot_one(a);
		obj[jseat] = dupl_obj(salvage,(pointP)0,(pointP)0);
		plot_one(salvage);
		salvage = 0;
	}
}

char *menuVV[] = {
	"edit text",
	"adj t/CRL",
	"psize",
	"what size",
	"dashed(t)",
	"double(t)",
	"heavy(t)",
	"isplin(t)",
	"fix",
	"user oper",
	"undo",
	NULL};

short fine_view(a)
	objP a;
{
	Texture * oldcur;

	if(!a) {
		jtag(0,"ZERO");
		return(-1);
	}
	if(subsel != UNDO_2) {
		isalvage = obseat(a);
		if(a->type != 'f') {
			rdup(a);	salvage = &rock;
		}
		else salvage = 0;
	}
	switch(subsel){
	case 0:
		e_text(a);
		break;
	case 1:
		adj_text(a);
		break;
	case 2:
		kbdstr(scbf,placelab,1);
		plot_one(a);
		if(a->type=='t') {
			a->size  = def_size = atoi(scbf);
		}
		else if(a->type != 'f') {
			a->ps = lin_size = atoi(scbf);
		}
		plot_one(a);
		break;
	case 3:
		if(a->type=='t') sprintf(scbf,"size = %d",a->size);
		else sprintf(scbf,"size = %d",a->ps);
		jtag(1,scbf);
		break;
	case 4:
		ch_color(a,DASH);
		break;
	case 5:
		if(a->type != 'p' && a->type != 'P') break;
		ch_color(a,DOUBLE);
		break;
	case 6:
		if(a->type != 'p' && a->type != 'P') break;
		ch_color(a,HEAVY);
		break;
	case 7:
		ch_color(a,'i');
		break;
	case 8:
		if( (jseat = obseat(a)) <0) break;
		host_one(jseat,"ifix");
		break;
	case 9:
		if( (jseat = obseat(a)) <0) break;
		if(!user_com) {
			story[0] = 'u';
			story[1] = ' ';
			story[2] = 0;
			oldcur = cursswitch(&deadmouse);
			if(a->type=='t') plot_one(a);
			if( kbdstr(scbf,*(a->p0),1)>0){
				strcat(story,scbf);
				wipe(*(a->p0),scbf);
			}
			else if(a->type=='t') strcat(story,a->txt+2);
			cursswitch(oldcur);
			if(a->type=='t') plot_one(a);			
			if(strlen(story)>2) sendstring(story);
			else {
				jtag(0,"No oper defined");
				break;
			}
		}
		host_one(jseat,"juser");
		break;
	case 10:
		obj_restore(a);
		break;
	}
	for(;;wait(MOUSE)) if(!(button123())) break;
}

char head[128], tail[128];

short e_text(a)
	objP a;
{
	Rectangle mv;
	register j, w0, w;
	short xleft, len;
	char ct;
	char * sp;
	Texture * oldcur;

	if(a->type != 't') return(-1);
	if((len=strlen(a->txt))>80) return(-1);
	/* flush junk */ while(kbdchar() != -1);
	xleft = bulletin.origin.x + 5;

	jtag(1,a->txt);
	w0 = strwidth(&defont,"abcdefghijklmnopqrstuvwxyz")/26;
	mv.origin.y = bulletin.origin.y + 5;
	mv.corner.y = mv.origin.y + defont.height;
	for(;;sleep(2),wait(MOUSE)){
		if(button23()) break;
		if(!button1()) continue;
		if(MY > bulletin.corner.y) continue;
		w = (MX-xleft)/w0;
		if(w>len) w = len;
		mv.origin.x = xleft + (w*w0);
		mv.corner.x = mv.origin.x + w0;
		rectf(&display,mv,dr_mode);
		release1();
		rectf(&display,mv,dr_mode);
		oldcur = cursswitch(&deadmouse);
		if(w>=len) tail[0] = 0;
		else {
			sp = a->txt + w;
			strcpy(tail,sp);
			*sp = 0;
		}
		strcpy(head,a->txt);
		sp = head + strlen(head);
		/* sp points now to the end of head */
		for(;;){
			jtag(1,head);
			mv.origin.x = placelab.x;
			mv.corner.x = mv.origin.x + w0;
			rectf(&display,mv,dr_mode);
			jtag(0,tail);
			wait(KBD);
			ct = kbdchar();
			rectf(&display,mv,dr_mode);
			if(ct=='\n' || ct=='\r')  break;
			if(ct=='\b') {
				if(sp<=head) continue;
				*(--sp) = 0;
			}
			else {
				*sp++ = ct;	*sp = 0;
			}
		}
		cursswitch(oldcur);
		if( (j = strlen(head) + strlen(tail) ) >=a->n) 
			a->txt =  Alloc((unsigned)(j+1));
		strcpy(a->txt,head);
		strcat(a->txt,tail);
		jtag(1,a->txt);
	}
	jtag(1," ");
	plot_one(salvage);
	plot_one(a);
}

short adj_text(a)
	objP a;
{
	char v;
	short n;

	if(a->type !='t') return(-1);
	plot_one(a);

	/* assume a->txt starts with \\ */
	v = a->txt[1];
	if(v=='C') v = 'R';
	else if(v=='R') v = 'L';
		else v = 'C';
	a->txt[1] = textmode = v;
	plot_one(a);
}

short ch_color(a,c)
	objP a;
	char c;
{		/* toggle for changing color */
	if(no_line(a->type)) return(0);
	plot_one(a);
	if(a->color != c) a->color = c;
	else a->color = SOLID;
	plot_one(a);
}
