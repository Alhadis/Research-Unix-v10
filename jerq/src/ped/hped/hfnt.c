#include "hped.h"

#include <sys/types.h>
#include <sys/stat.h>

/* this assumes MXFONT is 5 */
/* it better be the same function on both the host and the blit */

short mapfont(k){
	if(k<9) return(0);
	if(k<12) return(1);
	if(k<16) return(2);
	if(k<18) return(3);
	return(4);
}



extern char flocus[];

#define SPACIOUS
#ifdef SPACIOUS
char * fr_tab[] = { ".10", ".12", ".14", ".16" , ".18" };
#else
char * fr_tab[] = { ".8", ".10", ".12", ".14" , ".18" };
#endif

loadfont(c)
	short c;
{
	char font_file[64];
	struct stat sizeptr;
	FILE *fnt;
	short ok;

	if(c<0) c = 0;	if(c>=MXFONT) c = MXFONT-1;
	if(fontsent[c]){
		s_char('Y');	/* font already sent */
		s_short(c);
		rrecord("font already there %d",c);
		ok = 0;
	}
	else {
		strcpy(font_file,flocus);
		strcat(font_file,fr_tab[c]);
		rrecord(font_file);
		if(!(fnt = fopen(font_file,"r"))) {
			s_char('N');
			rrecord("font not found");
			ok = 0;
		}
		else {
			s_char('F'); /* Send font file to blit */
			s_short(c);
			if(stat(font_file,&sizeptr)) sizeptr.st_size = 0;
			rrecord("size = %d",sizeptr.st_size);
			s_short((short) sizeptr.st_size);
			while(--(sizeptr.st_size) >= 0)
				s_char(fgetc(fnt));
			fontsent[c] = 1;
			fclose(fnt);
			rrecord("\tsent to %d loc at blit",c);
			ok = 1;
		}
	}
	fflush(toblit);
	return(ok);
}

extern short needfnt[];

fmark(arr,narr)
	objP arr[];
	short narr;
{
	register i;
	register objP a;
	short t;
	for(i=0;i<narr;i++) {
		a = arr[i];
		switch(a->type){
		case 'f': fmark(a->mem,(short)a->size);
			  break;
		case 't':
			t = scale*a->size+0.5;
			t = mapfont(t);
			if(t<0) t = 0;
			if(t>4) t = 4;
			if(!fontsent[t]) needfnt[t] = 1;
			break;
		}
	}
}
