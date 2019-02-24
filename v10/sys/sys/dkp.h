struct dkp {
	struct	queue	*rdq;	/* associated read queue */
	struct	block	*inp;	/* msg being collected */
	struct	block	*inpe;	/*  end of msg */
	short	state;		/* flags */
	unsigned char trx;	/* # bytes in trailer being collected */
	unsigned char lastctl;	/* last non-URP control received */
	short	indata;		/* # bytes in message being collected */
	unsigned char iseq;	/* last good input sequence number */
	unsigned char lastecho;	/* last echo/rej sent */
	char	WS;		/* first non-consumed message */
	char	WACK;		/* first non-acknowledged message */
	char	WNX;		/* next message to be sent */
	unsigned char XW;	/* size of xmit window */
	unsigned char timer;	/* timeout for xmit */
	unsigned char outcnt;	/* count output chars for char mode */
	unsigned char trbuf[3];	/* trailer being collected */
	short	xsize;		/* size of each message */
	struct	block *xb[8];	/* the xmit window buffer */
};
