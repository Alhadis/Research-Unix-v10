/*	process by user defined system call	*/
/*	assumes \n separator for text		*/

#include "hped.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#define TOO_MANY 2000

struct stat sbf;

extern char user_com[64];

extern char *mktemp();

char cmdline[128];

char txbuf[1024];
Point ptbuf[1024];

char pedtmp1[32] = { "pXXXXXX" };
char pedtmp2[32] = { "qXXXXXX" };
short noname = 1;

utrans(a,bb)
	objP a, *bb;
{
	register i, j;
	register Point *pp;
	register char * txp;
	objP b, fam_tmp[4];
	char chbuf[128], *chp, tst_char;
	double dx, dy;
	short n;

	if(user_com[0]==' ') {	/*no command*/
		sprintf(mailer," No user defined command");
		goto abend;
	}

	if(noname){
		mktemp(pedtmp1);
		mktemp(pedtmp2);
		noname = 0;
	}

	b = dupl_obj(a);

	if( (fd1 = fopen(pedtmp1,"w")) == 0){
		sprintf(mailer," Cannot create tmp. file 1");
		goto abend;
	}

	n = TOO_MANY;

	switch(b->type){
	case 't':
		fprintf(fd1,"%s\n",b->txt+2);
		break;
	case 'f':
		fputs("o 8i\nr 0 1000 1000 0\n",fd1);
		wr_one(b);
		fputs("e\n",fd1);
		break;
	case 'p': case 'P': case 's': case 'S': case 'Q':
		for(i=0,pp=b->p0; i<b->n; i++,pp++) {
			if(pp->t<0) fputs("1e6\n",fd1);
			else fprintf(fd1,"%g %g\n",pp->x,pp->y);
		}
		fputs("1e6\n",fd1);		
		break;
	case 'c':
		sprintf(mailer," No user def. proc. for circles");
		goto abend;
	}
	fflush(fd1);
	fclose(fd1);

	if(b->type=='f') {
		if(stat(pedtmp1,&sbf)) {
			sprintf(mailer,"pedtmp1=%s GOT LOST",pedtmp1);
			goto abend2;
		}
		n = 2*sbf.st_size;
	}

	sprintf(cmdline,"%s < %s > %s",user_com,pedtmp1,pedtmp2);
	rrecord(cmdline);
	if(system(cmdline) != 0){
		sprintf(mailer,"Could not exec. %s",cmdline);
		goto abend2;

	}

	if(stat(pedtmp2,&sbf)) {
		sprintf(mailer,"pedtmp2=%s NOT FOUND",pedtmp2);
		goto abend2;
	}
	if(!sbf.st_size || sbf.st_size>n || sbf.st_mode&S_IFDIR) {
		sprintf(mailer,"Improper tmp file 2. s=%d mode=%o",
			sbf.st_size,sbf.st_mode);
		goto abend2;
	}

	if( (fd1 = fopen(pedtmp2,"r"))==0) {
		sprintf(mailer," Cannot open tmp. file 2");
		goto abend2;
	}
	
	tst_char = getc(fd1);
	if(isascii(tst_char)) ungetc(tst_char,fd1);
	else {
		sprintf(mailer," tmp. file 2 is not ascii");
		goto abend;
	} 
	
	switch(b->type){
	case 't':
		txp = txbuf;	*txp++ = *b->txt;	*txp++ = *(b->txt+1);	
		while(fgets(chbuf,sizeof(chbuf),fd1) != 0){
			chp = chbuf;
			while(*chp) *txp++ = *chp++;
			if(*(txp-1) != '\n') *txp++ = '\n';
			if( (txp-txbuf) > (sizeof(txbuf)-sizeof(chbuf)) ) {
				sprintf(mailer,"Too long text");
				goto abend;
			}
		}
		*txp=0;
		if((j=strlen(txbuf))>strlen(b->txt)){
			Free(b->txt);
			b->txt = malloc((unsigned)(j+1));
		}
		strcpy(b->txt,txbuf);
		break;
	case 'f':
		fclose(fd1);	/*cludge*/
		n = 0;
		rd_ped(pedtmp2,fam_tmp,&n);
		if(n != 1) {
			sprintf(mailer,"%d objects returned",n);
			goto abend;
		}
		b = fam_tmp[0];
		break;
	case 'p': case 'P': case 's': case 'S': case 'Q':
		i = 0;
		while(fgets(chbuf,sizeof(chbuf),fd1) != 0){
			if(chbuf[1]=='e') {
				ptbuf[i].x = ptbuf[i].y = ptbuf[i].t = -1;
			}
			else {
				chp = chbuf;
				ptbuf[i].x = grab(&chp);
				ptbuf[i].y = grab(&chp);
				ptbuf[i].t = 0;
			}
			i++;
			if(i>sizeof(ptbuf)){
				sprintf(mailer,"Too many points");
				goto abend;
			}
		}
		i--;	/* to get rid of last delimiter */
		dx = (b->p0)->x - ptbuf[0].x;
		dy = (b->p0)->y - ptbuf[0].y;
		if(i>b->n) {
			Free(b->p0);
			b->p0= (Point *)malloc((unsigned)((i+4)*sizeof(Point)));
			if(!b->p0) error(1," Bailout");
		}
		b->n = i;
		for(i=0;i<b->n;i++) { ptbuf[i].x += dx; ptbuf[i].y += dy; }
		for(i=0,pp=b->p0; i<b->n; i++,pp++) *pp = ptbuf[i];
		break;
	}
	fclose(fd1);
	ex_one(b);

	*bb = b;
	return(0);
	abend:
		fclose(fd1);
	abend2:
		rrecord(mailer);
		return(-1);
}
