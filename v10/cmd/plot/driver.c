#include <stdio.h>
#include <ctype.h>
#define void int

#ifdef T5620
#include "jcom.h"
extern int openflg;
extern int t;
#endif

#define MAXL 16
double	atof();
char	*strchr();
char *getl();
void	idle();
void	arc();
void	box();
void	circle();
void disc();
void	erase();
void	closepl();
void	color();
void	cfill();
void	pen();
void	frame();
void	grade();
void	ppause();
void	line();
void	move();
void	openpl();
void	point();
void	parabola();
void	range();
void	rmove();
void	text();
void	sbox();
void	vec();
void	rvec();
void	fill();
void	poly();
void	spline();
void	fspline();
void	lspline();
void	dspline();
void	cspline();
void	save();
void	restore();
void define();
void call();
void pinclude();
struct pcall {
	void	(*plot)();
	int	kount;
	char	*cc;
	int	numc;
} plots[] = {
	arc, 	7, 	"a", 	1,
	box, 	4, 	"bo", 	2,
	circle, 	3, 	"ci", 	2,
	disc,	3,	"di",	2,
	erase, 	-1, 	"e", 	1,
	closepl, 	-1, 	"cl", 	2,
	frame, 	4, 	"fr", 	2,
	grade, 	1, 	"g", 	1,
	ppause, 	-1, 	"pau", 	3,
	color, 	0, 	"co", 	2,
	cfill, 	0, 	"cf", 	2,
	pen, 	0, 	"pe", 	2,
	line, 	4, 	"li", 	2,
	move, 	2, 	"m", 	1,
	idle, 	-2, 	"id", 	2,
	openpl, 	0, 	"o", 	1,
	point, 	2, 	"poi", 	3,
	parabola, 	6, 	"par", 	3,
	range, 	4, 	"ra", 	2,
	rmove, 	2, 	"rm", 	2,
	text, 	0, 	"t", 	1,
	sbox, 	4, 	"sb", 	2,
	vec, 	2, 	"v", 	1,
	rvec, 	2, 	"rv", 	2,
	fill, 	256, 	"fi", 	2,
	poly, 	256, 	"pol", 	3,
	spline, 	256, 	"sp", 	2,
	fspline,	256,	"fs",	2,
	lspline,	256,	"ls",	2,
	dspline,	256,	"ds",	2,
	cspline,	256,	"cs",	2,
	save, 	-1, 	"sa", 	2,
	restore, 	-1, 	"re", 	2,
	define,	0,	"de",	2,
	call,	0,	"ca",	2,
	pinclude,	0,	"in",	2,
	0, 	0, 	0, 	0
};
struct pcall *pplots;
struct fcall {
	char *name;
	char *stash;
} flibr[MAXL];
struct fcall *fptr = flibr;
short	FN = 1;
double	SC = 1.0;
char *fstack[64], **fstp;

