/*
 * Type-specific code for TYPE=ccir601
 */
#include <picfile.h>
#include <libc.h>
int PRD_601(f, buf)
PICFILE *f;
char *buf;
{
	if(f->line==0){
		f->buf=malloc(f->width*2);
		if(f->buf==0){
			PIC_error="Can't allocate buffer";
			return 0;
		}
	}
	if(f->line==f->height){
		PIC_error="Read past end of picture";
		return 0;
	}
	/* read the file */
	f->line++;
	return 1;
}
int PWR_601(f, buf)
PICFILE *f;
char *buf;
{
	if(f->line==0){
		f->buf=malloc(f->width*2);
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
	/* write the file */
	f->line++;
	return 1;
}
int PCL_601(f)
PICFILE *f;
{
	free(f->buf);
}
