/*
 * Type-specific code for TYPE=dump
 */
#include <picfile.h>
int PRD_dmp(f, buf)
PICFILE *f;
char *buf;
{
	f->line++;
	return read(f->fd, buf, f->width*f->nchan)==f->width*f->nchan;
}
int PWR_dmp(f, buf)
PICFILE *f;
char *buf;
{
	if(f->line==0) PWR_header(f);
	f->line++;
	return write(f->fd, buf, f->width*f->nchan)==f->width*f->nchan;
}
int PCL_dmp(f)
PICFILE *f;
{
}
