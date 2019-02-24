/*	cat for PED	*/

#include "hped.h"


char *index();
float atof();

double LININCH=960.;
double Height=0;
double Width=0;

objP obj[MXOBJ];	/* Object pointer array */
short no_obj;	/* Number of obj pointers used */

objP bb[128];
short bn;
short modif;
short def_font;

Rectangle viewport;
char mailer[80];	/* place messages for blit from host I/O	*/
int tsize;
double xmin, ymin, xmax, ymax;
double scale = 1;
short xshft = 0, yshft = 0;

short blit_mem;

short fndefined = 0;

Point pp1, pp2, pp3;
double width, height;
char adjust = 'C';

int fontsent[MXFONT];

FILE *fd0, *fd1, *frecord;
extern short defsize;	/* defined in sub.c */

done(){}

char ranger[128];

int grade = 1, box = 0, fsize = 0;
int compact = 1;
int HOR_OFF = 0;
int VER_OFF = 0;
int XBASE = 0;
int YBASE = 0;
float FAC0 = 7.5;
float FAC = 7.5;	/* conversion from blit to 202, dcan */
short Fraction = 10;	/*spline density*/
float vert_space = 16.;

char vout = 'd';
main(arc,arv)
	char **arv;
{
	short more = 1;
	frecord = fopen("/dev/null","w");
	while(arc>1){
		if(arv[1][0]=='-'){
			switch(arv[1][1]){
			case 'T':
				switch(arv[1][2]){
				case 'a':
					LININCH = 710.;
					FAC0 = FAC = 5.6;
					vout = 'a';
					break;
				case 'd':
				case '2':
					FAC0 = FAC = 7.5;
					LININCH = 960.;
					vout  =  'd';
					break;
				}
				break;
			case 'b':
				box = 1 - box;
				break;
			case 'c':
				compact = 1 - compact;
				break;
			case 'f':
				Fraction = atoi(arv[1]+2);
				break;
			case 'F':
				FAC0 = FAC = atof(arv[1]+2);
				break;
			case 'g':
				grade = 1 - grade;
				break;
			case 'm':
				more =  1 - more;
				break;
			case 's':
				fsize = 1 - fsize;
				break;
			}
		}
		else {
			scoop(arv[1]);
			more = 0;
		}
		arc--;	arv++;
	}
	if(more) scoop(" ");
}

#define OLDTX 0
#define NEWTX 1
extern short txtform;

okfile(s)	/* this duplicates code in hread.c */
	char *s;
{
	if(s[0] != 'o' && s[1] != ' ') {
		sprintf(mailer,"tped: File is in wrong format: %s\n",s);
		return(-1);
	}
	switch(s[2]){
	case '6':
		sprintf(mailer,"tped: Old format file\n");
		return(OLDTX);
	case '8':
		return(NEWTX);
	default:
		sprintf(mailer,"tped: File is in wrong format: %s\n",s);
		return(-1);
	}
}

scoop(fname) char *fname;
{
	FILE *fd0, *fd2;
	char aa[128];
	register char * ap, *bp;
	register j, k;
	int txtscale;
	int ffound;

	j = 0;
	if(fname[0]==' ') {
		fd0 = stdin;
		printf(".lf 1 -\n");
	}
	else {
		fd0 = fopen(fname,"r");
		if(fd0 == NULL) {
			fprintf(stderr,"tped: File %s not found\n",fname);
			return(-1);
		}
		printf(".lf 1 %s\n",fname);
	}
	/* fprintf(stderr,"tped: fsize=%d\n",fsize); */
	while(fgets(aa,128,fd0) != NULL) {
		if(!j && (k=okfile(aa))>=0) {
			if(!k) fprintf(stderr,mailer);
			work(0,fd0,fsize);
			break;
		}
		else j++;
		fputs(aa,stdout);	/*pass all, except between .GS/.GE */
		if(aa[0]=='.' && aa[1]=='G'){
			txtscale = fsize;
			if(aa[2]=='S') {
				/*strip newline*/
				bp = index(aa,'\n');
				if(bp) *bp = NULL;
				ffound = 0;
				for(ap = aa+3 /*.GS*/; *ap && !ffound; ap++) {
					if(*(ap+1)=='=' || *ap=='-'){ /*control*/
						if(*ap=='-') ap++;
						switch(*ap){
						case 'h':
						case 'H':
							Height = atof(ap+2);
							break;
						case 'C':
						case 'L':
						case 'R':
							adjust = *ap;
							break;
						case 'w':
						case 'W':
							Width = atof(ap+2);
							break;
						case 's':
							txtscale = 1 - txtscale;
							break;
						default:
							fprintf(stderr,
							 "tped: %c= ?\n",*ap);
						}
						while(*ap != ' ' && *ap != NULL) ap++;
					}
					else if(*ap != ' '){
						ffound  = 1;
						bp = index(ap,' ');
						if(bp) *bp = 0;
						fd2 = fopen(ap,"r");
						if(fd2 != NULL) {
							work(1,fd2,txtscale);
							fclose(fd2);
						}
						else {
							fprintf(stderr,
							"tped: cannot open %s\n",ap);
							perror("tped");
						}
					}
				}
				if(!ffound) {
					work(0,fd0,txtscale);
				}
			}
		}
	}
	if(fd0 != stdin) fclose(fd0);
	printf(".lf %d\n",j+1);
	return(0);
}

