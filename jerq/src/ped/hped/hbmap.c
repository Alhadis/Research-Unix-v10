
/*	get byte stream from blit and place in BLITBLT		*/

#include "hped.h"

getbmap()
{
	short bn, h, w;
	register i, j, k, y;
	if((fd1=fopen("BLITBLT","w"))==NULL) error(1,"No BLITBLT");
	h = g_short();
	w = g_short();
	rrecord("h=%d w=%d",h,w);
	putc(h&0377,fd1);	putc((h>>8)&0377,fd1);
	putc(w&0377,fd1);	putc((w>>8)&0377,fd1);
	for(y=0;y<h;y++){
		j = 0;
		do{
			putc((bn = g_char()&0377),fd1);
			if(bn>127) {
				bn &= 0177;
				for(i=0;i<2;i++) putc(g_char(),fd1);
				j += bn;
			}
			else {
				for(k=0;k<bn;k++) {
					for(i=0;i<2;i++) putc(g_char(),fd1);
					j++;
				}
			}
		} while(j<w);
	}
	fclose(fd1);
}
