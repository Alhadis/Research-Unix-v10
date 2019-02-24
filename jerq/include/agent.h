#define	A_NEWLAYER	1	/* make a new layer */
#define	A_CURRENT	2	/* make layer process current */
#define	A_DELETE	3	/* delete a layer */
#define	A_TOP		4	/* bring a layer to top */
#define	A_BOTTOM	5	/* put a layer on bottom */
#define	A_MOVE		6	/* move a layer */
#define	A_RESHAPE	7	/* reshape a layer */
#define	A_NEW		8	/* make a new layer and send C_NEW to layers */
#define	A_EXIT		9	/* exit layers program */

struct agentrect{
	short	command;	/* either newlayer, reshape or current */
	short	chan;
	Rectangle r;		/* rectangle description */
};
