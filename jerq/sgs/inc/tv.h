/*
 *	static char *ID_tvh = "@(#) tv.h: 1.1 1/8/82";
 */

struct tventry {
	long	tv_addr;
	};

#define TVENTRY struct tventry
#define TVENTSZ sizeof(TVENTRY)
#define N3BTVSIZE	0x20000		/* size of 1 segment (128K) */
