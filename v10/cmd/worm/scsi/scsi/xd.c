#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"

#define	WIDTH	32

void
xd(uchar *p, int n, FILE *fp)
{
	register i, nd, l;
	unsigned char buf[WIDTH];
	int didstar;
	unsigned char *s;

	for(nd = 0; n > 0; n -= l, nd += l){
		l = min(WIDTH, n);
		if(nd && (l == WIDTH) && (memcmp(buf, p, l) == 0)){
			p += WIDTH;
			if(didstar++ == 0)
				fprintf(fp, "*\n");
			continue;
		}
		memcpy(buf, p, l);
		didstar = 0;
		fprintf(fp, "%5.5d", nd);
		s = p;
		for(i = 0; i < l; i++){
			if((i%4) == 0) putc(' ', fp);
			fprintf(fp, "%2.2x", *p++);
		}
		putc('\n', fp);
		fprintf(fp, "     ");
		for(i = 0; i < l; i++){
			if((i%4) == 0) putc(' ', fp);
			if((*s >= ' ') && (*s < 0177))
				fprintf(fp, " %c", *s++);
			else switch(*s++)
			{
			case '\n':	fprintf(fp, "\\n"); break;
			case '\t':	fprintf(fp, "\\t"); break;
			default:	fprintf(fp, ".."); break;
			}
		}
		putc('\n', fp);
	}
	fprintf(fp, "%5.5d\n", nd);
}
