/*	Read Input File for VAX part of PED		*/

#include "hped.h"

#include "device.h"

#ifdef NO_INTER
extern char ranger[];
#endif

/* grab number from string and adjust pointer * */
extern float atof();

float grab(aa) char **aa;
{
	float tn;	char *b;
	b = *aa;
	while(*b==' ') b++;	tn = atof(b);
	do {	b++;	} while( *b != ' ' && *b != 0);
	*aa = b;
	return(tn);
}


/* -- read objs from file `fd1' -- */

#define OLDTX 0
#define NEWTX 1

char *chbfp, token, tick;
char chbuf[1024];

char txbuf[2048];
char *txbp;
short txtform;

rd_ped(cfname, objin, ano_objin)
	char *cfname;
	objP objin[];
	short * ano_objin;
{
	FILE * fdt;
	short kk;

	fdt = fopen(cfname,"r");
	rrecord(cfname);
	if(fdt == 0) {
		sprintf(mailer," File %s not found\n",cfname);
		return(-1);
	}
	fgets(chbuf,sizeof(chbuf),fdt);
	if(chbuf[0] != 'o' || chbuf[1] != ' ') {
		sprintf(mailer," File %s is in wrong format",cfname);
		return(-1);
	}
	switch(chbuf[2]){
	case '6':
		txtform = OLDTX;
		rrecord("Old format file %s",cfname);
		break;
	case '8':
		txtform = NEWTX;
		rrecord("New format file %s",cfname);
		break;
	default:
		sprintf(mailer," File %s is in wrong format",cfname);
		return(-1);
	}
	/*	for portability to tped which calls directly rd_obj	*/
	kk = rd_objs(fdt,cfname,objin,ano_objin);
	fclose(fdt);
	return(kk);
}

#ifdef NO_INTER
rd_objs(fdt, objin, ano_objin)
	FILE *fdt;
	objP objin[];
	short * ano_objin;
#else
rd_objs(fdt, cfname, objin, ano_objin)
	FILE *fdt;
	char *cfname;
	objP objin[];
	short * ano_objin;
#endif
{
	register j;
	register netno;

	fd1 = fdt;
	netno = 0;
	j = 0;
	while(fgets(chbuf,sizeof(chbuf),fd1) != 0) {
		if(j>=MXOBJ) goto outside;
		switch(chbuf[0]){
		case '[':
			j += read_one(objin,ano_objin);
			netno++;
			break;
		case ']':
			rrecord("unbalanced ]");
			break;
		case 'r':
#ifdef NO_INTER
			strcpy(ranger,chbuf);
#else
			rrecord(chbuf);
#endif
			continue;
		case 'e':
			goto outside;
		default:
			rrecord("loose change %s",chbuf);
			continue;
		}
	}
	outside:
#ifndef NO_INTER
	rrecord(" %d objects read from file %s ",j,cfname);
	sprintf(mailer," %d (%d) objects read from file %s ",netno,j,cfname);
#endif
	return(j);
}

/*	read one object		*/

read_one(home,nextp) objP home[]; short * nextp;
{
	register j;
	Point pbuf[MXPTS], *pbfp;
	char font, color, pen, arhead;
	short ps = 9;
	short x1, y1;
	objP tmpob[128];
	short tmpo_n;
	char first_c;

	color = SOLID;	font = ROMAN;	arhead = ' ';
	pen = 0;
	token = ' ';
	j = 1;
	pbfp = pbuf;
	while(fgets(chbuf,sizeof(chbuf),fd1) != 0) {
		chbfp = chbuf;
		first_c = *chbfp++;	/* point past init. symbol*/
		switch(first_c){
		case ']':	/* finished with object */
			if(pbfp>pbuf)
			home[(*nextp)++] =
				mk_obj(pbuf,pbfp,token,color,arhead,pen,tick,ps);
			return(j);
		case 'f':
			rrecord("reading family");
			x1 = grab(&chbfp);	y1 = grab(&chbfp);
			tmpo_n = 0;
			while(fgets(chbuf,sizeof(chbuf),fd1)){
				switch(chbuf[0]){
				case '[':
					j += read_one(tmpob,&tmpo_n);
					break;
				case ']':
					home[(*nextp)++]=mk_fam(x1,y1,tmpo_n,tmpob);
					return(j);
				default:
					rrecord("unbalanced  %s",chbuf);
				}
			}
		case 'm':	/* text should be found soon */
			txbp = txbuf;
		case 'p': case 'P': case 's': case 'S':
		case 'Q':
			token = first_c;
			tick = *chbfp;
			if(tick != ' ') chbfp++;
		case 'v':
			pbfp->x = grab(&chbfp);	pbfp->y = grab(&chbfp);
			pbfp->t = 0;
			pbfp++;
			continue;
		case 'w':
			pbfp->x = grab(&chbfp);	pbfp->y = grab(&chbfp);
			pbfp->t = 1;
			pbfp++;
			continue;
		case 'x':
		case 'X':
			pbfp->x = pbfp->y = pbfp->t = -1; pbfp++;
			pbfp->x = grab(&chbfp);	pbfp->y = grab(&chbfp);
			if(first_c=='X') pbfp->t = 1;
			else pbfp->t = 0;
			pbfp++;
			continue;
		case 't':
			while(*chbfp==' ') chbfp++; /* skip blanks */
			while(*chbfp) *txbp++ = *chbfp++; *txbp = 0;
			if(txtform==OLDTX){
				for(txbp=txbuf; *txbp; txbp++)
					if(*txbp=='|') *txbp = SEPAR;
			}
			home[(*nextp)++] =
				mk_text(txbuf,(pbfp-1),font,tsize,pen,ps);
			pbfp = pbuf;
			continue;
		case 'u':
			while(*chbfp==' ') chbfp++; /* skip blanks */
			while(*chbfp) *txbp++ = *chbfp++;
			if(*(txbp-1)=='\n') txbp--;
			*txbp++ = SEPAR; *txbp = 0;
			/* add check for space */
			continue;
		case 'c':
			pbfp->x = grab(&chbfp);	pbfp->y = grab(&chbfp);
			pbfp++;
			pbfp->x = grab(&chbfp);
			home[(*nextp)++] =
				mk_circ((pbfp-1),(int)(pbfp->x),color,pen,ps);
			pbfp = pbuf;
			continue;
		case 'h':
			while(*chbfp==' ') chbfp++;	/*skip blanks*/
			ps = atoi(chbfp);
			if(ps<1) ps = 9;
			continue;
		case 'i':
		case 'k':
			arhead = *chbfp++;
			while(*chbfp==' ') chbfp++;	/*skip blanks*/
			if(*chbfp=='F' && first_c=='i') font = *(chbfp+1);
			else if(*chbfp=='P' && first_c=='i') tsize = atoi(chbfp+1);
			else if(*chbfp=='T') color = *(chbfp+1);
			else color = *chbfp;
			continue;
		case 'j':
			while(*chbfp==' ') chbfp++;	/*skip blanks*/
			pen = *(chbfp) - '0';
			continue;
		default:
			continue;
		}
	}
	return(0);
}