extern float grab();

work(vv,fdes,txtsc)
	FILE *fdes;
{
	char * pr;
	register j;
	double x1, y1, x2, y2;
	register vsp, hsp;
	char chbuf[128];
	int ofsize;

	if(vv) {
		fgets(chbuf,sizeof(chbuf),fdes);
		if((txtform = okfile(chbuf))<0) {
			fprintf(stderr,mailer);
			return(-1);
		}
	}
	j = rd_objs(fdes, obj,&no_obj);
	if(j<=0) return(-1);

	ofsize = fsize;
	fsize = txtsc;
	/* fprintf(stderr,"tped: work_fsize=%d\n",fsize); */

	pr = ranger + 1;
	while(*pr != ' ' && pr != NULL) pr++;
	x1 = grab(&pr);	y2 = grab(&pr);	/*ped puts them that way*/
	x2 = grab(&pr);	y1 = grab(&pr);
	XBASE = x1;
	YBASE = y1;
	FAC = FAC0;	/*reset*/
	vsp = (y2-y1)*FAC + 0.5;
	hsp = FAC*(x2-x1) + 0.5;
	if(vsp<0) vsp = 1;
	if(hsp<0) hsp = 1;
	j = vsp>hsp? vsp: hsp;
	if(j>8500){	/*cludge: should be precomputed*/
		FAC = FAC*8500/(float)j;
		vsp = (y2-y1)*FAC + 0.5;
		hsp = FAC*(x2-x1) + 0.5;
		if(vsp<0) vsp = 1;
		if(hsp<0) hsp = 1;	
	}
	if(Width) {
		Width *= LININCH;
		FAC *= (Width/hsp);
		hsp = Width;
		vsp = FAC*(y2-y1) + 0.5;
	}
	if(Height) {
		Height *= LININCH;
		FAC *= (Height/vsp);
		vsp = Height;
		hsp = FAC*(x2-x1) + 0.5;
	}
	/* OPEN */
	printf(".nr tP \\n(.s\n");	/* save point size */
	printf(".br\n.ne %du\n.rs\n.sp 0.5\n",vsp);
	switch(adjust){
	case 'C':
		printf(".in (\\n(.lu-%du)/2u\n",hsp);
		break;
	case 'R':
		printf(".in \\n(.lu-%du\n",hsp);
		break;
	case 'L':
	default:
		;	/* do nothing */
	}
	if(box) {
		x1 -= 20;	x2 += 20;
		y1 -= 20;	y2 += 20;
		pmove(x1,y1);	pvecr(x2-x1,(double)0);
		pvecr((double)0,y2-y1);	pvecr(x1-x2,(double)0);
		pvecr((double)0,y1-y2);
	}
	/* DRAW */
	draw_all();
	/* CLOSE */
	printf("\n.sp %du\n.sp 0.5\n.in 0\n",vsp);
	printf(".ps \\n(tP\n");	/* restore ps */
	no_obj = 0;	/*get rid of old pictures*/
	Height = Width = 0;	/*reset*/
	adjust = 'C';	/* reset */
	fsize = ofsize;
	return(0);
}
