#include	<stdio.h>
#include	<sgtty.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"piclib.h"

extern char *malloc();
static getfile(), getwindow(), gettd(), unwind(), putwindow(), puttd(), tdheader();

openf(s, image)
	char *s;
	struct pfile *image;
{
	int fd;

	strcpy(image->name, s);
	if ((fd = open(s, 0)) >= 0)
	{	fd = readheader(fd, image);
		if (image->type != RUNCODE)
			return fd;
	}

	return -1;
}

newf(image)
	struct pfile *image;
{
	int w = image->r.corner.x - image->r.origin.x;
	int h = image->r.corner.y - image->r.origin.y;

	switch (image->nchan) {
	default: image->pixblu = (unsigned char *) malloc(w*h); /* fall through */
	case  2: image->pixgrn = (unsigned char *) malloc(w*h); /* fall through */
	case  1: image->pixred = (unsigned char *) malloc(w*h); break;
	}
	if (image->pixred == NULL)
		return 0;
	switch (image->nchan) {
	default: break;
	case  1: image->pixgrn = image->pixred; /* fall through */
	case  2: image->pixblu = image->pixred; break;
	}
	return 1;
}

closef(image)
	struct pfile *image;
{
	switch (image->nchan) {
	case  3: if (image->pixblu) free(image->pixblu);
	case  2: if (image->pixgrn) free(image->pixgrn);
	case  1: if (image->pixred) free(image->pixred);
	default: break;
	}
}

dumpf(s, image)
	char *s;
	struct pfile *image;
{
	int fd, res=1;

	if ((fd = creat(s, 0666)) <= 0)
		return 0;

	switch (image->type) {
	case PICO:
		res = putfile(fd, image);
		break;
	case RUNCODE:
	case DUMP:
		res = puttd(fd, image);
		break;
	}
	close(fd);
	return res;
}

readheader(fd, files)
	struct pfile *files;
{ char line[512];
  int i, a, b, c, d, n, sawtype=0;
  extern double sqrt();
  struct sgttyb sttybuf, sttysave;
  struct stat stbuf;

	files->r.origin.x = files->r.origin.y = 0;
	files->r.corner.x = files->r.corner.y = 0;
	files->pixred = files->pixgrn = files->pixblu = NULL;
	files->nchan = 1;
	lseek(fd, 0L, 0);
	for (;;)
	{	for (i = 0; i < 127; i++)
		{	read(fd, &line[i], 1);
			if (line[i] == '\n')
			{	line[i] = '\0';
				break;
		}	}
		if (i == 0)
		{	if (sawtype)
				return fd;		/* end of header */
			i = 127;
		}
		if (i == 127)
		{	lseek(fd, 0L, 0);
			fstat(fd, &stbuf);
			n = (int) sqrt(stbuf.st_size+1.0);
			files->r.corner.x = files->r.corner.y = n;
		 	files->type = PICO;
			files->nchan = 1;
			return fd;			/* no header */
		}
		if (sscanf(line, "WINDOW=%d %d %d %d", &a, &b, &c, &d) == 4)
		{	if (a > c) i = a, a = c, c = i;		/* flip */
			if (b > d) i = b, b = d, d = i;
			if (a < 0) c -= a, a = 0;		/* normalize  */
			if (b < 0) d -= b, b = 0;
			files->r.origin.x = a; files->r.origin.y = b;
			files->r.corner.x = c; files->r.corner.y = d;
		} else if (sscanf(line, "NCHAN=%d", &a) == 1)
			files->nchan = a;
		  else if (strncmp(line, "CHAN=", 5) == 0
			&& (line[5] == 'm' || line[5] == 'r' ||
			    line[5] == 'b' || line[5] == 'g'))
			files->nchan = 1;
		  else if (strcmp(line, "TYPE=dump") == 0)
		{	files->type = DUMP; sawtype = 1;
		} else if (strcmp(line, "TYPE=runcode") == 0)
		{	files->type = RUNCODE; sawtype = 1;
		} else if (strcmp(line, "TYPE=pico") == 0)
		{	files->type = PICO; sawtype = 1;
		} else if (strcmp(line, "TYPE=binary") == 0)
		{	files->type = BINARY; sawtype = 1;
		} else if (strncmp(line, "TYPE", 4) == 0)
			return -1;		/* unknown format */
	}
}

readf(fd, image)
	struct pfile *image;
{
	int w = image->r.corner.x - image->r.origin.x;
	int h = image->r.corner.y - image->r.origin.y;

	if (w > 0 && h > 0 && (image->pixred != NULL || newf(image)))
	switch (image->type) {
		case BINARY:
		case PICO:
			return getfile(fd, image, w, h);
			break;
		case RUNCODE:
			return -1;
		case DUMP:
			return gettd(fd, image, w, h);
			break;
	}
	return 0;
}

static getfile(fd, image, w, h)
	struct pfile *image;
{
	if (!getwindow(fd, image->pixred, w, h)) return 0;
	if (image->nchan >= 2)
		if (!getwindow(fd, image->pixgrn, w, h)) return 0;
	if (image->nchan >= 3)
		if (!getwindow(fd, image->pixblu, w, h)) return 0;
	return 1;
}

