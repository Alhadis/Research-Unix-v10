/*
 * Metheus library macros
 */
#define	NMBUF	65536
char mbuf[NMBUF], *mbufp;
int mfd;
char *omdev="/dev/null";
char *getenv();
#define	msnare() \
	if((mfd=open(getenv("FB")?getenv("FB"):omdev, 2))<0) \
		perror(getenv("FB")?getenv("FB"):omdev), exit(1); \
	else write(mfd, mbuf, NMBUF), mbufp=mbuf
#define	mlong(a)	(mword((a)>>16), mword(a))
#define	mword(a)	(mbyte((a)>>8), mbyte(a))
#define	mbyte(a)	(mbufp==&mbuf[NMBUF]?(mflush(),*mbufp++=a):(*mbufp++=a))
/* versions of the above allowing moving checks out of the loop */
#define	mcheck(n)	(mbufp>=&mbuf[NMBUF-n]?mflush():0)
#define	mulong(a)	(muword((a)>>16), muword(a))
#define	muword(a)	(mubyte((a)>>8), mubyte(a))
#define	mubyte(a)	(*mbufp++=a)
#define	mcmd(a)		((mbufp-mbuf)&1?(mbyte(0),mword(a)):mword(a))
#define	mflush()	(((mbufp-mbuf)&1?(*mbufp++=0):0),write(mfd, mbuf, (mbufp-mbuf)*sizeof mbuf[0]),mbufp=mbuf)
/*
 * Metheus Omega 3610 (and other 3000 series) opcode manifests
 */
			/* arguments w: word, b: byte, l:long*/
#define	AFILL1	0x0068	/* */
#define	AFILL2	0x0069	/* w:edge value */
#define	AFILL3	0x0074	/*  */
#define	ARC	0x0062	/* w:length */
#define	BLANK	0x004b	/* b:mode */
#define	CARC	0x2000	/* w:cx, w:cy, w:sx, w:sy, w:angle */
#define	CHARS	0x006b	/* b*:character, b:esc */
#define	CIRCLE	0x2001	/* w:cx, w:cy, w:radius */
#define	CLEAR	0x0060	/*  */
#define	CMACT	0x3000	/*  */
#define	CMAP	0x0051	/* b:index, b:red, b:grn, b:blu */
#define	CMAPOL	0x301a	/* 8b:2 nibble-sized entries */
#define	CMBLNK	0x3019	/* w:count, ... */
#define	CMCOPY	0x3016	/* b:source, b:dest, w:srcindex, w:dstindex, w:count */
#define	CMFILL	0x3017	/* w:index, w:count, b:red, b:grn, b:blu */
#define	CMLDM	0x3013	/* w:index, w:count, b:red, b:grn, b:blu, ... */
#define	CMLOAD	0x3012	/* w:index, b:red, b:grn, b:blu */
#define	CMRAMP	0x3018	/* w:index, w:count, b:r0, b:g0, b:b0, b:r1, b:g1, b:b1 */
#define	CMSEL	0x3015	/* b:buff */
#define	CMWREN	0x3011	/* b:mask */
#define	COMPDR	0x0072	/*  */
#define	CSPACE	0x0048	/* b:dx, b:dy */
#define	CURS	0x0071	/*  */
#define	DFAULT	0x003f	/* b:reset */
#define	DRAW	0x0061	/*  */
#define	DRAWAL	0x3052	/*  */
#define	DRAWTA	0x0078	/* w:x, w:y */
#define	DRAWTL	0x0077	/* w:x, w:y */
#define	DRAWTS	0x0076	/* b:x, b:y */
#define	DRAWV	0x3050	/*  */
#define	DRRCUR	0x200c	/* b:index */
#define	FFILL	0x0065	/*  */
#define	FNTINX	0x3040	/* b:type */
#define	INI	0x005e	/*  */
#define	INQ	0x0041	/* w:parameter */
#define	LDFNTX	0x3041	/* b:type, b:ESC, w:0, w:0, b:height, b:width, ... */
#define	LDPATX	0x3042	/* w:pattern, ... */
#define	LDRCUR	0x3045	/* b:index, ... */
#define	MINVEC	0x3051	/* w:length */
#define	MODEB	0x007e	/*  */
#define	MODEW	0x007f	/*  */
#define	MOVP1	0x0052	/* w:x, w:y */
#define	MOVP2	0x0053	/* w:x, w:y */
#define	NOP	0x0000	/*  */
#define	PATRNX	0x3043	/* w:pattern */
#define	PIXBLT	0x0070	/* w:width, w:height, b:pattern */
#define	POLYC	0x0044	/*  */
#define	POLYD	0x2200		/* + count */	/* w:x, w:y, ... */
#define	POLYF	0x0067	/*  */
#define	POLYLN	0x2100		/* + count */	/* w:x, w:y, ... */
#define	POLYM	0x0045	/* w:x, w:y */
#define	POLYO	0x0066	/*  */
#define	POLYS	0x0056	/*  */
#define	POLYV	0x0057	/* w:x, w:y */
#define	PPAN	0x005b	/*  */
#define	RASTOP	0x0043	/* b:function, b:alu */
#define	RDCONF	0x005d	/*  */
#define	RDMSK	0x004c	/* b:mask */
#define	RDMSKL	0x3025	/* l:mask */
#define	RDMSKS	0x3024	/* w:mask */
#define	RDR	0x006e	/*  */
#define	RDRS	0x3035	/*  */
#define	RECT1	0x0063	/*  */
#define	RECT2	0x0064	/*  */
#define	RESTOR	0x0037	/*  */
#define	RLFILL	0x006a	/* w:runlength */
#define	RMOVP1	0x0054	/* w:x, w:y */
#define	RMOVP2	0x0055	/* w:x, w:y */
#define	RMVP1S	0x0033	/* b:x, b:y */
#define	RMVP2S	0x0034	/* b:x, b:y */
#define	RPIXEL	0x006c	/*  */
#define	SAVE	0x0036	/*  */
#define	SELRES	0x301c	/* b:resolution */
#define	SETAL	0x3053	/* w:level */
#define	SETCLL	0x3061	/* l:color */
#define	SETCLS	0x3060	/* w:color */
#define	SETCOL	0x004e	/* b:color */
#define	SETCSZ	0x0058	/* b:width, b:height */
#define	SETORN	0x0059	/* b:direction */
#define	SETROC	0x0042	/* b:cmd, b:data, ..., b:end */
#define	SIGRD	0x005c	/*  */
#define	SWSCEN	0x301b	/* w:mode */
#define	SYNCH	0x005f	/* b:wait count */
#define	SZCURS	0x0047	/* w:width, w:height */
#define	WPIXEL	0x006d	/*  */
#define	WRBANK	0x003d	/* b:bank */
#define	WRMASK	0x004f	/* b:mask */
#define	WRMSKL	0x3023	/* l:mask */
#define	WRMSKS	0x3022	/* w:mask */
#define	WRR	0x006f	/* b:data, ... */
#define	WRRS	0x3032	/* w:data, ... */
#define	XDRAW	0x0073	/*  */
#define	ZOOM	0x005a	/* b:scale */
