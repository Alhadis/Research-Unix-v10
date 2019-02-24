#include	"../scsi.h"

void
fixedstr(uchar *src, int len, char *dest)
{
	uchar *s;

	while((*src == ' ') && (len > 0))
		src++, len--;
	for(s = src+len-1; s >= src; s--)
		if(*s != ' ')
			break;
	memcpy(dest, (char *)src, len = s-src+1);
	dest[len] = 0;
}