static getwindow(fd, dest, w, d)
	unsigned char *dest;
{
	int i; unsigned char *z = dest;

	for (i = 0; i < d; i++, z += w)
		if (read(fd, z, w) != w)
			return 0;
	return 1;
}

static gettd(fd, image, w, d)
	struct pfile *image;
{
	unsigned char *z = (unsigned char *) malloc(w * image->nchan);
	unsigned char *r = image->pixred;
	unsigned char *g = image->pixgrn;
	unsigned char *b = image->pixblu;
	int i, res=1, j = w*image->nchan;

	if (z == NULL)
		return 0;
	for (i = 0; i < d; i++)
	{	if (read(fd, z, j) != j || !unwind(z, r, g, b, w, image->nchan))
		{	res = 0;
			break;
		}
		r += w; g += w; b += w;
	}
	free(z);
	return res;
}

static unwind(p, R, G, B, w, nchan)
	unsigned char *p, *R, *G, *B;
{
	register unsigned char *r = R, *g = G, *b = B;
	register unsigned char *z = p;
	register int i;

	switch (nchan) {
	default: return 0;	/* bad number of channels */
	case 4: for(i=0; i<w; i++){ *r++=*z++; *g++=*z++; *b++=*z++; z++; } break;
	case 3: for(i=0; i<w; i++){ *r++=*z++; *g++=*z++; *b++=*z++; 	  } break;
	case 2: for(i=0; i<w; i++){ *r++=*z++; *g++=*z++; 		  } break;
	case 1: for(i=0; i<w; i++){ *r++=*z++;				  } break;
	}
	return 1;
}

putheader(fd, image)
	struct pfile *image;
{
	int w = image->r.corner.x - image->r.origin.x;
	int d = image->r.corner.y - image->r.origin.y;

	if (image->nchan != 1 || w != d
	||  image->r.origin.x != 0
	||  image->r.origin.y != 0)
	{	if (image->type == PICO)
			picoheader(fd, image);
		else
			tdheader(fd, image);
	}
}

putfile(fd, image)
	struct pfile *image;
{
	int w = image->r.corner.x - image->r.origin.x;
	int d = image->r.corner.y - image->r.origin.y;

	if (image->nchan != 1 || w != d
	||  image->r.origin.x != 0
	||  image->r.origin.y != 0)
		picoheader(fd, image);
	if (!putwindow(fd, image->pixred, w, d))
		return 0;
	if (image->nchan >= 2)
	{	if (!putwindow(fd, image->pixgrn, w, d))
			return 0;
	}
	if (image->nchan >= 3)
		return putwindow(fd, image->pixblu, w, d);
	return 0;
}

static putwindow(fd, source, w, d)
	unsigned char *source;
{
	register unsigned char *z = source;
	register int i;

	for (i = 0; i < d; i++, z += w)
		if (write(fd, z, w) != w)
			return 0;
	return 1;
}

static puttd(fd, image)
	struct pfile *image;
{
	unsigned char *r = image->pixred;
	unsigned char *g = image->pixgrn;
	unsigned char *b = image->pixblu;
	int w = image->r.corner.x - image->r.origin.x;
	int d = image->r.corner.y - image->r.origin.y;
	int i, j, res=1;
	unsigned char *zz, *tmp = (unsigned char *) malloc(w*image->nchan);

	if (tmp == NULL)
		return 0;

	tdheader(fd, image);
	for (i = 0; i < d; i++)
	{	switch (image->nchan) {
		default: for (j = 0, zz = tmp; j < w; j++)
			 {	*zz++ = *r++, *zz++ = *g++, *zz++ = *b++;
				zz += image->nchan - 3;
			 }
			 break;
		case  3: for (j = 0, zz = tmp; j < w; j++)
			 {	*zz++ = *r++, *zz++ = *g++, *zz++ = *b++; }
			 break;
		case  2: for (j = 0, zz = tmp; j < w; j++)
			 {	*zz++ = *r++, *zz++ = *g++; }
			 break;
		case  1: for (j = 0, zz = tmp; j < w; j++) *zz++ = *r++;
			 break;
		}
		if (write(fd, tmp, w*image->nchan) != w*image->nchan)
		{	res = 0;
			break;
		}
	}
	free(tmp);
	return res;
}

static tdheader(fd, image)
	struct pfile *image;
{	char line[128];
	int n = image->nchan;
	int a = image->r.origin.x;
	int b = image->r.origin.y;
	int c = image->r.corner.x;
	int d = image->r.corner.y;

	sprintf(line, "TYPE=dump\nWINDOW=%d %d %d %d\nNCHAN=%d\n\n", a,b,c,d,n);
	write(fd, line, strlen(line));
}

picoheader(fd, image)
	struct pfile *image;
{	char line[128];
	int n = image->nchan;
	int a = image->r.origin.x;
	int b = image->r.origin.y;
	int c = image->r.corner.x;
	int d = image->r.corner.y;

	sprintf(line, "TYPE=pico\nWINDOW=%d %d %d %d\nNCHAN=%d\n\n", a,b,c,d,n);
	write(fd, line, strlen(line));
}
