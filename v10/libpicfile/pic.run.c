/*
 * Type-specific code for TYPE=runcode
 */
#include <picfile.h>
#include <libc.h>
#define	NBUF	4096	/* size of input buffer */
int PRD_run(f, buf)
PICFILE *f;
char *buf;
{
	register char *bufp, *runp, *erunp;
	int i, j, n, slack;
	if(f->line==0){
		f->buf=malloc(NBUF);
		if(f->buf==0){
			PIC_error="Can't allocate buffer";
			return 0;
		}
		f->ebuf=f->bufp=f->buf;
	}
	if(f->line==f->height){
		PIC_error="Read past end of picture";
		return 0;
	}
	bufp=buf;
	for(i=0;i!=f->width;){
		while(f->ebuf-f->bufp<f->nchan+1){	/* read until buffer has complete runs */
			slack=f->ebuf-f->bufp;
			runp=f->buf;
			while(f->bufp!=f->ebuf)
				*runp++ = *f->bufp++;
			n=read(f->fd, runp, NBUF-slack);
			if(n<=0){
				if(n==0) PIC_error="EOF reading picture";
				return 0;
			}
			f->ebuf=runp+n;
			f->bufp=f->buf;
		}
		n=*f->bufp++&255;
		i+=n+1;
		if(i>f->width){
			PIC_error="run spans scan lines";
			return 0;
		}
		runp=bufp;
		for(j=0;j!=f->nchan;j++)
			*bufp++=*f->bufp++;
		erunp=bufp+n*f->nchan;
		while(bufp!=erunp)
			*bufp++ = *runp++;
	}
	f->line++;
	return 1;
}
int PWR_run(f, buf)
PICFILE *f;
char *buf;
{
	register char *last, *runp, *pix, *next;
	int nrun, nchan, i;
	if(f->line==0){
		f->buf=malloc(f->width*(f->nchan+1));	/* big enough for any scan line */
		if(f->buf==0){
			PIC_error="Can't allocate buffer";
			return 0;
		}
		PWR_header(f);
	}
	if(f->line>=f->height){
		PIC_error="Write past end of picture";
		return 0;
	}
	f->line++;
	nchan=f->nchan;
	last=buf+nchan*(f->width-1);
	runp=f->buf;
	while(buf<=last){
		pix=buf;
		next=buf+nchan;
		while(next<=last && *pix==*next++)
			pix++;
		nrun=(pix-buf)/nchan;
		pix=buf;
		buf+=nrun*nchan+nchan;
		while(nrun>=255){
			*runp++=255;
			i=nchan;
			do
				*runp++=*pix++;
			while(--i>0);
			pix-=nchan;
			nrun-=256;
		}
		if(nrun>=0){
			*runp++=nrun;
			i=nchan;
			do
				*runp++=*pix++;
			while(--i>0);
		}
	}
	return write(f->fd, f->buf, runp-f->buf)==runp-f->buf;
}
int PCL_run(f)
PICFILE *f;
{
	free(f->buf);
}
