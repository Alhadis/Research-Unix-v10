/*
 * static char ID_codouth[] = "@(#) codeout.h: 1.5 11/11/83";
 */

#if ONEPROC
#define TBUFSIZE 512
#else
#define TBUFSIZE 10
#endif

typedef struct {
	long cvalue;
#if VAX
	unsigned char caction;
	unsigned char cnbits;
	int cindex;
#else
	unsigned short caction;
	unsigned short cindex;
	unsigned short cnbits;
#endif
} codebuf;

typedef struct symentag {
	long stindex;
	long fcnlen;
	long fwdindex;
	struct symentag *stnext;
} stent;

typedef struct {
	long relval;
	char *relname;
	short reltype;
} prelent;
