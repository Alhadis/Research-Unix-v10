/*	Write Output File from VAX		*/
/* -- save (write) all objs to file `fname' -- */

#include "hped.h"


wr_objs(cfname) char *cfname;
{
	register i, j, knt, netno;
	fd1 = fopen(cfname,"w");
	if(fd1 == 0) {
		sprintf(mailer," Cannot create %s. ",cfname);
		return(-1);
	}
	tot_range(1);
	fprintf(fd1,"o 8i\nr %g %g %g %g\n",xmin,ymax,xmax,ymin);
	netno = knt = 0;
	if(parked) {
		for(j=0;j<work_no;j++){
			if(!work_obj[j]) continue;
			if(!(work_obj[j]->status)) continue;
			knt += wr_one(work_obj[j]);
			netno++;
		}
		parked = 0;
	}
	else {
		for(j=0;j<no_obj;j++) {
			if(!obj[j]) continue;
			if(!(obj[j]->status)) continue;
			knt += wr_one(obj[j]);
			netno++;
		}
		for(j=0;j<bn;j++) {
			if(!bb[j]) continue;
			if(!(bb[j]->status)) continue;
			knt += wr_one(bb[j]);
			netno++;
		}
	}
	fputs("e\n",fd1);
	fclose(fd1);
	sprintf(mailer," %d (%d) objects written in file %s ",netno,knt,cfname);
	return(-1);
}

/* actual writing routine */

wr_one(aa) objP aa;
{
	Point *pp;
	char token, tg;
	register j, i, k;
	register char *cp;
	char *cq;

	k = 0;
	if(!aa) return(0);
	if(!aa->status) return(0);
	fprintf(fd1,"[\n");
	pp = aa->p0;
	token = aa->type == 't'? 'm': aa->type;
	fprintf(fd1,"j %d\n",aa->pen);
	fprintf(fd1,"h %d\n",aa->ps);
	if(aa->type=='Q'){	/*textured object*/
		tg = aa->size? '1': ' ';
		fprintf(fd1,"i%c T%c\n",tg,aa->color);
	}
	else {
		if(aa->type=='t'){
			j = aa->size + 0.5;
			fprintf(fd1,"i  P%d\ni F%c\n",j,aa->color);
		}
		else {
			if(aa->type=='p' || aa->type=='s' 
			|| aa->type=='P' || aa->type=='S') {
				tg = aa->size;
				if(tg<' ') tg += ' ';
				fprintf(fd1,"k%c %c\n",tg,aa->color);
			}
			else if(aa->type=='c') fprintf(fd1,"i %c\n",aa->color);
		}
	}
	if(token=='s' && *(aa->txt)=='0')
	fprintf(fd1,"so %g %g",pp->x,pp->y);
	else {
		if(token=='S' && *(aa->txt)>'0')
			fprintf(fd1,"Sw %g %g",pp->x,pp->y);
		else fprintf(fd1,"%c %g %g",token,pp->x,pp->y);
	}

	k = 1;
	switch(aa->type){
	case 't':
		cq = aa->txt;
		while(cp = pindex(cq,SEPAR)){
			*cp = 0;
			fprintf(fd1,"\nu %s",cq);
			*cp = SEPAR;
			cq = cp+1;
		}
		fprintf(fd1,"\nt %s\n",cq);
		goto telos;
	case 'c':
		fprintf(fd1," %g\n",aa->size);
		goto telos;
	case 'f':
		fputs("\n",fd1);
		for(i=0;i<aa->size;i++) k += wr_one(aa->mem[i]);
		goto telos;
	default:
		fputs("\n",fd1);
	}

	tg = 'v';
	if(aa->type=='s' || aa->type=='S') {
		for(j = 1,pp = aa->p0+1, cp = aa->txt+1; j<aa->n; j++, pp++, cp++){
			if(pp->t<0) {
				tg = 'x';
				continue;
			}
			if(*cp>'0') {
				if(tg=='x') tg = 'X'; 
				else tg = 'w';
			}
			fprintf(fd1,"%c %g %g\n",tg,pp->x,pp->y);
			tg = 'v';
		}
	}
	else {
		for(j = 1,pp = aa->p0+1; j<aa->n; j++, pp++){
			if(pp->t<0) {
				tg = 'x';
				continue;
			}
			fprintf(fd1,"%c %g %g\n",tg,pp->x,pp->y);
			tg = 'v';
		}
	}
	telos:;
	fputs("]\n",fd1);
	return(k);
}
