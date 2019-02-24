/*
 * Type-specific code for TYPE=pico
 * Not a standard type, but supported for Gerard.
 * Doesn't work on pipes because it calls lseek.
 */
#include <picfile.h>
#include <libc.h>
int PRD_pco(f, buf)
PICFILE *f;
char *buf;
{
	register char *bufp, *ein, *inp;
	int i, n;
	long offs;
	if(f->line==0){
		f->buf=malloc(f->width);
		if(f->buf==0){
			PIC_error="Can't allocate buffer";
			return 0;
		}
	}
	if(f->line==f->height){
		PIC_error="Read past end of picture";
		return 0;
	}
	offs=lseek(f->fd, 0L, 1);
	ein=f->buf+f->width;
	for(i=0;i!=f->nchan;i++){
		n=read(f->fd, f->buf, f->width);
		if(n!=f->width){
			if(n==0) PIC_error="End of file reading picture";
			return 0;
		}
		inp=f->buf;
		for(bufp=buf+i;inp!=ein;bufp+=f->nchan)
			*bufp=*inp++;
		lseek(f->fd, (long)f->width*(f->height-1), 1);
	}
	lseek(f->fd, offs+f->width, 0);
	f->line++;
	return 1;
}
int PWR_pco(f, buf)
PICFILE *f;
char *buf;
{
	register char *bufp, *eout, *outp;
	int i, n;
	long offs;
	if(f->line==0){
		f->buf=malloc(f->width);
		if(f->buf==0){
			PIC_error="Can't allocate buffer";
			return 0;
		}
		PWR_header(f);
	}
	if(f->line==f->height){
		PIC_error="Write past end of picture";
		return 0;
	}
	offs=lseek(f->fd, 0L, 1);
	eout=f->buf+f->width;
	for(i=0;i!=f->nchan;i++){
		outp=f->buf;
		for(bufp=buf+i;outp!=eout;bufp+=f->nchan)
			*outp++=*bufp;
		n=write(f->fd, f->buf, f->width);
		if(n!=f->width) return 0;
		lseek(f->fd, (long)f->width*(f->height-1), 1);
	}
	lseek(f->fd, offs+f->width, 0);
	f->line++;
	return 1;
}
int PCL_pco(f)
PICFILE *f;
{
	free(f->buf);
}
