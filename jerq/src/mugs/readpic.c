/*% vcc -gc %
 */
#include "fb.h"
#include <errno.h>
#include <libc.h>
#define	ARGC	512
char *malloc();
PICFILE *openpicr(file)
char *file;
{
	register PICFILE *f=(PICFILE *)malloc(sizeof(PICFILE));
	register i, argc=0, gotwindow=0, gotnchan=0, gotcmap=0;
	register char *s;
	char *strchr();
	char line[512];
	char *argv[ARGC];
	Rectangle r;
	if(f==NULL)
		return(NULL);
	f->file=NULL;
	f->argv=NULL;
	f->cmap=NULL;
	if((f->fd=fopen(file, "r"))==NULL
	|| fgets(line, sizeof line, f->fd)==NULL){
	Bad:
		for(i=0;i!=argc;i++)
			free(argv[i]);
		closepic(f);
		return(NULL);
	}
	f->file=malloc(strlen(file)+1);
	if(f->file!=NULL)
		strcpy(f->file, file);
	if(strcmp(line, "TYPE=runcode\n")==0)
		f->type=F_RUNCODE;
	else if(strcmp(line, "TYPE=dump\n")==0)
		f->type=F_DUMP;
	else{
		if(inferheader(f))
			return f;
		goto Bad;
	}
	while(fgets(line, sizeof line, f->fd)!=NULL && line[0]!='\n'){
		if(sscanf(line, "WINDOW=%d %d %d %d",
		    &r.origin.x, &r.origin.y, &r.corner.x, &r.corner.y)==4){
			if(r.corner.x<r.origin.x){
				i=r.origin.x;
				r.origin.x=r.corner.x;
				r.corner.x=i;
			}
			if(r.corner.y<r.origin.y){
				i=r.origin.y;
				r.origin.y=r.corner.y;
				r.corner.y=i;
			}
			f->r=r;
			gotwindow++;
		}
		else if(sscanf(line, "NCHAN=%d", &f->nchan)==1)
			gotnchan++;
		else if(strcmp(line, "CMAP=\n")==0)
			gotcmap++;
		else{
			if((s=strchr(line, '\n'))!=NULL)
				*s='\0';
			if(argc==ARGC || (argv[argc]=malloc(strlen(line)+1))==NULL)
				goto Bad;
			strcpy(argv[argc++], line);
			
		}
	}
	if(gotwindow!=1 || gotnchan!=1)
		goto Bad;
	if((f->argv=(char **)malloc((argc+1)*sizeof(char *)))==NULL)
		goto Bad;
	if(gotcmap){
		if((f->cmap=(unsigned char *)malloc(256*3))==NULL
		|| fread(f->cmap, 3, 256, f->fd)!=256)
			goto Bad;
	}
	for(i=0;i!=argc;i++)
		f->argv[i]=argv[i];
	return(f);
}
static struct{
	int x;
	int y;
	char *window;
}siz[]={
	512, 480, "WINDOW=0 0 512 480",		/* rs170 */
	512, 486, "WINDOW=0 0 512 486",		/* nyit */
	512, 488, "WINDOW=0 0 512 488",		/* lucasfilm */
	512, 496, "WINDOW=0 0 512 496",		/* allegra scanner? */
	512, 497, "WINDOW=0 0 512 497",		/* allegra scanner */
	512, 512, "WINDOW=0 0 512 512",		/* square */
	2048, 2047, "WINDOW=0 0 2048 2047",	/* Eikonix scanner */
	2048, 2048, "WINDOW=0 0 2048 2048",	/* Eikonix scanner */
	0, 0
};
static struct{
	int n;
	char *nchan;
	char *chan;
}dep[]={
	1,	"NCHAN=1", "CHAN=r",
	2,	"NCHAN=2", "CHAN=ra",
	3,	"NCHAN=3", "CHAN=rgb",
	4,	"NCHAN=4", "CHAN=rgba",
	7,	"NCHAN=7", "CHAN=rgbz...",
	8,	"NCHAN=8", "CHAN=rgbaz...",
	0
};
static inferheader(f)
PICFILE *f;
{
	register s, d, or=0;
	long size;
	double sqrt();
	char **argv;
	fseek(f->fd, 0L, 2);
	size=ftell(f->fd);
	fseek(f->fd, 0L, 0);
	clearerr(f->fd);
	for(s=0;siz[s].x;s++) for(d=0;dep[d].n;d++)
		if(siz[s].x*siz[s].y*dep[d].n==size){
			f->type=F_DUMP;
			f->r.or.x=f->r.or.y=0;
			f->r.co.x=siz[s].x;
			f->r.co.y=siz[s].y;
			f->nchan=dep[d].n;
			f->argv=(char **)malloc(5*sizeof(char *));
			if(f->argv!=NULL){
				f->argv[0]=strdup("TYPE=dump");
				f->argv[1]=strdup(siz[s].window);
				f->argv[2]=strdup(dep[d].nchan);
				f->argv[3]=strdup(dep[d].chan);
				f->argv[4]=0;
			}
			return 1;
		}
	return 0;
}
/*
 * skip a scan line of picture file f
 */
skippic(f)
register PICFILE *f;
{
	register wid=f->r.corner.x-f->r.origin.x;
	register i, j, cnt;
	switch(f->type){
	case F_RUNCODE:
		for(i=0;i!=wid;i+=cnt){
			if((cnt=getc(f->fd)+1)==EOF+1)
				return -1;
			for(j=0;j!=f->nchan;j++)
				getc(f->fd);
		}
		return 0;
	case F_DUMP:
		fseek(f->fd, wid*f->nchan, 1);
		return 0;
	}
}
readpic(f, p)
register PICFILE *f;
register char *p;
{
	register char *vp, *v, *ev;
	register j, i, k, cnt;
	register wid=f->r.corner.x-f->r.origin.x;
	switch(f->type){
	case F_RUNCODE:
		for(i=0;i!=wid;i+=cnt){
			cnt=getc(f->fd)+1;
			if(cnt==EOF+1)
				return(-1);
			v=p;
			for(k=0;k!=f->nchan;k++)
				*p++=getc(f->fd);
			if(i+cnt>wid){
				fprintf(stderr, "%s: scanline overflow\n", f->file);
				return(-1);
			}
			ev=p;
			for(j=1;j!=cnt;j++)
				for(vp=v;vp!=ev;*p++=*vp++);
		}
		return 0;
	case F_DUMP:
		if(fread(p, f->nchan, wid, f->fd)!=wid)
			return -1;
		return 0;
	}
}