int num[256],*nn;
double *ff[256], **fp;
char aa[256];
FILE *fd;
main(arc, arv)
int	arc;
char	**arv;
{
	char	*ap;
	fd = stdin;
	for (; arc > 1; arc--, arv++) {
		if (arv[1][0] == '-') {
			ap = arv[1];
			ap++;
			if (*ap == 'T')
				continue;
			if (*ap == 'D') {
				ptype(++ap);
				continue;
			}
			switch (*ap) {
			case 'e':
				erase();
				continue;
			case 'C':
				closepl();
				continue;
			case 'w':
				ppause();
				continue;
			case 'c':
				color(++ap);
				continue;
			case 'f':
				cfill(++ap);
				continue;
			case 'p':
				pen(++ap);
				continue;
			case 'o':
				openpl(++ap);
				continue;
			case 'g':
				grade(atof(++ap));
				continue;
			default:
				fprintf(stderr, "%s not allowed as argument\n",
				    ap);
				exit(1);
			}
			continue;
		}
		if ((fd = fopen(arv[1], "r")) == NULL) {
			perror();
			fprintf(stderr, "Cannot find file %s\n", arv[1]);
			continue;
		}
		if(!process())
			break;
		fclose(fd);
	}
	if (fd == stdin)
		process();
#ifdef T5620
	closep();
#endif
	exit(0);
}
process()
{
	short	acn,curl;
	double	X[512], *xp;
	char	*ap, *aq, truef;
	fstp = fstack;
	fp = ff;
	nn = num;
	while((ap=getl(aa))){
#ifdef T5620
		if(t == TQUIT)
			return(0);
#endif
		while(isspace(*ap)){
			if(*ap == '\n')
				ap=getl(aa);
			else ap++;
		}
		if(*ap == ':')continue;
		while(*ap == '.' && !(isdigit(*(ap+1))))
			ap++;
		if (isupper(*ap))
			*ap = tolower(*ap);
		if (!islower(*ap)){
			if(pplots == 0){
				closepl();
				fprintf(stderr,"file probably not jplot input\n");
				exit(1);
			}
			if (pplots->kount > 1)
				goto aresume;
			continue;
		}
		if(fp != ff){
			if(acn%2 != 0){
#ifdef T5620
				if(openflg) closep();
#endif
				fprintf(stderr,"phase error\n");
				exit(1);
			}
			*nn++ = acn/2;
			*nn = 0;
			(*pplots->plot)(num,ff);
			fp = ff;
			nn = num;
		}
		for (pplots = plots; pplots->plot != 0; pplots++)
			if (strncmp(ap, pplots->cc, pplots->numc) == 0){
				break;
			}
		if (pplots->plot == 0) {
#ifdef T5620
			 if(openflg) closep();
#endif
			fprintf(stderr,"no command for %s\n", ap);
			exit();
		}
		if (pplots->kount < 0) {
			(*pplots->plot)();
			continue;
		}
		if (*ap != '\n')
			while(islower(*ap))
				ap++;
		if (*ap != '\n')
			while (isspace(*ap) || *ap == ',')
				ap++; 		
		if (*ap == '\n') {
			if (!(pplots->kount))
				(*pplots->plot)(" ");
			continue;
		}
		if (!(pplots->kount)) {
			for(aq=ap; *aq !=  '\n';aq++)
				if(*aq == '\\')aq++;
			if (*ap == '"'){
				ap++;
				if(*(aq-1) == '"')
					aq--;
			}
			*aq = NULL;
			(*pplots->plot)(ap);
			continue;
		}
		acn = 0;
		xp = X;
		if(pplots->kount == 256){
			curl = 0;
			*fp++ = X;
		}
aresume:
		while (*ap != '\n') {
			while (isspace(*ap) || *ap == ',') 
				ap++;
			if (*ap == '\n')
				break;
			if(*ap == '{'){
				ap++;
				if(curl==0 || ( curl==1 && acn == 0)){
					curl++;
					continue;
				}
				*fp++ = xp;
				continue;
			}
			if(*ap == '}'){
				ap++;
				curl++;
				if(acn==0){
					*nn = 0;
					(*pplots->plot)(num,ff);
					fp = ff;
					nn = num;
					continue;
				}
				if(acn%2 != 0){
#ifdef T5620
					closep();
#endif
					fprintf(stderr,"phase error\n");
					exit(1);
				}
				*nn++ = acn/2;
				acn = 0;
				continue;
			}
			aq = ap;
			while (!(isspace(*ap)) && *ap != ',' && *ap != '\n')
				ap++;
			if (isdigit(*aq)||(*aq == '-'||*aq == '+'||*aq == '.')){
				*xp++ = atof(aq)*SC;
				if (++acn >= pplots->kount&&pplots->kount != 256) {
					switch (acn) {
					case 1:
						(*pplots->plot)(X[0]);
						break;
					case 2:
						(*pplots->plot)(X[0], X[1]);
						break;
					case 3:
						(*pplots->plot)(X[0],X[1],X[2]);
						break;
					case 4:
						(*pplots->plot)(X[0],X[1],X[2],X[3]);
						break;
					case 6:
						(*pplots->plot)(X[0],X[1],X[2],
							X[3], X[4], X[5]);
						break;
					case 7:
						(*pplots->plot)(X[0],X[1],X[2],
							X[3], X[4], X[5], X[6]);
						break;
					}
					acn = 0;
					xp = X;
				}
			}
		}
	}
	return(1);
}
char	*malloc();
char	*realloc();
char *names = 0;
char *enames = 0;
char *bstash = 0;
char *estash = 0;
unsigned size = 1024;
char *nstash = 0;
define(a)
char	*a;
{
	char	*ap, *aq;
	short	i, j;
	int curly = 0;
	ap = a;
	while(isalpha(*ap))ap++;
	if(ap == a){
		fprintf(stderr,"no name with define\n");
		exit(1);
	}
	i = ap - a;
	if(names+i+1 > enames){
		names = malloc((unsigned)512);
		enames = names + 512;
	}
	fptr->name = names;
	strncpy(names, a,i);
	names += i;
	*names++ = '\0';
	if(!bstash){
		bstash = nstash = malloc(size);
		estash = bstash + size;
	}
	fptr->stash = nstash;
	while(*ap != '{')
		if(*ap == '\n'){
			if((ap=fgets(aa,256,fd))==NULL){
				fprintf(stderr,"unexpected end of file\n");
				exit(1);
			}
		}
		else ap++;
	while((j=getc(fd))!= EOF){
		if(j == '{')curly++;
		else if(j == '}'){
			if(curly == 0)break;
			else curly--;
		}
		*nstash++ = j;
		if(nstash == estash){
			free(bstash);
			size += 1024;
			bstash = realloc(bstash,size);
			estash = bstash+size;
		}
	}
	*nstash++ = '\0';
	if(fptr++ >= &flibr[MAXL]){
		fprintf(stderr,"Too many objects\n");
		exit(1);
	}
}
call(a)
char *a;
{
	char *ap;
	struct fcall *f;
	char sav;
	ap = a;
	while(isalpha(*ap))ap++;
	sav = *ap;
	*ap = '\0';
	for(f=flibr;f<fptr;f++){
		if (!(strcmp(a, f->name)))
			break;
	}
	if(f == fptr){
		fprintf(stderr, "object %s not defined\n",a);
		exit(1);
	}
	*ap = sav;
	while (isspace(*ap) || *ap == ',') 
		ap++;
	if (*ap != '\0')
		SC = atof(ap);
	else SC = 1.;
	*(++fstp) = f->stash;
	FN = 0;
}
pinclude(a)
char	*a;
{
	FILE * fd1;
	char	aa[256], *ap;
	double	x[256], *xp;
	int	n;
	if ((fd1 = fopen(a + 1, "r")) == NULL) {
		perror();
		fprintf(stderr, "Cannot find %s\n", a + 1);
		return(0);
	}
	xp = x;
	while (fgets(aa, 256, fd1) != NULL) {
		ap = aa;
		while (*ap != NULL && *ap != '\n') {
			while (isspace(*ap) || *ap == ',') 
				ap++;
			if (*ap == NULL)
				break;
			*xp++ = atof(ap);
			while (!(isspace(*ap)) && *ap != ',' && *ap != NULL) 
				ap++;
		}
	}
	fclose(fd1);
}
char *getl(a)
char a[];
{
	char *ap;
newl:
	if(FN){
		if (fgets(a, 256, fd) == NULL)
			return(0);
		if(*a == '\n')goto newl;
		return(a);
	} else	 {
		if(**fstp == '\0'){
			fstp--;
			if(fstp == fstack){
				SC = 1.;
				FN = 1;
			}
			goto newl;
		}
		ap = *fstp;
		*fstp = strchr(*fstp,'\n') +1;
		return(ap);
	}
}
spline(n1, f1)
int n1[];
double *f1[];
{
	splin(0,n1,f1);
}
fspline(n1,f1)
int n1[];
double *f1[];
{
	splin(1,n1,f1);
}
lspline(n1,f1)
int n1[];
double *f1[];
{
	splin(2,n1,f1);
}
dspline(n1,f1)
int n1[];
double *f1[];
{
	splin(3,n1,f1);
}
cspline(n1,f1)
int n1[];
double *f1[];
{
	splin(4,n1,f1);
}
