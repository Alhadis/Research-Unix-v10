/* machine dependent stuff for core files */
#define TXTRNDSIZ 1024L
#define stacktop(siz) (0x80000000L-8*0x200)
#define stackbas(siz) (0x80000000L-8*0x200-siz)
#define	FPOFFSET		07660
#define	APOFFSET		07654
#define	DATAOFFSET	01140
