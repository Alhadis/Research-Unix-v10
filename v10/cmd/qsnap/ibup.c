/*
 * This software is provided solely for use with
 * the National Instruments GPIB11-series interfaces.
 *
 * Copyright 1980, 1983 National Instruments
 *
 * Jeffrey Kodosky
 * REV D:  10/09/83
 */


#define DCL	024
#define GET	010
#define GTL	001
#define LLO	021
#define PPC	005
#define PPD	0160
#define PPE0	0140
#define PPU	025
#define SDC	004
#define SPD	031
#define SPE	030
#define TCT	011
#define UNL	077
#define UNT	0137

#define OK	1
#define ENOTSAC (-4)
#define ENOFUN	(-20)
#define ENODEV	(-21)
#define ENOLAD	(-22)
#define ENOTAD	(-23)

#define CO	0
#define WT	1
#define RD	2
#define TR	3
#define CL	4
#define RM	5
#define LO	6
#define PP	7
#define PC	8
#define SS	9
#define MN	10
#define RC	11
#define SP	12
#define TS	13
#define FI	14


#define Try(f)	if((x=(f))<0) return x

static int open= 0;

struct device {
	char	tad, lad, sad;	/* GPIB addresses */
	char	rmd, eod, wmd;	/* read write modes */
	};

struct device devtbl[]= {
	{ 0125,	065,	0,	0,	0,	0 },	/* GPIB interface */
	{ 0103,	043,	0143,	0,	0,	0 },	/* QCR camera */
	{ 0102,	042,	0142,	0,	0,	0 },	/* QCR camera data */
	{ 0 },						/* empty slot */
	{ 0 },						/* empty slot */
	{ 0 }						/* empty slot */
	};

#define NDEV	((sizeof devtbl)/(sizeof devtbl[0]) -1)

static char cbf[7+2*NDEV];


ibup(f,dev,a2,a3,a4,a5,a6,a7){
	register int x, d;
	register char *s;
	char *ltns();

	if(!open) Try(init());
	d= dev;
	s= &cbf[0];
	switch(f){
		default: return ENOFUN;
		case 0: if(d<=0 || d>NDEV) return ENODEV;	/* write */
			*s++= UNL; *s++= UNT;
			*s++= devtbl[0].tad;
			if(*s= devtbl[0].sad) s++;
			if((s= ltns(s,d))==0) return ENOLAD;
			Try(cmd(s));
			Try(d=gpib(WT,a2,a3,devtbl[d].wmd));
			Try(gtsun());
			return d;
		case 1: if(d<=0 || d>NDEV) return ENODEV;	/* read */
			*s++= UNL; *s++= UNT;
			if((*s++= devtbl[d].tad)==0) return ENOTAD;
			if(*s= devtbl[d].sad) s++;
			*s++= devtbl[0].lad;
			if(*s= devtbl[0].sad) s++;
			Try(cmd(s));
			Try(d=gpib(RD,a2,a3,devtbl[d].rmd,devtbl[d].eod));
			Try(gtsun());
			return d;
		case 2: if(d<0) return gpib(CL);		/* clear */
			else if(d>NDEV) return ENODEV;
			if(d){	*s++= UNL; *s++= UNT;
				if((s= ltns(s,d))==0) return ENOLAD;
				*s++= SDC; }
			else *s++= DCL;
			return cmd(s);
		case 3: if(d<0 || d>NDEV) return ENODEV;	/* trigger */
			*s++= UNL; *s++= UNT;
			if((s= ltns(s,d))==0) return ENOLAD;
			*s++= GET;
			return cmd(s);
		case 4: Try(gpib(RM));				/* remote */
			if(d<0){ *s++= LLO; d= 0; }
			else if(d>NDEV) return ENODEV;
			if((s= ltns(s,d))==0) return ENOLAD;
			return cmd(s);
		case 5: if(d<0) return gpib(LO);		/* local */
			if(d>NDEV) return ENODEV;
			*s++= UNL; *s++= UNT;
			if((s= ltns(s,d))==0) return ENOLAD;
			*s++= GTL;
			return cmd(s);
		case 6: if(d<0) return gpib(TS);		/* poll */
			if(d>NDEV) return ENODEV;
			if(d==0) return gpib(PP);
			*s++= UNL; *s++= UNT;
			*s++= SPE;
			if((*s++= devtbl[d].tad)==0) return ENOTAD;
			if(*s= devtbl[d].sad) s++;
			*s++= devtbl[0].lad;
			if(*s= devtbl[0].sad) s++;
			Try(cmd(s));
			Try(gpib(RD, cbf, 1,0,0));
			d= cbf[0]&0377;
			cbf[0]= SPD;
			Try(cmd(&cbf[1]));
			return d;
		case 7: if(d<0 || d>NDEV) return ENODEV;	/* configure */
			if(d){	*s++= UNL; *s++= UNT;
				if((s= ltns(s,d))==0) return ENOLAD;
				*s++= PPC;
				if(a2>=0) *s++= PPE0 | (a3-1)&7 | (a2? 010:0);
				else	  *s++= PPD;
				}
			else *s++= PPU;
			return cmd(s);
		case 8: if(d<=0 || d>NDEV) return ENODEV;	/* passctrl */
			*s++= UNL; *s++= UNT;
			if((*s++= devtbl[d].tad)==0) return ENOTAD;
			if(*s= devtbl[d].sad) s++;
			*s++= TCT;
			Try(cmd(s));
			return gpib(PC);
		case 9: if(d<0 || d>NDEV) return ENODEV;	/* define */
			if(a2 && (a2<0100 || a2>=0137)) return ENOTAD;
			if(a3 && (a3<040 || a3>=077)) return ENOLAD;
			if(a4 && (a4<0140 || a4>=0177)) return ENOTAD;
			devtbl[d].tad= a2;
			devtbl[d].lad= a3;
			devtbl[d].sad= a4;
			devtbl[d].rmd= a5;
			devtbl[d].eod= a6;
			devtbl[d].wmd= a7;
			return OK;
		case 10:					/* finish */
			open= 0;
			return gpib(FI);
	}	}

static char *ltns(s,d) char *s;{
	register int n;
	if(d){	if((*s++= devtbl[d].lad)==0)
			return 0;
		if(*s= devtbl[d].sad) s++;
		}
	else for(d++, n=NDEV; n-->0; d++)
		if(*s= devtbl[d].lad){
			s++;
			if(*s= devtbl[d].sad) s++;
			}
	return s; }

static cmd(s) char *s;{
	return gpib(CO, &cbf[0], s- &cbf[0]); }

static init(){
	register int x;
	if((x=gpib(CL))>=0)
		x= gpib(RM);
	if(x==ENOTSAC) x= OK;
	open++;
	return x; }

static gtsun(){
	register int x;
	register char *s;

	s= &cbf[0];
	*s++= UNL; *s++= UNT;
	Try(cmd(s));
	return gpib(TR); }
