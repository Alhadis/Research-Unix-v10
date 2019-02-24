#include <jerq.h>
#include <font.h>

jstrwidth(s)
	char *s;
{
	return(strwidth(&defont,s));
}

strwidth(f,s)
	Font *f;
	register unsigned char *s;
{
	register wid=0;
	register n = f->n;
	register Fontchar *info;
	info = f->info;
	for(; *s; s++)
		if(*s < n) wid+=info[*s].width;
	return(wid);
}
