/*
 * one dkmodule per hardware interface to datakit
 * a given hardware device == (major device, range of minor devices)
 * channel 0 == (dev, lo)
 * there are hi-lo channels
 */

struct	dkmodule {
	char	*dkstate;		/* open/closed status of channels */
	struct	queue *listnrq;		/* channel to controller */
	short	dev;			/* major device of datakit interface */
	short	lo, hi;			/* range of devs on this controller */
	short	type;			/* type of listener */
};

#ifdef KERNEL
struct dkmodule *dkmodall(), *getdkmod();
#endif

/*
 * channel states
 */
#define	DKCLOSED 0
#define	DKRCLOSE 1		/* remote hung up, local still around */
#define	DKLCLOSE 2		/* closed locally, CMC hasn't acked yet */
#define	DKOPEN	 3		/* in use */

/*
 * listener types
 */
#define	CMCLD	'c'
#define	UNIXPLD	'u'

/*
 * M_PRICTL messages contain
 * one byte of subtype
 * perhaps another byte of channel number
 */

#define	DKMCLOSE	0	/* this channel is closing */
#define	DKMXINIT	01	/* re-init URP because of splice */
