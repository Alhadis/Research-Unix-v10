/*
**	Configuration data required by packet protocol driver
*/

typedef unsigned char	uchar;

struct Pconfig
{
	int	(*xfuncp)();		/* Function to be called for output */
	int	(*rfuncp)();		/* Function to be called for input */
	void	(*rcfuncp)();		/* Function to be called for control input */
	short	xfdesc;			/* File descriptor for output */
	short	xtimo;			/* Transmit timeout in tscan units */
	uchar	rtimo;			/* Receive timeout in tscan units */
	uchar	cdata;			/* Control information for reply packet */
	uchar	tscan;			/* Timeout scan rate */
	uchar	tflag;			/* Timeout running flag */
};

extern struct Pconfig	pconfig;

#define	MAXTIMORETRYS	1		/* Maximum packets retransmitted per timeout */
#define	Pscanrate	pconfig.tscan
#define	Prtimeout	pconfig.rtimo
#define	Pxtimeout	pconfig.xtimo
#define	Pxfdesc		pconfig.xfdesc
#ifndef	Blit
#define	Pxfunc(p, n)	(*pconfig.xfuncp)(pconfig.xfdesc, (char *)(p), (n))
#else
#define	Pxfunc(p, n)	(*pconfig.xfuncp)((char *)(p), (n))
#endif
#define	Prfuncp		pconfig.rfuncp
#define	Prcfuncp	pconfig.rcfuncp
#define	Pcdata		pconfig.cdata
#define	Ptflag		pconfig.tflag

/*
**	Default timeout settings for 1200 baud
*/

#define	PSCANRATE	1		/* Scan once a second */
#define	PRTIMEOUT	2		/* Packet should complete arrival in less than 1 second */
#define	PXTIMEOUT	9		/* Time to transmit 16 full sized packets */
