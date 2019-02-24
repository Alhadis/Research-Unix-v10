/*
 * munching squares
 */
#define	DELTA 13
#define	DADDR	((short *)(384L*1024L+030))
#define	DSTAT	((short *)(384L*1024L+040))
#define	MAXX 800
#define	MAXY 1024
#define	START	2000
bit[]={
	0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 0x100, 0x80, 0x40,
	0x20, 0x10, 0x8, 0x4, 0x2, 0x1
};

main(){
	register unsigned x, y;
	register long i;
	register short d;
	register short *screen=(short *)START;
	d=0;
	*DSTAT=0;
	*DADDR=START/4;
	for(i=0; i<(long)MAXY*(MAXX/16); i++)
		screen[i]=0;
	for(;;){
		for(x=0; x<MAXX; x++){
			y = (x^d) & 1023;
			if(y>=0 && y<MAXY)
				*(screen+y*(MAXX/16)+(x>>4)) ^= bit[x&0xF];
		}
		d+=DELTA;
	}
}
