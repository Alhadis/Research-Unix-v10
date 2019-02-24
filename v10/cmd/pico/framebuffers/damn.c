#include <stdio.h>
#include <fb.h>
#include "pico.h"

extern struct	SRC src[MANY];
extern short	CUROLD, CURSCRATCH;
extern int	DEF_LL, DEF_NL;

gettd(into)
	struct SRC *into;
{
	unsigned char *r = into->pixred + (into->sy)*DEF_LL + (into->sx);
	unsigned char *g = into->pixgrn + (into->sy)*DEF_LL + (into->sx);
	unsigned char *b = into->pixblu + (into->sy)*DEF_LL + (into->sx);
	PICFILE *tmp = picopen(into->fname);
	int nc = into->nchan;
	int  w = into->ex - into->sx;
	int  d = into->ey - into->sy;
	unsigned char *z = (unsigned char *) Emalloc(w*nc*sizeof(unsigned char));
	int i;

	for (i = 0; i < d; i++)
	{	if (readpic(tmp, z) == -1)
		{	fprintf(stderr, "unexpected eof\n");
			break;
		}
		if (i < into->oy) continue;
		unwind(z+nc*into->ox, r, g, b, w, nc);
		r += DEF_LL; g += DEF_LL; b += DEF_LL;
	}
	closepic(tmp);
	free(z);
}

unwind(p, R, G, B, w, nc)
	unsigned char *p, *R, *G, *B;
{
	register unsigned char *r = R, *g = G, *b = B;
	register unsigned char *z = p;
	register int i;

	switch (nc) {
	default: whoops("unwind: bad number of channels");
	case 4: for(i=0; i<w; i++){ *r++=*z++; *g++=*z++; *b++=*z++; z++; } break;
	case 3: for(i=0; i<w; i++){ *r++=*z++; *g++=*z++; *b++=*z++; 	  } break;
	case 2: for(i=0; i<w; i++){ *r++=*z++; *g++=*z++; 		  } break;
	case 1: for(i=0; i<w; i++){ *r++=*z++;				  } break;
	}
}

puttd(fd, x, y, w, d)
{
	register unsigned char *r = Old->pixred + y*DEF_LL + x;
	register unsigned char *g = Old->pixgrn + y*DEF_LL + x;
	register unsigned char *b = Old->pixblu + y*DEF_LL + x;
	register int i, j, nc = Old->nchan;
	char *zz, *tmp = (char *) Emalloc(w*nc*sizeof(char));

	for (i = 0; i < d; i++)
	{	switch (nc) {
		default: for (j = 0, zz = tmp; j < w; j++)
			 {	*zz++ = *r++, *zz++ = *g++, *zz++ = *b++;
				zz += nc - 3;
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

		if (!ewrite(fd, tmp, w*nc))
			break;

		r += DEF_LL-w, g += DEF_LL-w, b += DEF_LL-w;
	}
	free(tmp);
}

tdheader(fd, n, a, b, c, d)
{	char line[128];

	sprintf(line, "TYPE=dump\nWINDOW=%d %d %d %d\nNCHAN=%d\n\n", a,b,c,d,n);
	write(fd, line, strlen(line));
}
